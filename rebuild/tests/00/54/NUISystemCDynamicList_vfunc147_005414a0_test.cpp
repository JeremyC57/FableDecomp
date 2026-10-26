// BEGIN exact source snapshot SHA256 de8b4dc7ba4891a0feb4e3ef8574555daf67749f76eb2e2580c4a8f790f51de3
// 005414a0 NUISystem::CDynamicList::InsertChild recovered PDB005baf00.
// Retail field layout inherited from Update00541020: initialized15c, selection16c, vector174.
#include <new>
#include <stdlib.h>
namespace NUISystem {
class CComponent { public:void AddChild(struct CountedPointer&); //00533b50

virtual void Slot000()=0;
virtual void Slot001()=0;
virtual void Slot002()=0;
virtual void Slot003()=0;
virtual void Slot004()=0;
virtual void Slot005()=0;
virtual void Slot006()=0;
virtual void Slot007()=0;
virtual void Slot008()=0;
virtual void Slot009()=0;
virtual void Slot010()=0;
virtual void Slot011()=0;
virtual void Slot012()=0;
virtual void Slot013()=0;
virtual void Slot014()=0;
virtual void Slot015()=0;
virtual void Slot016()=0;
virtual void Slot017()=0;
virtual void Slot018()=0;
virtual void Slot019()=0;
virtual void Slot020()=0;
virtual void Slot021()=0;
virtual void Slot022()=0;
virtual void Slot023()=0;
virtual void Slot024()=0;
virtual void Slot025()=0;
virtual void Slot026()=0;
virtual void Slot027()=0;
virtual void Slot028()=0;
virtual void Slot029()=0;
virtual void Slot030()=0;
virtual void Slot031()=0;
virtual void Slot032()=0;
virtual void Slot033()=0;
virtual void Slot034()=0;
virtual void Slot035()=0;
virtual void Slot036()=0;
virtual void Slot037()=0;
virtual void Slot038()=0;
virtual void Slot039()=0;
virtual void Slot040()=0;
virtual void Slot041()=0;
virtual void Slot042()=0;
virtual void Slot043()=0;
virtual void Slot044()=0;
virtual void Slot045()=0;
virtual void Slot046()=0;
virtual void Slot047()=0;
virtual void ChangeState(unsigned long)=0;
virtual bool IsActive()const=0;
virtual void Slot050()=0;
virtual void Slot051()=0;
virtual void Slot052()=0;
virtual void Slot053()=0;
virtual void Slot054()=0;
virtual void Slot055()=0;
virtual void Slot056()=0;
virtual void Slot057()=0;
virtual void Slot058()=0;
virtual void Slot059()=0;
virtual void Slot060()=0;
virtual void Slot061()=0;
virtual void Slot062()=0;
virtual void Slot063()=0;
virtual void Slot064()=0;
virtual void Slot065()=0;
virtual void Slot066()=0;
virtual void Slot067()=0;
virtual void Slot068()=0;
virtual void Slot069()=0;
virtual void Slot070()=0;
virtual void Slot071()=0;
virtual void Slot072()=0;
virtual void Slot073()=0;
virtual void Slot074()=0;
virtual void Slot075()=0;
virtual void Slot076()=0;
virtual void Slot077()=0;
virtual void Slot078()=0;
virtual void Slot079()=0;
virtual void Slot080()=0;
virtual void Slot081()=0;
virtual void Slot082()=0;
virtual void Slot083()=0;
virtual void Slot084()=0;
virtual void Slot085()=0;
virtual unsigned long GetActiveState()const=0;
virtual void Slot087()=0;
virtual void Slot088()=0;
virtual void Slot089()=0;
virtual void Slot090()=0;
virtual void Slot091()=0;
virtual void Slot092()=0;
virtual void Slot093()=0;
virtual void Slot094()=0;
virtual void Slot095()=0;
virtual void Slot096()=0;
virtual void Slot097()=0;
virtual void Slot098()=0;
virtual void Slot099()=0;
virtual void Slot100()=0;
virtual void Slot101()=0;
virtual void Slot102()=0;
virtual void Slot103()=0;
virtual void Slot104()=0;
virtual void Slot105()=0;
virtual void Slot106()=0;
virtual void Slot107()=0;
virtual void Slot108()=0;
virtual void Slot109()=0;
virtual void Slot110()=0;
virtual void Slot111()=0;
virtual void Slot112()=0;
virtual void Slot113()=0;
virtual void Slot114()=0;
virtual void Slot115()=0;
virtual void Slot116()=0;
virtual void Slot117()=0;
virtual void Slot118()=0;
virtual void Slot119()=0;
virtual void Slot120()=0;
virtual void Slot121()=0;
virtual void Slot122()=0;
virtual void Slot123()=0;
virtual void Slot124()=0;
virtual void Slot125()=0;
virtual void Slot126()=0;
virtual void Slot127()=0;
virtual void Slot128()=0;
virtual void Slot129()=0;
virtual void Slot130()=0;
virtual void Slot131()=0;
virtual void Slot132()=0;
virtual void Slot133()=0;
virtual void Slot134()=0;
virtual void Slot135()=0;
virtual void Slot136()=0;
virtual void Slot137()=0;
virtual void Slot138()=0;
virtual void Slot139()=0;
virtual void Slot140()=0;
virtual void Slot141()=0;
virtual void ResetPositions()=0;
};
struct CountedPointer {CComponent* Object;void* Count;CComponent* GetObject()const{return Object;}};
struct EmptyTag{EmptyTag(){}};
struct ChildVector {CComponent** Start;CComponent** Finish;CComponent** Limit;
 unsigned int size()const{return Finish-Start;}CComponent** begin()const{return Start;}
 void Insert(CComponent**,CComponent*const&); //005418c0
 void Grow(CComponent**,CComponent*const&,const EmptyTag&,unsigned int,bool); //00485dbc
 void push_back(CComponent* const& value){if(Finish!=Limit){new(Finish) CComponent*(value);++Finish;}else Grow(Finish,value,EmptyTag(),1,true);}
};
// 00485dbc is the scalar-pointer vector overflow; bfea0e imports malloc.
// Values are copied into new storage and the const-reference is not retained.
#pragma auto_inline(off)
void ChildVector::Grow(CComponent** position,CComponent*const& value,const EmptyTag&,unsigned int count,bool atEnd){unsigned int oldSize=Finish-Start;unsigned int capacity=oldSize+(oldSize>count?oldSize:count);CComponent** storage=capacity?(CComponent**)malloc(capacity*sizeof(CComponent*)):0;CComponent** out=storage;CComponent** p;for(p=Start;p!=position;++p,++out)*out=*p;for(unsigned int i=0;i<count;++i,++out)*out=value;if(!atEnd)for(p=position;p!=Finish;++p,++out)*out=*p;if(Start)free(Start);Start=storage;Finish=out;Limit=storage+capacity;}
#pragma auto_inline(on)
class CChangingStateComponent:public CComponent { public:void AddChildDynamically(CountedPointer&);}; //0052cab0

class CDynamicList:public CChangingStateComponent { public:
 unsigned char BeforeInit[0x15c-4];int Initialised;unsigned char BeforeTop[8];int TopIndex,SelectionIndex,NumVisible;ChildVector Children;
 void InsertChild(unsigned long,CountedPointer&);
};
void CDynamicList::InsertChild(unsigned long index,CountedPointer& child){
 if(Initialised)CChangingStateComponent::AddChildDynamically(child);else CComponent::AddChild(child);
 if(index<Children.size()){Children.Insert(Children.begin()+index,child.GetObject());}
 else {index=Children.size();Children.push_back(child.GetObject());}
 if(Initialised){
 if(Children.size()==1){TopIndex=0;SelectionIndex=0;if(IsActive()&&GetActiveState()==1)Children.Start[SelectionIndex]->ChangeState(3);}
 else if(index<=(unsigned)SelectionIndex&&(unsigned)SelectionIndex<Children.size()-1){++SelectionIndex;if(SelectionIndex>=TopIndex+NumVisible)TopIndex=SelectionIndex-NumVisible+1;}
 ResetPositions();
 }
}
}

// END exact source snapshot
#include <stdio.h>
using namespace NUISystem;static int initializedAdds,plainAdds,resets,changes;
void CChangingStateComponent::AddChildDynamically(CountedPointer&){++initializedAdds;}void CComponent::AddChild(CountedPointer&){++plainAdds;}
void ChildVector::Insert(CComponent** p,CComponent*const& v){for(CComponent** q=Finish;q!=p;--q)*q=*(q-1);*p=v;++Finish;}

struct Test:CDynamicList{
void Slot000(){}
void Slot001(){}
void Slot002(){}
void Slot003(){}
void Slot004(){}
void Slot005(){}
void Slot006(){}
void Slot007(){}
void Slot008(){}
void Slot009(){}
void Slot010(){}
void Slot011(){}
void Slot012(){}
void Slot013(){}
void Slot014(){}
void Slot015(){}
void Slot016(){}
void Slot017(){}
void Slot018(){}
void Slot019(){}
void Slot020(){}
void Slot021(){}
void Slot022(){}
void Slot023(){}
void Slot024(){}
void Slot025(){}
void Slot026(){}
void Slot027(){}
void Slot028(){}
void Slot029(){}
void Slot030(){}
void Slot031(){}
void Slot032(){}
void Slot033(){}
void Slot034(){}
void Slot035(){}
void Slot036(){}
void Slot037(){}
void Slot038(){}
void Slot039(){}
void Slot040(){}
void Slot041(){}
void Slot042(){}
void Slot043(){}
void Slot044(){}
void Slot045(){}
void Slot046(){}
void Slot047(){}
void Slot050(){}
void Slot051(){}
void Slot052(){}
void Slot053(){}
void Slot054(){}
void Slot055(){}
void Slot056(){}
void Slot057(){}
void Slot058(){}
void Slot059(){}
void Slot060(){}
void Slot061(){}
void Slot062(){}
void Slot063(){}
void Slot064(){}
void Slot065(){}
void Slot066(){}
void Slot067(){}
void Slot068(){}
void Slot069(){}
void Slot070(){}
void Slot071(){}
void Slot072(){}
void Slot073(){}
void Slot074(){}
void Slot075(){}
void Slot076(){}
void Slot077(){}
void Slot078(){}
void Slot079(){}
void Slot080(){}
void Slot081(){}
void Slot082(){}
void Slot083(){}
void Slot084(){}
void Slot085(){}
void Slot087(){}
void Slot088(){}
void Slot089(){}
void Slot090(){}
void Slot091(){}
void Slot092(){}
void Slot093(){}
void Slot094(){}
void Slot095(){}
void Slot096(){}
void Slot097(){}
void Slot098(){}
void Slot099(){}
void Slot100(){}
void Slot101(){}
void Slot102(){}
void Slot103(){}
void Slot104(){}
void Slot105(){}
void Slot106(){}
void Slot107(){}
void Slot108(){}
void Slot109(){}
void Slot110(){}
void Slot111(){}
void Slot112(){}
void Slot113(){}
void Slot114(){}
void Slot115(){}
void Slot116(){}
void Slot117(){}
void Slot118(){}
void Slot119(){}
void Slot120(){}
void Slot121(){}
void Slot122(){}
void Slot123(){}
void Slot124(){}
void Slot125(){}
void Slot126(){}
void Slot127(){}
void Slot128(){}
void Slot129(){}
void Slot130(){}
void Slot131(){}
void Slot132(){}
void Slot133(){}
void Slot134(){}
void Slot135(){}
void Slot136(){}
void Slot137(){}
void Slot138(){}
void Slot139(){}
void Slot140(){}
void Slot141(){}
void ChangeState(unsigned long s){if(s==3)++changes;}bool IsActive()const{return true;}unsigned long GetActiveState()const{return 1;}void ResetPositions(){++resets;}};
int main(){Test a,b;CComponent* store[8];a.Children.Start=a.Children.Finish=store;a.Children.Limit=store+8;a.Initialised=1;a.TopIndex=0;a.SelectionIndex=-1;a.NumVisible=1;CountedPointer child;child.Object=&b;child.Count=0;
 a.InsertChild(9,child);if(a.SelectionIndex!=0||changes!=1||resets!=1||initializedAdds!=1)return 1;
 a.InsertChild(0,child);if(a.SelectionIndex!=1||a.TopIndex!=1||a.Children.size()!=2||resets!=2)return 2;
 a.Initialised=0;a.InsertChild(99,child);if(plainAdds!=1||resets!=2||a.Children.size()!=3)return 3;
 Test grown;grown.Initialised=0;grown.Children.Start=grown.Children.Finish=grown.Children.Limit=0;grown.InsertChild(0,child);if(grown.Children.size()!=1||grown.Children.Start[0]!=&b)return 4;free(grown.Children.Start);puts("005414a0_TEST PASS");return 0;}