// 007f9010 CTCAnimatingObject::PlayAnimation
// Flags:
// Leaf: ?PlayAnimation@CTCAnimatingObject@@QAEXJJ_N@Z
#include "engine/CBaseClassNonCopyable.h"
#define CTCBase CTCBaseFields
#include "engine/CTCBase.h"
#undef CTCBase
extern "C" void __cdecl free(void*);
class CThing;
enum EAnimUpdateMode {ANIM_UPDATE_NONE,ANIM_UPDATE_FROM_SPEED,ANIM_UPDATE_FROM_DIST,ANIM_UPDATE_PERCENTAGE_COMPLETE,ANIM_UPDATE_LOCKED_TO_MAIN};
// Layout: original CAnimInit44 and frame Current/Last float words at18/1c.
class CAnimFrameInfo {
public:
    float Current;
    float Last;
    CAnimFrameInfo() : Current(0.0f), Last(0.0f) {}
};
class CAnimInit {
public:
    EAnimUpdateMode UpdateMode;                 // +0x00
    unsigned char Type;                         // +0x04
    bool Looping;                               // +0x05
    long Anim;                                  // +0x08
    float Speed;                                // +0x0c
    float Alpha;                                // +0x10
    float LastAlpha;                            // +0x14
    CAnimFrameInfo Frame;                       // +0x18
    unsigned long Flags;                        // +0x20
    unsigned long TypeFlags;                    // +0x24
    long Priority;                              // +0x28
    CAnimInit(unsigned char type, unsigned long typeFlags, long anim, EAnimUpdateMode updateMode, bool looping, unsigned long flags, long priority)
    {
        Init(type, typeFlags, anim, updateMode, looping, flags, priority);
    }
    void Init(unsigned char type, unsigned long typeFlags, long anim, EAnimUpdateMode updateMode, bool looping, unsigned long flags, long priority);   // 0x0070C050 (Ego_r ?Init@CAnimInit@@QAEXEKJW4EAnimUpdateMode@@_NKJ@Z)
};


// STLport4.6 _list.h/_list.c and retail one-pointer list at14.
namespace std {
template<class T>class allocator{public:void deallocate(T*p,unsigned int){free(p);}};
template<class P,class T,class A>class _STLP_alloc_proxy:public A{public:P _M_data;};
struct _List_node_base{_List_node_base*_M_next;_List_node_base*_M_prev;};
template<class T>struct _List_node:public _List_node_base{T _M_data;};
template<class T>inline void __fastcall _Destroy(T*p){p->~T();}
template<class T,class A>class _List_base{
protected:_STLP_alloc_proxy<_List_node<T>*,_List_node<T>,allocator<_List_node<T> > > _M_node;
public:void clear(){
 _List_node<T>*cur=(_List_node<T>*)_M_node._M_data->_M_next;
 while(cur!=_M_node._M_data){_List_node<T>*tmp=cur;cur=(_List_node<T>*)cur->_M_next;_Destroy(&tmp->_M_data);_M_node.deallocate(tmp,1);}
 _M_node._M_data->_M_next=_M_node._M_data;_M_node._M_data->_M_prev=_M_node._M_data;
}
};
template<class T,class A=allocator<T> >class list:public _List_base<T,A>{};
}
// Layout: native shared base12, original protected reference ParentThing4.
class CTCBase:public CBaseClassNonCopyable{
protected:CThing&ParentThing;
private:bool ToBeRemoved,IsInGlobalUpdateTCs;
};
// Layout: original private Animating0c/Priority10/QueuedAnims14; native list4.
class CTCAnimatingObject:public CTCBase{
 bool Animating;long CurrentAnimationPriority;std::list<CAnimInit>QueuedAnims;
 void ClearQueuedAnims(){QueuedAnims.clear();}
 void PlayAnimation(const CAnimInit&,long);
public:void PlayAnimation(long,long,bool);
};
void CTCAnimatingObject::PlayAnimation(long anim, long priority, bool looping)
{
    if (!Animating || priority > CurrentAnimationPriority) {
        ClearQueuedAnims();
        CAnimInit init(6, 0, anim, ANIM_UPDATE_FROM_SPEED, looping, 0, 100);
        PlayAnimation(init, priority);
    }
}