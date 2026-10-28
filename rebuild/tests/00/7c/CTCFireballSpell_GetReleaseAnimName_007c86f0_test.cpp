#include <stdio.h>
#include <string.h>
class CCharStringData; class CCharString { public: CCharStringData* PStringData; CCharString(const char*,long=-1); };
enum EWEAPON_STATE { WEAPON_UNARMED=0,WEAPON_ARMED=1,WEAPON_BOW=2 };
class CTCFireballSpell { public: unsigned char _unmodeled00[0x64]; EWEAPON_STATE WeaponState; bool LeftHandAnim; CCharString GetReleaseAnimName() const; };
CCharString CTCFireballSpell::GetReleaseAnimName() const { switch(*reinterpret_cast<const EWEAPON_STATE*>(&WeaponState)) { case WEAPON_UNARMED: return CCharString(LeftHandAnim?"ANIM_HERO_WILL_FIREBALL_LEFTHANDED_RELEASE":"ANIM_HERO_WILL_FIREBALL_RIGHTHANDED_RELEASE"); case WEAPON_ARMED: return CCharString("ANIM_HERO_WILL_FIREBALL_ARMED_RELEASE"); case WEAPON_BOW: return CCharString("ANIM_HERO_WILL_FIREBALL_WITH_BOW_RELEASE"); default: return CCharString(""); } }
static int bad,calls; CCharString::CCharString(const char* text,long length) { if(length!=-1) bad=1; ++calls; PStringData=(CCharStringData*)text; }
int main() { CTCFireballSpell object; const char* expected[5]={"ANIM_HERO_WILL_FIREBALL_RIGHTHANDED_RELEASE","ANIM_HERO_WILL_FIREBALL_LEFTHANDED_RELEASE","ANIM_HERO_WILL_FIREBALL_ARMED_RELEASE","ANIM_HERO_WILL_FIREBALL_WITH_BOW_RELEASE",""};
for(int i=0;i<5;++i) { *reinterpret_cast<EWEAPON_STATE*>(&object.WeaponState)=(EWEAPON_STATE)(i<2?0:i-1); object.LeftHandAnim=i==1; CCharString value=object.GetReleaseAnimName(); if(bad || strcmp((const char*)value.PStringData,expected[i])) goto fail; } if(calls!=5) goto fail; puts("007c86f0_TEST PASS"); return 0; fail: puts("007c86f0_TEST FAIL"); return 1; }