#include <cstdio>
struct CUserProfileManager { char pad[0xc]; unsigned char b0c; char pad_lo[16-0xd]; unsigned char blo; char pad_hi[208-(16+1)]; unsigned char CameraYZInvertedNewProfile; char IsCameraYZInverted(); };
char CUserProfileManager::IsCameraYZInverted(){ if(this->b0c) return this->CameraYZInvertedNewProfile; return this->blo; }
int main(){ CUserProfileManager o; o.blo=3; o.CameraYZInvertedNewProfile=7;
 o.b0c=1; if(o.IsCameraYZInverted()!=7){ std::printf("005485b0_TEST FAIL\n"); return 1;}
 o.b0c=0; if(o.IsCameraYZInverted()!=3){ std::printf("005485b0_TEST FAIL\n"); return 1;}
 std::printf("005485b0_TEST PASS\n"); return 0;}