/* 
 * FILE        : object.h 
 * LAST UPDATE : 23 Jun 2002
 * AUTHOR(S)   : M. Bilderbeek & E. Boon
 *
 * DESCRIPTION :
 *   contains typedefs of objects. That's all folks!
 *
 */

#ifndef OBJECT_H
#define OBJECT_H

/*
 * DEFINITIONS
 */

#define OBJ_MAX_X (4*960)
#define OBJ_MAX_Y (4*784)

#define OBJ_MAX_DXY 64

#define OBJ_VOID -1

/*
 * TYPE DEFINITIONS
 */

typedef char obj_hdl_t;

typedef enum
{
	OBJ_UNUSED,
	OBJ_SHIP,
	OBJ_BULLET,
	OBJ_AST
} object_e;

typedef enum
{
	NEW,
	ALIVE,
	DYING,
	DEAD
} state_e;

typedef struct
{
	int x;
	int y;
	int x_prev;
	int y_prev;
	char dx;
	char dy;
	object_e type;
	state_e state;
} object_t;

/*
 * INTERFACE FUNCTIONS
 */

extern void objects_init (void);

extern obj_hdl_t object_create(int x, int y, char dx, char dy, object_e type);

extern void object_destroy(obj_hdl_t *object);

extern int object_get_x(obj_hdl_t object);

extern int object_get_y(obj_hdl_t object);

extern void object_set_x(obj_hdl_t object, int x);

extern void object_set_y(obj_hdl_t object, int y);

extern int object_get_x_prev(obj_hdl_t object);

extern int object_get_y_prev(obj_hdl_t object);

extern int object_get_dx(obj_hdl_t object);

extern int object_get_dy(obj_hdl_t object);

extern int object_get_type(obj_hdl_t object);

extern state_e object_get_state(obj_hdl_t object);

extern void object_set_state(obj_hdl_t object, state_e state);

extern void object_move(obj_hdl_t object);

extern void object_accel(obj_hdl_t object, char ddx, char ddy);
	
#endif

