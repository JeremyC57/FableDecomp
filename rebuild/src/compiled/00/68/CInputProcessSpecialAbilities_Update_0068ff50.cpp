// CInputProcessSpecialAbilities::Update(CProcessedInput& input) -- keep the special-ability buttons on screen: in spell mode show them (both flags) and latch the player's trigger lock; when the spell-mode game action is queued show them (first flag) and latch; otherwise, for a live hero whose kind-0xf6 menu component is not active, show them (second flag); then clear the latch
// Leaf: Update
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
// Naming wave 18: the global stand-in g_pPlayerGui (0x013B8790) is the private static CCountedPointer<CPlayerGui>
//   CTBaseSingletonNoDefaultAlloc<CPlayerGui>::PClass, read through the inline static accessor Get() (name-only: the bytes are unchanged).
// Retail: Fable.exe (TLC, VC7.1) @ 0x0068FF50, 210 bytes, `ret 4`.
// NF55 (2026-11-17): the CTCMenu stand-in of the kind-0xf6 component is CTCQuickAccessMenu (engine/CTCQuickAccessMenu.h, PDB
//   ?IsActive@CTCQuickAccessMenu@@QBE_NXZ = 0x00625930); the local std::pair / CVectorMap specialisation gave way to engine/CVectorMap.h.
// Callees: CPlayerGui::SetSpecialAbilityButtonsAsDisplaying(bool, bool) @ 0x00437580 on (&CTBaseSingletonNoDefaultAlloc<CPlayerGui>::Get()) (0x013B8790),
//   CInputProcessGameBase::IsGameActionEventInQueue(long action, long buttonEventType) @ 0x00687D50 (landed name),
//   CPlayer::GetPControlledCreature @ 0x00487DC0, the const component-map LowerBound @ 0x004365B0, the kind-0xf6 menu component's
//   CTCQuickAccessMenu::IsActive() const @ 0x00625930.
// Layout: engine/CInputProcessGameBase.h (MyPlayer +0x18), local model (the class carries virtuals); CPlayer (Ego_r.pdb,
//   the PlayerModes list 4 bytes in retail): RightTriggerTargetLocked +0x20c, SpellMode +0x218; retail CThing component
//   bitset +0x20 (kind 0xf6 = word +0x3c bit 22), map +0x44, Dead bit +0x91.
// Shape: two early-return arms each ending in the latch store; the menu pointer is preset to null and set only when
//   the component is present (`xor ecx,ecx` before the test), then called unconditionally.
// Layout (supersedes any local-model note above): engine/CPlayer.h now has the retail STLport 4.6 offsets, so the data
//   members come from the shared header; the class below keeps only the declarations this body needs.
#include "fable_compiler.h"
#include "engine/CCountedPointer.h"
template <class T> class CTBaseSingletonNoDefaultAlloc {              // the CRTP singleton base of CPlayerGui
    static CCountedPointer<T> PClass;                                // 0x013B8790 ?PClass@?$CTBaseSingletonNoDefaultAlloc@VCPlayerGui@@@@0V?$CCountedPointer@VCPlayerGui@@@@A (private static)
public:
    static T& __fastcall Get() { return *PClass; }                   // Ego_r / FableWin ?Get@?$CTBaseSingletonNoDefaultAlloc@VCPlayerGui@@@@SIAAVCPlayerGui@@XZ (inline)
};
#define CPlayer CPlayerDonor
#include "engine/CPlayer.h"
#undef CPlayer
class CPlayerGui { public:
    void SetSpecialAbilityButtonsAsDisplaying(bool a, bool b);   // 0x00437580
};
class CProcessedInput;
class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCQUICKACCESSMENU_ISACTIVE
#include "engine/CVectorMap.h"
#include "engine/CTCQuickAccessMenu.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*> CTCMap_Retail;      // engine/CVectorMap.h: the thing's component map at +0x44
class CThing { public:
    unsigned char  _pad00[0x20];
    unsigned long  m_tcMask[8];                    // +0x20: which component kinds the thing holds
    unsigned char  _pad40[4];
    CTCMap_Retail  m_tcs;                          // +0x44
    unsigned char  _pad54[0x91 - 0x54];                 // the map is the 16-byte CVectorMap (vector + Compare + Dirty)
    unsigned char  Dead : 1;                       // +0x91 bit 0
    unsigned char  _bits91 : 7;
    FABLE_FORCEINLINE unsigned long HasTC(long kind) const { return m_tcMask[kind >> 5] & (1UL << (kind & 31)); }
    FABLE_FORCEINLINE CTCBase* FindTC(long kind) const
    {
        const CTCMap_Retail* map = &m_tcs;
        const CTCMap_Retail::value_type* it = map->LowerBound((ETCInterfaceType)kind);
        if (it == map->end() || kind < it->first)
            it = map->end();
        return it->second;
    }
};
class CThingPlayerCreature : public CThing { };
class CPlayer : public CPlayerDonor {
public:
    CThingPlayerCreature* GetPControlledCreature() const;                        // 0x00487DC0
};
class CWorld; class CGameDefinitionManager; class CMainGameComponent; class CGamePlayerInterface;
class CInputProcessGameBase {                      // engine/CInputProcessGameBase.h layout
public:
    virtual void v00();
    unsigned char           _pad04[0x10];          // +0x04 CAInputProcess
    CWorld*                 World;                 // +0x14
    CPlayer*                MyPlayer;              // +0x18
    CGameDefinitionManager* DefinitionManager;     // +0x1c
    CMainGameComponent*     Component;             // +0x20
    CGamePlayerInterface*   PGamePlayerInterface;  // +0x24
    bool IsGameActionEventInQueue(long action, long buttonEventType);   // 0x00687D50
};
class CInputProcessSpecialAbilities : public CInputProcessGameBase {
public:
    virtual void Update(CProcessedInput& input);
};

void CInputProcessSpecialAbilities::Update(CProcessedInput& input)
{
    if (MyPlayer->SpellMode) {
        CTBaseSingletonNoDefaultAlloc<CPlayerGui>::Get().SetSpecialAbilityButtonsAsDisplaying(true, true);
        MyPlayer->RightTriggerTargetLocked = true;
        return;
    }
    if (IsGameActionEventInQueue(0x49, 2)) {
        CTBaseSingletonNoDefaultAlloc<CPlayerGui>::Get().SetSpecialAbilityButtonsAsDisplaying(true, false);
        MyPlayer->RightTriggerTargetLocked = true;
        return;
    }
    CThing* hero = MyPlayer->GetPControlledCreature();
    if (hero && !hero->Dead) {
        CTCQuickAccessMenu* menu = 0;
        if (hero->HasTC(0xf6))
            menu = (CTCQuickAccessMenu*)hero->FindTC(0xf6);
        if (!menu->IsActive())
            CTBaseSingletonNoDefaultAlloc<CPlayerGui>::Get().SetSpecialAbilityButtonsAsDisplaying(false, true);
    }
    MyPlayer->RightTriggerTargetLocked = false;
}