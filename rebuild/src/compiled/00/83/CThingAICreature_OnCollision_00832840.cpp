// CThingAICreature::OnCollision(ECollideType type, C3DVector* position, C3DVector* normal, CThing* other) -- base collision handling, then (both alive) a collision with the thing the creature perceives makes it directly aware of it
// Leaf: OnCollision
// Retail: Fable.exe (TLC, VC7.1) @ 0x00832840, 141 bytes (`ret 10h`).
// Identity: slot 41 of vtable 0x0127293C = CThingAICreature (rtti-vtables.tsv; manifest "vfunc_41"); the same slot of
//   CThingCreatureBase (0x0125AC0C) is 0x00667310 = CThingCreatureBase::OnCollision, called first with the same arguments.
// Callees: CThingCreatureBase::OnCollision @ 0x00667310, CInterfaceMap::LowerBound @ 0x0040F020,
//   CTCPerceiveThing::GetPerceivedThing @ 0x007446A0 (label "_Cons_val<...>" propagated),
//   CTCPerceiveThing::SetAsDirectlyAwareOfPerceivedThing @ 0x007449B0, CThing::PeekWorld @ 0x004C7AA0 (a world getter
//   whose result is unused; label "CViewBase::GetDisplayEngine" propagated).
// Layout: retail CThing: Dead = +0x91 bit 0, interface bitset +0x20 (TC 0x34 = CTCPerceiveThing: word +0x24 bit 20),
//   CVectorMap +0x44.
// Shape: the agent160 GetTC model with the component in a named local (esi) used twice; the type test sits inside the
//   alive block.
struct C3DVector { float X, Y, Z; };
enum ECollideType { COLLIDE_NONE = 0, COLLIDE_LANDSCAPE = 1 };
struct CTCBase;
struct CInterfaceMap {                          // CVectorMap<ETCInterfaceType, CTCBase*> at CThing+0x44: {first, last}
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
struct CWorld;
struct CThing;
struct CTCPerceiveThing {
    CThing* GetPerceivedThing();                // 0x007446A0
    void SetAsDirectlyAwareOfPerceivedThing();  // 0x007449B0
};

struct CThing {
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char Dead : 1;                     // +0x91 bit 0
    CWorld* PeekWorld();                        // 0x004C7AA0
    template<class T> __forceinline T* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        if (it == map->last || interfaceType < it->type)
            it = map->last;
        return (T*)it->component;
    }
    bool HasTC(long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31)))
            return true;
        return false;
    }
};

struct CThingCreatureBase : CThing {
    void OnCollision(ECollideType type, C3DVector* position, C3DVector* normal, CThing* other);     // 0x00667310
};

struct CThingAICreature : CThingCreatureBase {
    void OnCollision(ECollideType type, C3DVector* position, C3DVector* normal, CThing* other);
};

void CThingAICreature::OnCollision(ECollideType type, C3DVector* position, C3DVector* normal, CThing* other)
{
    CThingCreatureBase::OnCollision(type, position, normal, other);
    if (!Dead && other && !other->Dead) {
        if (HasTC(0x34)) {
            CTCPerceiveThing* perceive = GetTC<CTCPerceiveThing>(0x34);
            if (perceive->GetPerceivedThing() == other)
                perceive->SetAsDirectlyAwareOfPerceivedThing();
        }
        if (type == COLLIDE_LANDSCAPE)
            PeekWorld();
    }
}