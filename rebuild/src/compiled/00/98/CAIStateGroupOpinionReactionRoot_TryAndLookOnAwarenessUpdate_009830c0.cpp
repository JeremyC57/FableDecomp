// CAIStateGroup_OpinionReactionRoot::TryAndLookOnAwarenessUpdate() -- one look-at-position step towards the thing reacted to.
// Leaf: ?TryAndLookOnAwarenessUpdate@CAIStateGroup_OpinionReactionRoot@@QAE_NXZ
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009830C0, length 67 bytes.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (ThingReactingTo at +0x6b), CThing::GetPosition
// @ 0x004C73D0 (manifest "CVertexBufferWin32::DoSizeof" is propagated), CAIStateGroupBase::AIAction_LookAtPos
// (EAIActionRepeat, C3DVector, long, float) @ 0x0097D580 (repeat 1, flags 0x21, turn speed 0.0027777f).
// Shape: the thing is fetched into a named local first (the bare expression calls GetPItem after the pushes), the two literals pushed, then the by-value C3DVector (user copy constructor)
// is built in its argument slot (`sub esp,0Ch ; mov ecx,esp` + three dword moves) from the returned position,
// then the repeat mode. `xor ecx,ecx ; cmp eax,2 ; sete cl ; mov al,cl` is a plain `return r == 2;`.
// The pointer member is unaligned at +0x6b (engine header, pack 1), so the class is modelled packed.
// Name fix (agentNF5 2026-11-10): CAIStateGroupBase::AIAction_LookAtPos 0x0097D580 is protected, EAIActionReturn(EAIActionRepeat, C3DVector, long, float) (?AIAction_LookAtPos@CAIStateGroupBase@@IAE?AW4EAIActionReturn@@W4EAIActionRepeat@@VC3DVector@@JM@Z, Ego_r.pdb / FableWin.pdb).
enum EAIActionRepeat;                         // MS forward enum
enum EAIActionReturn;                         // MS forward enum
class CThing;

#include "engine/C3DVector.h"

#include "engine/CThing.h"
class CThingPos : public CThing { public:
};

class CBaseObject;
class CBaseObjectPointer;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"


class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();
    unsigned char _pad[0x10];                   // +0x4..+0x13
    protected:
    EAIActionReturn AIAction_LookAtPos(EAIActionRepeat repeat, C3DVector pos, long flags, float turnSpeed);   // 0x0097D580
    public:
};

class CTCTalk;

#pragma pack(push, 1)
class CAIStateGroup_OpinionReactionRoot : public CAIStateGroupBase { public:
    CTCTalk* PTCTalk;                           // +0x14
    long CurrentReactionType;                   // +0x18
    bool ReactionFlags[79];                     // +0x1c
    CIntelligentPointer<CThingPos> ThingReactingTo;    // +0x6b CIntelligentPointer<CThing>
    bool TryAndLookOnAwarenessUpdate();
};
#pragma pack(pop)

bool CAIStateGroup_OpinionReactionRoot::TryAndLookOnAwarenessUpdate()
{
    CThingPos* pThing = ThingReactingTo.Get();   // fetched before the literal pushes (a named local)
    return AIAction_LookAtPos((EAIActionRepeat)(1), pThing->GetPos(), 0x21, 0.0027777f) == 2;
}