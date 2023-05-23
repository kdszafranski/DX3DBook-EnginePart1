// Programming 2D Games
// Copyright (c) 2011 by: 
// Charles Kelly
// Game Engine Part 1
// Chapter 4 spacewar.cpp v1.0
// Spacewar is the class we create.

#include "spaceWar.h"

//=============================================================================
// Constructor
//=============================================================================
Spacewar::Spacewar()
{}

//=============================================================================
// Destructor
//=============================================================================
Spacewar::~Spacewar()
{
    releaseAll();           // call onLostDevice() for every graphics item
}

//=============================================================================
// Initializes the game
// Throws GameError on error
//=============================================================================
void Spacewar::initialize(HWND hwnd)
{
    Game::initialize(hwnd); // throws GameError

    initSprites();

    return;
}

//=============================================================================
// Initializes all the game sprites from textures
//=============================================================================
void Spacewar::initSprites() {
    // nebula texture
    if (!nebulaTexture.initialize(graphics, NEBULA_PATH))
    {
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing nebula texture"));
    }
    // nebula
    if (!nebula.initialize(graphics, 0, 0, 0, &nebulaTexture))
    {
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing nebula image"));
    }

    // planet texture
    if (!planetTexture.initialize(graphics, PLANET_PATH))
    {
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing planet texture"));
    }
    // planet
    if (!planet.initialize(graphics, 0, 0, 0, &planetTexture))
    {
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing planet image"));
    }
    // center planet, uses * 0.5 instead of / 2... performance likely?
    planet.setX(GAME_WIDTH * 0.5f - planet.getWidth() * 0.5f);
    planet.setY(GAME_HEIGHT * 0.5f - planet.getHeight() * 0.5f);

    // ship texture
    if (!shipTex.initialize(graphics, SHIP_PATH))
    {
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing ship texture"));
    }
    // ship has multiple frames, start with upper-left frame
    if (!ship.initialize(graphics, SHIP_WIDTH, SHIP_HEIGHT, SHIP_COLS, &shipTex))
    {
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing ship image"));
    }
    // start above and left of planet
    ship.setX(GAME_WIDTH / 4);
    ship.setY(GAME_HEIGHT / 4);
    // animate
    ship.setFrames(SHIP_START_FRAME, SHIP_END_FRAME);
    ship.setCurrentFrame(SHIP_START_FRAME);
    ship.setFrameDelay(SHIP_ANIM_DELAY);
    ship.setDegrees(45.0f);

}

//=============================================================================
// Update all game items
//=============================================================================
void Spacewar::update()
{
    // check if we want to exit
    CheckForExit();
    // update all game objects
    ship.update(frameTime);
    ship.setDegrees(ship.getDegrees() + frameTime * SHIP_ROTATION_RATE);
    //ship.setScale(ship.getScale() - frameTime * SHIP_SCALE_RATE);
    
    // move ship
    ship.setX(ship.getX() + frameTime * SHIP_SPEED);
    //ship.setY(ship.getY() + frameTime * SHIP_SPEED);

    // check edge bounds
    wrapScreenEdge();
}

void Spacewar::wrapScreenEdge() {
    // left/right bounds wrapping
    if (ship.getX() > GAME_WIDTH) {
        // off the edge to the right
        ship.setX((float)-ship.getWidth());
    } else if(ship.getX() < -ship.getWidth()) {
        ship.setX((float)GAME_WIDTH); // top left of image
    }

    // top/bottom bounds wrapping
    if (ship.getY() > GAME_HEIGHT) {
        // off the bottom edge, place it at the top
        ship.setY((float)-ship.getHeight());
    } else if (ship.getY() < -ship.getHeight()) {
        // off the top edge, place it at the bottom
        ship.setY((float)GAME_HEIGHT);
    }
}



//=============================================================================
// Artificial Intelligence
//=============================================================================
void Spacewar::ai()
{}

//=============================================================================
// Handle collisions
//=============================================================================
void Spacewar::collisions()
{}

//=============================================================================
// Render game items
//=============================================================================
void Spacewar::render()
{
    try {
        graphics->spriteBegin();
        nebula.draw();
        planet.draw();
        ship.draw();
        graphics->spriteEnd();
    }
    catch (...) {
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error in Graphics::render"));
    }

}


void Spacewar::CheckForExit() {
    // ESC key
    if (input->isKeyDown(ESC_KEY)) {
        PostQuitMessage(0);
    }

    // typed exit? or pressed B button
    if (input->getTextIn() == "exit" || input->getGamepadB(0)) {
        PostQuitMessage(0);
    }
}

//=============================================================================
// The graphics device was lost.
// Release all reserved video memory so graphics device may be reset.
//=============================================================================
void Spacewar::releaseAll()
{
    nebulaTexture.onLostDevice();
    planetTexture.onLostDevice();
    shipTex.onLostDevice();
    
    Game::releaseAll();
    return;
}

//=============================================================================
// The grahics device has been reset.
// Recreate all surfaces.
//=============================================================================
void Spacewar::resetAll()
{
    nebulaTexture.onResetDevice();
    planetTexture.onResetDevice();
    shipTex.onResetDevice();

    Game::resetAll();
    return;
}
