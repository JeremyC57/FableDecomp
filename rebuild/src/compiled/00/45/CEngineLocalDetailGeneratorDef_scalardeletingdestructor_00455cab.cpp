#pragma optimize("s",on)
#include <new>

class CParentDefClassBase {
public:
    virtual ~CParentDefClassBase();   // 0x00430300
};
class CEngineLocalDetailGeneratorDef;

struct CMember { void __fastcall dtor(); };
extern void __fastcall CMember_dtor_0045334a(CMember* def);

class CEngineLocalDetailGeneratorDef { public:
  char pad[0x3c];
};

void* __fastcall CEngineLocalDetailGeneratorDef_scalar_deleting_destructor(CEngineLocalDetailGeneratorDef* def, int /*edx*/, unsigned int flags)
{
  CMember_dtor_0045334a((CMember*)((char*)def + 0x3c));
  ((CParentDefClassBase*)(def))->CParentDefClassBase::~CParentDefClassBase();
  if (flags & 1) {
    ::operator delete(def);
  }
  return def;
}