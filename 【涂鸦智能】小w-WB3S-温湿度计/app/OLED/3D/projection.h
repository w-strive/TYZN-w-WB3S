/******************************************************************************
* ÎÄµµ: projection.h
* ×÷Õß: Wu LianWei
******************************************************************************/
#ifndef _PROJECTION_H_
#define _PROJECTION_H_

#include "STC8A8K64S4A12.h"
//#include "rotation_font3D.h"

typedef struct 
{
    int x; int y;
}_2D;
typedef struct 
{
    float x; float y; float z;
}_3D;



extern int  XOrigin;
extern int  YOrigin;

extern _2D OrtProject(_3D  Space);
extern _2D PerProject(_3D  Space);




#endif
