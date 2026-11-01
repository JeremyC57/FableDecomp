// CClass_006b8410::GetTarget_006b8410() -- forwards to NScreenEffect::CScreenEffectPriorityStack::Clear() on the member at +0x90 (sibling jump)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006B8410, oracle length 11 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  add ecx,0x90 ; jmp CBaseIntelligentPointer::Method_00881210
// Virtual getter that forwards to Method_00881210 @ 0x00881210 on the embedded smart pointer at
// +0x90: `this` is advanced to the member and the call becomes a sibling jump (same
// signature, no locals). The class comes from the vtable-slot label. One of 15 bodies.
class CBaseObject;

namespace NScreenEffect {
class CScreenEffectPriorityStack {                // Ego_r/FableWin (naming wave 17, c677: was CBaseIntelligentPointer::Method_00881210)
public:
    void Clear();                                 // 0x00881210 (?Clear@CScreenEffectPriorityStack@NScreenEffect@@QAEXXZ)
};
}

struct CClass_006b8410 {
    virtual void v00();
    unsigned char _pad[0x8c];
    NScreenEffect::CScreenEffectPriorityStack m_screenEffectStack;   // +0x90
    virtual void GetTarget_006b8410();
};

void CClass_006b8410::GetTarget_006b8410()
{
    m_screenEffectStack.Clear();
}