// NPlayerGui::CMultiStatBar::CMultiStatBar(CStatBarInfo*) -- CStatBase(info), own vtable, an empty update list, current update type 6, black opaque colour, no graphic or effect, both timers 0
// Leaf: ??0CMultiStatBar
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006543AF, length 80 bytes; frame-pointer size TU
// (`push ebp ; mov ebp,esp`, pass-through `push dword ptr [ebp+8]`, `fldz ; fstp` float zeroes):
// `optimize("y",off)` + `("s",on)`.
// Callees: NPlayerGui::CStatBase::CStatBase(CStatBarInfo*) @ 0x0065431D, the update list's constructor
// @ 0x00655BA9 (out of line: zeroes the sentinel pointer, mallocs the 0x14-byte node and self-links it;
// manifest "std_list_allocate_sentinel_0x14_2"). Vtable 0x0125A758 = NPlayerGui::CMultiStatBar
// (rtti-vtables.tsv): the chunk label "CTCCreatureFlyOnSpline::CTCCreatureFlyOnSpline" is propagated.
// Layout = Ego_r.pdb CMultiStatBar less 0x10 bytes (the shipped CStatBase is 0x10 smaller, as the landed
// CCrimeStatBar 0x00654392 found, and the list is the 4-byte sentinel pointer): Updates +0x68,
// CurrentType +0x6c, CurrentColour +0x70, CurrentGraphic +0x74, CurrentEffect +0x78, CurrentTimeLeft
// +0x7c, WaitingTimeLeft +0x80.
// Shape: the list takes its allocator by const reference, so the empty allocator temporary is homed in
// the high byte of the consumed argument slot (`lea eax,[ebp+0Bh]`); CRGBColour is stored R, G, B then
// A (its byte order is B, G, R, A and its constructor assigns in parameter order).
#pragma optimize("y", off)
#pragma optimize("s", on)
struct CStatBarInfo;

namespace NPlayerGui {

struct CRetailAllocator { CRetailAllocator() { } };

class CStatBase { public:
    virtual void v00();
    unsigned char _body[0x68 - 4];                          // +0x04 .. +0x68: the stat-bar base state
    CStatBase(CStatBarInfo* info);                          // 0x0065431D
};

struct CRetailList_StatUpdate {                             // retail list<CMultiStatBar::CStatUpdate>: the sentinel pointer
    void* Node;
    CRetailList_StatUpdate(const CRetailAllocator& al = CRetailAllocator());   // 0x00655BA9
};

struct CRGBColour {                                         // D3DCOLOR byte order
    unsigned char B, G, R, A;
    CRGBColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
};

class CMultiStatBar : public CStatBase { public:                          // vtable 0x0125A758
    virtual void v00();
    CRetailList_StatUpdate Updates;                         // +0x68
    long                   CurrentType;                     // +0x6c EStatUpdateType
    CRGBColour             CurrentColour;                   // +0x70
    long                   CurrentGraphic;                  // +0x74
    long                   CurrentEffect;                   // +0x78
    float                  CurrentTimeLeft;                 // +0x7c
    float                  WaitingTimeLeft;                 // +0x80
    CMultiStatBar(CStatBarInfo* info);
};

CMultiStatBar::CMultiStatBar(CStatBarInfo* info)
    : CStatBase(info)
    , Updates()
    , CurrentType(6)
    , CurrentColour(0, 0, 0, 255)
    , CurrentGraphic(0)
    , CurrentEffect(0)
    , CurrentTimeLeft(0.0f)
    , WaitingTimeLeft(0.0f)
{
}

}