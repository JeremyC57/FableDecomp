// NUnprovenLibraryState::CMember::OnEventFour(CContext*) -- Complete a member only when its state is one.
// Leaf: ?OnEventFour@CMember@NUnprovenLibraryState@@UAEXPAVCContext@2@@Z
// Identity: descriptive. Own shared virtual table012b3fdc slots1/4, helper00c265a0 compares ushort State116==1; helper00c26580 sets2 and calls context+44 interface slot8 with original receiver. No PDB covers this library. Names and event signatures remain descriptive ABI projections; original event meanings and unused fields unproven. Visible real helper bodies preserve ECX across the state check, not an invented calling convention.
// Layout: original member vptr0, opaque4..115, State116; context interface44. No applicable engine header. Unused interface slots are explicit projections.
namespace NUnprovenLibraryState{
class CMember;class CContext;
class CMember{public:virtual void UnknownSlot0(){}virtual void OnEventOne(CContext*);virtual void UnknownSlot2(){}virtual void UnknownSlot3(){}virtual void OnEventFour(CContext*);unsigned char UnprovenFields04[112];unsigned short State;int IsStateOne();void Complete(CContext*);};
class CListener{public:virtual void UnknownSlot0(){}virtual void UnknownSlot1(){}virtual void UnknownSlot2(){}virtual void UnknownSlot3(){}virtual void UnknownSlot4(){}virtual void UnknownSlot5(){}virtual void UnknownSlot6(){}virtual void UnknownSlot7(){} virtual void Complete(CMember*);};class CContext{public:unsigned char UnprovenPrefix00[44];CListener Listener;};
#pragma auto_inline(off)
int CMember::IsStateOne(){return State==1;}
void CMember::Complete(CContext* context){State=2;context->Listener.Complete(this);}
#pragma auto_inline(on)
}
void NUnprovenLibraryState::CMember::OnEventFour(CContext* context){if(IsStateOne())Complete(context);}