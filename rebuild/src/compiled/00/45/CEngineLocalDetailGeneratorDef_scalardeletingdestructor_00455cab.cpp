#pragma optimize("s",on)
#include <new>

struct CEngineLocalDetailGeneratorDef;

struct CMember { void __fastcall dtor(); };
extern void __fastcall CMember_dtor_0045334a(CMember* def);
extern void __fastcall CEngineLocalDetailGeneratorDef_dtor(CEngineLocalDetailGeneratorDef* def);

struct CEngineLocalDetailGeneratorDef {
  char pad[0x3c];
};

void* __fastcall CEngineLocalDetailGeneratorDef_scalar_deleting_destructor(CEngineLocalDetailGeneratorDef* def, int /*edx*/, unsigned int flags)
{
  CMember_dtor_0045334a((CMember*)((char*)def + 0x3c));
  CEngineLocalDetailGeneratorDef_dtor(def);
  if (flags & 1) {
    ::operator delete(def);
  }
  return def;
}