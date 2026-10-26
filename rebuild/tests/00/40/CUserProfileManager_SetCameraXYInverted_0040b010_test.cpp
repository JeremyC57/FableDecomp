#include <cstdio>

class CUserProfileManager { public:
  char pad0[0xc];
  char gate;
  char pad1[2];
  char CameraXYInverted;
  char pad2[0xbf];
  char CameraXYInvertedNewProfile;
};

static int g_sub1=0, g_sub2=0;
void __fastcall CUPM_sub1(CUserProfileManager* self){ (void)self; g_sub1++; }
void __fastcall CUPM_sub2(CUserProfileManager* self){ (void)self; g_sub2++; }

void __fastcall CUserProfileManager_SetCameraXYInverted(CUserProfileManager* self, char a, char b)
{
  if (self->gate)
    self->CameraXYInvertedNewProfile = a;
  self->CameraXYInverted = a;
  CUPM_sub1(self);
  if (b)
    CUPM_sub2(self);
}

int main(){
  CUserProfileManager o;
  o.gate=1; o.CameraXYInverted=0; o.CameraXYInvertedNewProfile=0;
  g_sub1=g_sub2=0;
  CUserProfileManager_SetCameraXYInverted(&o, 7, 1);
  if(o.CameraXYInvertedNewProfile!=7){ std::printf("FAIL flagCF\n"); return 1; }
  if(o.CameraXYInverted!=7){ std::printf("FAIL flagF\n"); return 1; }
  if(g_sub1!=1){ std::printf("FAIL sub1\n"); return 1; }
  if(g_sub2!=1){ std::printf("FAIL sub2\n"); return 1; }

  o.gate=0; o.CameraXYInverted=0; o.CameraXYInvertedNewProfile=3;
  g_sub1=g_sub2=0;
  CUserProfileManager_SetCameraXYInverted(&o, 9, 0);
  if(o.CameraXYInvertedNewProfile!=3){ std::printf("FAIL flagCF2\n"); return 1; }
  if(o.CameraXYInverted!=9){ std::printf("FAIL flagF2\n"); return 1; }
  if(g_sub1!=1){ std::printf("FAIL sub1b\n"); return 1; }
  if(g_sub2!=0){ std::printf("FAIL sub2b\n"); return 1; }

  std::printf("CUserProfileManager_0040b010_TEST PASS\n");
  return 0;
}