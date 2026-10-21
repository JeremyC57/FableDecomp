// CAIStateGroup_TavernServer::IsRunnable() -- runnable while carrying a jug; otherwise, for a village member with a live pub (adopted from the creature's workplace when unset), a live bar (or one found), a serving point (or one found) and a jug: free the old points and drop the jug pointer
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00967650, length 336 bytes (+ int3 padding).
// Callees: IsCarryingJug @ 0x00966CF0, CVectorMap::LowerBound @ 0x0040F020 (inlined GetInterface 0x23 =
//          CTCVillageMember, bit 3 of +0x24 tested as a byte), CTCVillageMember::GetPVillage @ 0x007EE540,
//          CBaseIntelligentPointer ctor/GetPItem/SetPItem/dtor (temporary vtable 0x01244344; the creature's workplace
//          pointer at +0x170), IS_THING_ALIVE<CIntelligentPointer<...>> @ 0x0078B2A0 (__fastcall, PBar), GetBar
//          @ 0x00966AF0, GetServingPoint @ 0x00966DA0, FreeServingPoint @ 0x009670A0 / FreeBarPoint @ 0x00967110 (manifest
//          labels name CAIStateGroup_TavernKeeper, propagated), GetJug @ 0x00967180, CIntelligentPointer(CThing*)
//          @ 0x0065E133 and operator= @ 0x007B82C0 (out of line).
// Shape: masked HasTC + entry pick; the pub is adopted through a frame temporary with the named-item operator=; the
//          final jug reset is an implicit conversion from 0 into the out-of-line operator=.
// Layout: engine/CAIStateGroup_TavernServer.h (PJug +0x14, PBar +0x1c, PPub +0x34, ServingActionPointIndex +0x40);
//          PBrain is the first dword of the flattened base at +0x04.
#include "engine/CAIStateGroup_TavernServer.h"

struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
struct CBaseIntelligentPointer;
struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad1[0x91 - 0x4c];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *ppOut = (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
            return true;
        }
        return false;
    }
};
struct CVillage;
struct CTCVillageMember {                       // interface 0x23
    CVillage* GetPVillage() const;              // 0x007EE540
};

struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CThing* GetPItem() const;                   // 0x00A01B50
    CBaseIntelligentPointer();                  // 0x00A01B10
    ~CBaseIntelligentPointer();                 // 0x00A01C10
    void SetPItem(CThing* item);                // 0x00A01B90
};
struct CIntelligentPointerThing : CBaseIntelligentPointer {   // CIntelligentPointer<CThing>, vtable 0x01244344
    virtual void v00();
    __forceinline CIntelligentPointerThing(CThing* item) : CBaseIntelligentPointer() { SetPItem(item); }
    __forceinline CIntelligentPointerThing& operator=(const CIntelligentPointerThing& other) { CThing* item = other.GetPItem(); SetPItem(item); return *this; }
};
struct CIntelligentPointerConstThing : CBaseIntelligentPointer {   // CIntelligentPointer<const CThing>
    CIntelligentPointerConstThing(CThing* item);    // 0x0065E133
    CIntelligentPointerConstThing& operator=(const CIntelligentPointerConstThing& other);   // 0x007B82C0
};
__forceinline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}
bool __fastcall IS_THING_ALIVE_Pointer(const CBaseIntelligentPointer& p);   // 0x0078B2A0 (IS_THING_ALIVE<CIntelligentPointer<T>>)

struct CThingAICreature {
    unsigned char _pad[0x170];
    CBaseIntelligentPointer PWorkplace;         // +0x170 (descriptive)
};
struct CAIBrain {
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};

struct CAIStateGroupTavernServerMethods : CAIStateGroup_TavernServer {
    CAIBrain* Brain() const { return *(CAIBrain* const*)_pad_0x04; }
    CThing* GetCreature() const { return Brain()->PCreature; }
    CIntelligentPointerConstThing& Jug() { return *(CIntelligentPointerConstThing*)PJug; }
    CBaseIntelligentPointer& Bar() { return *(CBaseIntelligentPointer*)PBar; }
    CIntelligentPointerThing& Pub() { return *(CIntelligentPointerThing*)PPub; }
    bool IsCarryingJug();                       // 0x00966CF0
    bool GetBar();                              // 0x00966AF0
    bool GetServingPoint();                     // 0x00966DA0
    void FreeServingPoint();                    // 0x009670A0
    bool GetJug();                              // 0x00967180
    void FreeBarPoint();                        // 0x00967110
    bool IsRunnable();
};

bool CAIStateGroupTavernServerMethods::IsRunnable()
{
    if (IsCarryingJug())
        return true;
    CTCVillageMember* member;
    if (GetCreature()->GetInterface(&member, 0x23)) {
        CVillage* village = member->GetPVillage();
        if (!Pub().GetPItem())
            Pub() = CIntelligentPointerThing(((CThingAICreature*)GetCreature())->PWorkplace.GetPItem());
        if (IS_THING_ALIVE(Pub()) && village
            && (IS_THING_ALIVE_Pointer(Bar()) || GetBar())
            && (ServingActionPointIndex != -1 || GetServingPoint())) {
            FreeServingPoint();
            if (GetJug()) {
                FreeBarPoint();
                Jug() = (CThing*)0;
                return true;
            }
        }
    }
    return false;
}
