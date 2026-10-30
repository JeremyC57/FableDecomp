class CLandscapeBackgroundPatch { public:
// Leaf: ClearTextures
  void* vtable;   // +0
  void* tex;      // +4
};

// IAT slot 0x0143FE30 = KERNEL32.dll!CloseHandle (__imp__CloseHandle@4); callee cleans its own stack arg
extern "C" __declspec(dllimport) int __stdcall CloseHandle(void* t);

void __fastcall CLandscapeBackgroundPatch_ClearTextures(CLandscapeBackgroundPatch* self)
{
  void* tex = self->tex;
  self->vtable = (void*)0x12b90ec;
  if (tex) {
    CloseHandle(tex);
    self->tex = 0;
  }
}