void __cdecl operator delete[](void* block);   // MSVCR71 import thunk 0x00BFEB1C (the lane <new> lacks it)
// Leaf: ClearTextures
void __cdecl operator delete(void* block);   // MSVCR71 import thunk 0x00BFE9BC

struct CInner {
};

extern void __fastcall CInner_dtor(CInner* p);

struct CLandscapeBackgroundPatch {
  int f0;
  int Parent;
  void* f8;
  CInner inner;
  void* CLandscapeBackgroundPatch_ClearTextures(int flag);
};

void* CLandscapeBackgroundPatch::CLandscapeBackgroundPatch_ClearTextures(int flag)
{
  if (this->f8) {
    operator delete[](this->f8);
    this->f8 = 0;
  }
  this->Parent = 0;
  CInner_dtor(&this->inner);
  if (flag & 1) {
    operator delete(this);
  }
  return this;
}