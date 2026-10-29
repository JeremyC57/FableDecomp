// 009eaf80 CConsole::AddCommandLineToHistory
// Flags:
// Leaf: ?AddCommandLineToHistory@CConsole@@AAEXABVCConsoleCommandLine@@@Z
#include <stddef.h>
#ifndef _MSC_VER
#define __forceinline inline
#endif
extern "C" void __cdecl free(void*);
class CBaseClass{public:virtual~CBaseClass();};
class CCharStringData;
class CCharString{public:CCharStringData*PStringData;};
// Layout: original base4, CurrentEntry4, UserEntry8, CursorPosition12, size16.
class CConsoleCommandLine:public CBaseClass{public:CCharString CurrentEntry;bool UserEntry;long CursorPosition;virtual~CConsoleCommandLine();};
namespace std{
template<class T>class allocator{};
template<class V,class T,class A>class _STLP_alloc_proxy:public A{public:V _M_data;};
struct input_iterator_tag{};struct forward_iterator_tag:input_iterator_tag{};struct bidirectional_iterator_tag:forward_iterator_tag{};
struct _List_node_base{_List_node_base*_M_next,*_M_prev;};
template<class T>struct _List_node:public _List_node_base{T _M_data;};
struct _List_iterator_base{
 _List_node_base*_M_node;
 __forceinline _List_iterator_base(_List_node_base*p):_M_node(p){}
 __forceinline void _M_incr(){_M_node=_M_node->_M_next;}
 __forceinline void _M_decr(){_M_node=_M_node->_M_prev;}
 __forceinline bool operator==(const _List_iterator_base&x)const{return _M_node==x._M_node;}
 __forceinline bool operator!=(const _List_iterator_base&x)const{return _M_node!=x._M_node;}
};
template<class T>struct _Nonconst_traits{typedef T*pointer;typedef T&reference;};
template<class T>struct _Const_traits{typedef const T*pointer;typedef const T&reference;};
template<class T,class Traits>struct _List_iterator:public _List_iterator_base{
 typedef _List_iterator<T,_Nonconst_traits<T> >iterator;
 __forceinline _List_iterator(_List_node<T>*p):_List_iterator_base(p){}
 __forceinline _List_iterator(const iterator&x):_List_iterator_base(x._M_node){}
 __forceinline _List_iterator&operator++(){this->_M_incr();return *this;}
 __forceinline _List_iterator&operator--(){this->_M_decr();return *this;}
};
template<class I>__forceinline ptrdiff_t __distance(const I&first,const I&last,const input_iterator_tag&){ptrdiff_t n=0;I current(first);while(current!=last){++current;++n;}return n;}
template<class I>__forceinline ptrdiff_t distance(const I&first,const I&last){return __distance(first,last,bidirectional_iterator_tag());}
template<class T,class A>class _List_base{public:_STLP_alloc_proxy<_List_node<T>*,_List_node<T>,allocator<_List_node<T> > >_M_node;};
template<class T,class A=allocator<T> >class list:public _List_base<T,A>{public:
 typedef _List_iterator<T,_Nonconst_traits<T> >iterator;
 typedef _List_iterator<T,_Const_traits<T> >const_iterator;
 __forceinline iterator begin(){return iterator(static_cast<_List_node<T>*>(this->_M_node._M_data->_M_next));}
 __forceinline iterator end(){return iterator(this->_M_node._M_data);}
 __forceinline const_iterator begin()const{return const_iterator(static_cast<_List_node<T>*>(this->_M_node._M_data->_M_next));}
 __forceinline const_iterator end()const{return const_iterator(this->_M_node._M_data);}
 __forceinline unsigned int size()const{unsigned int n=std::distance(begin(),end());return n;}
 __forceinline iterator erase(iterator position){
  _List_node_base*next=position._M_node->_M_next;
  _List_node_base*prev=position._M_node->_M_prev;
  _List_node<T>*node=static_cast<_List_node<T>*>(position._M_node);
  prev->_M_next=next;next->_M_prev=prev;node->_M_data.~T();free(node);
  return iterator(static_cast<_List_node<T>*>(next));
 }
 __forceinline void pop_back(){iterator tmp=end();erase(--tmp);}
 iterator insert(iterator,const T&);
};
}
template<class T>class CTBaseSingleton{public:virtual~CTBaseSingleton();};
// Layout: RTTI singletonbase4, unread72, history76; donor history88 reflects
// three16byte donor maps becoming12bytes. Other state is not read here.
class CConsole:public CTBaseSingleton<CConsole>{unsigned char pass9_unread_prefix[72];public:std::list<CConsoleCommandLine>CommandLineHistory;private:
 void ResetHistory();void AddCommandLineToHistory(const CConsoleCommandLine&);
};
void CConsole::AddCommandLineToHistory(const CConsoleCommandLine&line){
 if(CommandLineHistory.size()>=256)CommandLineHistory.pop_back();
 CommandLineHistory.insert(CommandLineHistory.begin(),line);
 ResetHistory();
}