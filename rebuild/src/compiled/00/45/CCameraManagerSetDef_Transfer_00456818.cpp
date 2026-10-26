#pragma optimize("s",on)
class CPersistContext { public:
  void Transfer(int* p);
};
class CCameraManagerSetDef { public:
  char pad0[0x3c];
  int f3c;
  int CameraManagerMain;
  int CameraManagerCombat;
  int CameraManagerPCMain;
};
void __fastcall CCameraManagerSetDef_Transfer(CCameraManagerSetDef* def, void* unused, CPersistContext* ctx) {
  ctx->Transfer(&def->f3c);
  ctx->Transfer(&def->CameraManagerMain);
  ctx->Transfer(&def->CameraManagerCombat);
  ctx->Transfer(&def->CameraManagerPCMain);
}