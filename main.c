#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL2_gfxPrimitives.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include "debugmalloc\debugmalloc.h"
#include ".\render.h"


void sdl_init(int szeles, int magas, SDL_Window **pwindow, SDL_Renderer **prenderer) {
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        SDL_Log("Nem indithato az SDL: %s", SDL_GetError());
        exit(1);
    }
    SDL_Window *window = SDL_CreateWindow("Moon Lander", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, szeles, magas, 0);
    if (window == NULL) {
        SDL_Log("Nem hozhato letre az ablak: %s", SDL_GetError());
        exit(1);
    }
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
    if (renderer == NULL) {
        SDL_Log("Nem hozhato letre a megjelenito: %s", SDL_GetError());
        exit(1);
    }

    *pwindow = window;
    *prenderer = renderer;
}


//map kreáláshoz struktúra a map pontjainak eltárolására
typedef struct mapKoordinatak {
    int x;
    int y;
    bool leszallo;
} mapKoordinatak;

//láncolt lista struktúrája és függvényei
typedef struct mapPont {
    struct mapKoordinatak;
    struct mapPont *kov;
} mapPont;

bool collisionDetector(mapPont *mapEleje, float urhajoX, float urhajoY, bool hajtomu, float velocityY, float velocityX, bool *nyertel){
    bool quit = false;

    mapPont *mozgo;
    mapPont *min;
    mapPont *max;
    for (mozgo = mapEleje; mozgo != NULL; mozgo = mozgo->kov){
        if(urhajoX + 10 >= mozgo->x){
            min = mozgo;
        }
        if(urhajoX + 10 < mozgo->x){
            max = mozgo;
            break;
        }
    }
    if(urhajoX > 620 || urhajoX < -12){
        quit = true;
    }else if((urhajoY + 21 > max->y)) {
        if(max->y == min->y){
            if(!hajtomu && velocityY < 3.0f && velocityX < 0.8f && velocityX > -0.8f){
                *nyertel = true;
            }
        }
        quit = true;
    }
    return quit;
}



mapPont *vegere_fuz(mapPont *eleje, int *ujX, int *ujY){
    mapPont *uj;
    uj = (mapPont*) malloc(sizeof(mapPont));
    uj->x = ujX;
    uj->y = ujY;
    uj->kov = NULL;

    if (eleje == NULL) {
       eleje = uj;
    } else {
       mapPont *mozgo = eleje;
       while (mozgo->kov != NULL)
          mozgo = mozgo->kov;
       mozgo->kov = uj;
    }
    return eleje;
}

mapPont *felszabadit(mapPont **eleje){
    mapPont *iter = *eleje;
    while (iter != NULL) {
        mapPont *kov = iter->kov;
        free(iter);
        iter = kov;
    }
}

//maphoz kapcsolodo fuggvenyek
void mapMentes(mapPont **eleje){
    FILE* mapFile;
    mapFile = fopen("map.txt", "w");
    if (mapFile != NULL) {
        mapPont *mozgo;
        for (mozgo = *eleje; mozgo != NULL; mozgo = mozgo->kov){
            //x és y koordináta kiírása egymás alá
            fprintf(mapFile,"%d\n", mozgo->x);
            fprintf(mapFile, "%d\n", mozgo->y);
        }
        fclose(mapFile);
    }
    else {
        perror("Nem sikerült megnyitni a fajlt");
    }
}

void mapBetoltes(SDL_Renderer *renderer,  mapPont **eleje){

    SDL_RenderClear(renderer);
    FILE *mapFile;
    mapFile = fopen("map.txt", "r");

    int a = 0;
    int temp = 0;
    int regiX = 0;
    int regiY = 470;
    for(int i = 0; fscanf(mapFile, "%d", &a) == 1; i++) {
        if((i+1) % 2 == 0){
            vegere_fuz(*eleje, temp, a);
            if( i >= 2){
                lineRGBA(renderer,  regiX, regiY , temp, a, 239, 239, 232, 255);
            }
            regiY = a;
            regiX = temp;
        }else{
            temp = a;
        }
    }

    fclose(mapFile);
    SDL_RenderPresent(renderer);
}

mapKoordinatak createMap(struct mapKoordinatak koordinata, SDL_Renderer *renderer, mapPont *eleje){
    struct mapKoordinatak uj;
    uj.x = koordinata.x + rand() % 40;
    uj.y = koordinata.y - rand() % 20;

    //Ha nincs leszállópálya, x% eséllyel rajzol bárhova egy leszállót, amúgy meg a végére
     if(!uj.leszallo && (rand() % 33) + 1 == 1){
        uj.y = koordinata.y;
        uj.x = koordinata.x + 40;
        uj.leszallo = true;
        lineRGBA(renderer, koordinata.x , koordinata.y , uj.x, uj.y, 219, 139, 182, 255);
     }else{
         //Ha a végére sincs leszállópálya, fixen rajzoljon egyet
         if(!uj.leszallo && koordinata.x > 600){
            uj.x = 640;
            uj.y = koordinata.y;
            lineRGBA(renderer, koordinata.x , koordinata.y , uj.x, uj.y, 219, 139, 182, 255);
            uj.leszallo = true;
         }else{
            //Ellenőrzi, hogy az új rajzolandó szakasz nem lesz-e túl alacsonyan vagy magasan. Ha túl magasan vagy alacsonyan lesz, az ellentétes
            //irányba rajzol, ha nincs se túl alacsonyan, se túl magasan, 50-50% eséllyel rajzolja a következőt meg.
            if(uj.y < 400){
                uj.y = koordinata.y + rand() % 20;
            }else if(uj.y > 450){
                uj.y = koordinata.y - rand() % 20;
            }else{
                if((rand() % 2) + 1 == 2){
                    uj.y = koordinata.y + rand() % 20;
                }
            }
            lineRGBA(renderer, koordinata.x , koordinata.y , uj.x, uj.y, 239, 239, 232, 255);
         }
     }

    //lancolt lista
    vegere_fuz(eleje, uj.x, uj.y);
    return uj;

}


int main(int argc, char *argv[]) {

    //random seed beállítása jelenlegi idő szerint, hogy ne mindig ugyanaz legyen a map
    srand(time(NULL));

    //kiírásokhoz
    SDL_Rect hovaMenu, hovaHud, hovaHudErtek;
    SDL_Texture *felirat_tMenu, *felirat_tHud, *felirat_tHudErtek;
    //fontok betöltése
    TTF_Init();
    TTF_Font *fontMenu = TTF_OpenFont("LiberationSerif-Regular.ttf", 32);
    TTF_Font *fontHud = TTF_OpenFont("LiberationSerif-Regular.ttf", 15);
    if (!fontMenu || !fontHud) {
        SDL_Log("Nem sikerult megnyitni a fontot! %s\n", TTF_GetError());
        exit(1);
    }

    //Láncolt lista inicializálás:
    mapPont *mapEleje;
    mapEleje = (mapPont*) malloc(sizeof(mapPont));
    mapEleje->x = 0;
    mapEleje->y = 470;
    mapEleje->kov = NULL;


    //űrhajó/játék valtozok
    float urhajoX = 312.0f;
    float urhajoY = 120.0f;
    float velocityY = 0.0001f;
    float velocityX = 0.0001f;
    float fuel = 25.0f;
    bool hajtomu = true;
    bool nyertel = false;

    //FPS, idő változók
    const int FPS = 60;
    const int frameDelay = 1000 / FPS;
    Uint32 frameStart;
    int frameTime;

    //Ez a 4 változó az űrhajó a mappal való ütközés detektáláshoz kell
    int minErtek = 0;
    int minHely = 0;
    int maxErtek = 0;
    int maxHely = 0;

    //Ablak
    SDL_Window *window;
    SDL_Renderer *renderer;
    sdl_init(640, 480, &window, &renderer);


    //Spaceship textura betöltése
    SDL_Texture *spaceShip = IMG_LoadTexture(renderer, "spaceship.png");
    if (spaceShip == NULL) {
        printf("Nem nyithato meg a kepfajl:");
        exit(1);
    }


    //csillag háttér menühöz
    for (int i = 0; i < 500; ++i){
        filledCircleRGBA(renderer, rand() % 640, rand() % 480, 2 , 239, 239, 232, 255);
    }

    felirat_render(renderer, 640, 240, "Nyomj egy gombot a folytatashoz", fontMenu, &felirat_tMenu, &hovaMenu);

    SDL_RenderPresent(renderer);

    //Főmenü
    bool quit = false;
    int menuState = 0;
    while (!quit) {
        SDL_Event event;
        SDL_WaitEvent(&event);
        switch (event.type) {
            case SDL_KEYDOWN:
                //Menüből kilépés a játékba, felirat eltüntetés, háttér feketére színezése
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                SDL_RenderClear(renderer);

                //Random map kreálás, első koordinátáinak eltárolása
                struct mapKoordinatak mk = {0, 470, false};
                mk = createMap(mk, renderer, mapEleje);
                //ciklus, amíg a pálya széléig nincs rajzolva
                while(mk.x < 640){
                    mk = createMap(mk, renderer, mapEleje);
                }

                //űrhajó alappozíciója
                urhajo_rajzol(renderer, spaceShip, urhajoX, urhajoY, 0, 0);

                SDL_RenderPresent(renderer);
                quit = true;
                break;
            case SDL_QUIT:
                quit = true;
                break;
        }
    }


    //űrhajó irányítás
    quit = false;
    while (!quit) {
        frameStart = SDL_GetTicks();
        //Kiíráshoz floatok konvertálása charba
        char fuelErtek[32];
        snprintf(fuelErtek, sizeof fuelErtek, "%f", fuel);
        char velocityXErtek[32];
        snprintf(velocityXErtek, sizeof velocityXErtek, "%f", velocityX);
        char velocityYErtek[32];
        snprintf(velocityYErtek, sizeof velocityYErtek, "%f", velocityY);
        char urhajoYErtek[32];
        snprintf(urhajoYErtek, sizeof urhajoYErtek, "-%f", urhajoY);

        //HUD
        boxRGBA(renderer, 410, 2, 590, 125, 124, 23, 32, 255);
        felirat_render(renderer, 930, 5, "Uzemanyag:", fontHud, &felirat_tHud, &hovaHud);
        felirat_render(renderer, 1100, 5, fuelErtek, fontHud, &felirat_tHudErtek, &hovaHudErtek);
        felirat_render(renderer, 930, 25, "Hajtomu:", fontHud, &felirat_tHud, &hovaHud);
        if(hajtomu){
            felirat_render(renderer, 1100, 25, "BE", fontHud, &felirat_tHudErtek, &hovaHudErtek);
        }else{
            felirat_render(renderer, 1100, 25, "KI", fontHud, &felirat_tHudErtek, &hovaHudErtek);
        }
        felirat_render(renderer, 930, 45, "X gyorsulas:", fontHud, &felirat_tHud, &hovaHud);
        felirat_render(renderer, 1100, 45, velocityXErtek, fontHud, &felirat_tHudErtek, &hovaHudErtek);
        felirat_render(renderer, 930, 65, "Y gyorsulas:", fontHud, &felirat_tHud, &hovaHud);
        felirat_render(renderer, 1100, 65, velocityYErtek, fontHud, &felirat_tHudErtek, &hovaHudErtek);
        felirat_render(renderer, 930, 85, "Magassag:", fontHud, &felirat_tHud, &hovaHud);
        felirat_render(renderer, 1100, 85, urhajoYErtek, fontHud, &felirat_tHudErtek, &hovaHudErtek);
        felirat_render(renderer, 930, 105, "Kontaktfeny:", fontHud, &felirat_tHud, &hovaHud);
        if(urhajoY > 370){
            felirat_render(renderer, 1100, 105, "BE", fontHud, &felirat_tHudErtek, &hovaHudErtek);
        }else{
            felirat_render(renderer, 1100, 105, "KI", fontHud, &felirat_tHudErtek, &hovaHudErtek);
        }

        //Hajtómű/üzemanyag logika
        if(!hajtomu){
            if(velocityY < 10.0f){
                velocityY += 0.005f;
            }
        }else{
            if(fuel <= 0.0f){
                hajtomu = false;
                fuel = 0.0f;
            }else{
                if(velocityY > 0.0f){
                    velocityY -= 0.005f;
                }else{
                    velocityY = 0.02f;
                    velocityX = 0.0f;
                }
                fuel = fuel - 0.04f;
            }
        }

        //Mozgás update
        if(velocityY != 0){
            float urhajoUjY = urhajoY + velocityY;
            float urhajoUjX = urhajoX + velocityX;
            urhajo_rajzol(renderer, spaceShip, urhajoUjX, urhajoUjY, urhajoX, urhajoY);
            urhajoY = urhajoUjY;
            urhajoX = urhajoUjX;
        }

        SDL_Event event;
        while(SDL_PollEvent(&event)){
            switch (event.type) {
                case SDL_KEYDOWN:
                    if (event.key.keysym.sym == SDLK_SPACE) {
                        hajtomu = !hajtomu;
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
                        //Jelenlegi pálya mentése fájlba
                        mapMentes(&mapEleje);

                    }
                    if (event.key.keysym.sym == SDLK_F9) {
                        //Pálya betöltése fájlból, alapértékek visszaállítása a játékhoz
                        felszabadit(&mapEleje);
                        mapPont *uj;
                        uj = ( mapPont*) malloc(sizeof( mapPont));
                        uj->x = 0;
                        uj->y = 470;
                        uj->kov = NULL;
                        mapEleje = uj;
                        mapBetoltes(renderer, &mapEleje);

                        //Űrhajó eltüntetése
                        boxRGBA(renderer, urhajoX + 9, urhajoY + 1, urhajoX + 21, urhajoY + 22, 0, 0, 0, 255);

                        //űrhajó/játék valtozok
                        urhajoX = 312.0f;
                        urhajoY = 120.0f;
                        velocityY = 0.0001f;
                        velocityX = 0.0001f;
                        fuel = 25.0f;
                        hajtomu = true;
                        nyertel = false;
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

        quit = collisionDetector(mapEleje, urhajoX, urhajoY, hajtomu, velocityY, velocityX, &nyertel);

    }

    //Játék vége állapot
    quit = false;
    while (!quit) {
        if(!nyertel){
            felirat_render(renderer, 640, 240, "Vesztettel :(", fontMenu, &felirat_tMenu, &hovaMenu);
        }else{
            felirat_render(renderer, 640, 240, "Nyertel :)", fontMenu, &felirat_tMenu, &hovaMenu);
        }
        SDL_RenderPresent(renderer);
        SDL_Event event;
        while(SDL_PollEvent(&event)){
            switch (event.type) {
                case SDL_QUIT:
                    quit = true;
                    break;
            }
        }
    }

    TTF_CloseFont(fontHud);
    TTF_CloseFont(fontMenu);
    SDL_DestroyTexture(spaceShip);
    felszabadit(&mapEleje);

    SDL_Quit();

    return 0;
}
