#ifdef FABLE_RETAIL_GFMAIN
#ifndef FABLE_ENGINE_GFMAIN_CTHREADEDFILEMANAGER
#define FABLE_ENGINE_GFMAIN_CTHREADEDFILEMANAGER
// Retail opt-in (Codex pass12): original PDB declarations, native GFMain field offsets.
#include "engine/CBaseClassNonCopyable.h"
class CThreadedFileManager:public CBaseClassNonCopyable{
 // Layout: unused native state remains opaque; only original static callbacks are referenced.
 unsigned char Body_4[0x1b4];
 static void(__fastcall*m_errorCallback)();static void(__fastcall*m_harddiskErrorCallback)(const char*);
public:void Uninitialise();static void __fastcall SetErrorCallback(void(__fastcall*f)()){m_errorCallback=f;}
 static void __fastcall SetHardDiskErrorCallback(void(__fastcall*f)(const char*)){m_harddiskErrorCallback=f;}
};
#endif

#else

#endif
