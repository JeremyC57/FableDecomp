// CAIStateGroup_SitRoundCampfire::IsRunnable() -- not while the creature has interface 0x4a; only when its idle scheduler prefers this group and (for a villager) it is curfew time: reset the seat state and latch the nearest campfire, runnable when it is alive
// Grade: DIFFER(ours v retail) -- this in esi (retail edi) and the final IS_THING_ALIVE becomes a branchless `mov al,1 ; test al,dl` merged into the shared exit instead of retail's separate true exit
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00939670, length 368 bytes (+ int3 padding).
// Callees: CVectorMap::LowerBound @ 0x0040F020 (inlined GetInterface 0xd3 = CTCIdleScheduler, bit 19 of +0x38, and 0x23 =
//          CTCVillageMember, bit 3 of +0x24), CTCIdleScheduler::IsPreferredIdle @ 0x00801BE0 (this group),
//          CTCVillageMember::GetPVillage @ 0x007EE540 (twice), CTCVillage::IsCurfewTime @ 0x00658D5D, CBaseIntelligentPointer
//          ctor/SetPItem/GetPItem/dtor (temporary CIntelligentPointer<CThingObject>, vtable 0x0125A858), GetPNearCampfire
//          @ 0x009395F0.
// Shape: masked HasTC tests (the 0x4a exit before the esi push); both lookups are the out-parameter if/assign form;
//          PCampfire is cleared, then assigned the search result, through frame temporaries.
// Layout: engine/CAIStateGroup_SitRoundCampfire.h (PCampfire +0x14, SeatPosIndex +0x1c, GotSeatPos +0x20, Sitting
//          +0x21); PBrain is the first dword at +0x04.
#include "engine/CAIStateGroup_SitRoundCampfire.h"

struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad1[0x91 - 0x4c];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    __forceinline unsigned long HasTC(long interfaceType) const
    {
        return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31));
    }
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
};
struct CAIStateGroupSitRoundCampfireMethods;
struct CTCIdleScheduler {                       // interface 0xd3
    bool IsPreferredIdle(CAIStateGroupSitRoundCampfireMethods* group);   // 0x00801BE0
};
struct CTCVillage {
    bool IsCurfewTime();                        // 0x00658D5D
};
struct CTCVillageMember {                       // interface 0x23
    CTCVillage* GetPVillage() const;            // 0x007EE540
};

struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CThing* GetPItem() const;                   // 0x00A01B50
    CBaseIntelligentPointer();                  // 0x00A01B10
    ~CBaseIntelligentPointer();                 // 0x00A01C10
    void SetPItem(CThing* item);                // 0x00A01B90
};
struct CIntelligentPointerObject : CBaseIntelligentPointer {   // CIntelligentPointer<CThingObject>, vtable 0x0125A858
    virtual void v00();
    __forceinline CIntelligentPointerObject(CThing* item) : CBaseIntelligentPointer() { SetPItem(item); }
    __forceinline CIntelligentPointerObject& operator=(const CIntelligentPointerObject& other) { CThing* item = other.GetPItem(); SetPItem(item); return *this; }
};
__forceinline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}
struct CAIBrain {
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};

struct CAIStateGroupSitRoundCampfireMethods : CAIStateGroup_SitRoundCampfire {
    CAIBrain* Brain() const { return *(CAIBrain* const*)_pad_0x04; }
    CThing* GetCreature() const { return Brain()->PCreature; }
    CIntelligentPointerObject& Campfire() { return *(CIntelligentPointerObject*)PCampfire; }
    CThing* GetPNearCampfire();                 // 0x009395F0
    bool IsRunnable();
};

bool CAIStateGroupSitRoundCampfireMethods::IsRunnable()
{
    if (GetCreature()->HasTC(0x4a))
        return false;
    CTCIdleScheduler* scheduler;
    if (GetCreature()->GetInterface(&scheduler, 0xd3) && !scheduler->IsPreferredIdle(this))
        return false;
    CTCVillageMember* member;
    if (GetCreature()->GetInterface(&member, 0x23) && member->GetPVillage()
        && !member->GetPVillage()->IsCurfewTime())
        return false;
    GotSeatPos = false;
    Sitting = false;
    SeatPosIndex = -1;
    Campfire() = CIntelligentPointerObject(0);
    Campfire() = CIntelligentPointerObject(GetPNearCampfire());
    if (IS_THING_ALIVE(Campfire()))
        return true;
    return false;
}
