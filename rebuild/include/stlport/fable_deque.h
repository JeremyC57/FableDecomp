#ifndef FABLE_STLPORT_FABLE_DEQUE_H
#define FABLE_STLPORT_FABLE_DEQUE_H
// STLport4.6.2 stl/_deque.h and _deque.c: native iterator/base hierarchy.
#include "stlport/fable_containers.h"
#ifdef FABLE_RETAIL_STLP_DEQUE_COPY_CTOR
#include "rebuild_abi.h"                     // FABLE_FASTCALL (NF84 opt-in below)
#endif
namespace std {
template<class T>struct _Deque_iterator_base {
 enum _Constants {_blocksize=128,__buffer_size=sizeof(T)<_blocksize?_blocksize/sizeof(T):1};
 typedef T value_type;typedef T** _Map_pointer;
 T* _M_cur;T* _M_first;T* _M_last;T** _M_node;
 _Deque_iterator_base():_M_cur(0),_M_first(0),_M_last(0),_M_node(0){}
 void _M_set_node(_Map_pointer node){_M_last=(_M_first=*(_M_node=node))+__buffer_size;}
 void _M_increment(){if(++_M_cur==_M_last){_M_set_node(_M_node+1);_M_cur=_M_first;}}
#ifdef FABLE_STLPORT_DEQUE_PUSH_BACK
 // Opt-in (chunk Q6, row 0x00912840 CAIStateGroup_FlyingThingBase::AddRandomBezierCurveToPath): STLport 4.6 _deque.h _M_decrement.
 void _M_decrement(){if(_M_cur==_M_first){_M_set_node(_M_node-1);_M_cur=_M_last;}--_M_cur;}
#endif
#if defined(FABLE_RETAIL_STLP_DEQUE_BACK_POP_BODIES) && !defined(FABLE_STLPORT_DEQUE_PUSH_BACK)
 // NF79 per-TU opt-in (rows 0x0059B039 deque<NUISystem::CComponent*>::back / 0x0059AEE5 pop_back): STLport 4.6 _M_decrement, declared
 // only: retail calls the deque<NUISystem::CComponent*> instance out of line (0x0059AC41, from back() at 0x0059B04F).
 void _M_decrement();
#endif
#if defined(FABLE_RETAIL_STLP_STACK_SIZE_PUSH) && defined(FABLE_RETAIL_STLP_STACK_TOP_POP)
 // NF79 per-TU opt-in (row 0x00596763 NUISystem::CFrontEndManager::GotoNextScreen): STLport 4.6 _deque.h _M_subtract, declared only:
 // retail calls the deque<NUISystem::CComponent*> instance out of line (0x0059A8F3; the out-of-line operator- 0x0059AAE3 is a
 // `jmp` to it) from the inline deque::size() / stack::size().
 ptrdiff_t _M_subtract(const _Deque_iterator_base& x)const;
#endif
#if defined(FABLE_RETAIL_STLP_DEQUE_MSUBTRACT_BODY) && !(defined(FABLE_RETAIL_STLP_STACK_SIZE_PUSH) && defined(FABLE_RETAIL_STLP_STACK_TOP_POP))
 // NF83 per-TU opt-in (round 7, row 0x0059A8F3, the out-of-line deque<NUISystem::CComponent*> COMDAT itself): the same STLport 4.6
 // _M_subtract declaration; its body is below the class.
 ptrdiff_t _M_subtract(const _Deque_iterator_base& x)const;
#endif
};
#ifdef FABLE_RETAIL_STLP_DEQUE_MSUBTRACT_BODY
// NF83 per-TU opt-in (round 7, row 0x0059A8F3): STLport 4.6 _deque.h _Deque_iterator_base::_M_subtract (the iterator difference:
// whole buffers between the nodes, then the partial ends), the body of the out-of-line deque<NUISystem::CComponent*> instance.
template<class T> ptrdiff_t _Deque_iterator_base<T>::_M_subtract(const _Deque_iterator_base<T>& x)const{
 return ptrdiff_t(__buffer_size)*(_M_node-x._M_node-1)+(_M_cur-_M_first)+(x._M_last-x._M_cur);
}
#endif
template<class T,class Traits>struct _Deque_iterator:public _Deque_iterator_base<T> {
 typedef T value_type;typedef random_access_iterator_tag iterator_category;typedef ptrdiff_t difference_type;
 typedef typename Traits::reference reference;typedef typename Traits::pointer pointer;
 _Deque_iterator(){}
 _Deque_iterator(const _Deque_iterator<T,_Nonconst_traits<T> >& x):_Deque_iterator_base<T>(x){}
#ifdef FABLE_RETAIL_GUARD_FRAMEUPDATE_PASS29
 pointer operator->()const{return this->_M_cur;}
#endif
 reference operator*()const{return *this->_M_cur;}
 _Deque_iterator& operator++(){this->_M_increment();return *this;}
 bool operator!=(const _Deque_iterator& other)const{return this->_M_cur!=other._M_cur;}
#ifdef FABLE_STLPORT_DEQUE_PUSH_BACK
 _Deque_iterator& operator--(){this->_M_decrement();return *this;}
#endif
#if defined(FABLE_RETAIL_STLP_DEQUE_BACK_POP_BODIES) && !defined(FABLE_STLPORT_DEQUE_PUSH_BACK)
 _Deque_iterator& operator--(){this->_M_decrement();return *this;}
#endif
#if defined(FABLE_RETAIL_STLP_STACK_SIZE_PUSH) && defined(FABLE_RETAIL_STLP_STACK_TOP_POP)
 difference_type operator-(const _Deque_iterator& x)const{return this->_M_subtract(x);}
#endif
};
template<class T,class Traits>struct iterator_traits<_Deque_iterator<T,Traits> > {
 typedef T value_type;typedef random_access_iterator_tag iterator_category;typedef ptrdiff_t difference_type;
 typedef typename Traits::reference reference;typedef typename Traits::pointer pointer;
};
#ifdef FABLE_RETAIL_STLP_DEQUE_COPY_CTOR
// NF84 per-TU opt-in (row 0x0059B4FE): STLport 4.6 _uninitialized.h __uninitialized_copy(_InputIter, _InputIter, _ForwardIter,
// const __true_type&) for non-pointer iterators, declared only: retail calls the deque<NUISystem::CComponent*> instance out of line as a
// __fastcall COMDAT (0x0059B49B: hidden result in ecx, the tag in edx, the three iterators by value; `ret 30h`).
template<class I,class O> O FABLE_FASTCALL __uninitialized_copy(I first,I last,O result,const __true_type&);
#endif
#ifdef FABLE_RETAIL_STLP_DEQUE_UNINIT_COPY_BODY
// NF86 per-TU opt-in (round 7, q11104, row 0x0059B49B; with FABLE_RETAIL_STLP_DEQUE_COPY_CTOR and FABLE_RETAIL_STLP_ORIGINAL_COPY_DISPATCH_PASS22):
// the STLport 4.6 _uninitialized.h body of that declaration, `return __copy_aux(first, last, result, _BothPtrType<I,O>::_Ret());`, for the
// TU that defines the deque<NUISystem::CComponent*> instance (retail 0x0059B49B, which calls __copy_aux 0x0059B2CC out of line). Body only.
template<class I,class O> O FABLE_FASTCALL __uninitialized_copy(I first,I last,O result,const __true_type&) {
    return __copy_aux(first,last,result,_BothPtrType<I,O>::_Ret());
}
#endif
template<class T,class A>class _Deque_base {
public:
 typedef T value_type;typedef A allocator_type;typedef allocator<T*> _Map_alloc_type;
 typedef _Deque_iterator<T,_Nonconst_traits<T> > iterator;
 static size_t buffer_size(){return _Deque_iterator_base<T>::__buffer_size;}
#ifdef FABLE_RETAIL_STLP_DEQUE_COPY_CTOR
 // NF84 per-TU opt-in (round 7, row 0x0059B4FE, the deque<NUISystem::CComponent*> copy constructor): the counted base constructor,
 // declared only: retail calls the instance out of line (0x0059AEF9, landed `??0?$_Deque_base@...@QAE@ABV?$allocator@...@1@I@Z`).
 _Deque_base(const A& a,size_t count);
#else
 _Deque_base(const A& a,size_t count):_M_start(),_M_finish(),_M_map(allocator<T*>(),0),_M_map_size(a,0){_M_initialize_map(count);}
#endif
 _Deque_base(const A& a):_M_start(),_M_finish(),_M_map(allocator<T*>(),0),_M_map_size(a,0){}
 ~_Deque_base(){if(_M_map._M_data){_M_destroy_nodes(_M_start._M_node,this->_M_finish._M_node+1);_M_map.deallocate(_M_map._M_data,_M_map_size._M_data);}}
protected:
 void _M_initialize_map(size_t count){
  size_t nodes=count/buffer_size()+1;
  _M_map_size._M_data=(std::max)((size_t)8,nodes+2);
  _M_map._M_data=_M_map.allocate(_M_map_size._M_data);
  T** first=_M_map._M_data+(_M_map_size._M_data-nodes)/2;T** last=first+nodes;
  _M_create_nodes(first,last);_M_start._M_set_node(first);_M_finish._M_set_node(last-1);
  _M_start._M_cur=_M_start._M_first;_M_finish._M_cur=_M_finish._M_first+count%buffer_size();
 }
 void _M_create_nodes(T** first,T** last){for(T** n=first;n<last;++n)*n=_M_map_size.allocate(buffer_size());}
 void _M_destroy_nodes(T** first,T** last){for(T** n=first;n<last;++n)_M_map_size.deallocate(*n,buffer_size());}
 iterator _M_start,_M_finish;
 _STLP_alloc_proxy<T**,T*,_Map_alloc_type> _M_map;
 _STLP_alloc_proxy<size_t,T,A> _M_map_size;
};
template<class T,class A=allocator<T> >class deque:protected _Deque_base<T,A> {
public:
 typedef T value_type;typedef size_t size_type;typedef T& reference;typedef const T& const_reference;
#ifdef FABLE_RETAIL_STLP_DEQUE_PASS22_POP
 bool empty()const{return this->_M_finish._M_cur==this->_M_start._M_cur;}
#ifdef FABLE_RETAIL_GUILDSEAL_DEQUE_POP_PASS30
 void _M_pop_front_aux();
 void pop_front() {
  if (this->_M_start._M_cur != this->_M_start._M_last - 1) {
   this->_M_start._M_cur->value_type::~value_type();
   ++this->_M_start._M_cur;
  } else _M_pop_front_aux();
 }
#else
 void pop_front(){
  if(this->_M_start._M_cur!=this->_M_start._M_last-1){_Destroy(this->_M_start._M_cur);++this->_M_start._M_cur;}
  else {_Destroy(this->_M_start._M_cur);this->_M_map_size.deallocate(this->_M_start._M_first,this->buffer_size());this->_M_start._M_set_node(this->_M_start._M_node+1);this->_M_start._M_cur=this->_M_start._M_first;}
 } // STLport4.6 _deque.h / _deque.c, expanded in0040C790
#endif
#endif
#if defined(FABLE_RETAIL_STLP_STACK_EMPTY) && !defined(FABLE_RETAIL_STLP_DEQUE_PASS22_POP)
 // NF86 per-TU opt-in (with stack::empty below): STLport 4.6 _deque.h `bool empty() const { return _M_finish == _M_start; }`, inline
 // (the iterator compare is the _M_cur compare in place, as in the FABLE_RETAIL_STLP_DEQUE_PASS22_POP spelling above). Body only.
 bool empty()const{return this->_M_finish._M_cur==this->_M_start._M_cur;}
#endif
#ifdef FABLE_RETAIL_GUARD_FRAMEUPDATE_PASS29
 typedef typename _Deque_base<T,A>::iterator iterator;
 iterator begin(){return this->_M_start;}
 iterator end(){return this->_M_finish;}
 void clear();
#endif
#ifdef FABLE_RETAIL_FLIGHT_PASS29
 reference front(){return *this->_M_start;}
 size_type size()const{return this->_M_finish._M_subtract(this->_M_start);}
#endif
 deque():_Deque_base<T,A>(A(),0){}
#ifdef FABLE_RETAIL_STLP_DEQUE_COPY_CTOR
 // NF84 per-TU opt-in (round 7, row 0x0059B4FE): STLport 4.6 _deque.h deque(const deque&) -- the base sized from the source
 // (`__x.get_allocator(), __x.size()`; size() is the out-of-line _M_subtract 0x0059A8F3 through the inline operator-, so this
 // needs FABLE_RETAIL_STLP_STACK_SIZE_PUSH + FABLE_RETAIL_STLP_STACK_TOP_POP), then __uninitialized_copy(__x.begin(), __x.end(),
 // _M_start, _IsPODType()) with the pointer element's __true_type (the iterator overload declared below, out of line in retail:
 // 0x0059B49B). get_allocator() / const begin() / end() are STLport's.
 typedef _Deque_iterator<T,_Const_traits<T> > const_iterator;
 A get_allocator()const{return this->_M_map_size;}
 const_iterator begin()const{return this->_M_start;}
 const_iterator end()const{return this->_M_finish;}
 deque(const deque& x):_Deque_base<T,A>(x.get_allocator(),x.size()){
  __uninitialized_copy(x.begin(),x.end(),this->_M_start,typename __type_traits<T>::is_POD_type());
 }
#endif
#if defined(FABLE_RETAIL_STLP_DEQUE_BACK_POP_BODIES) && !defined(FABLE_STLPORT_DEQUE_PUSH_BACK)
 // NF79 per-TU opt-in (rows 0x0059B039 / 0x0059AEE5, the out-of-line deque<NUISystem::CComponent*> COMDATs themselves): STLport 4.6 _deque.h
 // back() and pop_back() with their bodies; _M_pop_back_aux (_deque.c, protected; retail 0x0059AD35) declared only.
 typedef typename _Deque_base<T,A>::iterator iterator;
 reference back(){iterator tmp=this->_M_finish;--tmp;return *tmp;}
 void pop_back(){
  if(this->_M_finish._M_cur!=this->_M_finish._M_first){--this->_M_finish._M_cur;_Destroy(this->_M_finish._M_cur);}
  else _M_pop_back_aux();
 }
protected:
 void _M_pop_back_aux();
public:
#elif defined(FABLE_RETAIL_STLP_STACK_TOP_POP) && !defined(FABLE_STLPORT_DEQUE_PUSH_BACK)
 // NF76 per-TU opt-in (row 0x00599E3F NUISystem::CFrontEndManager::Update): STLport 4.6 _deque.h back() / pop_back(), declared only:
 // that TU calls the deque<NUISystem::CComponent*> COMDATs (back 0x0059B039, `mov ecx,edi ; call` at 0x00599E9C; pop_back 0x0059AEE5,
 // 0x00599EFF) from the inline stack::top() / pop() below.
 reference back();
 void pop_back();
#ifdef FABLE_RETAIL_STLP_STACK_SIZE_PUSH
 // NF79 nested opt-in (row 0x00596763): STLport 4.6 deque::size() over the out-of-line iterator difference (above).
 size_type size()const{return this->_M_finish-this->_M_start;}
#endif
#endif
 ~deque(){std::_Destroy(this->_M_start,this->_M_finish);}
#if defined(FABLE_RETAIL_STLP_DEQUE_PUSH_BACK_OUTOFLINE) && !defined(FABLE_STLPORT_DEQUE_PUSH_BACK)
 // Opt-in (wf13, row 0x00492973 CSubtitleRenderer::SetText): STLport 4.6 push_back, declared only: retail calls the
 // deque<CSubtitleRenderer::CSetTextInfo> instance out of line (0x004941C0, from the inline queue::push).
 void push_back(const value_type& t);
#endif
#ifdef FABLE_STLPORT_DEQUE_PUSH_BACK
 // Opt-in (chunk Q6, row 0x00912840): STLport 4.6 _deque.h back() / push_back() / _M_reserve_map_at_back(), inline as in the
 // library; _M_push_back_aux_v (_deque.c) and _M_reallocate_map are out of line (retail 0x00913D60 for deque<CBezierCurve>):
 // declarations only here, a row whose retail TU holds the _M_push_back_aux_v body defines it (see the row).
 typedef typename _Deque_base<T,A>::iterator iterator;
 reference back(){iterator tmp=this->_M_finish;--tmp;return *tmp;}
 void push_back(const value_type& t){
  if(this->_M_finish._M_cur!=this->_M_finish._M_last-1){_Construct(this->_M_finish._M_cur,t);++this->_M_finish._M_cur;}
  else _M_push_back_aux_v(t);
 }
protected:
 void _M_push_back_aux_v(const value_type& t);
 void _M_reserve_map_at_back(size_type nodes_to_add=1){
  if(nodes_to_add+1>this->_M_map_size._M_data-(this->_M_finish._M_node-this->_M_map._M_data))_M_reallocate_map(nodes_to_add,false);
 }
 void _M_reallocate_map(size_type nodes_to_add,bool add_at_front);
public:
#ifdef FABLE_RETAIL_STLP_DEQUE_CLEAR
 // Nested opt-in (chunk 2004, row 0x00912BA0 CAIStateGroup_FlyingThingBase::CalculatePrettyPath): STLport 4.6 deque::clear
 // (?clear@?$deque@..@std@@QAEXXZ), called out of line in retail (deque<CBezierCurve> 0x00913A20, landed). Declaration only.
 void clear();
#endif
#endif
};
#if defined(FABLE_RETAIL_STLP_DEQUE_POP_BACK_AUX_BODY) && defined(FABLE_RETAIL_STLP_DEQUE_BACK_POP_BODIES) && !defined(FABLE_STLPORT_DEQUE_PUSH_BACK)
// NF83 per-TU opt-in (round 7, row 0x0059AD35, the out-of-line deque<NUISystem::CComponent*> COMDAT itself): STLport 4.6 _deque.c
// _M_pop_back_aux: free the finish buffer, step the finish iterator to the previous node, point it at that buffer's last element
// and destroy it (trivial for a pointer element).
template<class T,class A> void deque<T,A>::_M_pop_back_aux(){
 this->_M_map_size.deallocate(this->_M_finish._M_first,this->buffer_size());
 this->_M_finish._M_set_node(this->_M_finish._M_node-1);
 this->_M_finish._M_cur=this->_M_finish._M_last-1;
 _Destroy(this->_M_finish._M_cur);
}
#endif
#if defined(FABLE_STLPORT_DEQUE_PUSH_BACK) && defined(FABLE_STLPORT_DEQUE_PUSH_BACK_AUX_NOINLINE)
// Per-TU opt-in (chunk Q6, row 0x00912840 CAIStateGroup_FlyingThingBase::AddRandomBezierCurveToPath): STLport 4.6 _deque.c
// _M_push_back_aux_v (exceptions off), body visible but kept out of line, as the retail TU calls it (deque<CBezierCurve> 0x00913D60):
// c2 then knows push_back keeps no address of the value (the 12-dword software-pipelined curve copy at 0x00912B19).
#pragma auto_inline(off)
template<class T,class A> void deque<T,A>::_M_push_back_aux_v(const value_type& t){
 value_type t_copy=t;
 _M_reserve_map_at_back();
 *(this->_M_finish._M_node+1)=this->_M_map_size.allocate(this->buffer_size());
 _Construct(this->_M_finish._M_cur,t_copy);
 this->_M_finish._M_set_node(this->_M_finish._M_node+1);
 this->_M_finish._M_cur=this->_M_finish._M_first;
}
#pragma auto_inline(on)
#endif
#ifdef FABLE_RETAIL_STLP_QUEUE
// Opt-in (wf13, row 0x00492973 CSubtitleRenderer::SetText): STLport 4.6 _queue.h queue (the adaptor over its protected
// sequence c), push only: `void push(const value_type& __x) { c.push_back(__x); }`.
template<class T,class Sequence=deque<T> >class queue {
public:
#ifdef FABLE_RETAIL_DISPLAYNEXTMESSAGE_PASS30
 typename Sequence::reference front(){ return c.front(); }
#endif
 typedef typename Sequence::value_type value_type;
#ifdef FABLE_RETAIL_STLP_DEQUE_PASS22_POP
 bool empty()const{return c.empty();}
 void pop(){c.pop_front();}
#endif
 void push(const value_type& x){c.push_back(x);}
protected:
 Sequence c;
};
#endif
#ifdef FABLE_RETAIL_STLP_STACK_TOP_POP
// NF76 per-TU opt-in (row 0x00599E3F): STLport 4.6 _stack.h top() / pop() (`return c.back();` / `c.pop_back();`), inline.
template<class T,class Sequence=deque<T> >class stack {protected:Sequence c;public:stack():c(){}explicit stack(const Sequence& s):c(s){}
 typename Sequence::reference top(){return c.back();}
 void pop(){c.pop_back();}
#ifdef FABLE_RETAIL_STLP_STACK_SIZE_PUSH
 // NF79 nested opt-in (row 0x00596763): STLport 4.6 _stack.h size() / push() (`return c.size();` / `c.push_back(x);`), inline;
 // push_back itself comes from FABLE_RETAIL_STLP_DEQUE_PUSH_BACK_OUTOFLINE (retail 0x0059B61C, out of line).
 typename Sequence::size_type size()const{return c.size();}
 void push(const typename Sequence::value_type& x){c.push_back(x);}
#endif
#ifdef FABLE_RETAIL_STLP_STACK_EMPTY
 // NF86 nested opt-in (round 7, q11112 / q11113 rows 0x0059686D / 0x00597B20): STLport 4.6 _stack.h `bool empty() const { return c.empty(); }`,
 // inline (deque::empty compares _M_finish._M_cur with _M_start._M_cur in place). Body only.
 bool empty()const{return c.empty();}
#endif
};
#else
template<class T,class Sequence=deque<T> >class stack {protected:Sequence c;public:stack():c(){}explicit stack(const Sequence& s):c(s){}};
#endif
}

#endif
