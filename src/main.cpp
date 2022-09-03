#define SDL_MAIN_HANDLED

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <bits/stdc++.h>

#include "Button.hpp"

const int SCREEN_WIDTH = 600;

const int MARGIN = 20;
const int BAR_WIDTH = 15;
const int BAR_HEIGHT_MIN = 10;
const int NUM_BARS = (SCREEN_WIDTH - MARGIN * 2) / BAR_WIDTH;
const int BAR_HEIGHT_MAX = NUM_BARS * BAR_HEIGHT_MIN;

const int SCREEN_HEIGHT = BAR_HEIGHT_MIN * NUM_BARS + MARGIN + 110;

const int BT_NUM_INLINE = 4;
const int BT_WIDTH = (SCREEN_WIDTH - MARGIN * (2 + BT_NUM_INLINE - 1)) / BT_NUM_INLINE;
const int BT_HEIGHT = 40;

const SDL_Color BLACK = {0, 0, 0, 255};
const SDL_Color SELECTED = {100, 100, 200, 255};
const SDL_Color BAR1 = {128, 128, 128, 255};
const SDL_Color BAR2 = {159, 159, 159, 255};
const SDL_Color BAR3 = {192, 192, 192, 255};
const SDL_Color BAR_COLORS[] = {BAR1, BAR2, BAR3};
const int BAR_ARR_SIZE = sizeof(BAR_COLORS) / sizeof(BAR_COLORS[0]);

const int FPS = 60;
const int FRAME_DELAY = 1000 / FPS;

int main(int argv, char *args[]){
    if(SDL_Init(SDL_INIT_VIDEO) < 0){
        return 1;
    }

    SDL_Window *win = SDL_CreateWindow("Sorting Algorithm Visualization", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

    if(win == NULL){
        std::cout << "Window could not be created. SDL_Error: " << SDL_GetError() << std::endl;
        return -1;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_SOFTWARE);

    if(TTF_Init()){
        std::cout << "TTF could not be initialized. TTF_Error" << SDL_GetError() << std::endl;
        return -1;
    }

    TTF_Font *font = TTF_OpenFont("res/font/font.ttf", 18);
    if(font == NULL){
        std::cout << "Failed to load font. SDL_ttf Error: " << TTF_GetError() << std::endl;
    }

    SDL_Event event;

    Uint32 frame_start;
    int frame_time;

    int arr[NUM_BARS];
    int arrSize = sizeof(arr) / sizeof(arr[0]);
    
    for(int i = 0; i < NUM_BARS; ++i){
        arr[i] = i + 1;
    }

    std::random_shuffle(arr, arr + arrSize);

    int x = 0;

    const char *bubbleSortBtText = "Bubble Sort";
    Button bubbleSortBt(MARGIN * (x + 1) + BT_WIDTH * x++, MARGIN, BT_WIDTH, BT_HEIGHT, bubbleSortBtText, font, BLACK);

    const char *selectionSortBtText = "Selection Sort";
    Button selectionSortBt(MARGIN * (x + 1) + BT_WIDTH * x++, MARGIN, BT_WIDTH, BT_HEIGHT, selectionSortBtText, font, BLACK);

    const char *insertionSortBtText = "Insertion Sort";
    Button insertionSortBt(MARGIN * (x + 1) + BT_WIDTH * x++, MARGIN, BT_WIDTH, BT_HEIGHT, insertionSortBtText, font, BLACK);

    const char *resetBtText = "Reset";
    Button resetBt(MARGIN * (x + 1) + BT_WIDTH * x++, MARGIN, BT_WIDTH, BT_HEIGHT, resetBtText, font, BLACK);

    int i, j, indexMin, count, key, temp;

    int resetY = 0;

    bool reset = false;
    bool resetMax = false;
    bool inloop = false;

    bool bubbleSort = false;
    bool selectionSort = false;
    bool insertionSort = false;

    SDL_Point mouse;

    bool running = true;
    while(running){
        frame_start = SDL_GetTicks();

        SDL_GetMouseState(&mouse.x, &mouse.y);

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);


        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

        bubbleSortBt.create(renderer);
        selectionSortBt.create(renderer);
        insertionSortBt.create(renderer);
        resetBt.create(renderer);

        if(reset && !resetMax && resetY <= BAR_HEIGHT_MAX + MARGIN){
            resetY += 10;
        }

        else if(reset){
            if(resetY > BAR_HEIGHT_MAX + MARGIN){
                std::random_shuffle(arr, arr + arrSize);
            }

            resetMax = true;

            if(resetY > 0){
                resetY -= 10;
            }

            else{
                reset = false;
                resetMax = false;

                resetBt.set_color(BLACK);
            }
        }

        if(bubbleSort && !reset){
            if(i < arrSize - 1){
                if(j < arrSize - i - 1){
                    if(arr[j] > arr[j + 1]){
                        int temp = arr[j + 1];
                        arr[j + 1] = arr[j];
                        arr[j] = temp;
                    }
                    
                    ++j;
                }

                else{
                    j = 0;
                    ++i;
                }
            }

            else{
                bubbleSort = false;
                bubbleSortBt.set_color(BLACK);
            }
        }

        else if(selectionSort && !reset){
            if(i < arrSize - 1){
                indexMin = i;

                for(int j = 0; j < arrSize - i; ++j){
                    if(arr[indexMin] > arr[i + j]){
                        indexMin = i + j;
                    }
                }

                temp = arr[i];
                arr[i] = arr[indexMin];
                arr[indexMin] = temp;
                    
                ++i;
            }

            else{
                selectionSort = false;
                selectionSortBt.set_color(BLACK);
            }
        }

        else if(insertionSort && !reset){
            if(i < arrSize){
                if(!inloop){
                    count = 0;
                    key = arr[i];

                    inloop = true;
                }

                if(inloop){
                    if(key < arr[i - 1 - count] && i - 1 - count >= 0){
                        arr[i - count] = arr[i - 1 - count];
                        arr[i - 1 - count] = key;

                        ++count;
                    }

                    else{
                        inloop = false;
                    }
                }

                if(!inloop)
                    ++i;
            }

            else{
                insertionSort = false;
                insertionSortBt.set_color(BLACK);
            }
        }

        for(int k = 0; k < NUM_BARS; ++k){
            SDL_SetRenderDrawColor(renderer, BAR_COLORS[k % BAR_ARR_SIZE].r, BAR_COLORS[k % BAR_ARR_SIZE].g, BAR_COLORS[k % BAR_ARR_SIZE].b, 255);

            int barHeight = BAR_HEIGHT_MIN + BAR_HEIGHT_MIN * arr[k];

            int x = MARGIN + BAR_WIDTH * k;
            int y = SCREEN_HEIGHT - barHeight - MARGIN + resetY;

            SDL_Rect bar = {x, y, BAR_WIDTH, barHeight};
            SDL_RenderFillRect(renderer, &bar);
        }

        if(SDL_PollEvent(&event)){
            if(event.type == SDL_QUIT)
                running = false;

            if(event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT){
                if(resetBt.clicked(mouse.x, mouse.y) && !bubbleSort && !selectionSort && !insertionSort){
                    reset = true;

                    resetBt.set_color(SELECTED);
                }

                if(bubbleSortBt.clicked(mouse.x, mouse.y) && !reset && !selectionSort && !insertionSort){
                    bubbleSort = true;

                    bubbleSortBt.set_color(SELECTED);

                    i = j = 0;
                }

                if(selectionSortBt.clicked(mouse.x, mouse.y) && !reset && !bubbleSort && !insertionSort){
                    selectionSort = true;

                    selectionSortBt.set_color(SELECTED);

                    i = j = 0;
                }

                if(insertionSortBt.clicked(mouse.x, mouse.y) && !reset && !bubbleSort && !selectionSort){
                    insertionSort = true;

                    insertionSortBt.set_color(SELECTED);

                    i = j = 0;
                    inloop = false;
                }
            }
        }

        SDL_RenderPresent(renderer);

        frame_time = SDL_GetTicks() - frame_start;
        if(FRAME_DELAY > frame_time){
            SDL_Delay(FRAME_DELAY - frame_time);
        }
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(win);

    SDL_Quit();

    return 0;
}
