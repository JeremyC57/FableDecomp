// NUISystem::CManager::ClearActionParams() -- empty both action-parameter lists: the UI action params (+0x60) then the thing action params (+0x5C)
// Leaf: ?ClearActionParams@CManager@NUISystem@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0041D207, 20 bytes (vtable 0x01230134 slot 29), thiscall, tail-jumps the second clear.
// Name: Ego_r.pdb and FableWin.pdb ?ClearActionParams@CManager@NUISystem@@UAEXXZ; slot order of the generated header
//   (AddThingActionParam .. RemoveUIActionParam, ClearActionParams, SetNumberAugmentations .. SetMetaLayer 34, GetLayer 36,
//   ClearBastardChild 37) puts it at slot 29. Was landed as a descriptive CObject_0041d207::DestroyMembers.
// Layout: Ego_r ThingActionParams list<long> / UIActionParams list<CActionParam*>; retail (STLport lists 4 bytes, maps 0xC,
//   two fewer inventory pointers than the Ego_r donor) puts them at +0x5C / +0x60 (BastardChild +0x80 and RightStickForce +0x94
//   as in the landed ClearBastardChild 0x0041DED2). Engine header NUISystem_CManager.h is the donor layout (header debt).
// Callees: _List_base<CActionParam*>::clear 0x0042A141, _List_base<long>::clear 0x0042A168 (STLport 4.6 _list.c, out of line).
#define FABLE_RETAIL_STLP_LIST_CLEAR_OUTOFLINE
#include "stlport/fable_containers.h"
namespace NUISystem {
class CActionParam;
class CManager { public:
    virtual void Slot0();
    unsigned char _pad04[0x5c - 4];
    std::list<long> ThingActionParams;                  // +0x5C
    std::list<CActionParam*> UIActionParams;            // +0x60
    virtual void ClearActionParams();
};
void CManager::ClearActionParams()
{
    UIActionParams.clear();
    ThingActionParams.clear();
}
}
