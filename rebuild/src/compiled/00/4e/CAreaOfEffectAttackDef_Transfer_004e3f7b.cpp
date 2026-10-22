#pragma optimize("s",on)
struct CPersistContext {                       // typed transfer helpers: each starts `push 0x122d70e ; call 0x404500` (CheckCRC)
    void TransferLong(int* value);                  // 0x00431102 Transfer(long&)
    void TransferFloat(int* value);                 // 0x00431061 Transfer(float&)
};
struct CCameraManagerSetDef {
  char pad0[0x3c];
  int f3c;
  int CameraManagerMain;
  int CameraManagerCombat;
  int CameraManagerPCMain;
};
void __fastcall CCameraManagerSetDef_Transfer(CCameraManagerSetDef* def, void* unused, CPersistContext* ctx) {
  ctx->TransferLong(&def->f3c);
  ctx->TransferLong(&def->CameraManagerMain);
  ctx->TransferFloat(&def->CameraManagerCombat);
  ctx->TransferFloat(&def->CameraManagerPCMain);
}