#pragma optimize("s",on)
class CPersistContext { public:                       // typed transfer helpers: each starts `push 0x122d70e ; call 0x404500` (CheckCRC)
    template <class T> void Transfer(T& value);                  // 0x00431102 Transfer(long&)
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
  ctx->Transfer(*(float*)(&def->CameraManagerCombat));
  ctx->Transfer(*(float*)(&def->CameraManagerPCMain));
}