/* $Id: bigblast.c,v 1.14 2003/02/15 12:03:56 manuel Exp $
 *
 * AUTHOR      : M. Bilderbeek & E. Boon
 *
 * DESCRIPTION :
 *   contains the main. Yup. That's it.
 */

/*
 * INCLUDES
 */
#include "stdlib.h"
#include "msxbios.h"
#include "renderer.h"
#include "object.h"
#include "ship.h"
#include "asteroid.h"
#include "bullet.h"
#include "explosio.h"
#include "collisio.h"
#include "scores.h"
#include "font.h"
#include "menu.h"
#include "controls.h"

/*
 * LOCAL DEFINITIONS
 */
#define MSG_BASE 60
#define MSG_BONUS_BASE 80
#define MENU_BASE_X 100
#define MENU_BASE_Y 150 

// This is the shield energy penalty for hitting an asteroid with shield on
#define AST_SH_PENALTY 25 		

int *JIFFY = (int *)0xFC9E;
unsigned int score;
unsigned char level;
static char quit=0;
static char noflives;

static int get_rnd_coord(int range)
{
	int netrange = range - GFX2OBJ(SHIP_TILE_SIZE);
	int coord = rand() %(netrange);
	if (coord > (netrange >> 1)) 
		coord += GFX2OBJ(SHIP_TILE_SIZE);
	return(coord);
}

static void add_bonus(unsigned char level, unsigned char noflives)
{
	char string[100];
	unsigned int levelbonus, timebonus, shieldbonus, lifebonus;
	
	sprintf(string,"BONUS");
	write_cent(string, MSG_BONUS_BASE);
	
	levelbonus=SC_LEVEL(level);
	score+=levelbonus;
	sprintf(string,"Level bonus: %d", levelbonus);
	write_cent(string, MSG_BONUS_BASE + 10);
	
	if (frame_counter<TIME_BONUS)
		timebonus=(TIME_BONUS-frame_counter);
	else
		timebonus=0;
	score+=timebonus;
	sprintf(string,"Time bonus: %d", timebonus);
	write_cent(string, MSG_BONUS_BASE + 20);

//	if (bullet_counter<BULLET_BONUS(level)) score+=(BULLET_BONUS(level)-bullet_counter);

	shieldbonus=the_ship.shield_energy;
	score+=shieldbonus;
	sprintf(string,"Shield bonus: %d", shieldbonus);
	write_cent(string, MSG_BONUS_BASE + 30);
	
	lifebonus=noflives*10;
	score+=lifebonus;
	sprintf(string,"Life bonus: %d", lifebonus);
	write_cent(string, MSG_BONUS_BASE + 40);

	render_info(noflives);
}

void play_level(char level)
{
	onoff_t fire = OFF;
	int i;
	ast_hdl_t a=0;
	char string[100];
	char not_finished = 1;

	onoff_t boost=OFF;
	onoff_t shield=OFF;
	rotdir_t rotdir=ROT_NONE;
	
	frame_counter = 0;    
	
	cls();
	sprintf(string, "Wave: %02d", level);
	write_cent(string, 102);
	*JIFFY=0;
	while (*JIFFY<50);

	playscreen_init();

	objects_init();
	ship_init();
	
	bullets_init();
	asteroids_init();
	explosions_init();
	
	for (i=0; i<0+level; i++) // Actually: load level data or so
	{
		a = asteroid_create(get_rnd_coord(OBJ_MAX_X), 
				       get_rnd_coord(OBJ_MAX_Y), AST_BIG);
		object_accel(the_asteroids[a].asteroid_obj,
			     rand() % (OBJ_MAX_DXY<<1)-(OBJ_MAX_DXY), 
			     rand() % (OBJ_MAX_DXY<<1)-(OBJ_MAX_DXY));
			  
	}
	
	while (!quit && noflives!=0 && not_finished)
	{
		render_frame(boost, shield, noflives);
		bullets_n_asteroids();
		if (the_ship.ship_obj!=OBJ_VOID)
		{
			check_controls(&rotdir, &boost, &shield, &fire);
			ship_rotate(rotdir);
			if (boost==ON) ship_accel();
			ship_move(); // en schiet op een beetje!
			ship_shield_set(shield);
			if (fire && nof_asteroids) bullet_fire();
			if (object_get_state(the_ship.ship_obj)==DYING)
			{
				ship_destroy();
				boost=OFF;
			}
			else if (ship_hit())
			{
				if (shield)
				{
					if (the_ship.shield_energy > AST_SH_PENALTY)
						the_ship.shield_energy-=AST_SH_PENALTY;
					else
						the_ship.shield_energy=0;
				}
				else
					object_set_state(the_ship.ship_obj,DYING);
			}	
		}
		else
		{
			if ((!nof_bullets)&&(!nof_explosions) && (--noflives)) 
			{
				ship_init();
			}
		}	
		not_finished=(nof_explosions || nof_bullets || nof_asteroids);
	
		asteroids_move();
		bullets_move();
		explosions_move();
		quit=check_quit();
	}
	render_info(noflives); // update noflives on screen
	if (nof_asteroids==0)
	{
		add_bonus(level, noflives);
		sprintf(string,"Wave %d completed!",level);
		write_cent(string, MSG_BASE);
		*JIFFY = 0;
		while (*JIFFY<100);
		while (!keypressed());
	}

}

void play_game()
{
	char string[100];
	score = 0;
	level = 0;
	noflives=5;

	while (!quit && noflives>0)
	{
		play_level(++level);
	}

	if (noflives==0) // This means that !quit==TRUE implicitly
	{
		sprintf(string,"Game over!");
		write_cent(string, MSG_BASE);
	
		sprintf(string,"Total score: %d points", score);
		write_cent(string, MSG_BASE+80);

		sprintf(string,"You reached wave %d", level);
		write_cent(string, MSG_BASE+100);
	
		*JIFFY = 0;
		while (*JIFFY<100);
	
		while (!keypressed());
	}
}

void main ()
{
	char string[100];

	char *CLICKSW=(char *)0xF3DB;
	char clicksw_old=*CLICKSW;
	*CLICKSW=0;        

	srand(*JIFFY);

	render_init();

	do
	{
		cls();
		sprintf(string,"Welcome to Big Blast!");
		write_cent(string, MSG_BASE);
		init_menu();
		while (keypressed());
		switch (menu_select())
		{
			case PLAY: play_game();	quit=0; break;
		 	case QUIT: quit=1; break;
		 	default: ;
		}
	}
	while (!quit);
	screen(0);
	*CLICKSW=clicksw_old;
	kilbuf();	
}
