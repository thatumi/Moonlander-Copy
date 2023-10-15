#include "render.h"

void spaceship_render(SDL_Renderer *renderer, SDL_Texture *spaceShip, float newX, float newY, float prevX, float prevY) {
    // Clear the previous spaceship location
    boxRGBA(renderer, prevX + 9, prevY + 1, prevX + 21, prevY + 22, 0, 0, 0, 255);

    // Render the spaceship at the new location
    SDL_Rect src = { NULL, NULL, 32, 34 };
    SDL_Rect dest = { newX, newY, 32, 34 };
    SDL_RenderCopy(renderer, spaceShip, &src, &dest);
}



void text_render(SDL_Renderer *renderer, int x, int y, char *text, TTF_Font *font, SDL_Texture **capture_t, SDL_Rect *where) {
    SDL_Surface *capture;
    SDL_Color textColor = {255, 255, 255, 0};

    // Render the text to a surface
    capture = TTF_RenderUTF8_Solid(font, text, textColor);

    // Create an SDL texture from the surface
    *capture_t = SDL_CreateTextureFromSurface(renderer, capture);

    // Set the rendering location and size
    where->x = (x - capture->w) / 2;
    where->y = y;
    where->w = capture->w;
    where->h = capture->h;

     // Copy the texture to the renderer
    SDL_RenderCopy(renderer, *capture_t, NULL, where);

    // Free the surface
    SDL_FreeSurface(capture);

    // Destroy the texture (should this be SDL_DestroyTexture(*capture_t)?)
    SDL_DestroyTexture(capture_t);
}

void renderMainMenu(SDL_Renderer *renderer, TTF_Font *fontMenu, SDL_Texture **text_tMenu, SDL_Rect *whereMenu) {
    text_render(renderer, 640, 240, "Press a button to continue", fontMenu, text_tMenu, whereMenu);
    SDL_RenderPresent(renderer);
}

void renderHUDText(SDL_Renderer *renderer, TTF_Font *fontHud, float fuel, bool thrusters,
    float velocityX, float velocityY, float spaceshipY,
    SDL_Texture **text_tHud, SDL_Texture **text_tHudValue,
    SDL_Rect *whereHud, SDL_Rect *whereHudValue) {

    char fuelValue[32];
    snprintf(fuelValue, sizeof fuelValue, "%f", fuel);
    char velocityXValue[32];
    snprintf(velocityXValue, sizeof velocityXValue, "%f", velocityX);
    char velocityYValue[32];
    snprintf(velocityYValue, sizeof velocityYValue, "%f", velocityY);
    char spaceshipYValue[32];
    snprintf(spaceshipYValue, sizeof spaceshipYValue, "-%f", spaceshipY);

    boxRGBA(renderer, 410, 2, 590, 125, 124, 23, 32, 255);
    text_render(renderer, 930, 5, "Fuel:", fontHud, text_tHud, whereHud);
    text_render(renderer, 1100, 5, fuelValue, fontHud, text_tHudValue, whereHudValue);
    text_render(renderer, 930, 25, "thrusters:", fontHud, text_tHud, whereHud);
    if (thrusters) {
        text_render(renderer, 1100, 25, "ON", fontHud, text_tHudValue, whereHudValue);
    } else {
        text_render(renderer, 1100, 25, "OFF", fontHud, text_tHudValue, whereHudValue);
    }
    text_render(renderer, 930, 45, "X velocity:", fontHud, text_tHud, whereHud);
    text_render(renderer, 1100, 45, velocityXValue, fontHud, text_tHudValue, whereHudValue);
    text_render(renderer, 930, 65, "Y velocity:", fontHud, text_tHud, whereHud);
    text_render(renderer, 1100, 65, velocityYValue, fontHud, text_tHudValue, whereHudValue);
    text_render(renderer, 930, 85, "Height:", fontHud, text_tHud, whereHud);
    text_render(renderer, 1100, 85, spaceshipYValue, fontHud, text_tHudValue, whereHudValue);
    text_render(renderer, 930, 105, "Contact light:", fontHud, text_tHud, whereHud);
    if (spaceshipY > 370) {
        text_render(renderer, 1100, 105, "ON", fontHud, text_tHudValue, whereHudValue);
    } else {
        text_render(renderer, 1100, 105, "OFF", fontHud, text_tHudValue, whereHudValue);
    }
}

void createStarryBackground(SDL_Renderer *renderer) {
    for (int i = 0; i < 500; ++i){
        filledCircleRGBA(renderer, rand() % 640, rand() % 480, 2, 239, 239, 232, 255);
    }
}

void loadSpaceShipTexture(SDL_Renderer *renderer, SDL_Texture **spaceShip) {
    *spaceShip = IMG_LoadTexture(renderer, "spaceship.png");
    if (*spaceShip == NULL) {
        printf("Image could not be opened");
        exit(1);
    }
}
