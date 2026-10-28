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