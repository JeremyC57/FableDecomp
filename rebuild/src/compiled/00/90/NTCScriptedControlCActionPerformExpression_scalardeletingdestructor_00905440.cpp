#pragma optimize("t", on)
class CCPPointerInfo;
template <class T> class CCountedPointer { public: T* Data; CCPPointerInfo* Info; };   // Ego_r.pdb members (Data, Info)
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
extern void __fastcall DestructFirstMember_0099eae0(void* member);
extern void __fastcall CActionPerformExpression_Destruct(void* action);
void* __fastcall CActionPerformExpression_CompositeScalarDeletingDestructor(void* action, int, unsigned int flags)
{
    DestructFirstMember_0099eae0(static_cast<unsigned char*>(action) + 28);
    ((CScriptThing*)(static_cast<unsigned char*>(action) + 12))->CScriptThing::~CScriptThing();
    CActionPerformExpression_Destruct(action);
    if (flags & 1)
        operator delete(action);
    return action;
}