// Native intrusive-pointer scalar deleting destructor @009CA780, corrected47-byte boundary.
// Original308-byte oracle spans unrelated functions. See boundary audit and preserved original.
// No code/data xrefs identify the concrete template argument; CHeroAbilityDef is a propagated label.
// Wrapper is one pointee pointer at0 with NO vptr. Pointee has genuine deletingdtor slot0, Delete slot1, refs4.
#define CIVCountedPointeeBase CIVCountedPointeeBasePdb
#include "engine/CIVCountedPointeeBase.h"
#undef CIVCountedPointeeBase
struct CIVCountedPointeeBase {
 virtual ~CIVCountedPointeeBase();
 virtual void Delete()const;
 unsigned long IVRefCount;
};
struct CNativeIntrusivePointer {
 CIVCountedPointeeBase* Object;
 ~CNativeIntrusivePointer(){CIVCountedPointeeBase* p=Object;if(p){if(--p->IVRefCount==0)p->Delete();Object=0;}}
 void* ScalarDeletingDestructor(unsigned long flags);
};
void* CNativeIntrusivePointer::ScalarDeletingDestructor(unsigned long flags) {
 this->~CNativeIntrusivePointer();
 if(flags&1)operator delete(this);
 return this;
}