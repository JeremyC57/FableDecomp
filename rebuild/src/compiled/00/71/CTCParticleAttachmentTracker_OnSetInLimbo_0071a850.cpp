// 0071a850 CTCParticleAttachmentTracker::OnSetInLimbo
// Flags:
// Leaf: ?OnSetInLimbo@CTCParticleAttachmentTracker@@UAEX_N@Z
#define CTCBase Pass9TCBaseFields
#include "engine/CTCBase.h"
#undef CTCBase
#define CThing Pass9ThingFields
#define CTCGraphicAppearance Pass9GraphicsFields
#include "engine/_quarantine/CThing.h"
#undef CTCGraphicAppearance
#undef CThing
#ifndef _MSC_VER
#define __fastcall
#define __forceinline inline
#endif
class CBaseClass{public:virtual~CBaseClass();};
class CBaseClassNonCopyable:public CBaseClass{protected:virtual~CBaseClassNonCopyable();};
class CBaseObject:public CBaseClass{unsigned char pass9_unread_pointer[4];};
class CClass:public CBaseObject{unsigned char pass9_unread_class[12];};
// Layout: header/RTTI true ancestry; donor Dead bit161 maps toretail145.
class CThing:public CClass{unsigned char pass9_unread_thing[125];public:unsigned char Dead:1;__forceinline bool IsDead()const{return Dead!=0;}void SetInLimbo(bool);};
class CBaseIntelligentPointer{public:virtual~CBaseIntelligentPointer();
 // Layout: PData is an opaque four-byte pointer representation. Its pointee
 // is not named or modeled; only the original typed accessor is called here.
 unsigned char PData[4];CBaseObject*GetPItem()const;
};
template<class T>class CIntelligentPointer:public CBaseIntelligentPointer{public:__forceinline T*operator->()const{return static_cast<T*>(GetPItem());}};
template<class T>__forceinline bool __fastcall IS_THING_ALIVE(const T&p){return p.operator->()!=0&&!p->IsDead();}
namespace std{
template<class T>class allocator{};
template<class V,class T,class A>class _STLP_alloc_proxy:public A{public:V _M_data;};
struct _List_node_base{_List_node_base*_M_next,*_M_prev;};
template<class T>struct _List_node:public _List_node_base{T _M_data;};
struct _List_iterator_base{_List_node_base*_M_node;_List_iterator_base(_List_node_base*n):_M_node(n){};void _M_incr(){_M_node=_M_node->_M_next;}};
template<class T>struct _Nonconst_traits{typedef T&reference;};
template<class T,class Traits>struct _List_iterator:public _List_iterator_base{
 _List_iterator(_List_node_base*n):_List_iterator_base(n){};
 T&operator*()const{return static_cast<_List_node<T>*>(this->_M_node)->_M_data;}
 _List_iterator&operator++(){this->_M_incr();return *this;}
 bool operator!=(const _List_iterator&x)const{return this->_M_node!=x._M_node;}
};
template<class T,class A>class _List_base{public:_STLP_alloc_proxy<_List_node_base*,_List_node<T>,allocator<_List_node<T> > >_M_node;};
template<class T,class A=allocator<T> >class list:public _List_base<T,A>{public:
 typedef _List_iterator<T,_Nonconst_traits<T> >iterator;
 iterator begin(){return iterator(this->_M_node._M_data->_M_next);}
 iterator end(){return iterator(this->_M_node._M_data);}
};
}
class CTCBase:public CBaseClassNonCopyable{public:virtual~CTCBase();CThing*ParentThing;bool ToBeRemoved,IsInGlobalUpdateTCs;};
// Layout: original AttachedParticles at12; retail list occupies4 bytes.
class CTCParticleAttachmentTracker:public CTCBase{public:std::list<CIntelligentPointer<CThing> >AttachedParticles;virtual void OnSetInLimbo(bool);};
FABLE_STATIC_ASSERT(sizeof(CTCBase)==sizeof(Pass9TCBaseFields));
FABLE_STATIC_ASSERT(sizeof(CBaseIntelligentPointer)==8);
FABLE_STATIC_ASSERT(sizeof(CTCParticleAttachmentTracker)==16);
void CTCParticleAttachmentTracker::OnSetInLimbo(bool inLimbo){
 for(std::list<CIntelligentPointer<CThing> >::iterator i=AttachedParticles.begin();i!=AttachedParticles.end();++i)
  if(IS_THING_ALIVE(*i))(*i)->SetInLimbo(inLimbo);
}