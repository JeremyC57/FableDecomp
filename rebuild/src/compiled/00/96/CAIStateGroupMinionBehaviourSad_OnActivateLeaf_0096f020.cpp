// CAIStateGroup_MinionBehaviourSad::OnActivateLeaf() -- calls the method @ 0x0073AE10 of the object at +0x24 with the constant 20
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0096F020, oracle length 11 bytes.
// Grade: candidate -- generated for a repeated body shape and graded in the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); the batch README records the lane verdict.
//
// Retail bytes:  mov ecx, dword ptr [ecx + 0x24] ; push 0x14 ; call 0x73ae10 ; ret 
// Tiny-body shape `ForwardConstant` synthesised from the instruction pattern (the class comes from the
// manifest label when it is a plain class name, else the method is landed under a descriptive
// placeholder); member names are descriptive. 
class CTCCreatureModeManager { public:
public:
    bool AddMode(unsigned long);   // 0x0073AE10
};

struct CAIStateGroup_MinionBehaviourSad {
    unsigned char _pad[0x24];
    CTCCreatureModeManager* m_pObject;                    // +0x24
    void OnActivateLeaf();
};

void CAIStateGroup_MinionBehaviourSad::OnActivateLeaf()
{
    m_pObject->AddMode(20);
}