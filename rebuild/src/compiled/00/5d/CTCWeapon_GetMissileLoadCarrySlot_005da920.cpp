// CTCWeapon::GetMissileLoadCarrySlot() const -- the carry slot a loaded missile goes to: the weapon definition's MissileLoadCarrySlot (+0x94),
//   read through a CDefPointer copy (AddRef, read, Release / delete on zero)
// Leaf: ?GetMissileLoadCarrySlot@CTCWeapon@@QBEJXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005DA920, length 35 bytes; /O2 TU (the release is `mov eax,[ecx+4] ; dec eax ; mov [ecx+4],eax`).
// Name (NF103, round 10, q11533): Ego_r.pdb 0x0063A760 / FableWin.pdb 0x0239B390 ?GetMissileLoadCarrySlot@CTCWeapon@@QBEJXZ; Ego_r donor order
//   IsAnalogueLoading 0x0063A730 then GetMissileLoadCarrySlot = retail 0x005DA8F0 / 0x005DA920; the body reads CWeaponDef::MissileLoadCarrySlot
//   (+0x94 in the retail layout of engine/CWeaponDef.h); the landed binders 0x008D5790 / 0x0078B070 spell it. Was the stand-in
//   CTCWeapon_Retail::GetWeaponDefLongAt94 over local CWeaponDef / CDefPointer models; now on the shared headers (the 0x005DA6E0 define set).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFPOINTER_RELEASE_SELFDELETING_POINTEE
#define FABLE_RETAIL_CIVCOUNTEDPOINTEEBASE_RELEASE_DELETES
#define FABLE_RETAIL_CTCWEAPON
#define FABLE_RETAIL_CTCWEAPON_MEMBERS
#define FABLE_RETAIL_CTCWEAPON_ONCREATE
#define FABLE_RETAIL_CTCWEAPON_GETMISSILELOADCARRYSLOT
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CThingBaseDef.h"
#include "engine/CWeaponDef.h"
#include "engine/CTCWeapon.h"

long CTCWeapon::GetMissileLoadCarrySlot() const
{
    CDefPointer<const CWeaponDef> weapon_def(WeaponDef);
    return weapon_def->MissileLoadCarrySlot;
}
