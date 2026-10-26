#pragma optimize("s",on)
struct SubObj { int x; };
extern "C" void __fastcall SubObj_dtor(SubObj* p);
class CEnvironmentThemeDaySetDef { public:
  char pad[0x3c];
  SubObj member;
  void realdtor();
  void* scalar_deleting_destructor(unsigned int flags);
};

void* CEnvironmentThemeDaySetDef::scalar_deleting_destructor(unsigned int flags) {
  SubObj_dtor((SubObj*)((char*)this + 0x3c));
  this->realdtor();
  if (flags & 1) {
    operator delete((void*)this);
  }
  return this;
}