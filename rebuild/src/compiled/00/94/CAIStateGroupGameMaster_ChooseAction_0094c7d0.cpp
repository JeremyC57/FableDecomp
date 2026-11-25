// CAIStateGroup_GameMaster::ChooseAction() -- calls the member method @ 0x0094A3B0 with the constant 0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0094C7D0, oracle length 8 bytes.
// Grade: candidate -- generated for a repeated body shape and graded in the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); the batch README records the lane verdict.
//
// Retail bytes:  push 0 ; call 0x94a3b0 ; ret 
// Tiny-body shape `CallWith` synthesised from the instruction pattern (the class comes from the
// manifest label when it is a plain class name, else the method is landed under a descriptive
// placeholder); member names are descriptive. 
struct CAIStateGroup_GameMaster {
    void ChooseAction();
};

// Naming (NF443 gen, 2026-11-25): callee-only rebind of this row's calls to their PDB spellings (T1 tracker; the target
//   self rows' Leaf / PDB name sets), through a bodiless declaration or the row's own model (no layout; rulings r2/r3, NF438 q1 friend):
//   0x0094A3B0 ?ChooseAction@CAIStateGroup_GoToPub@@IAE_N_N@Z (was ?Method@CAIStateGroup_GameMaster@@QAEXJ@Z)
struct CAIStateGroup_GameMaster;
class CAIStateGroup_GoToPub { public: protected: bool ChooseAction(bool); public: friend struct ::CAIStateGroup_GameMaster; };
void CAIStateGroup_GameMaster::ChooseAction()
{
    reinterpret_cast<CAIStateGroup_GoToPub*>(this)->ChooseAction(0);
}