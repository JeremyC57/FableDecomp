// 00af56e0 NParticleEngine::CParticleSystem::DeleteAllParticles
// Codex parked-50 best attempt: DIFFER(138v138); residue: 138/138 bytes; only the final iterator store uses EBX instead of retail EDI.
// Leaf: ?DeleteAllParticles@CParticleSystem@NParticleEngine@@QAEXXZ
// Flags:
#include <stdlib.h>
#include "rebuild_abi.h"
// Layout: quarantined generated header used only for physicalextent comparison;
// original classkey, namespace, access and actual base come from raw TPI.
#define CParticleSystem CParticleSystemFields
#include "engine/_quarantine/CParticleSystem.h"
#undef CParticleSystem
#include "engine/CBaseClassNonCopyable.h"
namespace NParticleEngine{class CParticle:public CBaseClassNonCopyable{public:virtual ~CParticle();};}
namespace std{
// Layout: STLport4.6.2 _construct.h50..62 and _type_traits.h pointer specialization.
struct __true_type {};
struct __false_type {};
template<class T>struct __type_traits {typedef __false_type has_trivial_destructor;};
template<class T>struct __type_traits<T*> {typedef __true_type has_trivial_destructor;};
template<class T>inline void __destroy_aux(T*p,const __false_type&){p->~T();}
template<class T>inline void __destroy_aux(T*,const __true_type&){}
template<class T>inline void _Destroy(T*p){typedef typename __type_traits<T>::has_trivial_destructor Tag;__destroy_aux(p,Tag());}

template<class T>class allocator{public:allocator(){}T*allocate(unsigned int count){return (T*)malloc(count*sizeof(T));}void deallocate(T*p,unsigned int){free(p);}};
template<class P,class T,class A>class _STLP_alloc_proxy:public A{public:P _M_data;_STLP_alloc_proxy(const A&a,P p):A(a),_M_data(p){}};
struct _List_node_base{_List_node_base*_M_next,*_M_prev;};template<class T>struct _List_node:public _List_node_base{T _M_data;};
struct _List_iterator_base{_List_node_base*_M_node;_List_iterator_base(){}_List_iterator_base(_List_node_base*p):_M_node(p){}bool operator!=(const _List_iterator_base&other)const{return _M_node!=other._M_node;}};
template<class T>struct _Nonconst_traits{typedef T&reference;typedef T*pointer;};template<class T>struct _Const_traits{typedef const T&reference;typedef const T*pointer;};
template<class T,class Traits>struct _List_iterator:public _List_iterator_base{typedef _List_iterator<T,_Nonconst_traits<T> >iterator;_List_iterator(){}_List_iterator(_List_node<T>*p):_List_iterator_base(p){}_List_iterator(const iterator&other):_List_iterator_base(other._M_node){}typename Traits::reference operator*()const{return static_cast<_List_node<T>*>(this->_M_node)->_M_data;}};
template<class T,class A>class _List_base{public:_STLP_alloc_proxy<_List_node<T>*,_List_node<T>,allocator<_List_node<T> > >_M_node;_List_base(const A&a):_M_node(allocator<_List_node<T> >(),0){(void)a;_List_node<T>*node=_M_node.allocate(1);node->_M_next=node;node->_M_prev=node;_M_node._M_data=node;}~_List_base(){clear();_M_node.deallocate(_M_node._M_data,1);}void clear(){_List_node<T>*node=static_cast<_List_node<T>*>(_M_node._M_data->_M_next);while(node!=_M_node._M_data){_List_node<T>*old=node;node=static_cast<_List_node<T>*>(node->_M_next);_Destroy(&old->_M_data);_M_node.deallocate(old,1);}_M_node._M_data->_M_next=_M_node._M_data;_M_node._M_data->_M_prev=_M_node._M_data;}};
template<class T,class A=allocator<T> >class list:public _List_base<T,A>{public:typedef _List_iterator<T,_Nonconst_traits<T> >iterator;list():_List_base<T,A>(A()){}iterator begin(){return iterator(static_cast<_List_node<T>*>(this->_M_node._M_data->_M_next));}iterator end(){return iterator(this->_M_node._M_data);}iterator erase(iterator position){_List_node_base*next=position._M_node->_M_next;_List_node_base*previous=position._M_node->_M_prev;_List_node<T>*node=static_cast<_List_node<T>*>(position._M_node);previous->_M_next=next;next->_M_prev=previous;_Destroy(&node->_M_data);this->_M_node.deallocate(node,1);return iterator(static_cast<_List_node<T>*>(next));}};
}
namespace NParticleEngine{
// Layout: actual polymorphic CBaseClass4; original unread prefix24 to Particles28.
// Containers shrink from donor: Particles28 and NoActiveParticles72, native112extent.
class CParticleSystem:public CBaseClass{unsigned char unreadPrefix[24];protected:std::list<CParticle*>Particles;private:unsigned char unreadMiddle[40];protected:long NoActiveParticles;private:unsigned char unreadTail[36];
#ifdef PASS9_TEST
friend int ::main();
#endif
public:void ClearParticle(std::list<CParticle*>::iterator&);void DeleteAllParticles();};
void CParticleSystem::DeleteAllParticles(){std::list<CParticle*>::iterator it=Particles.begin();std::list<CParticle*>::iterator current=it;while(it!=Particles.end()){ClearParticle(current);delete*it;it=Particles.erase(it);--NoActiveParticles;current=it;}Particles.clear();NoActiveParticles=0;}
}
FABLE_STATIC_ASSERT(sizeof(NParticleEngine::CParticleSystem)==sizeof(CParticleSystemFields));
