// CTCBulletTime::GetTimeMultiplier() const -- the bullet-time def's HeroCombatSlowness (the hero's combat time multiplier while bullet
// time runs)
// Leaf: ?GetTimeMultiplier@CTCBulletTime@@QBEMXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0079C740, 10 bytes (`mov eax,[ecx+20h] ; fld dword [eax+12Ch] ; ret`).
// Name (agentNF26, 2026-11-12): Ego_r.pdb + FableWin.pdb ?GetTimeMultiplier@CTCBulletTime@@QBEMXZ, already declared by
//   engine/CTCBulletTime.h at 0x0079C740. Both call sites (0x006A036C in landed 0x006A02B0, 0x008CDEB7 in landed 0x008CDD40) call it on the
//   PeekTC<CTCBulletTime> (TC 146 TCI_BULLET_TIME) result. The body reads +0x12C of the def at +0x20: Ego_r CTCBulletTime::PDef
//   (CDefPointer<const CSpecialAbilitiesBulletTimeDef>, the header's PDef_Object) and CSpecialAbilitiesBulletTimeDef::HeroCombatSlowness
//   (+0x12C in retail: landed Transfer 0x00464DB5 and ctor 0x00457537 put it there). Was _unattributed::GetFloatThrough_0x20_0x12c_0079c740
//   (descriptive), earlier CTCPhysicsBase::GetAirResistance (a propagated manifest label).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCBULLETTIME
#define FABLE_RETAIL_CTCBULLETTIME_MEMBERS
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/ETCInterfaceType.h"
#include "engine/CTCBulletTime.h"
#include "engine/CSpecialAbilitiesBulletTimeDef.h"

float CTCBulletTime::GetTimeMultiplier() const
{
    return ((const CSpecialAbilitiesBulletTimeDef*)PDef_Object)->HeroCombatSlowness;
}
