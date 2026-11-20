// CMovieSubtitles::~CMovieSubtitles() -- empty destructor body: the FormattedText wide string (+0x14) is destroyed, then the SubTitles list (+0) is cleared (every CSubTitle node freed) and its sentinel head freed
// Leaf: ??1CMovieSubtitles@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A3BDE0, length 35 bytes; no caller in Fable.exe (no E8/E9 or absolute reference).
// Identity: FableWin.pdb ??1CMovieSubtitles@@QAE@XZ (0x03209CB0; Ego_r.pdb has none: its ~CVideoSys expands it, as retail's
//   0x00A3BC20 does). The body is exactly the CMovieSubtitles member teardown of engine/CMovieSubtitles.h: CWideString::~CWideString
//   0x0099B510 on FormattedText (+0x14), then std::list<CSubTitle> SubTitles (+0): its out-of-line clear 0x009BE060
//   (?clear@?$_List_base@UCSubTitle@@V?$allocator@UCSubTitle@@@std@@@std@@QAEXXZ, the CSubTitle node destructor: CCharString +0x10,
//   CWideString +0xC, counted pointer +8) and free 0x00BFEA14 of the head node. The other members are trivially destructible.
// NF132 (q4100): was the stand-in CNamedEntryList_00a3bde0::~CNamedEntryList_00a3bde0 over a local CRetailList_009be060 (Clear stand-in).
//   The destructor stays the header's implicit one (engine/CMovieSubtitles.h; ~CVideoSys 0x00A3BC20 expands it): the helper below (not a
//   retail function) makes VC7.1 emit it as its COMDAT, the 0x007AEB70 / 0x009AC310 recipe.
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CSUBTITLE_STRUCT
#define FABLE_RETAIL_STLP_LIST_CLEAR_OUTOFLINE
#include "engine/CMovieSubtitles.h"

namespace {
// Fidelity: scaffolding, not retail: calls the implicit ~CMovieSubtitles so this object emits it out of line as a pick-any COMDAT (GOTCHAS
// nf86 / nf88). Internal to this TU; never called.
void DestroyCMovieSubtitles(CMovieSubtitles* subtitles) { subtitles->~CMovieSubtitles(); }
}
