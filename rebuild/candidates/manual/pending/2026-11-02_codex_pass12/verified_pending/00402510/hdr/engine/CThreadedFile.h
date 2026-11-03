#ifdef FABLE_RETAIL_GFMAIN
#ifndef FABLE_ENGINE_GFMAIN_CTHREADEDFILE
#define FABLE_ENGINE_GFMAIN_CTHREADEDFILE
// Retail opt-in (Codex pass12): original PDB declarations, native GFMain field offsets.
#include "engine/CBaseClassNonCopyable.h"
#include "engine/CThreadedFileManager.h"
class CThreadedFile:public CBaseClassNonCopyable{
 unsigned char Body_4[24]; // Original complete28-byte file, no instance accessed here.
 static CThreadedFileManager Manager;
public:static CThreadedFileManager*__fastcall GetManager(){return &Manager;}
};
#endif

#else

#endif
