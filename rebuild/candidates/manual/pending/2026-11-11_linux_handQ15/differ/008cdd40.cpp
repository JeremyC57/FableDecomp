// CCreatureAction_ReleaseTimeSpell::FinishAction() -- base FinishAction; release the camera's scripted control; re-equip
// Grade: DIFFER(ours v retail) -- regraded as-is in Q15 (FABLE_HDR_OVERLAY=$A/hdr): DIFFER(430v430), the frame-slot (0xD0 v 0xD4) / ebx-edi residue of the earlier note; NOT PORTED this pass -- the draft is still a local model, so it is not landable even if it matched; port onto the shared engine headers first (see NOTES.md)
// Earlier grade note: DIFFER(ours v retail) -- 21-24 lines: frame layout (retail 0xd4: key temp frame+0, camera frame+4) and this ebx v edi; shared CBaseObject/CIntelligentPointer + PeekTC/GetTC out-param, camera block, named multiplier (21), decl order, early return tried; class form, unscoped camera (24) (best try/008cdd40_e.cpp)
// Leaf: ?FinishAction@CCreatureAction_ReleaseTimeSpell@
// the item taken out (fast unsheathe carrying the haste key) when it lives, else register the creature's air resistance as
// a haste multiplier under HasteKey (adding a CTCHaste when missing) and recompute its speed
#define FABLE_RETAIL_CVECTORMAP_FORCEINLINE_FIND
#include "engine/ETCInterfaceType.h"
#include "engine/CVectorMap.h"
#define FABLE_RETAIL_BASE_OBJECT
#include "engine/CBaseObject.h"
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"
class CTCBase { public: virtual ~CTCBase(); };
struct CWorld;
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CTCMap;
struct CPlayer {
    unsigned char _pad[0x20a];
    unsigned char m_autoFaceTarget;                   // +0x20a
};
class CThingCreatureBase;
struct CCreatureActionBase;
struct CCharString;
struct CTCParamsBase;
struct CThing : CBaseObject {
    unsigned char _pad08[0x20 - 0x08];
    std::bitset<274> UsingComponents;                 // +0x20
    CTCMap Components;                                // +0x44
    unsigned char _pad54[0x91 - 0x54];
    unsigned char m_flags91;                          // +0x91 bit 0: dead
    CPlayer* GetPlayer();                             // 0x004C7A10
    bool SetCurrentAction(CCreatureActionBase* action);   // 0x006644F0 (CThingCreatureBase)
    CTCBase* AddTC(const CCharString& name, bool activate, CTCParamsBase* params);   // 0x004C9D60
    bool HasTC(ETCInterfaceType type) const { return UsingComponents.test(type); }
    template<class T> __forceinline bool PeekTC(const T** out) const
    {
        if (!HasTC(T::StatGetInterfaceType()))
            return false;
        ETCInterfaceType key = T::StatGetInterfaceType();
        const CTCMap* map = &Components;
        const std::pair<ETCInterfaceType, CTCBase*>* it = map->LowerBound(key);
        if (it == map->end() || T::StatGetInterfaceType() < it->first)
            it = map->end();
        *out = static_cast<const T*>(it->second);
        return true;
    }
    template<class T> __forceinline bool GetTC(T** out)
    {
        if (!HasTC(T::StatGetInterfaceType()))
            return false;
        ETCInterfaceType key = T::StatGetInterfaceType();
        CTCMap* map = &Components;
        std::pair<ETCInterfaceType, CTCBase*>* it = map->LowerBound(key);
        if (it == map->end() || T::StatGetInterfaceType() < it->first)
            it = map->end();
        *out = static_cast<T*>(it->second);
        return true;
    }
};
template<class T> inline bool __fastcall IS_THING_ALIVE(const T& p)
{
    if (p.Get() != 0) {
        if (!(p.Get()->m_flags91 & 1))
            return true;
    }
    return false;
}
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);  // 0x0099EBF0
    ~CCharString();                                   // 0x0099EAE0
};
struct CCreatureActionBase {                          // retail sizeof 0xa8
    virtual void v00();
    unsigned char _pad04[4];
    CIntelligentPointer<CThing> PCreature;            // +0x08
    unsigned char _pad10[0x2c - 0x10];
    CIntelligentPointer<CThing> PItemRemoved;         // +0x2c
    unsigned char _pad34[0xa8 - 0x34];
    CWorld* GetPWorld();                              // 0x00692940
    CThing* GetCreature() const { return PCreature.Get(); }
};
struct CCreatureAction_UnsheatheItemFromInventory : CCreatureActionBase {
    unsigned char _body[0x10];
    ~CCreatureAction_UnsheatheItemFromInventory();    // 0x005CC091
};
struct CCreatureAction_UnsheatheItemFromInventoryWithHasteAction : CCreatureAction_UnsheatheItemFromInventory {
    CCreatureActionBase* PHasteAction;                // +0xb8
    CCreatureAction_UnsheatheItemFromInventoryWithHasteAction(CCreatureActionBase* hasteAction, CThing* owner,
        const CCharString& animation, CThing* item, long priority, long timeout, CCreatureActionBase* next);   // 0x0069F1F0
};
struct CCombatActionBase : CCreatureActionBase {      // retail sizeof 0x114
    unsigned char _pad0a8[0x114 - 0xa8];
    void FinishAction();                              // 0x00859A20
};
struct CTCPhysicsBase : CTCBase {                     // interface 0x92
    static ETCInterfaceType __fastcall StatGetInterfaceType() { return (ETCInterfaceType)0x92; }
    float GetAirResistance() const;                         // 0x0079C740
};
struct CHasteMultiplier {                             // std::pair<void* const, float>
    void* first;
    float second;
    CHasteMultiplier(void* key, float value) : first(key), second(value) {}
};
struct CHasteMap {                                    // map<void*, float> (Dinkumware tree)
    struct iterator { void* node; };
    struct pairib { iterator first; bool second; };
    pairib insert(const CHasteMultiplier& value);     // 0x00580D21
};
struct CTCHaste : CTCBase {                           // interface 0x8C
    static ETCInterfaceType __fastcall StatGetInterfaceType() { return (ETCInterfaceType)0x8c; }
    unsigned char _pad04[0x18 - 4];
    CHasteMap Multipliers;                            // +0x18
    void RecalculateSpeed();                          // 0x00800020
};
struct CGameCameraManager {
    void ReleaseScriptedControl();                    // 0x00698950
};
struct CCameraHolder {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08();
    virtual bool GetCameraManager(CGameCameraManager** out);   // slot 9 (+0x24)
};
struct CWorld {
    unsigned char _pad[0x34];
    CCameraHolder* PCurrentGameCamera;                // +0x34
    CCameraHolder* GetCurrentGameCamera() { return PCurrentGameCamera; }
};
struct CCreatureAction_ReleaseTimeSpell : CCombatActionBase {
    void* HasteKey;                                   // +0x114 (PDB +0x12C)
    void FinishAction();
};

void CCreatureAction_ReleaseTimeSpell::FinishAction()
{
    CCombatActionBase::FinishAction();
    {
        CGameCameraManager* camera;
        if (GetPWorld()->GetCurrentGameCamera()->GetCameraManager(&camera))
            camera->ReleaseScriptedControl();
    }
    if (IS_THING_ALIVE(PItemRemoved)) {
        CCharString name("ST_ABILITY_FAST_UNSHEATHE_TO_COMBAT_POSE");
        GetCreature()->SetCurrentAction(&CCreatureAction_UnsheatheItemFromInventoryWithHasteAction((CCreatureActionBase*)HasteKey, GetCreature(), name, PItemRemoved.Get(), 100, 250, 0));
    } else {
        const CTCPhysicsBase* physics;
        const CThing* creature = PCreature.Get();
        if (creature->PeekTC(&physics)) {
            CTCHaste* haste;
            if (!PCreature.Get()->GetTC(&haste))
                haste = (CTCHaste*)PCreature.Get()->AddTC("CTCHaste", true, 0);
            CHasteMultiplier multiplier(HasteKey, physics->GetAirResistance());
            haste->Multipliers.insert(multiplier);
            haste->RecalculateSpeed();
        }
    }
}
