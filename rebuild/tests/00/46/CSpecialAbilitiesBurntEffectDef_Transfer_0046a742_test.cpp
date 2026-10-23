#include <cstdio>
struct CPersistContext {
  int count;
  int* seen[8];
  void Transfer(int* p) { if (count < 8) seen[count] = p; count++; }
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
int main() {
  CCameraManagerSetDef d;
  CPersistContext ctx; ctx.count = 0;
  CCameraManagerSetDef_Transfer(&d, 0, &ctx);
  if (ctx.count == 4 &&
      ctx.seen[0] == &d.f3c && ctx.seen[1] == &d.CameraManagerMain &&
      ctx.seen[2] == &d.CameraManagerCombat && ctx.seen[3] == &d.CameraManagerPCMain) {
    std::printf("CCameraManagerSetDef_0046a742_TEST PASS\n");
    return 0;
  }
  std::printf("FAIL count=%d\n", ctx.count);
  return 1;
}