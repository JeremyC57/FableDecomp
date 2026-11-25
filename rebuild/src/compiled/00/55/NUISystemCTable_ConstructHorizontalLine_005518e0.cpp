//005518e0 NUISystem::CTable::ConstructHorizontalLine.
// Revisit: inline-depth -- byte-exact, but reached with #pragma inline_depth regions retail most likely never wrote; redo pragma-free once the retail mechanism is found (owner 2026-11-02; docs/pipeline/INLINE_DEPTH_REVISIT.md).
// Layout: generated CTable donor's debug containers contract; retail Children+b0, row lookup+16c.
#include "fable_compiler.h"
#include "engine/CTable.h"
#include <new>
#include <stdlib.h>
// Naming (NF442 gen, 2026-11-25): callee-only rebind of this row's calls to their PDB spellings (T1 tracker; the target
//   self rows' Leaf / PDB name sets), through a bodiless declaration (no layout; rulings r2/r3):
//   0x0041D21B ?CreateComponent@CManager@NUISystem@@QAEPAVCComponent@2@J@Z (was ?CreateComponent@Manager@RetailUI@@QAEPAUComponent@2@K@Z)
namespace NUISystem { class CComponent; }
namespace NUISystem { class CManager { public: static CManager* __fastcall GetInstance();  public: NUISystem::CComponent* CreateComponent(long); }; }   // 0x0041E5F2 (Ego_r.pdb / FableWin.pdb)
// Name fix (agentNF 2026-11-10): the count block's deleter 0x00428B10 is ?DeleteData@?$CCountedPointer@VCComponent@NUISystem@@@@SIXPAX@Z
//   (FableWin.pdb; was ?DestroyComponent@RetailUI@@YIXPAX@Z). ROW DEBT: the row itself and its other callees are still spelled in the
//   invented namespace RetailUI (Pointer = CCountedPointer<NUISystem::CComponent> 0x00429C15, ChildrenVector::Grow = vector::_M_insert_overflow
//   0x005359D0, Table = NUISystem::CTable): needs a full re-author on the shared headers.
namespace NUISystem { class CComponent; }
template <class T> class CCountedPointer { public: static void __fastcall DeleteData(void* data); };   // 0x00428B10 (T = NUISystem::CComponent)
// NF44 (round 6): the RetailUI Vec / Colour / State / StatePair / StateNode / StateIterator / StateMap stand-ins (and their same-TU
// lower_bound / operator[] bodies) replaced by the shared engine/C2DVector.h, engine/NUISystem_CUIState.h and stlport/fable_map.h
// (FABLE_RETAIL_STLP_MAP: the STLport operator[] body visible, kept out of line by VC7.1), so the subscripts bind
// STLport map<unsigned long, NUISystem::CUIState>::operator[] 0x0042D5B1 (the landed row spelling).
#include "engine/C2DVector.h"
#include "engine/NUISystem_CUIState.h"
#define FABLE_RETAIL_STLP_MAP
#include "stlport/fable_map.h"
#define FABLE_RETAIL_STLP_FIND_RANDOM_DISPATCH
#define FABLE_RETAIL_STLP_FIND_NOINLINE_BODY
#include "stlport/fable_algorithm.h"
namespace RetailUI {
typedef ::C2DVector Vec;
typedef ::NUISystem::CUIState State;
typedef std::map<unsigned long, State> StateMap;

struct Component;
struct Info {long References;void (__fastcall* Destroy)(void*);void* Object;Info(void* p):References(1),Destroy(&::CCountedPointer< ::NUISystem::CComponent>::DeleteData),Object(p){}};   // deleter 0x00428B10
struct Pointer {Component* Object;Info* Count;Pointer():Object(0),Count(0){}FABLE_FORCEINLINE Pointer(Component* p):Object(p){Count=p?new Info(p):0;}Pointer(const Pointer& p):Object(p.Object),Count(p.Count){if(Count)++Count->References;}FABLE_FORCEINLINE ~Pointer(){if(Count&&--Count->References==0){Count->Destroy(Count->Object);::operator delete(Count);}Object=0;Count=0;}void Adopt(Component* p){Object=p;Count=p?new Info(p):0;}Component* operator->()const{return Object;}};
struct Tag {Tag(){}};
struct ChildrenVector {Pointer* First;Pointer* Last;Pointer* Limit;void Grow(Pointer*,const Pointer&,const Tag&,unsigned long,bool);FABLE_FORCEINLINE void push_back(const Pointer& p){if(Last!=Limit){new(Last)Pointer(p);++Last;}else Grow(Last,p,Tag(),1,true);}};
// Retail5359d0: nontrivial counted-pointer vector reallocation. It copies values,
// destroys old entries and frees old storage; the incoming value address does not escape.
#pragma auto_inline(off)
void ChildrenVector::Grow(Pointer* position,const Pointer& value,const Tag&,unsigned long count,bool atEnd){
 unsigned long oldSize=Last-First;unsigned long capacity=oldSize+(oldSize>count?oldSize:count);
 Pointer* storage=capacity?(Pointer*)malloc(capacity*sizeof(Pointer)):0;Pointer* out=storage;
 Pointer* p;for(p=First;p!=position;++p,++out)if(out)new(out)Pointer(*p);
 for(unsigned long i=0;i<count;++i,++out)if(out)new(out)Pointer(value);
 if(!atEnd)for(p=position;p!=Last;++p,++out)if(out)new(out)Pointer(*p);
 for(p=First;p!=Last;++p)p->~Pointer();if(First)free(First);First=storage;Last=out;Limit=storage+capacity;
}
#pragma auto_inline(on)
struct Component {
virtual void Slot000();
virtual void Slot001();
virtual void Slot002();
virtual void Slot003();
virtual void Slot004();
virtual void Slot005();
virtual void Slot006();
virtual void Slot007();
virtual void Slot008();
virtual void Slot009();
virtual void Slot010();
virtual void Slot011();
virtual void Slot012();
virtual void Slot013();
virtual void Slot014();
virtual void SetPosition(const Vec&);
virtual void Slot016();
virtual void Slot017();
virtual void Slot018();
virtual void Slot019();
virtual void Slot020();
virtual void Slot021();
virtual void Slot022();
virtual Vec GetSize()const;
virtual void Slot024();
virtual void Slot025();
virtual void Slot026();
virtual void Slot027();
virtual void Slot028();
virtual void Slot029();
virtual void Slot030();
virtual void Slot031();
virtual void Slot032();
virtual void Slot033();
virtual void Slot034();
virtual void Slot035();
virtual void Slot036();
virtual void Slot037();
virtual void Slot038();
virtual void Slot039();
virtual void Slot040();
virtual void Slot041();
virtual void Slot042();
virtual void Initialise();
virtual void Slot044();
virtual void Slot045();
virtual void Slot046();
virtual void Slot047();
virtual void Slot048();
virtual void Slot049();
virtual void Slot050();
virtual void Slot051();
virtual void Slot052();
virtual void Slot053();
virtual void Slot054();
virtual void Slot055();
virtual void Slot056();
virtual void Slot057();
virtual void Slot058();
virtual void Slot059();
virtual void Slot060();
virtual void Slot061();
virtual void Slot062();
virtual void Slot063();
virtual void Slot064();
virtual void Slot065();
virtual void Slot066();
virtual void Slot067();
virtual void Slot068();
virtual void Slot069();
virtual void Slot070();
virtual void Slot071();
virtual void Slot072();
virtual void Slot073();
virtual void Slot074();
virtual void Slot075();
virtual void Slot076();
virtual void Slot077();
virtual void Slot078();
virtual void Slot079();
virtual void Slot080();
virtual void Slot081();
virtual StateMap& GetStateMap();
virtual void Slot083();
virtual void Slot084();
virtual void Slot085();
virtual void Slot086();
virtual void Slot087();
virtual void Slot088();
virtual void Slot089();
virtual void Slot090();
virtual void Slot091();
virtual void Slot092();
virtual void Slot093();
virtual void Slot094();
virtual void Slot095();
virtual void Slot096();
virtual void Slot097();
virtual void Slot098();
virtual void Slot099();
virtual void Slot100();
virtual void Slot101();
virtual void Slot102();
virtual void Slot103();
virtual void Slot104();
virtual void Slot105();
virtual void Slot106();
virtual unsigned long GetName()const;
};
struct Manager {Component* CreateComponent(unsigned long);}; //0041d21b; the instance is NUISystem::CManager::GetInstance() 0x0041e5f2
struct Lookup {unsigned long* First;unsigned long* Last;unsigned long* Limit;};   // vector<unsigned long> (binders 0x00531090 / 0x00531560 / 0x00533BC0)
// 005348e0 is STLport std::__find<unsigned long*, unsigned long> (random-access overload; stlport/fable_algorithm.h declaration).
struct Table {unsigned char BeforeChildren[0xb0];ChildrenVector Children;unsigned char BeforeLookup[0x16c-0xbc];Lookup* Entries;void ConstructHorizontalLine(Pointer,Pointer,const Vec&,unsigned long,long);};
void Table::ConstructHorizontalLine(Pointer start,Pointer element,const Vec& position,unsigned long count,long offset){
 Vec at=position;Pointer line(reinterpret_cast<RetailUI::Component*>(reinterpret_cast<NUISystem::CManager*>(((Manager*)NUISystem::CManager::GetInstance()))->CreateComponent(start->GetName())));{float extent=(float)count;line->GetStateMap()[0UL].Zoom.X=extent;line->Initialise();line->SetPosition(at);line->GetStateMap()[0UL].Position=at;Children.push_back(line);at.X+=start->GetSize().X;}
 for(unsigned long i=0;i<count;++i){unsigned long key=i-offset;if(element.Object&&std::__find(Entries->First,Entries->Last,key,std::random_access_iterator_tag())!=Entries->Last){
Component* created=reinterpret_cast<RetailUI::Component*>(reinterpret_cast<NUISystem::CManager*>(((Manager*)NUISystem::CManager::GetInstance()))->CreateComponent(element->GetName()));
#pragma inline_depth(0)
Pointer child(created);
#pragma inline_depth()
child->Initialise();child->SetPosition(at);child->GetStateMap()[0UL].Position=at;Children.push_back(child);at.X+=element->GetSize().X;}}
}
}