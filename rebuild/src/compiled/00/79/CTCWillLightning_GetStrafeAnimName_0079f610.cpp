// CTCWillLightning::GetStrafeAnimName() const -- choose the spell animation for weapon state and handedness.
// Leaf: ?GetStrafeAnimName@CTCWillLightning@@QBE?AVCCharString@@XZ
// Identity: proven; PDB own const GetStrafeAnimName returns CCharString; caller CTCSpecialAbilities/FinishFireball SetAnims; own literal spell/pose names; WeaponState+12,LeftHandAnim+16.
// Layout: WillLightning reuses generated header unchanged with typed enum view over named opaque WeaponState. Fireball has no header; minimal data view retains the proved named fields+0x64/+0x68. String ctor uses actual external API.
#define CTCWillLightning CTCWillLightning_HeaderLayout
#include "engine/CTCWillLightning.h"
#undef CTCWillLightning
class CCharStringData; class CCharString { public: CCharStringData* PStringData; CCharString(const char*,long=-1); };
enum EWEAPON_STATE { WEAPON_UNARMED=0,WEAPON_ARMED=1,WEAPON_BOW=2 };
class CTCWillLightning:public CTCWillLightning_HeaderLayout { public: CCharString GetStrafeAnimName() const; };
CCharString CTCWillLightning::GetStrafeAnimName() const { switch(*reinterpret_cast<const EWEAPON_STATE*>(&WeaponState)) { case WEAPON_UNARMED: return CCharString(LeftHandAnim?"ANIM_HERO_WILL_LIGHTNING_LEFTHANDED_AIM":"ANIM_HERO_WILL_LIGHTNING_RIGHTHANDED_AIM"); case WEAPON_ARMED: return CCharString("ANIM_HERO_WILL_LIGHTNING_ARMED_AIM"); case WEAPON_BOW: return CCharString("ANIM_HERO_WILL_LIGHTNING_WITH_BOW_AIM"); default: return CCharString(""); } }