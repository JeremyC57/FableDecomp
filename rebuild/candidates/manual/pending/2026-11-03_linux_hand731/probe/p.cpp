#define FABLE_RETAIL_CENGINE
#include "engine/CEngine.h"
template<int N> struct Show;
Show<sizeof(CEngine)> a;
Show<offsetof(CEngine, EnableWireFrame)> b;
Show<offsetof(CEngine, NonInterpolatedWorldTime)> c;
Show<offsetof(CEngine, EngineComponents)> d;
Show<sizeof(CIEngine)> e;
