#pragma optimize("s",on)
struct CPersistContext {
  void Transfer(int* p);
};
struct CCameraManagerSetDef {
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