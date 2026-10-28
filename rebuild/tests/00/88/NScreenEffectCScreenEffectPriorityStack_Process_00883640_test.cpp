#include <stdio.h>
// NScreenEffect::CScreenEffectPriorityStack::Process(Container* stacks,ICustomProcessInterface* process) -- Advance three priority lists and process or erase each timed effect.
// Leaf: ??$Process@V?$list@UCDisplacementMonochromeEffectColourEntry@CScreenEffectPriorityStack@NScreenEffect@@V?$allocator@UCDisplacementMonochromeEffectColourEntry@CScreenEffectPriorityStack@NScreenEffect@@@std@@@std@@@CScreenEffectPriorityStack@NScreenEffect@@AAEXPAV?$list@UCDisplacementMonochromeEffectColourEntry@CScreenEffectPriorityStack@NScreenEffect@@V?$allocator@UCDisplacementMonochromeEffectColourEntry@CScreenEffectPriorityStack@NScreenEffect@@@std@@@std@@PAVICustomProcessInterface@01@@Z
// Identity: own caller00880e50 passes CDisplacementMonochromeEffectColourEntry stack and corresponding callback vtable; PDB exact private template and original CStackEntry fields.
#include <stdlib.h>
#include <new>
namespace std {
template<class T>class allocator{};
struct _List_node_base {_List_node_base* _M_next;_List_node_base* _M_prev;};template<class T>struct _List_node:public _List_node_base{T _M_data;};
template<class T>struct _Nonconst_traits {typedef T& reference;};
template<class T,class Traits>struct _List_iterator{_List_node_base* _M_node;_List_iterator(_List_node_base* p):_M_node(p){} _List_iterator(const _List_iterator& p):_M_node(p._M_node){}typename Traits::reference operator*()const{return ((_List_node<T>*)_M_node)->_M_data;} _List_iterator& operator++(){_M_node=_M_node->_M_next;return *this;}_List_iterator& operator--(){_M_node=_M_node->_M_prev;return *this;}bool operator!=(const _List_iterator& p)const{return _M_node!=p._M_node;}};
template<class T,class A=allocator<T> >class list {public:typedef T value_type;typedef _List_iterator<T,_Nonconst_traits<T> > iterator;_List_node_base* _M_node;iterator begin(){return iterator(_M_node->_M_next);}iterator end(){return iterator(_M_node);}
iterator erase(iterator position){_List_node_base* next=position._M_node->_M_next;_List_node_base* prev=position._M_node->_M_prev;prev->_M_next=next;next->_M_prev=prev;((_List_node<T>*)position._M_node)->_M_data.~T();free(position._M_node);return iterator(next);}
};
}
struct TestAccess;
namespace NScreenEffect {
// Layout: no active shared owner/entry header. Original CStackEntry vptr0,curdurationcount4,durationcount8,fadingout12. Derived parameter bytes are untouched; sizes96/32/60 from PDB and retail STLport correction. Owner fields are unused by this member template.
class CScreenEffectPriorityStack {public:
struct CStackEntry {virtual ~CStackEntry();long curdurationcount,durationcount;bool fadingout;};
struct CRadialBlurEntry:public CStackEntry {unsigned char param[48];unsigned char posparam[32];virtual ~CRadialBlurEntry();};
struct CDisplacementMonochromeEffectColourEntry:public CStackEntry {unsigned char paramcol[16];virtual ~CDisplacementMonochromeEffectColourEntry();};
struct CScreenFilterEntry:public CStackEntry {unsigned char param[32];unsigned char reset[12];virtual ~CScreenFilterEntry();};
class ICustomProcessInterface {public:virtual ~ICustomProcessInterface(){} virtual void ResetToDefault()=0;virtual void OnErase(CStackEntry*)=0;virtual void Process(float,const CStackEntry*)=0;};
private:template<class Container>void Process(Container*,ICustomProcessInterface*);friend struct ::TestAccess;
};
template<class Container>void CScreenEffectPriorityStack::Process(Container* stacks,ICustomProcessInterface* process){process->ResetToDefault();for(long priority=0;priority<3;++priority){typename Container::iterator last=stacks[priority].end();for(typename Container::iterator it=stacks[priority].begin();it!=last;++it){typename Container::value_type& entry=*it;long duration=entry.durationcount;if(entry.curdurationcount<duration)++entry.curdurationcount;if(entry.fadingout && entry.curdurationcount>=duration){process->OnErase(&entry);it=stacks[priority].erase(it);--it;}else{float weight=1.0f;if(duration>0){weight=(float)entry.curdurationcount/(float)duration;if(weight<0.0f)weight=0.0f;else if(weight>1.0f)weight=1.0f;}if(entry.fadingout)weight=1.0f-weight;process->Process(weight,&entry);}}}}
}
template void NScreenEffect::CScreenEffectPriorityStack::Process<std::list<NScreenEffect::CScreenEffectPriorityStack::CDisplacementMonochromeEffectColourEntry> >(std::list<NScreenEffect::CScreenEffectPriorityStack::CDisplacementMonochromeEffectColourEntry>*,NScreenEffect::CScreenEffectPriorityStack::ICustomProcessInterface*);

static int destroyed;
namespace NScreenEffect {
CScreenEffectPriorityStack::CStackEntry::~CStackEntry(){++destroyed;}
CScreenEffectPriorityStack::CRadialBlurEntry::~CRadialBlurEntry(){}
CScreenEffectPriorityStack::CDisplacementMonochromeEffectColourEntry::~CDisplacementMonochromeEffectColourEntry(){}
CScreenEffectPriorityStack::CScreenFilterEntry::~CScreenFilterEntry(){}
}
typedef NScreenEffect::CScreenEffectPriorityStack Owner;typedef Owner::CDisplacementMonochromeEffectColourEntry Entry;typedef std::list<Entry> List;
struct TestAccess {static void run(Owner& owner,List* lists,Owner::ICustomProcessInterface* process){owner.Process(lists,process);}};
class Callback:public Owner::ICustomProcessInterface {public:int resets,erases,count,bad;float weights[8];const Owner::CStackEntry* entries[8];Callback():resets(0),erases(0),count(0),bad(0){}void ResetToDefault(){++resets;}void OnErase(Owner::CStackEntry* p){++erases;if(!p->fadingout || p->curdurationcount<p->durationcount)bad=1;}void Process(float w,const Owner::CStackEntry* p){if(count<8){weights[count]=w;entries[count]=p;}else bad=1;++count;}};
static Entry* add(List& list,long current,long duration,bool fading){typedef std::_List_node<Entry> Node;Node* n=(Node*)malloc(sizeof(Node));new(&n->_M_data)Entry;n->_M_data.curdurationcount=current;n->_M_data.durationcount=duration;n->_M_data.fadingout=fading;n->_M_next=list._M_node;n->_M_prev=list._M_node->_M_prev;n->_M_prev->_M_next=n;list._M_node->_M_prev=n;return &n->_M_data;}
int main(){Owner owner;List lists[3];std::_List_node_base heads[3];for(int i=0;i<3;++i){heads[i]._M_next=heads[i]._M_prev=&heads[i];lists[i]._M_node=&heads[i];}Callback empty;TestAccess::run(owner,lists,&empty);bool ok=empty.resets==1 && !empty.erases && !empty.count;
Entry* expected[6];expected[0]=add(lists[0],0,4,false);add(lists[0],3,4,true);expected[1]=add(lists[0],7,4,false);expected[2]=add(lists[1],-5,4,false);expected[3]=add(lists[1],0,4,true);expected[4]=add(lists[2],-2,0,false);add(lists[2],0,0,true);expected[5]=add(lists[2],0,-1,false);
Callback callback;TestAccess::run(owner,lists,&callback);float weights[6]={0.25f,1.0f,0.0f,0.75f,1.0f,1.0f};ok=ok && callback.resets==1 && callback.erases==2 && callback.count==6 && destroyed==2 && !callback.bad;
for(int i=0;i<6;++i)ok=ok && callback.weights[i]==weights[i] && callback.entries[i]==expected[i];ok=ok && expected[0]->curdurationcount==1 && expected[1]->curdurationcount==7 && expected[2]->curdurationcount==-4 && expected[3]->curdurationcount==1 && expected[4]->curdurationcount==-1 && expected[5]->curdurationcount==0;
for(int i=0;i<3;++i){while(lists[i].begin()!=lists[i].end())lists[i].erase(lists[i].begin());}ok=ok && destroyed==8;
puts(ok?"00883640_TEST PASS":"00883640_TEST FAIL");return ok?0:1;}