#include <stdio.h>
// NUnprovenLibraryCallbacks::CGroup::VisitTwo(...) -- Dispatch the callback while preserving the saved next link.
// Leaf: ?VisitTwo@CGroup@NUnprovenLibraryCallbacks@@UAEXPAX0@Z
// Identity: descriptive. Own table012b40a4 slots1..4, actual constructor00c29f50 sets index4=-1 and First8=null; Add00c29ec0 stores member.Group48 and Next52. These original libpk names are not covered by PDB; existing catalog CSteeringGroup/CSteeringAgent are descriptive and collide with unrelated proven engine CSteeringAgent. Isolated descriptive namespace avoids claiming that identity. Arguments are opaque native-width ABI words projected as pointers. Retail deliberately reloads First for each callback while traversal advances through saved Next; this behavior is retained, including head changes during callbacks.
// Layout: group vptr0, Index4, First8; member vptr0, unproven44-byte field range4..47, Group48, Next52. No engine header fits this library identity; unused virtual slot0 is only an ABI projection.
namespace NUnprovenLibraryCallbacks{
class CGroup;
class CMember{public:virtual void UnknownSlot0(){}virtual void VisitOne(void*)=0;virtual void VisitTwo(void*,void*)=0;virtual void VisitThree(void*,void*)=0;virtual void VisitFour(void*)=0;unsigned char UnprovenFields04[44];CGroup* Group;CMember* Next;};
class CGroup{public:virtual long* GetIndexSlot(){return &Index;}virtual void VisitOne(void*);virtual void VisitTwo(void*,void*);virtual void VisitThree(void*,void*);virtual void VisitFour(void*);long Index;CMember* First;};
}
void NUnprovenLibraryCallbacks::CGroup::VisitTwo(void* first,void* second){for(CMember* p=First;p;){CMember* next=p->Next;First->VisitTwo(first,second);p=next;}}
void NUnprovenLibraryCallbacks::CGroup::VisitOne(void*){}
void NUnprovenLibraryCallbacks::CGroup::VisitFour(void*){}
void NUnprovenLibraryCallbacks::CGroup::VisitThree(void*,void*){}
using namespace NUnprovenLibraryCallbacks;static void* expected1;static void* expected2;static int count,bad,order[8],mode;static CGroup* group;static CMember* replacement;class TestMember:public CMember{public:int id;TestMember(int i):id(i){Next=0;Group=0;}void Record(int slot,void* a,void* b){order[count++]=id;if(a!=expected1||b!=expected2||slot!=2)bad=1;if(mode==1)Next=0;if(mode==2&&count==1)group->First=replacement;}virtual void VisitOne(void* a){Record(1,a,0);}virtual void VisitTwo(void* a,void* b){Record(2,a,b);}virtual void VisitThree(void* a,void* b){Record(3,a,b);}virtual void VisitFour(void* a){Record(4,a,0);}};
int main(){int x=1,y=2;expected1=&x;expected2=&y;CGroup g;group=&g;g.First=0;g.VisitTwo(&x,&y);bool ok=count==0;TestMember a(1),b(2),c(3);for(mode=0;mode<3;++mode){a.Next=&b;b.Next=&c;c.Next=0;g.First=&a;replacement=&b;count=bad=0;g.VisitTwo(&x,&y);ok=ok&&!bad&&count==3&&order[0]==1;ok=ok&&order[1]==(mode==2?2:1)&&order[2]==(mode==2?2:1);}puts(ok?"00c29d30_TEST PASS":"00c29d30_TEST FAIL");return ok?0:1;}