// NPlayerGui::CAutoPickupBar::CAutoPickupBar(CStatBarInfo* info) -- CStatBase(info), own vtable, an empty pickup list, current def index / count 0, both timers 0
// Leaf: ??0CAutoPickupBar
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006543FF, length 57 bytes; frame-pointer size TU (`push ebp ; mov ebp,esp`,
// pass-through `push dword ptr [ebp+8]`, `and dword ptr [..],0` integer zeroes, `fldz ; fstp` float zeroes):
// `optimize("y",off)` + `("s",on)` like the landed NPlayerGui::CMultiStatBar constructor 0x006543AF.
// Callees: NPlayerGui::CStatBase::CStatBase(CStatBarInfo*) @ 0x0065431D, the pickup list's constructor @ 0x00655BE9 (manifest
// "std_list_allocate_sentinel_0x14_3"). Vtable 0x0125A768 = NPlayerGui::CAutoPickupBar (rtti-vtables.tsv): the chunk label
// "CTCPreCalculatedNavigationRoute::CTCPreCalculatedNavigationRoute" is propagated.
// Layout = Ego_r.pdb CAutoPickupBar (0x94) less 0x10 bytes (the shipped CStatBase is 0x10 smaller, as the landed CMultiStatBar /
// CCrimeStatBar found; the list is the 4-byte sentinel pointer): Pickups +0x68, CurrentDefIndex +0x6c, CurrentCount +0x70,
// CurrentTimeLeft +0x74, WaitingTimeLeft +0x78.
// Shape: the list takes its allocator by const reference, so the empty allocator temporary is homed in the high byte of the
// consumed argument slot (`lea eax,[ebp+0Bh]`); the two float zeroes are separate `fldz ; fstp` pairs interleaved with the
// `and` integer zeroes.
#pragma optimize("y", off)
#pragma optimize("s", on)
struct CStatBarInfo;

namespace NPlayerGui {

struct CRetailAllocator { CRetailAllocator() { } };

struct CStatBase {
    virtual void v00();
    unsigned char _body[0x68 - 4];                          // +0x04 .. +0x68: the stat-bar base state
    CStatBase(CStatBarInfo* info);                          // 0x0065431D
};

struct CRetailList_AutoPickup {                             // retail list<CAutoPickupBar::CAutoPickup>: the sentinel pointer
    void* Node;
    CRetailList_AutoPickup(const CRetailAllocator& al = CRetailAllocator());   // 0x00655BE9
};

struct CAutoPickupBar : CStatBase {                         // vtable 0x0125A768
    virtual void v00();
    CRetailList_AutoPickup Pickups;                         // +0x68
    long                   CurrentDefIndex;                 // +0x6c
    long                   CurrentCount;                    // +0x70
    float                  CurrentTimeLeft;                 // +0x74
    float                  WaitingTimeLeft;                 // +0x78
    CAutoPickupBar(CStatBarInfo* info);
};

CAutoPickupBar::CAutoPickupBar(CStatBarInfo* info)
    : CStatBase(info)
    , Pickups()
    , CurrentDefIndex(0)
    , CurrentCount(0)
    , CurrentTimeLeft(0.0f)
    , WaitingTimeLeft(0.0f)
{
}

}
