/* 
 * FILE        : ship.h 
 * LAST UPDATE : 10 May 2002
 * AUTHOR(S)   : M. Bilderbeek & E. Boon
 *
 * DESCRIPTION :
 *   contains typedefs of the ship. That's all folks!
 */

#ifndef SHIP_H
#define SHIP_H

/*
 * INCLUDES
 */

#include "object.h"

/*
 * DEFINITIONS
 */

#define MAX_NOF_SHIPS 2
#define MAX_SHIELD_ENERGY 255
#define MAX_HEADING 32

/*
 * TYPE DEFINITIONS
 */

typedef enum
{
	OFF,
	ON
} onoff_t;

typedef enum
{
	COUNTERCLOCKWISE = -1,
	ROT_NONE,
	CLOCKWISE
} rotdir_t;

typedef struct
{
	obj_hdl_t ship_obj;
	char heading;
	char heading_prev;
	unsigned char shield_energy;
	onoff_t shield_state;
} ship_t;

typedef struct
{
	char x;
	char y;
} heading_table_t[MAX_HEADING];

extern ship_t the_ship;
extern heading_table_t heading2dxdy;

/*
 * EXTERNAL FUNCTIONS
 */

extern void ship_init();

extern void ship_destroy();

extern void ship_rotate(rotdir_t direction);

extern void ship_shield_set(onoff_t state);

extern void ship_move();

extern void ship_accel();

#endif
