// CIsHornTaskWithThisHorn::CIsHornTaskWithThisHorn(CIntelligentPointer<CThing> p0) -- the horn-task predicate keeps the horn (PHorn = p0)
// Leaf: ??0CIsHornTaskWithThisHorn@@QAE@V?$CIntelligentPointer@VCThing@@@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0065F0FA, 28 bytes, thiscall, `ret 8` (the by-value 8-byte intelligent pointer, destroyed by the callee).
// Name: FableWin.pdb 0x02160C17 ??0CIsHornTaskWithThisHorn@@QAE@V?$CIntelligentPointer@VCThing@@@@@Z (engine/CIsHornTaskWithThisHorn.h,
//   Ego_r/FableWin layout: PHorn, CIntelligentPointer<CThing>, size 8); its only caller is CTCVillage::UpdateHornTasks (landed 0x0065BE6C)
//   at 0x0065BF03, which builds the predicate for find_if 0x0065FFCA (`CIsHornTaskWithThisHorn(p_horn)`; that TU does not expand the
//   constructor). NF95 2026-11-19 (q11276): was the stand-in extern "C" CTCActionUseBuyHouse_OnCreate (a propagated catalog label)
//   calling a made-up copyctor_4a9730.
// Callees: CIntelligentPointer<CThing>::CIntelligentPointer(const CIntelligentPointer&) 0x004A9730 (out of line: the vtable 0x01238C6C
//   store proves <CThing>), CBaseIntelligentPointer::~CBaseIntelligentPointer() 0x00A01C10 (the by-value argument).
#define FABLE_RETAIL_CGAMETIMEMANAGER_FULL
#define FABLE_RETAIL_HORN_TASKS
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_CONVERT_MUTABLE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COPY_OUTOFLINE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_GET_ORIGINAL_CHAIN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_DEREFERENCE
#define FABLE_RETAIL_CCOUNTEDPOINTER
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CVILLAGETASKHORN
#define FABLE_RETAIL_CISHORNTASKWITHTHISHORN_CTOR_OUTOFLINE
#include "engine/CThing.h"
#include "engine/CIsHornTaskWithThisHorn.h"
CIsHornTaskWithThisHorn::CIsHornTaskWithThisHorn(CIntelligentPointer<CThing> p0) : PHorn(p0)
{
}
