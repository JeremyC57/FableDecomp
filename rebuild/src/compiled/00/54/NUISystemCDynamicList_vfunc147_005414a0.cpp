// 005414a0 NUISystem::CDynamicList::InsertChild recovered PDB005baf00.
// Retail field layout inherited from Update00541020: initialized15c, selection16c, vector174.
#include <new>
#include <stdlib.h>
namespace NUISystem {
struct CComponent {void AddChild(struct CountedPointer&); //00533b50

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
struct CChangingStateComponent:CComponent {void AddChildDynamically(CountedPointer&);}; //0052cab0

struct CDynamicList:CChangingStateComponent {
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