#pragma optimize("s",on)
class CPersistContext { public:
  template <class T> void Transfer(T& value);
};
class CCameraManagerSetDef { public:
  char pad0[0x3c];
  int f3c;
  int CameraManagerMain;
  int CameraManagerCombat;
  int CameraManagerPCMain;
};
void __fastcall CCameraManagerSetDef_Transfer(CCameraManagerSetDef* def, void* unused, CPersistContext* ctx) {
  ctx->Transfer(*(long*)(&def->f3c));
  ctx->Transfer(*(long*)(&def->CameraManagerMain));
  ctx->Transfer(*(long*)(&def->CameraManagerCombat));
  ctx->Transfer(*(long*)(&def->CameraManagerPCMain));
}