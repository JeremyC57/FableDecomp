#include <cstdio>
struct CUserProfileManager { char pad[0xc]; unsigned char UseNewProfileValues; char pad_lo[15-0xd]; unsigned char blo; char pad_hi[207-(15+1)]; unsigned char CameraXYInvertedNewProfile; char IsCameraXYInverted(); };
char CUserProfileManager::IsCameraXYInverted(){ if(this->UseNewProfileValues) return this->CameraXYInvertedNewProfile; return this->blo; }
int main(){ CUserProfileManager o; o.blo=3; o.CameraXYInvertedNewProfile=7;
 o.UseNewProfileValues=1; if(o.IsCameraXYInverted()!=7){ std::printf("00548590_TEST FAIL\n"); return 1;}
 o.UseNewProfileValues=0; if(o.IsCameraXYInverted()!=3){ std::printf("00548590_TEST FAIL\n"); return 1;}
 std::printf("00548590_TEST PASS\n"); return 0;}