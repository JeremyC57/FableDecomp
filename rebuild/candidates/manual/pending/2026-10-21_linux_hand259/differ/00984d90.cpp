// CAIStateGroup_OpinionReactionRoot::FleeIsRunnable() -- villagers outside a safe region flee hostile things; otherwise flee only within 12 units
// Leaf: FleeIsRunnable
// Grade: DIFFER(455v455) -- one register pair after 4 grades: retail loads the creature into esi for the 0x23 villager lookup and turns it into the map with `add esi,44h` (ours `mov eax,...` + `lea esi,[eax+44h]`; addressing m_interfaces directly gives lea ecx / [esi+48h]); everything else, incl. the three conditionally-destroyed CCountedPointer temporaries (ebx flag mask), matches
// Retail: Fable.exe (TLC, VC7.1) @ 0x00984D90; best of the probes graded under VC7.1 RTM (probe/00984d90b.cpp).
struct CTCBase;
struct C3DVector { float x, y, z; };
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type) const; // 0x004365B0 (const copy)
};
struct CThing {
    unsigned char _pad0[0x10];
    unsigned char OpinionCategory;              // +0x10 (descriptive)
    unsigned char _pad11[0x20 - 0x11];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    const C3DVector& GetPosition() const;       // 0x004C73D0
    long GetTeam() const;                       // 0x00662000
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            const CInterfaceMap* map = &m_interfaces;
            const CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
};
extern "C" double __cdecl sqrt(double);
#pragma intrinsic(sqrt)
struct CTCVillage {
    unsigned char _pad[0x7c];
    bool UnderAttack;                           // +0x7c
};
struct CTCVillager;
CTCVillage* __fastcall GFGetPVillageInterface(CTCVillager* villager);    // 0x007EE540
struct CRegionDef {
    virtual void v00();
    virtual void Destroy();
    long m_refs;                                // +0x04
    unsigned char _pad[0x70 - 8];
    bool IsSafe;                                // +0x70
    bool NoFlee;                                // +0x71
};
struct CCountedRegionDef {
    CRegionDef* m_p;
    __forceinline CCountedRegionDef(const CCountedRegionDef& o) : m_p(o.m_p) { if (m_p) ++m_p->m_refs; }
    __forceinline ~CCountedRegionDef() { if (m_p && --m_p->m_refs == 0) m_p->Destroy(); }
    CRegionDef* operator->() const { return m_p; }
    operator bool() const { return m_p != 0; }
};
struct CRegion {
    unsigned char _pad[0x24];
    CCountedRegionDef PDef;                     // +0x24
    CCountedRegionDef GetDef() const { return PDef; }
};
struct CWorldMap {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void v08(); virtual void v09(); virtual void v0a(); virtual void v0b();
    virtual void v0c(); virtual void v0d(); virtual void v0e(); virtual void v0f();
    virtual long GetMapNumberAt(const C3DVector& pos);   // slot 16 (+0x40)
    long GetRegionNumberMapIsIn(long map);      // 0x004FC190
    CRegion& GetRegion(long region);            // 0x004FC180
};
struct CWorld {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void v08(); virtual void v09(); virtual void v0a(); virtual void v0b();
    virtual CWorldMap* GetWorldMap();           // slot 12 (+0x30)
};
struct CThingPointerView {
    CThing* GetPItem() const;                   // 0x00A01B50 CBaseIntelligentPointer::GetPItem
};
struct CAIBrain {
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};
#pragma pack(push, 1)
struct CAIStateGroup_OpinionReactionRoot {
    void* __vftable;
    CAIBrain* PBrain;                           // +0x04
    unsigned char _pad08[0x6b - 0x08];
    unsigned char ThingReactingTo[9];           // +0x6b CIntelligentPointer<CThing>
    CWorld* GetPWorld();                        // 0x008FCFC0
    bool FleeIsRunnable();
};
#pragma pack(pop)

bool CAIStateGroup_OpinionReactionRoot::FleeIsRunnable()
{
    CThingPointerView* reactingTo = (CThingPointerView*)ThingReactingTo;
    if (reactingTo->GetPItem()->OpinionCategory == 2) {
        CTCVillager* villager;
        if (PBrain->PCreature->GetInterface(&villager, 0x23)) {
            CTCVillage* village = GFGetPVillageInterface(villager);
            if (village != 0 && !village->UnderAttack && PBrain->PCreature->GetTeam() != 1) {
                CWorldMap* map = GetPWorld()->GetWorldMap();
                CRegion& region = map->GetRegion(map->GetRegionNumberMapIsIn(map->GetMapNumberAt(PBrain->PCreature->GetPosition())));
                bool safe = region.GetDef() && region.GetDef()->IsSafe && region.GetDef()->NoFlee;
                if (safe)
                    return false;
            }
        }
    }
    CThing* creature = PBrain->PCreature;
    CThing* other = reactingTo->GetPItem();
    const C3DVector& from = creature->GetPosition();
    const C3DVector& to = other->GetPosition();
    float dx = to.x - from.x;
    float dy = to.y - from.y;
    float dz = to.z - from.z;
    if ((float)sqrt(dx * dx + dy * dy + dz * dz) < 12.0f)
        return true;
    return false;
}
