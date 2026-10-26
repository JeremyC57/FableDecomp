#pragma optimize("s",on)
// Leaf: OnReadFinished
struct CActiveFile {
  void* buf;
  void notify();
};
extern "C" void __cdecl free(void* block);   // 0x00BFEA14 (MSVCR71)
void __fastcall CActiveFile_OnReadFinished(CActiveFile* self) {
  self->notify();
  void* p = self->buf;
  if (p) free(p);
}