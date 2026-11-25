#pragma optimize("t", on)
// Naming (NF423, 2026-11-25): the member release 0x005ED2C0 now binds its PDB spelling ??1?$CCountedPointer@VCScriptModuleBase@NCameraModes@@@@QAE@XZ (FableWin.pdb name set; the self row's
//   Leaf), called on the member through the shared engine/CCountedPointer.h under FABLE_RETAIL_CCOUNTEDPOINTER_DTOR_DECL_ONLY (declaration only: retail calls the out-of-line COMDAT); was the free stand-in ?DestructMember_005ed2c0@@YIXPAX@Z. Object otherwise unchanged.
#define FABLE_RETAIL_CCOUNTEDPOINTER_DTOR_DECL_ONLY
#include "engine/CCountedPointer.h"
namespace NCameraModes {
class CScriptModuleBase;
struct CBase {                                         // RTTI base of every camera mode
    virtual ~CBase();                                  // 0x0056E0E0 (Ego_r ??1CBase@NCameraModes@@UAE@XZ)
};
}
void* __fastcall CScriptedControl_CompositeScalarDeletingDestructor(void* control, int, unsigned int flags)
{
    reinterpret_cast<CCountedPointer<NCameraModes::CScriptModuleBase>*>(static_cast<unsigned char*>(control) + 164)->CCountedPointer<NCameraModes::CScriptModuleBase>::~CCountedPointer();
    ((NCameraModes::CBase*)control)->NCameraModes::CBase::~CBase();   // the base destructor (namepass CAM2)
    if (flags & 1)
        operator delete(control);
    return control;
}