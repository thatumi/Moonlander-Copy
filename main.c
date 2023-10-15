#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL2_gfxPrimitives.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include ".\map.h"
#include "debugmalloc\debugmalloc.h"
#include ".\render.h"
#include ".\sdl.h"

/**
 * @brief Detect collisions between a spaceship and map points.
 *
 * This function checks for collisions between a spaceship and map points in a linked list.
 * It takes into account the spaceship's position, thrusters status, velocities, and whether
 * the player has won the game. If a collision or victory condition is met, it sets the 'won'
 * parameter accordingly and returns a boolean value indicating if the game should quit.
 *
 * @param mapStart Pointer to the start of the linked list of map points.
 * @param spaceshipX The X-coordinate of the spaceship.
 * @param spaceshipY The Y-coordinate of the spaceship.
 * @param thrusters A boolean indicating if the spaceship's thrusters are active.
 * @param velocityY The vertical velocity of the spaceship.
 * @param velocityX The horizontal velocity of the spaceship.
 * @param won A pointer to a boolean variable that will be set to 'true' if the player wins.
 *
 * @return 'true' if the game should quit due to a collision or victory condition, 'false' otherwise.
 */
bool collisionDetector(mapPoint *mapStart, float spaceshipX, float spaceshipY, bool thrusters, float velocityY, float velocityX, bool *won){
    bool quit = false;
    mapPoint *mover;
    mapPoint *min;
    mapPoint *max;

    for (mover = mapStart; mover != NULL; mover = mover->next){
        if(spaceshipX + 10 >= mover->x){
            min = mover;
        }
        if(spaceshipX + 10 < mover->x){
            max = mover;
            break;
        }
    }
    if(spaceshipX > 620 || spaceshipX < -12){
        quit = true;
    }else if((spaceshipY + 21 > max->y)) {
        if(max->y == min->y){
            if(!thrusters && velocityY < 3.0f && velocityX < 0.8f && velocityX > -0.8f){
                *won = true;
            }
        }
        quit = true;
    }
    return quit;
}

/**
 * @brief Handle the end game state and display the result.
 */
void handleEndGameState(SDL_Renderer *renderer, TTF_Font *fontMenu, SDL_Texture **text_tMenu, SDL_Rect *whereMenu, bool won) {
    if (!won) {
        text_render(renderer, 640, 240, "You lost :(", fontMenu, text_tMenu, whereMenu);
    } else {
        text_render(renderer, 640, 240, "You're winner", fontMenu, text_tMenu, whereMenu);
    }
    SDL_RenderPresent(renderer);
}

/**
 * @brief Initialize the random seed based on the current time.
 */
void initializeRandomSeed() {
    srand(time(NULL));
}

/**
 * @brief Initialize the fonts for rendering text.
 * @param[out] fontMenu - Pointer to the menu font.
 * @param[out] fontHud - Pointer to the HUD font.
 */
void initializeFonts(TTF_Font **fontMenu, TTF_Font **fontHud) {
    TTF_Init();
    *fontMenu = TTF_OpenFont("LiberationSerif-Regular.ttf", 32);
    *fontHud = TTF_OpenFont("LiberationSerif-Regular.ttf", 15);
    if (!fontMenu || !fontHud) {
        SDL_Log("Could not open the font! %s\n", TTF_GetError());
        exit(1);
    }
}

/**
 * @brief Initialize spaceship and game variables.
 * @param[out] spaceshipX - Initial X coordinate of the spaceship.
 * @param[out] spaceshipY - Initial Y coordinate of the spaceship.
 * @param[out] velocityY - Initial Y velocity of the spaceship.
 * @param[out] velocityX - Initial X velocity of the spaceship.
 * @param[out] fuel - Initial fuel level.
 * @param[out] thrusters - Initial state of thrusters.
 * @param[out] won - Initial game state.
 */
void initializeGameVariables(float *spaceshipX, float *spaceshipY, float *velocityY,
                            float *velocityX, float *fuel, bool *thrusters, bool *won) {
    *spaceshipX = 312.0f;
    *spaceshipY = 120.0f;
    *velocityY = 0.0001f;
    *velocityX = 0.0001f;
    *fuel = 25.0f;
    *thrusters = true;
    *won = false;
}

/**
 * @brief Update spaceship thrust and fuel.
 *
 * This function handles the spaceship's thrust and fuel logic based on user input.
 *
 * @param thrusters Indicates if thrusters are activated.
 * @param velocityX The spaceship's horizontal velocity.
 * @param velocityY The spaceship's vertical velocity.
 * @param fuel The remaining fuel.
 */
void updateSpaceshipThrustAndFuel(bool thrusters, float *velocityX, float *velocityY, float *fuel) {
    if (!thrusters) {
        if (*velocityY < 10.0f) {
            *velocityY += 0.005f;
        }
    } else {
        if (*fuel <= 0.0f) {
            thrusters = false;
            *fuel = 0.0f;
        } else {
            if (*velocityY > 0.0f) {
                *velocityY -= 0.005f;
            } else {
                *velocityY = 0.02f;
                *velocityX = 0.0f;
            }
            *fuel -= 0.04f;
        }
    }
}

/**
 * @brief Update spaceship position based on velocity.
 *
 * This function updates the spaceship's position based on its velocity and renders it.
 *
 * @param renderer The SDL renderer.
 * @param spaceShip The spaceship's texture.
 * @param spaceshipX The current X coordinate of the spaceship.
 * @param spaceshipY The current Y coordinate of the spaceship.
 * @param velocityX The spaceship's horizontal velocity.
 * @param velocityY The spaceship's vertical velocity.
 */
void updateSpaceshipPosition(SDL_Renderer *renderer, SDL_Texture *spaceShip, float *spaceshipX, float *spaceshipY, float velocityX, float velocityY) {
    if (velocityY != 0) {
        float spaceshipNewY = *spaceshipY + velocityY;
        float spaceshipNewX = *spaceshipX + velocityX;
        spaceship_render(renderer, spaceShip, spaceshipNewX, spaceshipNewY, *spaceshipX, *spaceshipY);
        *spaceshipY = spaceshipNewY;
        *spaceshipX = spaceshipNewX;
    }
}

/**
 * @brief The main function of the lunar landing game.
 *
 * This function serves as the entry point for the lunar landing game. It performs several
 * key tasks such as initializing the game's components, rendering the main menu, handling
 * user input, and managing game state transitions. It also manages spaceship controls,
 * collision detection, and the game loop, making it the heart of the game's functionality.
 *
 * @param argc Number of command-line arguments.
 * @param argv Array of command-line arguments.
 * @return An integer representing the program's exit status.
 */
int main(int argc, char *argv[]) {
    // Initialize random seed
    initializeRandomSeed();

    // Initialize variables for menu, HUD, and fonts
    SDL_Rect whereMenu, whereHud, whereHudValue;
    SDL_Texture *text_tMenu, *text_tHud, *text_tHudValue;
    TTF_Font *fontMenu, *fontHud;
    initializeFonts(&fontMenu, &fontHud);

    // Initialize linked list for map points
    mapPoint *mapStart;
    initializeMap(&mapStart);

    // Initialize spaceship and game variables
    float spaceshipX, spaceshipY, velocityY, velocityX, fuel;
    bool thrusters, won;
    initializeGameVariables(&spaceshipX, &spaceshipY, &velocityY, &velocityX, &fuel, &thrusters, &won);


    //FPS, time vars
    const int FPS = 60;
    const int frameDelay = 1000 / FPS;
    Uint32 frameStart;
    int frameTime;

    // Initialize SDL window and renderer
    SDL_Window *window;
    SDL_Renderer *renderer;
    initializeSDLWindow(&window, &renderer);


    // Load spaceship texture
    SDL_Texture *spaceShip;
    loadSpaceShipTexture(renderer, &spaceShip);

    // Create starry background for the menu
    createStarryBackground(renderer);

    // Render the main menu and wait for user input
    renderMainMenu(renderer, fontMenu, &text_tMenu, &whereMenu);


    // Main Menu
    bool quit = false;
    while (!quit) {
        SDL_Event event;
        SDL_WaitEvent(&event);
        switch (event.type) {
            case SDL_KEYDOWN:
                // Exit the menu to start the game, clear text, and set the background to black
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                SDL_RenderClear(renderer);

                // Create a random map, storing its initial coordinates
                struct mapCoordinates mk = {0, 470, false};
                mk = createMap(mk, renderer, mapStart);
                // Loop until the map is drawn to the edge
                while(mk.x < 640){
                    mk = createMap(mk, renderer, mapStart);
                }

                // Set the initial position of the spaceship
                spaceship_render(renderer, spaceShip, spaceshipX, spaceshipY, 0, 0);

                SDL_RenderPresent(renderer);
                quit = true;
                break;
            case SDL_QUIT:
                quit = true;
                break;
        }
    }


    // Spaceship Controls
    quit = false;
    while (!quit) {
        frameStart = SDL_GetTicks();
        renderHUDText(renderer, fontHud, fuel, thrusters, velocityX, velocityY, spaceshipY, &text_tHud, &text_tHudValue, &whereHud, &whereHudValue);

        // Thruster/fuel logic
        updateSpaceshipThrustAndFuel(thrusters, &velocityX, &velocityY, &fuel);

        // Update movement
        updateSpaceshipPosition(renderer, spaceShip, &spaceshipX, &spaceshipY, velocityX, velocityY);

        // Handle keyboard input
        SDL_Event event;
        while(SDL_PollEvent(&event)){
            switch (event.type) {
                case SDL_KEYDOWN:
                    if (event.key.keysym.sym == SDLK_SPACE) {
                        thrusters = !thrusters;
                    }
                    if (event.key.keysym.sym == SDLK_RIGHT) {
                        if(velocityX < 1.5f){
                            velocityX += 0.06f;
                        }
                    }
                    if (event.key.keysym.sym == SDLK_LEFT) {
                        if(velocityX > -1.5f){
                            velocityX -= 0.06f;
                        }
                    }
                    if (event.key.keysym.sym == SDLK_F5) {
                         // Save the current map to a file
                        mapSave(&mapStart);
                    }
                    if (event.key.keysym.sym == SDLK_F9) {
                        // Load a map from a file and reset game variables
                        mapLoad(renderer, &mapStart);

                        // Clear the spaceship
                        boxRGBA(renderer, spaceshipX + 9, spaceshipY + 1, spaceshipX + 21, spaceshipY + 22, 0, 0, 0, 255);

                        // Reset spaceship/game variables
                        spaceshipX = 312.0f;
                        spaceshipY = 120.0f;
                        velocityY = 0.0001f;
                        velocityX = 0.0001f;
                        fuel = 25.0f;
                        thrusters = true;
                        won = false;
                    }
                    break;

                case SDL_QUIT:
                    quit = true;
                    break;
            }
        }

        frameTime = SDL_GetTicks() - frameStart;
        if(frameDelay > frameTime){
            SDL_Delay(frameDelay - frameTime);
        }

        SDL_RenderPresent(renderer);
        quit = collisionDetector(mapStart, spaceshipX, spaceshipY, thrusters, velocityY, velocityX, &won);
    }

    // End Game State
    quit = false;
    while (!quit) {
        // Handle end game state
        handleEndGameState(renderer, fontMenu, &text_tMenu, &whereMenu, won);
        SDL_Event event;
        while(SDL_PollEvent(&event)){
            switch (event.type) {
                case SDL_QUIT:
                    quit = true;
                    break;
            }
        }
    }

    // Clean up fonts, spaceship texture, and linked list
    TTF_CloseFont(fontHud);
    TTF_CloseFont(fontMenu);
    SDL_DestroyTexture(spaceShip);
    clear(&mapStart);

    SDL_Quit();

    return 0;
}
