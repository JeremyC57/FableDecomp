#ifdef FABLE_RETAIL_GFMAIN
#ifndef FABLE_ENGINE_GFMAIN_NUISYSTEM_CIME
#define FABLE_ENGINE_GFMAIN_NUISYSTEM_CIME
// Retail opt-in (Codex pass12): original PDB declarations, native GFMain field offsets.
class CPersistContext;
namespace NUISystem{class CIME{
 // Layout: original complete84-byte CIME; no member accessed by this row.
 unsigned char Body_0[84];
public:static CIME&__fastcall Get();static void __fastcall LoadIMESettings(CPersistContext&);void Initialise(HWND);void Shutdown();
 static bool __fastcall ProcessWinMessage(HWND,unsigned int,unsigned int,long,long&);static bool __fastcall FilterMessage(tagMSG*);
};}
#endif

#else

#endif
