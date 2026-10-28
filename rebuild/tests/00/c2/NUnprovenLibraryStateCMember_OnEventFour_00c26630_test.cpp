#include <stdio.h>
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
namespace NUnprovenLibraryState{static CMember* expected;static CListener* expectedListener;static int calls,bad;void CListener::Complete(CMember* p){++calls;if(p!=expected||p->State!=2||this!=expectedListener)bad=1;}void CMember::OnEventOne(CContext*){}}using namespace NUnprovenLibraryState;int main(){CMember member;CContext context;expected=&member;expectedListener=&context.Listener;bool ok=true;unsigned short values[]={0,1,2,65535};for(int i=0;i<4;++i){member.State=values[i];calls=0;member.OnEventFour(&context);ok=ok&&!bad&&calls==(values[i]==1?1:0)&&member.State==(values[i]==1?2:values[i]);}puts(ok?"00c26630_TEST PASS":"00c26630_TEST FAIL");return ok?0:1;}