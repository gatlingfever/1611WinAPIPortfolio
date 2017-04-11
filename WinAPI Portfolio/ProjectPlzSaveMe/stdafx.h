#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <unordered_set>
#include <unordered_map>
#include <list>
#include <vector>
#include <time.h>
#include <windowsx.h>
#include <map>

class C_WINDOW;
class C_GAME;
class C_RENDER;
class C_BITMAPMGR;
class C_GRAPHICS;
class C_OBJECT;
class C_OBJMGR;
class C_TIMER;
enum class EC_ConstNUM;

struct fpoint
{
	float fX;
	float fY;
};

typedef std::unordered_set<C_OBJECT*>::const_iterator objmap_citer;

#include "Resource.h"
#include "window.h"
#include "game.h"
#include "graphics.h"
#include "render.h"
#include "bitmapmgr.h"
#include "object.h"
#include "objmgr.h"
#include "timer.h"