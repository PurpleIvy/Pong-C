#include <SDL3/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

#define PSTARTPOSX 100.f
#define PSTARTPOSY 200.f
#define PHEIGHT 100.f
#define PWIDTH 10.f
#define HEIGHT 480
#define WIDTH 640
#define BALLSTARTX WIDTH/2
#define BALLSTARTY HEIGHT/2
#define STARTVELX 0.05f
#define STARTVELY 0.01f

typedef struct Player
{
	SDL_FRect rect;
	int score;
} Player;

Player p1;
Player p2;

SDL_Window *Window;
SDL_Renderer *Renderer;
SDL_Event event;

static void cleanup(int signal){

		SDL_DestroyRenderer(Renderer);
		SDL_DestroyWindow(Window);
		SDL_Quit();
		exit(signal);
}

bool check_collision(SDL_FRect a, SDL_FRect b)
{
	SDL_FPoint ball;

	ball.x = a.x;
	ball.y = a.y;

	if (SDL_PointInRectFloat(&ball, &b)) return true;

	else return false;

}


int main(){

	srand(time(NULL));

	bool finished = false;

	uint64_t nowtime = SDL_GetPerformanceCounter();
	uint64_t lasttime = 0;
	double deltatime = 0;

	float ballvelx = STARTVELX;
	float ballvely = STARTVELY;

	p1.rect.x = PSTARTPOSX;
	p1.rect.y = PSTARTPOSY;
	p1.rect.w = PWIDTH;
	p1.rect.h = PHEIGHT;

	p2.rect.x = WIDTH - PSTARTPOSX;
	p2.rect.y = HEIGHT - PSTARTPOSY;
	p2.rect.h = 100.f;
	p2.rect.w = 10.f;
	
	SDL_FRect ball;
	ball.x = BALLSTARTX;
	ball.y = BALLSTARTY;
	ball.h = 10.f;
	ball.w = 10.f;


	SDL_CreateWindowAndRenderer(WIDTH, HEIGHT, 0, &Window, &Renderer);
	SDL_SetRenderDrawColor(Renderer, 0, 0, 0, 255);
	SDL_RenderClear(Renderer);
	SDL_RenderPresent(Renderer);

	while (1){
	lasttime = nowtime;
	nowtime = SDL_GetPerformanceCounter();

	deltatime = (double)((nowtime - lasttime)*100 / (double)SDL_GetPerformanceFrequency());

		SDL_PollEvent(&event);
		switch (event.key.keysym.sym)
		{
			case SDLK_w: 	p1.rect.y -= 200.0f * deltatime; break;
			case SDLK_s: 	p1.rect.y += 200.0f * deltatime; break;
			case SDLK_UP:   p2.rect.y -= 200.0f * deltatime; break;
			case SDLK_DOWN: p2.rect.y += 200.0f * deltatime; break;
			if (finished) { 	    case SDLK_q: cleanup(0); break; }
		}
		
		switch(event.type){
			case SDL_EVENT_QUIT:
			case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
				cleanup(0);
				break;
		}


		if (p1.rect.y > HEIGHT - p1.rect.h) p1.rect.y = HEIGHT - p1.rect.h - 10;
		if (p2.rect.y > HEIGHT - p2.rect.h) p2.rect.y = HEIGHT - p2.rect.h - 10;
		if (p1.rect.y < 0) p1.rect.y = 10;
		if (p2.rect.y < 0) p2.rect.y = 10;

		float velval = rand()%3 + 1;

		float xvel = velval * deltatime;
		float yvel = velval * deltatime;

		if (check_collision(ball, p2.rect))  { ballvelx = -xvel; }
		if (check_collision(ball, p1.rect))  { ballvelx = xvel;  }

		if (ball.y >= HEIGHT - ball.h) { ballvely = -yvel; ball.y = HEIGHT - ball.h;}
		if (ball.y <= 0) { ballvely = yvel; ball.y = ball.h;}

		//printf("ballvelx %f\n", ballvelx);
		//printf("ballvely %f\n", ballvely);
		ball.x += ballvelx;
		ball.y += ballvely;

		float a = rand()%3 + 1;
		float b = rand()%3 + 1;
		float c = rand()%3 + 1;
		float d = rand()%3 + 1;

		if (ball.x >= WIDTH - ball.w) { p1.score++; ballvelx = -a * deltatime; ballvely = b * deltatime; ball.x = BALLSTARTX; ball.y = BALLSTARTY; }
		if (ball.x <= 0) { p2.score++; ballvelx = c * deltatime; ballvely = -d * deltatime; ball.x = BALLSTARTX; ball.y = BALLSTARTY; }

		SDL_SetRenderDrawColor(Renderer, 255, 255, 255, 255);
	//	SDL_RenderLine(Renderer, 0, 0, 100, 100);
		
		if (p1.score < 11 || p2.score < 11)	
		{	
			SDL_RenderFillRect(Renderer, &p1.rect);
			SDL_RenderFillRect(Renderer, &p2.rect);
			SDL_RenderFillRect(Renderer, &ball);
		}
		
		
		SDL_RenderPresent(Renderer);
		SDL_SetRenderDrawColor(Renderer, 0, 0, 0, 255);
		SDL_RenderClear(Renderer);
//		printf("ball.x %f\n", ball.x);
//		printf("ball.y %f\n", ball.y);
		printf("score = %d:%d\n", p1.score, p2.score);

		if (p1.score == 11 && p2.score < 11) { printf("Congratulations, you won the game. Press Q to exit\n"); finished = true; }
		if (p2.score == 11 && p1.score < 11) { printf("You lost the game, better luck next time. Press Q to exit\n"); finished = true; }
	}


	return 0;


}
