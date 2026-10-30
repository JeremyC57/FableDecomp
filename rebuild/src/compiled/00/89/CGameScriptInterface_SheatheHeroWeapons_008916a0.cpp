class CGSISheatheHeroWeapons_Target;
class CCreatureActionBase;
class CThingPlayerCreature;

class CPlayer
{
public:
    CThingPlayerCreature* GetPControlledCreature() const;   // 0x00487DC0
};

class CPlayerManager
{
public:
    CPlayer& GetMainPlayer();   // 0x00449970 (Ego_r 0x004450F0)
};

class CGSISheatheHeroWeapons_Target
{
public:
    bool IsThingCarryingWeapon();
    void PrepareSheathe();
    void UpdateSheathe();
    void FinaliseSheathe();
    void SetCurrentAction(CCreatureActionBase* action);

    unsigned char m_Unused[0x91];
    unsigned char m_Flags;
};
class CThingPlayerCreature : public CGSISheatheHeroWeapons_Target { };

class CGSISheatheHeroWeapons_ActionStorage
{
public:
    CCreatureActionBase* Construct(
        CGSISheatheHeroWeapons_Target* target,
        CCreatureActionBase* previousAction,
        int priority);
    void Destroy();

private:
    unsigned char m_Bytes[0xB0];
};

struct CGSISheatheHeroWeapons_Interface
{
    unsigned char m_Unused[0x14];
    CPlayerManager* m_LookupSource;
};

// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

void CGameScriptInterface::SheatheHeroWeapons() const
{
    CGSISheatheHeroWeapons_Target* const target =
        reinterpret_cast<
            const CGSISheatheHeroWeapons_Interface*>(
                this)->m_LookupSource
            ->GetMainPlayer().GetPControlledCreature();

    if (target != 0 &&
        (target->m_Flags & 1) == 0 &&
        target->IsThingCarryingWeapon())
    {
        target->PrepareSheathe();
        target->UpdateSheathe();
        target->UpdateSheathe();
        target->FinaliseSheathe();

        CGSISheatheHeroWeapons_ActionStorage actionStorage;
        CCreatureActionBase* const action =
            actionStorage.Construct(
                target,
                0,
                100);
        target->SetCurrentAction(action);
        actionStorage.Destroy();
    }
}
