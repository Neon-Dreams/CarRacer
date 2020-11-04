#include <SDL.h>
#include <SDL_image.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

#define SCREEN_WIDTH 500
#define SCREEN_HEIGHT 300

#define SPRITE_WIDTH 73
#define SPRITE_HEIGHT 41

#define CHANGE_X 1
#define CHANGE_Y 2

int boundary(SDL_Rect* rect, int change, const int SIDE);
int collision(SDL_Rect* car, SDL_Rect* barrier);

int main(int argc, char** argv) {

	SDL_Init(SDL_INIT_VIDEO);
	IMG_Init(IMG_INIT_PNG);

	SDL_Event event;
	bool quit = false;

	srand((unsigned)time(NULL));

	SDL_Window *window = SDL_CreateWindow("Car Racer", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);

	SDL_Renderer* render = SDL_CreateRenderer(window, -1, 0);
	
	SDL_Surface* car = IMG_Load("sprites/car.png");
	SDL_Texture* car_texture = SDL_CreateTextureFromSurface(render, car);

	SDL_Surface* barrier = IMG_Load("sprites/barrier.png");
	SDL_Texture* barrier_texture = SDL_CreateTextureFromSurface(render, barrier);


	SDL_SetRenderDrawColor(render, 105, 105, 105, 0);
	SDL_RenderClear(render);

	SDL_Rect car_rect;

	car_rect.x = 10;
	car_rect.y = (SCREEN_HEIGHT / 2) - (SPRITE_HEIGHT / 2);
	car_rect.w = SPRITE_WIDTH;
	car_rect.h = SPRITE_HEIGHT;

	SDL_Rect rect_barrier;

	rect_barrier.x = 300;
	rect_barrier.y = 150;
	rect_barrier.w = 20;
	rect_barrier.h = 80;

	Uint32 seconds;
	int count = 0;
	int speed = 5;

	while (!quit) {
		
		// Create Barrier
		if (rect_barrier.x < 0) {
			rect_barrier.y = (rand() % SCREEN_HEIGHT) + 1;
			rect_barrier.x = SCREEN_WIDTH;
			count += 1;
		}

		// Collision detection
	
		if (collision(&car_rect, &rect_barrier) == 0) {
			SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Oh no!", "You crashed!", window);
			quit = true;
		}

		// Barrier movement and speed
		if (count > 10) {
			speed -= 1;
			count = 0;
		}
		if (speed == 0) speed = 5;

		seconds = SDL_GetTicks();
		if ((seconds % speed) == 0)
			rect_barrier.x -= 1;

		while (SDL_PollEvent(&event)) {


			if (event.type == SDL_QUIT)
				quit = true;

			switch (event.key.keysym.sym) {

			case SDLK_UP:
				boundary(&car_rect, -10, CHANGE_Y);
				break;

			case SDLK_DOWN:
				boundary(&car_rect, 10, CHANGE_Y);
				break;

			case SDLK_LEFT:
				boundary(&car_rect, -10, CHANGE_X);
				break;

			case SDLK_RIGHT:
				boundary(&car_rect, 10, CHANGE_X);
				break;
			}
		}

		SDL_RenderClear(render);
		SDL_RenderCopy(render, barrier_texture, NULL, &rect_barrier);
		SDL_RenderCopy(render, car_texture, NULL, &car_rect);
		SDL_RenderPresent(render);
		
	}

	SDL_DestroyTexture(car_texture);
	SDL_DestroyTexture(barrier_texture);
	SDL_DestroyRenderer(render);
	SDL_FreeSurface(car);
	SDL_FreeSurface(barrier);
	SDL_DestroyWindow(window);
	IMG_Quit();
	SDL_Quit();
	return 0;
}

int boundary(SDL_Rect* rect, int change, const int SIDE) {
	if (SIDE == CHANGE_X) {
		int change_value = rect->x + change;
		if (change_value < 0)
			rect->x = 0;
		else if (change_value > (SCREEN_WIDTH - SPRITE_WIDTH))
			rect->x = SCREEN_WIDTH - SPRITE_WIDTH;
		else
			rect->x = change_value;
		return 0;
	}

	else if (SIDE == CHANGE_Y) {
		int change_value = rect->y + change;
		if (change_value < 0)
			rect->y = 0;
		else if (change_value > (SCREEN_HEIGHT - SPRITE_HEIGHT))
			rect->y = SCREEN_HEIGHT - SPRITE_HEIGHT;
		else
			rect->y = change_value;
		return 0;
	}

	else return -1;
}

int collision(SDL_Rect* car, SDL_Rect* barrier) {

	if (
		(car->y+SPRITE_HEIGHT > barrier->y) && (car->y < (barrier->y+80))
		) {
		if (barrier->x == (car->x+SPRITE_WIDTH))
			return 0;
		else return -1;
	}
	return -1;

}