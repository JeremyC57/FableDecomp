// CTCBetting::CTCBetting(CThing* thing) -- CTCBase(thing), own vtable, two empty primitive-handle vectors resized to 5, Active cleared, the frame counters and PreviousDonation zeroed
// Leaf: ??0CTCBetting@@QAE@AAVCThing@@@Z
// Ported 2026-11-16 onto the shared engine/CTCBase.h: the local base model CTCBase_Calls (which bound ??0CTCBase_Calls@@QAE@PAVCThing@@@Z to 0x00686800) is gone;
//   the base call is ??0CTCBase@@QAE@AAVCThing@@@Z and this constructor takes CThing& as the PDBs spell it (??0CTCBetting@@QAE@AAVCThing@@@Z).
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
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"                     // CTCBase(CThing&) 0x00686800 ??0CTCBase@@QAE@AAVCThing@@@Z
class CThing;

class CEnginePrimitiveHandle;

struct CRetailHandleVector {                                // VC6 Dinkumware vector<CEnginePrimitiveHandle>: three words
    CEnginePrimitiveHandle* m_pFirst;
    CEnginePrimitiveHandle* m_pLast;
    CEnginePrimitiveHandle* m_pEnd;
    CRetailHandleVector() : m_pFirst(0), m_pLast(0), m_pEnd(0) { }
    void Resize(long count);                                // 0x00443F90
};

class CTCBetting : public CTCBase { public:                         // vtable 0x01270624
    bool                Active;                             // +0x0c
    CRetailHandleVector BetHandles;                         // +0x10
    CRetailHandleVector MoneyHandles;                       // +0x1c
    long                FrameBetWasLastChanged;             // +0x28
    long                FrameBetChangeStarted;              // +0x2c
    long                Mult;                               // +0x30
    long                FrameToStart;                       // +0x34
    long                PreviousDonation;                   // +0x38
    CTCBetting(CThing& thing);
};

CTCBetting::CTCBetting(CThing& thing)
    : CTCBase(thing)
{
    Active = false;
    BetHandles.Resize(5);
    MoneyHandles.Resize(5);
    FrameBetWasLastChanged = 0;
    FrameBetChangeStarted = 0;
    PreviousDonation = 0;
}