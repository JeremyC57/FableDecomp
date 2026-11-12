// CUserProfileManager::RedefineControlPassive(const CActionInputControl& new_control, const CActionInputControl& old_control, long index)
// -- replace the passive input that is the same action and button as the old control (for the two item actions only the index-th
// one of that action), then apply the profile values.
// Leaf: ?RedefineControlPassive@CUserProfileManager@@QAEXABVCActionInputControl@@0J@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00407CD0, 310 bytes (`ret 0Ch`).
// Naming: FableWin.pdb / Ego_r.pdb ?RedefineControlPassive@CUserProfileManager@@QAEXABVCActionInputControl@@0J@Z; the action values are
//   EGameAction GAME_ACTION_QUICK_ACCESS_ITEM (55) and GAME_ACTION_CONTEXT_SENSITIVE_ITEM (56); the two counter names are descriptive.
// Callees: CActionInputControl::GetDirection 0x0040ED10 (twice per candidate), CUserProfileManager::ApplyValues 0x00407370.
// Shape: every access is spelled AssignedInputsPassive[i] (no reference local: the entry address is formed after the first action
//   compare, `lea edi,[edx+ecx]` at 0x00407D23, and the array start is read again for the second switch); GetDirection's body is
//   visible to the TU (FABLE_RETAIL_CACTIONINPUTCONTROL_GETDIRECTION_BODY), so the old control's action stays in esi and the entry
//   address in edi across both calls; the copy is the trivial 28-byte assignment (`rep movsd`, 7 dwords); size() is recomputed
//   every pass; the context-sensitive counter starts at -1 and the quick-access one at 0, as retail initialises them.
#define FABLE_RETAIL_CUSERPROFILEMANAGER_PASSIVEINPUTS
#define FABLE_RETAIL_CACTIONINPUTCONTROL_GETDIRECTION_BODY
#include "engine/CUserProfileManager.h"
// The per-entry test, expanded in retail: a flat "same action, same direction, same controller type, same key / button of that
// controller" (1 Xbox button, 2 keyboard key, 3 mouse button). Not CActionInputControl::IsSameActionAndButton 0x0040EDA0 (which calls
// IsSameButton 0x00405DB0 and has no caller in retail): the nested pair expands with the result in cl. Name descriptive.
static inline bool IsSameControl(const CActionInputControl& control,const CActionInputControl& other)
{
    if(control.GameAction!=other.GameAction || control.GetDirection()!=other.GetDirection() || control.ControllerType!=other.ControllerType)
        return false;
    switch(control.ControllerType){
    case 1:
        if(control.XboxButton!=other.XboxButton)
            return false;
        break;
    case 2:
        if(control.KeyboardKey!=other.KeyboardKey)
            return false;
        break;
    case 3:
        if(control.MouseButton!=other.MouseButton)
            return false;
        break;
    }
    return true;
}
void CUserProfileManager::RedefineControlPassive(const CActionInputControl& new_control,const CActionInputControl& old_control,long index)
{
    long context_sensitive_count=-1;
    long quick_access_count=0;
    for(unsigned long i=0;i<AssignedInputsPassive.size();++i){
        bool same=IsSameControl(AssignedInputsPassive[i],old_control);
        switch(old_control.GameAction){
        case GAME_ACTION_QUICK_ACCESS_ITEM:
            if(quick_access_count==index && same)
                AssignedInputsPassive[i]=new_control;
            break;
        case GAME_ACTION_CONTEXT_SENSITIVE_ITEM:
            if(context_sensitive_count==index && same)
                AssignedInputsPassive[i]=new_control;
            break;
        default:
            if(same)
                AssignedInputsPassive[i]=new_control;
            break;
        }
        switch(AssignedInputsPassive[i].GameAction){
        case GAME_ACTION_QUICK_ACCESS_ITEM:
            ++quick_access_count;
            break;
        case GAME_ACTION_CONTEXT_SENSITIVE_ITEM:
            ++context_sensitive_count;
            break;
        }
    }
    ApplyValues();
}