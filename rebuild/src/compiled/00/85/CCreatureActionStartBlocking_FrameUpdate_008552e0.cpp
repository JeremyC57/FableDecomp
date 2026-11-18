// CCreatureAction_StartBlocking::FrameUpdate() -- set the action's +0x10 word to 2, then run the base CCombatActionBase::FrameUpdate (tail jmp)
// Leaf: ?FrameUpdate@CCreatureAction_StartBlocking@@UAEXXZ
// NF75 (round 7, land1117fu q10916): the base call binds Ego_r.pdb / FableWin.pdb ?FrameUpdate@CCombatActionBase@@UAEXXZ (public virtual,
//   0x00859D80; was the extern "C" stand-in @FrameUpdate_CCombatActionBase@4), called non-virtually as retail's tail `jmp` shows; the
//   row's own member is the PDB ?FrameUpdate@CCreatureAction_StartBlocking@@UAEXXZ (public virtual; was a __fastcall non-virtual).
//   In the row's pre-existing local model the first word is the vptr of those virtuals (was `int vtbl`); no field moves.
class CCombatActionBase
{ public:
    virtual void FrameUpdate();     // 0x00859D80
    int IntelligentPointer;
    int field_8;
    int field_c;
    int field_10;
};

class CCreatureAction_StartBlocking : public CCombatActionBase
{ public:
    virtual void FrameUpdate();
};

void CCreatureAction_StartBlocking::FrameUpdate()
{
    field_10 = 2;
    CCombatActionBase::FrameUpdate();
}
