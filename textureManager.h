#ifndef _TEXTURE_MANAGER_H
#define _TEXTURE_MANAGER_H

#include "graphics.h"
#include "constants.h"

class TextureManager
{
private:
	UINT width;
	UINT height;
	LP_TEXTURE texture;
	Graphics *graphics;
	const char *file;
	bool initialized;
	HRESULT hr;

public:
	// constructor
	TextureManager();

	// destructor
	virtual ~TextureManager();

	// Initialize the textureManager
	// Pre: *g points to Graphics object
	//      *file points to name of texture file to load
	// Post: The texture file is loaded
	virtual bool initialize(Graphics* g, const char* file);

	void onLostDevice();
	void onResetDevice();

	// inline methods
	// returns a pointer to the texture
	LP_TEXTURE getTexture() const { return texture; }
	// returns the texture width
	UINT getWidth() const { return width; }
	// returns the texture height
	UINT getHeight() const { return height; }


};

#endif // !_TEXTURE_MANAGER_H