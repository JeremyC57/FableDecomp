#ifdef FABLE_RETAIL_GFMAIN
#ifndef FABLE_ENGINE_GFMAIN_CSYSTEMREGISTRY
#define FABLE_ENGINE_GFMAIN_CSYSTEMREGISTRY
// Retail opt-in (Codex pass12): original PDB declarations, native GFMain field offsets.
#include "engine/CWideString.h"
class CSystemRegistry{protected:HKEY hRoot,hKey;public:
 static const CWideString GAME_REG_KEY,GFX_RESET_KEY_NAME;
 CSystemRegistry(const CWideString&,long);~CSystemRegistry();void WriteBOOL(const CWideString&,int)const;
};
#endif

#else

#endif
