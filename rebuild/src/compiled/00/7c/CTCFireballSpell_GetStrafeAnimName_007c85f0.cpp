// CTCFireballSpell::GetStrafeAnimName() const -- choose the spell animation for weapon state and handedness.
// Leaf: ?GetStrafeAnimName@CTCFireballSpell@@QBE?AVCCharString@@XZ
// Identity: proven; PDB own const GetStrafeAnimName returns CCharString; caller CTCSpecialAbilities/FinishFireball SetAnims; own literal spell/pose names; WeaponState+100,LeftHandAnim+104.
// Layout: WillLightning reuses generated header unchanged with typed enum view over named opaque WeaponState. Fireball has no header; minimal data view retains the proved named fields+0x64/+0x68. String ctor uses actual external API.
class CCharStringData; class CCharString { public: CCharStringData* PStringData; CCharString(const char*,long=-1); };
enum EWEAPON_STATE { WEAPON_UNARMED=0,WEAPON_ARMED=1,WEAPON_BOW=2 };
class CTCFireballSpell { public: unsigned char _unmodeled00[0x64]; EWEAPON_STATE WeaponState; bool LeftHandAnim; CCharString GetStrafeAnimName() const; };
CCharString CTCFireballSpell::GetStrafeAnimName() const { switch(*reinterpret_cast<const EWEAPON_STATE*>(&WeaponState)) { case WEAPON_UNARMED: return CCharString(LeftHandAnim?"ANIM_HERO_WILL_FIREBALL_LEFTHANDED_AIM":"ANIM_HERO_WILL_FIREBALL_RIGHTHANDED_AIM"); case WEAPON_ARMED: return CCharString("ANIM_HERO_WILL_FIREBALL_ARMED_AIM"); case WEAPON_BOW: return CCharString("ANIM_HERO_WILL_FIREBALL_WITH_BOW_AIM"); default: return CCharString(""); } }