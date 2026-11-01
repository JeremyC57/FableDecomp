// 00794e60 behavior fixture: target and STLport iterator/increment bodies pasted.
// Real constructed receivers, tree nodes and retained intelligent handles.
// Unused interfaces reduced; external engine/actions are checked boundaries, not native layout proof.
#include <stdio.h>
#ifndef _MSC_VER
#define __cdecl
#endif
static int failures=0;static void Check(bool x){if(!x)++failures;}
static int events[32],eventCount;static void Event(int n){Check(eventCount<32);events[eventCount++]=n;}
namespace std{typedef int ptrdiff_t;struct bidirectional_iterator_tag{};typedef bool _Rb_tree_Color_type;template<class A,class B>struct pair{A first;B second;pair():first(A()),second(B()){}};
struct _Rb_tree_node_base{typedef _Rb_tree_node_base*_Base_ptr;_Rb_tree_Color_type _M_color;_Base_ptr _M_parent,_M_left,_M_right;};
template<class V>struct _Rb_tree_node:public _Rb_tree_node_base{V _M_value_field;};
template<class Dummy>class _Rb_global{public:typedef _Rb_tree_node_base*_Base_ptr;static _Base_ptr __cdecl _M_increment(_Base_ptr);static _Base_ptr __cdecl _M_decrement(_Base_ptr);};
typedef _Rb_global<bool>_Rb_global_inst;
struct _Rb_tree_base_iterator{typedef _Rb_tree_node_base*_Base_ptr;typedef bidirectional_iterator_tag iterator_category;typedef ptrdiff_t difference_type;_Base_ptr _M_node;bool operator==(const _Rb_tree_base_iterator&x)const{return _M_node==x._M_node;}bool operator!=(const _Rb_tree_base_iterator&x)const{return _M_node!=x._M_node;}};
template<class T>struct _Nonconst_traits{typedef T value_type;typedef T&reference;typedef T*pointer;};
template<class V,class Traits>struct _Rb_tree_iterator:public _Rb_tree_base_iterator{
 typedef V value_type;typedef typename Traits::reference reference;typedef typename Traits::pointer pointer;typedef _Rb_tree_iterator<V,Traits>_Self;typedef _Rb_tree_node<V>*_Link_type;
 _Rb_tree_iterator(){_M_node=0;}_Rb_tree_iterator(_Link_type p){_M_node=p;}_Rb_tree_iterator(const _Rb_tree_iterator<V,_Nonconst_traits<V> >&p){_M_node=p._M_node;}
 reference operator*()const{return _Link_type(_M_node)->_M_value_field;}pointer operator->()const{return &operator*();}
 _Self&operator++(){_M_node=_Rb_global_inst::_M_increment(_M_node);return*this;}_Self&operator--(){_M_node=_Rb_global_inst::_M_decrement(_M_node);return*this;}
};
template<class Dummy>inline typename _Rb_global<Dummy>::_Base_ptr __cdecl _Rb_global<Dummy>::_M_increment(_Base_ptr node){
 if(node->_M_right){node=node->_M_right;while(node->_M_left)node=node->_M_left;}
 else{_Base_ptr parent=node->_M_parent;while(node==parent->_M_right){node=parent;parent=parent->_M_parent;}if(node->_M_right!=parent)node=parent;}return node;
}

template<class K,class V>class map{public:typedef pair<K,V>value_type;typedef _Rb_tree_node<value_type>node_type;typedef _Rb_tree_iterator<value_type,_Nonconst_traits<value_type> >iterator;node_type header,nodes[3];unsigned int count;map():count(0){SetCount(0);}void SetCount(unsigned int n){count=n;header._M_color=false;header._M_parent=0;header._M_left=header._M_right=&header;for(unsigned int j=0;j<3;++j){nodes[j]._M_parent=nodes[j]._M_left=nodes[j]._M_right=0;nodes[j]._M_color=true;}if(n){unsigned int root=n/2;header._M_parent=&nodes[root];header._M_left=&nodes[0];header._M_right=&nodes[n-1];nodes[root]._M_parent=&header;if(root){nodes[root]._M_left=&nodes[0];nodes[0]._M_parent=&nodes[root];}if(root+1<n){nodes[root]._M_right=&nodes[root+1];nodes[root+1]._M_parent=&nodes[root];}}}iterator begin(){return static_cast<node_type*>(header._M_left);}iterator end(){return &header;}};
}
enum ETCInterfaceType{SoundType=0x10,AbilityType=0xa7,UnholyType=0x86};
class CTCBase{public:virtual~CTCBase(){}};class CTCSoundPlayer;class CTCSpecialAbilities;
class CThing{public:int id,refs,kills;bool dead;mutable int deadChecks;CThing(int n=0):id(n),refs(0),kills(0),dead(false),deadChecks(0){}virtual~CThing(){}bool IsDead()const{++deadChecks;return dead;}void Kill(bool b){Check(b&&!dead);++kills;dead=true;Event(400+id);}bool HasTC(ETCInterfaceType)const;CTCBase&GetTC(ETCInterfaceType);bool GetTC(CTCSoundPlayer**);void RemoveTC(ETCInterfaceType);};
static int liveHandles=0;
template<class T>class CIntelligentPointer{T*ptr;public:mutable int gets;CIntelligentPointer():ptr(0),gets(0){++liveHandles;}CIntelligentPointer(const CIntelligentPointer&o):ptr(o.ptr),gets(0){++liveHandles;if(ptr)++ptr->refs;}~CIntelligentPointer(){if(ptr)--ptr->refs;--liveHandles;}void Reset(T*p){if(ptr)--ptr->refs;ptr=p;if(ptr)++ptr->refs;}T*Get()const{++gets;return ptr;}};
class CThingCreatureBase:public CThing{public:CThingCreatureBase(int n=0):CThing(n){}void FinishCurrentAction(){Event(200);}};
class CTCSoundPlayer:public CTCBase{public:unsigned long expected;int stops;CTCSoundPlayer():expected(0),stops(0){}void StopAttachedSound(unsigned long n){Check(n==expected);++stops;Event(600);}};
class CTCSpecialAbilities:public CTCBase{public:bool AbilityMutexLock;int CurrentAbility;CTCSpecialAbilities():AbilityMutexLock(true),CurrentAbility(7){}void UnLockAbilitiyMutex(){AbilityMutexLock=false;CurrentAbility=0;Event(300);}static void FadeOrKillEffect(CIntelligentPointer<CThing>&p){CThing*t=p.Get();Check(t&&!t->dead);Event(500+t->id);t->dead=true;p.Reset(0);}};
static CThingCreatureBase*expectedParent;static CTCSoundPlayer*sound;static CTCSpecialAbilities*abilities;static bool hasSound,hasAbility;static int hasQueries,componentGets,soundQueries,removes;
bool CThing::HasTC(ETCInterfaceType n)const{Check(this==expectedParent&&n==AbilityType);++hasQueries;return hasAbility;}
CTCBase&CThing::GetTC(ETCInterfaceType n){Check(this==expectedParent&&n==AbilityType&&hasAbility);++componentGets;return*abilities;}
bool CThing::GetTC(CTCSoundPlayer**out){Check(this==expectedParent);++soundQueries;if(hasSound){*out=sound;return true;}return false;}
void CThing::RemoveTC(ETCInterfaceType n){Check(this==expectedParent&&n==UnholyType);++removes;Event(700);}
class CEnvironment{public:void TransitionToDefaultThemeAllChannels(long n,float f){Check(n==2&&f==0.001f);Event(150);}};
class CWorld{public:CEnvironment environment;CEnvironment&GetEnvironment(){return environment;}};
class CTCUnholyPower:public CTCBase{public:class PerCreatureInfo{public:CIntelligentPointer<CThingCreatureBase>ActualCreature;};CThing&ParentThing;CWorld&world;std::map<CThingCreatureBase*,PerCreatureInfo>LastFrameCreaturesHit;bool CutSceneCancel;CIntelligentPointer<CThing>BuildUpEffect,DarkFacesEffect,HandEffect1,HandEffect2;unsigned long LoopingSound;bool clearOriginal;int resetIndex;
 CTCUnholyPower(CThing&t,CWorld&w):ParentThing(t),world(w),CutSceneCancel(false),LoopingSound(0),clearOriginal(false),resetIndex(0){}CWorld&GetWorld()const{return world;}
 void ResetCreatureHitState(CIntelligentPointer<CThingCreatureBase>p){CThingCreatureBase*t=p.Get();Check(t&&t->refs>=3);Event(100+t->id);if(clearOriginal)LastFrameCreaturesHit.nodes[resetIndex]._M_value_field.second.ActualCreature.Reset(0);Check(p.Get()==t&&t->refs>=2);++resetIndex;}
 void StopSound(unsigned long sample){CTCSoundPlayer*sound;if(ParentThing.GetTC(&sound))sound->StopAttachedSound(sample);}
 void CancelForCutscene();
};
void CTCUnholyPower::CancelForCutscene(){
 std::map<CThingCreatureBase*,PerCreatureInfo>::iterator end=LastFrameCreaturesHit.end();
 for(std::map<CThingCreatureBase*,PerCreatureInfo>::iterator it=LastFrameCreaturesHit.begin();it!=end;++it){CIntelligentPointer<CThingCreatureBase>creature=it->second.ActualCreature;ResetCreatureHitState(creature);}
 CutSceneCancel=true;
 GetWorld().GetEnvironment().TransitionToDefaultThemeAllChannels(2,0.001f);
 CThingCreatureBase&parent=static_cast<CThingCreatureBase&>(ParentThing);parent.FinishCurrentAction();
 if(parent.HasTC(static_cast<ETCInterfaceType>(0xa7)))static_cast<CTCSpecialAbilities&>(parent.GetTC(static_cast<ETCInterfaceType>(0xa7))).UnLockAbilitiyMutex();
 if(BuildUpEffect.Get()&&!BuildUpEffect.Get()->IsDead())BuildUpEffect.Get()->Kill(true);
 if(HandEffect1.Get()&&!HandEffect1.Get()->IsDead())CTCSpecialAbilities::FadeOrKillEffect(HandEffect1);
 if(HandEffect2.Get()&&!HandEffect2.Get()->IsDead())CTCSpecialAbilities::FadeOrKillEffect(HandEffect2);
 if(DarkFacesEffect.Get()&&!DarkFacesEffect.Get()->IsDead())DarkFacesEffect.Get()->Kill(true);
 StopSound(LoopingSound);
 parent.RemoveTC(static_cast<ETCInterfaceType>(0x86));
}

int main(){unsigned int cases=0;
 for(unsigned int count=0;count<=3;++count)for(int pattern=0;pattern<81;++pattern)for(int components=0;components<4;++components)for(int mutation=0;mutation<2;++mutation){
  Check(liveHandles==0);CThingCreatureBase parent(9),creatures[3];expectedParent=&parent;CTCSoundPlayer soundObject;CTCSpecialAbilities abilityObject;sound=&soundObject;abilities=&abilityObject;hasSound=(components&1)!=0;hasAbility=(components&2)!=0;hasQueries=componentGets=soundQueries=removes=0;CWorld world;CThing effects[4];int states[4],digits=pattern;for(int j=0;j<4;++j){states[j]=digits%3;digits/=3;effects[j].id=j;effects[j].dead=states[j]==2;}
  {CTCUnholyPower spell(parent,world);spell.LoopingSound=(mutation?0x80000000UL:0UL)+123;soundObject.expected=spell.LoopingSound;spell.clearOriginal=mutation!=0;spell.LastFrameCreaturesHit.SetCount(count);for(unsigned int j=0;j<count;++j){creatures[j].id=j;spell.LastFrameCreaturesHit.nodes[j]._M_value_field.first=&creatures[j];spell.LastFrameCreaturesHit.nodes[j]._M_value_field.second.ActualCreature.Reset(&creatures[j]);}
   CIntelligentPointer<CThing>*handles[4]={&spell.BuildUpEffect,&spell.HandEffect1,&spell.HandEffect2,&spell.DarkFacesEffect};for(int j=0;j<4;++j)handles[j]->Reset(states[j]?&effects[j]:0);int baseHandles=liveHandles;eventCount=0;spell.CancelForCutscene();
   int expected[32],n=0;for(unsigned int j=0;j<count;++j)expected[n++]=100+j;expected[n++]=150;expected[n++]=200;if(hasAbility)expected[n++]=300;for(int j=0;j<4;++j)if(states[j]==1)expected[n++]=(j==1||j==2?500:400)+j;if(hasSound)expected[n++]=600;expected[n++]=700;Check(eventCount==n);for(int j=0;j<n&&j<eventCount;++j)Check(events[j]==expected[j]);
   Check(spell.CutSceneCancel&&spell.resetIndex==(int)count);Check(liveHandles==baseHandles);Check(hasQueries==1&&componentGets==(hasAbility?1:0)&&soundQueries==1&&removes==1);Check(soundObject.stops==(hasSound?1:0));Check(abilityObject.AbilityMutexLock==!hasAbility&&abilityObject.CurrentAbility==(hasAbility?0:7));
   for(int j=0;j<4;++j){Check(effects[j].dead==(states[j]!=0));Check(effects[j].kills==((j==0||j==3)&&states[j]==1?1:0));Check(effects[j].deadChecks==(states[j]?1:0));int wantedGets=states[j]==0?1:states[j]==2?2:3;Check(handles[j]->gets==wantedGets);Check(effects[j].refs==(states[j]&&!(states[j]==1&&(j==1||j==2))?1:0));}
   for(unsigned int j=0;j<count;++j)Check(creatures[j].refs==(mutation?0:1));
  }Check(liveHandles==0);for(int j=0;j<4;++j)Check(effects[j].refs==0);for(unsigned int j=0;j<count;++j)Check(creatures[j].refs==0);++cases;
 }
 Check(cases==2592);if(failures){fprintf(stderr,"failures=%d cases=%u\n",failures,cases);return 1;}puts("00794e60_TEST PASS");return 0;
}
