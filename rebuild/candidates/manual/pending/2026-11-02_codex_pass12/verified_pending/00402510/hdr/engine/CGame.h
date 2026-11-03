#ifdef FABLE_RETAIL_GFMAIN
#ifndef FABLE_ENGINE_GFMAIN_CGAME
#define FABLE_ENGINE_GFMAIN_CGAME
// Retail opt-in (Codex pass12): original PDB declarations, native GFMain field offsets.
#include "engine/CInitBaseClass.h"
class CGameComponent;
class CGame:public CInitBaseClass{CGameComponent*CurrentGameComponent;unsigned char ParameterBuffer[512];bool Quit;public:bool Initialise();void Uninitialise();void Play();};
extern CGame GGame;
#endif

#else

#endif
