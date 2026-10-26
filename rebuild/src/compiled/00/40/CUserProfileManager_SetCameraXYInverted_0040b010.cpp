class CUserProfileManager { public:
  char pad0[0xc];
  char gate;   /* +0xc */
  char pad1[2];
  char CameraXYInverted;  /* +0xf */
  char pad2[0xbf];
  char CameraXYInvertedNewProfile; /* +0xcf */
  void CUserProfileManager_SetCameraXYInverted(char a, char b);
  void sub2();
protected:
    void ApplyValues();   // 0x00407370
};

void CUserProfileManager::CUserProfileManager_SetCameraXYInverted(char a, char b)
{
  if (this->gate)
    this->CameraXYInvertedNewProfile = a;
  this->CameraXYInverted = a;
  ApplyValues();
  if (b)
    this->sub2();
}