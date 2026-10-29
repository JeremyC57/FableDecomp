// std::vector<CSubtitledMessage>::_M_fill_insert -- complete original value68 and actual STLport helper bodies
// Leaf: ?_M_fill_insert@?$vector@VCSubtitledMessage@@V?$allocator@VCSubtitledMessage@@@std@@@std@@QAEXPAVCSubtitledMessage@@IABV3@@Z
// Flags:
#include <stddef.h>
#include <new>
#include "rebuild_abi.h"
extern "C" void*__cdecl malloc(unsigned int);
extern "C" void __cdecl free(void*);
class CCharStringData;class CCharString {public:CCharStringData*PStringData;CCharString();CCharString(const CCharString&);~CCharString();};
class CWideStringData;class CWideString {public:CWideStringData*PStringData;CWideString();CWideString(const CWideString&);~CWideString();};
namespace std {
typedef ::size_t size_t;typedef ::ptrdiff_t ptrdiff_t;
struct __true_type {};struct __false_type {};
template<class T>struct __type_traits {typedef __true_type this_dummy_member_must_be_first;typedef __false_type has_trivial_default_constructor;typedef __false_type has_trivial_copy_constructor;typedef __false_type has_trivial_assignment_operator;typedef __false_type has_trivial_destructor;typedef __false_type is_POD_type;};
template<class T>class allocator {public:typedef T value_type;typedef T*pointer;typedef const T*const_pointer;typedef T&reference;typedef const T&const_reference;typedef size_t size_type;typedef ptrdiff_t difference_type;template<class U>struct rebind {typedef allocator<U>other;};allocator(){}allocator(const allocator&){}template<class U>allocator(const allocator<U>&){}~allocator(){}T*allocate(size_type n,const void* =0){return n?reinterpret_cast<T*>(malloc(n*sizeof(T))):0;}void deallocate(pointer p,size_type){if(p)free(p);}};
template<class T,class A>struct _Alloc_traits {typedef A _Orig;typedef A allocator_type;};
template<class T,class U>struct _Alloc_traits<T,allocator<U> > {typedef allocator<U>_Orig;typedef allocator<T>allocator_type;};
template<class V,class T,class A>class _STLP_alloc_proxy:public A {typedef A _Base;typedef _STLP_alloc_proxy<V,T,A>_Self;public:V _M_data;_STLP_alloc_proxy(const A&a,V p):A(a),_M_data(p){}T*allocate(size_t n){return static_cast<_Base&>(*this).allocate(n,0);}void deallocate(T*p,size_t n){static_cast<_Base&>(*this).deallocate(p,n);}};
template<class T>inline void __fastcall _Destroy(T*p){p->~T();}
template<class F>inline void __fastcall __destroy_aux(F first,F last,const __false_type&){for(;first!=last;++first)_Destroy(&*first);}
template<class F>inline void __fastcall __destroy_aux(F,F,const __true_type&){}
template<class F,class T>inline void __fastcall __destroy(F first,F last,T*){typedef typename __type_traits<T>::has_trivial_destructor _Trivial_destructor;__destroy_aux(first,last,_Trivial_destructor());}
template<class T>inline void __fastcall _Destroy(T*first,T*last){__destroy(first,last,first);}
template<class T,class U>inline void __fastcall _Construct(T*p,const U&v){::new(static_cast<void*>(p))T(v);}
#pragma auto_inline(off)
template<class I,class F>__declspec(noinline) F __fastcall __uninitialized_copy(I first,I last,F result,const __false_type&){F cur=result;for(;first!=last;++first,++cur)_Construct(&*cur,*first);return cur;}
#pragma auto_inline(on)

struct input_iterator_tag {};struct forward_iterator_tag:public input_iterator_tag {};struct bidirectional_iterator_tag:public forward_iterator_tag {};
struct _List_node_base {_List_node_base*_M_next,*_M_prev;};
template<class T>struct _List_node:public _List_node_base {T _M_data;};
struct _List_iterator_base {_List_node_base*_M_node;_List_iterator_base(_List_node_base*n):_M_node(n){}_List_iterator_base(){}void _M_incr(){_M_node=_M_node->_M_next;}void _M_decr(){_M_node=_M_node->_M_prev;}bool operator==(const _List_iterator_base&x)const{return _M_node==x._M_node;}bool operator!=(const _List_iterator_base&x)const{return _M_node!=x._M_node;}};
template<class T>struct _Nonconst_traits {typedef T value_type;typedef T*pointer;typedef T&reference;};template<class T>struct _Const_traits {typedef T value_type;typedef const T*pointer;typedef const T&reference;typedef _Nonconst_traits<T>_Non_const_traits;};
template<class T,class Traits>struct _List_iterator:public _List_iterator_base {typedef T value_type;typedef typename Traits::pointer pointer;typedef typename Traits::reference reference;typedef _List_iterator<T,_Nonconst_traits<T> >iterator;typedef _List_iterator<T,_Const_traits<T> >const_iterator;typedef _List_iterator<T,Traits>_Self;typedef bidirectional_iterator_tag iterator_category;typedef _List_node<T>_Node;typedef size_t size_type;typedef ptrdiff_t difference_type;_List_iterator(_Node*n):_List_iterator_base(n){}_List_iterator(){}_List_iterator(const iterator&x):_List_iterator_base(x._M_node){}reference operator*()const{return static_cast<_Node*>(_M_node)->_M_data;}_Self&operator++(){this->_M_incr();return *this;}};
template<class T>struct _Is_integer {typedef __false_type _Integral;};
template<class T,class A>class _List_base {protected:typedef _List_node<T>_Node;typedef typename _Alloc_traits<_Node,A>::allocator_type _Node_allocator_type;public:typedef typename _Alloc_traits<T,A>::allocator_type allocator_type;allocator_type get_allocator()const{return allocator_type(static_cast<const _Node_allocator_type&>(_M_node));}_List_base(const allocator_type&a):_M_node(_Node_allocator_type(a),static_cast<_Node*>(0)){_Node*n=_M_node.allocate(1);n->_M_next=n;n->_M_prev=n;_M_node._M_data=n;}~_List_base(){clear();_M_node.deallocate(_M_node._M_data,1);}void clear(){_Node*cur=static_cast<_Node*>(_M_node._M_data->_M_next);while(cur!=_M_node._M_data){_Node*tmp=cur;cur=static_cast<_Node*>(cur->_M_next);_Destroy(&tmp->_M_data);_M_node.deallocate(tmp,1);}_M_node._M_data->_M_next=_M_node._M_data;_M_node._M_data->_M_prev=_M_node._M_data;}public:_STLP_alloc_proxy<_Node*,_Node,_Node_allocator_type>_M_node;};
template<class T,class A=allocator<T> >class list:public _List_base<T,A> {typedef _List_base<T,A>_Base;typedef list<T,A>_Self;public:typedef T value_type;typedef T*pointer;typedef const T*const_pointer;typedef T&reference;typedef const T&const_reference;typedef _List_node<T>_Node;typedef size_t size_type;typedef ptrdiff_t difference_type;typedef typename _Base::allocator_type allocator_type;typedef _List_iterator<T,_Nonconst_traits<T> >iterator;typedef _List_iterator<T,_Const_traits<T> >const_iterator;protected:_Node*_M_create_node(const T&x){_Node*p=this->_M_node.allocate(1);_Construct(&p->_M_data,x);return p;}public:explicit list(const allocator_type&a=allocator_type()):_List_base<T,A>(a){}list(const list&x):_List_base<T,A>(x.get_allocator()){insert(begin(),x.begin(),x.end());}~list(){}iterator begin(){return iterator(static_cast<_Node*>(this->_M_node._M_data->_M_next));}const_iterator begin()const{return const_iterator(static_cast<_Node*>(this->_M_node._M_data->_M_next));}iterator end(){return iterator(this->_M_node._M_data);}const_iterator end()const{return const_iterator(this->_M_node._M_data);}bool empty()const{return this->_M_node._M_data->_M_next==this->_M_node._M_data;}iterator insert(iterator position,const T&x){_Node*tmp=_M_create_node(x);_List_node_base*n=position._M_node,*p=n->_M_prev;tmp->_M_next=n;tmp->_M_prev=p;p->_M_next=tmp;n->_M_prev=tmp;return tmp;}template<class I>void insert(iterator pos,I first,I last){typedef typename _Is_integer<I>::_Integral _Integral;_M_insert_dispatch(pos,first,last,_Integral());}template<class I>void _M_insert_dispatch(iterator pos,I first,I last,const __false_type&){for(;first!=last;++first)insert(pos,*first);}void push_back(const T&x){insert(end(),x);}};


template<class T>inline const T&__fastcall max(const T&a,const T&b){return a<b?b:a;}
struct random_access_iterator_tag:public bidirectional_iterator_tag {};
template<class I>struct iterator_traits {typedef typename I::iterator_category iterator_category;typedef typename I::difference_type difference_type;typedef typename I::value_type value_type;typedef typename I::pointer pointer;typedef typename I::reference reference;};
template<class T>struct iterator_traits<T*> {typedef random_access_iterator_tag iterator_category;typedef ptrdiff_t difference_type;typedef T value_type;typedef T*pointer;typedef T&reference;};
template<class I,class B,class D>__declspec(noinline) B __fastcall __copy_backward(I first,I last,B result,const random_access_iterator_tag&,D*){for(D n=last-first;n>0;--n)*--result=*--last;return result;}
template<class I,class O>inline O __fastcall __copy_backward_ptrs(I first,I last,O result,const __false_type&){return __copy_backward(first,last,result,typename iterator_traits<I>::iterator_category(),static_cast<typename iterator_traits<I>::difference_type*>(0));}
template<class F,class T>__declspec(noinline) void __fastcall fill(F first,F last,const T&v){for(;first!=last;++first)*first=v;}
template<class F,class S,class T>__forceinline F __fastcall __uninitialized_fill_n(F first,S n,const T&v,const __false_type&){F cur=first;for(;n>0;--n,++cur)_Construct(&*cur,v);return cur;}
template<class F,class S,class T>__declspec(noinline) F __fastcall uninitialized_fill_n(F first,S n,const T&v){return __uninitialized_fill_n(first,n,v,typename __type_traits<T>::is_POD_type());}

template<class T,class A>class _Vector_base {public:typedef typename _Alloc_traits<T,A>::allocator_type allocator_type;_Vector_base(const A&a):_M_start(0),_M_finish(0),_M_end_of_storage(a,0){}_Vector_base(size_t n,const A&a):_M_start(0),_M_finish(0),_M_end_of_storage(a,0){_M_start=_M_end_of_storage.allocate(n);_M_finish=_M_start;_M_end_of_storage._M_data=_M_start+n;}~_Vector_base(){if(_M_start)_M_end_of_storage.deallocate(_M_start,_M_end_of_storage._M_data-_M_start);}protected:T*_M_start;T*_M_finish;_STLP_alloc_proxy<T*,T,allocator_type>_M_end_of_storage;};
template<class T,class A=allocator<T> >class vector:public _Vector_base<T,A> {typedef _Vector_base<T,A>_Base;public:typedef T value_type;typedef T*pointer;typedef const T*const_pointer;typedef T*iterator;typedef const T*const_iterator;typedef T&reference;typedef const T&const_reference;typedef size_t size_type;typedef ptrdiff_t difference_type;typedef typename _Vector_base<T,A>::allocator_type allocator_type;protected:typedef typename __type_traits<T>::has_trivial_assignment_operator _TrivialAss;typedef typename __type_traits<T>::has_trivial_assignment_operator _IsPODType;public:allocator_type get_allocator()const{return static_cast<const allocator_type&>(this->_M_end_of_storage);}size_type size()const{return size_type(this->_M_finish-this->_M_start);}iterator begin(){return this->_M_start;}const_iterator begin()const{return this->_M_start;}iterator end(){return this->_M_finish;}const_iterator end()const{return this->_M_finish;}explicit vector(const allocator_type&a=allocator_type()):_Vector_base<T,A>(a){}vector(const vector&x):_Vector_base<T,A>(x.size(),x.get_allocator()){this->_M_finish=__uninitialized_copy(static_cast<const_pointer>(x._M_start),static_cast<const_pointer>(x._M_finish),this->_M_start,_IsPODType());}~vector(){_Destroy(this->_M_start,this->_M_finish);}void _M_fill_insert(iterator,size_type,const T&);protected:__declspec(noinline) void _M_insert_overflow(pointer,const T&,const __false_type&,size_type,bool=false);void _M_clear(){_Destroy(this->_M_start,this->_M_finish);this->_M_end_of_storage.deallocate(this->_M_start,this->_M_end_of_storage._M_data-this->_M_start);}void _M_set(pointer s,pointer f,pointer e){this->_M_start=s;this->_M_finish=f;this->_M_end_of_storage._M_data=e;}};
template<class T,class A>void vector<T,A>::_M_insert_overflow(pointer position,const T&x,const __false_type&,size_type fill_len,bool atend){const size_type old_size=size();const size_type len=old_size+(max)(old_size,fill_len);pointer new_start=this->_M_end_of_storage.allocate(len);pointer new_finish=new_start;new_finish=__uninitialized_copy(this->_M_start,position,new_start,__false_type());if(fill_len==1){_Construct(new_finish,x);++new_finish;}else new_finish=__uninitialized_fill_n(new_finish,fill_len,x,__false_type());if(!atend)new_finish=__uninitialized_copy(position,this->_M_finish,new_finish,__false_type());_M_clear();_M_set(new_start,new_finish,new_start+len);}
template <class _Tp, class _Alloc>
void 
vector<_Tp, _Alloc>::_M_fill_insert(
				    iterator __position, 
				    size_type __n, const _Tp& __x) {
  if (__n != 0) {
    if (size_type(this->_M_end_of_storage._M_data - this->_M_finish) >= __n) {
      _Tp __x_copy = __x;
      const size_type __elems_after = this->_M_finish - __position;
      pointer __old_finish = this->_M_finish;
      if (__elems_after > __n) {
        __uninitialized_copy(this->_M_finish - __n, this->_M_finish, this->_M_finish, _IsPODType());
        this->_M_finish += __n;
        __copy_backward_ptrs(__position, __old_finish - __n, __old_finish, _TrivialAss());
        std::fill(__position, __position + __n, __x_copy);
      }
      else {
        uninitialized_fill_n(this->_M_finish, __n - __elems_after, __x_copy);
        this->_M_finish += __n - __elems_after;
        __uninitialized_copy(__position, __old_finish, this->_M_finish, _IsPODType());
        this->_M_finish += __elems_after;
        std::fill(__position, __old_finish, __x_copy);
      }
    }
    else 
      _M_insert_overflow(__position, __x, _IsPODType(), __n);
  }
}

}

namespace std {
template<class A,class B>struct pair {typedef A first_type;typedef B second_type;A first;B second;pair():first(A()),second(B()){}pair(const A&a,const B&b):first(a),second(b){}};
}

class CASoundBank;
enum EConversationAttitude {};enum EConversationProtagonist {};enum EConversationCameraPosition {};enum EConversationCameraFocus {};enum EConversationCameraZoomType {};enum EConversationCameraView {};
template<class T>class CArray:public std::vector<T> {};
template<class K,class V>class CKeyPairCompareLess {};
template<class K,class V,class C=CKeyPairCompareLess<K,V> >class CVectorMap:public CArray<std::pair<K,V> > {public:C Compare;bool Dirty;};
class CSubtitledMessage {public:class CCameraTag {public:EConversationProtagonist CameraPositionPerson;EConversationCameraPosition CameraPosition;EConversationProtagonist CameraFocusPerson;EConversationCameraFocus CameraFocusPosition;EConversationCameraZoomType CameraZoomType;EConversationCameraView CameraView;};CASoundBank*PSpeechBank;long SpeechIndex;bool Speech3D;CWideString Text;CCharString Narrator;CVectorMap<float,EConversationAttitude>AttitudeTags;CVectorMap<float,CCharString>AnimationTags;CVectorMap<float,CCameraTag>CameraTags;CSubtitledMessage();CSubtitledMessage(const CSubtitledMessage&);~CSubtitledMessage();CSubtitledMessage&operator=(const CSubtitledMessage&);};
FABLE_STATIC_ASSERT(sizeof(CSubtitledMessage)==68);
FABLE_STATIC_ASSERT(sizeof(CSubtitledMessage::CCameraTag)==24);
FABLE_STATIC_ASSERT(offsetof(CSubtitledMessage,AttitudeTags)==20);
FABLE_STATIC_ASSERT(offsetof(CSubtitledMessage,CameraTags)==52);
namespace std {template void vector<CSubtitledMessage>::_M_fill_insert(CSubtitledMessage*,unsigned int,const CSubtitledMessage&);}