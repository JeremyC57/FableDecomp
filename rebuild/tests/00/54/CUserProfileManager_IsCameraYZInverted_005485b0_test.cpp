#include <cstdio>
struct CUserProfileManager { char pad[0xc]; unsigned char UseNewProfileValues; char pad_lo[16-0xd]; unsigned char blo; char pad_hi[208-(16+1)]; unsigned char CameraYZInvertedNewProfile; char IsCameraYZInverted(); };
char CUserProfileManager::IsCameraYZInverted(){ if(this->UseNewProfileValues) return this->CameraYZInvertedNewProfile; return this->blo; }
int main(){ CUserProfileManager o; o.blo=3; o.CameraYZInvertedNewProfile=7;
 o.UseNewProfileValues=1; if(o.IsCameraYZInverted()!=7){ std::printf("005485b0_TEST FAIL\n"); return 1;}
 o.UseNewProfileValues=0; if(o.IsCameraYZInverted()!=3){ std::printf("005485b0_TEST FAIL\n"); return 1;}
 std::printf("005485b0_TEST PASS\n"); return 0;}