// CCreatureActionBase::GetActionID() const -- returns the action's id (ActionID, +0x28)
// Leaf: ?GetActionID@CCreatureActionBase@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00692620, 4 bytes (`mov eax,[ecx+28h]; ret`).
// Identity: the PDB name sets hold ?GetActionID@CCreatureActionBase@@QBEJXZ (no GetActionCode); Ego_r and
// engine/CCreatureActionBase.h put long ActionID at +0x28 (no STL member before it, so the retail offset is the
// same); all 33 call sites are creature-action code (CActionDoCreatureAction, the scripted-control actions,
// CAIStateGroup_* perform-action helpers, CTCStealth::FrameUpdate).  The old body was a __fastcall free function
// under the propagated label "CWorld::DrawGetEnvironment".
// The shared header is a flat generated struct with no member functions, so it is included under another name
// and the member offset is asserted against it.
#define CCreatureActionBase CCreatureActionBasePdb
#include "engine/CCreatureActionBase.h"
#undef CCreatureActionBase

class CCreatureActionBase {
public:
    unsigned char _pad00[0x28];
    long ActionID;                                  // +0x28
    long GetActionID() const;
};
FABLE_STATIC_ASSERT(offsetof(CCreatureActionBase, ActionID) == offsetof(CCreatureActionBasePdb, ActionID));

long CCreatureActionBase::GetActionID() const
{
    return ActionID;
}
