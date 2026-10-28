#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#define __forceinline inline
typedef __SIZE_TYPE__ PlacementSize;
#else
typedef unsigned int PlacementSize;
#endif
inline void* operator new(PlacementSize,void* p) { return p; }
#ifdef _MSC_VER
typedef unsigned __int64 UID;
#else
struct UID{unsigned int Low,High;};
#endif
#ifdef _MSC_VER
extern "C" void* __cdecl malloc(unsigned int);
#else
#include <stdlib.h>
#endif

namespace std {
template<class A,class B> struct pair { A first; B second; pair(const A& a,const B& b):first(a),second(b) {} pair(const pair& p):first(p.first),second(p.second) {} };
class _List_node_base { public: _List_node_base* _M_next; _List_node_base* _M_prev; };
template<class T> class _List_node:public _List_node_base { public: T _M_data; };
template<class T> class allocator {};
template<class T,class Alloc=allocator<T> > class list { public: _List_node_base* _M_node; void push_back(const T& value) { insert(_M_node,value); } void insert(_List_node_base* position,const T& value) { _List_node<T>* node=(_List_node<T>*)malloc(sizeof(_List_node<T>)); new(&node->_M_data) T(value); _List_node_base* previous=position->_M_prev; node->_M_next=position; node->_M_prev=previous; previous->_M_next=node; position->_M_prev=node; } };
}
namespace std{class _Rb_tree_node_base{public:bool _M_color;_Rb_tree_node_base* _M_parent;_Rb_tree_node_base* _M_left;_Rb_tree_node_base* _M_right;};template<class Dummy>class _Rb_global{public:static _Rb_tree_node_base* __cdecl _M_increment(_Rb_tree_node_base* _M_node){if(_M_node->_M_right){_M_node=_M_node->_M_right;while(_M_node->_M_left)_M_node=_M_node->_M_left;}else{_Rb_tree_node_base* y=_M_node->_M_parent;while(_M_node==y->_M_right){_M_node=y;y=y->_M_parent;}if(_M_node->_M_right!=y)_M_node=y;}return _M_node;}};typedef _Rb_global<bool> _Rb_global_inst;template<class T>class _Rb_tree_node:public _Rb_tree_node_base{public:T _M_value_field;};template<class T>class _Rb_tree_iterator{public:_Rb_tree_node_base* _M_node;const T& operator*()const{return ((_Rb_tree_node<T>*)_M_node)->_M_value_field;}bool operator!=(const _Rb_tree_iterator& b)const{return _M_node!=b._M_node;}_Rb_tree_iterator& operator++(){_M_node=_Rb_global_inst::_M_increment(_M_node);return *this;}};template<class T>class _Rb_tree_view{public:_Rb_tree_node_base* _M_header;unsigned int _M_node_count;void* _unused_allocator;_Rb_tree_iterator<T> begin()const{_Rb_tree_iterator<T> p;p._M_node=_M_header->_M_left;return p;}_Rb_tree_iterator<T> end()const{_Rb_tree_iterator<T> p;p._M_node=_M_header;return p;}};}
class CScriptedMapBrush_Fields{public:unsigned char _unmodeled00[0x14];std::_Rb_tree_view<UID> ThingsToDelete,ThingsToMove,ThingsToAdd;};
class CScriptedMapBrush:public CScriptedMapBrush_Fields{public:void GetThingsToMove(std::list<UID>&)const;};
void CScriptedMapBrush::GetThingsToMove(std::list<UID>& out)const{const std::_Rb_tree_view<UID>& things=*(const std::_Rb_tree_view<UID>*)&ThingsToMove;for(std::_Rb_tree_iterator<UID> p=things.begin();p!=things.end();++p)out.push_back(*p);}
static UID value(unsigned int low,unsigned int high){unsigned int words[2]={low,high};UID x;memcpy(&x,words,sizeof(x));return x;}static bool equal(const UID& a,const UID& b){return memcmp(&a,&b,sizeof(a))==0;}int main(){CScriptedMapBrush brush;std::_Rb_tree_node<UID> head,n[3];head._M_parent=&n[1];head._M_left=&n[0];head._M_right=&n[2];for(int i=0;i<3;++i){n[i]._M_parent=&n[1];n[i]._M_left=n[i]._M_right=0;n[i]._M_value_field=value(i+1,i+11);}n[1]._M_parent=&head;n[1]._M_left=&n[0];n[1]._M_right=&n[2];brush.ThingsToMove._M_header=&head;std::list<UID> out;std::_List_node_base root;root._M_next=root._M_prev=&root;out._M_node=&root;brush.GetThingsToMove(out);{std::_List_node_base* p=root._M_next;for(int i=0;i<3;++i){if(p==&root || !equal(((std::_List_node<UID>*)p)->_M_data,n[i]._M_value_field))goto fail;std::_List_node_base* next=p->_M_next;free(p);p=next;}if(p!=&root)goto fail;}head._M_left=&head;head._M_parent=0;head._M_right=&head;root._M_next=root._M_prev=&root;brush.GetThingsToMove(out);if(root._M_next!=&root)goto fail;puts("0082e860_TEST PASS");return 0;fail:puts("0082e860_TEST FAIL");return 1;}
