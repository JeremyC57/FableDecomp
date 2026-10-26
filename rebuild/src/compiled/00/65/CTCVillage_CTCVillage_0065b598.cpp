// CTCVillage::~CTCVillage() -- detach every live villager from this village (village-member interface 35, SetPVillage(0)), then destroy the members in reverse order and chain to the CTCBase destructor
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0065B598, length 325 bytes; size TU (unaligned entry): `optimize("s",on)`.
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50; IS_THING_ALIVE (__fastcall, `T* const&`) 0x0040DCE0;
//   std::bitset<>::test(size_t) const 0x00410DE0 (the thing's interface bitset at +0x20); CVectorMap<ETCInterfaceType,
//   CTCBase*>::LowerBound 0x0040F020 (inlined GetTC, key homed in the IS_THING_ALIVE temporary's slot);
//   CTCVillageMember::SetPVillage(CTCVillage*) 0x007EE270; ~CBaseIntelligentPointer 0x00A01C10; the member containers'
//   destructors (catalog labels "CActiveFile::OnReadFinished" are propagated): list<CIntelligentPointer<CThing>> 0x004B8940,
//   list<CIntelligentPointer<CThingMarker>> 0x0065F300, set<long> 0x004AC140, the six task lists 0x006605FA .. 0x0066066D,
//   list<CIntelligentPointer<CThingTrackNode>> 0x0065F22C, list<CIntelligentPointer<CThingObject>> 0x0065F1FA,
//   list<CIntelligentPointer<CThingAICreature>> 0x00512890, list<CIntelligentPointer<CThingBuilding>> 0x0065F1C8;
//   CTCBase::~CTCBase 0x00686830 (manifest "~CChunkedFileChunk" is propagated).
// Layout: engine/CTCVillage.h carries the PDB offsets with 12-byte lists; retail STLport lists are the bare 4-byte head
//   pointer (as in the landed AddTorchToVillage 0x006592E2: VillageTorches +0x30) and the set is 12 bytes, so retail is
//   VillageBuildings +0x28 .. VillageDummies +0x74 and PDB-0x8c from Initialised (PDef +0x94, GuardFollowingPlayer +0xBD,
//   GuardWarningHero +0xDC; the landed OnKill 0x0065B0B4 uses the same -0x8c shift). Local model for that reason.
// Shape: the villager walk re-reads the list head for the end test; the member pointer is uninitialised when the
//   interface is absent (retail calls SetPVillage on the stale [ebp-8]); the CDefPointer release is inline
//   (`dec [ecx+4] ; jne ; call [eax+4]` then `and [edi],0`).
#pragma optimize("s", on)
class CTCBase;
class CBaseObject;
class CTCVillage;

struct CBitset {
    bool test(unsigned long position) const;    // 0x00410DE0
};
struct CInterfaceMap {                          // CVectorMap<ETCInterfaceType, CTCBase*> at CThing+0x44: {first, last}
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
class CThing {
public:
    unsigned char _pad00[0x20];
    CBitset       m_present;                    // +0x20
    unsigned char _pad24[0x20];
    CInterfaceMap m_interfaces;                 // +0x44
    bool HasTC(long type) const { return m_present.test(type); }
    __forceinline CTCBase* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        if (it == map->last || interfaceType < it->type)
            it = map->last;
        return it->component;
    }
};
class CThingAICreature : public CThing {};
bool __fastcall IS_THING_ALIVE(CThing* const& thing);   // 0x0040DCE0 (bound through a converted CThing* temporary)

class CTCVillageMember { public:
    void SetPVillage(CTCVillage* village);      // 0x007EE270
};

struct CBaseIntelligentPointer {                // vptr + item (+ link words)
    void* _vfptr;
    CBaseObject* m_pItem;
    CBaseObject* GetPItem() const;              // 0x00A01B50
    ~CBaseIntelligentPointer();                 // 0x00A01C10
};

struct SPersonNode {                            // STLport list node: next, prev, value
    SPersonNode* _M_next;
    SPersonNode* _M_prev;
    CBaseIntelligentPointer _M_data;            // CIntelligentPointer<CThingAICreature>
};

struct CThingPointerList          { void* _M_node; ~CThingPointerList(); };          // 0x004B8940 list<CIntelligentPointer<CThing>>
struct CMarkerPointerList         { void* _M_node; ~CMarkerPointerList(); };         // 0x0065F300 list<CIntelligentPointer<CThingMarker>>
struct CCrimeSet                  { void* _M_header[3]; ~CCrimeSet(); };              // 0x004AC140 set<long>
struct CCrateTaskList             { void* _M_node; ~CCrateTaskList(); };             // 0x0066066D list<CCountedPointer<CVillageTaskCarryCrate>>
struct CSecuritySweepTaskList     { void* _M_node; ~CSecuritySweepTaskList(); };     // 0x00660656 list<CCountedPointer<CVillageTaskSecuritySweep>>
struct CCropTaskList              { void* _M_node; ~CCropTaskList(); };              // 0x0066063F list<CCountedPointer<CVillageTaskPickCrops>>
struct CPatrolTaskList            { void* _M_node; ~CPatrolTaskList(); };            // 0x00660628 list<CCountedPointer<CVillageTaskPatrol>>
struct CHornTaskList              { void* _M_node; ~CHornTaskList(); };              // 0x00660611 list<CCountedPointer<CVillageTaskHorn>>
struct CTorchTaskList             { void* _M_node; ~CTorchTaskList(); };             // 0x006605FA list<CCountedPointer<CVillageTaskTorch>>
struct CTrackNodePointerList      { void* _M_node; ~CTrackNodePointerList(); };      // 0x0065F22C list<CIntelligentPointer<CThingTrackNode>>
struct CObjectPointerList         { void* _M_node; ~CObjectPointerList(); };         // 0x0065F1FA list<CIntelligentPointer<CThingObject>>
struct CCreaturePointerList {                   // list<CIntelligentPointer<CThingAICreature>>
    SPersonNode* _M_node;
    ~CCreaturePointerList();                    // 0x00512890
};
struct CBuildingPointerList       { void* _M_node; ~CBuildingPointerList(); };       // 0x0065F1C8 list<CIntelligentPointer<CThingBuilding>>

struct CDefPointee {
    virtual void v00();
    virtual void Destroy();                     // slot 1
    long RefCount;                              // +0x04
    void Release()
    {
        if (--RefCount == 0)
            Destroy();
    }
};
struct CVillageDefPointer {                     // CDefPointer<const CVillageDef>
    CDefPointee* Object;
    __forceinline ~CVillageDefPointer()
    {
        if (Object) {
            Object->Release();
            Object = 0;
        }
    }
};

class CTCBase {
public:
    virtual ~CTCBase();                         // 0x00686830
    unsigned char _pad_0x04[0x8];
};

#pragma pack(push, 1)
class CTCVillage : public CTCBase { public:                   // vtable 0x0125A9AC
    long  TorchTasksUpdateFrame;                // +0x0c
    long  FrameStartedSecuritySweep;            // +0x10
    float PositionPlayerWasInWhenSecuritySweepCalculated[3];   // +0x14
    long  LastAweCrowdEventFrame;               // +0x20
    long  LastLynchCrowdEventFrame;             // +0x24
    CBuildingPointerList   VillageBuildings;    // +0x28
    CCreaturePointerList   VillagePeople;       // +0x2c
    CThingPointerList      VillageTorches;      // +0x30
    CThingPointerList      VillageHorns;        // +0x34
    CThingPointerList      VillageGates;        // +0x38
    CObjectPointerList     VillageCrops;        // +0x3c
    CThingPointerList      VillageCages;        // +0x40
    CThingPointerList      VillageCampfires;    // +0x44
    CTrackNodePointerList  VillagePatrols;      // +0x48
    CTorchTaskList         TorchTasks;          // +0x4c
    CHornTaskList          HornTasks;           // +0x50
    CPatrolTaskList        PatrolTasks;         // +0x54
    CCropTaskList          CropTasks;           // +0x58
    CSecuritySweepTaskList SecuritySweepTasks;  // +0x5c
    CCrateTaskList         CrateTasks;          // +0x60
    CCrimeSet              PlayersCrimes;       // +0x64
    CMarkerPointerList     VillageMarkers;      // +0x70
    CThingPointerList      VillageDummies;      // +0x74
    unsigned char          _pad_0x78[0x1c];     // Initialised .. TimeIsPaused
    CVillageDefPointer     PDef;                // +0x94
    unsigned char          _pad_0x98[0x25];     // StanceToHero .. TeacherTeaching
    CBaseIntelligentPointer GuardFollowingPlayer;   // +0xbd
    unsigned char          _pad_0xc5[0x17];
    CBaseIntelligentPointer GuardWarningHero;   // +0xdc
    bool                   CurrentIsHeroCriminal;   // +0xe4
    unsigned char          _pad_0xe5[3];
    virtual ~CTCVillage();
};
#pragma pack(pop)

CTCVillage::~CTCVillage()
{
    for (SPersonNode* node = VillagePeople._M_node->_M_next; node != VillagePeople._M_node; node = node->_M_next) {
        CThingAICreature* person = (CThingAICreature*)node->_M_data.GetPItem();
        if (IS_THING_ALIVE(person)) {
            CTCVillageMember* member;
            if (person->HasTC(35))
                member = (CTCVillageMember*)person->GetTC(35);
            member->SetPVillage(0);
        }
    }
}