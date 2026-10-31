// CCreatureAction_ReleaseTimeSpell::FinishAction() -- base FinishAction; release the game camera manager's scripted control; if the removed item still lives, re-equip it with a fast unsheathe action carrying the haste key, else register the creature's air resistance as its action-speed multiplier under HasteKey (adding a CTCHaste when missing)
// Grade: DIFFER(430v430) -- frame 0xd0 v 0xd4: retail gives the camera-manager pointer its own slot (+0x14) and the haste pair / insert result their own slots (+0x18 / +0x20), ours packs the pair into the name/key slot (+0x10) and the camera at +0x18
// Leaf: ?FinishAction@CCreatureAction_ReleaseTimeSpell@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008CDD40, 430 bytes, plain `ret` (virtual).
// Chunk 615: respelled with the PDB names (GetWorld, CWorld::GetGameCamera, CAIGameCameraBase::GetCameraManager slot 9,
//   GetTC<T> and the const GetTC through GetPCreature(), CTCHaste::SetActionSpeedMultiplier inline over
//   UpdateActionSpeedMultiplier, SetCurrentAction(const CCreatureActionBase&), AddTC(..., const CTCParamsBase*)): same 32
//   masked lines as the parked file. Tried: camera pointer in a block (frame 0xcc), named action local (428), name as a
//   temporary (422).
// Callees: CCombatActionBase::FinishAction 0x00859A20, CCreatureActionBase::GetWorld 0x00692940 (protected, CWorld&; donor
//   order PeekPlayer/GetPlayer/GetWorld/PeekWorld = retail 0x00692920..0x00692950), CGameCameraManager::ReleaseScriptedControl
//   0x00698950, GetPItem 0x00A01B50, LowerBound 0x0040F020 / const 0x004365B0, CCharString 0x0099EBF0 / 0x0099EAE0,
//   CCreatureAction_UnsheatheItemFromInventoryWithHasteAction ctor 0x0069F1F0 / ~CCreatureAction_UnsheatheItemFromInventory
//   0x005CC091, CThingCreatureBase::SetCurrentAction 0x006644F0, CThing::AddTC 0x004C9D60, CTCPhysicsBase::GetAirResistance
//   0x0079C740, map<void*, float>::insert 0x00580D21, CTCHaste::UpdateActionSpeedMultiplier 0x00800020 (private).
// Layout: CCreatureActionBase PCreature +0x08, PItemRemoved +0x2c (retail 0xa8); CCombatActionBase 0x114; HasteKey +0x114
//   (Ego_r +0x12c); CThing UsingComponents +0x20, Components +0x44, Dead +0x91; CWorld PCurrentGameCamera +0x34; CTCHaste
//   Multipliers +0x18 (engine headers are padding-only, so the members used are modelled locally).
#define UNPROVEN_SLOT(n) virtual void UnprovenSlot##n();
class CTCBase;
enum ETCInterfaceType { TCI_HASTE = 0x8C, TCI_PHYSICS = 0x92 };   // descriptive enumerator names
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; pair(const T1& a, const T2& b) : first(a), second(b) {} }; }
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > {
public:
    struct SEntry { ETCInterfaceType first; CTCBase* second; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const ETCInterfaceType& type);             // 0x0040F020
    const SEntry* LowerBound(const ETCInterfaceType& type) const; // 0x004365B0
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);  // 0x0099EBF0
    ~CCharString();                                   // 0x0099EAE0
};
class CTCParamsBase;
class CThing {
public:
    unsigned char _pad0[0x20];
    unsigned long UsingComponents[9];                 // +0x20
    CInterfaceMap Components;                         // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char Dead : 1;                           // +0x91
    bool IsDead() const { return Dead; }
    CTCBase* AddTC(const CCharString& name, bool activate, const CTCParamsBase* params);   // 0x004C9D60
    template <class T> __forceinline bool GetTC(T** out)
    {
        long type = T::StatGetInterfaceType();
        if (UsingComponents[type >> 5] & (1UL << (type & 31))) {
            ETCInterfaceType key = (ETCInterfaceType)type;
            CInterfaceMap* map = &Components;
            CInterfaceMap::SEntry* it = map->LowerBound(key);
            *out = (T*)((it == map->last || type < it->first) ? map->last : it)->second;
            return true;
        }
        return false;
    }
    template <class T> __forceinline bool GetTC(const T** out) const
    {
        long type = T::StatGetInterfaceType();
        if (UsingComponents[type >> 5] & (1UL << (type & 31))) {
            ETCInterfaceType key = (ETCInterfaceType)type;
            const CInterfaceMap* map = &Components;
            const CInterfaceMap::SEntry* it = map->LowerBound(key);
            *out = (const T*)((it == map->last || type < it->first) ? map->last : it)->second;
            return true;
        }
        return false;
    }
};
class CCreatureActionBase;
class CThingCreatureBase : public CThing {
public:
    bool SetCurrentAction(const CCreatureActionBase& action);   // 0x006644F0
};
class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer {
public:
    virtual ~CBaseIntelligentPointer();               // 0x00A01C10
    CBaseObject* GetPItem() const;                    // 0x00A01B50
private:
    CBaseObjectPointer* PData;
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
};
inline bool IS_THING_ALIVE(const CIntelligentPointer<CThing>& p)
{
    if (p.GetPItem() != 0) {
        if (!p.GetPItem()->IsDead())
            return true;
    }
    return false;
}
class CGameCameraManager { public:
    void ReleaseScriptedControl();                    // 0x00698950
};
class CAIGameCameraBase { public:
    UNPROVEN_SLOT(0) UNPROVEN_SLOT(1) UNPROVEN_SLOT(2) UNPROVEN_SLOT(3) UNPROVEN_SLOT(4)
    UNPROVEN_SLOT(5) UNPROVEN_SLOT(6) UNPROVEN_SLOT(7) UNPROVEN_SLOT(8)
    virtual bool GetCameraManager(CGameCameraManager** out);   // slot 9 (+0x24)
};
class CWorld { public:
    unsigned char _pad[0x34];
    CAIGameCameraBase* PCurrentGameCamera;            // +0x34
    CAIGameCameraBase* GetGameCamera() { return PCurrentGameCamera; }
};
class CCreatureActionBase {                           // retail sizeof 0xa8
public:
    virtual void v00();
    unsigned char _pad04[4];
    CIntelligentPointer<CThingCreatureBase> PCreature;   // +0x08
    unsigned char _pad10[0x2c - 0x10];
    CIntelligentPointer<CThing> PItemRemoved;         // +0x2c
    unsigned char _pad34[0xa8 - 0x34];
    const CThingCreatureBase* GetPCreature() { return PCreature.GetPItem(); }
protected:
    CWorld& GetWorld();                               // 0x00692940
};
class CCreatureAction_UnsheatheItemFromInventory : public CCreatureActionBase {
public:
    unsigned char _body[0x10];
    ~CCreatureAction_UnsheatheItemFromInventory();    // 0x005CC091
};
class CCreatureAction_UnsheatheItemFromInventoryWithHasteAction : public CCreatureAction_UnsheatheItemFromInventory {
public:
    void* HasteKey;                                   // +0xb8
    CCreatureAction_UnsheatheItemFromInventoryWithHasteAction(void* hasteKey, CThingCreatureBase& owner,
        const CCharString& animation, CThing& item, long priority, long timeout, CCreatureActionBase* next);   // 0x0069F1F0
};
class CCombatActionBase : public CCreatureActionBase {   // retail sizeof 0x114
public:
    unsigned char _pad0a8[0x114 - 0xa8];
    void FinishAction();                              // 0x00859A20
};
class CTCPhysicsBase { public:
    static ETCInterfaceType __fastcall StatGetInterfaceType() { return TCI_PHYSICS; }
    float GetAirResistance() const;                   // 0x0079C740
};
class CHasteMap { public:                             // map<void*, float>
    typedef std::pair<void* const, float> value_type;
    struct iterator { void* node; };
    std::pair<iterator, bool> insert(const value_type& value);   // 0x00580D21
};
class CTCHaste { public:
    static ETCInterfaceType __fastcall StatGetInterfaceType() { return TCI_HASTE; }
    unsigned char _pad0[0x18];
    CHasteMap Multipliers;                            // +0x18
    void SetActionSpeedMultiplier(void* key, float multiplier)
    {
        Multipliers.insert(CHasteMap::value_type(key, multiplier));
        UpdateActionSpeedMultiplier();
    }
private:
    void UpdateActionSpeedMultiplier();               // 0x00800020
};
class CCreatureAction_ReleaseTimeSpell : public CCombatActionBase {
public:
    void* HasteKey;                                   // +0x114
    void FinishAction();
};

void CCreatureAction_ReleaseTimeSpell::FinishAction()
{
    CCombatActionBase::FinishAction();
    CGameCameraManager* camera;
    if (GetWorld().GetGameCamera()->GetCameraManager(&camera))
        camera->ReleaseScriptedControl();
    if (IS_THING_ALIVE(PItemRemoved)) {
        CCharString name("ST_ABILITY_FAST_UNSHEATHE_TO_COMBAT_POSE");
        PCreature.GetPItem()->SetCurrentAction(CCreatureAction_UnsheatheItemFromInventoryWithHasteAction(HasteKey, *PCreature.GetPItem(), name, *PItemRemoved.GetPItem(), 100, 250, 0));
    } else {
        const CTCPhysicsBase* physics;
        if (GetPCreature()->GetTC(&physics)) {
            CTCHaste* haste;
            if (!PCreature.GetPItem()->GetTC(&haste))
                haste = (CTCHaste*)PCreature.GetPItem()->AddTC("CTCHaste", true, 0);
            haste->SetActionSpeedMultiplier(HasteKey, physics->GetAirResistance());
        }
    }
}
