#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL2_gfxPrimitives.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

void urhajo_rajzol(SDL_Renderer *renderer, SDL_Texture *spaceShip, float ujX, float ujY, float elozoX, float elozoY) {
    //Az űrhajó előző helyének eltüntetése
    boxRGBA(renderer, elozoX + 9, elozoY + 1, elozoX + 21, elozoY + 22, 0, 0, 0, 255);

    //Űrhajó új helyzetének kirajzolása
    SDL_Rect src = { NULL, NULL, 32, 34 };
    SDL_Rect dest = { ujX, ujY, 32, 34 };
    SDL_RenderCopy(renderer, spaceShip, &src, &dest);
}



void felirat_render(SDL_Renderer *renderer, int x, int y, char *text, TTF_Font *font, SDL_Texture **felirat_t, SDL_Rect *hova) {
    SDL_Surface *felirat;
    SDL_Color textColor = {255, 255, 255, 0};

    felirat = TTF_RenderUTF8_Solid(font, text, textColor);
    *felirat_t = SDL_CreateTextureFromSurface(renderer, felirat);
    hova->x = (x - felirat->w) / 2;
    hova->y = y;
    hova->w = felirat->w;
    hova->h = felirat->h;
    SDL_RenderCopy(renderer, *felirat_t, NULL, hova);
    SDL_FreeSurface(felirat);
    SDL_DestroyTexture(felirat_t);
}
