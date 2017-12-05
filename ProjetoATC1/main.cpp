
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <SDL.h>
#include <time.h>
#include <stdlib.h>
#include<SDL_image.h>


#define VETOR 900
#define max_vel  16
#define min_vel 4
#define vel_pos_acidente 5

#define	 tempo_verde  5000
#define	tempo_amarelo 2000
#define	tempo_vermelho  8000

// the window we'll be rendering to
SDL_Window* gWindow = NULL;
// the surface contained by the window
SDL_Renderer* gRenderer = NULL;
// the image we will load and show on the screen
SDL_Texture* gXOut = NULL;
SDL_Texture* gCar = NULL;
SDL_Texture* gbombeiros = NULL;
SDL_Texture* gcamiao = NULL;
SDL_Texture* gtaxi = NULL;
SDL_Texture* gsemaforo_verde = NULL;
SDL_Texture* gsemaforo_vermelho = NULL;
SDL_Texture* gsemaforo_amarelo = NULL;

SDL_Texture* gacidente = NULL;
SDL_Texture* groadc = NULL;


struct  carro
{

	int velocidadeX;
	int velocidadeY;
	int numero;
	int x;
	int y;
	int aux; //1=anda da direita pa esquerda, 2=anda da esquerda pa direita, 3= anda de cima pa baixo, 4=anda de baixo pa cima
	int dim_yf;
	int dim_xf;

};


struct semaforo {
	int vermelho ;
	int verde;
	int amarelo;
	int	cor;
	int numero;
	int x;
	int y;
};

struct timer
{
	int tempo_ini;
	int tempo_atual;
	int temporizador;
};
typedef enum {
	FALSE,
	TRUE
}bool_t;

void coloca_semaforo(struct semaforo sem,/* SDL_Surface *gsemaforo,*/ int vertical);
void tamanho_semaforo(int desx_sem, int desy_sem, int origemw, int origwmh, SDL_Texture *gsemaforo);
int map[900][900];
int map_aux[900][900];

void inicia_vetor();
void coloca_carro(struct carro car);
void mexe_map(struct carro car1);
void gere_carro(int num, int vx, int vy, int x, int y, struct carro *car, int dim_x, int dim_y);
void find_car_xy(struct carro *car);
void render_car(struct carro car, SDL_Texture *gCar);
int velRand();
void update_speed(struct carro *car);
int get_time(void);
void temporizador(struct timer *temp);
void semaforo_mecanica(struct timer *temp, struct semaforo *semaf, int *contador);
int verifica_distancia(struct carro car, struct semaforo sem);
void pos_semaforo(struct semaforo *sem, int x, int y, int num);
void para_carro(struct carro *car);
bool_t para_carro_semaforo(struct carro *car, struct semaforo *sem);
bool_t car_matriz(struct carro car, struct carro car1);
void local_acidente(struct carro car, SDL_Texture *gacidente);
void verifica_acidente(struct carro *car, struct carro *car1,SDL_Texture *gacidente);





int init()
{
	int imgFlags = IMG_INIT_PNG;
	// initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		return 0;
	}
	if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
		printf("Warning: Linear texture filtering not enabled!\n");
	// create window
	gWindow = SDL_CreateWindow("Simulaçao Semaforo", SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		VETOR - 25, VETOR + 40, SDL_WINDOW_SHOWN);
	if (gWindow == NULL) {
		printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
		return 0;
	}
	// get window surface
	gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
	if (gRenderer == NULL) {
		printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
		return 0;
	}
	SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
	
	if (!(IMG_Init(imgFlags) & imgFlags)) {
		printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
		return 0;
	}
	return 1;
}

SDL_Texture* loadTexture(const char *path)
{
	// the final texture
	SDL_Texture* newTexture = NULL;
	// load image at specified path
	SDL_Surface* loadedSurface = IMG_Load(path);
	if (loadedSurface == NULL) {
		printf("Unable to load image %s! SDL_image Error: %s\n", path, IMG_GetError());
		return NULL;
	}
	// create texture from surface pixels
	newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
	if (newTexture == NULL) {
		printf("Unable to create texture from %s! SDL Error: %s\n", path, SDL_GetError());
		return NULL;
	}
	// get rid of old loaded surface
	SDL_FreeSurface(loadedSurface);
	return newTexture;
}


int loadMedia(){
	// load splash image
	/*gXOut = SDL_LoadBMP("assets/road.bmp");
	if (gXOut == NULL) {
		printf("Unable to load image. SDL Error: %s\n", SDL_GetError());
		return 0;
	}*/
	gXOut = loadTexture("assets/road.png");
	if (gXOut== NULL) {
		printf("Failed to load texture image assets/road.png !\n");
		return 0;
	}

	
	gCar = loadTexture("assets/popo.png");
	if (gCar == NULL) {
		printf("Failed to load texture image assets/popo.png!\n");
		return 0;
	}
	gbombeiros = loadTexture("assets/bombeiros.png");
	if (gbombeiros == NULL) {
		printf("Failed to load texture image assets/bombeiros.png!\n");
		return 0;
	}
	gcamiao = loadTexture("assets/camiao.png");
	if (gcamiao == NULL) {
		printf("Failed to load texture image assets/camiao.png!\n");
		return 0;
	}
	gtaxi = loadTexture("assets/taxi.png");
	if (gtaxi == NULL) {
		printf("Failed to load texture image assets/taxi.png!\n");
		return 0;
	}

	gsemaforo_verde = loadTexture("assets/semaforoverde.bmp");
	if (gsemaforo_verde == NULL) {
		printf("Failed to load texture image assets/semaforoverde.bmp!\n");
		return 0;
	}

	gsemaforo_amarelo = loadTexture("assets/semaforoamarelo.bmp");
	if (gsemaforo_amarelo == NULL) {
		printf("Failed to load texture image assets/semaforoamarelo.bmp!\n");
		return 0;
	}

	gsemaforo_vermelho = loadTexture("assets/semaforovermelho.bmp");
	if (gsemaforo_vermelho == NULL) {
		printf("Failed to load texture image assets/semaforovermelho.bmp!\n");
		return 0;
	}
	gacidente = loadTexture("assets/acidente.png");
	if (gacidente == NULL) {
		printf("Failed to load texture image assets/acidente.png!\n");
		return 0;
	}
	groadc= loadTexture("assets/roadc.png");
	if (groadc == NULL) {
		printf("Failed to load texture image assets/roadc.png!\n");
		return 0;
	}


	return 1;
}
void clean()
{
	// deallocate surface
	SDL_DestroyTexture(gXOut);
	gXOut = NULL;
	// destroy window
	SDL_DestroyTexture(gCar);
	gCar = NULL;

	SDL_DestroyTexture(gbombeiros);
	gbombeiros = NULL;

	SDL_DestroyTexture(gcamiao);
	gcamiao = NULL;

	SDL_DestroyTexture(gtaxi);
	gtaxi = NULL;

	SDL_DestroyTexture(gsemaforo_verde);
	gsemaforo_verde = NULL;

	SDL_DestroyTexture(gsemaforo_vermelho);
	gsemaforo_vermelho = NULL;

	SDL_DestroyTexture(gsemaforo_amarelo);
	gsemaforo_amarelo = NULL;

	SDL_DestroyTexture(groadc);
	groadc = NULL;

	SDL_DestroyTexture(gacidente);
	gacidente = NULL;


	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;
	// quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}
int main(int argc, char* args[])
{
	unsigned int auxiliar = clock();
	
	struct carro popo1, bombeiros, camiao,taxi;
	struct semaforo sem,sem1,sem2,sem3;
	struct timer tempo1,tempo2;

	srand(auxiliar);
	int cont = 3;
	int cont1 = 0;
	inicia_vetor();

	gere_carro(1, 0, -10, 422, VETOR, &popo1,58,91);
	gere_carro(2, 0, 2, 340, 0, &bombeiros,57,115);
	gere_carro(3, -4, 0, 800, 420, &camiao,104,47);
	gere_carro(4, 18, 0, 800, 500, &taxi,94,47);

	coloca_carro(popo1);
	coloca_carro(bombeiros);
	coloca_carro(camiao);
	coloca_carro(taxi);
	pos_semaforo(&sem, 400, 300, 90);
	pos_semaforo(&sem1, 400, 500, 91);
	pos_semaforo(&sem2, 300, 400, 92);
	pos_semaforo(&sem3, 500, 400, 93);
	

	// start up SDL and create window
	if (init()) {
		// load media
		if (loadMedia()) {
			// main loop flag
			int quit = 0;
			// event handler
			SDL_Event e;
			// while application is running
			while (!quit)
			{
				// handle events on queue
				while (SDL_PollEvent(&e) != 0)
				{
					// user requests quit
					if (e.type == SDL_QUIT)
					{
						quit = 1;
					}
				}


				// apply the image
				SDL_RenderClear(gRenderer);
				SDL_RenderCopy(gRenderer, gXOut, NULL, NULL);



				find_car_xy(&popo1);
				render_car(popo1, gCar);

				update_speed(&popo1);
				mexe_map(popo1);



				find_car_xy(&bombeiros);
				render_car(bombeiros, gbombeiros);
				update_speed(&bombeiros);
				mexe_map(bombeiros);



				find_car_xy(&camiao);
				render_car(camiao, gcamiao);

				update_speed(&camiao);
				mexe_map(camiao);

				find_car_xy(&taxi);
				render_car(taxi, gtaxi);
				update_speed(&taxi);
				
				mexe_map(taxi);
				find_car_xy(&taxi);


				SDL_RenderCopy(gRenderer, groadc, NULL, NULL);


				semaforo_mecanica(&tempo1, &sem, &cont);
				sem1.amarelo = sem.amarelo;
				sem1.vermelho = sem.vermelho;
				sem1.verde = sem.verde;
				

				//faz_semaforo2(&tempo2, &sem2, &cont);
			    semaforo_mecanica(&tempo2, &sem2, &cont1);
				sem3.amarelo = sem2.amarelo;
				sem3.vermelho = sem2.vermelho;
				sem3.verde = sem2.verde;
				
				verifica_acidente(&popo1, &camiao, gacidente);
				verifica_acidente(&popo1, &taxi, gacidente);
				verifica_acidente(&camiao, &popo1, gacidente);
				verifica_acidente(&camiao, &bombeiros, gacidente);
				verifica_acidente(&bombeiros, &camiao, gacidente);
				verifica_acidente(&bombeiros, &taxi, gacidente);
				verifica_acidente(&taxi, &bombeiros, gacidente);
				verifica_acidente(&taxi, &popo1, gacidente);

				coloca_semaforo(sem, 1);
				coloca_semaforo(sem1,  1);
				coloca_semaforo(sem2,  1);
				coloca_semaforo(sem3, 1);
				if (para_carro_semaforo(&taxi,&sem2))
				{
					para_carro(&taxi);
				}
				if (para_carro_semaforo(&camiao, &sem3))
				{
					para_carro(&camiao);
				}
				if (para_carro_semaforo(&popo1, &sem1))
				{
					para_carro(&popo1);
				}
				if (para_carro_semaforo(&bombeiros, &sem))
				{
					para_carro(&bombeiros);
				}
				


				// update the surface
				SDL_RenderPresent(gRenderer);
	 		SDL_Delay(1);
			}
		}
	}
	// free resources and close SDL
	clean();
	return 0;
}
void inicia_vetor()
{
	int i;
	int j;
	for ( i = 0; i<VETOR; i++)
		for ( j = 0; j < VETOR; j++)
		{
			map[i][j] = 0;
		}


}
void mexe_map(struct carro car1)
{
	int i;
	int j;

	for (i = 0; i <VETOR; i++)
		for (j = 0; j<VETOR; j++)
		{

			if (map[i][j] == car1.numero)
			{

				map[i][j] = 0;


				if (i > VETOR - max_vel) { i = 0; };

				if (car1.velocidadeY == 0 && car1.velocidadeX < 0 &&i<max_vel)i = VETOR-max_vel;

				map[i + car1.velocidadeX][j + car1.velocidadeY] = car1.numero;








				return;
			}


		}

}
void gere_carro(int num, int vx, int vy, int x, int y, struct carro *car, int dim_x, int dim_y)
{
	(*car).numero = num;
	(*car).velocidadeX = vx;
	(*car).velocidadeY = vy;
	(*car).x = x;
	(*car).y = y;
	(*car).dim_xf = dim_x;
	(*car).dim_yf = dim_y;

}
void find_car_xy(struct carro *car)
{
	int i;
	int j;
	for ( i = 0; i<VETOR; i++)
		for (j = 0; j < VETOR; j++)
		{
			if (map[i][j] == (*car).numero)
			{
				(*car).x = i;
				(*car).y = j;
			}
		}
}
void coloca_carro(struct carro car)
{
	map[car.x][car.y] = car.numero;

}
void render_car(struct carro car, SDL_Texture *gCar)
{
	SDL_Rect origem;
	SDL_Rect destino;
	origem.x = 0;
	origem.y = 0;
	origem.w = car.dim_xf;
	origem.h = car.dim_yf;

	

	destino.x = car.x;
	destino.y = car.y;
	destino.w = car.dim_xf;
	destino.h = car.dim_yf;

	SDL_RenderCopy(gRenderer, gCar, &origem, &destino);


}
int velRand()
{
	int a;
	do
	{
		a = rand() % max_vel;
		if (a <= min_vel)a = min_vel;

	} while (a == 0 || a > max_vel);
	return a;
}
void update_speed(struct carro *car)
{
	int i;
	int j;
	for (i = 0; i < VETOR; i++)
		for (j = 0; j < VETOR; j++)
		{
			if (map[i][j] == (*car).numero && (i > VETOR - max_vel || j > VETOR - max_vel) && ((*car).velocidadeX>0 || (*car).velocidadeY>0))
			{

				if ((*car).velocidadeX == 0)
				{
					(*car).velocidadeY = velRand();
					return;
				}
				if ((*car).velocidadeY == 0)

				{
					(*car).velocidadeX = velRand();
					return;

				}

			}if (map[i][j] == (*car).numero && (i < max_vel || j < max_vel) && ((*car).velocidadeX < 0 || (*car).velocidadeY < 0))
			{
				if ((*car).velocidadeX == 0)
				{
					(*car).velocidadeY = -velRand();
					return;
				}
				else
				{
					(*car).velocidadeX = -velRand();
					return;

				}
			}
			if (map[i][j] == (*car).numero && (i < max_vel || j < max_vel) && ((*car).velocidadeX < 0 && (*car).velocidadeY == 0))
			{
				
					(*car).velocidadeX = -velRand();
					return;

			}
		}

}
void tamanho_semaforo(int desx_sem, int desy_sem, int origemw, int origwmh, SDL_Texture *gsemaforo) {
	SDL_Rect origem;
	SDL_Rect destino;
	origem.x = 0;
	origem.y = 0;
	origem.w = origemw;
	origem.h = origwmh;

	
	destino.x = desx_sem;
	destino.y = desy_sem;
	destino.w = origemw;
	destino.h = origwmh;

	SDL_RenderCopy(gRenderer, gsemaforo, &origem, &destino);
	
}

void coloca_semaforo(struct semaforo sem, /*SDL_Surface *gsemaforo, */  int vertical) {

	if (vertical == 1) {
		if (sem.vermelho == 1) {
			tamanho_semaforo(sem.x, sem.y, 34, 71, gsemaforo_vermelho);
		}

		if (sem.amarelo == 1) {
			tamanho_semaforo(sem.x, sem.y, 34, 71, gsemaforo_amarelo);
		}

		if (sem.verde == 1) {
			tamanho_semaforo(sem.x, sem.y, 34, 71, gsemaforo_verde);
		}
	}

	
}

int get_time(void) {
	int tempo;
	tempo = clock();
	return tempo;
}

void temporizador(struct timer *temp)
{
	(*temp).temporizador = (*temp).tempo_atual - (*temp).tempo_ini;
	
}

void semaforo_mecanica(struct timer *temp, struct semaforo *semaf,int *contador)
{
	
	(*temp).tempo_atual = get_time();
		temporizador(temp);

	if (*contador == 1 && (*temp).temporizador >= 1000)
	{
		(*temp).tempo_ini = get_time();
		(*semaf).verde = 1;
		(*semaf).amarelo = 0;
		(*semaf).vermelho = 0;
		(*contador)++;
		(*temp).tempo_ini = get_time();
		return;
	}
	
		
		if (*contador == 2 && (*temp).temporizador >= tempo_verde)
		{
			(*semaf).verde = 0;
			(*semaf).amarelo = 1;
			(*semaf).vermelho = 0;
			(*contador)++;
			(*temp).tempo_ini = get_time();
			return;
		} 
		if (*contador==3 && (*temp).temporizador >= tempo_amarelo)
		{
			(*semaf).verde = 0;
			(*semaf).amarelo = 0;
			(*semaf).vermelho = 1;
			(*contador)=0;
			(*temp).tempo_ini = get_time();
		
			return;
		}
		if (*contador == 0 && (*temp).temporizador >= tempo_vermelho) {
			(*semaf).verde = 0;
			(*semaf).amarelo = 0;
			(*semaf).vermelho = 1;
			(*contador)++;
			(*temp).tempo_ini = get_time();
			return;


		}
	}


int verifica_distancia(struct carro car, struct semaforo sem)
{
	int posx;
	int posy;
	
	int distancia;
	if (car.velocidadeX != 0 && car.velocidadeY==0)
	{
		if (car.velocidadeX < 0)
		{

			posx = car.x - car.dim_xf+70;
			
			distancia = posx - sem.x;
			if (distancia < 0)return -1;
			else return distancia;

		}
		if (car.velocidadeX > 0)
		{
			posx = car.x + car.dim_xf;
			
			distancia = sem.x -posx;
			if (distancia < 0)return -1;
			else return distancia;
		}
		else return 1;
	}
	if (car.velocidadeY != 0 && car.velocidadeX == 0)
	{
		if (car.velocidadeY < 0)
		{
			
			posy = car.y - car.dim_yf;
			distancia = posy - sem.y;
			if (distancia < 0)return -1;
			else return distancia;

		}
		if (car.velocidadeY > 0)
		{
		
			posy = car.y + car.dim_yf;
			distancia = sem.y - posy+50;
			if (distancia < 0)return -1;
			else return distancia;
		}
		else return -1;
	}
	return 0;
	
}

void pos_semaforo(struct semaforo *sem, int x, int y, int num)
{
	(*sem).x = x;
	(*sem).y = y;
	map[x][y] = num;

}

void para_carro(struct carro *car)
{
	if ((*car).velocidadeX != 0 && (*car).velocidadeY == 0)
	{
		if ((*car).velocidadeX < 0)
		{
			(*car).aux = 1; //1=anda da direita pa esquerda
			(*car).velocidadeX = 0;
			(*car).velocidadeY = 0;
			return;
		}
		if ((*car).velocidadeX > 0)
		{
			(*car).aux = 2; //2=anda da esquerda pa direita
			(*car).velocidadeX = 0;
			(*car).velocidadeY = 0;
			return;
		}
	}
	if ((*car).velocidadeY != 0 && (*car).velocidadeX == 0)
	{
		if ((*car).velocidadeY < 0)
		{
			(*car).aux = 4; //4=anda de baixo pa cima
			(*car).velocidadeX = 0;
			(*car).velocidadeY = 0;
			return;
		}
		if ((*car).velocidadeY > 0)
		{
			(*car).aux = 3; //3=anda de cima pa baixo
			(*car).velocidadeX = 0;
			(*car).velocidadeY = 0;
			return;
		}
	}

}

bool_t para_carro_semaforo(struct carro *car, struct semaforo *sem)
{
	int a = verifica_distancia(*car, *sem);
	if (verifica_distancia(*car, *sem) < 150 && verifica_distancia(*car, *sem) > 50 && (*sem).amarelo == 1 && (*car).velocidadeY==0)
	{
		if((*car).velocidadeX>3)
		(*car).velocidadeX = (*car).velocidadeX-1;
		if ((*car).velocidadeX<0 &&car->velocidadeX<-3)
			(*car).velocidadeX = (*car).velocidadeX + 1;
		return FALSE;
	}
	if (verifica_distancia(*car, *sem) < 6 && verifica_distancia(*car, *sem) > -1 && (*sem).amarelo == 1 && (*car).velocidadeY == 0)
	{
		if ((*car).velocidadeX <= 3 && car->velocidadeX>0)
			return FALSE;
		if ((*car).velocidadeX<0 && car->velocidadeX>-3)
			return TRUE;
		return FALSE;
	}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	if (verifica_distancia(*car, *sem) <150  && verifica_distancia(*car, *sem) > 50 && (*sem).vermelho == 1 && (*car).velocidadeY == 0)
	{
		if ((*car).velocidadeX>3)
			(*car).velocidadeX = (*car).velocidadeX - 1;


		if ((*car).velocidadeX<0 && car->velocidadeX<-3)
			(*car).velocidadeX = (*car).velocidadeX + 1;
		return FALSE;
	}
	if (verifica_distancia(*car, *sem) < 5 && verifica_distancia(*car, *sem) > -1 && (*sem).vermelho == 1 && (*car).velocidadeY == 0)
	{
		if ((*car).velocidadeX <= 5 && car->velocidadeX>0)
			return TRUE;
		if ((*car).velocidadeX<0 && car->velocidadeX>-5)
			return TRUE;
		return FALSE;
	}
	
	if (verifica_distancia(*car, *sem) == -1)
	{
		return FALSE;
	}

	if (verifica_distancia(*car, *sem) < 150 && verifica_distancia(*car, *sem) > 50 && (*sem).amarelo == 1 && (*car).velocidadeX == 0)
	{
		if ((*car).velocidadeY > 3)
		(*car).velocidadeY = ((*car).velocidadeY) - 1;
		if ((*car).velocidadeY < 0 && car->velocidadeY < -3)
			(*car).velocidadeY = ((*car).velocidadeY) + 1;
		return FALSE;
	}

	if (verifica_distancia(*car, *sem) < 6 && verifica_distancia(*car, *sem) > -1 && (*sem).amarelo == 1 && (*car).velocidadeX == 0)
	{
		if ((*car).velocidadeY <= 3 && car->velocidadeY > 0)
		return TRUE;
		if ((*car).velocidadeY < 0 && car->velocidadeY > -3)
			return TRUE;
		return FALSE;
	}

	if (verifica_distancia(*car, *sem) < 150 && verifica_distancia(*car, *sem) > 50 && (*sem).vermelho == 1 && (*car).velocidadeX == 0)
	{
		if ((*car).velocidadeY > 3)
		(*car).velocidadeY = ((*car).velocidadeY) - 1;
		if ((*car).velocidadeY < 0 && car->velocidadeY < -3)
			(*car).velocidadeY = ((*car).velocidadeY) + 1;
		return FALSE;
	}
	if (verifica_distancia(*car, *sem) < 6 && verifica_distancia(*car, *sem) > -1 && (*sem).vermelho == 1 && (*car).velocidadeX == 0)
	{
		if ((*car).velocidadeY <= 5 && car->velocidadeY >0)
		return TRUE;
		if ((*car).velocidadeY < 0 && car->velocidadeY > -5)
			return TRUE;
		return FALSE;
	}


	///////////

	if ((*sem).verde == 1 && (*car).aux==2 &&(*car).velocidadeX==0 && (*car).velocidadeY == 0 )
	{
		car->velocidadeX = vel_pos_acidente;
		car->aux = 0;
		return FALSE;
	}
	if ((*sem).verde == 1 && (*car).aux == 1 && (*car).velocidadeX == 0 && (*car).velocidadeY == 0)
	{
		car->velocidadeX = -vel_pos_acidente;
		car->aux = 0;
		return FALSE;
	}
	
	if ((*sem).verde == 1 && (*car).aux == 3 && (*car).velocidadeX == 0 && (*car).velocidadeY == 0)
	{
		car->velocidadeY = vel_pos_acidente;
		car->aux = 0;
		return FALSE;
	}
	if ((*sem).verde == 1 && (*car).aux == 4 && (*car).velocidadeX == 0 && (*car).velocidadeY == 0)
	{
		car->velocidadeY = -vel_pos_acidente;
		car->aux = 0;
		return FALSE;
	}

	else return FALSE;
}
bool_t car_matriz(struct carro car, struct carro car1)
{

	//int map_aux[900][900];
	int i;
	int j;
	int k;
	int l;
	for (i = car.x; i < (car.x + car.dim_xf); i++)
		for (j = car.y; j < (car.y + car.dim_yf); j++) {
			map_aux[i][j] = car.numero;
		}


	for (i = car1.x; i < (car1.x + car1.dim_xf); i++)
		for (j = car1.y; j < (car1.y + car1.dim_yf); j++) {
			map_aux[i][j] = car1.numero;
		}

	for ( i = car.x; i < (car.x + car.dim_xf); i++)
		for (j = car.y; j < (car.y + car.dim_yf); j++) {

			if (map_aux[i][j] != car.numero) {
				for (k = 0; k < VETOR; k++)
					for ( l = 0; l < VETOR; l++)
						map_aux[k][l] = 0;
				return TRUE;
			}
		}

	for (k = 0; k < VETOR; k++)
		for (l = 0; l < VETOR; l++)map_aux[k][l] = 0;
	return FALSE;

}

void local_acidente(struct carro car, SDL_Texture *gacidente) {

	SDL_Rect origem;
	SDL_Rect destino;
	origem.x = 0;
	origem.y = 0;
	origem.w = 100;
	origem.h = 60;

	

	destino.x = car.x;
	destino.y = car.y;
	destino.w = 100;
	destino.h = 60;

	SDL_RenderCopy(gRenderer, gacidente, &origem, &destino);
}

void verifica_acidente(struct carro *car, struct carro *car1, SDL_Texture *gacidente) {

	if (car_matriz(*car, *car1)) {

		local_acidente(*car, gacidente);

		(*car).velocidadeX = 0;
		(*car).velocidadeY = 0;
		(*car1).velocidadeX = 0;
		(*car1).velocidadeY = 0;

	}// teste exemplo
	//puika
	//teste 2

	//oi
}



