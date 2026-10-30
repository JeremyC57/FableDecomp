#pragma optimize("s",on)
#include <new>

// Layout: CParentDefClassBase is the retail def chain from the shared headers (FABLE_RETAIL_DEF_CLASS_BASE opt-in:
//   vtable 0x01230CBC, 0x39 bytes packed; ctor 0x00430370, ~CParentDefClassBase 0x00430300, copy 0x0046EE98,
//   operator= 0x00431ED0).
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CParentDefClassBase.h"
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