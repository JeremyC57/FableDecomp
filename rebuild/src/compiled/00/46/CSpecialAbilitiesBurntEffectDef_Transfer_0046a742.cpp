#pragma optimize("s",on)
struct CPersistContext {                       // typed transfer helpers: each starts `push 0x122d70e ; call 0x404500` (CheckCRC)
    void TransferFloat(int* value);                 // 0x00431061 Transfer(float&)
    void Transfer_00466a47(int* value);             // 0x00466A47 per-type transfer instantiation, element type not attributed
};
struct CCameraManagerSetDef {
  char pad0[0x3c];
  int f3c;
  int f40;
  int f44;
  int f48;
};
void __fastcall CCameraManagerSetDef_Transfer(CCameraManagerSetDef* self, void* unused, CPersistContext* ctx) {
  ctx->TransferFloat(&self->f3c);
  ctx->TransferFloat(&self->f40);
  ctx->TransferFloat(&self->f44);
  ctx->Transfer_00466a47(&self->f48);
}