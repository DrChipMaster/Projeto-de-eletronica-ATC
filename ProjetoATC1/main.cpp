
#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif
#include <stdio.h>
#include <SDL.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include<SDL_image.h>


#define VETOR 900
#define max_vel  16
#define min_vel 4
#define vel_pos_acidente 5

#define	 tempo_verde  5000
#define	tempo_amarelo 2000
#define	tempo_vermelho  8000
SDL_Renderer* gRenderer2 = NULL;

typedef struct textura_s
{
	int dim_yf;
	int dim_xf;
	SDL_Texture *gCarro;
	char path[30];

}textura_t;

 typedef  struct  carro_s
{

	int velocidadeX;
	int velocidadeY;
	int numero;
	int x;
	int y;
	int aux; //1=anda da direita pa esquerda, 2=anda da esquerda pa direita, 3= anda de cima pa baixo, 4=anda de baixo pa cima
	textura_t textura_carro;

}carro_t;


typedef struct semaforo_s {
	int vermelho ;
	int verde;
	int amarelo;
	int	cor;
	int numero;
	int x;
	int y;
}semaforo_t;

typedef struct tempo_s
{
	int tempo_ini;
	int tempo_atual;
	int temporizador;
}tempo_t ;
typedef enum {
	FALSE,
	TRUE
}bool_t;


void coloca_semaforo(semaforo_t sem, int vertical, SDL_Texture *sem_verde, SDL_Texture *sem_amarelo, SDL_Texture *sem_vermelho, SDL_Renderer *gRenderer);
void tamanho_semaforo(int desx_sem, int desy_sem, int origemw, int origwmh, SDL_Texture *gsemaforo,SDL_Renderer *gRenderer);
int map[900][900];
int map_aux[900][900];

void inicia_vetor();
void coloca_carro(carro_t carro);
void mexe_map(carro_t carro1);
void gere_carro(int num, int vx, int vy, int x, int y,carro_t *carro, int dim_x, int dim_y,char path[30]);
void find_car_xy(carro_t *car);
void render_car(carro_t *carro, SDL_Renderer *gRenderer);
int velRand();
void update_speed(carro_t *carro);
int get_time(void);
void temporizador( tempo_t *tempo);
void semaforo_mecanica( tempo_t *tempo,semaforo_t *semaf, int *contador);
int verifica_distancia(carro_t carro,semaforo_t sem);
void pos_semaforo(semaforo_t *sem, int x, int y, int num);
void para_carro(carro_t *carro);
bool_t para_carro_semaforo(carro_t *carro,semaforo_t *sem);
bool_t car_matriz(carro_t carro, carro_t carro1);
void local_acidente(carro_t carro, SDL_Texture *gacidente, SDL_Renderer *gRenderer);
void verifica_acidente(carro_t *carro, carro_t *carro1 ,SDL_Texture *gacidente, SDL_Renderer *gRenderer);
bool_t inicializa_toda_textura(textura_t *textura1, textura_t *textura2, textura_t *textura3, textura_t *textura4, textura_t *textura5, textura_t *textura6, textura_t *textura7, textura_t *textura8, textura_t *textura9, textura_t *textura10, SDL_Renderer *gRenderer);





int init(SDL_Window * gWindow,SDL_Renderer *gRenderer)
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
	gRenderer2 = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
	if (gRenderer2 == NULL) {
		printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
		return 0;
	}
	SDL_SetRenderDrawColor(gRenderer2, 0xFF, 0xFF, 0xFF, 0xFF);
	
	if (!(IMG_Init(imgFlags) & imgFlags)) {
		printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
		return 0;
	}
	return 1;
}

SDL_Texture* loadTexture(const char *path ,SDL_Renderer* gRenderer)
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
	newTexture = SDL_CreateTextureFromSurface(gRenderer2, loadedSurface);
	if (newTexture == NULL) {
		printf("Unable to create texture from %s! SDL Error: %s\n", path, SDL_GetError());
		return NULL;
	}
	// get rid of old loaded surface
	SDL_FreeSurface(loadedSurface);
	return newTexture;
}


bool_t loadMedia(textura_t *textura,SDL_Renderer *gRenderer){
		textura->gCarro = loadTexture(textura->path,gRenderer);
	if (textura->gCarro== NULL) {
		return FALSE;
	}
	else
	return TRUE;
}
void clean()
{
	// deallocate surface
	//SDL_DestroyTexture(gXOut);
	//gXOut = NULL;
	//// destroy window
	//SDL_DestroyTexture(gCar);
	//gCar = NULL;

	//SDL_DestroyTexture(gbombeiros);
	//gbombeiros = NULL;

	//SDL_DestroyTexture(gcamiao);
	//gcamiao = NULL;

	//SDL_DestroyTexture(gtaxi);
	//gtaxi = NULL;

	//SDL_DestroyTexture(gsemaforo_verde);
	//gsemaforo_verde = NULL;

	//SDL_DestroyTexture(gsemaforo_vermelho);
	//gsemaforo_vermelho = NULL;

	//SDL_DestroyTexture(gsemaforo_amarelo);
	//gsemaforo_amarelo = NULL;

	//SDL_DestroyTexture(groadc);
	//groadc = NULL;

	//SDL_DestroyTexture(gacidente);
	//gacidente = NULL;


	//SDL_DestroyRenderer(gRenderer);
	//SDL_DestroyWindow(gWindow);
	//gWindow = NULL;
	// quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}
int main(int argc, char* args[])
{
	unsigned int auxiliar = clock();
	
	carro_t carro1, bombeiros, camiao,taxi;
    semaforo_t sem,sem1,sem2,sem3;
    tempo_t tempo1,tempo2;
	textura_t estrada,semaforo_verde,semaforo_amarelo,semaforo_vermelho,acidente,estrada_fora;

	srand(auxiliar);
	int cont = 3;
	int cont1 = 0;


	SDL_Window* gWindow = NULL;
	// the surface contained by the window
	SDL_Renderer* gRenderer = NULL;
	// the image we will load and show on the screen
	/*SDL_Texture* gXOut = NULL;
	SDL_Texture* gCar = NULL;
	SDL_Texture* gbombeiros = NULL;
	SDL_Texture* gcamiao = NULL;
	SDL_Texture* gtaxi = NULL;
	SDL_Texture* gsemaforo_verde = NULL;
	SDL_Texture* gsemaforo_vermelho = NULL;
	SDL_Texture* gsemaforo_amarelo = NULL;

	SDL_Texture* gacidente = NULL;
	SDL_Texture* groadc = NULL;*/

	inicia_vetor();
	strcpy(estrada.path, "assets/road.bmp") ;
	strcpy(semaforo_verde.path, "assets/semaforoverde.bmp");
	strcpy(semaforo_amarelo.path, "assets/semaforoamarelo.bmp");
	strcpy(semaforo_vermelho.path, "assets/semaforovermelho.bmp");
	strcpy(acidente.path, "assets/acidente.png");
	strcpy(estrada_fora.path, "assets/roadc.png");


	gere_carro(1, 0, -10, 422, VETOR, &carro1,58,91,"assets/popo.png");
	gere_carro(2, 0, 2, 340, 0, &bombeiros,57,115,"assets/bombeiros.png");
	gere_carro(3, -4, 0, 800, 420, &camiao,104,47,"assets/camiao.png");
	gere_carro(4, 18, 0, 800, 500, &taxi,94,47,"assets/taxi.png");

	coloca_carro(carro1);
	coloca_carro(bombeiros);
	coloca_carro(camiao);
	coloca_carro(taxi);
	pos_semaforo(&sem, 400, 300, 90);
	pos_semaforo(&sem1, 400, 500, 91);
	pos_semaforo(&sem2, 300, 400, 92);
	pos_semaforo(&sem3, 500, 400, 93);
	

	// start up SDL and create window
	if (init(gWindow,gRenderer)) {
		// load media
		if (inicializa_toda_textura(&estrada,&acidente,&estrada_fora,&semaforo_amarelo,&semaforo_verde,&semaforo_vermelho,&carro1.textura_carro,&bombeiros.textura_carro,&taxi.textura_carro,&camiao.textura_carro,gRenderer)) {
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
				SDL_RenderClear(gRenderer2);
				SDL_RenderCopy(gRenderer2, estrada.gCarro, NULL, NULL);



				find_car_xy(&carro1);
				render_car(&carro1 , gRenderer);

				update_speed(&carro1);
				mexe_map(carro1);



				find_car_xy(&bombeiros);
				render_car(&bombeiros, gRenderer);
				update_speed(&bombeiros);
				mexe_map(bombeiros);



				find_car_xy(&camiao);
				render_car(&camiao, gRenderer);

				update_speed(&camiao);
				mexe_map(camiao);

				find_car_xy(&taxi);
				render_car(&taxi,  gRenderer);
				update_speed(&taxi);
				
				mexe_map(taxi);
				find_car_xy(&taxi);


				SDL_RenderCopy(gRenderer,estrada_fora.gCarro, NULL, NULL);


				semaforo_mecanica(&tempo1, &sem, &cont);
				sem1.amarelo = sem.amarelo;
				sem1.vermelho = sem.vermelho;
				sem1.verde = sem.verde;
				

				//faz_semaforo2(&tempo2, &sem2, &cont);
			    semaforo_mecanica(&tempo2, &sem2, &cont1);
				sem3.amarelo = sem2.amarelo;
				sem3.vermelho = sem2.vermelho;
				sem3.verde = sem2.verde;
				
				verifica_acidente(&carro1, &camiao,acidente.gCarro,gRenderer);
				verifica_acidente(&carro1, &taxi, acidente.gCarro, gRenderer);
				verifica_acidente(&camiao, &carro1, acidente.gCarro, gRenderer);
				verifica_acidente(&camiao, &bombeiros, acidente.gCarro, gRenderer);
				verifica_acidente(&bombeiros, &camiao, acidente.gCarro, gRenderer);
				verifica_acidente(&bombeiros, &taxi, acidente.gCarro, gRenderer);
				verifica_acidente(&taxi, &bombeiros, acidente.gCarro, gRenderer);
				verifica_acidente(&taxi, &carro1, acidente.gCarro, gRenderer);

				coloca_semaforo(sem, 1,semaforo_verde.gCarro, semaforo_amarelo.gCarro, semaforo_vermelho.gCarro,gRenderer);
				coloca_semaforo(sem1,  1, semaforo_verde.gCarro, semaforo_amarelo.gCarro, semaforo_vermelho.gCarro, gRenderer);
				coloca_semaforo(sem2,  1, semaforo_verde.gCarro, semaforo_amarelo.gCarro, semaforo_vermelho.gCarro, gRenderer);
				coloca_semaforo(sem3, 1, semaforo_verde.gCarro, semaforo_amarelo.gCarro, semaforo_vermelho.gCarro, gRenderer);
				if (para_carro_semaforo(&taxi,&sem2))
				{
					para_carro(&taxi);
				}
				if (para_carro_semaforo(&camiao, &sem3))
				{
					para_carro(&camiao);
				}
				if (para_carro_semaforo(&carro1, &sem1))
				{
					para_carro(&carro1);
				}
				if (para_carro_semaforo(&bombeiros, &sem))
				{
					para_carro(&bombeiros);
				}
				


				// update the surface
				SDL_RenderPresent(gRenderer2);
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
void mexe_map(carro_t carro1)
{
	int i;
	int j;

	for (i = 0; i <VETOR; i++)
		for (j = 0; j<VETOR; j++)
		{

			if (map[i][j] == carro1.numero)
			{

				map[i][j] = 0;


				if (i > VETOR - max_vel) { i = 0; };

				if (carro1.velocidadeY == 0 && carro1.velocidadeX < 0 &&i<max_vel)i = VETOR-max_vel;

				map[i + carro1.velocidadeX][j + carro1.velocidadeY] = carro1.numero;








				return;
			}


		}

}
void gere_carro(int num, int vx, int vy, int x, int y, carro_t *carro, int dim_x, int dim_y, char path[30])
{
	carro->numero = num;
	carro->velocidadeX = vx;
	carro->velocidadeY = vy;
	carro->x = x;
	carro->y = y;
	carro->textura_carro.dim_xf = dim_x;
	carro->textura_carro.dim_yf = dim_y;
	//carro->textura_carro.gCarro = gCarro;
	strcpy(carro->textura_carro.path, path);
	

}
void find_car_xy(carro_t *carro)
{
	int i;
	int j;
	for ( i = 0; i<VETOR; i++)
		for (j = 0; j < VETOR; j++)
		{
			if (map[i][j] == (*carro).numero)
			{
				(*carro).x = i;
				(*carro).y = j;
			}
		}
}
void coloca_carro(carro_t carro)
{
	map[carro.x][carro.y] = carro.numero;

}
void render_car(carro_t *carro,  SDL_Renderer *gRenderer)
{
	SDL_Rect origem;
	SDL_Rect destino;
	origem.x = 0;
	origem.y = 0;
	origem.w = carro->textura_carro.dim_xf;
	origem.h = carro->textura_carro.dim_yf;

	

	destino.x = carro->x;
	destino.y = carro->y;
	destino.w = carro->textura_carro.dim_xf;
	destino.h = carro->textura_carro.dim_yf;

	SDL_RenderCopy(gRenderer2, carro->textura_carro.gCarro, &origem, &destino);


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
void update_speed(carro_t *carro)
{
	int i;
	int j;
	for (i = 0; i < VETOR; i++)
		for (j = 0; j < VETOR; j++)
		{
			if (map[i][j] == carro->numero && (i > VETOR - max_vel || j > VETOR - max_vel) && (carro->velocidadeX>0 || carro->velocidadeY>0))
			{

				if (carro->velocidadeX == 0)
				{
					carro->velocidadeY = velRand();
					return;
				}
				if (carro->velocidadeY == 0)

				{
					carro->velocidadeX = velRand();
					return;

				}

			}if (map[i][j] == carro->numero && (i < max_vel || j < max_vel) && (carro->velocidadeX < 0 || carro->velocidadeY < 0))
			{
				if (carro->velocidadeX == 0)
				{
					carro->velocidadeY = -velRand();
					return;
				}
				else
				{
					carro->velocidadeX = -velRand();
					return;

				}
			}
			if (map[i][j] == carro->numero && (i < max_vel || j < max_vel) && (carro->velocidadeX < 0 && carro->velocidadeY == 0))
			{
				
					carro->velocidadeX = -velRand();
					return;

			}
		}

}
void tamanho_semaforo(int desx_sem, int desy_sem, int origemw, int origwmh, SDL_Texture *gsemaforo, SDL_Renderer *gRenderer) {
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

	SDL_RenderCopy(gRenderer2, gsemaforo, &origem, &destino);
	
}

void coloca_semaforo(semaforo_t sem, int vertical, SDL_Texture *sem_verde, SDL_Texture *sem_amarelo, SDL_Texture *sem_vermelho,SDL_Renderer *gRenderer)
{
	if (vertical == 1) {
		if (sem.vermelho == 1) {
			tamanho_semaforo(sem.x, sem.y, 34, 71, sem_vermelho,gRenderer);
		}

		if (sem.amarelo == 1) {
			tamanho_semaforo(sem.x, sem.y, 34, 71, sem_amarelo, gRenderer);
		}

		if (sem.verde == 1) {
			tamanho_semaforo(sem.x, sem.y, 34, 71, sem_verde, gRenderer);
		}
	}

	
}

int get_time(void) {
	int tempo;
	tempo = clock();
	return tempo;
}

void temporizador(tempo_t *tempo)
{
	tempo->temporizador = tempo->tempo_atual - tempo->tempo_ini;
	
}

void semaforo_mecanica(tempo_t *tempo,semaforo_t *semaf,int *contador)
{
	
	tempo->tempo_atual = get_time();
		temporizador(tempo);

	if (*contador == 1 && tempo->temporizador >= 1000)
	{
		tempo->tempo_ini = get_time();
		semaf->verde = 1;
		semaf->amarelo = 0;
		semaf->vermelho = 0;
		(*contador)++;
		tempo->tempo_ini = get_time();
		return;
	}
	
		
		if (*contador == 2 && tempo->temporizador >= tempo_verde)
		{
			semaf->verde = 0;
			semaf->amarelo = 1;
			semaf->vermelho = 0;
			(*contador)++;
			tempo->tempo_ini = get_time();
			return;
		} 
		if (*contador==3 && tempo->temporizador >= tempo_amarelo)
		{
			semaf->verde = 0;
			semaf->amarelo = 0;
			semaf->vermelho = 1;
			(*contador)=0;
			tempo->tempo_ini = get_time();
		
			return;
		}
		if (*contador == 0 && tempo->temporizador >= tempo_vermelho) {
			semaf->verde = 0;
			semaf->amarelo = 0;
			semaf->vermelho = 1;
			(*contador)++;
			tempo->tempo_ini = get_time();
			return;


		}
	}


int verifica_distancia(carro_t carro,  semaforo_t sem)
{
	int posx;
	int posy;
	
	int distancia;
	if (carro.velocidadeX != 0 && carro.velocidadeY==0)
	{
		if (carro.velocidadeX < 0)
		{

			posx = carro.x - carro.textura_carro.dim_xf+70;
			
			distancia = posx - sem.x;
			if (distancia < 0)return -1;
			else return distancia;

		}
		if (carro.velocidadeX > 0)
		{
			posx = carro.x + carro.textura_carro.dim_xf;
			
			distancia = sem.x -posx;
			if (distancia < 0)return -1;
			else return distancia;
		}
		else return 1;
	}
	if (carro.velocidadeY != 0 && carro.velocidadeX == 0)
	{
		if (carro.velocidadeY < 0)
		{
			
			posy = carro.y - carro.textura_carro.dim_yf;
			distancia = posy - sem.y;
			if (distancia < 0)return -1;
			else return distancia;

		}
		if (carro.velocidadeY > 0)
		{
		
			posy = carro.y + carro.textura_carro.dim_yf;
			distancia = sem.y - posy+50;
			if (distancia < 0)return -1;
			else return distancia;
		}
		else return -1;
	}
	return 0;
	
}

void pos_semaforo(semaforo_t *sem, int x, int y, int num)
{
	sem->x = x;
	sem->y = y;
	map[x][y] = num;

}

void para_carro(carro_t *carro)
{
	if (carro->velocidadeX != 0 && carro->velocidadeY == 0)
	{
		if (carro->velocidadeX < 0)
		{
			carro->aux = 1; //1=anda da direita pa esquerda
			carro->velocidadeX = 0;
			carro->velocidadeY = 0;
			return;
		}
		if (carro->velocidadeX > 0)
		{
			carro->aux = 2; //2=anda da esquerda pa direita
			carro->velocidadeX = 0;
			carro->velocidadeY = 0;
			return;
		}
	}
	if (carro->velocidadeY != 0 && carro->velocidadeX == 0)
	{
		if (carro->velocidadeY < 0)
		{
			carro->aux = 4; //4=anda de baixo pa cima
			carro->velocidadeX = 0;
			carro->velocidadeY = 0;
			return;
		}
		if (carro->velocidadeY > 0)
		{
			carro->aux = 3; //3=anda de cima pa baixo
			carro->velocidadeX = 0;
			carro->velocidadeY = 0;
			return;
		}
	}

}

bool_t para_carro_semaforo(carro_t *carro, semaforo_t *sem)
{
	
	if (verifica_distancia(*carro, *sem) < 150 && verifica_distancia(*carro, *sem) > 50 && (*sem).amarelo == 1 && carro->velocidadeY==0)
	{
		if(carro->velocidadeX>3)
		carro->velocidadeX = carro->velocidadeX-1;
		if (carro->velocidadeX<0 &&carro->velocidadeX<-3)
			carro->velocidadeX = carro->velocidadeX + 1;
		return FALSE;
	}
	if (verifica_distancia(*carro, *sem) < 6 && verifica_distancia(*carro, *sem) > -1 && (*sem).amarelo == 1 && (*carro).velocidadeY == 0)
	{
		if (carro->velocidadeX <= 3 && carro->velocidadeX>0)
			return FALSE;
		if (carro->velocidadeX<0 && carro->velocidadeX>-3)
			return TRUE;
		return FALSE;
	}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	if (verifica_distancia(*carro, *sem) <150  && verifica_distancia(*carro, *sem) > 50 && (*sem).vermelho == 1 && (*carro).velocidadeY == 0)
	{
		if (carro->velocidadeX>3)
			carro->velocidadeX = carro->velocidadeX - 1;


		if (carro->velocidadeX<0 && carro->velocidadeX<-3)
			carro->velocidadeX = carro->velocidadeX + 1;
		return FALSE;
	}
	if (verifica_distancia(*carro, *sem) < 5 && verifica_distancia(*carro, *sem) > -1 && (*sem).vermelho == 1 && carro->velocidadeY == 0)
	{
		if (carro->velocidadeX <= 5 && carro->velocidadeX>0)
			return TRUE;
		if (carro->velocidadeX<0 && carro->velocidadeX>-5)
			return TRUE;
		return FALSE;
	}
	
	if (verifica_distancia(*carro, *sem) == -1)
	{
		return FALSE;
	}

	if (verifica_distancia(*carro, *sem) < 150 && verifica_distancia(*carro, *sem) > 50 && (*sem).amarelo == 1 && carro->velocidadeX == 0)
	{
		if (carro->velocidadeY > 3)
		carro->velocidadeY = (carro->velocidadeY) - 1;
		if (carro->velocidadeY < 0 && carro->velocidadeY < -3)
			carro->velocidadeY = (carro->velocidadeY) + 1;
		return FALSE;
	}

	if (verifica_distancia(*carro, *sem) < 6 && verifica_distancia(*carro, *sem) > -1 && (*sem).amarelo == 1 && carro->velocidadeX == 0)
	{
		if (carro->velocidadeY <= 3 && carro->velocidadeY > 0)
		return TRUE;
		if (carro->velocidadeY < 0 && carro->velocidadeY > -3)
			return TRUE;
		return FALSE;
	}

	if (verifica_distancia(*carro, *sem) < 150 && verifica_distancia(*carro, *sem) > 50 && (*sem).vermelho == 1 && carro->velocidadeX == 0)
	{
		if (carro->velocidadeY > 3)
		carro->velocidadeY = (carro->velocidadeY) - 1;
		if (carro->velocidadeY < 0 && carro->velocidadeY < -3)
			carro->velocidadeY = (carro->velocidadeY) + 1;
		return FALSE;
	}
	if (verifica_distancia(*carro, *sem) < 6 && verifica_distancia(*carro, *sem) > -1 && (*sem).vermelho == 1 && carro->velocidadeX == 0)
	{
		if (carro->velocidadeY <= 5 && carro->velocidadeY >0)
		return TRUE;
		if (carro->velocidadeY < 0 && carro->velocidadeY > -5)
			return TRUE;
		return FALSE;
	}


	///////////

	if ((*sem).verde == 1 && carro->aux==2 &&carro->velocidadeX==0 && carro->velocidadeY == 0 )
	{
		carro->velocidadeX = vel_pos_acidente;
		carro->aux = 0;
		return FALSE;
	}
	if ((*sem).verde == 1 && carro->aux == 1 && carro->velocidadeX == 0 && carro->velocidadeY == 0)
	{
		carro->velocidadeX = -vel_pos_acidente;
		carro->aux = 0;
		return FALSE;
	}
	
	if ((*sem).verde == 1 && carro->aux == 3 && carro->velocidadeX == 0 && carro->velocidadeY == 0)
	{
		carro->velocidadeY = vel_pos_acidente;
		carro->aux = 0;
		return FALSE;
	}
	if ((*sem).verde == 1 && carro->aux == 4 && carro->velocidadeX == 0 && carro->velocidadeY == 0)
	{
		carro->velocidadeY = -vel_pos_acidente;
		carro->aux = 0;
		return FALSE;
	}

	else return FALSE;
}
bool_t car_matriz(carro_t carro, carro_t carro1)
{

	//int map_aux[900][900];
	int i;
	int j;
	int k;
	int l;
	for (i = carro.x; i < (carro.x + carro.textura_carro.dim_xf); i++)
		for (j = carro.y; j < (carro.y + carro.textura_carro.dim_yf); j++) {
			map_aux[i][j] = carro.numero;
		}


	for (i = carro1.x; i < (carro1.x + carro1.textura_carro.dim_xf); i++)
		for (j = carro1.y; j < (carro1.y + carro1.textura_carro.dim_yf); j++) {
			map_aux[i][j] = carro1.numero;
		}

	for ( i = carro.x; i < (carro.x + carro.textura_carro.dim_xf); i++)
		for (j = carro.y; j < (carro.y + carro.textura_carro.dim_yf); j++) {

			if (map_aux[i][j] != carro.numero) {
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

void local_acidente(carro_t carro, SDL_Texture *gacidente, SDL_Renderer *gRenderer) {

	SDL_Rect origem;
	SDL_Rect destino;
	origem.x = 0;
	origem.y = 0;
	origem.w = 100;
	origem.h = 60;

	

	destino.x = carro.x;
	destino.y = carro.y;
	destino.w = 100;
	destino.h = 60;

	SDL_RenderCopy(gRenderer2, gacidente, &origem, &destino);
}

void verifica_acidente(carro_t *carro,carro_t *carro1, SDL_Texture *gacidente, SDL_Renderer *gRenderer) {

	if (car_matriz(*carro, *carro1)) {

		local_acidente(*carro, gacidente,gRenderer);

		carro->velocidadeX = 0;
		carro->velocidadeY = 0;
		carro1->velocidadeX = 0;
		carro1->velocidadeY = 0;

	}
}
bool_t inicializa_toda_textura(textura_t *textura1, textura_t *textura2, textura_t *textura3, textura_t *textura4, textura_t *textura5, textura_t *textura6, textura_t *textura7, textura_t *textura8, textura_t *textura9, textura_t *textura10,SDL_Renderer *gRenderer) {
	if (loadMedia(textura1, gRenderer) == FALSE)
	{
		printf("A imagem %s, nao foi carregada com sucesso!!", textura1->path);
		return FALSE;
	}
	if (loadMedia(textura2, gRenderer) == FALSE)
	{
		printf("A imagem %s, nao foi carregada com sucesso!!", textura2->path);
		return FALSE;
	}
	if (loadMedia(textura3, gRenderer) == FALSE)
	{
		printf("A imagem %s, nao foi carregada com sucesso!!", textura3->path);
		return FALSE;
	}
	if (loadMedia(textura4, gRenderer) == FALSE)
	{
		printf("A imagem %s, nao foi carregada com sucesso!!", textura4->path);
		return FALSE;
	}
	if (loadMedia(textura5, gRenderer) == FALSE)
	{
		printf("A imagem %s, nao foi carregada com sucesso!!", textura5->path);
		return FALSE;
	}
	if (loadMedia(textura6, gRenderer) == FALSE)
	{
		printf("A imagem %s, nao foi carregada com sucesso!!", textura6->path);
		return FALSE;
	}
	if (loadMedia(textura7, gRenderer) == FALSE)
	{
		printf("A imagem %s, nao foi carregada com sucesso!!", textura7->path);
		return FALSE;
	}
	if (loadMedia(textura8, gRenderer) == FALSE)
	{
		printf("A imagem %s, nao foi carregada com sucesso!!", textura8->path);
		return FALSE;
	}
	if (loadMedia(textura9, gRenderer) == FALSE)
	{
		printf("A imagem %s, nao foi carregada com sucesso!!", textura9->path);
		return FALSE;
	}
	if (loadMedia(textura10, gRenderer) == FALSE)
	{
		printf("A imagem %s, nao foi carregada com sucesso!!", textura10->path);
		return FALSE;
	}
	else return TRUE;
}