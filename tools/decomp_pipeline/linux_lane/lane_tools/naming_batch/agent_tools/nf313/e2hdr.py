# e2hdr.py: add the FABLE_RETAIL_CAISTATEGROUPBASE_FLAT_AIACTION_LOOKATPOS opt-in (declarations only) to the overlay's flat view
p='/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad/w22/agentNF313/hdr/engine/CAIStateGroupBase.h'
t=open(p,encoding='utf-8').read()
old="""class CAIBrain;
class CCharStringData;

#pragma pack(push, 1)
class CAIStateGroupBase {
public:
    void*            __vftable;                              // +0x00 vptr, or first dword of a flattened base subobject
"""
assert t.count(old)==1
new="""class CAIBrain;
class CCharStringData;
#ifdef FABLE_RETAIL_CAISTATEGROUPBASE_FLAT_AIACTION_LOOKATPOS
// NF313 add-only opt-in (2026-11-23, binder 0x0094A6D0 CAIStateGroup_GoToPub::WalkToPoint, whose local struct derives this flat view):
// declaration only of the protected base member AIAction_LookAtPos 0x0097D580, Ego_r.pdb + FableWin.pdb
// ?AIAction_LookAtPos@CAIStateGroupBase@@IAE?AW4EAIActionReturn@@W4EAIActionRepeat@@VC3DVector@@JM@Z (a member's mangling carries no
// class key, so the flat view gives the PDB spelling; protected, so only a derived class can call it). MS forward enums; the row
// defines the enumerators it uses.
class C3DVector;
enum EAIActionReturn; enum EAIActionRepeat;
#endif

#pragma pack(push, 1)
class CAIStateGroupBase {
#ifdef FABLE_RETAIL_CAISTATEGROUPBASE_FLAT_AIACTION_LOOKATPOS
protected:
    EAIActionReturn AIAction_LookAtPos(EAIActionRepeat repeat, C3DVector pos, long flags, float speed);   // 0x0097D580
#endif
public:
    void*            __vftable;                              // +0x00 vptr, or first dword of a flattened base subobject
"""
open(p,'w',encoding='utf-8').write(t.replace(old,new)); print('hdr ok')
p='/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad/w22/agentNF313/rows/0094a6d0.cpp'
L=open(p,encoding='utf-8').read().split('\n')
assert L[11]=='#include "fable_compiler.h"' and L[12]=='#include "engine/CAIStateGroupBase.h"' and L[13]=='', L[11:14]
L[12:14]=['#define FABLE_RETAIL_CAISTATEGROUPBASE_FLAT_AIACTION_LOOKATPOS   // NF313: the protected base member 0x0097D580 (declaration only)','#include "engine/CAIStateGroupBase.h"']
i=[k for k,l in enumerate(L) if 'EAIActionReturn AIAction_LookAtPos(EAIActionRepeat repeat, C3DVector pos, long lookFlags, float tolerance);' in l]
assert len(i)==1
L[i[0]]='    // AIAction_LookAtPos 0x0097D580: the protected CAIStateGroupBase member (header opt-in above; Ego_r + FableWin IAE spelling)'
open(p,'w',encoding='utf-8').write('\n'.join(L)); print('row ok')
