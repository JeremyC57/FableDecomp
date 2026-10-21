// CAIStateGroup_MinionReactToCriminal::IsStillValid() const -- with a live home: record whether the creature and the live hero are inside the house and whether any village guard is, then defer to IsValid
// Grade: DIFFER(ours v retail) -- first draft: retail spills this at entry and keeps the home pointer in ebx / this in ebp; ours keeps this in ebx and computes the flags in a different register plan (358v387)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00931200, length 400 bytes (+ int3 padding).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PHome +0x1c, PHero +0x14, the members' pointers),
//          CThing::GetPosition @ 0x004C73D0, CThingBuilding::IsPointInside @ 0x0082D000, the inlined GetInterface 0x23
//          (CTCVillageMember, bit 3 of +0x24), CTCVillageMember::GetPVillage @ 0x007EE540 (twice), the village's member
//          list at +0x2c (STLport list of intelligent pointers), IsValid @ 0x00930EE0.
// Shape: `this` is spilled at entry (ebp is reused inside the member loop); the hero flag is an int-valued `&&`
//          (mov eax,1 / xor eax,eax) stored as a byte; guards are members with interface 0xb4 (bit 20 of +0x34).
// Layout: engine/CAIStateGroup_MinionReactToCriminal.h (PHero +0x14, PHome +0x1c, HeroInsideHouse +0x2d, MeInsideHouse
//          +0x2e, GuardInsideHouse +0x2f); PBrain is the first dword at +0x04.
#include "engine/CAIStateGroup_MinionReactToCriminal.h"

struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
struct C3DVector {
    float X, Y, Z;
};
struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad1[0x91 - 0x4c];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    const C3DVector& GetPosition();             // 0x004C73D0
    bool IsPointInside(const C3DVector& point); // 0x0082D000 (CThingBuilding)
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
struct CBaseIntelligentPointer {
    void* __vftable;
    CThing* m_pItem;                            // +0x04
    CThing* GetPItem() const;                   // 0x00A01B50
};
__forceinline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}
struct CPointerList {                           // STLport list<CIntelligentPointer<CThingAICreature>>
    struct SNode { SNode* m_pNext; SNode* m_pPrev; CBaseIntelligentPointer m_value; };
    SNode* m_pHead;
    struct iterator {
        SNode* m_pNode;
        iterator(SNode* node) : m_pNode(node) { }
        CBaseIntelligentPointer& operator*() const { return m_pNode->m_value; }
        iterator& operator++() { m_pNode = m_pNode->m_pNext; return *this; }
        bool operator!=(const iterator& o) const { return m_pNode != o.m_pNode; }
    };
    iterator begin() { return iterator(m_pHead->m_pNext); }
    iterator end() { return iterator(m_pHead); }
};
struct CVillage {
    unsigned char _pad[0x2c];
    CPointerList Members;                       // +0x2c (descriptive)
};
struct CTCVillageMember {                       // interface 0x23
    CVillage* GetPVillage() const;              // 0x007EE540
};
struct CAIBrain {
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};

struct CAIStateGroupMinionReactToCriminalMethods : CAIStateGroup_MinionReactToCriminal {
    CAIBrain* Brain() const { return *(CAIBrain* const*)_pad_0x04; }
    CThing* GetCreature() const { return Brain()->PCreature; }
    CBaseIntelligentPointer& Hero() { return *(CBaseIntelligentPointer*)PHero; }
    CBaseIntelligentPointer& Home() { return *(CBaseIntelligentPointer*)PHome; }
    bool IsValid();                             // 0x00930EE0
    bool IsStillValid();
};

bool CAIStateGroupMinionReactToCriminalMethods::IsStillValid()
{
    if (IS_THING_ALIVE(Home())) {
        CThing* creature = GetCreature();
        MeInsideHouse = Home().GetPItem()->IsPointInside(creature->GetPosition());
        HeroInsideHouse = IS_THING_ALIVE(Hero()) && Home().GetPItem()->IsPointInside(Hero().GetPItem()->GetPosition());
        GuardInsideHouse = false;
        CTCVillageMember* member;
        if (GetCreature()->GetInterface(&member, 0x23) && member->GetPVillage()) {
            CPointerList& members = member->GetPVillage()->Members;
            for (CPointerList::iterator it = members.begin(); it != members.end(); ++it) {
                CBaseIntelligentPointer& other = *it;
                if (IS_THING_ALIVE(other) && other.GetPItem()->HasTC(0xb4)
                    && Home().GetPItem()->IsPointInside(other.GetPItem()->GetPosition())) {
                    GuardInsideHouse = true;
                    break;
                }
            }
        }
    }
    return IsValid();
}
