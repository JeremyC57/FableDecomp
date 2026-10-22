#pragma optimize("s",on)
struct CPersistContext {                       // typed transfer helpers: each starts `push 0x122d70e ; call 0x404500` (CheckCRC)
    void TransferFloat(int* value);                 // 0x00431061 Transfer(float&)
    void Transfer_00466a47(int* value);             // 0x00466A47 per-type transfer instantiation, element type not attributed
};
struct CCameraManagerSetDef {
  char pad0[0x3c];
  int f3c;
  int CameraManagerMain;
  int CameraManagerCombat;
  int CameraManagerPCMain;
};
void __fastcall CCameraManagerSetDef_Transfer(CCameraManagerSetDef* def, void* unused, CPersistContext* ctx) {
  ctx->TransferFloat(&def->f3c);
  ctx->TransferFloat(&def->CameraManagerMain);
  ctx->TransferFloat(&def->CameraManagerCombat);
  ctx->Transfer_00466a47(&def->CameraManagerPCMain);
}