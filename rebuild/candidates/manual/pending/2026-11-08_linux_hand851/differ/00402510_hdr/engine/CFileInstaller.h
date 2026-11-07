#ifdef FABLE_RETAIL_GFMAIN
#ifndef FABLE_ENGINE_GFMAIN_CFILEINSTALLER
#define FABLE_ENGINE_GFMAIN_CFILEINSTALLER
// Retail opt-in (Codex pass12): original PDB declarations, native GFMain field offsets.
#include "engine/CTBaseSingleton.h"
class CFileInstaller:public CTBaseSingleton<CFileInstaller>{
protected:
 unsigned char Body_4[0x7d]; // Layout: all native state up to IgnoreTimeStampWhenCopying+81 retained.
 bool IgnoreTimeStampWhenCopying;unsigned char Body_82[0x1e];
public:virtual~CFileInstaller();void SetIgnoreTimeStampWhenCopying(bool value){IgnoreTimeStampWhenCopying=value;}
};
FABLE_STATIC_ASSERT(sizeof(CFileInstaller)==0xa0);
#endif

#else

#endif
