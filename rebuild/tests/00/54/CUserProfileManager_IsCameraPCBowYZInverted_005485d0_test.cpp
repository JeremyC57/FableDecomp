#include <cstdio>
struct CUserProfileManager { char pad[0xc]; unsigned char b0c; char pad_lo[20-0xd]; unsigned char blo; char pad_hi[212-(20+1)]; unsigned char CameraPCBowYZInvertedNewProfile; char IsCameraPCBowYZInverted(); };
char CUserProfileManager::IsCameraPCBowYZInverted(){ if(this->b0c) return this->CameraPCBowYZInvertedNewProfile; return this->blo; }
int main(){ CUserProfileManager o; o.blo=3; o.CameraPCBowYZInvertedNewProfile=7;
 o.b0c=1; if(o.IsCameraPCBowYZInverted()!=7){ std::printf("005485d0_TEST FAIL\n"); return 1;}
 o.b0c=0; if(o.IsCameraPCBowYZInverted()!=3){ std::printf("005485d0_TEST FAIL\n"); return 1;}
 std::printf("005485d0_TEST PASS\n"); return 0;}