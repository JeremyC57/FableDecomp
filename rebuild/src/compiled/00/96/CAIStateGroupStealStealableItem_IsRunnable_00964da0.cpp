// CAIStateGroup_StealStealableItem::IsRunnable() -- unless a cleanup is pending, for a creature whose follow-state component allows stealing: reset the action state, latch the stealable item and an escape point; runnable when both are alive
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00964DA0, length 336 bytes (+ int3 padding).
// Callees: CVectorMap::LowerBound const @ 0x004365B0 (inlined GetInterface 0xAA, bit 10 of +0x34), FindPStealableItem
//          @ 0x00964360, FindPEscapePoint(bool) @ 0x00964CA0, CBaseIntelligentPointer ctor/SetPItem/GetPItem/dtor
//          (temporaries CIntelligentPointer<CThing> vtable 0x01238C6C and CIntelligentPointer<const CThing> vtable
//          0x01237F1C).
// Shape: the component lookup goes through the const overload (retail calls the TU's const LowerBound copy) with
//          the if/assign pick; both pointers are assigned through frame temporaries with the named-item operator=;
//          a same-TU const LowerBound stand-in lets the temporaries reuse the dead key slot (8-byte frame);
//          the result is the int-valued `&&` of two inline IS_THING_ALIVE tests (`mov eax,1` / `xor eax,eax`).
// Layout: engine/CAIStateGroup_StealStealableItem.h (PStealableItem +0x14, PEscapePoint +0x1c, ActionPointIndex +0x2c,
//          GotActionPoint +0x30, ReachedDestination +0x31, PickUpActionID +0x34, NeedsCleanup +0x38); the 0xAA
//          component's +0x68 / +0x6a flags are descriptive.
#include "engine/CAIStateGroup_StealStealableItem.h"

class CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type) const; // 0x004365B0 (stand-in body below)
};
#pragma auto_inline(off)
// Same-TU stand-in for the const CVectorMap::LowerBound (0x004365B0, STLport lower_bound; retail's LTCG saw its body):
// the key's address does not escape, so its frame slot is free again for the pointer temporaries.
__declspec(noinline) CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type) const
{
    SEntry* it = first;
    int len = int(last - first);
    while (len > 0) {
        int half = len / 2;
        SEntry* middle = it + half;
        if (middle->type < *type) {
            it = middle + 1;
            len = len - half - 1;
        } else
            len = half;
    }
    return it;
}
#pragma auto_inline(on)
class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad1[0x91 - 0x4c];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    template<class T> __forceinline void GetInterface(T** ppOut, long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            const CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
        }
    }
};
struct CTCFollowerState {                       // interface 0xAA
    unsigned char _pad[0x68];
    bool CanSteal;                              // +0x68 (descriptive)
    unsigned char _pad69;
    bool EscapeAnywhere;                        // +0x6a (descriptive)
};

struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CThing* GetPItem() const;                   // 0x00A01B50
    CBaseIntelligentPointer();                  // 0x00A01B10
    ~CBaseIntelligentPointer();                 // 0x00A01C10
    void SetPItem(CThing* item);                // 0x00A01B90
};
struct CIntelligentPointerThing : CBaseIntelligentPointer {   // CIntelligentPointer<CThing>, vtable 0x01238C6C
    virtual void v00();
    __forceinline CIntelligentPointerThing(CThing* item) : CBaseIntelligentPointer() { SetPItem(item); }
    __forceinline CIntelligentPointerThing& operator=(const CIntelligentPointerThing& other) { CThing* item = other.GetPItem(); SetPItem(item); return *this; }
};
struct CIntelligentPointerConstThing : CBaseIntelligentPointer {   // CIntelligentPointer<const CThing>, vtable 0x01237F1C
    virtual void v00();
    __forceinline CIntelligentPointerConstThing(CThing* item) : CBaseIntelligentPointer() { SetPItem(item); }
    __forceinline CIntelligentPointerConstThing& operator=(const CIntelligentPointerConstThing& other) { CThing* item = other.GetPItem(); SetPItem(item); return *this; }
};
__forceinline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}
class CAIBrain { public:
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};

struct CAIStateGroupStealStealableItemMethods : CAIStateGroup_StealStealableItem {
    CAIBrain* Brain() const { return *(CAIBrain* const*)_pad_0x04; }
    CThing* GetCreature() const { return Brain()->PCreature; }
    CIntelligentPointerThing& Item() { return *(CIntelligentPointerThing*)PStealableItem; }
    CIntelligentPointerConstThing& Escape() { return *(CIntelligentPointerConstThing*)PEscapePoint; }
    CThing* FindPStealableItem();               // 0x00964360
    CThing* FindPEscapePoint(bool anywhere);    // 0x00964CA0
    bool IsRunnable();
};

bool CAIStateGroupStealStealableItemMethods::IsRunnable()
{
    if (!NeedsCleanup) {
        CTCFollowerState* state;
        ((const CThing*)GetCreature())->GetInterface(&state, 0xaa);
        if (state->CanSteal) {
            ActionPointIndex = -1;
            ReachedDestination = false;
            GotActionPoint = false;
            PickUpActionID = -1;
            Item() = CIntelligentPointerThing(FindPStealableItem());
            Escape() = CIntelligentPointerConstThing(FindPEscapePoint(!state->EscapeAnywhere));
            return IS_THING_ALIVE(Item()) && IS_THING_ALIVE(Escape());
        }
    }
    return false;
}