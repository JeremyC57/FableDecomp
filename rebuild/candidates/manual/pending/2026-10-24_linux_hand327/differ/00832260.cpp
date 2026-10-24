// CThingAICreature::InitialActivate() -- after the base activation, resolve the saved relationship UIDs (home and work buildings, father, mother, spouse) into live intelligent pointers, registering the parents and spouse with the creature's family component (0x57) and Me with each parent's, then drop the saved block and clear the brain's pending auto-placement flag
// Leaf: ?InitialActivate@CThingAICreature
// Grade: DIFFER(ours v retail) -- 1052v1076: register allocation around the family component -- retail keeps the thing
//   manager in ebx across IsLevelLess, spills the family pointer to the lookup-key word ([esp+14h], frame 0x10) and re-reads
//   the low UID dword after the != 0 test; ours keeps the family pointer in ebx, the manager in edi and both UID halves in
//   registers (frame 8). Tried: plain inline helpers (out of line, 685v1076), __forceinline helpers/ctors, the UID by const&.
// Retail: Fable.exe (TLC, VC7.1) @ 0x00832260, 1076 bytes (+ int3 padding), plain `ret`; virtual InitialActivate of CThingAICreature.
// Callees: the base InitialActivate 0x008389E0 (shared by thirteen thing vtables; catalog label "AddCombinationBlend" is
//   propagated), CThing::IsLevelLess 0x004C7930, CThingManager::RemapUIDForSerialisation 0x0051E210 / GetThingWithUID
//   0x0051ED80 (g_pThingManager 0x013B8A1C), the interface-map LowerBound 0x0040F020, CBaseIntelligentPointer ctor 0x00A01B10 /
//   SetPItem 0x00A01B90 / GetPItem 0x00A01B50 / dtor 0x00A01C10 (CIntelligentPointer<CThing> vtable 0x01244344,
//   CIntelligentPointer<CThingAICreature> vtable 0x0123E7E0), the family component's Add 0x00778B30 (landed as
//   CPointerListOwner_00778b30::Add), the brain's auto-placement reset 0x0088B6E0, operator delete 0x00BFE9BC.
// Layout: retail CThingAICreature (Ego_r.pdb less 0x28 in this range): RelationshipsInit (CCountedPointer) +0x160/+0x164,
//   HomeBuilding +0x168, WorkBuilding +0x170, FatherCreature +0x178, MotherCreature +0x180, SpouseCreature +0x188, PBrain +0x1a8,
//   EnableCreatureAutoPlacing bit 7 of +0x1c8. CRelationshipsInit: five ulong64 UIDs (home, work, father, mother, spouse).
//   CThing: presence bits +0x20, interface map +0x44, level +0x8e, dead bit 0 of +0x91.
// Shape: each relative is `uid ? GetThingWithUID(Remap(false, IsLevelLess(), level, uid))` (the manager loaded first) and,
//   when alive, assigned through an intelligent-pointer temporary built from the raw pointer (the landed ConstructFromParams
//   0x00830360 model); the family component is `p = 0; if (HasTC) p = GetTC` (a spilled local sharing its slot with the
//   lookup keys).
struct CBaseObject;
class CTCBase;
struct CListItem;
struct CInterfaceMap {                          // CVectorMap<ETCInterfaceType, CTCBase*> at CThing+0x44
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
struct CPointerListOwner_00778b30 {             // the family component (TC 0x57): a pointer list at +0xc
    void Add(const CListItem* item);            // 0x00778B30
};
class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad4c[0x8e - 0x4c];
    short         m_level;                      // +0x8e
    unsigned char _pad90;                       // +0x90
    unsigned char Flags91;                      // +0x91 bit 0 Dead
    bool IsLevelLess();                         // 0x004C7930
    bool IsDead() const { return (Flags91 & 1) != 0; }
    unsigned long HasTC(long type) const { return m_presentBits[type >> 5] & (1UL << (type & 31)); }
    __forceinline CTCBase* GetTC(long type)
    {
        long key = type;
        CInterfaceMap::SEntry* it = m_interfaces.LowerBound(&key);
        if (it == m_interfaces.last || type < it->type)
            it = m_interfaces.last;
        return it->component;
    }
};
struct CThingManager {
    unsigned __int64 RemapUIDForSerialisation(bool saving, bool levelLess, long level, unsigned __int64 uid);   // 0x0051E210
    CThing* GetThingWithUID(unsigned __int64 uid);   // 0x0051ED80
};
extern CThingManager* g_pThingManager;          // 0x013B8A1C
void operator delete(void* p);                   // 0x00BFE9BC (MSVCR71)

struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* PItem;                              // +0x04
    CBaseIntelligentPointer();                  // 0x00A01B10
    ~CBaseIntelligentPointer();                 // 0x00A01C10
    CThing* GetPItem() const;                   // 0x00A01B50
    void SetPItem(CThing* item);                // 0x00A01B90
};
struct CIntelligentPointerThing : CBaseIntelligentPointer {          // CIntelligentPointer<CThing>, vtable 0x01244344
    virtual void v00();
    __forceinline CIntelligentPointerThing(CThing* item) : CBaseIntelligentPointer() { SetPItem(item); }
    CIntelligentPointerThing& operator=(const CIntelligentPointerThing& other) { SetPItem(other.GetPItem()); return *this; }
};
struct CIntelligentPointerAICreature : CBaseIntelligentPointer {     // CIntelligentPointer<CThingAICreature>, vtable 0x0123E7E0
    virtual void v00();
    __forceinline CIntelligentPointerAICreature(CThing* item) : CBaseIntelligentPointer() { SetPItem(item); }
    CIntelligentPointerAICreature& operator=(const CIntelligentPointerAICreature& other) { SetPItem(other.GetPItem()); return *this; }
};
struct CCPPointerInfo {
    long RefCount;
    void (__fastcall* DeleteFunc)(void* data);
    void* Data;
};
template <class T> struct CCountedPointer {
    T*              Data;
    CCPPointerInfo* Info;
    void Reset()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
        Info = 0;
        Data = 0;
    }
};
struct CAIBrain {
    void ClearPendingAutoPlacement();           // 0x0088B6E0 (clears the brain's byte +0x8f)
};

class CThingGameObject : public CThing {
public:
    void InitialActivate();                     // 0x008389E0
};
class CThingAICreature : public CThingGameObject {
public:
    struct CRelationshipsInit {
        unsigned __int64 HomeBuildingUID;       // +0x00
        unsigned __int64 WorkBuildingUID;       // +0x08
        unsigned __int64 FatherCreatureUID;     // +0x10
        unsigned __int64 MotherCreatureUID;     // +0x18
        unsigned __int64 SpouseCreatureUID;     // +0x20
    };
    unsigned char _pad94[0x160 - 0x94];
    CCountedPointer<CRelationshipsInit> RelationshipsInit;   // +0x160
    CIntelligentPointerThing      HomeBuilding;              // +0x168
    CIntelligentPointerThing      WorkBuilding;              // +0x170
    CIntelligentPointerAICreature FatherCreature;            // +0x178
    CIntelligentPointerAICreature MotherCreature;            // +0x180
    CIntelligentPointerAICreature SpouseCreature;            // +0x188
    unsigned char _pad190[0x1a8 - 0x190];
    CAIBrain*     PBrain;                                    // +0x1a8 (CCountedPointer<CAIBrain>::Data)
    unsigned char _pad1ac[0x1c8 - 0x1ac];
    bool PerformingVillageTaskFlag : 1;                      // +0x1c8
    bool AISuspended : 1;
    bool HasInformation : 1;
    bool WanderWithInformation : 1;
    bool WaveWithInformation : 1;
    bool ContinueAIWithInformation : 1;
    bool ImmuneToAttack : 1;
    bool EnableCreatureAutoPlacing : 1;                      // bit 7
    __forceinline CThing* GetThingFromSavedUID(unsigned __int64 uid)
    {
        CThingManager* manager = g_pThingManager;
        return g_pThingManager->GetThingWithUID(manager->RemapUIDForSerialisation(false, IsLevelLess(), m_level, uid));
    }
    void InitialActivate();
};

void CThingAICreature::InitialActivate()
{
    CThingGameObject::InitialActivate();
    CRelationshipsInit* init = RelationshipsInit.Data;
    if (init) {
        CPointerListOwner_00778b30* family = 0;
        if (HasTC(0x57))
            family = (CPointerListOwner_00778b30*)GetTC(0x57);
        if (init->HomeBuildingUID != 0) {
            CThing* thing = GetThingFromSavedUID(init->HomeBuildingUID);
            if (thing && !thing->IsDead())
                HomeBuilding = CIntelligentPointerThing(thing);
        }
        if (RelationshipsInit.Data->WorkBuildingUID != 0) {
            CThing* thing = GetThingFromSavedUID(RelationshipsInit.Data->WorkBuildingUID);
            if (thing && !thing->IsDead())
                WorkBuilding = CIntelligentPointerThing(thing);
        }
        if (RelationshipsInit.Data->FatherCreatureUID != 0) {
            CThing* thing = GetThingFromSavedUID(RelationshipsInit.Data->FatherCreatureUID);
            if (thing && !thing->IsDead()) {
                FatherCreature = CIntelligentPointerAICreature(thing);
                if (family)
                    family->Add((CListItem*)FatherCreature.GetPItem());
                CThing* father = FatherCreature.GetPItem();
                if (father->HasTC(0x57))
                    ((CPointerListOwner_00778b30*)father->GetTC(0x57))->Add((CListItem*)this);
            }
        }
        if (RelationshipsInit.Data->MotherCreatureUID != 0) {
            CThing* thing = GetThingFromSavedUID(RelationshipsInit.Data->MotherCreatureUID);
            if (thing && !thing->IsDead()) {
                MotherCreature = CIntelligentPointerAICreature(thing);
                if (family)
                    family->Add((CListItem*)MotherCreature.GetPItem());
                CThing* mother = MotherCreature.GetPItem();
                if (mother->HasTC(0x57))
                    ((CPointerListOwner_00778b30*)mother->GetTC(0x57))->Add((CListItem*)this);
            }
        }
        if (RelationshipsInit.Data->SpouseCreatureUID != 0) {
            CThing* thing = GetThingFromSavedUID(RelationshipsInit.Data->SpouseCreatureUID);
            if (thing && !thing->IsDead()) {
                SpouseCreature = CIntelligentPointerAICreature(thing);
                if (family)
                    family->Add((CListItem*)SpouseCreature.GetPItem());
            }
        }
    }
    RelationshipsInit.Reset();
    if (EnableCreatureAutoPlacing && PBrain)
        PBrain->ClearPendingAutoPlacement();
}
