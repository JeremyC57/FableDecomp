// CCreatureAction_ScorpionSummon::CreateBabies() -- find free spots within 3.0 of the king scorpion; with none, do
// Grade: DIFFER(723v722) nd 103->61 -- lever: the family name as a named CCharString in its own block scope (retail re-takes the string's address, `lea eax,[esp+10h] ; push eax`, instead of the ctor's eax). Tried boss/family declared early, tools inlined (no gain). Residue: frame slots familyIndex/tools/family def shifted by one word (retail 0x18/0x1c/0x10) and one byte longer. Next: find the slot that should share the name string's word
// Leaf: CreateBabies
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008C4FD0, length 722 bytes, plain `ret`.
// Callees: CDefinitionManager::Get 0x0044C6B0 (static) / GetDefGlobalIndex 0x009AD410 / GetDef<CCreatureGenerationFamilyDef>
//   0x0074D220, GetPWorld 0x00692940, world slot 12 (+0x30) world map, GetValidPositions 0x0074A5D0 (__fastcall),
//   GetPItem 0x00A01B50, CThing::GetPosition 0x004C73D0, LowerBound 0x0040F020, CTCBoss::CountLivingHelperCreatures
//   0x007AEEC0 / SetHelperCreature 0x007AF310, GetRandomSeed 0x0049D810, GFROR13 0x00497890 (__fastcall),
//   CThingAICreature::Create 0x00833800 (__fastcall), CCreatureAction_RiseUp ctor 0x008BD880 / dtor 0x0062E900,
//   SetCurrentAction 0x006644F0, CCharString 0x0099EBF0 / copy 0x0099EC30 / =(const char*) 0x0099EFE0 / ~ 0x0099EAE0,
//   free 0x00BFEA14.
// Layout: CThingAICreatureInit and CCreatureGenerationFamilyDef member names from ghidra_out/struct_layouts_egor.tsv,
//   offsets from retail bytes (family Creatures vector +0x3c/+0x40; world +0x20 search tools; thing +0x91 bit 0 dead;
//   boss TC interface 0xad); the rise-up action's 0x118 bytes is the retail frame room above it.
extern "C" void __cdecl free(void* block);                   // 0x00BFEA14
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);         // 0x0099EBF0
    CCharString(const CCharString& other);                   // 0x0099EC30
    ~CCharString();                                          // 0x0099EAE0
    CCharString& operator=(const char* text);                // 0x0099EFE0
};
struct C3DVector {
    float X, Y, Z;
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }
};
unsigned long __fastcall GFROR13(unsigned long value);       // 0x00497890
inline unsigned long GFRandom(unsigned long* seed)
{
    *seed = *seed * 0x24a1 + 0x24df;
    *seed = GFROR13(*seed);
    return *seed;
}
struct CPositionVector {                                     // STLport vector<C3DVector>
    C3DVector* _M_start;
    C3DVector* _M_finish;
    C3DVector* _M_end_of_storage;
    CPositionVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    ~CPositionVector() { if (_M_start) free(_M_start); }
    unsigned long size() const { return _M_finish - _M_start; }
    C3DVector& operator[](unsigned long i) { return _M_start[i]; }
};
struct CDefBase {
    virtual void v00();
    virtual void Destroy();                                  // slot 1
    long RefCount;                                           // +4
};
template <class T> struct CDefPointer {
    T* Object;
    CDefPointer() : Object(0) { }
    ~CDefPointer() { if (Object && --Object->RefCount == 0) Object->Destroy(); }
    T* operator->() const { return Object; }
};
struct CCreatureGenerationFamilyDef : CDefBase {
    unsigned char _pad08[0x3c - 8];
    long*         CreaturesStart;                            // +0x3c vector<long> Creatures
    long*         CreaturesFinish;                           // +0x40
};
struct CDefinitionManager {
    static CDefinitionManager* Get();                        // 0x0044C6B0
    long GetDefGlobalIndex(const CCharString& name);         // 0x009AD410
    bool GetDef(long index, CDefPointer<CCreatureGenerationFamilyDef>* out);   // 0x0074D220
};
struct CWorldMap;
struct CThingSearchTools;
struct CTCShapeManager;
void __fastcall GetValidPositions(CPositionVector* out, CWorldMap* map, CThingSearchTools* tools,
                                  const C3DVector* position, float radius, bool flag, CTCShapeManager* shapes);   // 0x0074A5D0
struct CWorld {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual CWorldMap* GetWorldMap();                        // slot 12 (+0x30)
    unsigned char      _pad04[0x20 - 4];
    CThingSearchTools* SearchTools;                          // +0x20
    unsigned long* GetRandomSeed();                          // 0x0049D810
};
struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);                    // 0x0040F020
};
class CThingAICreature;
struct CTCBoss {
    long CountLivingHelperCreatures();                       // 0x007AEEC0
    void SetHelperCreature(CThingAICreature* creature);      // 0x007AF310
};
class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                          // +0x20
    CInterfaceMap m_interfaces;                              // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char m_flags91;                                 // +0x91 bit 0: dead
    const C3DVector* GetPosition() const;                    // 0x004C73D0
    template <class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *ppOut = (T*)(it == map->last || interfaceType < it->type ? map->last : it)->component;
            return true;
        }
        return false;
    }
};
struct CThingBuilding;
struct CThingPhysical;
struct CThingAICreatureInit {
    CThingPhysical* OriginalThing;                           // +0x00
    CThingPhysical* PreviousThing;                           // +0x04
    CCharString     ScriptName;                              // +0x08
    CCharString     OverridingBrainName;                     // +0x0c
    bool            PlayerCharacter;                         // +0x10
    bool            Drawable;                                // +0x11
    bool            PlaceAtEndOfList;                        // +0x12
    bool            EnableCreatureAutoPlacing;               // +0x13
    CThingBuilding* HomeBuilding;                            // +0x14
    CThingBuilding* WorkBuilding;                            // +0x18
    CThingAICreatureInit(const CCharString& scriptName = CCharString(""),
                         const CCharString& brainName = CCharString(""))
        : OriginalThing(0), PreviousThing(0), ScriptName(scriptName), OverridingBrainName(brainName),
          PlayerCharacter(false), Drawable(true), PlaceAtEndOfList(true), EnableCreatureAutoPlacing(false),
          HomeBuilding(0), WorkBuilding(0) { }
};
struct CCreatureActionBase;
class CThingAICreature : public CThing {
public:
    static CThingAICreature* __fastcall Create(long defIndex, const C3DVector* position, long player,
                                               CThingAICreatureInit* init);   // 0x00833800
    bool SetCurrentAction(const CCreatureActionBase& action);   // 0x006644F0
};
struct CCreatureAction_RiseUp {
    unsigned char _data[0x118];
    CCreatureAction_RiseUp(CThing* creature);                // 0x008BD880
    ~CCreatureAction_RiseUp();                               // 0x0062E900
    operator const CCreatureActionBase&() const { return *(const CCreatureActionBase*)this; }
};
struct CBaseIntelligentPointer {
    void*   __vftable;
    CThing* m_pItem;
    CThing* GetPItem() const;                                // 0x00A01B50
};
struct CCreatureAction_ScorpionSummon {
    void*                   __vftable;
    unsigned char           _pad04[4];
    CBaseIntelligentPointer PCreature;                       // +0x08
    CThing* GetCreature() const { return PCreature.GetPItem(); }
    CWorld* GetPWorld() const;                               // 0x00692940
    void CreateBabies();
};

void CCreatureAction_ScorpionSummon::CreateBabies()
{
    long familyIndex;
    {
        CCharString familyName("KING_SCORPION_SUMMON_FAMILY");
        familyIndex = CDefinitionManager::Get()->GetDefGlobalIndex(familyName);
    }
    CPositionVector positions;
    CWorld* world = GetPWorld();
    CThingSearchTools* tools = GetPWorld()->SearchTools;
    GetValidPositions(&positions, world->GetWorldMap(), tools, GetCreature()->GetPosition(), 3.0f, false, 0);
    if (positions.size() == 0)
        return;
    CDefPointer<CCreatureGenerationFamilyDef> family;
    if (CDefinitionManager::Get()->GetDef(familyIndex, &family)) {
        CTCBoss* boss;
        GetCreature()->GetInterface(&boss, 0xad);
        long count = (family->CreaturesFinish - family->CreaturesStart) - boss->CountLivingHelperCreatures();
        long i = 0;
        do {
            unsigned long n = positions.size();
            unsigned long r = GFRandom(GetPWorld()->GetRandomSeed());
            C3DVector position = positions[n ? r % n : 0];
            long creatureDef = family->CreaturesStart[i++];
            CThingAICreatureInit init;
            init.ScriptName = "SUMMONED_CREATURE";
            CThingAICreature* creature = CThingAICreature::Create(creatureDef, &position, 4, &init);
            if (creature && !(creature->m_flags91 & 1)) {
                creature->SetCurrentAction(CCreatureAction_RiseUp(creature));
                boss->SetHelperCreature(creature);
            }
        } while (i < count);
    }
}
