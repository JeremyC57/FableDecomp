// CTCCreatureGenerator::CreateCreatures(CThingCreatureBase* triggerer) -- spawn one generation wave: pick a random creature family, pick its generation type (open / appear / hidden ambush / undead), then for each creature def of the family that is enabled for its group spawn an AI creature at a random generation point (reachable from the hero when the generator follows the hero), mark it, give it its arrival action, track it and stop at the creation limit
// Grade: DIFFER(1152v1208) 217 diff lines (parked 1248/516): shared def model with FABLE_DEFPTR_INLINE = inline (the __forceinline form gives 1420v1208/408: every release expanded) + the retail GetSubDef<CCreatureDef> body same-TU noinline (0x00485815). Left: which CDefPointer members stay out of line vs expand (size 1152 v 1208), this in edi v esi, zero in ebx. PENDING ENullDef ctor (retry wave 2 sec2c)
// Was: DIFFER(1248v1208) -- frame 0x284 v 0x27c; ours holds 0 in ebx and the position vector in ebp (retail: immediates and ebx), this in edi (retail esi); family def kept in ebp only partly; structure (jump table, loop, releases) otherwise as retail
// Leaf: CreateCreatures
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0074B2B0, 1216 bytes, `ret 4`; a 5-way jump table on the generation type.
// Callees: LowerBound 0x0040F020 (hive 0x62) / const 0x004365B0 (navigation 7), CTCCreatureHive::Open 0x0077D230,
// HasHitCreationLimit 0x0074A190, CTCBase::GetWorld 0x00686A80 (const) / 0x00686A70, CWorld::GetPRandomSeed 0x0049D810,
// GFRandom 0x004CDAE0 (__fastcall), vector<long>::at 0x0074D120, CTCBase::GetDefinitionManager 0x006869D0,
// GetDef<CCreatureGenerationFamilyDef> 0x0074D220, CThing::GetPosition 0x004C73D0, CWorld::GetCurrentPlayer 0x0049D9D0,
// GetPItem 0x00A01B50, IS_THING_ALIVE<CThing*> 0x0043DBD0, CThing::GetDefinitionManager 0x004C7990, GetDef<CThingCreatureDef>
// 0x00513160, GetSubDef<CCreatureDef> 0x00485815, CWorld::IsCreatureGenerationEnabledForGroup 0x0049D910,
// CTCCreatureNavigation::IsAbleToNavigateFromPosToPos 0x006CB620, CThingAICreatureInit ctor 0x004FACD0 / dtor 0x004FAD50,
// CThingAICreature::Create 0x00833800 (__fastcall), SetAsGameStatePersisted 0x004C7880, CCreatureAction_Ambush ctor 0x008BFDA0,
// CCreatureActionAppear ctor 0x0074DBB0, SetCurrentAction 0x006644F0, ~CCombatActionBase 0x0062E900, CIntelligentPointer<CThing>
// ctor 0x004A96F0 / ~CBaseIntelligentPointer 0x00A01C10, list push_back 0x004BA4E0, CreateCreaturesUndead 0x0074AF50.
// Layout (retail, the landed UpdateUndead/ctor models): CreatureFamilies vector<long> +0x0c, CreatedCreatures list +0x18,
// ScriptName +0x48, OpenPositions +0x60, HiddenPositions +0x6c (12-byte STLport vectors), CreationCount +0x84, UseHeroPosition
// +0xa1; family def: generation types +0x50, creature defs +0x3c; creature def group +0xac; g_GeneratedCreatureCount 0x013BAD54.
struct C3DVector { float X, Y, Z; };
class CCharString { public:
    void* m_pData;
    CCharString(const char* text, long length = -1);         // 0x0099EBF0
    ~CCharString();                                          // 0x0099EAE0
};
extern long g_GeneratedCreatureCount;                        // 0x013BAD54
unsigned long __fastcall GFRandom(unsigned long range, unsigned long* seed);   // 0x004CDAE0

#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CDefClassBase.h"
#define FABLE_DEFPTR_INLINE inline
enum ENullDef { NULL_DEF };                                  // enumerator name unproven (in neither PDB name list)
template <class T> class CDefPointer {
public:
    CDefPointer(ENullDef) : Object(0) { }                               // PROPOSED header addition (FableWin ??0..QAE@W4ENullDef@@@Z)
    FABLE_DEFPTR_INLINE CDefPointer(const CDefPointer& other) : Object(other.Object) { if (Object) Object->AddRef(); }
    template <class U> FABLE_DEFPTR_INLINE CDefPointer(const CDefPointer<U>& other) : Object(other.InternalGetObject())
    {
        if (Object) Object->AddRef();
    }
    FABLE_DEFPTR_INLINE ~CDefPointer() { Release(); }
    FABLE_DEFPTR_INLINE CDefPointer& operator=(const CDefPointer& other) { Assign(other.Object); return *this; }
    template <class U> FABLE_DEFPTR_INLINE CDefPointer& operator=(const CDefPointer<U>& other) { Assign(other.InternalGetObject()); return *this; }
    template <class U> FABLE_DEFPTR_INLINE void SetDynamicCast(const CDefPointer<U>& other) { Assign(other.InternalGetObject()); }
    CDefPointeeBase* InternalGetObject() const { return Object; }
    T* Get() const { return static_cast<T*>(Object); }
    T* operator->() const { return Get(); }
    T& operator*() const { return *Get(); }
    bool IsNull() const { return Object == 0; }
    unsigned long GetRefCount() const { return Object->GetRefCount(); }
    void Reset(T*);                                                      // unproven body (FableWin ?Reset@..QAEXPBV..@Z)
    FABLE_DEFPTR_INLINE void Release()
    {
        if (Object) {
            CDefPointeeBase* object = Object;
            if (object->Release() == 0)
                object->Delete();
            Object = 0;
        }
    }
    CDefPointeeBase* Object;                                             // +0x00
private:
    FABLE_DEFPTR_INLINE void Assign(CDefPointeeBase* object)                                 // the inline body of operator= / SetDynamicCast
    {
        if (Object != object) {
            Release();
            Object = object;
            if (Object) Object->AddRef();
        }
    }
};

class CCreatureDef : public CDefClassBase {
public:
    unsigned char _pad25[0xac - 0x25];
    unsigned long GenerationGroup;                           // +0xac
};
class CThingCreatureDef : public CDefClassBase {};
struct CLongVector {
    long* _M_start;
    long* _M_finish;
    long* _M_end_of_storage;
    unsigned long size() const { return _M_finish - _M_start; }
    long& operator[](unsigned long n) const { return _M_start[n]; }
};
class CCreatureGenerationFamilyDef : public CDefClassBase {
public:
    unsigned char _pad25[0x3c - 0x25];
    CLongVector Creatures;                                   // +0x3c
    unsigned char _pad48[0x50 - 0x48];
    CLongVector GenerationTypes;                             // +0x50
};
class CDefinitionManager {
    friend class CDefClassBase;
public:
    bool GetDef(long index, CDefPointer<const CCreatureGenerationFamilyDef>* out) const;   // 0x0074D220
protected:
    CDefPointer<CDefClassBase> GetPDefFromGlobalIndex(long index);   // 0x009ADA10
};
// The retail GetSubDef<CCreatureDef> body (0x00485815), same-TU noinline (only the caller is graded).
template <>
__declspec(noinline) bool CDefClassBase::GetSubDef<CCreatureDef>(CDefPointer<const CCreatureDef>* out) const
{
    const CSubDefInfo* info = GetPSubDefInfo(CCharString("CCreatureDef", -1));
    if (info) {
        CDefPointer<const CDefClassBase> def = PDefManager->GetPDefFromGlobalIndex(info->DefIndex);
        out->SetDynamicCast(def);
        return true;
    }
    return false;
}
struct CThingDefinitionManager {
    bool GetDef(long index, CDefPointer<const CThingCreatureDef>* out);   // 0x00513160
};
struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);                    // 0x0040F020
    const SEntry* LowerBound(const long* type) const;        // 0x004365B0
};
struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];                          // +0x20
    CInterfaceMap m_interfaces;                              // +0x44
    unsigned char _pad4c[0x90 - 0x4c];
    signed char   m_region;                                  // +0x90
    unsigned char _pad91;
    unsigned char m_flags92;                                 // +0x92
    const C3DVector& GetPosition() const;                    // 0x004C73D0
    CThingDefinitionManager* GetDefinitionManager();         // 0x004C7990
    void SetAsGameStatePersisted(bool persisted);            // 0x004C7880
    __forceinline unsigned long HasTC(long interfaceType) const
    {
        return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31));
    }
    template<class T> __forceinline T* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
    }
    template<class T> __forceinline T* GetTCConst(long interfaceType) const
    {
        long type = interfaceType;
        const CInterfaceMap* map = &m_interfaces;
        const CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
    }
};
bool __fastcall IS_THING_ALIVE(CThing* const& thing);        // 0x0043DBD0
struct CThingCreatureBase : CThing {
    bool SetCurrentAction(const struct CCreatureActionBase& action);   // 0x006644F0
};
struct CThingAICreature : CThingCreatureBase { };
struct CThingPhysical;
struct CThingBuilding;
struct CThingAICreatureInit {                                // Ego_r.pdb, 28 bytes
    CThingPhysical* OriginalThing;
    CThingPhysical* PreviousThing;
    CCharString ScriptName;
    CCharString OverridingBrainName;
    bool PlayerCharacter;
    bool Drawable;
    bool PlaceAtEndOfList;
    bool EnableCreatureAutoPlacing;
    CThingBuilding* HomeBuilding;
    CThingBuilding* WorkBuilding;
    CThingAICreatureInit(CThingPhysical* original, CThingPhysical* previous, bool playerCharacter,
                         const CCharString& scriptName, const CCharString& overridingBrainName, bool drawable);   // 0x004FACD0
    ~CThingAICreatureInit();                                 // 0x004FAD50
};
CThingAICreature* __fastcall CreateAICreature(long defIndex, const C3DVector& pos, long region, const CThingAICreatureInit& init);   // 0x00833800
struct CCreatureActionBase {
    virtual void v00();
    unsigned char _body[0x110];
    ~CCreatureActionBase();                                  // 0x0062E900
};
struct CCreatureAction_Ambush : CCreatureActionBase {
    unsigned char _ambush[0x0c];
    CCreatureAction_Ambush(CThingCreatureBase* creature, CThingCreatureBase* target, const C3DVector& home);   // 0x008BFDA0
};
struct CCreatureActionAppear : CCreatureActionBase {
    CCreatureActionAppear(CThingAICreature* creature);       // 0x0074DBB0
};
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;
    ~CBaseIntelligentPointer();                              // 0x00A01C10
    CThing* GetPItem() const;                                // 0x00A01B50
};
struct CIntelligentPointerThing : CBaseIntelligentPointer {
    CIntelligentPointerThing(CThing* item);                  // 0x004A96F0
};
struct CThingList {                                          // list<CIntelligentPointer<CThing>> (retail: the node pointer)
    void* _M_node;
    void push_back(const CIntelligentPointerThing& value);   // 0x004BA4E0
};
struct CPlayer {
    unsigned char _pad00[0x34];
    CBaseIntelligentPointer PlayerCharacter;                 // +0x34
};
struct CWorld {
    unsigned long* GetPRandomSeed();                         // 0x0049D810
    CPlayer* GetCurrentPlayer() const;                       // 0x0049D9D0
    bool IsCreatureGenerationEnabledForGroup(unsigned long group) const;   // 0x0049D910
};
struct CTCCreatureNavigation {
    bool IsAbleToNavigateFromPosToPos(const C3DVector& from, const C3DVector& to, float tolerance);   // 0x006CB620
};
struct CTCCreatureHive {
    void Open();                                             // 0x0077D230
};
struct CPositionVector {
    C3DVector* _M_start;
    C3DVector* _M_finish;
    C3DVector* _M_end_of_storage;
    bool empty() const { return _M_start == _M_finish; }
    unsigned long size() const { return _M_finish - _M_start; }
    C3DVector& operator[](unsigned long n) { return _M_start[n]; }
};
struct CRetailVectorLong {
    long* _M_start;
    long* _M_finish;
    long* _M_end_of_storage;
    long& at(unsigned long n);                               // 0x0074D120
    bool empty() const { return _M_start == _M_finish; }
    unsigned long size() const { return _M_finish - _M_start; }
};
struct CTCCreatureGenerator {
    virtual void v00();
    CThing*            ParentThing;                          // +0x04
    unsigned char      _pad08[0x0c - 0x08];
    CRetailVectorLong  CreatureFamilies;                     // +0x0c
    CThingList         CreatedCreatures;                     // +0x18
    unsigned char      _pad1c[0x48 - 0x1c];
    CCharString        ScriptName;                           // +0x48
    unsigned char      _pad4c[0x60 - 0x4c];
    CPositionVector    OpenPositions;                        // +0x60
    CPositionVector    HiddenPositions;                      // +0x6c
    unsigned char      _pad78[0x84 - 0x78];
    long               CreationCount;                        // +0x84
    unsigned char      _pad88[0xa1 - 0x88];
    bool               UseHeroPosition;                      // +0xa1
    const CWorld* GetWorldConst() const;                     // 0x00686A80
    CWorld* GetWorld();                                      // 0x00686A70
    CDefinitionManager* GetDefinitionManager() const;        // 0x006869D0
    bool HasHitCreationLimit();                              // 0x0074A190
    void CreateCreaturesUndead(CThingCreatureBase* triggerer);   // 0x0074AF50
    void CreateCreatures(CThingCreatureBase* triggerer);
};

void CTCCreatureGenerator::CreateCreatures(CThingCreatureBase* triggerer)
{
    if (ParentThing->HasTC(0xca))
        return;
    if (ParentThing->HasTC(0x62))
        ParentThing->GetTC<CTCCreatureHive>(0x62)->Open();
    if (CreatureFamilies.empty())
        return;
    if (HasHitCreationLimit())
        return;
    unsigned long familyCount = CreatureFamilies.size();
    long familyIndex = GFRandom(familyCount, ((CWorld*)GetWorldConst())->GetPRandomSeed());
    CDefPointer<const CCreatureGenerationFamilyDef> family(NULL_DEF);
    GetDefinitionManager()->GetDef(CreatureFamilies.at(familyIndex), &family);
    const CCreatureGenerationFamilyDef* familyDef = family.Get();
    unsigned long typeCount = familyDef->GenerationTypes.size();
    long typeIndex = GFRandom(typeCount, GetWorld()->GetPRandomSeed());
    bool hidden = false;
    bool appear = false;
    CPositionVector* positions;
    switch (familyDef->GenerationTypes[typeIndex]) {
    case 0:
        positions = &OpenPositions;
        break;
    case 1:
        positions = &OpenPositions;
        appear = true;
        break;
    case 2:
        positions = &HiddenPositions;
        hidden = true;
        break;
    case 4:
        CreateCreaturesUndead(triggerer);
        return;
    default:
        return;
    }
    if (positions->empty())
        return;
    CTCCreatureNavigation* navigation = 0;
    C3DVector centre = ParentThing->GetPosition();
    if (UseHeroPosition && !hidden) {
        CThing* hero = GetWorld()->GetCurrentPlayer()->PlayerCharacter.GetPItem();
        if (!IS_THING_ALIVE(hero))
            return;
        if (hero->HasTC(7))
            navigation = hero->GetTCConst<CTCCreatureNavigation>(7);
        centre = hero->GetPosition();
        if (navigation == 0)
            return;
    }
    for (long* it = family->Creatures._M_start; it != family->Creatures._M_finish; ++it) {
        CDefPointer<const CThingCreatureDef> creatureDef(NULL_DEF);
        if (!ParentThing->GetDefinitionManager()->GetDef(*it, &creatureDef))
            continue;
        CDefPointer<const CCreatureDef> creature(NULL_DEF);
        if (!creatureDef->GetSubDef(&creature) || !GetWorldConst()->IsCreatureGenerationEnabledForGroup(creature->GenerationGroup))
            continue;
        unsigned long positionCount = positions->size();
        C3DVector* pos = &(*positions)[GFRandom(positionCount, GetWorld()->GetPRandomSeed())];
        if (navigation && !navigation->IsAbleToNavigateFromPosToPos(centre, *pos, 0.0f))
            continue;
        CThingAICreatureInit init(0, 0, false, ScriptName, "", true);
        CThingAICreature* created = CreateAICreature(*it, *pos, ParentThing->m_region, init);
        created->SetAsGameStatePersisted(false);
        created->m_flags92 &= ~0x10;
        if (hidden) {
            if (triggerer)
                created->SetCurrentAction(CCreatureAction_Ambush(created, triggerer, ParentThing->GetPosition()));
        } else if (appear) {
            created->SetCurrentAction(CCreatureActionAppear(created));
        }
        CreatedCreatures.push_back(CIntelligentPointerThing(created));
        ++g_GeneratedCreatureCount;
        ++CreationCount;
        if (HasHitCreationLimit())
            break;
    }
}
