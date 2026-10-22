struct CUserProfileManager {
  char pad0[0xc];
  char gate;   /* +0xc */
  char pad1[2];
  char CameraXYInverted;  /* +0xf */
  char pad2[0xbf];
  char CameraXYInvertedNewProfile; /* +0xcf */
  void CUserProfileManager_SetCameraXYInverted(char a, char b);
  void sub1();
  void sub2();
};

void CUserProfileManager::CUserProfileManager_SetCameraXYInverted(char a, char b)
{
  if (this->gate)
    this->CameraXYInvertedNewProfile = a;
  this->CameraXYInverted = a;
  this->sub1();
  if (b)
    this->sub2();
}