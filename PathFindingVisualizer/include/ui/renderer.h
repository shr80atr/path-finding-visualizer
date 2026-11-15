#ifndef RENDERER_H
#define RENDERER_H

#include "grid.h"

void Renderer_DrawGrid(const Grid *grid);

void Renderer_DrawLegend(void);

void Renderer_DrawRowNumbers(void);

void Renderer_DrawCell(const Node *node);

#endif