#pragma optimize("t", on)
// Leaf: ?CThingSwitch_CompositeScalarDeletingDestructor@@YIPAXPAXHI@Z
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
// Naming wave 28 (family sfb): the base destructor is ??1CSpawnedFuncBase@@UAE@XZ 0x00CDD4C0 (chunk 696: the ctor 0x00CDD450 stores vtable
//   0x012C2F9C, whose ??_G 0x00CDD4A0 calls it; Ego_r.pdb + FableWin.pdb), called qualified. Was the free
//   __fastcall ?AutoTinyCompositeDestructor@@YIXPAX@Z stand-in.
// Naming (NF423, 2026-11-25): the member release 0x00CE1000 now binds its PDB spelling ??1?$CCountedPointer@VCEntityScriptBase@@@@QAE@XZ
//   (FableWin.pdb name set; the self row's Leaf; retail calls 0x00CE1000 here), called on the +0x34 member through the shared
//   engine/CCountedPointer.h under FABLE_RETAIL_CCOUNTEDPOINTER_DTOR_DECL_ONLY (declaration only: retail calls the out-of-line COMDAT);
//   was the free stand-in ?AutoTinySecondMemberDestructor@@YIXPAX@Z. Object otherwise unchanged.
#define FABLE_RETAIL_CCOUNTEDPOINTER_DTOR_DECL_ONLY
#include "engine/CCountedPointer.h"
class CEntityScriptBase;
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
class CSpawnedFuncBase { public: virtual ~CSpawnedFuncBase(); };   // 0x00CDD4C0 (vtable 0x012C2F9C)
void* __fastcall CThingSwitch_CompositeScalarDeletingDestructor(void* self, int, unsigned int flags)
{
    ((CScriptThing*)(static_cast<unsigned char*>(self) + 72))->CScriptThing::~CScriptThing();
    reinterpret_cast<CCountedPointer<CEntityScriptBase>*>(static_cast<unsigned char*>(self) + 52)->CCountedPointer<CEntityScriptBase>::~CCountedPointer();
    ((CSpawnedFuncBase*)(self))->CSpawnedFuncBase::~CSpawnedFuncBase();
    if (flags & 1)
        operator delete(self);
    return self;
}