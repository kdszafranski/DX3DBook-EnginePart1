#include "image.h"

Image::Image()
{
	initialized = false;            // set true when successfully initialized
	spriteData.width = 2;
	spriteData.height = 2;
	spriteData.x = 0.0;
	spriteData.y = 0.0;
	spriteData.scale = 1.0;
	spriteData.angle = 0.0;
	spriteData.rect.left = 0;       // used to select one frame from multi-frame image
	spriteData.rect.top = 0;
	spriteData.rect.right = spriteData.width;
	spriteData.rect.bottom = spriteData.height;
	spriteData.texture = NULL;      // the sprite texture (picture)
	spriteData.flipHorizontal = false;
	spriteData.flipVertical = false;
	frameCols = 1;
	textureManager = NULL;
	startFrame = 0;
	endFrame = 0;
	currentFrame = 0;
	frameDelay = 1.0;               // default to 1 second per frame of animation
	animTimer = 0.0;
	visible = true;                 // the image is visible
	loop = false;                    // loop frames
	animComplete = false;
	graphics = NULL;                // link to graphics system
	colorFilter = graphicsNS::WHITE; // WHITE for no change
}

Image::~Image() { }

//=============================================================================
// Initialize the Image.
// Post: returns true if successful, false if failed
// pointer to Graphics
// width of Image in pixels  (0 = use full texture width)
// height of Image in pixels (0 = use full texture height)
// number of columns in texture (1 to n) (0 same as 1)
// pointer to TextureManager
//=============================================================================
bool Image::initialize(Graphics* g, int width, int height, int ncols, 
						TextureManager* textureM)
{
	try {
		graphics = g;
		textureManager = textureM;
		spriteData.texture = textureManager->getTexture();
		// set width and height
		if (width == 0) {
			width = textureManager->getWidth();
		}
		spriteData.width = width;

		if (height == 0) {
			height = textureManager->getHeight();
		}
		spriteData.height = height;

		// set up columns in our texture
		// ncols 0 means default (of 1 column)
		frameCols = ncols;
		if (ncols == 0) {
			frameCols = 1;
		}

		// configure spriteData.rect to draw currentFrame
		spriteData.rect.left = (currentFrame % frameCols) * spriteData.width;
		// right edge + 1
		spriteData.rect.right = spriteData.rect.left + spriteData.width;

		spriteData.rect.top = (currentFrame / frameCols) * spriteData.height;
		// bottom edge + 1
		spriteData.rect.bottom = spriteData.rect.top + spriteData.height;
	}
	catch (...) { 
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error in Image::init"));
		return false; 
	}

	initialized = true;
	return true;
}

//=============================================================================
// Draw the image using color as filter
// The color parameter is optional, WHITE is assigned as default in image.h
// Pre : spriteBegin() is called
// Post: spriteEnd() is called
//=============================================================================
void Image::draw(COLOR_ARGB color)
{
	if (!visible || graphics == NULL)
		return;

	// get fresh texture incase onReset() was called
	spriteData.texture = textureManager->getTexture();
	
	if (color == graphicsNS::FILTER)                     // if draw with filter
		graphics->drawSprite(spriteData, colorFilter);  // use colorFilter
	else
		graphics->drawSprite(spriteData, color);        // use color as filter
}

//=============================================================================
// Draw this image using the specified SpriteData.
//   The current SpriteData.rect is used to select the texture.
// Pre : spriteBegin() is called
// Post: spriteEnd() is called
//=============================================================================
void Image::draw(SpriteData sd, COLOR_ARGB color)
{
	if (!visible || graphics == NULL)
		return;
	sd.rect = spriteData.rect;                  // use this Images rect to select texture
	sd.texture = textureManager->getTexture();  // get fresh texture incase onReset() was called

	if (color == graphicsNS::FILTER)             // if draw with filter
		graphics->drawSprite(sd, colorFilter);  // use colorFilter
	else
		graphics->drawSprite(sd, color);        // use color as filter
}

void Image::update(float frameTime) {
	if (endFrame - startFrame > 0)          // if animated sprite
	{
		animTimer += frameTime;             // total elapsed time
		if (animTimer > frameDelay)
		{
			animTimer -= frameDelay;
			currentFrame++;
			if (currentFrame < startFrame || currentFrame > endFrame)
			{
				if (loop == true)            // if looping animation
					currentFrame = startFrame;
				else                        // not looping animation
				{
					currentFrame = endFrame;
					animComplete = true;    // animation complete
				}
			}
			setRect();                      // set spriteData.rect
		}
	}
}

//=============================================================================
// Set the current frame of the image
//=============================================================================
void Image::setCurrentFrame(int c)
{
	if (c >= 0)
	{
		currentFrame = c;
		animComplete = false;
		setRect();  // set spriteData.rect
	}
}

//=============================================================================
//  Set spriteData.rect to draw currentFrame
//=============================================================================
inline void Image::setRect()
{
	// configure spriteData.rect to draw currentFrame
	spriteData.rect.left = (currentFrame % frameCols) * spriteData.width;
	// right edge + 1
	spriteData.rect.right = spriteData.rect.left + spriteData.width;
	spriteData.rect.top = (currentFrame / frameCols) * spriteData.height;
	// bottom edge + 1
	spriteData.rect.bottom = spriteData.rect.top + spriteData.height;
}
