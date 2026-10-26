extern "C" void __cdecl free(void* block);   // 0x00BFEA14 (MSVCR71)

class CBankFileAsyncData { public:
  char pad0[0xc];
  void* Buffer;      /* +0x0c */
  char pad10[0x1c-0x10];
  unsigned int MemoryPool; /* +0x1c */
  void* p20;      /* +0x20 */
  void Inner(void* a, unsigned int b);
};

void __fastcall CBankFileAsyncData_FreeBuffer(CBankFileAsyncData* self)
{
  if (self->p20 != 0) {
    self->Inner(self->Buffer, self->MemoryPool + 4);
    if (self->p20 != 0) {
      free(self->p20);
    }
  }
}