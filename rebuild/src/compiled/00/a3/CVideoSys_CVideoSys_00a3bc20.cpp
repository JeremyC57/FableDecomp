// CVideoSys::~CVideoSys() -- delete the texture renderer, CoUninitialize, destroy the subtitle set (its wide string and its subtitle list)
// Leaf: ??1CVideoSys@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A3BC20, length 80 bytes.
// Callees: the texture renderer's virtual deleting destructor (slot 3, `push 1 ; call [eax+0Ch]`),
// CoUninitialize (ole32 import 0x01440648), CWideString::~CWideString @ 0x0099B510 (the subtitle set's
// string at +0x44), the subtitle list's clear @ 0x009BE060 (STLport _list.c
// ?clear@?$_List_base@UCSubTitle@@V?$allocator@UCSubTitle@@@std@@@std@@QAEXXZ, out of line), free @ 0x00BFEA14.
// Shape: `if (TexRenderer) { delete TexRenderer; TexRenderer = 0; }` (explicit null test, the clear kept
// before the join), the import call, then the implicit ~CMovieSubtitles on `esi+30h`: the CWideString
// member (+0x14 of the set) first, then the list's inline ~_List_base: out-of-line clear, free the head node.
// NF132 (q4100): moved onto the retail engine/CVideoSys.h / CMovieSubtitles.h / CTextureRenderer.h classes and the shared STLport
//   list (FABLE_RETAIL_STLP_LIST_CLEAR_OUTOFLINE, CSubTitle struct-keyed as in both PDBs: FABLE_RETAIL_CSUBTITLE_STRUCT); was a
//   local CVideoSys / CMovieSubtitles / CSubtitleList (Clear stand-in) / CWideString / CTextureRenderer model.
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CVIDEOSYS
#define FABLE_RETAIL_CSUBTITLE_STRUCT
#define FABLE_RETAIL_STLP_LIST_CLEAR_OUTOFLINE
#include "engine/CVideoSys.h"
#include "engine/CTextureRenderer.h"
extern "C" __declspec(dllimport) void __stdcall CoUninitialize(void);

CVideoSys::~CVideoSys()
{
    if (TexRenderer) {
        delete TexRenderer;
        TexRenderer = 0;
    }
    CoUninitialize();
}
