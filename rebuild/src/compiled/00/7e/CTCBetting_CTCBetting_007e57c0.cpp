// CTCBetting::CTCBetting(CThing* thing) -- CTCBase(thing), own vtable, two empty primitive-handle vectors resized to 5, Active cleared, the frame counters and PreviousDonation zeroed
// Leaf: ??0CTCBetting@@QAE@AAVCThing@@@Z
// Ported 2026-11-16 onto the shared engine/CTCBase.h: the local base model CTCBase_Calls (which bound ??0CTCBase_Calls@@QAE@PAVCThing@@@Z to 0x00686800) is gone;
//   the base call is ??0CTCBase@@QAE@AAVCThing@@@Z and this constructor takes CThing& as Ego_r.pdb and FableWin.pdb spell it (??0CTCBetting@@QAE@AAVCThing@@@Z).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007E57C0, length 81 bytes (+ int3 padding).
// Callees: CTCBase::CTCBase(CThing&) @ 0x00686800, vector<CEnginePrimitiveHandle>::Resize(long) @ 0x00443F90
// (x2, count 5; the manifest label "CEnginePrimitiveRenderer::AddPrimitive" is propagated).
// Vtable 0x01270624 = CTCBetting (rtti-vtables.tsv).
// Layout = Ego_r.pdb CTCBetting with the retail 12-byte vectors: Active +0xc, BetHandles +0x10,
// MoneyHandles +0x1c, FrameBetWasLastChanged +0x28, FrameBetChangeStarted +0x2c, Mult +0x30,
// FrameToStart +0x34, PreviousDonation +0x38 (PDB +0x30..+0x40, 8 higher).
// Shape: the vtable store precedes the vector zeroing (inline member constructors through each
// vector's address register, all sharing the ebx zero); Active is the first BODY assignment (an
// initialiser stores it before the vectors), then the Resize calls and the three counter zeroes (Mult and FrameToStart are left untouched).
// NF56 (round 7, queue q10427): moved onto engine/CTCBetting.h (overlay opt-in FABLE_RETAIL_CTCBETTING_CTOR for the ctor declaration):
//   BetHandles / MoneyHandles are the header's STLport std::vector<CEnginePrimitiveHandle> (stlport/fable_containers.h), so the calls bind
//   resize(size_type) 0x00443F90 as ?resize@?$vector@VCEnginePrimitiveHandle@@V?$allocator@VCEnginePrimitiveHandle@@@std@@@std@@QAEXI@Z
//   (FABLE_RETAIL_STLP_VECTOR_RESIZE_ONE_DECL, the 0x0064F659 pattern). Were the local CRetailHandleVector stand-in (Resize(long)) and a
//   local CTCBetting model.
#define FABLE_RETAIL_STLP_VECTOR_RESIZE_ONE_DECL
#include "stlport/fable_containers.h"
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCBETTING
#define FABLE_RETAIL_CTCBETTING_CTOR
#include "engine/CTCBase.h"                     // CTCBase(CThing&) 0x00686800 ??0CTCBase@@QAE@AAVCThing@@@Z
#include "engine/CTCBetting.h"
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#include "engine/CEnginePrimitiveHandle.h"

CTCBetting::CTCBetting(CThing& thing)
    : CTCBase(thing)
{
    Active = false;
    BetHandles.resize(5);
    MoneyHandles.resize(5);
    FrameBetWasLastChanged = 0;
    FrameBetChangeStarted = 0;
    PreviousDonation = 0;
}