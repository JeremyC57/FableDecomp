#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <new>
#ifndef _MSC_VER
#define __fastcall
#define __forceinline inline
#define __declspec(x)
#endif
#define FABLE_FASTCALL __fastcall
#define FABLE_STATIC_ASSERT(x)
#define FABLE_RETAIL_DEF_CONSTRUCTORS
#define FABLE_RETAIL_CWIDESTRING_CTOR
struct ClickableFixture;


#define FABLE_RETAIL_UI_ACTIONS
#define FABLE_STLPORT_UI_ACTIONS
#define FABLE_STLPORT_CONSTRUCTORS
#define FABLE_RETAIL_CWIDESTRING_COPY
#define FABLE_RETAIL_DEF_CLASS_BASE
#ifndef FABLE_ENGINE_NUISYSTEM_CCLICKABLE_H
#define FABLE_ENGINE_NUISYSTEM_CCLICKABLE_H
#ifndef FABLE_ENGINE_NUISYSTEM_CCHANGINGSTATECOMPONENT_H
#define FABLE_ENGINE_NUISYSTEM_CCHANGINGSTATECOMPONENT_H
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
#ifdef FABLE_STLPORT_UI_ACTIONS
#include <new>
#endif
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
#ifdef FABLE_STLPORT_UI_ACTIONS
protected:
    _List_node<_Tp>* _M_create_node(const _Tp& x) {
        _List_node<_Tp>* p=this->_M_node.allocate(1);
        new((void*)&p->_M_data) _Tp(x);
        return p;
    }
public:
    iterator insert(iterator position,const _Tp& x) {
        _List_node<_Tp>* tmp=_M_create_node(x);
        _List_node_base* n=position._M_node;
        _List_node_base* p=n->_M_prev;
        tmp->_M_next=n;tmp->_M_prev=p;p->_M_next=tmp;n->_M_prev=tmp;
        return iterator(tmp);
    }
    void push_back(const _Tp& x) { insert(end(),x); }
#endif
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
#ifdef FABLE_RETAIL_UI_ACTIONS
    CActionParam() : ActionParamType(UI_ACTION_PARAM_TYPE_NONE) { }
    CActionParam(EActionParamType type) : ActionParamType(type) { }
    virtual ~CActionParam() { }
#else
    CActionParam();
    virtual ~CActionParam();
#endif
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

namespace NUISystem {

class CChangingStateComponent : public CComponent {
public:
    enum EChangingStateComponentState {UI_NO_CHANGING=0,UI_CHILDREN_CHANGING=1,UI_INTERNAL_CHANGING=2,UI_LAST=4};
    CChangingStateComponent(long);
    virtual ~CChangingStateComponent();
    virtual void Initialise(); 
    virtual void Update(float); 
    virtual void ForceState(unsigned long, bool); 
    virtual void ChangeState(unsigned long, float); 
    virtual void ChangeState(unsigned long); 
    virtual NUISystem::CUIState * GetCurrentState(); 
    virtual bool HasState(unsigned long); 
    virtual bool ChangedState(); 
    virtual unsigned long GetCurrentStateIndex(); 
    virtual unsigned long GetTargetStateIndex() const; 
    virtual float GetUpdateTime() const; 
    virtual void AddChildDynamically(CCountedPointer<NUISystem::CComponent> &); 
    virtual bool IsPositionIndependent() const; 
    virtual bool IsZoomIndependent() const; 
    virtual void ProcessChangeState(); 
public:
    virtual NUISystem::CUIState * GetState(unsigned long); 
    virtual const NUISystem::CUIState * GetState(unsigned long) const; 
    virtual void UpdateStateChange(); 
    virtual bool ChangedStateLastUpdate(); 
    virtual bool InternalChanged(); 
protected:
    virtual bool AllChildrenChangedState(); 
    virtual void OnActivate(); 
    virtual void OnDeactivate(); 
protected:
    NUISystem::CChangingStateComponent::EChangingStateComponentState StatesBeingDone; 
    NUISystem::CChangingStateComponent::EChangingStateComponentState StatesDone; 
    std::list<enum NUISystem::CChangingStateComponent::EChangingStateComponentState,std::allocator<enum NUISystem::CChangingStateComponent::EChangingStateComponentState> > StatesToDo; 
    float ParentUpdateTime; 
    unsigned long CurrentState; 
    unsigned long TargetState; 
    unsigned long RequestedState; 
    float UpdateTime; 
    bool PreviousUpdateChanged; 
    unsigned long PreviousState; 
};
}
#endif

#ifndef FABLE_RETAIL_HEADER_CWIDESTRING
#define FABLE_RETAIL_HEADER_CWIDESTRING

class CWideStringData;
class CWideString {
    friend struct ::ClickableFixture;
public:
#if defined(FABLE_RETAIL_CWIDESTRING_CTOR)
    CWideString();
#endif
#ifdef FABLE_RETAIL_CWIDESTRING_COPY
    CWideString(const CWideString&);
#endif
    ~CWideString();
protected:
    CWideStringData* PStringData;
};
#endif

#ifndef FABLE_NUISYSTEM_ACTION_ENUM_H
#define FABLE_NUISYSTEM_ACTION_ENUM_H
namespace NUISystem {
enum EActionType {
    UI_ACTION_TYPE_NONE=0,
    UI_ACTION_TYPE_WIELD=1,
    UI_ACTION_TYPE_AUGMENT=2,
    UI_ACTION_TYPE_CHANGESTATE=3,
    UI_ACTION_TYPE_TAKE_BOAST=4,
    UI_ACTION_TYPE_TELEPORT=5,
    UI_ACTION_TYPE_TAKE_QUEST=6,
    UI_ACTION_TYPE_BUY_STAT=7,
    UI_ACTION_TYPE_BUY_ABILITY=8,
    UI_ACTION_TYPE_NEXT_SCREEN_OPTIONS=9,
    UI_ACTION_TYPE_NEXT_SCREEN_LIVE_AWARE=10,
    UI_ACTION_TYPE_NEXT_SCREEN_EXTRAS=11,
    UI_ACTION_TYPE_NEXT_SCREEN_AUDIO_OPTIONS=12,
    UI_ACTION_TYPE_NEXT_SCREEN_VIDEO_OPTIONS=13,
    UI_ACTION_TYPE_NEXT_SCREEN_CAMERA_OPTIONS=14,
    UI_ACTION_TYPE_NEW_GAME=15,
    UI_ACTION_TYPE_NEXT_SCREEN_PROFILES=16,
    UI_ACTION_TYPE_LOAD_GAME=17,
    UI_ACTION_TYPE_LOGIN=18,
    UI_ACTION_TYPE_FRIENDS=19,
    UI_ACTION_TYPE_APPEAR_OFFLINE=20,
    UI_ACTION_TYPE_DELETE=21,
    UI_ACTION_TYPE_ADD_CHILD=22,
    UI_ACTION_TYPE_ADD_CHILD_AUGMENTATION=23,
    UI_ACTION_TYPE_DELETE_ALL=24,
    UI_ACTION_TYPE_USE_ITEM=25,
    UI_ACTION_TYPE_ITEM_ASSIGN_LEFT=26,
    UI_ACTION_TYPE_ITEM_ASSIGN_RIGHT=27,
    UI_ACTION_TYPE_ITEM_ASSIGN_DOWN=28,
    UI_ACTION_TYPE_ITEM_ASSIGN_UP=29,
    UI_ACTION_TYPE_CLOTHING_WEAR=30,
    UI_ACTION_TYPE_UPDATE_MANNEQUIN_CLOTHING=31,
    UI_ACTION_TYPE_BUY=36,
    UI_ACTION_TYPE_SELL=37,
    UI_ACTION_TYPE_DELETE_PREVIOUS=38,
    UI_ACTION_TYPE_RESET_CLOTHING_MANNEQUIN=39,
    UI_ACTION_TYPE_SET_BRIGHTNESS=40,
    UI_ACTION_TYPE_SET_SOUND=41,
    UI_ACTION_TYPE_SET_MUSIC=42,
    UI_ACTION_TYPE_SET_CAMERA_ROTATION=43,
    UI_ACTION_TYPE_SET_CAMERA_UP_DOWN=44,
    UI_ACTION_TYPE_SET_VIBRATION=45,
    UI_ACTION_TYPE_SET_HEADPHONES=46,
    UI_ACTION_TYPE_SET_SUBTITLES=47,
    UI_ACTION_TYPE_OPEN_MAP=48,
    UI_ACTION_TYPE_OPEN_ABILITIES=49,
    UI_ACTION_TYPE_OPEN_WEAPONS=50,
    UI_ACTION_TYPE_OPEN_STATS=51,
    UI_ACTION_TYPE_OPEN_CLOTHING=52,
    UI_ACTION_TYPE_OPEN_ITEMS=53,
    UI_ACTION_TYPE_CLOSE_MAP=54,
    UI_ACTION_TYPE_CLOSE_ABILITIES=55,
    UI_ACTION_TYPE_CLOSE_WEAPONS=56,
    UI_ACTION_TYPE_CLOSE_STATS=57,
    UI_ACTION_TYPE_CLOSE_CLOTHING=58,
    UI_ACTION_TYPE_CLOSE_ITEMS=59,
    UI_ACTION_TYPE_CLOSE_MENU=60,
    UI_ACTION_TYPE_GUILD_SEAL_RECALL=61,
    UI_ACTION_TYPE_LOAD=62,
    UI_ACTION_TYPE_SAVE=63,
    UI_ACTION_TYPE_CLOSE_PAUSE_MENU=64,
    UI_ACTION_TYPE_PLAY_MOVIE=65,
    UI_ACTION_TYPE_NEXT_SCREEN_PROFILES_SAVED_GAMES=66,
    UI_ACTION_TYPE_NEXT_SCREEN_CREDITS=67,
    UI_ACTION_TYPE_CONSTRUCT_WEAPONS_LIST=68,
    UI_ACTION_TYPE_CONSTRUCT_CLOTHING_LIST=69,
    UI_ACTION_TYPE_CONSTRUCT_ITEMS_LIST=70,
    UI_ACTION_TYPE_CONSTRUCT_SKILLS_LIST=71,
    UI_ACTION_TYPE_CONSTRUCT_QUESTS_LIST=72,
    UI_ACTION_TYPE_CONSTRUCT_MAP_LIST=73,
    UI_ACTION_TYPE_CONSTRUCT_STATS_LIST=74,
    UI_ACTION_TYPE_CHANGE_CHILD_STATE=75,
    UI_ACTION_TYPE_CONSTRUCT_EXPERIENCE_LIST=76,
    UI_ACTION_TYPE_CONSTRUCT_PERSONALITY_LIST=77,
    UI_ACTION_TYPE_SET_HUD=78,
    UI_ACTION_TYPE_CLOSE_QUICK_ACCESS_MENU=79,
    UI_ACTION_TYPE_CLOSE_TELEPORT_MENU=80,
    UI_ACTION_TYPE_BUY_BULK=81,
    UI_ACTION_TYPE_SELL_BULK=82,
    UI_ACTION_TYPE_SET_TUTORIALS=83,
    UI_ACTION_TYPE_SET_SHOW_BUDDY_NAMES=84,
    UI_ACTION_TYPE_QUIT_TO_FRONT_END=85,
    UI_ACTION_TYPE_PREVIOUS_SCREEN=86,
    UI_ACTION_TYPE_ACCEPT_CHANGES=87,
    UI_ACTION_TYPE_SET_DIALOGUE=88,
    UI_ACTION_TYPE_XLIVE=89,
    UI_ACTION_TYPE_XLIVE_BACK=90,
    UI_ACTION_TYPE_LOGOUT=91,
    UI_ACTION_TYPE_DEMOS=92,
    UI_ACTION_TYPE_SET_GUILD_MASTER=93,
    UI_ACTION_TYPE_SET_HUD_TOOLTIPS=94,
    UI_ACTION_TYPE_UNDO_BUY_ABILITY=200,
    UI_ACTION_TYPE_UNDO_BUY_STAT=201,
    UI_ACTION_TYPE_GIVE_EXCLUSIVE_INPUT=202,
    UI_ACTION_TYPE_REMOVE_EXCLUSIVE_INPUT=203,
    UI_ACTION_TYPE_SEND_BACK_EVENT=204,
    UI_ACTION_TYPE_SHOW_SPELL_SELECTION=205,
    UI_ACTION_TYPE_SET_SPELL_TO_ASSIGN=206,
    UI_ACTION_TYPE_TELEPORT_TO_BOAST=207,
    UI_ACTION_TYPE_DROP_QUEST=208,
    UI_ACTION_TYPE_ASSIGN_SPELL=209,
    UI_ACTION_TYPE_UNASSIGN_SPELL=210,
    UI_ACTION_TYPE_RESET_ASSIGNED_SPELLS=211,
    UI_ACTION_TYPE_CHANGE_ALPHA=212,
    UI_ACTION_TYPE_CHANGE_NUMBER=213,
    UI_ACTION_TYPE_DELETE_PROFILE=214,
    UI_ACTION_TYPE_GO_TO_DELETE_PROFILE_SCREEN=215,
    UI_ACTION_TYPE_POP_MAP_STATE=216,
    UI_ACTION_TYPE_NEXT_SCREEN_INVALID_PROFILE=219,
    UI_ACTION_TYPE_NEXT_SCREEN_INVALID_SAVE=220,
    UI_ACTION_TYPE_FREE_SPACE_ON_T=221,
    UI_ACTION_TYPE_FREE_SPACE_ON_U=222,
    UI_ACTION_TYPE_FLASH_DPAD_DOWN=223,
    UI_ACTION_TYPE_FLASH_DPAD_UP=224,
    UI_ACTION_TYPE_FLASH_DPAD_LEFT=225,
    UI_ACTION_TYPE_FLASH_DPAD_RIGHT=226,
    UI_ACTION_TYPE_STOP_DPAD_FLASHING=227,
    UI_ACTION_TYPE_CONSTRUCT_LOGBOOK_LIST=228,
    UI_ACTION_TYPE_GO_TO_MAIN_MENU_FROM_START=229,
    UI_ACTION_TYPE_SEND_OWNED_EVENT=230,
    UI_ACTION_TYPE_SEND_EVENT=231,
    UI_ACTION_TYPE_MOVE_COMPONENT=232,
    UI_ACTION_TYPE_EXIT_LIVE_GUI=233,
    UI_ACTION_TYPE_OPEN_PC_SKILLS_MENU=234,
    UI_ACTION_TYPE_CONSTRUCT_EXPRESSIONS_LIST=235,
    UI_ACTION_TYPE_SCROLL_ACTIVE_LIST_UP=236,
    UI_ACTION_TYPE_SCROLL_ACTIVE_LIST_DOWN=237,
    UI_ACTION_TYPE_OPEN_PC_INVENTORY_MENU=238,
    UI_ACTION_TYPE_OPEN_PC_OPTIONS_MENU=239,
    UI_ACTION_TYPE_CONSTRUCT_PC_STYLE_CARDS_LIST=240,
    UI_ACTION_TYPE_OPEN_PC_BUY_TRADING_GOODS_LIST=241,
    UI_ACTION_TYPE_CLOSE_PC_BUY_TRADING_GOODS_LIST=242,
    UI_ACTION_TYPE_CONSTRUCT_MAGIC_LIST_PC=243,
    UI_ACTION_TYPE_ASSIGN_SPELL_PC=244,
    UI_ACTION_TYPE_OBSERVE_EVENT=245,
    UI_ACTION_TYPE_SET_SPELL_TO_ASSIGN_PC=246,
    UI_ACTION_TYPE_RESET_ASSIGNED_SPELLS_PC=247,
    UI_ACTION_TYPE_UNASSIGN_SPELL_PC=248,
    UI_ACTION_TYPE_IGNORE_EVENT=249,
    UI_ACTION_TYPE_NEXT_SCREEN_PROFILES_FOR_DELETE=250,
    UI_ACTION_TYPE_ASSIGN_EXPRESSION_ITEM_PC=251,
    UI_ACTION_TYPE_UNASSIGN_EXPRESSION_ITEM_PC=252,
    UI_ACTION_TYPE_SET_EXPRESSION_ITEM_TO_ASSIGN_PC=253,
    UI_ACTION_TYPE_OPEN_PC_MAP_MENU=254,
    UI_ACTION_TYPE_OPEN_PC_STATUS_MENU=255,
    UI_ACTION_TYPE_OPEN_PC_LOG_BOOK=256,
    UI_ACTION_TYPE_CONSTRUCT_PC_QUESTS_LIST=257,
    UI_ACTION_TYPE_SET_RESOLUTION=258,
    UI_ACTION_TYPE_SET_SHADOW_DETAIL=259,
    UI_ACTION_TYPE_SET_DECALS=260,
    UI_ACTION_TYPE_SET_WEATHER_EFFECTS=261,
    UI_ACTION_TYPE_SET_VERTICAL_SYNC=262,
    UI_ACTION_TYPE_SET_GLOW_EFFECTS=263,
    UI_ACTION_TYPE_SET_REVERSE_STEREO=264,
    UI_ACTION_TYPE_SET_ANTIALIASING=265,
    UI_ACTION_TYPE_SET_TEXTURE_DETAIL=266,
    UI_ACTION_TYPE_SET_MESH_RESOLUTION=267,
    UI_ACTION_TYPE_SET_WATER_REFLECTION=268,
    UI_ACTION_TYPE_SET_LANDSCAPE_DETAIL=269,
    UI_ACTION_TYPE_SET_PARTICLE_DETAIL=270,
    UI_ACTION_TYPE_ADD_OBSERVER=271,
    UI_ACTION_TYPE_SCROLL_ACTIVE_LIST_UNTIL_CHILD_CONTAINING=272,
    UI_ACTION_TYPE_GO_BACK=273,
    UI_ACTION_TYPE_CLOSE_ACTIVE_MENU=274,
    UI_ACTION_TYPE_REMOVE_OBSERVER=275,
    UI_ACTION_TYPE_TAVERN_GAME_BET_DOWN=276,
    UI_ACTION_TYPE_TAVERN_GAME_BET_UP=277,
    UI_ACTION_TYPE_RESPAWN=278,
    UI_ACTION_TYPE_CONTINUE=279,
    UI_ACTION_TYPE_OPEN_PC_SELL_TRADING_GOODS_LIST=280,
    UI_ACTION_TYPE_OPEN_PC_WANTED_TRADING_GOODS_LIST=281,
    UI_ACTION_TYPE_CONSTRUCT_PC_EXPERIENCE_LIST=282,
    UI_ACTION_TYPE_GO_TO_REDEFINE_KEYS_MENU=283,
    UI_ACTION_TYPE_RESET_KEYS=284,
    UI_ACTION_TYPE_ASSIGN_SPELL_TO_ITEM_SLOT_PC=285,
    UI_ACTION_TYPE_CONSTRUCT_STYLE_CARDS_LIST=286,
    UI_ACTION_TYPE_SCROLL_DESCRIPTION_DOWN=287,
    UI_ACTION_TYPE_SCROLL_DESCRIPTION_UP=288,
    UI_ACTION_TYPE_CONSTRUCT_PC_PERSONALITY_LIST=289,
    UI_ACTION_TYPE_SEND_OWNED_EVENT_FORCE_OBSERVATION=291,
    UI_ACTION_TYPE_GO_TO_MAIN_MENU_FROM_PROFILE_LIST=292,
    UI_ACTION_TYPE_NEW_PROFILE=293,
    UI_ACTION_TYPE_NEW_PROFILE_RETURN_PRESSED=294,
    UI_ACTION_TYPE_NEW_PROFILE_ESCAPE_PRESSED=295,
    UI_ACTION_TYPE_QUIT_GAME=296,
    UI_ACTION_TYPE_NEXT_SCREEN_OPTIONS_SUB_MENU=297,
    UI_ACTION_TYPE_SET_MESH_DETAIL=298,
    UI_ACTION_TYPE_SET_EFFECTS_DETAIL=299,
    UI_ACTION_TYPE_SET_SCREEN_ASPECT_RATIO=300,
    UI_ACTION_TYPE_APPLY_PROFILE_VALUES=301,
    UI_ACTION_TYPE_CLOSE_ACTIVE_TRADE_MENU=302,
    UI_ACTION_TYPE_SCROLL_ACTIVE_LIST_DOWN_MAX=303,
    UI_ACTION_TYPE_SCROLL_ACTIVE_LIST_UP_MAX=304,
    UI_ACTION_TYPE_SCROLL_DESCRIPTION_DOWN_MAX=305,
    UI_ACTION_TYPE_SCROLL_DESCRIPTION_UP_MAX=306,
    UI_ACTION_TYPE_SCROLL_DESCRIPTION_DOWN_ONE_PIXEL=307,
    UI_ACTION_TYPE_SCROLL_DESCRIPTION_UP_ONE_PIXEL=308,
    UI_ACTION_TYPE_CLOSE_TRADE_MENU_IF_LEAF=309,
    UI_ACTION_TYPE_EXIT_LIVE_GUI_IF_LEAF=310,
    UI_ACTION_TYPE_RESET_KEYS_WASD=311,
    UI_ACTION_TYPE_NEXT_SCREEN_OPTIONS_SCOREBOARD=312,
    UI_ACTION_TYPE_SET_CONTROL_METHOD=313,
    UI_ACTION_TYPE_GOTO_QUIT_PROMPT=314,
    UI_ACTION_TYPE_PC_QUIT_TO_FRONT_END=315,
    UI_ACTION_TYPE_PC_ACCEPT_VIDEO_CHANGES=316,
    UI_ACTION_TYPE_SET_REFRESH_RATE=317,
    UI_ACTION_TYPE_SET_CAMERA_SENSITIVITY=318,
    UI_ACTION_TYPE_SET_BOW_CAMERA=319,
    UI_ACTION_TYPE_SET_CAMERA_RESETTING=320,
    UI_ACTION_TYPE_GOTO_ABOUT_SCREEN=321,
    UI_ACTION_TYPE_CLOSE_FRAME=322,
    UI_ACTION_TYPE_SET_SHOW_TARGETING_STATUS=323,
    UI_ACTION_TYPE_RESTORE_DEFAULTS_GAMEPLAY=324,
    UI_ACTION_TYPE_RESTORE_DEFAULTS_VIDEO=325,
    UI_ACTION_TYPE_RESTORE_DEFAULTS_AUDIO=326,
    UI_ACTION_TYPE_SET_EXPRESSION_ITEM_TO_ASSIGN_SWAPPING_PC=327,
    UI_ACTION_TYPE_SET_SPELL_TO_ASSIGN_SWAPPING_PC=328,
    UI_ACTION_TYPE_SET_SPELL_TO_ASSIGN_IN_ITEMS_SWAPPING_PC=329,
    UI_ACTION_ADD_MESH_CHILD=2000,
    UI_ACTION_LOAD_WEAPON_DESC=2001,
    UI_ACTION_LOAD_CLOTHING_DESC=2002,
    UI_ACTION_LOAD_ITEM_DESC=2003,
    UI_ACTION_LOAD_WEAPON_MENU_ENTRY_NAME=2004,
    UI_ACTION_LOAD_CLOTHING_MENU_ENTRY_NAME=2005,
    UI_ACTION_LOAD_ITEM_MENU_ENTRY_NAME=2006,
    UI_ACTION_LOAD_SELL_ITEM_DESC=2007,
    UI_ACTION_LOAD_SELL_ACTION_MENU=2008,
    UI_ACTION_LOAD_BUY_ITEM_DESC=2009,
    UI_ACTION_LOAD_BUY_ACTION_MENU=2010,
    UI_ACTION_LOAD_WANTED_ITEM_DESC=2011,
    UI_ACTION_LOAD_QUICK_MENU_EXPRESSIONS_ICON=2012,
    UI_ACTION_LOAD_FILE_DESC=2013,
    UI_ACTION_LOAD_FILE_SAVEGAME_MINIMAP=2014,
    UI_ACTION_REMOVE_MESH_CHILD=2015,
    UI_ACTION_UNLOAD_WEAPON_DESC=2016,
    UI_ACTION_UNLOAD_CLOTHING_DESC=2017,
    UI_ACTION_UNLOAD_ITEM_DESC=2018,
    UI_ACTION_UNLOAD_SELL_ITEM_DESC=2019,
    UI_ACTION_UNLOAD_SELL_ACTION_MENU=2020,
    UI_ACTION_UNLOAD_BUY_ITEM_DESC=2021,
    UI_ACTION_UNLOAD_BUY_ACTION_MENU=2022,
    UI_ACTION_UNLOAD_WANTED_ITEM_DESC=2023,
    UI_ACTION_TYPE_CHEAT_MORALITY=3000,
    UI_ACTION_TYPE_CHEAT_RENOWN=3001,
    UI_ACTION_TYPE_CLOSE_BOAST_MENU=3002,
    UI_ACTION_TYPE_PLAY_SOUND=3003,
    UI_ACTION_TYPE_TAKE_QUEST_FOR_BOAST=3004,
    UI_ACTION_TYPE_OPEN_PC_MSN_CHAT=3005,
    UI_ACTION_TYPE_ACTIVATE_MSN_CONVERSATION=3006,
    UI_ACTION_TYPE_SEND_MESSAGE=3007,
    UI_ACTION_TYPE_SELECT_CONTACT=3008,
    UI_ACTION_TYPE_SCROLL_VIEWPORT_UP=3009,
    UI_ACTION_TYPE_SCROLL_VIEWPORT_DOWN=3010,
    UI_ACTION_TYPE_SCROLL_LIST_DOWN=3011,
    UI_ACTION_TYPE_SCROLL_LIST_UP=3012,
    UI_ACTION_TYPE_OPEN_SCOREBOARD=3013,
    UI_ACTION_TYPE_CHOOSE_CLAN=3014,
    UI_ACTION_TYPE_PHOTO_CAPTION=3015,
    UI_ACTION_TYPE_DISCARD_PHOTO=3016,
    UI_ACTION_TYPE_GO_TO_SCOREBOARD_SCREEN=3017,
    UI_ACTION_TYPE_ACTIVATE_EDITBOX=3018,
    UI_ACTION_TYPE_DEACTIVATE_EDITBOX=3019,
    UI_ACTION_TYPE_SET_EDITBOX_VALUES=3020,
    UI_ACTION_TYPE_SCOREBOARD_OK=3021,
    UI_ACTION_TYPE_LOAD_PHOTO=3022,
    UI_ACTION_TYPE_SCROLL_PHOTO_DOWN=3023,
    UI_ACTION_TYPE_SCROLL_PHOTO_UP=3024,
    UI_ACTION_TYPE_NEXT_SCREEN_START=3025,
    UI_ACTION_TYPE_OPEN_PHOTOJOURNAL=4000,
    UI_ACTION_TYPE_OPEN_PHOTO=4001,
    UI_ACTION_TYPE_EXIT_PHOTOJOURNAL_CAPTURE=4002
};
}
#endif

class C2DBoxI;
namespace NUISystem {

class CHoverable : public CChangingStateComponent {
public:
    CHoverable(long);
    CHoverable(const CHoverable&);
    virtual ~CHoverable();
    void GetDataFromDef();
    virtual void Initialise();
    virtual void Update(float);
    virtual void ProcessEvent(EEvent);
    virtual void AddActionOnHovered(CCountedPointer<CAction>&);
    virtual void AddActionOnUnhovered(CCountedPointer<CAction>&);
    virtual void IgnoreEvent(EEvent);
    virtual void ChangePosition(const C2DVector&,float,bool);
    virtual void RemoveFromHoveredList();
protected:
    virtual bool IsHovered(C2DBoxI&) const;
    virtual void OnHovered();
    virtual void OnUnhovered();
    virtual void UpdateHovering();
    virtual void OnDeactivate();
    void SetWasHovered(bool);
    long StateBeforeHovered;
    bool WasHovered;
    static std::list<CHoverable*> GAlreadyHovered;
    std::list<CCountedPointer<CAction> >* PActionsOnHovered;
    std::list<CCountedPointer<CAction> >* PActionsOnUnhovered;
};
class CClickable : public CHoverable {
public:
    CClickable(long);
    CClickable(const CClickable&);
    virtual ~CClickable();
    void GetDataFromDef();
    virtual void Initialise();
    virtual void Update(float);
    virtual void ProcessEvent(EEvent);
    virtual void AddActionOnLeftClicked(CCountedPointer<CAction>&);
    virtual void AddActionOnLeftHeld(CCountedPointer<CAction>&);
    virtual void AddActionOnRightClicked(CCountedPointer<CAction>&);
    virtual void AddActionOnLeftUnclicked(CCountedPointer<CAction>&);
    virtual bool IsClickable() const;
    virtual void OnLeftClicked();
    virtual void OnLeftUnclicked();
    virtual void OnRightClicked();
    virtual void OnRightUnclicked();
protected:
    virtual void OnHovered();
    virtual void OnUnhovered();
    virtual void OnDeactivate();
    long StateBeforeLeftClicked;
    bool WasLeftClicked;
    std::list<CCountedPointer<CAction> >* PActionsOnLeftClicked;
    std::list<CCountedPointer<CAction> >* PActionsOnLeftHeld;
    std::list<CCountedPointer<CAction> >* PActionsOnLeftUnclicked;
    long StateBeforeRightClicked;
    bool WasRightClicked;
    std::list<CCountedPointer<CAction> >* PActionsOnRightClicked;
    float InputDelay;
    float LastInputTime;
};

class CAction {
public:
    CAction();
    ~CAction();
    void AddParam(CActionParam* param) { UIActionParams.push_back(param); }
    EActionType ActionType;
    std::list<CActionParam*> UIActionParams;
    std::list<long> ThingActionParams;
protected:
    bool Valid;
};

class CActionParamString : public CActionParam {
public:
    CActionParamString(CWideString& value)
        : CActionParam(UI_ACTION_PARAM_TYPE_STRING),Value(value) { }
    virtual ~CActionParamString();
    CWideString Value;
};
}
#endif

#ifndef FABLE_ENGINE_NUISYSTEM_CUIDEF_H
#define FABLE_ENGINE_NUISYSTEM_CUIDEF_H
#ifndef FABLE_ENGINE_CDEFSTRING_H
#define FABLE_ENGINE_CDEFSTRING_H


class CCharString;
class CDefString {
public:
 CDefString():TablePos(-1){}
 CDefString(const CCharString&);
private:
 long TablePos;
};
#endif

#ifndef FABLE_ENGINE_CVECTORMAP_H
#define FABLE_ENGINE_CVECTORMAP_H



template<class T> class CArray : public std::vector<T,std::allocator<T> > { };
template<class K,class V> class CKeyPairCompareLess {
public:
    bool operator()(const std::pair<K,V>& a,const std::pair<K,V>& b) const { return a.first<b.first; }
};
template<class K,class V,class CompareType=CKeyPairCompareLess<K,V> >
class CVectorMap : public CArray<std::pair<K,V> > {
public:
    typedef std::pair<K,V> value_type;
    typedef value_type* iterator;
    typedef const value_type* const_iterator;
#ifdef FABLE_RETAIL_VISIBLE_LOWERBOUND
    __declspec(noinline) iterator LowerBound(const K& key) {
        iterator first=this->begin();
        long count=this->end()-first;
        while(count>0) {
            long half=count/2;
            iterator middle=first+half;
            if(middle->first<key) { first=middle+1; count-=half+1; }
            else count=half;
        }
        return first;
    }
    __declspec(noinline) const_iterator LowerBound(const K& key) const {
        const_iterator first=this->begin();
        long count=this->end()-first;
        while(count>0) {
            long half=count/2;
            const_iterator middle=first+half;
            if(middle->first<key) { first=middle+1; count-=half+1; }
            else count=half;
        }
        return first;
    }
#else
    iterator LowerBound(const K&);
    const_iterator LowerBound(const K&) const;
#endif
    iterator Find(const K& key) {
        iterator i=LowerBound(key);
        return i==this->end() || key<i->first ? this->end() : i;
    }
    const_iterator Find(const K& key) const {
        const_iterator i=LowerBound(key);
        return i==this->end() || key<i->first ? this->end() : i;
    }
private:
    CompareType Compare;
    bool Dirty;
};
#endif

#define FABLE_PDCB_TYPED_MEMBERS
#ifndef FABLE_ENGINE_CPARENTDEFCLASSBASE_H
#define FABLE_ENGINE_CPARENTDEFCLASSBASE_H

























#ifdef FABLE_RETAIL_DEF_CLASS_BASE
#include <stddef.h>

#ifndef FABLE_ENGINE_CDEFCLASSBASE_H
#define FABLE_ENGINE_CDEFCLASSBASE_H























#ifdef FABLE_RETAIL_DEF_CLASS_BASE
#include <stddef.h>

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
#else 
#include <stddef.h>


class CResourceList;

#pragma pack(push, 1)
class CResource {
public:
    void*          __vftable;                                
    unsigned char  _pad_0x04[0x4];                           
    CResourceList* ResourceList;                             
    CResource*     PrevResource;                             
    CResource*     NextResource;                             
    unsigned long  ResourceSize;                             
    unsigned long  LastUsedFrame;                            
};
#pragma pack(pop)


#endif 

#endif 


class CDefPointeeBase : public CResource {
public:
    virtual ~CDefPointeeBase() { }                                       
};

#endif 

class CCharString;
class CDefString;
class CDefinitionManager;
class CPersistContext;
class CDataInputStream;
class CDataOutputStream;
class CValidateDefinitionInitBase;
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class C> class CVectorMap;
template <class T> class CDefPointer;
#pragma pack(push, 1)
class CDefClassBase : public CDefPointeeBase {
#ifdef FABLE_RETAIL_DEF_CONSTRUCTORS
private:
    CDefClassBase(); 
    friend class CParentDefClassBase;
#endif
public:
    class CSubDefInfo {                                                  
    public:
        long DefIndex;                                                   
        long OriginalParentDefIndex;                                     
    };
    CDefClassBase(const CDefClassBase&);                                 
    CDefClassBase& operator=(const CDefClassBase&);                      
    template <class T> bool GetSubDef(CDefPointer<const T>*) const;      
    bool AreDefaultValsApplied() const { return DefaultValsApplied; }    
    void SetDefaultValsApplied(bool value) { DefaultValsApplied = value; }   
    virtual ~CDefClassBase() { }                                         
    virtual bool UnloadResource();                                       
    virtual CDefString GetInstantiationName() const;                     
    virtual void SetInstantiationName(CDefString);                       
    virtual void AddSubDef(const CCharString&, long, long);              
    virtual void RemoveSubDef(const CCharString&);                       
    virtual CSubDefInfo* GetPSubDefInfo(const CCharString&);             
    virtual const CSubDefInfo* GetPSubDefInfo(const CCharString&) const; 
    virtual bool IsParentDef() const;                                    
    virtual void PostTransfer();                                         
    virtual void Transfer(CPersistContext&) = 0;                         
    virtual void Copy(const CDefClassBase*) = 0;                         
    virtual unsigned long GetSizeofClass() = 0;                          
    virtual void InternalSaveBinary(CDataOutputStream&);                 
    virtual void InternalLoadBinary(CDataInputStream&);                  
    virtual void Validate(const CValidateDefinitionInitBase&, const CCharString&) const;  
    virtual CVectorMap<unsigned long, CSubDefInfo, CKeyPairCompareLess<unsigned long, CSubDefInfo> >*
        GetPSubDefInfoMap();                                             
    CDefinitionManager* PDefManager;                                     
    unsigned long       GlobalIndex;                                     
    unsigned char       Setup : 1;                                       
    unsigned char       Template : 1;
    unsigned char       DefaultValsApplied : 1;
};
#pragma pack(pop)
#else 
#include <stddef.h>


class CDefinitionManager;

#pragma pack(push, 1)
class CDefClassBase {
public:
    void*               __vftable;                           
    unsigned char       _base_0x04[0x18];                    
    CDefinitionManager* PDefManager;                         
    unsigned long       GlobalIndex;                         
    unsigned char       DefaultValsApplied[0x1];             
};
#pragma pack(pop)


#endif 

#endif 

class CDataInputStream;
class CDataOutputStream;
#pragma pack(push, 1)
class CParentDefClassBase : public CDefClassBase {
public:
    CParentDefClassBase();                                               
    CParentDefClassBase(const CParentDefClassBase&);                     
    CParentDefClassBase& operator=(const CParentDefClassBase&);          
    virtual ~CParentDefClassBase();                                      
    virtual CDefString GetInstantiationName() const;                     
    virtual void SetInstantiationName(CDefString);                       
    virtual void AddSubDef(const CCharString&, long, long);              
    virtual void RemoveSubDef(const CCharString&);                       
    virtual CSubDefInfo* GetPSubDefInfo(const CCharString&);             
    virtual const CSubDefInfo* GetPSubDefInfo(const CCharString&) const; 
    virtual bool IsParentDef() const;                                    
    virtual void InternalSaveBinary(CDataOutputStream&);                 
    virtual void InternalLoadBinary(CDataInputStream&);                  
    virtual CVectorMap<unsigned long, CSubDefInfo, CKeyPairCompareLess<unsigned long, CSubDefInfo> >*
        GetPSubDefInfoMap();                                             
#ifdef FABLE_PDCB_TYPED_MEMBERS
    CDefString InstantiationName;                                        
    CVectorMap<unsigned long, CSubDefInfo, CKeyPairCompareLess<unsigned long, CSubDefInfo> > SubDefInfoMap;   
#else
    long          InstantiationName_TablePos;                            
    unsigned char SubDefInfoMap[0x10];                                   
#endif
};
#pragma pack(pop)
#else 
#include <stddef.h>


#pragma pack(push, 1)
class CParentDefClassBase {
public:
    void*         __vftable;                                 
    unsigned char _base_0x04[0x21];                          
    long          InstantiationName_TablePos;                
    unsigned char SubDefInfoMap[0x10];                       
};
#pragma pack(pop)


#endif 

#endif 




namespace NUISystem {


class CUIDef : public CParentDefClassBase {
public:
    CUIDef();
    CUIDef(const CUIDef&);
    virtual ~CUIDef();
    virtual void Transfer(CPersistContext&);
    virtual void Copy(const CDefClassBase*);
    virtual unsigned long GetSizeofClass();
    unsigned char _pad_39[3];
    NUISystem::EType Type; 
    unsigned char States[12]; 
    unsigned long MeshIndex; 
    CDefString Font; 
    CWideString TextValue; 
    float Height; 
    float Width; 
    unsigned char ExpansionType[4]; 
    unsigned char Sprites[12]; 
    unsigned char Children[12]; 
    unsigned char HorizontalSeparations[12]; 
    unsigned char VerticalSeparations[12]; 
    unsigned char NonScrollingChildren[12]; 
    unsigned char MeshType[4]; 
    unsigned char TextWindow[16]; 
    long Layer; 
    float Angle; 
    bool PositionIsCenter; 
    bool TextLineBreak; 
    bool ScaleText; 
    bool Independant; 
    float ScrollingSpeed; 
    NUISystem::EActionType Action; 
    NUISystem::EActionType ActionOnBack; 
    NUISystem::EActionType ActionOnSelected; 
    NUISystem::EActionType ActionOnUnselected; 
    NUISystem::EActionType ActionOnDestruction; 
    NUISystem::EActionType PreAction; 
    NUISystem::EActionType ActionOnDroppedNowhere; 
    NUISystem::EActionType ActionOnLeftClicked; 
    NUISystem::EActionType ActionOnLeftUnclicked; 
    NUISystem::EActionType ActionOnLeftHeld; 
    NUISystem::EActionType ActionOnRightClicked; 
    NUISystem::EActionType ActionOnDropped; 
    NUISystem::EActionType ActionOnDraggedUp; 
    NUISystem::EActionType ActionOnDraggedDown; 
    NUISystem::EActionType ActionOnLeftClickedUnder; 
    NUISystem::EActionType ActionOnLeftClickedAbove; 
    unsigned char Up[12]; 
    unsigned char Forward[12]; 
    unsigned char RotationAxis[12]; 
    float RotationSpeed; 
    unsigned long AnimationIndex; 
    long DownArrow; 
    long UpArrow; 
    long UpLimit; 
    long DownLimit; 
    bool Wrapping; 
    bool Inverted; 
    unsigned char PositionOffset[8]; 
    unsigned char AlphaOffset; 
    bool Scrolling; 
    unsigned char ViewArea[16]; 
    bool ComputeOffsetsOnActivate; 
    unsigned char _pad_15d[3];
    NUISystem::EState ParentStatePrerequisite; 
    unsigned char Dimensions[8]; 
    unsigned char Min[8]; 
    unsigned char Max[8]; 
    unsigned char Step[8]; 
    float InputDelay; 
    bool DrawFromViewport; 
    unsigned char _pad_189[3];
    long SliderLeft; 
    long SliderRight; 
    unsigned long TextBankIndex; 
    long ActionText; 
    long KeyText; 
    long UndefinedWarning; 
    long Redefiner; 
    unsigned char ActionMap[12]; 
    unsigned char ActionMapAliases[12]; 
    unsigned char ActionOrder[12]; 
    bool EditBoxParentIsButton; 
    bool PasswordBox; 
    unsigned char _pad_1ce[2];
    long EditBoxCharLimit; 
    bool EditBoxUsesIME; 
    bool DisallowSpaceAsFirstChar; 
    unsigned char _pad_1d6[2];
    CWideString MovieFilename; 
    bool LayerIndependant; 
    unsigned char _pad_1dd[3];
    unsigned char SwappingStates[12]; 
    unsigned char SwappingTimes[12]; 
    bool BastardChild; 
    unsigned char _pad_1f9[3];
    unsigned char Alignement[4]; 
    bool RandomSwap; 
    unsigned char _pad_201[3];
    long HoveredState; 
    bool UseRelativeZoom; 
    bool UseRelativePosition; 
    bool PartOfListTree; 
    unsigned char _pad_20b[1];
    long LeftClickedState; 
    long RightClickedState; 
    unsigned char ShapeChildren[12]; 
    bool UseViewArea; 
    bool PCStyle; 
    unsigned char _pad_222[2];
    unsigned char Sprite2DFlag[4]; 
};
}
#endif

#ifndef FABLE_ENGINE_CDEFPOINTER_H
#define FABLE_ENGINE_CDEFPOINTER_H























#ifndef FABLE_RETAIL_DEF_CLASS_BASE
#error engine/CDefPointer.h is the retail def model: define FABLE_RETAIL_DEF_CLASS_BASE before including it
#else


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

namespace NUISystem {
void CClickable::GetDataFromDef()
{
    CDefPointer<const CUIDef> pdefinition=LoadAndReturnDef();
    InputDelay=pdefinition->InputDelay;
    if(pdefinition->ActionOnLeftClicked != UI_ACTION_TYPE_NONE) {
        CCountedPointer<CAction> paction=new CAction;
        paction->ActionType=pdefinition->ActionOnLeftClicked;
        if(paction->ActionType == UI_ACTION_TYPE_PLAY_MOVIE) {
            CWideString nonConstFilename=pdefinition->MovieFilename;
            paction->AddParam(new CActionParamString(nonConstFilename));
        }
        AddActionOnLeftClicked(paction);
    }
    if(pdefinition->ActionOnLeftUnclicked != UI_ACTION_TYPE_NONE) {
        CCountedPointer<CAction> paction=new CAction;
        paction->ActionType=pdefinition->ActionOnLeftUnclicked;
        if(paction->ActionType == UI_ACTION_TYPE_PLAY_MOVIE) {
            CWideString nonConstFilename=pdefinition->MovieFilename;
            paction->AddParam(new CActionParamString(nonConstFilename));
        }
        AddActionOnLeftUnclicked(paction);
    }
    if(pdefinition->ActionOnLeftHeld != UI_ACTION_TYPE_NONE) {
        CCountedPointer<CAction> paction=new CAction;
        paction->ActionType=pdefinition->ActionOnLeftHeld;
        if(paction->ActionType == UI_ACTION_TYPE_PLAY_MOVIE) {
            CWideString nonConstFilename=pdefinition->MovieFilename;
            paction->AddParam(new CActionParamString(nonConstFilename));
        }
        AddActionOnLeftHeld(paction);
    }
    if(pdefinition->ActionOnRightClicked != UI_ACTION_TYPE_NONE) {
        CCountedPointer<CAction> paction=new CAction;
        paction->ActionType=pdefinition->ActionOnRightClicked;
        if(paction->ActionType == UI_ACTION_TYPE_PLAY_MOVIE) {
            CWideString nonConstFilename=pdefinition->MovieFilename;
            paction->AddParam(new CActionParamString(nonConstFilename));
        }
        AddActionOnRightClicked(paction);
    }
}
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

C2DVector NUISystem::CComponent::GetDynamicSize() const { throw 1; }
void NUISystem::CChangingStateComponent::Initialise() { throw 1; }
void NUISystem::CChangingStateComponent::Update(float) { throw 1; }
void NUISystem::CChangingStateComponent::ForceState(unsigned long, bool) { throw 1; }
void NUISystem::CChangingStateComponent::ChangeState(unsigned long, float) { throw 1; }
void NUISystem::CChangingStateComponent::ChangeState(unsigned long) { throw 1; }
NUISystem::CUIState * NUISystem::CChangingStateComponent::GetCurrentState() { throw 1; }
bool NUISystem::CChangingStateComponent::HasState(unsigned long) { throw 1; }
bool NUISystem::CChangingStateComponent::ChangedState() { throw 1; }
unsigned long NUISystem::CChangingStateComponent::GetCurrentStateIndex() { throw 1; }
unsigned long NUISystem::CChangingStateComponent::GetTargetStateIndex() const { throw 1; }
float NUISystem::CChangingStateComponent::GetUpdateTime() const { throw 1; }
void NUISystem::CChangingStateComponent::AddChildDynamically(CCountedPointer<NUISystem::CComponent> &) { throw 1; }
bool NUISystem::CChangingStateComponent::IsPositionIndependent() const { throw 1; }
bool NUISystem::CChangingStateComponent::IsZoomIndependent() const { throw 1; }
void NUISystem::CChangingStateComponent::ProcessChangeState() { throw 1; }
NUISystem::CUIState * NUISystem::CChangingStateComponent::GetState(unsigned long) { throw 1; }
const NUISystem::CUIState * NUISystem::CChangingStateComponent::GetState(unsigned long) const { throw 1; }
void NUISystem::CChangingStateComponent::UpdateStateChange() { throw 1; }
bool NUISystem::CChangingStateComponent::ChangedStateLastUpdate() { throw 1; }
bool NUISystem::CChangingStateComponent::InternalChanged() { throw 1; }
bool NUISystem::CChangingStateComponent::AllChildrenChangedState() { throw 1; }
void NUISystem::CChangingStateComponent::OnActivate() { throw 1; }
void NUISystem::CChangingStateComponent::OnDeactivate() { throw 1; }
NUISystem::CChangingStateComponent::~CChangingStateComponent() { }
void NUISystem::CHoverable::Initialise() { throw 1; }
void NUISystem::CHoverable::Update(float) { throw 1; }
void NUISystem::CHoverable::ProcessEvent(EEvent) { throw 1; }
void NUISystem::CHoverable::AddActionOnHovered(CCountedPointer<CAction>&) { throw 1; }
void NUISystem::CHoverable::AddActionOnUnhovered(CCountedPointer<CAction>&) { throw 1; }
void NUISystem::CHoverable::IgnoreEvent(EEvent) { throw 1; }
void NUISystem::CHoverable::ChangePosition(const C2DVector&,float,bool) { throw 1; }
void NUISystem::CHoverable::RemoveFromHoveredList() { throw 1; }
bool NUISystem::CHoverable::IsHovered(C2DBoxI&) const { throw 1; }
void NUISystem::CHoverable::OnHovered() { throw 1; }
void NUISystem::CHoverable::OnUnhovered() { throw 1; }
void NUISystem::CHoverable::UpdateHovering() { throw 1; }
void NUISystem::CHoverable::OnDeactivate() { throw 1; }
NUISystem::CHoverable::~CHoverable() { }
void NUISystem::CClickable::Initialise() { throw 1; }
void NUISystem::CClickable::Update(float) { throw 1; }
void NUISystem::CClickable::ProcessEvent(EEvent) { throw 1; }
void NUISystem::CClickable::AddActionOnLeftClicked(CCountedPointer<CAction>&) { throw 1; }
void NUISystem::CClickable::AddActionOnLeftHeld(CCountedPointer<CAction>&) { throw 1; }
void NUISystem::CClickable::AddActionOnRightClicked(CCountedPointer<CAction>&) { throw 1; }
void NUISystem::CClickable::AddActionOnLeftUnclicked(CCountedPointer<CAction>&) { throw 1; }
bool NUISystem::CClickable::IsClickable() const { throw 1; }
void NUISystem::CClickable::OnLeftClicked() { throw 1; }
void NUISystem::CClickable::OnLeftUnclicked() { throw 1; }
void NUISystem::CClickable::OnRightClicked() { throw 1; }
void NUISystem::CClickable::OnRightUnclicked() { throw 1; }
void NUISystem::CClickable::OnHovered() { throw 1; }
void NUISystem::CClickable::OnUnhovered() { throw 1; }
void NUISystem::CClickable::OnDeactivate() { throw 1; }
NUISystem::CClickable::~CClickable() { }

NUISystem::CChangingStateComponent::CChangingStateComponent(long n) : CComponent(n) { }
NUISystem::CHoverable::CHoverable(long n) : CChangingStateComponent(n) { }
NUISystem::CClickable::CClickable(long n) : CHoverable(n) { }
bool CResource::UnloadResource() { throw 1; }
unsigned long CResource::GetUnloadableMemory() const { throw 1; }
bool CResource::IsResourceLoaded() const { throw 1; }
void CResource::OnResourceLoaded() { throw 1; }
bool CResource::CanAbortResourceLoad(long) const { throw 1; }
void CResource::OnResourceLoadAborted() { throw 1; }
unsigned long CResource::GetDebugMemoryInUse() const { throw 1; }
void CResource::ResetDebugMemoryInUse() { throw 1; }
bool CDefClassBase::UnloadResource() { throw 1; }
CDefString CDefClassBase::GetInstantiationName() const { throw 1; }
void CDefClassBase::SetInstantiationName(CDefString) { throw 1; }
void CDefClassBase::AddSubDef(const CCharString&, long, long) { throw 1; }
void CDefClassBase::RemoveSubDef(const CCharString&) { throw 1; }
CDefClassBase::CSubDefInfo* CDefClassBase::GetPSubDefInfo(const CCharString&) { throw 1; }
const CDefClassBase::CSubDefInfo* CDefClassBase::GetPSubDefInfo(const CCharString&) const { throw 1; }
bool CDefClassBase::IsParentDef() const { throw 1; }
void CDefClassBase::PostTransfer() { throw 1; }
void CDefClassBase::InternalSaveBinary(CDataOutputStream&) { throw 1; }
void CDefClassBase::InternalLoadBinary(CDataInputStream&) { throw 1; }
void CDefClassBase::Validate(const CValidateDefinitionInitBase&, const CCharString&) const { throw 1; }
CDefString CParentDefClassBase::GetInstantiationName() const { throw 1; }
void CParentDefClassBase::SetInstantiationName(CDefString) { throw 1; }
void CParentDefClassBase::AddSubDef(const CCharString&, long, long) { throw 1; }
void CParentDefClassBase::RemoveSubDef(const CCharString&) { throw 1; }
CDefClassBase::CSubDefInfo* CParentDefClassBase::GetPSubDefInfo(const CCharString&) { throw 1; }
const CDefClassBase::CSubDefInfo* CParentDefClassBase::GetPSubDefInfo(const CCharString&) const { throw 1; }
bool CParentDefClassBase::IsParentDef() const { throw 1; }
void CParentDefClassBase::InternalSaveBinary(CDataOutputStream&) { throw 1; }
void CParentDefClassBase::InternalLoadBinary(CDataInputStream&) { throw 1; }

CVectorMap<unsigned long,CDefClassBase::CSubDefInfo,CKeyPairCompareLess<unsigned long,CDefClassBase::CSubDefInfo> >* CDefClassBase::GetPSubDefInfoMap() { throw 1; }
CVectorMap<unsigned long,CDefClassBase::CSubDefInfo,CKeyPairCompareLess<unsigned long,CDefClassBase::CSubDefInfo> >* CParentDefClassBase::GetPSubDefInfoMap() { throw 1; }
void CIVCountedPointeeBase::Delete() const { delete this; }
CResource::CResource() { IVRefCount=0; }
CResource::~CResource() { }
CDefClassBase::CDefClassBase() { }
CParentDefClassBase::CParentDefClassBase() { }
CParentDefClassBase::~CParentDefClassBase() { }
static unsigned int liveActions=0,liveParams=0,wideCopies=0,wideDestructions=0,defsDeleted=0;
CWideString::CWideString() : PStringData(0) { }
CWideString::CWideString(const CWideString& x) : PStringData(x.PStringData) { ++wideCopies; }
CWideString::~CWideString() { ++wideDestructions; }
namespace std {
template<class T,class A> void _List_base<T,A>::clear() {
    _List_node_base* p=_M_node._M_data->_M_next;
    while(p!=_M_node._M_data) { _List_node_base* next=p->_M_next;((_List_node<T>*)p)->_M_data.~T();free(p);p=next; }
    _M_node._M_data->_M_next=_M_node._M_data;_M_node._M_data->_M_prev=_M_node._M_data;
}
}
namespace NUISystem {
CUIDef::CUIDef() { }
CUIDef::~CUIDef() { ++defsDeleted; }
void CUIDef::Transfer(CPersistContext&) { throw 1; }
void CUIDef::Copy(const CDefClassBase*) { throw 1; }
unsigned long CUIDef::GetSizeofClass() { return sizeof(CUIDef); }
CAction::CAction() : ActionType(UI_ACTION_TYPE_NONE),Valid(true) { ++liveActions; }
CAction::~CAction() { for(std::list<CActionParam*>::iterator i=UIActionParams.begin();i!=UIActionParams.end();++i)delete *i;--liveActions; }
CActionParamString::~CActionParamString() { --liveParams; }
}
static void check(bool ok,int id) {if(!ok){fprintf(stderr,"clickable check %d failed\n",id);exit(1);}}
struct ClickableFixture : NUISystem::CClickable {
    NUISystem::CUIDef* definition;
    NUISystem::EActionType expected[4];
    CCountedPointer<NUISystem::CAction> retained[4];
    bool retain,finalReference;
    unsigned int calls[4],sequence,lastSlot;
    static int movieToken;
    ClickableFixture() : CClickable(0),definition(0),retain(false),finalReference(false),sequence(0),lastSlot(0) { memset(calls,0,sizeof calls); }
    virtual CDefPointer<const NUISystem::CUIDef> LoadAndReturnDef() const {
        CDefPointer<const NUISystem::CUIDef> result;result.Object=definition;result.Object->AddRef();return result;
    }
    void accept(unsigned int slot,CCountedPointer<NUISystem::CAction>& action) {
        check(action.Info && action.Info->RefCount==1,10);
        check(action->ActionType==expected[slot]&&expected[slot]!=NUISystem::UI_ACTION_TYPE_NONE,11);
        check(!calls[slot]&&(!sequence||slot>lastSlot),12);++calls[slot];++sequence;lastSlot=slot;
        if(expected[slot]==NUISystem::UI_ACTION_TYPE_PLAY_MOVIE) {
            std::list<NUISystem::CActionParam*>::iterator i=action->UIActionParams.begin();
            check(i!=action->UIActionParams.end(),13);
            NUISystem::CActionParam* param=*i;++i;check(i==action->UIActionParams.end(),14);
            check(param->ActionParamType==NUISystem::UI_ACTION_PARAM_TYPE_STRING,15);
            NUISystem::CActionParamString* text=static_cast<NUISystem::CActionParamString*>(param);
            check(text->Value.PStringData==(CWideStringData*)&movieToken,16);++liveParams;
        } else check(action->UIActionParams.empty(),17);
        check(action->ThingActionParams.empty(),18);
        if(retain)retained[slot]=action;
    }
    virtual void AddActionOnLeftClicked(CCountedPointer<NUISystem::CAction>& a) {accept(0,a);}
    virtual void AddActionOnLeftUnclicked(CCountedPointer<NUISystem::CAction>& a) {accept(1,a);}
    virtual void AddActionOnLeftHeld(CCountedPointer<NUISystem::CAction>& a) {accept(2,a);}
    virtual void AddActionOnRightClicked(CCountedPointer<NUISystem::CAction>& a) {accept(3,a);}
    void run(unsigned int combination,bool keep,bool last) {
        retain=keep;finalReference=last;definition=new NUISystem::CUIDef;
        definition->IVRefCount=last?0:1;definition->InputDelay=1.25f;definition->MovieFilename.PStringData=(CWideStringData*)&movieToken;
        unsigned int n=combination,active=0,movies=0;for(unsigned int i=0;i<4;++i){unsigned int v=n%3;n/=3;expected[i]=v==0?NUISystem::UI_ACTION_TYPE_NONE:v==1?NUISystem::UI_ACTION_TYPE_PLAY_MOVIE:(NUISystem::EActionType)1;if(v)++active;if(v==1)++movies;}
        definition->ActionOnLeftClicked=expected[0];definition->ActionOnLeftUnclicked=expected[1];definition->ActionOnLeftHeld=expected[2];definition->ActionOnRightClicked=expected[3];
        InputDelay=-2.0f;LastInputTime=8.0f;wideCopies=wideDestructions=0;unsigned int deletedBefore=defsDeleted;
        GetDataFromDef();
        check(InputDelay==1.25f&&LastInputTime==8.0f,20);check(sequence==active,21);
        for(unsigned int j=0;j<4;++j)check(calls[j]==(expected[j]!=NUISystem::UI_ACTION_TYPE_NONE?1u:0u),22);
        check(wideCopies==movies*2,23);check(liveActions==(keep?active:0)&&liveParams==(keep?movies:0),24);
        if(last){check(defsDeleted==deletedBefore+1,25);definition=0;}else{check(definition->IVRefCount==1,26);delete definition;definition=0;}
        for(unsigned int k=0;k<4;++k)retained[k].Reset(0);
        check(liveActions==0&&liveParams==0,27);
    }
};
int ClickableFixture::movieToken=9;
int main(){for(unsigned int combination=0;combination<81;++combination)for(unsigned int retain=0;retain<2;++retain)for(unsigned int last=0;last<2;++last){ClickableFixture f;f.run(combination,retain!=0,last!=0);}puts("0055b040_TEST PASS");return 0;}
