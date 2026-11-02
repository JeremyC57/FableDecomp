#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <new>
#ifndef _MSC_VER
#define __fastcall
#define __declspec(x)
#endif
#define FABLE_FASTCALL __fastcall
#define FABLE_STATIC_ASSERT(x)
#define FABLE_RETAIL_DEF_CLASS_BASE
struct UIFixture;
#ifndef FABLE_ENGINE_CDEFPOINTER_H
#define FABLE_ENGINE_CDEFPOINTER_H























#ifndef FABLE_RETAIL_DEF_CLASS_BASE
#error engine/CDefPointer.h is the retail def model: define FABLE_RETAIL_DEF_CLASS_BASE before including it
#else

#ifndef FABLE_ENGINE_CDEFPOINTEEBASE_H
#define FABLE_ENGINE_CDEFPOINTEEBASE_H




#include <stddef.h>

#ifndef FABLE_ENGINE_CRESOURCE_H
#define FABLE_ENGINE_CRESOURCE_H







#ifdef FABLE_RETAIL_DEF_CLASS_BASE
#include <stddef.h>

#ifndef FABLE_ENGINE_CIVCOUNTEDPOINTEEBASE_H
#define FABLE_ENGINE_CIVCOUNTEDPOINTEEBASE_H















#ifdef FABLE_RETAIL_DEF_CLASS_BASE
#include <stddef.h>
class CIVCountedPointeeBase {
public:
#ifdef FABLE_RETAIL_CIVCOUNTEDPOINTEEBASE
    CIVCountedPointeeBase();
#endif
    virtual ~CIVCountedPointeeBase() { }                                 
    virtual void Delete() const;                                         
    void AddRef() const { ++IVRefCount; }                                
    long Release() const { return --IVRefCount; }                        
    long GetRefCount() const { return IVRefCount; }                      
#ifdef FABLE_RETAIL_CIVCOUNTEDPOINTEEBASE
private:
#endif
    mutable unsigned long IVRefCount;                                    
};
#else 
#include <stddef.h>


#pragma pack(push, 1)
class CIVCountedPointeeBase {
public:
    void*         __vftable;                                 
    unsigned long IVRefCount;                                
};
#pragma pack(pop)


#endif 

#endif 

class CResourceList;
class CResource : public CIVCountedPointeeBase {
public:
    CResource();                                                         
    virtual ~CResource();                                                
    virtual bool UnloadResource();                                       
    virtual unsigned long GetUnloadableMemory() const;                   
    virtual bool IsResourceLoaded() const;                               
    virtual void OnResourceLoaded();                                     
    virtual bool CanAbortResourceLoad(long) const;                       
    virtual void OnResourceLoadAborted();                                
    virtual unsigned long GetDebugMemoryInUse() const;                   
    virtual void ResetDebugMemoryInUse();                                
    CResourceList* ResourceList;                                         
    CResource*     PrevResource;                                         
    CResource*     NextResource;                                         
    unsigned long  ResourceSize;                                         
    unsigned long  LastUsedFrame;                                        
};

#endif
#endif


class CDefPointeeBase : public CResource {
public:
    virtual ~CDefPointeeBase() { }                                       
};

#endif 

#if defined(_MSC_VER)
#define FABLE_DEFPTR_INLINE __forceinline
#else
#define FABLE_DEFPTR_INLINE inline
#endif

enum ENullDef { NULL_DEF };                                          

template <class T> class CDefPointer {
public:
    FABLE_DEFPTR_INLINE CDefPointer(ENullDef = NULL_DEF) : Object(0) {}             
    FABLE_DEFPTR_INLINE CDefPointer(const CDefPointer& other) : Object(other.Object) { if (Object) Object->AddRef(); }
    template <class U> FABLE_DEFPTR_INLINE CDefPointer(const CDefPointer<U>& other) : Object(other.InternalGetObject())
    {
        if (Object) Object->AddRef();
    }
    FABLE_DEFPTR_INLINE ~CDefPointer() { Release(); }
    FABLE_DEFPTR_INLINE CDefPointer& operator=(const CDefPointer& other) { Assign(other.Object); return *this; }
    template <class U> FABLE_DEFPTR_INLINE CDefPointer& operator=(const CDefPointer<U>& other) { Assign(other.InternalGetObject()); return *this; }
    template <class U> FABLE_DEFPTR_INLINE void SetDynamicCast(const CDefPointer<U>& other) { Assign(other.InternalGetObject()); }
    CDefPointeeBase* InternalGetObject() const { return Object; }
    T* Get() const { return static_cast<T*>(Object); }
    T* operator->() const { return Get(); }
    T& operator*() const { return *Get(); }
    bool IsNull() const { return Object == 0; }
    unsigned long GetRefCount() const { return Object->GetRefCount(); }
    void Reset(T*);                                                      
    FABLE_DEFPTR_INLINE void Release()
    {
        if (Object) {
            CDefPointeeBase* object = Object;
            if (object->Release() == 0)
                object->Delete();
            Object = 0;
        }
    }
    CDefPointeeBase* Object;                                             
private:
    FABLE_DEFPTR_INLINE void Assign(CDefPointeeBase* object)                                 
    {
        if (Object != object) {
            Release();
            Object = object;
            if (Object) Object->AddRef();
        }
    }
};

#endif 
#endif 



#define FABLE_RETAIL_C2DVECTOR_UI
#define FABLE_STLPORT_UI
#define FABLE_RETAIL_CCHARSTRING
#ifndef FABLE_ENGINE_NUISYSTEM_CCOMPONENT_H
#define FABLE_ENGINE_NUISYSTEM_CCOMPONENT_H

#ifndef FABLE_NUISYSTEM_ENUMS_H
#define FABLE_NUISYSTEM_ENUMS_H

namespace NUISystem {
enum EState {
    UI_STATE_INACTIVE=0,
    UI_STATE_ACTIVE=1,
    UI_STATE_DEAD=2,
    UI_STATE_SELECTED=3,
    UI_STATE_UNSELECTED=4,
    UI_STATE_LIST_ACTIVE=5,
    UI_STATE_LIST_INACTIVE=6,
    UI_STATE_LIST_UNSELECTED=7,
    UI_STATE_CUSTOM=8
};
enum EStateChangeType {
    STATE_CHANGE_SIMULTANEOUS=0,
    STATE_CHANGE_PARENT_FIRST=1,
    STATE_CHANGE_CHILDREN_FIRST=2,
    STATE_CHANGE_PARENT_ONLY=3,
    STATE_CHANGE_CHILDREN_ONLY=4
};
enum EType {
    UI_TYPE_SPRITE=0,
    UI_TYPE_MORPHING_SPRITE=1,
    UI_TYPE_TABLE=2,
    UI_TYPE_MESH=3,
    UI_TYPE_COMPOSITE=4,
    UI_TYPE_CHANGING_STATE_COMPOSITE=5,
    UI_TYPE_TEXT=6,
    UI_TYPE_MENU_ENTRY=7,
    UI_TYPE_LIST=8,
    UI_TYPE_VIEWPORT=9,
    UI_TYPE_FRONTEND_SCREEN=10,
    UI_TYPE_FRONTEND_BUTTON=11,
    UI_TYPE_FRONTEND_LIST=12,
    UI_TYPE_SCROLLING_VIEWPORT=13,
    UI_TYPE_LIST_ARROW=14,
    UI_TYPE_SLIDER=15,
    UI_TYPE_TEXT_SLIDER=16,
    UI_TYPE_MOVIE=17,
    UI_TYPE_SWAPPING_STATE_COMPOSITE=18,
    UI_TYPE_SCROLLING_COMPOSITE=19,
    UI_TYPE_TEXT_CONTAINER=20,
    UI_TYPE_ZOOMING_COMPOSITE=21,
    UI_TYPE_COMPONENT_CONTAINER=22,
    UI_TYPE_SPELL_CONTAINER=23,
    UI_TYPE_SPELL_CONTAINER_LIST=24,
    UI_TYPE_YESNO=25,
    UI_TYPE_OK=26,
    UI_TYPE_PARTICLE_EFFECT=27,
    UI_TYPE_CONTROLLERDISCONNECT=28,
    UI_TYPE_DIRTYDISC=29,
    UI_TYPE_ICON_TEXT=30,
    UI_TYPE_DYNAMIC_LIST=31,
    UI_TYPE_MOUSE_POINTER=32,
    UI_TYPE_HOVERABLE=33,
    UI_TYPE_CLICKABLE=34,
    UI_TYPE_DRAGGABLE=35,
    UI_TYPE_DRAGGABLE_INTO=36,
    UI_TYPE_EDIT_BOX=37,
    UI_TYPE_NAVIGATION_BUTTON=38,
    UI_TYPE_KEY_REDEFINER=39,
    UI_TYPE_REDEFINER_LIST=40,
    UI_TYPE_SCROLLBAR=41,
    UI_TYPE_SCROLLBAR_OUTSIDE=42,
    UI_TYPE_SCROLLABLE_LIST=43
};
enum EActionParamType {
    UI_ACTION_PARAM_TYPE_NONE=0,
    UI_ACTION_PARAM_TYPE_COMPONENT=1,
    UI_ACTION_PARAM_TYPE_STATE=2,
    UI_ACTION_PARAM_TYPE_SLONG=3,
    UI_ACTION_PARAM_TYPE_STRING=4,
    UI_ACTION_PARAM_TYPE_ULONG=5,
    UI_ACTION_PARAM_TYPE_FLOAT=6,
    UI_ACTION_PARAM_TYPE_DATE_AND_TIME=7,
    UI_ACTION_PARAM_TYPE_EBOOL=8,
    UI_ACTION_PARAM_TYPE_COUNTED_POINTER=9,
    UI_ACTION_PARAM_TYPE_2DVECTOR=10
};
enum EEvent {
    UI_EVENT_UP=0,
    UI_EVENT_DOWN=1,
    UI_EVENT_LEFT=2,
    UI_EVENT_RIGHT=3,
    UI_EVENT_SELECT=4,
    UI_EVENT_BACK=5,
    UI_EVENT_RIGHT_STICK_UP=6,
    UI_EVENT_RIGHT_STICK_DOWN=7,
    UI_EVENT_KEYBOARD_ARROWLEFT=8,
    UI_EVENT_KEYBOARD_ARROWRIGHT=9,
    UI_EVENT_KEYBOARD_SPACE=10,
    UI_EVENT_KEYBOARD_BACKSPACE=11,
    UI_EVENT_ZOOM_IN=12,
    UI_EVENT_ZOOM_OUT=13,
    UI_EVENT_SPELL_ROTATE_LEFT=14,
    UI_EVENT_SPELL_ROTATE_RIGHT=15,
    UI_EVENT_SPELL_LIST_UP=16,
    UI_EVENT_SPELL_LIST_DOWN=17,
    UI_EVENT_SPELL_ASSIGN=18,
    UI_EVENT_SPELL_UNASSIGN=19,
    UI_EVENT_SLIDER_LEFT=20,
    UI_EVENT_SLIDER_RIGHT=21,
    UI_EVENT_DELETE=22,
    UI_EVENT_X=23,
    UI_EVENT_Y=24,
    UI_EVENT_MOUSE_MOVEMENT=25,
    UI_EVENT_LEFT_MOUSE_CLICK=26,
    UI_EVENT_RIGHT_MOUSE_CLICK=27,
    UI_EVENT_LEFT_MOUSE_RELEASE=28,
    UI_EVENT_RIGHT_MOUSE_RELEASE=29,
    UI_EVENT_COMPONENT_DROPPED=30,
    UI_EVENT_LEFT_MOUSE_HELD=31,
    UI_EVENT_RIGHT_MOUSE_HELD=32,
    UI_EVENT_KEY_DOWN=33,
    UI_EVENT_CHAR_DOWN=34,
    UI_EVENT_MIDDLE_MOUSE_CLICK=35,
    UI_EVENT_MOUSE_WHEEL_UP=36,
    UI_EVENT_MOUSE_WHEEL_DOWN=37,
    UI_EVENT_MOUSE_BUTTON_4_CLICK=38,
    UI_EVENT_MOUSE_BUTTON_5_CLICK=39,
    UI_EVENT_MOUSE_BUTTON_6_CLICK=40,
    UI_EVENT_MOUSE_BUTTON_7_CLICK=41,
    UI_EVENT_MOUSE_BUTTON_8_CLICK=42
};
}
#endif

#ifndef FABLE_ENGINE_C2DVECTOR_H
#define FABLE_ENGINE_C2DVECTOR_H


#ifdef FABLE_RETAIL_C2DVECTOR_UI


class C2DVector {
public:
    float X,Y;
    C2DVector() { }
    C2DVector(float x,float y) : X(x),Y(y) { }
    C2DVector(const C2DVector& p) : X(p.X),Y(p.Y) { }
    C2DVector operator+(const C2DVector& vector) const { return C2DVector(X+vector.X,Y+vector.Y); }
    C2DVector operator-(const C2DVector& vector) const { return C2DVector(X-vector.X,Y-vector.Y); }
};
#else
class C2DVector { public: float X,Y; };
#endif
#endif

#ifndef FABLE_ENGINE_CCHARSTRING_H
#define FABLE_ENGINE_CCHARSTRING_H


class CCharStringData;
class CCharString {
public:
    CCharString();
    CCharString(const char* text, long length = -1);
    CCharString(const CCharString& other);
    ~CCharString();
protected:
    CCharStringData* PStringData;
};
#endif

#ifndef FABLE_ENGINE_CCOUNTEDPOINTER_H
#define FABLE_ENGINE_CCOUNTEDPOINTER_H











































class CCPPointerInfo {
public:
    unsigned long RefCount;                                              
    void (FABLE_FASTCALL* DeleteFunc)(void*);                             
    void* Data;                                                          
    CCPPointerInfo(unsigned long refCount, void (FABLE_FASTCALL* deleteFunc)(void*), void* data)
        : RefCount(refCount), DeleteFunc(deleteFunc), Data(data) { }     
};

template <class T> class CCountedPointer {
public:
    T* Data;                                                             
    CCPPointerInfo* Info;                                                

    CCountedPointer() : Data(0), Info(0) { }
    CCountedPointer(T* data) : Data(data)
    {
        if (data)
            Info = new CCPPointerInfo(1, DeleteData, (void*)Data);
        else
            Info = 0;
    }
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info)
    {
        if (Info)
            ++Info->RefCount;
    }
    template <class U> CCountedPointer(const CCountedPointer<U>& other) : Data(other.Data), Info(other.Info)
    {
        if (Info)
            ++Info->RefCount;
    }
    ~CCountedPointer()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
        Data = 0;
        Info = 0;
    }
    CCountedPointer& operator=(const CCountedPointer& other)
    {
        ShareData(other.Data, other.Info);
        return *this;
    }
    template <class U> CCountedPointer& operator=(const CCountedPointer<U>& other)
    {
        ShareData(other.Data, other.Info);
        return *this;
    }
    T* operator->() const { return Data; }
    T& operator*() const { return *Data; }
    T* Get() const { return Data; }
    T* get() const { return Data; }
    bool IsNull() const { return Data == 0; }
    unsigned long GetReferenceCount() const { return Info ? Info->RefCount : 0; }   
    void Validate() const;                                               
    void ReleaseData()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
        Info = 0;
        Data = 0;
    }
    void ShareData(T* data, CCPPointerInfo* info)
    {
        if (Info != info) {
            ReleaseData();
            Data = data;
            Info = info;
            if (info)
                ++info->RefCount;
        }
    }
    void Reset(T* data)
    {
        ReleaseData();
        Data = data;
        if (data)
            Info = new CCPPointerInfo(1, DeleteData, (void*)Data);
    }
    template <class U> void SetStaticCast(const CCountedPointer<U>& other)   
    {
        ShareData(static_cast<T*>(other.Data), other.Info);              
    }
    void reset(T* data) { Reset(data); }                                 
    static void FABLE_FASTCALL DeleteData(void* data) { delete static_cast<T*>(data); }                    
};

#endif 

#ifndef FABLE_STLPORT_CONTAINERS_H
#define FABLE_STLPORT_CONTAINERS_H



#include <stddef.h>
#include <stdlib.h>
#ifdef FABLE_STLPORT_VISIBLE_TRUE_OVERFLOW
#include <string.h>
#endif
#if defined(FABLE_STLPORT_VISIBLE_PUSH_BACK) || defined(FABLE_STLPORT_HTTP)
#include <new>
#include <string.h>
#endif
namespace std {
struct __true_type { };
#ifdef FABLE_STLPORT_HTTP
struct __false_type { __false_type() { } };
struct input_iterator_tag { };
struct forward_iterator_tag : public input_iterator_tag { forward_iterator_tag() { } };
struct bidirectional_iterator_tag : public forward_iterator_tag { };
struct random_access_iterator_tag : public bidirectional_iterator_tag { };
template<class T> inline const T& (max)(const T& a,const T& b) { return a<b?b:a; }
template<class T> inline const T& (min)(const T& a,const T& b) { return b<a?b:a; }
template<class T,class U> inline void _Construct(T* p,const U& v) { new(p) T(v); }
template<class T> inline void _Destroy(T* p) { p->~T(); }
inline void _Destroy(char*,char*) { }
template<class I> inline void _Destroy(I first,I last) { for(;first!=last;++first) _Destroy(&*first); }
template<class I,class O> inline O __uninitialized_copy(I first,I last,O result,const __false_type&) {
    O cur=result;for(;first!=last;++first,++cur) _Construct(&*cur,*first);return cur;
}
template<class I,class N,class T> inline I __uninitialized_fill_n(I first,N n,const T& x,const __false_type&) {
    I cur=first;for(;n>0;--n,++cur) _Construct(&*cur,x);return cur;
}
#endif
#ifdef FABLE_STLPORT_HTTP
template<class T> struct __type_traits { typedef __false_type has_trivial_assignment_operator; };
template<> struct __type_traits<char> { typedef __true_type has_trivial_assignment_operator; };
inline void* __copy_trivial(const void* first,const void* last,void* result) {
    return last==first?result:(char*)memmove(result,first,(const char*)last-(const char*)first)+((const char*)last-(const char*)first);
}
template<class T> inline T* __uninitialized_copy(const T* first,const T* last,T* result,const __true_type&) {
    return (T*)__copy_trivial(first,last,result);
}
template<class I,class T> inline void fill(I first,I last,const T& x) { for(;first!=last;++first) *first=x; }
template<class I,class N,class T> inline I fill_n(I first,N n,const T& x) { for(;n>0;--n,++first) *first=x;return first; }
template<class T> inline T* __copy_backward_ptrs(const T* first,const T* last,T* result,const __true_type&) {
    ptrdiff_t n=last-first;return (T*)memmove(result-n,first,n*sizeof(T));
}
template<class T> inline T* __copy_backward_ptrs(const T* first,const T* last,T* result,const __false_type&) {
    while(first!=last) *--result=*--last;return result;
}
#endif
template<class _Tp> class allocator {
public:
#ifdef FABLE_STLPORT_HTTP
    allocator() { }
    allocator(const allocator&) { }
    _Tp* allocate(size_t n,const void* =0) { return n?(_Tp*)malloc(n*sizeof(_Tp)):0; }
    void deallocate(_Tp* p,size_t) { if(p) free(p); }
#else
#ifdef FABLE_STLPORT_CONSTRUCTORS
    _Tp* allocate(size_t n,const void* =0) { return n?(_Tp*)malloc(n*sizeof(_Tp)):0; }
#endif
    void deallocate(_Tp* p, size_t) { free(p); }
#endif
};
template<class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& a, _Value p)
        : _MaybeReboundAlloc(a), _M_data(p) { }
};
template<class _Tp, class _Alloc> class _Vector_base {
    friend struct ::UIFixture;
    friend struct ::UIFixture;
public:
    _Vector_base(const _Alloc& a)
        : _M_start(0), _M_finish(0), _M_end_of_storage(a,0) { }
#ifdef FABLE_STLPORT_UI
    _Vector_base(size_t n,const _Alloc& a)
        : _M_start(0), _M_finish(0), _M_end_of_storage(a,0) {
        _M_start=n?(_Tp*)malloc(n*sizeof(_Tp)):0;
        _M_finish=_M_start;
        _M_end_of_storage._M_data=_M_start+n;
    }
#endif
    ~_Vector_base() {
        if (_M_start != 0)
            _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data-_M_start);
    }
protected:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*,_Tp,_Alloc> _M_end_of_storage;
};
template<class _Tp, class _Alloc=allocator<_Tp> >
class vector : public _Vector_base<_Tp,_Alloc> {
public:
    typedef _Tp* iterator;
    typedef const _Tp* const_iterator;
    typedef _Tp value_type;
    typedef size_t size_type;
    explicit vector(const _Alloc& a=_Alloc()) : _Vector_base<_Tp,_Alloc>(a) { }
#ifdef FABLE_STLPORT_UI
    vector(const vector&);
    _Alloc get_allocator() const { return _Alloc(this->_M_end_of_storage); }
#endif
#ifdef FABLE_STLPORT_HTTP
    ~vector() { _Destroy(this->_M_start,this->_M_finish); }
#endif
    iterator begin() { return this->_M_start; }
    const_iterator begin() const { return this->_M_start; }
    iterator end() { return this->_M_finish; }
    const_iterator end() const { return this->_M_finish; }
    size_type size() const { return size_type(this->_M_finish-this->_M_start); }
    bool empty() const { return this->_M_start==this->_M_finish; }
    _Tp& operator[](size_type n) { return *(begin()+n); }
    const _Tp& operator[](size_type n) const { return *(begin()+n); }
#ifdef FABLE_STLPORT_HTTP
    typedef typename __type_traits<_Tp>::has_trivial_assignment_operator _IsPODType;
    void push_back(const _Tp& x) {
        if(this->_M_finish!=this->_M_end_of_storage._M_data) {
            _Construct(this->_M_finish,x); ++this->_M_finish;
        } else _M_insert_overflow(this->_M_finish,x,_IsPODType(),1,true);
    }
    iterator erase(iterator first,iterator last) {
        iterator result=first;
        for(iterator source=last;source!=this->_M_finish;++source,++result) *result=*source;
        _Destroy(result,this->_M_finish);this->_M_finish=result;return first;
    }
    void clear() { erase(begin(),end()); }
    void resize(size_type n,_Tp x) {
        if(n<size()) erase(begin()+n,end()); else _M_fill_insert(end(),n-size(),x);
    }
    void resize(size_type n) { resize(n,_Tp()); }
protected:
    void _M_insert_overflow(iterator position,const _Tp& x,const __false_type&,size_type fillLength,bool atEnd) {
        const size_type oldSize=size();const size_type len=oldSize+(max)(oldSize,fillLength);
        _Tp* newStart=this->_M_end_of_storage.allocate(len);_Tp* newFinish=newStart;
        newFinish=__uninitialized_copy(this->_M_start,position,newStart,__false_type());
        if(fillLength==1) {_Construct(newFinish,x);++newFinish;}
        else newFinish=__uninitialized_fill_n(newFinish,fillLength,x,__false_type());
        if(!atEnd) newFinish=__uninitialized_copy(position,this->_M_finish,newFinish,__false_type());
        _M_clear();_M_set(newStart,newFinish,newStart+len);
    }
    __declspec(noinline) void _M_insert_overflow(iterator position,const _Tp& x,const __true_type&,size_type fillLength,bool atEnd) {
        const size_type oldSize=size();const size_type len=oldSize+(max)(oldSize,fillLength);
        _Tp* newStart=this->_M_end_of_storage.allocate(len);
        _Tp* newFinish=(_Tp*)__copy_trivial(this->_M_start,position,newStart);
        newFinish=fill_n(newFinish,fillLength,x);
        if(!atEnd) newFinish=(_Tp*)__copy_trivial(position,this->_M_finish,newFinish);
        _M_clear();_M_set(newStart,newFinish,newStart+len);
    }
    void _M_clear() {
        _Destroy(this->_M_start,this->_M_finish);
        this->_M_end_of_storage.deallocate(this->_M_start,this->_M_end_of_storage._M_data-this->_M_start);
    }
    void _M_set(iterator s,iterator f,iterator e) {
        this->_M_start=s;this->_M_finish=f;this->_M_end_of_storage._M_data=e;
    }
public:
    __declspec(noinline) void _M_fill_insert(iterator position,size_type n,const _Tp& x) {
        if(n!=0) {
            if(size_type(this->_M_end_of_storage._M_data-this->_M_finish)>=n) {
                _Tp copy=x;const size_type after=this->_M_finish-position;iterator oldFinish=this->_M_finish;
                if(after>n) {
                    __uninitialized_copy(this->_M_finish-n,this->_M_finish,this->_M_finish,_IsPODType());
                    this->_M_finish+=n;
                    __copy_backward_ptrs(position,oldFinish-n,oldFinish,_IsPODType());
                    std::fill(position,position+n,copy);
                } else {
                    __uninitialized_fill_n(this->_M_finish,n-after,copy,__false_type());
                    this->_M_finish+=n-after;
                    __uninitialized_copy(position,oldFinish,this->_M_finish,_IsPODType());
                    this->_M_finish+=after;std::fill(position,oldFinish,copy);
                }
            } else _M_insert_overflow(position,x,_IsPODType(),n,false);
        }
    }
#elif defined(FABLE_STLPORT_VISIBLE_PUSH_BACK)
    void push_back(const _Tp& x) {
        if (this->_M_finish!=this->_M_end_of_storage._M_data) {
            new ((void*)this->_M_finish) _Tp(x);
            ++this->_M_finish;
        } else {
            _M_insert_overflow(end(),x,__true_type(),size_type(1),true);
        }
    }
protected:
#ifdef FABLE_STLPORT_VISIBLE_TRUE_OVERFLOW
    
    void _M_insert_overflow(iterator position,const _Tp& x,
                                                const __true_type&,size_type fillLength,bool atEnd) {
        const size_type oldSize=size();
        const size_type len=oldSize+(oldSize<fillLength ? fillLength : oldSize);
        _Tp* newStart=(_Tp*)malloc(len*sizeof(_Tp));
        size_type before=position-this->_M_start;
        memmove(newStart,this->_M_start,before*sizeof(_Tp));
        _Tp* newFinish=newStart+before;
        for(size_type n=fillLength;n>0;--n,++newFinish) *newFinish=x;
        if(!atEnd) {
            size_type after=this->_M_finish-position;
            memmove(newFinish,position,after*sizeof(_Tp));
            newFinish+=after;
        }
        if(this->_M_start) free(this->_M_start);
        this->_M_start=newStart;
        this->_M_finish=newFinish;
        this->_M_end_of_storage._M_data=newStart+len;
    }
#else
    void _M_insert_overflow(iterator,const _Tp&,const __true_type&,size_type,bool);
#endif
#else
    void push_back(const _Tp&);
#endif
};
template<class _T1,class _T2> struct pair {
    _T1 first;
    _T2 second;
};
struct _List_node_base {
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};
template<class _Tp> struct _List_node : public _List_node_base { _Tp _M_data; };
template<class _Tp> struct _Nonconst_traits { typedef _Tp* pointer; typedef _Tp& reference; };
template<class _Tp> struct _Const_traits { typedef const _Tp* pointer; typedef const _Tp& reference; };
struct _List_iterator_base {
    _List_node_base* _M_node;
    _List_iterator_base(_List_node_base* p) : _M_node(p) { }
    _List_iterator_base() { }
    void _M_incr() { _M_node=_M_node->_M_next; }
    bool operator==(const _List_iterator_base& y) const { return _M_node==y._M_node; }
    bool operator!=(const _List_iterator_base& y) const { return _M_node!=y._M_node; }
};
template<class _Tp,class _Traits> struct _List_iterator : public _List_iterator_base {
    typedef typename _Traits::reference reference;
    typedef typename _Traits::pointer pointer;
    typedef _List_iterator<_Tp,_Nonconst_traits<_Tp> > iterator;
    _List_iterator(_List_node_base* x) : _List_iterator_base(x) { }
    _List_iterator() { }
    reference operator*() const { return ((_List_node<_Tp>*)_M_node)->_M_data; }
    pointer operator->() const { return &operator*(); }
    _List_iterator& operator++() { this->_M_incr(); return *this; }
};
template<class _Tp,class _Alloc> class _List_base {
public:
#ifdef FABLE_STLPORT_CONSTRUCTORS
    _List_base(const _Alloc&) : _M_node(allocator<_List_node<_Tp> >(),0) {
        _List_node<_Tp>* n=_M_node.allocate(1);n->_M_next=n;n->_M_prev=n;_M_node._M_data=n;
    }
    ~_List_base() { clear();_M_node.deallocate(_M_node._M_data,1); }
#endif
    _STLP_alloc_proxy<_List_node<_Tp>*,_List_node<_Tp>,allocator<_List_node<_Tp> > > _M_node;
    void clear();
};
template<class _Tp,class _Alloc=allocator<_Tp> > class list : public _List_base<_Tp,_Alloc> {
public:
#ifdef FABLE_STLPORT_CONSTRUCTORS
    explicit list(const _Alloc& a=_Alloc()) : _List_base<_Tp,_Alloc>(a) { }
#endif
    typedef _List_iterator<_Tp,_Nonconst_traits<_Tp> > iterator;
    typedef _List_iterator<_Tp,_Const_traits<_Tp> > const_iterator;
    iterator begin() { return iterator(this->_M_node._M_data->_M_next); }
    const_iterator begin() const { return const_iterator(this->_M_node._M_data->_M_next); }
    iterator end() { return iterator(this->_M_node._M_data); }
    const_iterator end() const { return const_iterator(this->_M_node._M_data); }
    bool empty() const { return this->_M_node._M_data->_M_next==this->_M_node._M_data; }
};
template<size_t _Nw> struct _Base_bitset {
    typedef unsigned long _WordT;
    _WordT _M_w[_Nw];
    static size_t _S_whichword(size_t n) { return n/(8*sizeof(_WordT)); }
    static size_t _S_whichbit(size_t n) { return n%(8*sizeof(_WordT)); }
    static _WordT _S_maskbit(size_t n) { return _WordT(1)<<_S_whichbit(n); }
    _WordT _M_getword(size_t n) const { return _M_w[_S_whichword(n)]; }
};
void __stl_throw_out_of_range(const char*);
template<size_t _Nb> class bitset : public _Base_bitset<(_Nb+8*sizeof(unsigned long)-1)/(8*sizeof(unsigned long))> {
public:
    bool _Unchecked_test(size_t n) const { return (this->_M_getword(n)&this->_S_maskbit(n))!=0; }
    bool test(size_t n) const { if(n>=_Nb) __stl_throw_out_of_range("bitset"); return _Unchecked_test(n); }
};
template<class _Arg,class _Result> struct unary_function {
    typedef _Arg argument_type;
    typedef _Result result_type;
};
template<class _RandomAccessIter,class _RandomNumberGenerator>
void __fastcall random_shuffle(_RandomAccessIter,_RandomAccessIter,_RandomNumberGenerator&);
}
#endif

class CIEngine;
class CEnginePrimitiveHandle;
class CRGBColour;
class C2DBoxF;
template<class T> class CDefPointer;
namespace std { template<class T> struct less; template<class K,class T,class C,class A> class map; }
namespace NUISystem {
class CComponent;
class CAction;
class CUIDef;
class CUIState;
class CDeletion;

class CUIInterface {
public:
    virtual ~CUIInterface(); 
    virtual void Update(float); 
    virtual void Draw(CIEngine &, CEnginePrimitiveHandle *, long, long *, NUISystem::CComponent *); 
};

class CObserver {
public:
    CObserver();
    virtual ~CObserver();
    virtual void ProcessEvent(EEvent)=0;
    virtual bool IsObservingEvent(EEvent);
    virtual void ObserveEvent(EEvent);
    virtual void IgnoreEvent(EEvent);
    virtual void IgnoreAllEvents();
    virtual void ObserveAllEvents();
    virtual void SetPreventObserving(bool);
protected:
    unsigned char EventsToObserve[12]; 
    bool PreventObservation; 
};

class CActionParam {
public:
    CActionParam();
    virtual ~CActionParam();
    EActionParamType ActionParamType;
};


class CComponent : public CUIInterface, public CObserver, public CActionParam {
public:
    CComponent(long);
    virtual ~CComponent();
    virtual void Update(float);
    virtual void Draw(CIEngine&,CEnginePrimitiveHandle*,long,long*,CComponent*);
    virtual NUISystem::CComponent * FindChild(CCharString); 
    virtual NUISystem::CComponent * FindChildForward(CCharString); 
    virtual void FindAllChildren(CCharString, std::list<NUISystem::CComponent *,std::allocator<NUISystem::CComponent *> > &); 
    virtual void FindAllChildrenOfType(NUISystem::EType, std::list<NUISystem::CComponent *,std::allocator<NUISystem::CComponent *> > &); 
    virtual void SetPAcceptButton(NUISystem::CComponent *); 
    virtual NUISystem::CComponent * FindChildNonRecursive(CCharString); 
    virtual NUISystem::CComponent * FindFirstChildOfType(NUISystem::EType); 
    virtual bool HasChild(NUISystem::CComponent *) const; 
    virtual void SetZoom(const C2DVector &); 
    virtual C2DVector GetZoom() const; 
    virtual void SetColour(const CRGBColour &); 
    virtual CRGBColour GetColour() const; 
    virtual void SetPosition(const C2DVector &); 
    virtual C2DVector GetPosition() const; 
    virtual C2DVector GetParentPosition() const; 
    virtual void SetParentPosition(C2DVector &); 
    virtual C2DVector GetParentZoom() const; 
    virtual void SetParentZoom(C2DVector &); 
    virtual CRGBColour GetParentColour() const; 
    virtual void SetParentColour(CRGBColour &); 
    virtual C2DVector GetSize() const; 
    virtual C2DBoxF GetBoundingBox() const; 
    virtual const C2DVector & GetRenderPosition() const; 
    virtual const C2DVector & GetRenderZoom() const; 
    virtual const CRGBColour & GetRenderColour() const; 
    virtual void SetRenderPosition(C2DVector &); 
    virtual void SetRenderZoom(C2DVector &); 
    virtual void SetRenderColour(CRGBColour &); 
    virtual void ChangePosition(const C2DVector &, float, bool); 
    virtual void ChangePositionDelta(const C2DVector &, float, bool); 
    virtual void ChangeTargetPositionDelta(const C2DVector &, float, bool); 
    virtual void UpdatePosition(float); 
    virtual void ChangeZoom(const C2DVector &, float, bool); 
    virtual void ChangeZoomDelta(const C2DVector &, float, bool); 
    virtual void UpdateZoom(float); 
    virtual void ChangeColour(const CRGBColour &, float, bool); 
    virtual void ChangeColourDelta(const CRGBColour &, float, bool); 
    virtual void UpdateColour(float); 
    virtual void ResetState(); 
    virtual void ForceState(unsigned long, bool); 
    virtual void Initialise(); 
    virtual bool HasState(unsigned long); 
    virtual bool HasStateInChildren(unsigned long); 
    virtual void ChangeState(unsigned long); 
    virtual void ChangeState(unsigned long, float); 
    virtual void ChangeState(NUISystem::CUIState &); 
    virtual bool ChangedState(); 
    virtual NUISystem::CUIState * GetCurrentState(); 
    virtual void SetParent(NUISystem::CComponent *); 
    virtual NUISystem::CComponent * GetParent() const; 
    virtual std::vector<CCountedPointer<NUISystem::CComponent>,std::allocator<CCountedPointer<NUISystem::CComponent> > > & GetChildren(); 
    virtual float GetUpdateTime() const; 
    virtual C2DVector GetTargetPosition() const; 
    virtual CRGBColour GetTargetColour() const; 
    virtual C2DVector GetTargetZoom() const; 
    virtual void ProcessChangeState(); 
    virtual void AddChild(CCountedPointer<NUISystem::CComponent> &); 
    virtual void AddChildDynamically(CCountedPointer<NUISystem::CComponent> &); 
    virtual void AddChildFront(CCountedPointer<NUISystem::CComponent> &); 
    virtual void SetDeletion(NUISystem::CDeletion); 
    virtual NUISystem::CDeletion & GetDeletion(); 
    virtual void RemoveChild(unsigned long); 
    virtual NUISystem::EType GetType() const; 
    virtual void AddAction(CCountedPointer<NUISystem::CAction> &); 
    virtual void AddActionOnSelected(CCountedPointer<NUISystem::CAction> &); 
    virtual void AddActionOnBack(CCountedPointer<NUISystem::CAction> &); 
    virtual void AddPreAction(CCountedPointer<NUISystem::CAction> &); 
    virtual void AddActionOnInactive(CCountedPointer<NUISystem::CAction> &); 
    virtual void AddActionOnLeftClicked(CCountedPointer<NUISystem::CAction> &); 
    virtual void AddActionOnLeftHeld(CCountedPointer<NUISystem::CAction> &); 
    virtual void AddActionOnRightClicked(CCountedPointer<NUISystem::CAction> &); 
    virtual void AddActionOnDropped(CCountedPointer<NUISystem::CAction> &); 
    virtual void AddActionOnDroppedNowhere(CCountedPointer<NUISystem::CAction> &); 
    virtual void AddActionOnDraggedUp(CCountedPointer<NUISystem::CAction> &); 
    virtual void AddActionOnDraggedDown(CCountedPointer<NUISystem::CAction> &); 
    virtual void AddActionOnHovered(CCountedPointer<NUISystem::CAction> &); 
    virtual void AddActionOnUnhovered(CCountedPointer<NUISystem::CAction> &); 
    virtual void AddActionOnLeftUnclicked(CCountedPointer<NUISystem::CAction> &); 
    virtual void Die(); 
    virtual std::map<unsigned long,NUISystem::CUIState,std::less<unsigned long>,std::allocator<std::pair<unsigned long const ,NUISystem::CUIState> > > & GetStates(); 
    virtual void SetName(const CCharString &); 
    virtual CCharString GetName(); 
    virtual NUISystem::CComponent * FindComponent(NUISystem::CComponent *); 
    virtual unsigned long GetCurrentStateIndex(); 
    virtual unsigned long GetTargetStateIndex() const; 
    virtual float CanScroll(bool, C2DBoxF &); 
    virtual void FakeChildDying(NUISystem::CComponent *); 
    virtual void SetParentViewport(NUISystem::CComponent *); 
    virtual void ClearHandle(); 
    virtual void MakeUnavailable(); 
    virtual void MakeUnavailableDynamically(); 
    virtual void MakeAvailable(); 
    virtual void MakeAvailableDynamically(); 
    virtual void AddActionIAmParamOf(CCountedPointer<NUISystem::CAction> &); 
    virtual NUISystem::CComponent * FindFirstChildOfTypeNonRecursive(NUISystem::EType); 
    virtual bool IsAllowedToScroll() const; 
    virtual NUISystem::CComponent * GetParentViewport(); 
    virtual bool IsBastardChild() const; 
    virtual bool IsIndependent() const; 
    virtual bool IsPositionIndependent() const; 
    virtual bool IsZoomIndependent() const; 
    virtual bool IsLayerIndependent() const; 
    virtual bool IsDrawFromViewport() const; 
    virtual bool IsPositionIsCenter() const; 
    virtual long GetDefIndex() const; 
    virtual CDefPointer<NUISystem::CUIDef const > LoadAndReturnDef() const; 
    virtual void OnObserverRemove(); 
    virtual C2DVector CanMove(const C2DVector &); 
    virtual C2DVector GetDynamicSize() const; 
    virtual C2DVector GetTargetRenderPosition() const; 
    virtual bool IsPCStyle() const; 
    virtual void SetRelativeParentPosition(C2DVector &); 
    virtual void SetRelativeParentZoom(C2DVector &); 
    virtual bool IsUsingRelativeZoom() const; 
    virtual bool IsUsingRelativePosition() const; 
    virtual C2DVector GetRelativeRenderPosition(); 
    virtual C2DVector GetRelativeRenderZoom(); 
    virtual void SetRelativeRenderPosition(C2DVector); 
    virtual C2DVector SetRelativeRenderZoom(C2DVector); 
    virtual C2DVector GetTargetRelativeRenderPosition() const; 
    virtual C2DVector GetTargetRelativeRenderZoom() const; 
    virtual void AddPositionChild(NUISystem::CComponent *); 
    virtual void SetPositionParent(NUISystem::CComponent *); 
    virtual NUISystem::CComponent * GetPositionParent(); 
    virtual void PositionChildDying(NUISystem::CComponent *); 
    virtual void PositionParentDying(NUISystem::CComponent *); 
    virtual long GetRenderLayer() const; 
    virtual void AddActionOnDesctruction(CCountedPointer<NUISystem::CAction> &); 
    virtual void ExecuteActions(std::list<CCountedPointer<NUISystem::CAction>,std::allocator<CCountedPointer<NUISystem::CAction> > > *); 
    virtual bool IsClickable() const; 
    virtual void RemoveFromHoveredList(); 
    virtual void ProcessEvent(EEvent);
protected:
    CCharString Name; 
    unsigned char States[12]; 
    float Time; 
    C2DVector Position; 
    C2DVector TargetPosition; 
    C2DVector InitialPosition; 
    C2DVector ParentPosition; 
    C2DVector RenderPosition; 
    C2DVector Zoom; 
    C2DVector TargetZoom; 
    C2DVector InitialZoom; 
    C2DVector ParentZoom; 
    C2DVector RenderZoom; 
    unsigned char Colour[4]; 
    unsigned char TargetColour[4]; 
    unsigned char InitialColour[4]; 
    unsigned char ParentColour[4]; 
    unsigned char RenderColour[4]; 
    float PositionTimeElapsed; 
    float PositionTime; 
    float ZoomTimeElapsed; 
    float ZoomTime; 
    float ColourTimeElapsed; 
    float ColourTime; 
    std::vector<CCountedPointer<NUISystem::CComponent>,std::allocator<CCountedPointer<NUISystem::CComponent> > > Children; 
    std::vector<CCountedPointer<NUISystem::CComponent>,std::allocator<CCountedPointer<NUISystem::CComponent> > > ChildrenToDelete; 
    NUISystem::CComponent * PParent; 
    C2DVector Size; 
    unsigned char Deletion[8]; 
    NUISystem::CComponent * PParentViewport; 
    unsigned char ActionsIAmParamOf[4]; 
    std::vector<unsigned long,std::allocator<unsigned long> > ShapeChildren; 
    C2DVector RenderPositionSave; 
    C2DVector RelativeRenderPosition; 
    C2DVector RelativeParentPosition; 
    C2DVector RelativeRenderZoom; 
    C2DVector RelativeParentZoom; 
    NUISystem::CComponent * PPositionParent; 
    unsigned char PositionChildren[12]; 
    std::list<CCountedPointer<NUISystem::CAction>,std::allocator<CCountedPointer<NUISystem::CAction> > > * PActionsOnDestruction; 
    unsigned char Flags; 
    unsigned char Type; 
    unsigned char FlagsSetTwo; 
    char Layer; 
    long DefIndex; 
};
}
#endif

#include <string.h>
namespace std {
#pragma auto_inline(off)
template<> vector<unsigned long>::vector(const vector& x)
    : _Vector_base<unsigned long,allocator<unsigned long> >(x.size(),x.get_allocator())
{
    if(x._M_start!=x._M_finish)
        this->_M_finish=(unsigned long*)((char*)memmove(this->_M_start,x._M_start,(char*)x._M_finish-(char*)x._M_start)+((char*)x._M_finish-(char*)x._M_start));
}
#pragma auto_inline(on)
struct input_iterator_tag { };
struct forward_iterator_tag : input_iterator_tag { };
struct bidirectional_iterator_tag : forward_iterator_tag { };
struct random_access_iterator_tag : bidirectional_iterator_tag { };
#pragma auto_inline(off)
template<class I,class T> I __fastcall __find(I __first,I __last,const T& __val,const random_access_iterator_tag&)
{
    ptrdiff_t __trip_count=(__last-__first)>>2;
    for(;__trip_count>0;--__trip_count) {
        if(*__first==__val) return __first; ++__first;
        if(*__first==__val) return __first; ++__first;
        if(*__first==__val) return __first; ++__first;
        if(*__first==__val) return __first; ++__first;
    }
    switch(__last-__first) {
    case 3: if(*__first==__val) return __first; ++__first;
    case 2: if(*__first==__val) return __first; ++__first;
    case 1: if(*__first==__val) return __first; ++__first;
    default: return __last;
    }
}
#pragma auto_inline(on)
template<class I,class T> I __fastcall find(I first,I last,const T& value) { return __find(first,last,value,random_access_iterator_tag()); }
}
C2DVector NUISystem::CComponent::GetDynamicSize() const
{
    C2DVector max;
    C2DVector min;
    if (Children.empty()) return C2DVector(0.0f,0.0f);
    std::vector<unsigned long> shape_children(ShapeChildren);
    long first_child=-1;
    if(shape_children.empty()) first_child=0;
    for(unsigned long count=0;count<Children.size() && !shape_children.empty();++count) {
        if(std::find(shape_children.begin(),shape_children.end(),count)!=shape_children.end()) {
            first_child=count;
            break;
        }
    }
    if(first_child==-1) return C2DVector(0.0f,0.0f);
    min=Children[first_child]->GetPosition();
    max=min+Children[first_child]->GetDynamicSize();
    for(unsigned long count=0;count<Children.size();++count) {
        if((shape_children.empty() || std::find(shape_children.begin(),shape_children.end(),count)!=shape_children.end()) && count!=first_child) {
            if(Children[count]->GetType()==UI_TYPE_SPRITE && Children[count]->IsPositionIsCenter()) {
                if(Children[count]->GetPosition().X-Children[count]->GetDynamicSize().X*0.5f<min.X)
                    min.X=Children[count]->GetPosition().X-Children[count]->GetDynamicSize().X*0.5f;
                if(Children[count]->GetPosition().Y-Children[count]->GetDynamicSize().Y*0.5f<min.Y)
                    min.Y=Children[count]->GetPosition().Y-Children[count]->GetDynamicSize().Y*0.5f;
            } else {
                if(Children[count]->GetPosition().X<min.X) min.X=Children[count]->GetPosition().X;
                if(Children[count]->GetPosition().Y<min.Y) min.Y=Children[count]->GetPosition().Y;
            }
            if(Children[count]->GetType()==UI_TYPE_SPRITE && Children[count]->IsPositionIsCenter()) {
                if(Children[count]->GetPosition().X+Children[count]->GetDynamicSize().X*0.5f>max.X)
                    max.X=Children[count]->GetPosition().X+Children[count]->GetDynamicSize().X*0.5f;
                if(Children[count]->GetPosition().Y+(float)(Children[count]->GetDynamicSize().Y*0.5f)>max.Y)
                    max.Y=Children[count]->GetPosition().Y+(float)(Children[count]->GetDynamicSize().Y*0.5f);
            } else {
                if(Children[count]->GetPosition().X+Children[count]->GetDynamicSize().X>max.X)
                    max.X=Children[count]->GetPosition().X+Children[count]->GetDynamicSize().X;
                if(Children[count]->GetPosition().Y+Children[count]->GetDynamicSize().Y>max.Y)
                    max.Y=Children[count]->GetPosition().Y+Children[count]->GetDynamicSize().Y;
            }
        }
    }
    return max-min;
}

class C2DBoxF { public: float TLX,TLY,BRX,BRY; };
class CRGBColour {
public:
#ifdef _MSC_VER
    union { struct { unsigned char B,G,R,A; }; unsigned long IntValue; };
#else
    struct Channels { unsigned char B,G,R,A; };
    union { Channels channels; unsigned int IntValue; };
#endif
};
namespace NUISystem {
class CDeletion {
public:
    enum EDeletionMethod {UI_DO_NOT_DELETE=0,UI_DELETE_THIS_INSTANCE=1,UI_DELETE_ALL_INSTANCES=2,UI_DELETE_THIS_INSTANCE_PER_PARENT=3};
    EDeletionMethod Method;
    std::list<CComponent*> AssociatedParents;
};
}
CCharString::CCharString() : PStringData(0) { }
CCharString::~CCharString() { }
NUISystem::CUIInterface::~CUIInterface() { }
void NUISystem::CUIInterface::Update(float) { throw 1; }
void NUISystem::CUIInterface::Draw(CIEngine&,CEnginePrimitiveHandle*,long,long*,CComponent*) { throw 1; }
NUISystem::CObserver::CObserver() : PreventObservation(false) { memset(EventsToObserve,0,sizeof(EventsToObserve)); }
NUISystem::CObserver::~CObserver() { }
bool NUISystem::CObserver::IsObservingEvent(EEvent) { throw 1; }
void NUISystem::CObserver::ObserveEvent(EEvent) { throw 1; }
void NUISystem::CObserver::IgnoreEvent(EEvent) { throw 1; }
void NUISystem::CObserver::IgnoreAllEvents() { throw 1; }
void NUISystem::CObserver::ObserveAllEvents() { throw 1; }
void NUISystem::CObserver::SetPreventObserving(bool) { throw 1; }
NUISystem::CActionParam::CActionParam() : ActionParamType(UI_ACTION_PARAM_TYPE_COMPONENT) { }
NUISystem::CActionParam::~CActionParam() { }
NUISystem::CComponent::CComponent(long) { }
NUISystem::CComponent::~CComponent() { }
void NUISystem::CComponent::Update(float) { throw 1; }
void NUISystem::CComponent::Draw(CIEngine&,CEnginePrimitiveHandle*,long,long*,CComponent*) { throw 1; }
void NUISystem::CComponent::ProcessEvent(EEvent) { throw 1; }
NUISystem::CComponent * NUISystem::CComponent::FindChild(CCharString) { throw 1; }
NUISystem::CComponent * NUISystem::CComponent::FindChildForward(CCharString) { throw 1; }
void NUISystem::CComponent::FindAllChildren(CCharString, std::list<NUISystem::CComponent *,std::allocator<NUISystem::CComponent *> > &) { throw 1; }
void NUISystem::CComponent::FindAllChildrenOfType(NUISystem::EType, std::list<NUISystem::CComponent *,std::allocator<NUISystem::CComponent *> > &) { throw 1; }
void NUISystem::CComponent::SetPAcceptButton(NUISystem::CComponent *) { throw 1; }
NUISystem::CComponent * NUISystem::CComponent::FindChildNonRecursive(CCharString) { throw 1; }
NUISystem::CComponent * NUISystem::CComponent::FindFirstChildOfType(NUISystem::EType) { throw 1; }
bool NUISystem::CComponent::HasChild(NUISystem::CComponent *) const { throw 1; }
void NUISystem::CComponent::SetZoom(const C2DVector &) { throw 1; }
C2DVector NUISystem::CComponent::GetZoom() const { throw 1; }
void NUISystem::CComponent::SetColour(const CRGBColour &) { throw 1; }
CRGBColour NUISystem::CComponent::GetColour() const { throw 1; }
void NUISystem::CComponent::SetPosition(const C2DVector &) { throw 1; }
C2DVector NUISystem::CComponent::GetPosition() const { throw 1; }
C2DVector NUISystem::CComponent::GetParentPosition() const { throw 1; }
void NUISystem::CComponent::SetParentPosition(C2DVector &) { throw 1; }
C2DVector NUISystem::CComponent::GetParentZoom() const { throw 1; }
void NUISystem::CComponent::SetParentZoom(C2DVector &) { throw 1; }
CRGBColour NUISystem::CComponent::GetParentColour() const { throw 1; }
void NUISystem::CComponent::SetParentColour(CRGBColour &) { throw 1; }
C2DVector NUISystem::CComponent::GetSize() const { throw 1; }
C2DBoxF NUISystem::CComponent::GetBoundingBox() const { throw 1; }
const C2DVector & NUISystem::CComponent::GetRenderPosition() const { throw 1; }
const C2DVector & NUISystem::CComponent::GetRenderZoom() const { throw 1; }
const CRGBColour & NUISystem::CComponent::GetRenderColour() const { throw 1; }
void NUISystem::CComponent::SetRenderPosition(C2DVector &) { throw 1; }
void NUISystem::CComponent::SetRenderZoom(C2DVector &) { throw 1; }
void NUISystem::CComponent::SetRenderColour(CRGBColour &) { throw 1; }
void NUISystem::CComponent::ChangePosition(const C2DVector &, float, bool) { throw 1; }
void NUISystem::CComponent::ChangePositionDelta(const C2DVector &, float, bool) { throw 1; }
void NUISystem::CComponent::ChangeTargetPositionDelta(const C2DVector &, float, bool) { throw 1; }
void NUISystem::CComponent::UpdatePosition(float) { throw 1; }
void NUISystem::CComponent::ChangeZoom(const C2DVector &, float, bool) { throw 1; }
void NUISystem::CComponent::ChangeZoomDelta(const C2DVector &, float, bool) { throw 1; }
void NUISystem::CComponent::UpdateZoom(float) { throw 1; }
void NUISystem::CComponent::ChangeColour(const CRGBColour &, float, bool) { throw 1; }
void NUISystem::CComponent::ChangeColourDelta(const CRGBColour &, float, bool) { throw 1; }
void NUISystem::CComponent::UpdateColour(float) { throw 1; }
void NUISystem::CComponent::ResetState() { throw 1; }
void NUISystem::CComponent::ForceState(unsigned long, bool) { throw 1; }
void NUISystem::CComponent::Initialise() { throw 1; }
bool NUISystem::CComponent::HasState(unsigned long) { throw 1; }
bool NUISystem::CComponent::HasStateInChildren(unsigned long) { throw 1; }
void NUISystem::CComponent::ChangeState(NUISystem::CUIState &) { throw 1; }
void NUISystem::CComponent::ChangeState(unsigned long, float) { throw 1; }
void NUISystem::CComponent::ChangeState(unsigned long) { throw 1; }
bool NUISystem::CComponent::ChangedState() { throw 1; }
NUISystem::CUIState * NUISystem::CComponent::GetCurrentState() { throw 1; }
void NUISystem::CComponent::SetParent(NUISystem::CComponent *) { throw 1; }
NUISystem::CComponent * NUISystem::CComponent::GetParent() const { throw 1; }
std::vector<CCountedPointer<NUISystem::CComponent>,std::allocator<CCountedPointer<NUISystem::CComponent> > > & NUISystem::CComponent::GetChildren() { throw 1; }
float NUISystem::CComponent::GetUpdateTime() const { throw 1; }
C2DVector NUISystem::CComponent::GetTargetPosition() const { throw 1; }
CRGBColour NUISystem::CComponent::GetTargetColour() const { throw 1; }
C2DVector NUISystem::CComponent::GetTargetZoom() const { throw 1; }
void NUISystem::CComponent::ProcessChangeState() { throw 1; }
void NUISystem::CComponent::AddChild(CCountedPointer<NUISystem::CComponent> &) { throw 1; }
void NUISystem::CComponent::AddChildDynamically(CCountedPointer<NUISystem::CComponent> &) { throw 1; }
void NUISystem::CComponent::AddChildFront(CCountedPointer<NUISystem::CComponent> &) { throw 1; }
void NUISystem::CComponent::SetDeletion(NUISystem::CDeletion) { throw 1; }
NUISystem::CDeletion & NUISystem::CComponent::GetDeletion() { throw 1; }
void NUISystem::CComponent::RemoveChild(unsigned long) { throw 1; }
NUISystem::EType NUISystem::CComponent::GetType() const { throw 1; }
void NUISystem::CComponent::AddAction(CCountedPointer<NUISystem::CAction> &) { throw 1; }
void NUISystem::CComponent::AddActionOnSelected(CCountedPointer<NUISystem::CAction> &) { throw 1; }
void NUISystem::CComponent::AddActionOnBack(CCountedPointer<NUISystem::CAction> &) { throw 1; }
void NUISystem::CComponent::AddPreAction(CCountedPointer<NUISystem::CAction> &) { throw 1; }
void NUISystem::CComponent::AddActionOnInactive(CCountedPointer<NUISystem::CAction> &) { throw 1; }
void NUISystem::CComponent::AddActionOnLeftClicked(CCountedPointer<NUISystem::CAction> &) { throw 1; }
void NUISystem::CComponent::AddActionOnLeftHeld(CCountedPointer<NUISystem::CAction> &) { throw 1; }
void NUISystem::CComponent::AddActionOnRightClicked(CCountedPointer<NUISystem::CAction> &) { throw 1; }
void NUISystem::CComponent::AddActionOnDropped(CCountedPointer<NUISystem::CAction> &) { throw 1; }
void NUISystem::CComponent::AddActionOnDroppedNowhere(CCountedPointer<NUISystem::CAction> &) { throw 1; }
void NUISystem::CComponent::AddActionOnDraggedUp(CCountedPointer<NUISystem::CAction> &) { throw 1; }
void NUISystem::CComponent::AddActionOnDraggedDown(CCountedPointer<NUISystem::CAction> &) { throw 1; }
void NUISystem::CComponent::AddActionOnHovered(CCountedPointer<NUISystem::CAction> &) { throw 1; }
void NUISystem::CComponent::AddActionOnUnhovered(CCountedPointer<NUISystem::CAction> &) { throw 1; }
void NUISystem::CComponent::AddActionOnLeftUnclicked(CCountedPointer<NUISystem::CAction> &) { throw 1; }
void NUISystem::CComponent::Die() { throw 1; }
std::map<unsigned long,NUISystem::CUIState,std::less<unsigned long>,std::allocator<std::pair<unsigned long const ,NUISystem::CUIState> > > & NUISystem::CComponent::GetStates() { throw 1; }
void NUISystem::CComponent::SetName(const CCharString &) { throw 1; }
CCharString NUISystem::CComponent::GetName() { throw 1; }
NUISystem::CComponent * NUISystem::CComponent::FindComponent(NUISystem::CComponent *) { throw 1; }
unsigned long NUISystem::CComponent::GetCurrentStateIndex() { throw 1; }
unsigned long NUISystem::CComponent::GetTargetStateIndex() const { throw 1; }
float NUISystem::CComponent::CanScroll(bool, C2DBoxF &) { throw 1; }
void NUISystem::CComponent::FakeChildDying(NUISystem::CComponent *) { throw 1; }
void NUISystem::CComponent::SetParentViewport(NUISystem::CComponent *) { throw 1; }
void NUISystem::CComponent::ClearHandle() { throw 1; }
void NUISystem::CComponent::MakeUnavailable() { throw 1; }
void NUISystem::CComponent::MakeUnavailableDynamically() { throw 1; }
void NUISystem::CComponent::MakeAvailable() { throw 1; }
void NUISystem::CComponent::MakeAvailableDynamically() { throw 1; }
void NUISystem::CComponent::AddActionIAmParamOf(CCountedPointer<NUISystem::CAction> &) { throw 1; }
NUISystem::CComponent * NUISystem::CComponent::FindFirstChildOfTypeNonRecursive(NUISystem::EType) { throw 1; }
bool NUISystem::CComponent::IsAllowedToScroll() const { throw 1; }
NUISystem::CComponent * NUISystem::CComponent::GetParentViewport() { throw 1; }
bool NUISystem::CComponent::IsBastardChild() const { throw 1; }
bool NUISystem::CComponent::IsIndependent() const { throw 1; }
bool NUISystem::CComponent::IsPositionIndependent() const { throw 1; }
bool NUISystem::CComponent::IsZoomIndependent() const { throw 1; }
bool NUISystem::CComponent::IsLayerIndependent() const { throw 1; }
bool NUISystem::CComponent::IsDrawFromViewport() const { throw 1; }
bool NUISystem::CComponent::IsPositionIsCenter() const { throw 1; }
long NUISystem::CComponent::GetDefIndex() const { throw 1; }
CDefPointer<NUISystem::CUIDef const > NUISystem::CComponent::LoadAndReturnDef() const { throw 1; }
void NUISystem::CComponent::OnObserverRemove() { throw 1; }
C2DVector NUISystem::CComponent::CanMove(const C2DVector &) { throw 1; }
C2DVector NUISystem::CComponent::GetTargetRenderPosition() const { throw 1; }
bool NUISystem::CComponent::IsPCStyle() const { throw 1; }
void NUISystem::CComponent::SetRelativeParentPosition(C2DVector &) { throw 1; }
void NUISystem::CComponent::SetRelativeParentZoom(C2DVector &) { throw 1; }
bool NUISystem::CComponent::IsUsingRelativeZoom() const { throw 1; }
bool NUISystem::CComponent::IsUsingRelativePosition() const { throw 1; }
C2DVector NUISystem::CComponent::GetRelativeRenderPosition() { throw 1; }
C2DVector NUISystem::CComponent::GetRelativeRenderZoom() { throw 1; }
void NUISystem::CComponent::SetRelativeRenderPosition(C2DVector) { throw 1; }
C2DVector NUISystem::CComponent::SetRelativeRenderZoom(C2DVector) { throw 1; }
C2DVector NUISystem::CComponent::GetTargetRelativeRenderPosition() const { throw 1; }
C2DVector NUISystem::CComponent::GetTargetRelativeRenderZoom() const { throw 1; }
void NUISystem::CComponent::AddPositionChild(NUISystem::CComponent *) { throw 1; }
void NUISystem::CComponent::SetPositionParent(NUISystem::CComponent *) { throw 1; }
NUISystem::CComponent * NUISystem::CComponent::GetPositionParent() { throw 1; }
void NUISystem::CComponent::PositionChildDying(NUISystem::CComponent *) { throw 1; }
void NUISystem::CComponent::PositionParentDying(NUISystem::CComponent *) { throw 1; }
long NUISystem::CComponent::GetRenderLayer() const { throw 1; }
void NUISystem::CComponent::AddActionOnDesctruction(CCountedPointer<NUISystem::CAction> &) { throw 1; }
void NUISystem::CComponent::ExecuteActions(std::list<CCountedPointer<NUISystem::CAction>,std::allocator<CCountedPointer<NUISystem::CAction> > > *) { throw 1; }
bool NUISystem::CComponent::IsClickable() const { throw 1; }
void NUISystem::CComponent::RemoveFromHoveredList() { throw 1; }

static void fail(int n) { fprintf(stderr,"UI bounds check %d\n",n);exit(n); }
class UIChild : public NUISystem::CComponent {
public:
    bool centered;
    mutable unsigned long positionCalls,sizeCalls,typeCalls,centerCalls;
    UIChild() : CComponent(0),centered(false),positionCalls(0),sizeCalls(0),typeCalls(0),centerCalls(0) { Position=C2DVector(0,0);Size=C2DVector(0,0);Type=0; }
    virtual C2DVector GetDynamicSize() const { ++sizeCalls;return Size; }
    virtual C2DVector GetPosition() const { ++positionCalls;return Position; }
    virtual NUISystem::EType GetType() const { ++typeCalls;return (NUISystem::EType)Type; }
    virtual bool IsPositionIsCenter() const { ++centerCalls;return centered; }
    void setup(float x,float y,float width,float height,bool center,unsigned char type) {Position=C2DVector(x,y);Size=C2DVector(width,height);centered=center;Type=type;}
    void add(UIChild* child);
    void select(unsigned long n);
    C2DVector evaluate() const {return CComponent::GetDynamicSize();}
    C2DVector pos()const{return Position;}
    C2DVector dim()const{return Size;}
    unsigned char kind()const{return Type;}
};
struct UIFixture {
    template<class T> static void append(std::vector<T>& values,const T& value) {
        size_t n=values.size();T* dst=(T*)malloc((n+1)*sizeof(T));
        size_t i;for(i=0;i<n;++i)new(dst+i)T(values[i]);new(dst+n)T(value);
        for(i=0;i<n;++i)values[i].~T();free(values._M_start);
        values._M_start=dst;values._M_finish=dst+n+1;values._M_end_of_storage._M_data=values._M_finish;
    }
};
void UIChild::add(UIChild* child){CCountedPointer<NUISystem::CComponent> p;p.Data=child;p.Info=0;UIFixture::append(Children,p);}
void UIChild::select(unsigned long n){UIFixture::append(ShapeChildren,n);}
static unsigned int scenarios=0;
static void run(unsigned int n,unsigned int filter,unsigned int mode) {
    UIChild parent;UIChild children[9];bool selected[9]={false,false,false,false,false,false,false,false,false};
    unsigned int i;
    for(i=0;i<n;++i) {
        children[i].setup((float)((int)((i*5+mode)%19)-9)*0.5f,(float)((int)((i*7+mode)%17)-8)*0.5f,(float)(i%4+1),(float)(i%3+1),(i+mode)%2==0,(unsigned char)((i+mode)%3?0:6));
        parent.add(&children[i]);
        selected[i]=filter==0 || (filter==1 && i%2==0) || (filter==2 && i%2==1) || (filter==4 && i==n-1);
    }
    if(filter) {
        for(i=0;i<n;++i)if(selected[i]){parent.select(i);if(mode%2)parent.select(i);}
        parent.select(1000);
    }
    int first=-1;for(i=0;i<n;++i)if(selected[i]){first=(int)i;break;}
    C2DVector expected(0.0f,0.0f);
    if(first>=0) {
        C2DVector low=children[first].pos();C2DVector high=low+children[first].dim();
        for(i=0;i<n;++i)if(selected[i] && (int)i!=first) {
            C2DVector p=children[i].pos(),d=children[i].dim();
            float factor=children[i].kind()==0 && children[i].centered?0.5f:0.0f;
            float left=p.X-d.X*factor,bottom=p.Y-d.Y*factor;
            float right=p.X+d.X*(1.0f-factor),top=p.Y+d.Y*(1.0f-factor);
            if(left<low.X)low.X=left;if(bottom<low.Y)low.Y=bottom;
            if(right>high.X)high.X=right;if(top>high.Y)high.Y=top;
        }
        expected=high-low;
    }
    C2DVector actual=parent.evaluate();if(actual.X!=expected.X||actual.Y!=expected.Y)fail(1);
    for(i=0;i<n;++i) {
        if(!selected[i]&&(children[i].positionCalls||children[i].sizeCalls||children[i].typeCalls||children[i].centerCalls))fail(2);
        if((int)i==first && (children[i].positionCalls!=1 || children[i].sizeCalls!=1 || children[i].typeCalls || children[i].centerCalls))fail(3);
        if(selected[i]&&(int)i!=first && (children[i].typeCalls!=2 || children[i].centerCalls!=(children[i].kind()==0?2u:0u)))fail(4);
    }
    ++scenarios;
}
int main(){for(unsigned int n=0;n<=9;++n)for(unsigned int filter=0;filter<5;++filter)for(unsigned int mode=0;mode<17;++mode)run(n,filter,mode);if(scenarios!=850)fail(5);puts("00531560_TEST PASS");return 0;}
