#pragma optimize("s",on)
// Leaf: OnReadFinished
extern "C" void __cdecl free(void* block);   // 0x00BFEA14 (MSVCR71)
struct CActiveFile {
  void* buf;
  void OtherStep();
};
void __fastcall OnReadFinished(CActiveFile* self) {
  self->OtherStep();
  if (self->buf)
    free(self->buf);
}