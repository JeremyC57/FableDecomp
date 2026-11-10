// Naming wave 18: the global stand-in gPlayerGui (0x013B8790) is the private static CCountedPointer<CPlayerGui>
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
// Naming wave 18: the shared const float 1e-4f at 0x0129BA3C is ?DELTA_NOTIONAL_ZERO@@3MB (raw Ego_r public + typed data record; was EPSILON).
//   CTBaseSingletonNoDefaultAlloc<CPlayerGui>::PClass, read through the inline static accessor Get() (name-only: the bytes are unchanged).
// Name fix (agentNF6 2026-11-10): CThingCreatureBase::SetCurrentAction 0x006644F0 is ?SetCurrentAction@CThingCreatureBase@@QAE_NABVCCreatureActionBase@@@Z (Ego_r.pdb / FableWin.pdb): a CThingCreatureBase member, bool, the action by const reference.
class CCreatureActionBase;
class CThingCreatureBase;

class CBaseIntelligentPointer
{
public:
    CThingCreatureBase* GetPItem() const;
};

struct HeroHandle
{
    unsigned char unused00[0x34];
    CBaseIntelligentPointer item;
};

class CPlayer;
// CPlayerManager: the shared model engine/CPlayerManager.h (Ego_r.pdb CPlayerManager; the five accessors declared
//   there with their retail addresses: GetPlayer 0x004498C0 / 0x00449910, GetMainPlayer 0x00449970 / 0x00449960,
//   GetMainPlayerNumber 0x00449700).
#include "engine/CPlayerManager.h"
 /* 0x00449970 CPlayerManager::GetMainPlayer (Ego_r 0x004450F0) */
class HeroResolver
{
public:

};

class CCreatureActionBase
{
public:
    virtual ~CCreatureActionBase();
};

class CThingCreatureBase
{
public:
    virtual void Slot000(); virtual void Slot004(); virtual void Slot008(); virtual void Slot00C();
    virtual void Slot010(); virtual void Slot014(); virtual void Slot018(); virtual void Slot01C();
    virtual void Slot020(); virtual void Slot024(); virtual void Slot028(); virtual void Slot02C();
    virtual void Slot030(); virtual void Slot034(); virtual void Slot038(); virtual void Slot03C();
    virtual void Slot040(); virtual void Slot044(); virtual void Slot048(); virtual void Slot04C();
    virtual void Slot050(); virtual void Slot054(); virtual void Slot058(); virtual void Slot05C();
    virtual void Slot060(); virtual void Slot064(); virtual void Slot068(); virtual void Slot06C();
    virtual void Slot070(); virtual void Slot074(); virtual void Slot078(); virtual void Slot07C();
    virtual void Slot080(); virtual void Slot084(); virtual void Slot088(); virtual void Slot08C();
    virtual void Slot090(); virtual void Slot094(); virtual void Slot098(); virtual void Slot09C();
    virtual void Slot0A0(); virtual void Slot0A4(); virtual void Slot0A8(); virtual void Slot0AC();
    virtual void Slot0B0(); virtual void Slot0B4(); virtual void Slot0B8(); virtual void Slot0BC();
    virtual void Slot0C0(); virtual void Slot0C4(); virtual void Slot0C8(); virtual void Slot0CC();
    virtual void Slot0D0(); virtual void Slot0D4(); virtual void Slot0D8(); virtual void Slot0DC();
    virtual void Slot0E0(); virtual void Slot0E4(); virtual void Slot0E8();
    virtual void ChangeHealth(float amount, bool applyHealthChange);

    bool SetCurrentAction(const CCreatureActionBase& action);   // 0x006644F0
};

struct CreatureOverlay
{
    void* vtable;
    unsigned char unused04[0x8D];
    unsigned char flags91;
    unsigned char unused92[0x22];
    float healthB4;
};

class CCreatureAction_StabbedToDeath : public CCreatureActionBase
{
public:
    CCreatureAction_StabbedToDeath(CThingCreatureBase& creature, bool unknown);
private:
    unsigned char storage[0xA8];
};

#include "engine/CCountedPointer.h"
template <class T> class CTBaseSingletonNoDefaultAlloc {              // the CRTP singleton base of CPlayerGui
    static CCountedPointer<T> PClass;                                // 0x013B8790 ?PClass@?$CTBaseSingletonNoDefaultAlloc@VCPlayerGui@@@@0V?$CCountedPointer@VCPlayerGui@@@@A (private static)
public:
    static T& __fastcall Get() { return *PClass; }                   // Ego_r / FableWin ?Get@?$CTBaseSingletonNoDefaultAlloc@VCPlayerGui@@@@SIAAVCPlayerGui@@XZ (inline)
};
class CPlayerGui
{
public:
    void AddDamageChange(float amount, bool isDamage);
};
extern const float DELTA_NOTIONAL_ZERO;
extern const float gDamageThreshold;

class CGameScriptInterface
{
public:
    unsigned char unused00[0x14];
    HeroResolver* heroResolver;
    void ChangeHeroHealthBy(float amount, bool rawHealthMode, bool applyHealthChange) const;
};

void CGameScriptInterface::ChangeHeroHealthBy(
    float amount,
    bool rawHealthMode,
    bool applyHealthChange) const
{
    HeroHandle* handle = ((HeroHandle*)&((CPlayerManager*)heroResolver)->GetMainPlayer());
    if (handle == 0)
        return;
    CThingCreatureBase* hero = handle->item.GetPItem();
    if (hero == 0 || (((CreatureOverlay*)hero)->flags91 & 1) != 0)
        return;

    hero->ChangeHealth(amount, rawHealthMode);
    if (rawHealthMode && ((CreatureOverlay*)hero)->healthB4 < DELTA_NOTIONAL_ZERO)
    {
        hero->SetCurrentAction(CCreatureAction_StabbedToDeath(*hero, false));
    }
    if (*(const bool*)&applyHealthChange && amount < gDamageThreshold)
        CTBaseSingletonNoDefaultAlloc<CPlayerGui>::Get().AddDamageChange(-amount, true);
}