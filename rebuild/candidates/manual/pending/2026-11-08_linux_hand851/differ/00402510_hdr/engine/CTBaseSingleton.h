#ifdef FABLE_RETAIL_GFMAIN
#ifndef FABLE_ENGINE_GFMAIN_CTBASESINGLETON
#define FABLE_ENGINE_GFMAIN_CTBASESINGLETON
// Retail opt-in (Codex pass12): original PDB declarations, native GFMain field offsets.
#include "engine/CCountedPointer.h"
template<class T>class CTBaseSingleton {public:virtual~CTBaseSingleton();static T&__fastcall Get();static void __fastcall Release(){PClass.ReleaseData();}private:static CCountedPointer<T>PClass;};
#endif

#else

#endif
