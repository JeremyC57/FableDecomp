#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <new>
#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#define __stdcall
#define __declspec(x)
#define __forceinline inline
#define __int64 long
#define FABLE_JOIN2(a,b) a##b
#define FABLE_JOIN(a,b) FABLE_JOIN2(a,b)
#define FABLE_STATIC_ASSERT(x) typedef char FABLE_JOIN(check_size_,__LINE__)[1]
#else
#define FABLE_JOIN2(a,b) a##b
#define FABLE_JOIN(a,b) FABLE_JOIN2(a,b)
#define FABLE_STATIC_ASSERT(x) typedef char FABLE_JOIN(check_size_,__LINE__)[(x)?1:-1]
#endif
#define FABLE_FASTCALL __fastcall
#define private public
#define protected public



























class CBaseClass {
public:
    CBaseClass();                                                        
    virtual ~CBaseClass();                                               
};
FABLE_STATIC_ASSERT(sizeof(CBaseClass) == 0x4);

class CBaseObjectPointer;
class CBaseObject : public CBaseClass {
protected:
    CBaseObject();                                                       
    CBaseObject(const CBaseObject&);                                     
    CBaseObject& operator=(const CBaseObject&);                          
public:
    void ClearIntelligentPointers();                                     
    virtual void Validate() const;                                       
protected:
    virtual ~CBaseObject();                                              
    void InternalValidate() const;                                       
public:
    CBaseObjectPointer* IntelligentPointer;                              
};
FABLE_STATIC_ASSERT(sizeof(CBaseObject) == 0x8);










































class CCPPointerInfo {
public:
    unsigned long RefCount;                                              
    void (FABLE_FASTCALL* DeleteFunc)(void*);                             
    void* Data;                                                          
    CCPPointerInfo(unsigned long refCount, void (FABLE_FASTCALL* deleteFunc)(void*), void* data)
        : RefCount(refCount), DeleteFunc(deleteFunc), Data(data) { }     
};
FABLE_STATIC_ASSERT(sizeof(CCPPointerInfo) == 0xc);

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



#include <stddef.h>
#include <stdlib.h>
#include <new>
#include <string.h>
namespace std {
struct __true_type { };
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
template<class T> struct __type_traits { typedef __false_type has_trivial_assignment_operator; typedef __false_type has_trivial_destructor;
};
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
template<class _Tp> class allocator {
public:
    allocator() { }
    allocator(const allocator&) { }
    _Tp* allocate(size_t n,const void* =0) { return n?(_Tp*)malloc(n*sizeof(_Tp)):0; }
    void deallocate(_Tp* p,size_t) { if(p) free(p); }
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
    ~vector() { _Destroy(this->_M_start,this->_M_finish); }
    iterator begin() { return this->_M_start; }
    const_iterator begin() const { return this->_M_start; }
    iterator end() { return this->_M_finish; }
    const_iterator end() const { return this->_M_finish; }
    size_type size() const { return size_type(this->_M_finish-this->_M_start); }
    bool empty() const { return this->_M_start==this->_M_finish; }
    _Tp& operator[](size_type n) { return *(begin()+n); }
    const _Tp& operator[](size_type n) const { return *(begin()+n); }
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
    __declspec(noinline)
    void _M_insert_overflow(iterator position,const _Tp& x,const __true_type&,size_type fillLength,bool atEnd) {
        const size_type oldSize=size();const size_type len=oldSize+(max)(oldSize,fillLength);
        _Tp* newStart=this->_M_end_of_storage.allocate(len);
        _Tp* newFinish=(_Tp*)__copy_trivial(this->_M_start,position,newStart);
        newFinish=fill_n(newFinish,fillLength,x);
        if(!atEnd) newFinish=(_Tp*)__copy_trivial(position,this->_M_finish,newFinish);
        _M_clear();_M_set(newStart,newFinish,newStart+len);
    }
protected:
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
};
template<class _T1,class _T2> struct pair {
    typedef _T1 first_type;
    typedef _T2 second_type;
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
    _List_base(const _Alloc&) : _M_node(allocator<_List_node<_Tp> >(),0) {
        _List_node<_Tp>* n=_M_node.allocate(1);n->_M_next=n;n->_M_prev=n;_M_node._M_data=n;
    }
    ~_List_base() { clear();_M_node.deallocate(_M_node._M_data,1); }
    _STLP_alloc_proxy<_List_node<_Tp>*,_List_node<_Tp>,allocator<_List_node<_Tp> > > _M_node;
    void clear() {
        _List_node<_Tp>* cur=(_List_node<_Tp>*)_M_node._M_data->_M_next;
        while(cur!=_M_node._M_data) {
            _List_node<_Tp>* tmp=cur;
            cur=(_List_node<_Tp>*)cur->_M_next;
            tmp->_M_data.~_Tp();
            _M_node.deallocate(tmp,1);
        }
        _M_node._M_data->_M_next=_M_node._M_data;
        _M_node._M_data->_M_prev=_M_node._M_data;
    }
};
template<class _Tp,class _Alloc=allocator<_Tp> > class list : public _List_base<_Tp,_Alloc> {
public:
    explicit list(const _Alloc& a=_Alloc()) : _List_base<_Tp,_Alloc>(a) { }
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
class CPersistContext;class CSymbolMap;
namespace NParticleEngine {
class CParticleSystem;
class CEmitterDatabaseSerialiseInfo {public:const CSymbolMap& GraphicSymbols;const CSymbolMap& MeshSymbols;};
float __fastcall GetMaxSpawnDistance();float __fastcall GetMaxDrawDistance();
float __fastcall GetFadeOutStartDistance();float __fastcall GetFadeInStartDistance();float __fastcall GetFadeInEndDistance();

class CParticleEmitter:public CBaseObject {
public:CParticleEmitter();virtual ~CParticleEmitter();
 void DatabaseSerialise(CPersistContext&,const CEmitterDatabaseSerialiseInfo&);
protected:
 std::vector<CCountedPointer<CParticleSystem> > ParticleSystems;
 unsigned char UpdateParameters[36]; 
 unsigned char PrimitiveGroupHandle[8]; 
 unsigned short Priority:3;
 unsigned short IsScreenDisplacement:1;
 unsigned short ReadZBuffer:1;
 unsigned short Dead:1;
 unsigned short ParticleCreationDisabled:1;
 unsigned short ContinuousEmitter:1;
 unsigned short Initialised:1;
 unsigned short WaitingToSleep:1;
 unsigned short Sleeping:1;
 unsigned short Emitter2D:1;
 unsigned short Emitter2DLayer:7;
 unsigned char PreWaterEffect:1;
 unsigned char WaterEffect:1;
 unsigned char ZBufferWriteable:1;
 unsigned char Visible:1;
 unsigned char AlwaysUpdate:1;
 unsigned char DieIfOffsceen:1;
 unsigned char OffscreenUpdate:1;
 unsigned char ClipEffectToWeatherMask:1;
 unsigned char EnableDithering:1;
 unsigned char CalcBoundingSphereOnceOnly:1;
 unsigned char BoundingSphereValid:1;
 unsigned char ClearedParticles:1;
 unsigned char ClearedDecals:1;
 unsigned char OldPos[12]; 
 unsigned char Velocity[14]; 
 unsigned char OrientationInterpolator[52]; 
 unsigned char PositionInterpolator[28]; 
 unsigned char Transform[48]; 
 unsigned char InverseTransform[48]; 
 unsigned char EndTransform[48]; 
 CCharString Name;
 unsigned char BSphere[16]; 
 float DistanceToCam;
 float MaxSpawnDistance;
 float MaxDrawDistance;
 float FadeOutStart;
 float FadeInStart;
 float FadeInEnd;
};
}
FABLE_STATIC_ASSERT(sizeof(NParticleEngine::CParticleEmitter)==0x16c);


class CIEngine;
namespace NParticleEngine {

class CParticleSystem:public CBaseClass {
public:CParticleSystem();virtual ~CParticleSystem();
 void DatabaseSerialise(CPersistContext&,const CEmitterDatabaseSerialiseInfo&);
 void SetParentEmitter(const CParticleEmitter& p){PParentEmitter=&p;}
protected:
 const CParticleEmitter* PParentEmitter;CIEngine* PEngine;
 unsigned char Components[16],Particles[4],Decals[4],UpdateParameters[36];
 long NoActiveParticles,NoActiveDecals;
 bool Initialised:1,Enabled:1,Dead:1,NormalisedTimeEnabled:1,UseAllAttractors:1,Visible:1,StayWithEmitter:1,ScaleParticles:1;
 float GlobalAlpha,FadeAlpha,NormalisedTime;
 unsigned char Scale[12];CCharString Name;
};
}
FABLE_STATIC_ASSERT(sizeof(NParticleEngine::CParticleSystem)==0x70);
class CDefinitionManager;
class CSymbolMap;
class CStringParser;
class CCharString;
class CMemoryDataInputStream;
class CDataOutputStream;
namespace std { template<class T> class allocator; template<class A,class B> struct pair; template<class T> struct less; template<class K,class V,class C,class A> class map; template<class T,class A> class vector; }


class CPersistContext_TransferableVectorGenericType;
class CPersistContext_TransferableComponentGenericType;
class CPersistContextWriteSection;
template<class T> class CPersistTraits;
class CPersistContext {
public:
    CPersistContext(CStringParser&,void (__fastcall*)(const CCharString&),bool,const CDefinitionManager*,const CSymbolMap*,CCharString*);
    bool IsSaving()const{return Mode==MODE_SAVE_TEXT||Mode==MODE_SAVE_BINARY;}
    CStringParser* GetPParser(){return PParser;}
    void WriteString(const CCharString&);
    template<class T>void Transfer(const char*,T&);
    template<class T>void Transfer(const char*,T&,const T&);
    enum EMode { MODE_LOAD_TEXT=0,MODE_SAVE_TEXT=1,MODE_LOAD_BINARY=2,MODE_SAVE_BINARY=3,MODE_CLEAR=4 };
    template<class T> void Transfer(T&);
    template<class T> void TransferVector(std::vector<T,std::allocator<T> >&);
    template<class V,class K> void TransferMap(std::map<K,V,std::less<K>,std::allocator<std::pair<const K,V> > >&);
    template<class T> void TransferVectorOfSubComponents(std::vector<T,std::allocator<T> >&);
protected:
    const CDefinitionManager* PDefinitionManager;
    const CSymbolMap* PSymbolMap;
    CStringParser* PParser;
    void (__fastcall *ErrorCallback)(const CCharString&);
    CCharString* PStringToClean;
    bool UseDefaults,UseCRCCheck,SingleTransfer,Finished;
    EMode Mode;
    bool Binary;
    CCharString* PStringToSaveTo;
    CMemoryDataInputStream* PLoadStream;
    CDataOutputStream* PSaveStream;
};
FABLE_STATIC_ASSERT(sizeof(CPersistContext)==44);





class CParsedItem;

class CStringParserCommentBoundaries {public:const char* PStart;const char* PEnd;};
class CStringParser : public CBaseClass {
public:
    CStringParser(const CCharString&,void (__fastcall*)(const CCharString&)=0,const CCharString& filename=CCharString(""),long baseLineOffset=0);
    bool ReadAsStringUntilString(const CCharString&,CParsedItem&);
    bool SkipPastString(const CCharString&);
    bool SkipPastNextItemIfIs(const CCharString&);
    bool SkipUntilString(const CCharString&);
    CCharString ReadAsStringUntilString(const CCharString&);
    long SkipPastNewline();
    bool AtEnd(){return String[CursorPos.StringPos]==0;}
    CCharString ReadAsSymbol();
    CCharString ReadAsString();
    class CCursorPos {
    public:
        bool InQuotes;
        unsigned long StringPos,Line;
    };
protected:
    const char* String;
    unsigned long BaseLineOffset;
    CCharString Filename,TempCharString;
    std::list<CStringParserCommentBoundaries> Comments;
    void (__fastcall *ErrorCallback)(const CCharString&);
    CCursorPos CursorPos;
};
FABLE_STATIC_ASSERT(sizeof(CStringParser)==0x28);
namespace NParticleEngine {
void CParticleEmitter::DatabaseSerialise(CPersistContext& context,const CEmitterDatabaseSerialiseInfo& info) {
 if(context.IsSaving())return;
 context.Transfer("EmitterName",Name);
 {bool temp=Emitter2D;context.Transfer("Emitter2D",temp,false);Emitter2D=temp;}
 {bool temp=PreWaterEffect;context.Transfer("PreWaterEffect",temp,false);PreWaterEffect=temp;}
 {bool temp=WaterEffect;context.Transfer("WaterEffect",temp,false);WaterEffect=temp;}
 {bool temp=ZBufferWriteable;context.Transfer("ZBufferWriteable",temp,false);ZBufferWriteable=temp;}
 {bool temp=DieIfOffsceen;context.Transfer("DieIfOffsceen",temp,true);DieIfOffsceen=temp;}
 {bool temp=OffscreenUpdate;context.Transfer("OffscreenUpdate",temp,false);OffscreenUpdate=temp;}
 {bool temp=ClipEffectToWeatherMask;context.Transfer("ClipEffectToWeatherMask",temp,false);ClipEffectToWeatherMask=temp;}
 {bool temp=EnableDithering;context.Transfer("EnableDithering",temp,true);EnableDithering=temp;}
 {bool temp=CalcBoundingSphereOnceOnly;context.Transfer("CalcBoundingSphereOnceOnly",temp,false);CalcBoundingSphereOnceOnly=temp;}
 if(context.IsSaving()) {
 {bool temp=ContinuousEmitter;context.Transfer("ContinuousEmitter",temp,false);ContinuousEmitter=temp;}
 {bool temp=IsScreenDisplacement;context.Transfer("IsScreenDisplacement",temp,false);IsScreenDisplacement=temp;}
 {bool temp=ReadZBuffer;context.Transfer("ReadZBuffer",temp,true);ReadZBuffer=temp;}
  context.Transfer("MaxSpawnDistance",MaxSpawnDistance,0.0f);
  context.Transfer("MaxDrawDistance",MaxDrawDistance,0.0f);
  context.Transfer("FadeStart",FadeOutStart,0.0f);
  context.Transfer("FadeInEnd",FadeInEnd,0.0f);
  context.Transfer("FadeInStart",FadeInStart,0.0f);
  long pri=Priority;context.Transfer("Priority",pri,0L);
  for(std::vector<CCountedPointer<CParticleSystem> >::iterator it_system=ParticleSystems.begin();it_system!=ParticleSystems.end();++it_system) {
   context.WriteString("BEGIN_SYSTEM\r\n");
   (*it_system)->DatabaseSerialise(context,info);
   context.WriteString("END_SYSTEM\r\n");
  }
 } else {
  CStringParser& parser=*context.GetPParser();
 {bool temp=ContinuousEmitter;context.Transfer("ContinuousEmitter",temp,false);ContinuousEmitter=temp;}
 {bool temp=IsScreenDisplacement;context.Transfer("IsScreenDisplacement",temp,false);IsScreenDisplacement=temp;}
 {bool temp=ReadZBuffer;context.Transfer("ReadZBuffer",temp,true);ReadZBuffer=temp;}
  context.Transfer("MaxSpawnDistance",MaxSpawnDistance,GetMaxSpawnDistance());
  context.Transfer("MaxDrawDistance",MaxDrawDistance,GetMaxDrawDistance());
  context.Transfer("FadeStart",FadeOutStart,GetFadeOutStartDistance());
  context.Transfer("FadeInEnd",FadeInEnd,GetFadeInEndDistance());
  context.Transfer("FadeInStart",FadeInStart,GetFadeInStartDistance());
  long pri;context.Transfer("Priority",pri,0L);Priority=pri;
  while(parser.SkipPastString("BEGIN_SYSTEM")) {
   CCountedPointer<CParticleSystem> psystem=new CParticleSystem;
   psystem->SetParentEmitter(*this);
   CCharString system_script=parser.ReadAsStringUntilString("END_SYSTEM");
   CStringParser system_parser(system_script);
   CPersistContext context2(system_parser,0,true,0,0,&system_script);
   psystem->DatabaseSerialise(context2,info);
   ParticleSystems.push_back(psystem);
  }
  MaxDrawDistance=(std::min)(GetMaxDrawDistance(),MaxDrawDistance);
  MaxSpawnDistance=(std::min)(FadeOutStart,MaxSpawnDistance);
  FadeOutStart=(std::min)(MaxDrawDistance,FadeOutStart);
  FadeInEnd=(std::min)(FadeOutStart,FadeInEnd);
  FadeInStart=(std::min)(FadeInEnd,FadeInStart);
 }
}
}

static void check(bool v,int n){if(!v){fprintf(stderr,"particle check %d failed\n",n);exit(1);}}
static unsigned int stringLive=0,systemLive=0,serialCalls=0,transfers=0,boolCalls=0,floatCalls=0,longCalls=0,writeCalls=0;
static unsigned int scenario=0,expectSystems=0,copyMode=0;
static bool loadDefaults=false,switchSave=false;
static const char* boolNames[]={"Emitter2D","PreWaterEffect","WaterEffect","ZBufferWriteable","DieIfOffsceen","OffscreenUpdate","ClipEffectToWeatherMask","EnableDithering","CalcBoundingSphereOnceOnly","ContinuousEmitter","IsScreenDisplacement","ReadZBuffer"};
static const bool boolDefaults[]={false,false,false,false,true,false,false,true,false,false,false,true};
static const char* floatNames[]={"MaxSpawnDistance","MaxDrawDistance","FadeStart","FadeInEnd","FadeInStart"};
static float inputs[5];static float actualDefaults[5];
static NParticleEngine::CParticleEmitter* active=0;
static const NParticleEngine::CEmitterDatabaseSerialiseInfo* activeInfo=0;
static CPersistContext* mainContext=0;
static const char* text(const CCharString& s){return s.PStringData?(const char*)s.PStringData:"";}
CCharString::CCharString():PStringData(0){++stringLive;}
CCharString::CCharString(const char* s,long n):PStringData(0){if(n<0)n=(long)strlen(s);char* p=(char*)malloc((size_t)n+1);memcpy(p,s,(size_t)n);p[n]=0;PStringData=(CCharStringData*)p;++stringLive;}
CCharString::CCharString(const CCharString& s):PStringData(0){const char* x=text(s);char* p=(char*)malloc(strlen(x)+1);strcpy(p,x);PStringData=(CCharStringData*)p;++stringLive;}
CCharString::~CCharString(){free(PStringData);--stringLive;}
CBaseClass::CBaseClass(){} CBaseClass::~CBaseClass(){}
CBaseObject::CBaseObject():IntelligentPointer(0){} CBaseObject::~CBaseObject(){}
void CBaseObject::Validate()const{}
CStringParser::CStringParser(const CCharString& s,void(__fastcall* callback)(const CCharString&),const CCharString& f,long line):String(text(s)),BaseLineOffset(line),Filename(f),ErrorCallback(callback){memset(&CursorPos,0,sizeof(CursorPos));}
bool CStringParser::SkipPastString(const CCharString& x){check(!strcmp(text(x),"BEGIN_SYSTEM"),1);const char* hit=strstr(String+CursorPos.StringPos,text(x));if(!hit)return false;CursorPos.StringPos=(unsigned long)(hit-String+strlen(text(x)));return true;}
CCharString CStringParser::ReadAsStringUntilString(const CCharString& x){check(!strcmp(text(x),"END_SYSTEM"),2);const char* p=String+CursorPos.StringPos;const char* end=strstr(p,text(x));check(end!=0,3);CursorPos.StringPos=(unsigned long)(end-String);CCharString result(p,(long)(end-p));if(copyMode){CCharString copy(result);return copy;}return result;}
CPersistContext::CPersistContext(CStringParser& p,void(__fastcall* callback)(const CCharString&),bool defaults,const CDefinitionManager* defs,const CSymbolMap* symbols,CCharString* clean):PDefinitionManager(defs),PSymbolMap(symbols),PParser(&p),ErrorCallback(callback),PStringToClean(clean),UseDefaults(defaults),UseCRCCheck(false),SingleTransfer(false),Finished(false),Mode(MODE_LOAD_TEXT),Binary(false),PStringToSaveTo(0),PLoadStream(0),PSaveStream(0){}
static void load(CPersistContext& c,const char* n,bool& x,const bool& d){check(&c==mainContext&&boolCalls<12&&!strcmp(n,boolNames[boolCalls])&&d==boolDefaults[boolCalls],4);if(!c.IsSaving())x=loadDefaults?d:((scenario>>boolCalls)&1)!=0;++boolCalls;++transfers;if(switchSave&&boolCalls==9)c.Mode=CPersistContext::MODE_SAVE_TEXT;}
static void load(CPersistContext& c,const char* n,float& x,const float& d){check(&c==mainContext&&floatCalls<5&&!strcmp(n,floatNames[floatCalls]),5);actualDefaults[floatCalls]=d;if(!c.IsSaving())x=loadDefaults?d:inputs[floatCalls];++floatCalls;++transfers;}
static void load(CPersistContext& c,const char* n,long& x,const long& d){check(&c==mainContext&&!strcmp(n,"Priority")&&d==0&&!longCalls,6);if(!c.IsSaving())x=loadDefaults?0:(long)(scenario+9);++longCalls;++transfers;}
template<class T>void CPersistContext::Transfer(const char* n,T& x,const T& d){load(*this,n,x,d);}
template<class T>void CPersistContext::Transfer(const char* n,T& x){check(this==mainContext&&!strcmp(n,"EmitterName")&&!strcmp(text(x),"owner"),7);++transfers;}
void CPersistContext::WriteString(const CCharString& s){check(this==mainContext&&IsSaving()&&!strcmp(text(s),writeCalls%2?"END_SYSTEM\r\n":"BEGIN_SYSTEM\r\n"),8);++writeCalls;}
namespace NParticleEngine {
float __fastcall GetMaxDrawDistance(){return 60.0f;}float __fastcall GetMaxSpawnDistance(){return 48.0f;}float __fastcall GetFadeOutStartDistance(){return 48.0f;}float __fastcall GetFadeInEndDistance(){return 0.0f;}float __fastcall GetFadeInStartDistance(){return 0.0f;}
CParticleEmitter::CParticleEmitter():Name("owner"){
 Priority=5;IsScreenDisplacement=1;ReadZBuffer=0;Dead=1;ParticleCreationDisabled=1;ContinuousEmitter=1;Initialised=1;WaitingToSleep=1;Sleeping=1;Emitter2D=1;Emitter2DLayer=73;
 PreWaterEffect=1;WaterEffect=1;ZBufferWriteable=1;Visible=1;AlwaysUpdate=1;DieIfOffsceen=1;OffscreenUpdate=1;ClipEffectToWeatherMask=1;EnableDithering=1;CalcBoundingSphereOnceOnly=1;BoundingSphereValid=1;ClearedParticles=1;ClearedDecals=1;
 MaxSpawnDistance=4;MaxDrawDistance=8;FadeOutStart=2;FadeInEnd=1;FadeInStart=0;DistanceToCam=32;
 memset(UpdateParameters,0x75,sizeof(UpdateParameters));memset(PrimitiveGroupHandle,0x76,sizeof(PrimitiveGroupHandle));memset(OldPos,0x77,sizeof(OldPos));memset(Velocity,0x78,sizeof(Velocity));
}
CParticleEmitter::~CParticleEmitter(){}
CParticleSystem::CParticleSystem():PParentEmitter(0),PEngine(0),NoActiveParticles(0),NoActiveDecals(0){++systemLive;}
CParticleSystem::~CParticleSystem(){--systemLive;}
void CParticleSystem::DatabaseSerialise(CPersistContext& c,const CEmitterDatabaseSerialiseInfo& info){check(&info==activeInfo,9);check(PParentEmitter==active,10);if(&c!=mainContext){check(c.UseDefaults&&!c.ErrorCallback&&!c.PDefinitionManager&&!c.PSymbolMap&&c.PStringToClean,11);check(strstr(text(*c.PStringToClean),"system")!=0,12);}else check(c.IsSaving(),13);++serialCalls;}
}
static float expectedMin(float a,float b){return a<b?a:b;}
static void run(unsigned int mode,unsigned int count,unsigned int pattern,bool defaults,bool transition,unsigned int cp){
 scenario=pattern;loadDefaults=defaults;switchSave=transition;copyMode=cp;expectSystems=count;
 serialCalls=transfers=boolCalls=floatCalls=longCalls=writeCalls=0;
 inputs[0]=(pattern&1)?96.0f:16.0f;inputs[1]=(pattern&2)?120.0f:32.0f;inputs[2]=(pattern&4)?64.0f:8.0f;inputs[3]=(pattern&8)?80.0f:4.0f;inputs[4]=(pattern&16)?100.0f:2.0f;
 char buffer[256];buffer[0]=0;for(unsigned int i=0;i<count;++i)strcat(buffer,"BEGIN_SYSTEM system END_SYSTEM ");
 CCharString script(buffer);CStringParser parser(script);CPersistContext context(parser,0,true,0,0,0);context.Mode=(CPersistContext::EMode)mode;mainContext=&context;
 union Token{double d;void* p;} token;memset(&token,0,sizeof(token));const CSymbolMap& symbol=*reinterpret_cast<const CSymbolMap*>(&token);NParticleEngine::CEmitterDatabaseSerialiseInfo info={symbol,symbol};activeInfo=&info;
 {
  NParticleEngine::CParticleEmitter e;active=&e;
  if(transition&&mode!=1&&mode!=3){for(unsigned int i=0;i<count;++i){CCountedPointer<NParticleEngine::CParticleSystem> p=new NParticleEngine::CParticleSystem;p->SetParentEmitter(e);e.ParticleSystems.push_back(p);}}
  e.DatabaseSerialise(context,info);
  if(mode==1||mode==3){check(!transfers&&!serialCalls&&!writeCalls&&e.ParticleSystems.empty(),14);check(e.Priority==5&&e.MaxDrawDistance==8,15);}
  else {
   check(transfers==19&&boolCalls==12&&floatCalls==5&&longCalls==1,16);check(e.ParticleSystems.size()==count&&serialCalls==count,17);
   const bool actual[]={e.Emitter2D!=0,e.PreWaterEffect!=0,e.WaterEffect!=0,e.ZBufferWriteable!=0,e.DieIfOffsceen!=0,e.OffscreenUpdate!=0,e.ClipEffectToWeatherMask!=0,e.EnableDithering!=0,e.CalcBoundingSphereOnceOnly!=0,e.ContinuousEmitter!=0,e.IsScreenDisplacement!=0,e.ReadZBuffer!=0};
   for(unsigned int i=0;i<12;++i){bool want=transition&&i>=9?(i!=11):(defaults?boolDefaults[i]:((pattern>>i)&1)!=0);check(actual[i]==want,18);}
   if(transition){check(writeCalls==2*count&&e.Priority==5&&e.MaxDrawDistance==8,19);for(unsigned int i=0;i<5;++i)check(actualDefaults[i]==0,20);}
   else {
    check(!writeCalls&&e.Priority==(defaults?0:(pattern+9)&7),21);
    float values[5];const float defs[]={48,60,48,0,0};for(unsigned int i=0;i<5;++i){check(actualDefaults[i]==defs[i],22);values[i]=defaults?defs[i]:inputs[i];}
    values[1]=expectedMin(values[1],60);values[0]=expectedMin(values[0],values[2]);values[2]=expectedMin(values[2],values[1]);values[3]=expectedMin(values[3],values[2]);values[4]=expectedMin(values[4],values[3]);
    check(e.MaxSpawnDistance==values[0]&&e.MaxDrawDistance==values[1]&&e.FadeOutStart==values[2]&&e.FadeInEnd==values[3]&&e.FadeInStart==values[4],23);
   }
   for(unsigned int i=0;i<count;++i)check(e.ParticleSystems[i].GetReferenceCount()==1,24);
  }
  check(e.Dead&&e.ParticleCreationDisabled&&e.Initialised&&e.WaitingToSleep&&e.Sleeping&&e.Visible&&e.AlwaysUpdate&&e.BoundingSphereValid&&e.ClearedParticles&&e.ClearedDecals&&e.Emitter2DLayer==73&&e.DistanceToCam==32,25);
#ifdef _MSC_VER
  check(sizeof(e)==0x16c&&sizeof(NParticleEngine::CParticleSystem)==0x70&&sizeof(CStringParser)==0x28&&sizeof(CPersistContext)==0x2c,26);
  check((char*)&e.Name-(char*)&e==0x140&&(char*)&e.MaxSpawnDistance-(char*)&e==0x158,27);
#endif
 }
 check(systemLive==0,28);
}
int main(){unsigned int counts[]={0,1,3},patterns[]={0,31,85,682,1365,4095};unsigned int cases=0;for(unsigned int mode=0;mode<5;++mode)for(unsigned int n=0;n<3;++n)for(unsigned int p=0;p<6;++p)for(unsigned int defaults=0;defaults<2;++defaults)for(unsigned int transition=0;transition<2;++transition)for(unsigned int cp=0;cp<2;++cp){run(mode,counts[n],patterns[p],defaults!=0,transition!=0,cp);check(!stringLive&&!systemLive,29);++cases;}check(cases==720,30);puts("00aef1f0_TEST PASS");return 0;}
