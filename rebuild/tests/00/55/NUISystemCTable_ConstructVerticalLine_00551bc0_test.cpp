// BEGIN exact source snapshot SHA256 c5cc018963295687633f28e186024ab20cec3469650b61185a6bb12fbc222292
//00551bc0 NUISystem::CTable::ConstructVerticalLine.
// Layout: generated CTable donor's debug containers contract; retail Children+b0, column lookup+168.
#include "engine/CTable.h"
#include <new>
#include <stdlib.h>
namespace RetailUI {
struct Vec {float X,Y;Vec(){}Vec(float x,float y):X(x),Y(y){}Vec(const Vec& v):X(v.X),Y(v.Y){}};
// CUIState PDB layout is 36 bytes. Retail 52e5d0 initializes the fields below;
// 429e29 copies colour as four bytes. The map key is unsigned long, not a component pointer.
struct Colour {unsigned char R,G,B,A;Colour():R(255),G(255),B(255),A(255){}Colour(const Colour& c):R(c.R),G(c.G),B(c.B),A(c.A){}};
struct State {unsigned long Flags,ChangeType;Vec Position,Zoom;Colour ColourValue;float UpdateTime;bool LinearChange;
 State():Flags(7),ChangeType(0),Position(0,0),Zoom(1,1),UpdateTime(-1),LinearChange(false){}
 State(const State& s):Flags(s.Flags),ChangeType(s.ChangeType),Position(s.Position),Zoom(s.Zoom),ColourValue(s.ColourValue),UpdateTime(s.UpdateTime),LinearChange(s.LinearChange){}
};
struct StatePair {unsigned long Key;State Value;StatePair():Key(0){}StatePair(const unsigned long& k,const State& s):Key(k),Value(s){}};
struct StateNode {long Colour;StateNode* Parent;StateNode* Left;StateNode* Right;StatePair Pair;};
struct StateIterator {StateNode* Node;explicit StateIterator(StateNode* n):Node(n){}StateIterator(const StateIterator& i):Node(i.Node){}StatePair& operator*()const{return Node->Pair;}bool operator==(const StateIterator& i)const{return Node==i.Node;}};
struct StateMap {StateNode* Header;unsigned long Count;StateNode* LowerBoundNode(const unsigned long&);StateIterator InsertUnique(StateIterator,const StatePair&);State& operator[](const unsigned long&);};
// Faithful dependency bodies, 42ad6c and 42d5b1: neither retains the caller's key address.
#pragma auto_inline(off)
StateNode* StateMap::LowerBoundNode(const unsigned long& key){StateNode* candidate=Header;StateNode* node=Header->Parent;while(node){if(node->Pair.Key>=key){candidate=node;node=node->Left;}else node=node->Right;}return candidate;}
State& StateMap::operator[](const unsigned long& key){StateIterator i(LowerBoundNode(key));if(i==StateIterator(Header)||key<(*i).Key){State value;i=InsertUnique(i,StatePair(key,value));}return (*i).Value;}
#pragma auto_inline(on)

struct Component;
void __fastcall DestroyComponent(void*); //00428b10
struct Info {long References;void (__fastcall* Destroy)(void*);void* Object;Info(void* p):References(1),Destroy(DestroyComponent),Object(p){}};
struct Pointer {Component* Object;Info* Count;Pointer():Object(0),Count(0){}__forceinline Pointer(Component* p):Object(p){Count=p?new Info(p):0;}Pointer(const Pointer& p):Object(p.Object),Count(p.Count){if(Count)++Count->References;}__forceinline ~Pointer(){if(Count&&--Count->References==0){Count->Destroy(Count->Object);::operator delete(Count);}Object=0;Count=0;}void Adopt(Component* p){Object=p;Count=p?new Info(p):0;}Component* operator->()const{return Object;}};
struct Tag {Tag(){}};
struct ChildrenVector {Pointer* First;Pointer* Last;Pointer* Limit;void Grow(Pointer*,const Pointer&,const Tag&,unsigned long,bool);__forceinline void push_back(const Pointer& p){if(Last!=Limit){new(Last)Pointer(p);++Last;}else Grow(Last,p,Tag(),1,true);}};
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
struct Manager {static Manager* __fastcall GetInstance();Component* CreateComponent(unsigned long);}; //0041e5f2,0041d21b
struct Lookup {long* First;long* Last;long* Limit;};
// 005348e0 is std::find over four-byte scalar entries, not a pair/map search.
#pragma auto_inline(off)
long* __fastcall Find(long* first,long* last,const long& key,const Tag&){for(;first!=last;++first)if(*first==key)return first;return last;}
#pragma auto_inline(on)
struct Table {unsigned char BeforeChildren[0xb0];ChildrenVector Children;unsigned char BeforeLookup[0x168-0xbc];Lookup* Entries;void ConstructVerticalLine(Pointer,Pointer,const Vec&,unsigned long,long);};
void Table::ConstructVerticalLine(Pointer start,Pointer element,const Vec& position,unsigned long count,long offset){
 Vec at=position;Pointer line(Manager::GetInstance()->CreateComponent(start->GetName()));{float extent=(float)count;line->GetStateMap()[0UL].Zoom.Y=extent;line->Initialise();line->SetPosition(at);line->GetStateMap()[0UL].Position=at;Children.push_back(line);at.Y+=start->GetSize().Y;}
 for(unsigned long i=0;i<count;++i){long key=(long)i-offset;if(element.Object&&Find(Entries->First,Entries->Last,key,Tag())!=Entries->Last){
Component* created=Manager::GetInstance()->CreateComponent(element->GetName());
#pragma inline_depth(0)
Pointer child(created);
#pragma inline_depth()
child->Initialise();child->SetPosition(at);child->GetStateMap()[0UL].Position=at;Children.push_back(child);at.Y+=element->GetSize().Y;}}
}
}

// END exact source snapshot
#include <stdio.h>
#include <string.h>
using namespace RetailUI;
static StateMap maps[8];static StateNode headers[8],nodes[8];static Component objects[8];static int created,initialised,destroyed;static Vec positions[8];
void __fastcall RetailUI::DestroyComponent(void*){++destroyed;}Manager* __fastcall Manager::GetInstance(){static Manager m;return &m;}Component* Manager::CreateComponent(unsigned long){return &objects[created++];}
StateIterator StateMap::InsertUnique(StateIterator,const StatePair& value){StateNode* n=new StateNode;n->Pair=value;n->Parent=Header;n->Left=n->Right=0;Header->Parent=n;++Count;return StateIterator(n);}
void Component::Slot000(){}
void Component::Slot001(){}
void Component::Slot002(){}
void Component::Slot003(){}
void Component::Slot004(){}
void Component::Slot005(){}
void Component::Slot006(){}
void Component::Slot007(){}
void Component::Slot008(){}
void Component::Slot009(){}
void Component::Slot010(){}
void Component::Slot011(){}
void Component::Slot012(){}
void Component::Slot013(){}
void Component::Slot014(){}
void Component::Slot016(){}
void Component::Slot017(){}
void Component::Slot018(){}
void Component::Slot019(){}
void Component::Slot020(){}
void Component::Slot021(){}
void Component::Slot022(){}
void Component::Slot024(){}
void Component::Slot025(){}
void Component::Slot026(){}
void Component::Slot027(){}
void Component::Slot028(){}
void Component::Slot029(){}
void Component::Slot030(){}
void Component::Slot031(){}
void Component::Slot032(){}
void Component::Slot033(){}
void Component::Slot034(){}
void Component::Slot035(){}
void Component::Slot036(){}
void Component::Slot037(){}
void Component::Slot038(){}
void Component::Slot039(){}
void Component::Slot040(){}
void Component::Slot041(){}
void Component::Slot042(){}
void Component::Slot044(){}
void Component::Slot045(){}
void Component::Slot046(){}
void Component::Slot047(){}
void Component::Slot048(){}
void Component::Slot049(){}
void Component::Slot050(){}
void Component::Slot051(){}
void Component::Slot052(){}
void Component::Slot053(){}
void Component::Slot054(){}
void Component::Slot055(){}
void Component::Slot056(){}
void Component::Slot057(){}
void Component::Slot058(){}
void Component::Slot059(){}
void Component::Slot060(){}
void Component::Slot061(){}
void Component::Slot062(){}
void Component::Slot063(){}
void Component::Slot064(){}
void Component::Slot065(){}
void Component::Slot066(){}
void Component::Slot067(){}
void Component::Slot068(){}
void Component::Slot069(){}
void Component::Slot070(){}
void Component::Slot071(){}
void Component::Slot072(){}
void Component::Slot073(){}
void Component::Slot074(){}
void Component::Slot075(){}
void Component::Slot076(){}
void Component::Slot077(){}
void Component::Slot078(){}
void Component::Slot079(){}
void Component::Slot080(){}
void Component::Slot081(){}
void Component::Slot083(){}
void Component::Slot084(){}
void Component::Slot085(){}
void Component::Slot086(){}
void Component::Slot087(){}
void Component::Slot088(){}
void Component::Slot089(){}
void Component::Slot090(){}
void Component::Slot091(){}
void Component::Slot092(){}
void Component::Slot093(){}
void Component::Slot094(){}
void Component::Slot095(){}
void Component::Slot096(){}
void Component::Slot097(){}
void Component::Slot098(){}
void Component::Slot099(){}
void Component::Slot100(){}
void Component::Slot101(){}
void Component::Slot102(){}
void Component::Slot103(){}
void Component::Slot104(){}
void Component::Slot105(){}
void Component::Slot106(){}

void Component::SetPosition(const Vec& p){positions[this-objects]=p;}Vec Component::GetSize()const{return Vec(3,4);}void Component::Initialise(){++initialised;}StateMap& Component::GetStateMap(){return maps[this-objects];}unsigned long Component::GetName()const{return 7;}

int main(){for(int j=0;j<8;++j){maps[j].Header=&headers[j];maps[j].Count=1;headers[j].Parent=&nodes[j];nodes[j].Parent=&headers[j];nodes[j].Left=nodes[j].Right=0;nodes[j].Pair.Key=0;}Table table;Pointer storage[5];table.Children.First=table.Children.Last=storage;table.Children.Limit=storage+5;long entries[2]={99,0};Lookup lookup={entries,entries+2,entries+2};table.Entries=&lookup;Pointer start(&objects[6]),cell(&objects[7]);table.ConstructVerticalLine(start,cell,Vec(10,20),2,0);if(created!=2||initialised!=2||table.Children.Last-storage!=2||nodes[0].Pair.Value.Zoom.Y!=2||nodes[0].Pair.Value.Position.X!=10||nodes[1].Pair.Value.Position.Y!=24||positions[1].X!=10)return 1;if(start.Count->References!=1||cell.Count->References!=1)return 2;ChildrenVector grown;grown.First=(Pointer*)malloc(sizeof(Pointer));new(grown.First)Pointer(start);grown.Last=grown.Limit=grown.First+1;grown.push_back(cell);if(grown.Last-grown.First!=2||grown.First[0].Object!=start.Object||grown.First[1].Object!=cell.Object||start.Count->References!=2||cell.Count->References!=2)return 3;grown.First[0].~Pointer();grown.First[1].~Pointer();free(grown.First);StateNode emptyHeader;emptyHeader.Parent=0;StateMap empty;empty.Header=&emptyHeader;empty.Count=0;State& inserted=empty[9UL];if(inserted.Flags!=7||inserted.Zoom.X!=1||inserted.UpdateTime!=-1||emptyHeader.Parent->Pair.Key!=9||empty.Count!=1)return 4;delete emptyHeader.Parent;puts("00551bc0_TEST PASS");return 0;}