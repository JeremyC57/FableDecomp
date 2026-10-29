#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <new>
#ifndef _MSC_VER
#define __cdecl
#endif
#define private public
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

#undef private
// These tokens are fixture-owned string handles, not a CCharStringData layout.
struct FixtureToken{unsigned int id;int references;};
static unsigned int destroyed,inserted,resetCalls;
static CConsole*expectedConsole;
static const CConsoleCommandLine*expectedLine;
CBaseClass::~CBaseClass(){}
template<class T>CTBaseSingleton<T>::~CTBaseSingleton(){}
CConsoleCommandLine::~CConsoleCommandLine(){++destroyed;if(CurrentEntry.PStringData)--reinterpret_cast<FixtureToken*>(CurrentEntry.PStringData)->references;}
void CConsole::ResetHistory(){if(this!=expectedConsole||inserted!=1)abort();++resetCalls;}
namespace std{
template<class T,class A>typename list<T,A>::iterator list<T,A>::insert(iterator pos,const T&value){
 if(&value!=expectedLine||pos._M_node!=this->_M_node._M_data->_M_next)abort();++inserted;
 _List_node<T>*node=new(malloc(sizeof(_List_node<T>)))_List_node<T>;
 node->_M_data.CurrentEntry=value.CurrentEntry;node->_M_data.UserEntry=value.UserEntry;node->_M_data.CursorPosition=value.CursorPosition;
 if(value.CurrentEntry.PStringData)++reinterpret_cast<FixtureToken*>(value.CurrentEntry.PStringData)->references;
 node->_M_next=pos._M_node;node->_M_prev=pos._M_node->_M_prev;
 node->_M_prev->_M_next=node;pos._M_node->_M_prev=node;
 return iterator(node);
}
}
int main(){
 typedef std::_List_node<CConsoleCommandLine>Node;
 unsigned int cases=0;
 for(int n=0;n<=271;++n)for(int pattern=0;pattern<3;++pattern){
  int count=n==271?1000:n;
  FixtureToken tokens[9];for(int k=0;k<9;++k){tokens[k].id=k;tokens[k].references=0;}
  CConsole console;expectedConsole=&console;
  Node*head=new(malloc(sizeof(Node)))Node;head->_M_data.CurrentEntry.PStringData=0;
  Node**nodes=static_cast<Node**>(malloc(sizeof(Node*)*(count+1)));
  for(int k=0;k<count;++k){nodes[k]=new(malloc(sizeof(Node)))Node;
   int id=pattern==0?k%8:pattern==1?0:(k%2?7:3);
   nodes[k]->_M_data.CurrentEntry.PStringData=reinterpret_cast<CCharStringData*>(&tokens[id]);++tokens[id].references;
   nodes[k]->_M_data.UserEntry=(k&1)!=0;nodes[k]->_M_data.CursorPosition=k;
  }
  head->_M_next=count?nodes[0]:head;head->_M_prev=count?nodes[count-1]:head;
  for(int k=0;k<count;++k){nodes[k]->_M_prev=k?nodes[k-1]:head;nodes[k]->_M_next=k+1<count?nodes[k+1]:head;}
  console.CommandLineHistory._M_node._M_data=head;
  CConsoleCommandLine line;line.CurrentEntry.PStringData=reinterpret_cast<CCharStringData*>(&tokens[pattern==2?0:8]);++reinterpret_cast<FixtureToken*>(line.CurrentEntry.PStringData)->references;
  line.UserEntry=true;line.CursorPosition=123456;expectedLine=&line;
  destroyed=inserted=resetCalls=0;
  console.AddCommandLineToHistory(line);
  if(destroyed!=(count>=256?1u:0u)||inserted!=1||resetCalls!=1)return 1;
  Node*added=static_cast<Node*>(head->_M_next);
  if(added->_M_data.CurrentEntry.PStringData!=line.CurrentEntry.PStringData||!added->_M_data.UserEntry||added->_M_data.CursorPosition!=123456||added->_M_prev!=head)return 2;
  std::_List_node_base*prev=added;Node*walk=static_cast<Node*>(added->_M_next);
  const int retained=count-(count>=256?1:0);
  for(int k=0;k<retained;++k){
   if(walk!=nodes[k]||walk->_M_prev!=prev||walk->_M_data.CursorPosition!=k||walk->_M_data.UserEntry!=((k&1)!=0))return 3;
   prev=walk;walk=static_cast<Node*>(walk->_M_next);
  }
  if(walk!=head||head->_M_prev!=prev||prev->_M_next!=head)return 4;
  walk=static_cast<Node*>(head->_M_next);while(walk!=head){Node*next=static_cast<Node*>(walk->_M_next);walk->~Node();free(walk);walk=next;}
  head->~Node();free(head);free(nodes);
  for(int k=0;k<9;++k)if(tokens[k].references!=(line.CurrentEntry.PStringData==reinterpret_cast<CCharStringData*>(&tokens[k])?1:0))return 5;
  ++cases;
 }
 if(cases!=816)return 6;puts("009eaf80_TEST PASS");return 0;
}