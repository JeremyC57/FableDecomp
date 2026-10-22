#pragma optimize("s",on)
struct CPersistContext {                       // typed transfer helpers: each starts `push 0x122d70e ; call 0x404500` (CheckCRC)
    void TransferLong(int* value);                  // 0x00431102 Transfer(long&)
    void TransferFloat(int* value);                 // 0x00431061 Transfer(float&)
};
struct CCameraManagerSetDef {
  char pad0[0x3c];
  int f3c;
  int f40;
  int f44;
  int f48;
};
void __fastcall CCameraManagerSetDef_Transfer(CCameraManagerSetDef* self, void* unused, CPersistContext* ctx) {
  ctx->TransferLong(&self->f3c);
  ctx->TransferLong(&self->f40);
  ctx->TransferFloat(&self->f44);
  ctx->TransferFloat(&self->f48);
}