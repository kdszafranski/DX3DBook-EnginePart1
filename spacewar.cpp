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

}

//=============================================================================
// Update all game items
//=============================================================================
void Spacewar::update()
{
    //// check if we want to exit
    CheckForExit();

    //// vibrate gamepad based on trigger input level
    //const BYTE leftTriggerAmount = input->getGamepadLeftTrigger(0);
    //if (leftTriggerAmount > 0) {
    //    // BYTE -> WORD 0xFF -> 0xFFFF which is 255 times larger 255 * 255 = 65025
    //    // still off a bit. max gamepad motor speed is 65535...
    //    input->gamePadVibrateLeft(0, leftTriggerAmount * 255, 1);
    //}
    //
    //// vibrate gamepad based on trigger input level
    //const BYTE rightTriggerAmount = input->getGamepadRightTrigger(0);
    //if (rightTriggerAmount > 0) {
    //    input->gamePadVibrateRight(0, rightTriggerAmount * 255, 1);
    //}

    // left click or X button changes bg color to red
    if (input->getMouseLButton() || input->getGamepadX(0)) {
        graphics->setBackColor(SETCOLOR_ARGB(255, 128, 0, 0));
    }
    // right click or A button changes bg color to yellow
    if (input->getMouseRButton() || input->getGamepadA(0)) {
        graphics->setBackColor(SETCOLOR_ARGB(255, 255, 255, 0));
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
    //LPCSTR temp = (LPCSTR)input->getGamepadLeftTrigger(0);
    //TextOutA(graphics->getDC(), 10, 10, (LPCSTR)temp, 4);
    
    // display entered characters, flickers... should be thru D3D
    /*LPCSTR temp = input->getTextIn().c_str();
    TextOutA(graphics->getDC(), 10, 10, temp, 20);*/

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

    Game::resetAll();
    return;
}
