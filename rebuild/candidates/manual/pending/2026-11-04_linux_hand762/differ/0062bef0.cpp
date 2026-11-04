// CWorld::EAUseProjectileWeapon(const CGameEvent*) -- game-event action: if the event player's hero can act and holds a live projectile weapon in its primary slot, step the hero's projectile-weapon mode, enter or leave player mode 0x19 to match, and play the PROJECTILE_WEAPON_ZOOM_IN camera sound
// Grade: DIFFER(ours v retail) -- 2 lines (was 12): out-param GetTC(T**) for TCI_HERO_RANGED_ATTACKS (non-const LowerBound) + PeekTC(const T**) const for carrying/0xA7/weapon over the real CVectorMap fixed the key registers; left: the GetCameraManager out-local takes the this-spill slot (frame+4) where retail reuses the ranged slot (frame+0); scopes/decl order/accessor tried (best try/0062bef0_f.cpp)
// Leaf: ?EAUseProjectileWeapon@CWorld@
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x0062BEF0, 481 bytes (+ int3 padding), `ret 4`, no /GS.
//   PDB ?EAUseProjectileWeapon@CWorld@@QAEXPBVCGameEvent@@@Z.
// Callees: CWorld::GetPlayer(long) 0x0049D850 (x2), CPlayer::HasMode 0x00634160, CPlayer::GetHeroCreature 0x00487DC0,
//   CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound 0x0040F020 (interface 0x81 = CTCHeroRangedAttacks) and its
//   second copy 0x004365B0 (0x46 CTCCarrying, 0xA7, 0x48 CTCWeapon), CPlayer::IsFirstPersonTargeting 0x004874D0,
//   CThingCreatureBase::IsPerformingAnyAction 0x006622A0, CTCCarrying::GetThingInPrimarySlot 0x006A1440,
//   IS_THING_ALIVE<CThing*> 0x0040DCE0 (`__fastcall`, by reference), CTCWeapon::IsProjectileWeapon 0x005DAA40,
//   CTCHeroRangedAttacks::IncrementProjectileWeaponMode 0x00789C90 and the mode query 0x00789AB0 (manifest
//   "C3DMeshInfo::HasPhysicsMesh", propagated), CPlayer::AddMode 0x006345C0 / RemoveMode 0x00634190, the world camera
//   source's GetCameraManager (slot 9, +0x24), CGameCameraManager::PlayCameraSound 0x00698B20, CCharString
//   0x0099EBF0 / ~CCharString 0x0099EAE0 ("PROJECTILE_WEAPON_ZOOM_IN" 0x01259B94).
// Layout: local models at retail offsets, as the landed EAUseQuickAccessItemInCutscene 0x0062AD90 (engine/ headers
//   carry the PDB's 16-byte containers); CWorld m_pCameraSource +0x34 as the landed 0x0075C5F0.
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
struct CMemoryBuffer {
    unsigned char Data[0x20];                      // +0x00
    long          Length;                          // +0x20
    long          ReadPos;                         // +0x24
    long          WritePos;                        // +0x28
    CMemoryBuffer() : Length(0), ReadPos(0), WritePos(0) { }
    void Read(unsigned long length, void* data);   // 0x009F18F0
    __forceinline long ReadLong() { long value; Read(4, &value); return value; }
};
struct CGameEvent {
    long          Type;                            // +0x00
    signed char   Player;                          // +0x04
    unsigned char Data[32];                        // +0x05
    unsigned char EndPos;                          // +0x25
    bool          Valid;                           // +0x26
    bool          Replacement;                     // +0x27
    long GetData(CMemoryBuffer& buffer) const;     // 0x009F17A0
};

#define FABLE_RETAIL_CVECTORMAP_FORCEINLINE_FIND
#include "engine/ETCInterfaceType.h"
#include "engine/CVectorMap.h"
class CTCBase { public: virtual ~CTCBase(); };
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CTCMap;
struct CCreatureActionBase {
    virtual void Slot00(); virtual void Slot01(); virtual void Slot02(); virtual void Slot03(); virtual void Slot04();
    virtual void Slot05(); virtual void Slot06(); virtual void Slot07(); virtual void Slot08(); virtual void Slot09();
    virtual void Slot10(); virtual void Slot11(); virtual void Slot12(); virtual void Slot13(); virtual void Slot14();
    virtual void Slot15(); virtual void Slot16(); virtual void Slot17(); virtual void Slot18(); virtual void Slot19();
    virtual void Slot20(); virtual void Slot21(); virtual void Slot22(); virtual void Slot23(); virtual void Slot24();
    virtual void Slot25(); virtual void Slot26(); virtual void Slot27(); virtual void Slot28(); virtual void Slot29();
    virtual void Slot30(); virtual void Slot31(); virtual void Slot32(); virtual void Slot33(); virtual void Slot34();
    virtual void Slot35(); virtual void Slot36(); virtual void Slot37(); virtual void Slot38();
    virtual bool BlocksQuickAccess();              // slot 39 (+0x9C)
};

struct CTCInterfaceA7 : CTCBase {                  // interface 0xA7: a flag at +0xC vetoes item use
    static ETCInterfaceType __fastcall StatGetInterfaceType() { return TCI_SPECIAL_ABILITIES; }
    unsigned char _pad[0xc - 4];
    bool Blocking;                                 // +0xC
};


struct CBaseObject {
    void* m_pVtable;
};
struct CThingCreatureBase : CBaseObject {
    unsigned char _pad04[0x20 - 4];
    std::bitset<274> UsingComponents;              // +0x20
    CTCMap Components;                             // +0x44
    unsigned char _pad54[0x91 - 0x54];
    unsigned char Dead : 1;                        // +0x91 bit 0
    bool HasTC(ETCInterfaceType type) const { return UsingComponents.test(type); }
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
    unsigned char _pad92[0x100 - 0x92];
    CCreatureActionBase* PCurrentAction;           // +0x100 CCountedPointer<CCreatureActionBase>::Data
    bool IsPerformingAnyAction();                  // 0x006622A0
};

struct CTCCarrying : CTCBase {
    static ETCInterfaceType __fastcall StatGetInterfaceType() { return TCI_CARRYING; }
    CThingCreatureBase* GetThingInPrimarySlot() const;   // 0x006A1440
};
struct CTCWeapon : CTCBase {
    static ETCInterfaceType __fastcall StatGetInterfaceType() { return TCI_WEAPON; }
    bool IsProjectileWeapon() const;               // 0x005DAA40
};
struct CTCHeroRangedAttacks : CTCBase {
    static ETCInterfaceType __fastcall StatGetInterfaceType() { return TCI_HERO_RANGED_ATTACKS; }
    void IncrementProjectileWeaponMode();          // 0x00789C90
    bool IsToUseFirstPersonTargetingMode() const;  // 0x00789AB0
};
bool __fastcall IS_THING_ALIVE(CBaseObject* const& thing);   // 0x0040DCE0

enum EPlayerMode { PLAYER_MODE_0x19 = 0x19, PLAYER_MODE_0x2F = 0x2f };
struct CPlayer {
    bool HasMode(EPlayerMode mode);                // 0x00634160
    CThingCreatureBase* GetHeroCreature();         // 0x00487DC0
    bool IsFirstPersonTargeting();                 // 0x004874D0
    void AddMode(EPlayerMode mode, bool b);        // 0x006345C0
    void RemoveMode(EPlayerMode mode);             // 0x00634190
};
struct CGameCameraManager {
    void PlayCameraSound(const CCharString& sound);   // 0x00698B20
};
struct CCameraManagerSource {
    virtual void Slot00(); virtual void Slot01(); virtual void Slot02(); virtual void Slot03(); virtual void Slot04();
    virtual void Slot05(); virtual void Slot06(); virtual void Slot07(); virtual void Slot08();
    virtual bool GetCameraManager(CGameCameraManager** camera);   // slot 9 (+0x24)
};
struct CWorld {
    unsigned char _pad00[0x34];
    CCameraManagerSource* m_pCameraSource;         // +0x34
    CPlayer* GetPlayer(long number);               // 0x0049D850
    void EAUseProjectileWeapon(const CGameEvent* event);
};

void CWorld::EAUseProjectileWeapon(const CGameEvent* event)
{
    CPlayer* eventPlayer = GetPlayer(event->Player);
    if (eventPlayer->HasMode(PLAYER_MODE_0x2F))
        return;
    CPlayer* player = GetPlayer(event->Player);
    if (!player)
        return;
    CThingCreatureBase* hero = player->GetHeroCreature();
    if (!hero || hero->Dead)
        return;
    CTCHeroRangedAttacks* ranged;
    if (!hero->GetTC(&ranged))
        return;
    const CTCCarrying* carrying;
    if (!hero->PeekTC(&carrying))
        return;
    if (!player->IsFirstPersonTargeting() && hero->IsPerformingAnyAction())
        return;
    const CTCInterfaceA7* a7;
    if (hero->PeekTC(&a7) && a7->Blocking)
        return;
    CThingCreatureBase* thing = carrying->GetThingInPrimarySlot();
    if (!IS_THING_ALIVE(thing))
        return;
    const CTCWeapon* weapon;
    if (!thing->PeekTC(&weapon) || !weapon->IsProjectileWeapon())
        return;
    ranged->IncrementProjectileWeaponMode();
    if (ranged->IsToUseFirstPersonTargetingMode())
        player->AddMode(PLAYER_MODE_0x19, false);
    else
        player->RemoveMode(PLAYER_MODE_0x19);
    {
        CGameCameraManager* camera;
        m_pCameraSource->GetCameraManager(&camera);
        camera->PlayCameraSound("PROJECTILE_WEAPON_ZOOM_IN");
    }
}
