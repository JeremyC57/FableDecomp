#ifndef _MSC_VER
#define __cdecl
#define __fastcall
#define __thiscall
#define __forceinline inline
#define __declspec(x)
#define __int64 long
#endif
#define FABLE_RETAIL_CTHING_CONSTRUCTORS
#define FABLE_STLPORT_CONSTRUCTORS
#define FABLE_AUTOPLACE_TEST_ACCESS
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

#ifndef FABLE_ENGINE_CRIGHTHANDEDSET_H
#define FABLE_ENGINE_CRIGHTHANDEDSET_H
#ifndef FABLE_ENGINE_C3DVECTOR_H
#define FABLE_ENGINE_C3DVECTOR_H


#ifdef _MSC_VER
#define FABLE_VECTOR_INLINE __forceinline
#else
#define FABLE_VECTOR_INLINE inline
#endif
#ifdef FABLE_RETAIL_VECTOR_GEOMETRY
#ifndef FABLE_ENGINE_GFMATHS_H
#define FABLE_ENGINE_GFMATHS_H
extern "C" double __cdecl sqrt(double);
#pragma intrinsic(sqrt)
extern const float DELTA_NOTIONAL_ZERO;
extern unsigned long GFastOneOverSqrtTable[128];
inline float __fastcall GFFloatAbs(float n){
#ifdef _MSC_VER
 *(unsigned long*)&n &= 0x7fffffffUL;
#else
 *(unsigned int*)&n &= 0x7fffffffU;
#endif
 return n;
}
#ifdef _MSC_VER
__forceinline
#else
inline
#endif
float __fastcall GFFastOneOverSqrt(float n){
#ifdef _MSC_VER
 typedef unsigned long word_type;
#else
 typedef unsigned int word_type;
#endif
 word_type bits=*(word_type*)&n;
 word_type exponent=(0x17c-((bits>>23)&0xff))<<22;
 word_type mantissa=(word_type)GFastOneOverSqrtTable[(bits>>17)&0x7f]<<15;
 float estimate;
 *(word_type*)&estimate=(exponent&0xff800000)|mantissa;
 return (3.0f-estimate*estimate*n)*estimate*0.5f;
}
#endif

#endif
class C3DVector {
public:
    float X,Y,Z;
    C3DVector() {}
    C3DVector(float x,float y,float z):X(x),Y(y),Z(z){}
#ifdef FABLE_RETAIL_VECTOR_GEOMETRY
    C3DVector(const C3DVector& p){*this=p;}
#else
    C3DVector(const C3DVector& other):X(other.X),Y(other.Y),Z(other.Z){}
#endif
    FABLE_VECTOR_INLINE void Initialise(float x,float y,float z){X=x;Y=y;Z=z;}
    FABLE_VECTOR_INLINE const C3DVector operator-(const C3DVector& other)const{return C3DVector(X-other.X,Y-other.Y,Z-other.Z);}
    FABLE_VECTOR_INLINE const C3DVector operator+(const C3DVector& other)const{return C3DVector(X+other.X,Y+other.Y,Z+other.Z);}
    FABLE_VECTOR_INLINE const C3DVector operator*(float scale)const{return C3DVector(X*scale,Y*scale,Z*scale);}
    FABLE_VECTOR_INLINE bool operator==(const C3DVector& other)const{return X==other.X&&Y==other.Y&&Z==other.Z;}
    FABLE_VECTOR_INLINE float GetSquaredMagnitude()const{return X*X+Z*Z+Y*Y;}
    FABLE_VECTOR_INLINE float GetSquaredMagnitude2D()const{return X*X+Y*Y;}
    FABLE_VECTOR_INLINE const float Dot(const C3DVector& other)const{return X*other.X+Y*other.Y+Z*other.Z;}
    FABLE_VECTOR_INLINE const C3DVector Cross(const C3DVector& other)const{return C3DVector(Y*other.Z-Z*other.Y,Z*other.X-X*other.Z,X*other.Y-Y*other.X);}
#ifdef FABLE_RETAIL_VECTOR_GEOMETRY
    FABLE_VECTOR_INLINE void Clear(){X=Y=Z=0.0f;}
    FABLE_VECTOR_INLINE const C3DVector& operator+=(const C3DVector& v){X+=v.X;Y+=v.Y;Z+=v.Z;return *this;}
    FABLE_VECTOR_INLINE const C3DVector& operator*=(float v){X*=v;Y*=v;Z*=v;return *this;}
    FABLE_VECTOR_INLINE float GetMagnitude()const{return (float)sqrt(GetSquaredMagnitude());}
    FABLE_VECTOR_INLINE void LimitLower(const C3DVector& v){if(X<v.X)X=v.X;if(Y<v.Y)Y=v.Y;if(Z<v.Z)Z=v.Z;}
    FABLE_VECTOR_INLINE void LimitUpper(const C3DVector& v){if(X>v.X)X=v.X;if(Y>v.Y)Y=v.Y;if(Z>v.Z)Z=v.Z;}
    FABLE_VECTOR_INLINE bool IsNonZero()const{return DELTA_NOTIONAL_ZERO*DELTA_NOTIONAL_ZERO<X*X+Y*Y+Z*Z;}
    FABLE_VECTOR_INLINE C3DVector& Normalise(){float one_on_magnitude=GFFastOneOverSqrt(GetSquaredMagnitude());X*=one_on_magnitude;Y*=one_on_magnitude;Z*=one_on_magnitude;return *this;}
#endif
#ifdef FABLE_RETAIL_VECTOR_SPLINE
    FABLE_VECTOR_INLINE const C3DVector& operator*=(float v){X*=v;Y*=v;Z*=v;return *this;}
    FABLE_VECTOR_INLINE const C3DVector& operator/=(float v){float reciprocal=1.0f/v;X*=reciprocal;Y*=reciprocal;Z*=reciprocal;return *this;}
    FABLE_VECTOR_INLINE float GetMagnitude()const{return (float)sqrt(GetSquaredMagnitude());}
#endif
    C3DVector& AccurateNormalise();
    void Rotate(const C3DVector&,float);
};
#undef FABLE_VECTOR_INLINE
#endif



class CRightHandedSet { protected: C3DVector Up,Forward; public: const C3DVector& GetForward() const { return Forward; } };
#endif



#define FABLE_RETAIL_VISIBLE_THINGSEARCH
#define FABLE_STLPORT_VISIBLE_TRUE_OVERFLOW
#define FABLE_STLPORT_VISIBLE_PUSH_BACK
#define FABLE_RETAIL_VISIBLE_LOWERBOUND
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CTCCREATURENAVIGATION
#define FABLE_RETAIL_CTHINGSEARCHTOOLS
#define FABLE_RETAIL_CWORLD
#ifdef FABLE_RETAIL_CTHINGCREATUREBASE
#ifndef FABLE_ENGINE_CTHINGCREATUREBASE_H
#define FABLE_ENGINE_CTHINGCREATUREBASE_H
#ifdef FABLE_RETAIL_CTHINGGAMEOBJECT
#ifndef FABLE_ENGINE_CTHINGGAMEOBJECT_H
#define FABLE_ENGINE_CTHINGGAMEOBJECT_H
#ifdef FABLE_RETAIL_CTHINGPHYSICAL
#ifndef FABLE_ENGINE_CTHINGPHYSICAL_H
#define FABLE_ENGINE_CTHINGPHYSICAL_H
#ifdef FABLE_RETAIL_CTHING
#ifndef FABLE_ENGINE_CTHING_H
#define FABLE_ENGINE_CTHING_H


#ifndef FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_CCLASS
#endif
#ifndef FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_DEF_CLASS_BASE
#endif
#ifndef FABLE_ENGINE_CCLASS_H
#define FABLE_ENGINE_CCLASS_H










#ifdef FABLE_RETAIL_CCLASS
#include <stddef.h>
#pragma once



#if defined(_MSC_VER)
#  define FABLE_CDECL __cdecl
#  define FABLE_STDCALL __stdcall
#  define FABLE_FASTCALL __fastcall
#  define FABLE_THISCALL __thiscall
#else
#  define FABLE_CDECL
#  define FABLE_STDCALL
#  define FABLE_FASTCALL
#  define FABLE_THISCALL
#endif

typedef signed char fable_i8;
typedef unsigned char fable_u8;
typedef signed short fable_i16;
typedef unsigned short fable_u16;
typedef signed int fable_i32;
typedef unsigned int fable_u32;

typedef char fable_require_i8[sizeof(fable_i8) == 1 ? 1 : -1];
typedef char fable_require_i16[sizeof(fable_i16) == 2 ? 1 : -1];
typedef char fable_require_i32[sizeof(fable_i32) == 4 ? 1 : -1];
#ifdef _MSC_VER
typedef char fable_require_ptr32[sizeof(void*) == 4 ? 1 : -1];
#endif

#define FABLE_JOIN_INNER(a, b) a##b
#define FABLE_JOIN(a, b) FABLE_JOIN_INNER(a, b)
#ifdef _MSC_VER
#define FABLE_STATIC_ASSERT(expression) \
    typedef char FABLE_JOIN(fable_static_assert_, __LINE__)[(expression) ? 1 : -1]
#else
#define FABLE_STATIC_ASSERT(expression) typedef char FABLE_JOIN(_test_size_,__LINE__)[1]
#endif

#ifndef FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_BASE_OBJECT
#endif
#ifndef FABLE_ENGINE_CBASEOBJECT_H
#define FABLE_ENGINE_CBASEOBJECT_H
















#ifdef FABLE_RETAIL_BASE_OBJECT

#ifndef FABLE_ENGINE_CBASECLASS_H
#define FABLE_ENGINE_CBASECLASS_H









class CBaseClass {
public:
    CBaseClass();                                                        
    virtual ~CBaseClass();                                               
};
FABLE_STATIC_ASSERT(sizeof(CBaseClass) == 0x4);

#endif 

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
#else 




#include <stddef.h>


struct CBaseObjectPointer;

#pragma pack(push, 1)
class CBaseObject {
public:
    void*               __vftable;                           
    CBaseObjectPointer* IntelligentPointer;                  
};
#pragma pack(pop)

FABLE_STATIC_ASSERT(sizeof(CBaseObject) == 0x8);
FABLE_STATIC_ASSERT(offsetof(CBaseObject, __vftable) == 0x0);
FABLE_STATIC_ASSERT(offsetof(CBaseObject, IntelligentPointer) == 0x4);

#endif 
#endif 

class CCharString;
class CClassFactory;
class CClass : public CBaseObject {
public:
    virtual void Save(CCharString&);                                     
    void Unregister(CClassFactory&);                                     
    void PostConstructionInit(CClassFactory&);                           
    unsigned long GetType() const { return Type; }                       
    unsigned char ClassListPos[0x4];                                     
    unsigned char TypeListPos[0x4];                                      
    unsigned char Type;                                                  
};
FABLE_STATIC_ASSERT(sizeof(CClass) == 0x14);
FABLE_STATIC_ASSERT(offsetof(CClass, Type) == 0x10);
#else 





#include <stddef.h>


#pragma pack(push, 1)
class CClass {
public:
    void*         __vftable;                                 
    unsigned char _base_0x04[0x4];                           
    unsigned char ClassListPos[0x4];                         
    unsigned char TypeListPos[0x4];                          
    unsigned char Type;                                      
};
#pragma pack(pop)

FABLE_STATIC_ASSERT(sizeof(CClass) == 0x11);
FABLE_STATIC_ASSERT(offsetof(CClass, __vftable) == 0x0);
FABLE_STATIC_ASSERT(offsetof(CClass, ClassListPos) == 0x8);
FABLE_STATIC_ASSERT(offsetof(CClass, TypeListPos) == 0xc);
FABLE_STATIC_ASSERT(offsetof(CClass, Type) == 0x10);

#endif 
#endif 

#ifndef FABLE_ENGINE_CCHARSTRING_H
#define FABLE_ENGINE_CCHARSTRING_H


#if defined(FABLE_RETAIL_SCOREBOARD) || defined(FABLE_RETAIL_PERSIST_OPINION)
#define FABLE_RETAIL_CCHARSTRING_DATA
#pragma once

#include <stddef.h>



#ifndef FABLE_RETAIL_CCHARSTRING_DATA
class CWideStringData
{ public:
    wchar_t* text;
    fable_u32 unknown04;
    fable_u32 unknown08;
    fable_i32 owners;
};

#endif

#pragma pack(push, 1)
#ifdef FABLE_RETAIL_CCHARSTRING_DATA



template<class T> class CBasicString {
protected:
    T* PData;
    unsigned long StringLength;
    unsigned long DataLength : 31;
    bool UseFastExtend : 1;
public:
    const T* PeekData() const { return PData; }
#ifdef FABLE_RETAIL_PERSIST_OPINION
    bool operator==(const T* text) const;
#endif
};
class CCharStringData {
public:
    CBasicString<char> Data;
protected:
    long NoRefs;
};
#else
class CCharStringData
{ public:
    char* text;
    fable_u32 unknown04;
    fable_u32 unknown08;
    fable_u8 flags0C;
    fable_i32 owners;
};
#endif
#pragma pack(pop)

#ifndef FABLE_RETAIL_CCHARSTRING_DATA

class CCharString;

class CWideString
{
public:
    CWideString();
    CWideString(const CWideString& other);
    explicit CWideString(const wchar_t* text);
    ~CWideString();
    operator const wchar_t*() const;
    const wchar_t* GetAsCharArray() const;                     
    long GetLength() const;
    long FindCharPos(wchar_t c) const;                         
    void __cdecl Sprintf(const wchar_t* format, ...);          
    void SetLength(long length);                               
    const CWideString& operator=(const CWideString& other);
    const CWideString& operator=(const wchar_t* text);
    const CWideString& operator+=(const CWideString& other);
    const CWideString& operator+=(const wchar_t* text);
    bool operator!=(const wchar_t* other) const;

    static CWideString FABLE_FASTCALL CreateFromCharString(
        const CCharString& text);

private:
    explicit CWideString(const CCharString& text);
    void UnassignString();

protected:
    CWideStringData* AllocStringData(                          
        const wchar_t* text,
        long length);

private:

    CWideStringData* storage_;
};

CWideString FABLE_FASTCALL operator+(
    const CWideString& left,
    const CWideString& right);
CWideString FABLE_FASTCALL operator+(
    const CWideString& left,
    const wchar_t* right);
CWideString FABLE_FASTCALL operator+(
    const wchar_t* left,
    const CWideString& right);

class CCharString
{
public:
    CCharString();
    CCharString(const char* text, long length);
    CCharString(const CCharString& other);                     
    ~CCharString();
    operator const char*() const;
    const CCharString& operator=(const CCharString& other);
    const CCharString& operator=(const char* text);
    CWideString ToWideString() const;

protected:
    void UnassignString();                                     
    CCharStringData* AllocStringData(                          
        const char* text,
        long length);

private:
    CCharStringData* storage_;
};

CCharString FABLE_FASTCALL operator+(
    const CCharString& left,
    const char* right);



extern fable_i32 g_CWideStringInstanceCount_013BCA20;
extern fable_i32 g_CCharStringInstanceCount_013BD800;
extern wchar_t g_FableEmptyWideString_0129A8E0[1];

FABLE_STATIC_ASSERT(sizeof(CWideStringData) == 0x10);
FABLE_STATIC_ASSERT(sizeof(CCharStringData) == 0x11);
FABLE_STATIC_ASSERT(sizeof(CWideString) == 0x04);
FABLE_STATIC_ASSERT(sizeof(CCharString) == 0x04);

#endif

#endif
class CCharStringData;
class CCharString {
public:
    CCharString();
    CCharString(const char* text, long length = -1);
    CCharString(const CCharString& other);
#ifdef FABLE_RETAIL_SCOREBOARD
    const char* GetAsCharArray() const { const char* result=&NullChar; if(PStringData) result=PStringData->Data.PeekData(); return result; }
#endif
#ifdef FABLE_RETAIL_EFFECT_LOOKUP
    unsigned long GetCRC() const; 
#endif
#ifdef FABLE_RETAIL_PERSIST_OPINION
    bool operator==(const char* text) const { if (PStringData) return PStringData->Data==text; if (strcmp(text,"")==0) return true; return false; }
#endif
    ~CCharString();
protected:
#ifdef FABLE_RETAIL_SCOREBOARD
    static const char NullChar;
#endif
    CCharStringData* PStringData;
};
#endif

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
FABLE_STATIC_ASSERT(sizeof(CIVCountedPointeeBase) == 0x8);
#else 
#include <stddef.h>


#pragma pack(push, 1)
class CIVCountedPointeeBase {
public:
    void*         __vftable;                                 
    unsigned long IVRefCount;                                
};
#pragma pack(pop)

FABLE_STATIC_ASSERT(sizeof(CIVCountedPointeeBase) == 0x8);
FABLE_STATIC_ASSERT(offsetof(CIVCountedPointeeBase, __vftable) == 0x0);
FABLE_STATIC_ASSERT(offsetof(CIVCountedPointeeBase, IVRefCount) == 0x4);

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
FABLE_STATIC_ASSERT(sizeof(CResource) == 0x1c);
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

FABLE_STATIC_ASSERT(sizeof(CResource) == 0x1c);
FABLE_STATIC_ASSERT(offsetof(CResource, __vftable) == 0x0);
FABLE_STATIC_ASSERT(offsetof(CResource, ResourceList) == 0x8);
FABLE_STATIC_ASSERT(offsetof(CResource, PrevResource) == 0xc);
FABLE_STATIC_ASSERT(offsetof(CResource, NextResource) == 0x10);
FABLE_STATIC_ASSERT(offsetof(CResource, ResourceSize) == 0x14);
FABLE_STATIC_ASSERT(offsetof(CResource, LastUsedFrame) == 0x18);

#endif 

#endif 


class CDefPointeeBase : public CResource {
public:
    virtual ~CDefPointeeBase() { }                                       
};
FABLE_STATIC_ASSERT(sizeof(CDefPointeeBase) == 0x1c);

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
FABLE_STATIC_ASSERT(sizeof(CDefPointer<CDefPointeeBase>) == 0x4);

#endif 
#endif 

#ifndef FABLE_ENGINE_CVECTORMAP_H
#define FABLE_ENGINE_CVECTORMAP_H
#ifdef FABLE_VECTOR_MAP_GEOMETRY


#ifndef FABLE_STLPORT46_GEOMETRY_H
#define FABLE_STLPORT46_GEOMETRY_H

#include <stddef.h>
#include <stdlib.h>
#ifdef _MSC_VER
inline void* __cdecl operator new(size_t,void* p){return p;}
#else
#include <new>
#endif
namespace std {
struct __false_type {};
template<class T>class allocator {
public:
 allocator(){}
 T* allocate(size_t n,const void* =0){return n?(T*)malloc(n*sizeof(T)):0;}
 void deallocate(T* p,size_t){if(p)free(p);}
};
template<class V,class T,class A>class _STLP_alloc_proxy:public A {
public:V _M_data;_STLP_alloc_proxy(const A& a,V p):A(a),_M_data(p){}
};
template<class A,class B>struct pair {
 typedef A first_type;typedef B second_type;
 A first;B second;
 pair():first(A()),second(B()){}
 pair(const A&a,const B&b):first(a),second(b){}
 template<class C,class D>pair(const pair<C,D>&p):first(p.first),second(p.second){}
};
template<class A,class B,class R>struct binary_function{typedef A first_argument_type;typedef B second_argument_type;typedef R result_type;};
template<class A,class R>struct unary_function{typedef A argument_type;typedef R result_type;};
template<class T>struct less:binary_function<T,T,bool>{bool operator()(const T&a,const T&b)const{return a<b;}};
template<class T>struct _Identity:unary_function<T,T>{const T&operator()(const T&v)const{return v;}};
template<class P>struct _Select1st:unary_function<P,typename P::first_type>{const typename P::first_type&operator()(const P&v)const{return v.first;}};
template<class T>inline const T& (max)(const T&a,const T&b){return a<b?b:a;}
template<class T>inline const T& (min)(const T&a,const T&b){return b<a?b:a;}
template<class T>inline void swap(T&a,T&b){T temporary=a;a=b;b=temporary;}
template<class T,class U>inline void _Construct(T*p,const U&v){new(p)T(v);}
template<class T>inline void _Destroy(T*p){p->~T();}
template<class I>inline void _Destroy(I first,I last){for(;first!=last;++first)_Destroy(&*first);}
template<class T,class A>class _Vector_base {
public:
 _Vector_base(const A&a):_M_start(0),_M_finish(0),_M_end_of_storage(a,0){}
 ~_Vector_base(){_M_end_of_storage.deallocate(_M_start,_M_end_of_storage._M_data-_M_start);}
protected:T*_M_start;T*_M_finish;_STLP_alloc_proxy<T*,T,A>_M_end_of_storage;
};
template<class T,class A=allocator<T> >class vector:public _Vector_base<T,A>{
public:
 typedef T value_type;typedef T*iterator;typedef const T*const_iterator;typedef unsigned int size_type;typedef A allocator_type;
 explicit vector(const A&a=A()):_Vector_base<T,A>(a){}
 ~vector(){_Destroy(this->_M_start,this->_M_finish);}
 iterator begin(){return this->_M_start;}const_iterator begin()const{return this->_M_start;}
 iterator end(){return this->_M_finish;}const_iterator end()const{return this->_M_finish;}
 size_type size()const{return size_type(this->_M_finish-this->_M_start);}
 size_type capacity()const{return size_type(this->_M_end_of_storage._M_data-this->_M_start);}
 T&operator[](size_type i){return this->_M_start[i];}const T&operator[](size_type i)const{return this->_M_start[i];}
 void reserve(size_type);
 void push_back(const T&v){if(this->_M_finish!=this->_M_end_of_storage._M_data){_Construct(this->_M_finish,v);++this->_M_finish;}else _M_insert_overflow(this->_M_finish,v,__false_type(),1,true);}
protected:
 void _M_insert_overflow(T*,const T&,const __false_type&,size_type,bool=false);
#ifdef FABLE_GEOMETRY_VECTOR_BODIES
 void _M_clear(){_Destroy(this->_M_start,this->_M_finish);this->_M_end_of_storage.deallocate(this->_M_start,this->_M_end_of_storage._M_data-this->_M_start);}
 void _M_set(T* s,T* f,T* e){this->_M_start=s;this->_M_finish=f;this->_M_end_of_storage._M_data=e;}
 template<class I>T* _M_allocate_and_copy(size_type n,I first,I last){T* result=this->_M_end_of_storage.allocate(n);T* out=result;for(;first!=last;++first,++out)_Construct(out,*first);return result;}
#endif
};
#ifdef FABLE_GEOMETRY_VECTOR_BODIES
template<class T,class A>
__declspec(noinline) void vector<T,A>::reserve(size_type n){
 if(capacity()<n){const size_type old_size=size();T* temp;if(this->_M_start){temp=_M_allocate_and_copy(n,this->_M_start,this->_M_finish);_M_clear();}else temp=this->_M_end_of_storage.allocate(n);_M_set(temp,temp+old_size,temp+n);}
}
template<class T,class A>
__declspec(noinline) void vector<T,A>::_M_insert_overflow(T* position,const T& value,const __false_type&,size_type fill_len,bool at_end){
 const size_type old_size=size();const size_type length=old_size+(max)(old_size,fill_len);
 T* new_start=this->_M_end_of_storage.allocate(length);T* new_finish=new_start;
 for(T* first=this->_M_start;first!=position;++first,++new_finish)_Construct(new_finish,*first);
 if(fill_len==1){_Construct(new_finish,value);++new_finish;}else for(size_type count=0;count<fill_len;++count,++new_finish)_Construct(new_finish,value);
 if(!at_end)for(T* first=position;first!=this->_M_finish;++first,++new_finish)_Construct(new_finish,*first);
 _M_clear();_M_set(new_start,new_finish,new_start+length);
}
#endif
struct _Rb_tree_node_base {
 typedef bool _Color_type;typedef _Rb_tree_node_base*_Base_ptr;
 _Color_type _M_color;_Base_ptr _M_parent,_M_left,_M_right;
};
template<class T>struct _Rb_tree_node:public _Rb_tree_node_base{T _M_value_field;};
template<class Dummy>class _Rb_global{
public:typedef _Rb_tree_node_base*_Base_ptr;
 static _Base_ptr __cdecl _M_increment(_Base_ptr);
};
template<class Dummy>
_Rb_tree_node_base* __cdecl _Rb_global<Dummy>::_M_increment(_Base_ptr node){
 if(node->_M_right){node=node->_M_right;while(node->_M_left)node=node->_M_left;}
 else{_Base_ptr y=node->_M_parent;while(node==y->_M_right){node=y;y=y->_M_parent;}if(node->_M_right!=y)node=y;}
 return node;
}
typedef _Rb_global<bool> _Rb_global_inst;
template<class T>struct _Nonconst_traits{typedef T value_type;typedef T&reference;typedef T*pointer;};
template<class T>struct _Const_traits{typedef T value_type;typedef const T&reference;typedef const T*pointer;};
struct _Rb_tree_base_iterator{typedef _Rb_tree_node_base*_Base_ptr;_Base_ptr _M_node;
 bool operator==(const _Rb_tree_base_iterator& y)const{return _M_node==y._M_node;}
 bool operator!=(const _Rb_tree_base_iterator& y)const{return _M_node!=y._M_node;}
};
template<class T,class Traits>struct _Rb_tree_iterator:public _Rb_tree_base_iterator{
 typedef _Rb_tree_iterator<T,Traits>_Self;typedef _Rb_tree_iterator<T,_Nonconst_traits<T> >iterator;
 typedef typename Traits::reference reference;typedef typename Traits::pointer pointer;
 _Rb_tree_iterator(){_M_node=0;} _Rb_tree_iterator(_Rb_tree_node<T>*n){_M_node=n;}
 _Rb_tree_iterator(const iterator&i){_M_node=i._M_node;}
 reference operator*()const{return ((_Rb_tree_node<T>*)_M_node)->_M_value_field;}
 pointer operator->()const{return &(operator*());}
 _Self&operator++(){_M_node=_Rb_global_inst::_M_increment(_M_node);return *this;}
 _Self operator++(int){_Self old=*this;_M_node=_Rb_global_inst::_M_increment(_M_node);return old;}
};
template<class T,class A>struct _Rb_tree_base{
 typedef _Rb_tree_node<T>_Node;
 _Rb_tree_base(const A&a):_M_header(allocator<_Node>(),(_Node*)0){_M_header._M_data=_M_header.allocate(1);}
 ~_Rb_tree_base(){_M_header.deallocate(_M_header._M_data,1);}
protected:_STLP_alloc_proxy<_Node*,_Node,allocator<_Node> >_M_header;
};
template<class K,class V,class KeyOfValue,class Compare,class A>class _Rb_tree:public _Rb_tree_base<V,A>{
public:
 typedef K key_type;typedef V value_type;typedef _Rb_tree_node<V>*_Link_type;typedef _Rb_tree_node_base*_Base_ptr;
 typedef _Rb_tree_iterator<V,_Nonconst_traits<V> >iterator;typedef _Rb_tree_iterator<V,_Const_traits<V> >const_iterator;
 _Rb_tree():_Rb_tree_base<V,A>(A()),_M_node_count(0),_M_key_compare(Compare()){
  this->_M_header._M_data->_M_color=false;_M_root()=0;_M_leftmost()=this->_M_header._M_data;_M_rightmost()=this->_M_header._M_data;
 }
 _Rb_tree(const _Rb_tree&);
 ~_Rb_tree(){clear();}
 iterator begin(){return iterator(_M_leftmost());}const_iterator begin()const{return const_iterator(_M_leftmost());}
 iterator end(){return iterator(this->_M_header._M_data);}const_iterator end()const{return const_iterator(this->_M_header._M_data);}
 bool empty()const{return _M_node_count==0;}unsigned int size()const{return _M_node_count;}
 void clear(){if(_M_node_count){_M_erase(_M_root());_M_leftmost()=this->_M_header._M_data;_M_root()=0;_M_rightmost()=this->_M_header._M_data;_M_node_count=0;}}
 pair<iterator,bool>insert_unique(const V&);
 iterator find(const K&);
 void erase(iterator);
protected:
 unsigned int _M_node_count;Compare _M_key_compare;
 _Link_type&_M_root()const{return (_Link_type&)this->_M_header._M_data->_M_parent;}
 _Link_type&_M_leftmost()const{return (_Link_type&)this->_M_header._M_data->_M_left;}
 _Link_type&_M_rightmost()const{return (_Link_type&)this->_M_header._M_data->_M_right;}
protected:
 static _Link_type& __cdecl _S_left(_Base_ptr x){return (_Link_type&)x->_M_left;}
 static _Link_type& __cdecl _S_right(_Base_ptr x){return (_Link_type&)x->_M_right;}
private:void _M_erase(_Link_type);
};
template<class K,class V,class KeyOfValue,class Compare,class A>
void _Rb_tree<K,V,KeyOfValue,Compare,A>::_M_erase(_Link_type x){
 while(x){_M_erase(_S_right(x));_Link_type y=_S_left(x);_Destroy(&x->_M_value_field);this->_M_header.deallocate(x,1);x=y;}
}
template<class K,class Compare=less<K>,class A=allocator<K> >class set{
 typedef _Rb_tree<K,K,_Identity<K>,Compare,A>_Rep_type;_Rep_type _M_t;
public:
 typedef typename _Rep_type::const_iterator iterator;typedef iterator const_iterator;
 iterator begin()const{return _M_t.begin();}iterator end()const{return _M_t.end();}
 unsigned int size()const{return _M_t.size();}bool empty()const{return _M_t.empty();}void clear(){_M_t.clear();}
 pair<iterator,bool>insert(const K&v){typedef typename _Rep_type::iterator _Rep_iterator;pair<_Rep_iterator,bool>p=_M_t.insert_unique(v);return pair<iterator,bool>(p.first,p.second);}
};
template<class K,class T,class Compare=less<K>,class A=allocator<pair<const K,T> > >class map{
 typedef _Rb_tree<K,pair<const K,T>,_Select1st<pair<const K,T> >,Compare,A>_Rep_type;_Rep_type _M_t;
public:
 typedef pair<const K,T>value_type;typedef typename _Rep_type::iterator iterator;typedef typename _Rep_type::const_iterator const_iterator;
 iterator begin(){return _M_t.begin();}iterator end(){return _M_t.end();}
 iterator find(const K&k){return _M_t.find(k);}void erase(iterator i){_M_t.erase(i);}T&operator[](const K&);
};
template<class K,class T,class Compare=less<K>,class A=allocator<pair<const K,T> > >class multimap{
 typedef _Rb_tree<K,pair<const K,T>,_Select1st<pair<const K,T> >,Compare,A>_Rep_type;_Rep_type _M_t;
public:typedef typename _Rep_type::iterator iterator;void erase(iterator i){_M_t.erase(i);}
};
}
#endif

#else
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
#ifdef FABLE_RETAIL_SCOREBOARD
    __declspec(noinline) void resize(size_type n,const _Tp& x);
#else
    void resize(size_type n,_Tp x) {
        if(n<size()) erase(begin()+n,end()); else _M_fill_insert(end(),n-size(),x);
    }
#endif
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
#ifdef FABLE_STLPORT_UI_CHANGE_STATE
    _List_iterator(_List_node<_Tp>* x) : _List_iterator_base(x) { }
#else
    _List_iterator(_List_node_base* x) : _List_iterator_base(x) { }
#endif
    _List_iterator() { }
#ifdef FABLE_STLPORT_UI_CHANGE_STATE
    _List_iterator(const iterator& x) : _List_iterator_base(x._M_node) { }
#endif
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
#ifdef FABLE_STLPORT_UI_CHANGE_STATE
    iterator begin() { return iterator((_List_node<_Tp>*)this->_M_node._M_data->_M_next); }
    const_iterator begin() const { return const_iterator((_List_node<_Tp>*)this->_M_node._M_data->_M_next); }
#else
    iterator begin() { return iterator(this->_M_node._M_data->_M_next); }
    const_iterator begin() const { return const_iterator(this->_M_node._M_data->_M_next); }
#endif
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

#endif


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

#ifndef FABLE_ENGINE_ETCINTERFACETYPE_H
#define FABLE_ENGINE_ETCINTERFACETYPE_H

enum ETCInterfaceType {
    TCI_NULL = 0,
    TCI_PERFORM_ACTION = 1,
    TCI_PHYSICS = 2,
    TCI_APPEARANCE_MORPH = 3,
    TCI_HERO_STATS = 4,
    TCI_WOUND = 5,
    TCI_SMOKE_GENERATOR = 6,
    TCI_NAVIGATION = 7,
    TCI_TARGETING = 8,
    TCI_TARGETED = 9,
    TCI_CREATURE_STATS = 10,
    TCI_CREATURE_AI = 11,
    TCI_CREATURE_EXPRESSION = 12,
    TCI_ON_ACTION_USE = 13,
    TCI_CARRIED_ON_ACTION_USE = 14,
    TCI_TALK = 15,
    TCI_SOUND_PLAYER = 16,
    TCI_INVENTORY = 17,
    TCI_INVENTORY_CLOTHING = 18,
    TCI_INVENTORY_WEAPONS = 19,
    TCI_INVENTORY_STATS = 20,
    TCI_INVENTORY_MAGIC = 21,
    TCI_INVENTORY_EXPERIENCE = 22,
    TCI_INVENTORY_TRADE = 23,
    TCI_INVENTORY_AUGMENT = 24,
    TCI_INVENTORY_QUESTS = 25,
    TCI_INVENTORY_ITEM = 26,
    TCI_INVENTORY_MAP = 27,
    TCI_EDITOR = 28,
    TCI_EDITOR_ANIMATION_THING = 29,
    TCI_EDITOR_ANIMATION_CREATURE = 30,
    TCI_SCRIPTED_CONTROL = 31,
    TCI_MESH_AUTOMATIC_ENTITY_CREATOR = 32,
    TCI_THING_OWNER = 33,
    TCI_VILLAGE = 34,
    TCI_VILLAGE_MEMBER = 35,
    TCI_JACK_SHIELD = 36,
    TCI_JACK_PILLAR = 37,
    TCI_DOOR = 38,
    TCI_LIGHT = 39,
    TCI_CLOCK = 40,
    TCI_HERO = 41,
    TCI_ROTATE = 42,
    TCI_RESPAWN = 43,
    TCI_WEATHERVANE = 44,
    TCI_SINGLE_USER_ACTION_POINTS = 45,
    TCI_NAMED_USER_ACTION_POINTS = 46,
    TCI_EXCLUSIVE_USE = 47,
    TCI_COMBAT = 48,
    TCI_ENTITY_MODE_MANAGER = 49,
    TCI_CREATED_ENTITY = 50,
    TCI_OWNED_ENTITY = 51,
    TCI_PERCEIVE_THING = 52,
    TCI_BED = 53,
    TCI_STEALTH = 54,
    TCI_NOISE = 55,
    TCI_CUPBOARD = 56,
    TCI_TROPHY = 57,
    TCI_MAPWHO = 58,
    TCI_DRIVER_QUEST = 59,
    TCI_CONTAINER = 60,
    TCI_CREATURE_GENERATOR = 61,
    TCI_DRIVER_CAMERA_POINTS = 62,
    TCI_CAMERA_POINT = 63,
    TCI_DISCOVERY = 64,
    TCI_EXPLOSION = 65,
    TCI_ENTITY_EVENTS = 66,
    TCI_LOOK = 67,
    TCI_SHAPE_MANAGER = 68,
    TCI_EMOTEICON = 69,
    TCI_CARRYING = 70,
    TCI_CARRYABLE = 71,
    TCI_WEAPON = 72,
    TCI_ENEMY = 73,
    TCI_BANDIT = 74,
    TCI_PARTICLE_EMITTER = 75,
    TCI_WEAPON_TRAIL = 76,
    TCI_SOLID_TO_THING = 77,
    TCI_SMASHABLE = 78,
    TCI_CREATURE_OPINION_OF_HERO = 79,
    TCI_OPINION_SOURCE = 80,
    TCI_HERO_OPINION_DEED_LOG = 81,
    TCI_HERO_PODIUM = 82,
    TCI_SHOP = 83,
    TCI_STOCK_ITEM = 84,
    TCI_BLACKSMITHS = 85,
    TCI_TIME_APPEARANCE_FADE = 86,
    TCI_FAMILY_MEMBER = 87,
    TCI_NAVIGATION_SEED = 88,
    TCI_GIFT = 89,
    TCI_ANIMATION = 90,
    TCI_GRAPHIC_APPEARANCE_NEW = 91,
    TCI_GRAPHIC_APPEARANCE_BASIC_COMPONENT = 92,
    TCI_HERO_SUIT = 93,
    TCI_HERO_ATTACHABLE_APPEARANCE_MODIFIERS = 94,
    TCI_HIGHLIGHT_ITEM = 95,
    TCI_EMPTY_DRIVER = 96,
    TCI_CREATURE_ITEM_LEVITATION = 97,
    TCI_CREATURE_HIVE = 98,
    TCI_REGION_ENTRANCE = 99,
    TCI_REGION_EXIT = 100,
    TCI_ACTIVATION_RECEPTOR = 101,
    TCI_ACTIVATION_TRIGGER = 102,
    TCI_DEGRADABLE = 103,
    TCI_HERO_EXPERIENCE = 104,
    TCI_EXPERIENCE_ORB = 105,
    TCI_HERO_CENTRE = 106,
    TCI_HERO_GUIDE = 107,
    TCI_QUEST_CARD = 108,
    TCI_FLAMMABLE = 109,
    TCI_HERO_CENTRE_DOOR_MARKER = 110,
    TCI_HERO_ABILITIES = 111,
    TCI_BOASTING_PODIUM = 112,
    TCI_BOASTING_POSITION = 113,
    TCI_BOASTING_AREA = 114,
    TCI_BOASTING_CROWD_CONTROL = 115,
    TCI_CREATURE_GROUP_BOASTING_CROWD = 116,
    TCI_VOLUME_CONTAINMENT_TRACKER = 117,
    TCI_DEAD_SHOT_IN_THING_TRACKER = 118,
    TCI_SHOT_DEFINITION = 119,
    TCI_SKELETAL_MORPH = 120,
    TCI_SWITCHABLE_NAVIGATION = 121,
    TCI_REGION_FOLLOWER = 122,
    TCI_GATHER_POINT = 123,
    TCI_DAY_OR_NIGHT_ONLY_SUPPORT = 124,
    TCI_TRAP = 125,
    TCI_UNDEAD_SOUL = 126,
    TCI_HIT_LOCATIONS = 127,
    TCI_MONEY_RECEIVER = 128,
    TCI_HERO_RANGED_ATTACKS = 129,
    TCI_PARTICLE_ATTACHER = 130,
    TCI_ANIMATING_OBJECT = 131,
    TCI_BERSERK = 132,
    TCI_DIVINE_WRATH = 133,
    TCI_UNHOLY_POWER = 134,
    TCI_MULTI_STRIKE = 135,
    TCI_MULTI_ARROW = 136,
    TCI_BURNT_EFFECT = 137,
    TCI_ELECTROCUTED_EFFECT = 138,
    TCI_CREATURE_TINT_EFFECT = 139,
    TCI_HASTE = 140,
    TCI_FORCE_PUSHED = 141,
    TCI_REGION_DISPLAY = 142,
    TCI_EXPRESSION = 143,
    TCI_SHOP_KEEPER = 144,
    TCI_SUMMON_SPELL = 145,
    TCI_BULLET_TIME = 146,
    TCI_PHYSICAL_SHIELD_SPELL = 147,
    TCI_WILL_LIGHTNING = 148,
    TCI_WILL_ENFLAME = 149,
    TCI_FORCE_PUSH_POWER = 150,
    TCI_DRAIN_LIFE = 151,
    TCI_LIFE_DRAINABLE = 152,
    TCI_HEAL_LIFE = 153,
    TCI_GHOST_SWORD = 154,
    TCI_EFFECT_ON_DIE = 155,
    TCI_WILL_RESPONSE = 156,
    TCI_FIREBALL_SPELL = 157,
    TCI_TURNCOAT_SPELL = 158,
    TCI_TURNCOAT = 159,
    TCI_SUMMONABLE_CREATURE = 160,
    TCI_SUMMONNED_CREATURE = 161,
    TCI_INTERNAL_MARKER = 162,
    TCI_CREATURE_GROUP = 163,
    TCI_CREATURE_GROUP_TAG = 164,
    TCI_MESH_REPLACER = 165,
    TCI_PRE_CALCULATED_NAVIGATION_ROUTE = 166,
    TCI_SPECIAL_ABILITIES = 167,
    TCI_BATTLE_CHARGE = 168,
    TCI_ASSASSIN_RUSH = 169,
    TCI_AI_SCRATCHPAD = 170,
    TCI_TAVERN = 171,
    TCI_SINGLE_PERSON_OCCUPIABLE = 172,
    TCI_BOSS = 173,
    TCI_ROCK_TROLL_SHIELD = 174,
    TCI_COMBAT_LEADER = 175,
    TCI_FISHING_SPOT = 176,
    TCI_FISHING = 177,
    TCI_STEALABLE_ITEM_LOCATION = 178,
    TCI_INTERESTING_TO_VILLAGERS = 179,
    TCI_GUARD = 180,
    TCI_CRATE_STACK = 181,
    TCI_WATER_WADER = 182,
    TCI_OVERHEAD_DISPLAY = 183,
    TCI_TAVERN_TABLE = 184,
    TCI_TAVERN_BAR = 185,
    TCI_TAVERN_TANKARD = 186,
    TCI_TAVERN_JUG = 187,
    TCI_FOLLOWED = 188,
    TCI_TRESPASSABLE = 189,
    TCI_TRESPASSER = 190,
    TCI_OBJECT_AUGMENTATIONS = 191,
    TCI_AUGMENTATION = 192,
    TCI_GOSSIP = 193,
    TCI_ACTION_USE_SCRIPTED_HOOK = 194,
    TCI_DRUNKENNESS = 195,
    TCI_ON_HERO_PUSH = 196,
    TCI_VILLAGE_CRIME = 197,
    TCI_IN_GAME_MENU = 198,
    TCI_COOP_SPIRIT = 199,
    TCI_HIT_ON_COLLISION = 200,
    TCI_GENERATES_EXPERIENCE_ON_KILLING = 201,
    TCI_CREATURE_GENERATOR_CREATOR = 202,
    TCI_HERO_RECEIVE_ITEMS = 203,
    TCI_GOLD = 204,
    TCI_CREATURE_HIT_NOTIFICATION = 205,
    TCI_AICREATURE_WILL_POWER_INDICATOR = 206,
    TCI_TAVERN_GAME = 207,
    TCI_GAME_PLAYER = 208,
    TCI_DECAPITATION = 209,
    TCI_TAVERN_GAME_MASTER = 210,
    TCI_IDLE_SCHEDULER = 211,
    TCI_SLEEP = 212,
    TCI_BUYABLE_HOUSE = 213,
    TCI_DUMMY_VILLAGER = 214,
    TCI_WIFE = 215,
    TCI_PARTICLE_ATTACHMENT_TRACKER = 216,
    TCI_CREATURE_FLY_ON_SPLINE = 217,
    TCI_WALL_MOUNT = 218,
    TCI_WALL_MOUNT_EFFECTS = 219,
    TCI_FISH = 220,
    TCI_TELEPORTER = 221,
    TCI_EXPLODING_OBJECT = 222,
    TCI_HERO_SPECIAL_MOVEMENT = 223,
    TCI_HERO_REMEMBER_CRIMES = 224,
    TCI_RESURRECTION_ITEM = 225,
    TCI_KRAKEN = 226,
    TCI_KRAKEN_TENTACLE = 227,
    TCI_CREATURE_FLY_IN_CIRCLES = 228,
    TCI_INFO_DISPLAY = 229,
    TCI_SPOT_LIGHT = 230,
    TCI_AMBIENT_BIRD = 231,
    TCI_BOAST_UI = 232,
    TCI_TEXTURE_DECAL = 233,
    TCI_FISHING_ROD = 234,
    TCI_THING_UID_DUMMY_FOR_SERIALISATION = 235,
    TCI_RUMBLE = 236,
    TCI_PHYSICAL_OBSTRUCTION = 237,
    TCI_ELECTROCUTE_ON_TOUCH = 238,
    TCI_DIGGING_SPOT = 239,
    TCI_DICE_GAME = 240,
    TCI_PLOT = 241,
    TCI_BOB = 242,
    TCI_FINE_DIALOGUE = 243,
    TCI_SOUND_ATMOSPHERE = 244,
    TCI_NYMPH = 245,
    TCI_QUICK_ACCESS_MENU = 246,
    TCI_WOBBLE = 247,
    TCI_CAMERA_COLLISION = 248,
    TCI_FADE_OUT_AND_IN = 249,
    TCI_AI_SUMMON = 250,
    TCI_FLASH_ON_HIT = 251,
    TCI_QUEST_COMPLETION_UI = 252,
    TCI_DESTROY_THING_ON_ACTION_FINISH = 253,
    TCI_OBSTACLE = 254,
    TCI_ATMOS_PLAYER = 255,
    TCI_FLOURISH_TARGET = 256,
    TCI_CREDITS_UI = 257,
    TCI_BETTING = 258,
    TCI_LIFE_TIMER = 259,
    TCI_FIRE_RANGED_WEAPON_ON_LEAVING_STATE_GROUP = 260,
    TCI_AVOID_REGION_EXIT = 261,
    TCI_GRAPHIC_APPEARANCE_STATIC_MESH_FADE_OUT_ON_PROXIMITY = 262,
    TCI_ORACLE_MINIGAME = 263,
    TCI_FIREHEART_MINIGAME = 264,
    TCI_DESTROY_THING_ON_SPECIAL_ABILITY_FINISH = 265,
    TCI_LIGHTNING_ORB = 266,
    TCI_LIGHTNING_NODE = 267,
    TCI_HAS_NAME = 268,
    TCI_HERO_ONLINE_SCOREBOARD = 269,
    TCI_PHOTOJOURNAL_CAPTURE = 270,
    TCI_EXPLOSIVE_TRAIL = 271,
    TCI_ABSORB_HITS = 272,
    TCI_GENERATED_CREATURE = 273,
    MAX_NO_TCI_TYPES = 274
};
#endif

class C3DVector; class CTCPhysicsBase; class CTCBase; class CTCDriverBase;
class CThingBaseDef; class CWorld; class CPlayerManager;class CTCParamsBase;
namespace NGraphicAppearance { class CTCGraphicAppearance; }
#ifdef FABLE_RETAIL_TYPED_GETTC


template<class From,class To> inline To __fastcall GFDebugPDynamicCast(From value,To) { return static_cast<To>(value); }
#endif
class CThing : public CClass {
#ifdef FABLE_ONHIT_TEST_ACCESS
    friend class COnHitFixture;
#endif

#ifdef FABLE_AUTOPLACE_TEST_ACCESS
    friend class CAutoPlaceFixture;
#endif
protected:
#ifdef FABLE_RETAIL_CTHING_CONSTRUCTORS
    CThing(unsigned long);
#endif
    unsigned __int64 UniqueID;
    std::bitset<274> UsingComponents;
    CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*> > Components;
    std::list<CTCBase*> UpdateComponentsList;
    CCharString SerialisationSectionName;
    CTCDriverBase* PTCDriver;
    CTCPhysicsBase* PhysicsTC;
    NGraphicAppearance::CTCGraphicAppearance* PTCGraphicAppearance;
    long WorldFrameCreatedAt;
    unsigned long ThingTypeFlags;
    CDefPointer<const CThingBaseDef> PBaseDef;
    CCharString ScriptName;
    CCharString ScriptData;
public:
    CThing* RenderListPrev;
    CThing* RenderListNext;
    CThing* UpdateListPrev;
    CThing* UpdateListNext;
protected:
    unsigned short DefGlobalIndex;
    short CurrentMapNumber;
    signed char PlayerNumber;

protected:
    unsigned char Dead : 1;
    unsigned char CreatedByOwnerThingCreation : 1;
    unsigned char IncludeInThingScans : 1;
    unsigned char Activated : 1;
    unsigned char GloballyLoaded : 1;
    unsigned char InLimbo : 1;
    unsigned char KillLocked : 1;
    unsigned char CreatedBySerialisation : 1;
    unsigned char DeletedOnOwnerLevelOutOfScope : 1;
    unsigned char Drawable : 1;
    unsigned char Dying : 1;
    unsigned char ForceFrameUpdate : 1;
    unsigned char LevelPersistent : 1;
    unsigned char GameStatePersisted : 1;
    unsigned char PersistOnUnloadWhenInLimbo : 1;
    unsigned char PausedDuringCutscenes : 1;
    unsigned char NotPausedDuringCutscenes : 1;
    unsigned char DeathPersistent : 1;
    unsigned char KillOnLevelUnload : 1;
    unsigned char ActivatedScript : 1;
    unsigned char LevelIsUnloading : 1;
    unsigned char IsAffectedByBulletTimeOverride : 1;
    unsigned char AddingTCsPermitted : 1;

private:
    unsigned char InitialisationStage : 3;
public:
    void Kill(bool); 
    CTCBase* AddTC(const CCharString&,bool,const CTCParamsBase*);
    CDefPointer<const CThingBaseDef> GetPBaseDef() const { return PBaseDef; }
    bool IsDead() const { return Dead!=0; }
    bool IsIncludedInThingScans() const { return IncludeInThingScans && Activated && !Dead && !InLimbo; }
    bool IsAlive() const { return !Dead; }
    bool HasTC(ETCInterfaceType type) const { return UsingComponents.test(type); }
#ifdef FABLE_RETAIL_TYPED_GETTC
    template<class T> __forceinline bool GetTC(T** result) {
        if(HasTC(T::StatGetInterfaceType())) {
            *result=GFDebugPDynamicCast(Components.Find(T::StatGetInterfaceType())->second,(T*)0);
            return true;
        }
        return false;
    }
    template<class T> __forceinline bool PeekTC(T** result) const {
        if(HasTC(T::StatGetInterfaceType())) {
            *result=GFDebugPDynamicCast(static_cast<const CTCBase*>(Components.Find(T::StatGetInterfaceType())->second),(T*)0);
            return true;
        }
        return false;
    }
#endif
#ifdef FABLE_RETAIL_TYPED_GETTCDRIVER
    
    
    template<class T> __forceinline bool GetTCDriver(T** result) {
        const bool& present=HasTC(T::StatGetInterfaceType());
        if(present) *result=GFDebugPDynamicCast(&GetTC(T::StatGetInterfaceType()),(T*)0);
        return present;
    }
#endif
#ifdef FABLE_RETAIL_FORCE_GETTC
    __forceinline CTCBase& GetTC(ETCInterfaceType type) { return *Components.Find(type)->second; }
    __forceinline const CTCBase& GetTC(ETCInterfaceType type) const { return *Components.Find(type)->second; }
#else
    CTCBase& GetTC(ETCInterfaceType type) { return *Components.Find(type)->second; }
    const CTCBase& GetTC(ETCInterfaceType type) const { return *Components.Find(type)->second; }
#endif
    CTCPhysicsBase* GetPhysicsTC() { return PhysicsTC; }
    static void CheckRetailLayout() {
        FABLE_STATIC_ASSERT(offsetof(CThing,UniqueID)==0x18);
        FABLE_STATIC_ASSERT(offsetof(CThing,UsingComponents)==0x20);
        FABLE_STATIC_ASSERT(offsetof(CThing,Components)==0x44);
        FABLE_STATIC_ASSERT(offsetof(CThing,PhysicsTC)==0x60);
        FABLE_STATIC_ASSERT(offsetof(CThing,PBaseDef)==0x70);
    }
    const CTCPhysicsBase* GetPhysicsTC() const { return PhysicsTC; }
    const C3DVector& GetPos() const;
    C3DVector GetPhysPos() const;
protected:
    CWorld& GetWorld();
    const CWorld& PeekWorld() const;
    unsigned long* GetPWorldSeed() const;
    CPlayerManager& GetPlayerManager();
};
FABLE_STATIC_ASSERT(sizeof(CThing)==0x98);

#endif
#else
#ifndef FABLE_ENGINE_CTHING_H
#define FABLE_ENGINE_CTHING_H






#include <stddef.h>


class CCharStringData;
class CDefPointeeBase;
class CTCDriverBase;
struct CTCGraphicAppearance;
class CTCPhysicsBase;

#pragma pack(push, 1)
class CThing {
public:
    void*                 __vftable;                         
    unsigned char         _pad_0x04[0x4];                    
    unsigned char         _pad_0x08[0x4];                    
    unsigned char         _pad_0x0c[0x4];                    
    unsigned char         _pad_0x10[0x1];                    
    unsigned char         _pad_0x11[0x7];                    
    unsigned __int64      UniqueID;                          
    unsigned char         UsingComponents[0x24];             
    unsigned char         Components[0x10];                  
    unsigned char         UpdateComponentsList[0x4];         
    CCharStringData*      SerialisationSectionName_PStringData; 
    CTCDriverBase*        PTCDriver;                         
    CTCPhysicsBase*       PhysicsTC;                         
    CTCGraphicAppearance* PTCGraphicAppearance;              
    long                  WorldFrameCreatedAt;               
    unsigned long         ThingTypeFlags;                    
    CDefPointeeBase*      PBaseDef_Object;                   
    CCharStringData*      ScriptName_PStringData;            
    CCharStringData*      ScriptData_PStringData;            
    CThing*               RenderListPrev;                    
    CThing*               RenderListNext;                    
    CThing*               UpdateListPrev;                    
    CThing*               UpdateListNext;                    
    unsigned short        DefGlobalIndex;                    
    short                 CurrentMapNumber;                  
    signed char           PlayerNumber;                      
    unsigned char         CreatedBySerialisation[0x1];       
    unsigned char         PausedDuringCutscenes[0x1];        
    unsigned char         AddingTCsPermitted[0x1];           
    unsigned char         InitialisationStage[0x4];          
};
#pragma pack(pop)

FABLE_STATIC_ASSERT(sizeof(CThing) == 0x98);
FABLE_STATIC_ASSERT(offsetof(CThing, __vftable) == 0x0);
FABLE_STATIC_ASSERT(offsetof(CThing, UniqueID) == 0x18);
FABLE_STATIC_ASSERT(offsetof(CThing, UsingComponents) == 0x20);
FABLE_STATIC_ASSERT(offsetof(CThing, Components) == 0x44);
FABLE_STATIC_ASSERT(offsetof(CThing, UpdateComponentsList) == 0x54);
FABLE_STATIC_ASSERT(offsetof(CThing, SerialisationSectionName_PStringData) == 0x58);
FABLE_STATIC_ASSERT(offsetof(CThing, PTCDriver) == 0x5c);
FABLE_STATIC_ASSERT(offsetof(CThing, PhysicsTC) == 0x60);
FABLE_STATIC_ASSERT(offsetof(CThing, PTCGraphicAppearance) == 0x64);
FABLE_STATIC_ASSERT(offsetof(CThing, WorldFrameCreatedAt) == 0x68);
FABLE_STATIC_ASSERT(offsetof(CThing, ThingTypeFlags) == 0x6c);
FABLE_STATIC_ASSERT(offsetof(CThing, PBaseDef_Object) == 0x70);
FABLE_STATIC_ASSERT(offsetof(CThing, ScriptName_PStringData) == 0x74);
FABLE_STATIC_ASSERT(offsetof(CThing, ScriptData_PStringData) == 0x78);
FABLE_STATIC_ASSERT(offsetof(CThing, RenderListPrev) == 0x7c);
FABLE_STATIC_ASSERT(offsetof(CThing, RenderListNext) == 0x80);
FABLE_STATIC_ASSERT(offsetof(CThing, UpdateListPrev) == 0x84);
FABLE_STATIC_ASSERT(offsetof(CThing, UpdateListNext) == 0x88);
FABLE_STATIC_ASSERT(offsetof(CThing, DefGlobalIndex) == 0x8c);
FABLE_STATIC_ASSERT(offsetof(CThing, CurrentMapNumber) == 0x8e);
FABLE_STATIC_ASSERT(offsetof(CThing, PlayerNumber) == 0x90);
FABLE_STATIC_ASSERT(offsetof(CThing, CreatedBySerialisation) == 0x91);
FABLE_STATIC_ASSERT(offsetof(CThing, PausedDuringCutscenes) == 0x92);
FABLE_STATIC_ASSERT(offsetof(CThing, AddingTCsPermitted) == 0x93);
FABLE_STATIC_ASSERT(offsetof(CThing, InitialisationStage) == 0x94);

#endif 

#endif

#ifndef FABLE_ENGINE_CINTELLIGENTPOINTER_H
#define FABLE_ENGINE_CINTELLIGENTPOINTER_H

class CBaseObject;
class CBaseObjectPointer;


class CBaseIntelligentPointer {
public:
#ifdef FABLE_RETAIL_PERSIST_OPINION
    CBaseIntelligentPointer();
#endif
    virtual ~CBaseIntelligentPointer();
    CBaseObject* GetPItem() const;
private:
    CBaseObjectPointer* PData;
};
template<class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
#ifdef FABLE_RETAIL_PERSIST_OPINION
    CIntelligentPointer() { }
#endif
    T* Get() const { return static_cast<T*>(GetPItem()); }
    T* operator->() const { return Get(); }
};
FABLE_STATIC_ASSERT(sizeof(CBaseIntelligentPointer)==8);
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

#endif 

class CThingPhysical : public CThing {
#ifdef FABLE_ONHIT_TEST_ACCESS
    friend class COnHitFixture;
#endif

private:
    unsigned char HealthOverTimeEffects[4];
protected:
    float Scale;
    CIntelligentPointer<CThingPhysical> PLastCreator;
    CIntelligentPointer<CThingPhysical> POriginalCreator;
    float MaxHealth;
    float Health;
    long CanComeBetweenCameraAndObservedThing;
    unsigned char Damageable:1, Killable:1, RespondToHits:1, EvadeAllHits:1,
        NegateAllHits:1, AbleToBeEngagedInCombat:1;
public:
#ifdef FABLE_RETAIL_CTHING_CONSTRUCTORS
    CThingPhysical(unsigned long);
#endif
    const C3DVector& PeekPos() const;
    float GetHealth() const { return Health; }
};
FABLE_STATIC_ASSERT(sizeof(CThingPhysical)==0xc0);
#endif
#else
#ifndef FABLE_ENGINE_CTHINGPHYSICAL_H
#define FABLE_ENGINE_CTHINGPHYSICAL_H






#include <stddef.h>


#pragma pack(push, 1)
class CThingPhysical {
public:
    void*         __vftable;                                 
    unsigned char _pad_0x04[0x4];                            
    unsigned char _pad_0x08[0x4];                            
    unsigned char _pad_0x0c[0x4];                            
    unsigned char _pad_0x10[0x1];                            
    unsigned char _pad_0x11[0x7];                            
    unsigned char _pad_0x18[0x4];                            
    unsigned char _pad_0x1c[0x4];                            
    unsigned char _pad_0x20[0x38];                           
    unsigned char _pad_0x58[0x4];                            
    unsigned char _pad_0x5c[0x4];                            
    unsigned char _pad_0x60[0x4];                            
    long          f64;                                       
    unsigned char _pad_0x68[0x4];                            
    unsigned char _pad_0x6c[0x4];                            
    unsigned char _pad_0x70[0x4];                            
    unsigned char _pad_0x74[0x4];                            
    unsigned char _pad_0x78[0x4];                            
    unsigned char _pad_0x7c[0x4];                            
    unsigned char _pad_0x80[0x4];                            
    unsigned char _pad_0x84[0x4];                            
    unsigned char _pad_0x88[0x4];                            
    unsigned char _pad_0x8c[0x4];                            
    unsigned char _pad_0x90[0x4];                            
    unsigned char _pad_0x94[0x1];                            
    unsigned char _pad_0x95[0x3];                            
    unsigned char HealthOverTimeEffects[0x4];                
    float         Scale;                                     
    unsigned char PLastCreator[0x8];                         
    unsigned char POriginalCreator[0x8];                     
    float         MaxHealth;                                 
    float         Health;                                    
    long          CanComeBetweenCameraAndObservedThing;      
    unsigned char AbleToBeEngagedInCombat[0x4];              
};
#pragma pack(pop)

FABLE_STATIC_ASSERT(sizeof(CThingPhysical) == 0xc0);
FABLE_STATIC_ASSERT(offsetof(CThingPhysical, __vftable) == 0x0);
FABLE_STATIC_ASSERT(offsetof(CThingPhysical, f64) == 0x64);
FABLE_STATIC_ASSERT(offsetof(CThingPhysical, HealthOverTimeEffects) == 0x98);
FABLE_STATIC_ASSERT(offsetof(CThingPhysical, Scale) == 0x9c);
FABLE_STATIC_ASSERT(offsetof(CThingPhysical, PLastCreator) == 0xa0);
FABLE_STATIC_ASSERT(offsetof(CThingPhysical, POriginalCreator) == 0xa8);
FABLE_STATIC_ASSERT(offsetof(CThingPhysical, MaxHealth) == 0xb0);
FABLE_STATIC_ASSERT(offsetof(CThingPhysical, Health) == 0xb4);
FABLE_STATIC_ASSERT(offsetof(CThingPhysical, CanComeBetweenCameraAndObservedThing) == 0xb8);
FABLE_STATIC_ASSERT(offsetof(CThingPhysical, AbleToBeEngagedInCombat) == 0xbc);

#endif 

#endif




class CHitParameters;
class CThingGameObject : public CThingPhysical {
protected:
    CIntelligentPointer<CThing> PThingStandingOn;
private:
    unsigned char AddToComboMultiplierOnHit:1,AddToComboMultiplierOnHitOverrideSet:1,
        GiveHeroStatChangesOnBeingHit:1,GiveHeroStatChangesOnBeingHitOverrideSet:1;
public:
#ifdef FABLE_RETAIL_CTHING_CONSTRUCTORS
    CThingGameObject(unsigned long);
#endif
    virtual void OnHit(CHitParameters&);
};
FABLE_STATIC_ASSERT(sizeof(CThingGameObject)==0xd0);
#endif
#else
#ifndef FABLE_ENGINE_CTHINGGAMEOBJECT_H
#define FABLE_ENGINE_CTHINGGAMEOBJECT_H






#include <stddef.h>


#pragma pack(push, 1)
class CThingGameObject {
public:
    void*         __vftable;                                 
    unsigned char _pad_0x04[0x4];                            
    unsigned char _pad_0x08[0x4];                            
    unsigned char _pad_0x0c[0x4];                            
    unsigned char _pad_0x10[0x1];                            
    unsigned char _pad_0x11[0x7];                            
    unsigned char _pad_0x18[0x4];                            
    unsigned char _pad_0x1c[0x4];                            
    unsigned char _pad_0x20[0x38];                           
    unsigned char _pad_0x58[0x4];                            
    unsigned char _pad_0x5c[0x4];                            
    unsigned char _pad_0x60[0x4];                            
    unsigned char _pad_0x64[0x4];                            
    unsigned char _pad_0x68[0x4];                            
    unsigned char _pad_0x6c[0x4];                            
    unsigned char _pad_0x70[0x4];                            
    unsigned char _pad_0x74[0x4];                            
    unsigned char _pad_0x78[0x4];                            
    unsigned char _pad_0x7c[0x4];                            
    unsigned char _pad_0x80[0x4];                            
    unsigned char _pad_0x84[0x4];                            
    unsigned char _pad_0x88[0x4];                            
    unsigned char _pad_0x8c[0x4];                            
    unsigned char _pad_0x90[0x4];                            
    unsigned char _pad_0x94[0x1];                            
    unsigned char _pad_0x95[0x7];                            
    unsigned char _pad_0x9c[0x4];                            
    unsigned char _pad_0xa0[0x4];                            
    unsigned char _pad_0xa4[0x4];                            
    unsigned char _pad_0xa8[0x4];                            
    unsigned char _pad_0xac[0x4];                            
    unsigned char _pad_0xb0[0x1];                            
    unsigned char _pad_0xb1[0x3];                            
    unsigned char _pad_0xb4[0x4];                            
    unsigned char _pad_0xb8[0x4];                            
    unsigned char _pad_0xbc[0x1];                            
    unsigned char _pad_0xbd[0x3];                            
    unsigned char PThingStandingOn[0x8];                     
    unsigned char GiveHeroStatChangesOnBeingHitOverrideSet[0x8]; 
};
#pragma pack(pop)

FABLE_STATIC_ASSERT(sizeof(CThingGameObject) == 0xd0);
FABLE_STATIC_ASSERT(offsetof(CThingGameObject, __vftable) == 0x0);
FABLE_STATIC_ASSERT(offsetof(CThingGameObject, PThingStandingOn) == 0xc0);
FABLE_STATIC_ASSERT(offsetof(CThingGameObject, GiveHeroStatChangesOnBeingHitOverrideSet) == 0xc8);

#endif 

#endif




class CThingBuilding; class CThingCreatureDef; class CCreatureActionBase;
class CCreatureInteraction; class CTCCreatureModeManager;
class CThingCreatureBase : public CThingGameObject {
#ifdef FABLE_ONHIT_TEST_ACCESS
    friend class COnHitFixture;
#endif

#ifdef FABLE_AUTOPLACE_TEST_ACCESS
    friend class CAutoPlaceFixture;
#endif
private:
    unsigned long LastMessageEventICreatedID;
    unsigned char CombatCollisionDebugGraphics[12];
protected:
    CDefPointer<const CThingCreatureDef> PDef;
    long ShotAccuracyPercentage;
    C3DVector InitialPos;
    CIntelligentPointer<CThingCreatureBase> PLastAttackedByCreature;
    unsigned long WFLastAttackedByCreature;
    CCountedPointer<CCreatureActionBase> PCurrentAction;
    std::list<CCountedPointer<CCreatureActionBase> > PQueuedActions;
    C3DVector MovementVector;
    C3DVector HeadPosOffset;
    long IdleCounter;
    float TurnSpeed;
    CCountedPointer<CCreatureInteraction> PCreatureInteraction;
    CTCCreatureModeManager* PTCModeManager;
    unsigned char PreviousActionHandedness[4];
    long PreviousActionHandednessWF;
    unsigned char BodyReorienter[4];
    unsigned char CombatDebugGraphics[12];
    CIntelligentPointer<CThing> PItemToUnsheatheAfterCutscene;
    char* DebugText;
    bool CurrentlyFrameUpdating:1,Blinking:1,MeleeAttacker:1,MeleeAttacked:1,
        LeaveDeadBodyOverride:1,LeaveExperienceOrbsOverride:1,FollowHeroThroughTeleporters:1,HeadPosCached:1;
    bool UseMovementInActions:1,UpdateMovementVectorThisFrame:1,AbortingCurrentAction:1,DemonDoor:1,
        Oracle:1,HadFacingAngleYZSetByBodyOrientation:1,UnderInfluenceOfEpicSpell:1,HiddenOnMiniMap:1;
    bool IsShowingDebugInfo:1;
    void ReactToGenericStrike(const CHitParameters&);
public:
#ifdef FABLE_RETAIL_CTHING_CONSTRUCTORS
    CThingCreatureBase(unsigned long);
#endif
    bool AutoPlaceNearBuilding(const CThingBuilding*,float);
    static void CheckRetailLayout() {
        FABLE_STATIC_ASSERT(offsetof(CThingCreatureBase,PDef)==0xe0);
        FABLE_STATIC_ASSERT(offsetof(CThingCreatureBase,PCurrentAction)==0x100);
        FABLE_STATIC_ASSERT(offsetof(CThingCreatureBase,PQueuedActions)==0x108);
    }
    virtual void OnHit(CHitParameters&);
    bool SetCurrentAction(const CCreatureActionBase&);
};
FABLE_STATIC_ASSERT(sizeof(CThingCreatureBase)==0x160);
#endif
#else
#ifndef FABLE_ENGINE_CTHINGCREATUREBASE_H
#define FABLE_ENGINE_CTHINGCREATUREBASE_H






#include <stddef.h>


class CCPPointerInfo;
class CCreatureActionBase;
class CCreatureInteraction;
class CDefPointeeBase;
class CTCCreatureModeManager;
class CThingBodyReorienter;

#pragma pack(push, 1)
class CThingCreatureBase {
public:
    void*                   __vftable;                       
    unsigned char           _pad_0x04[0x4];                  
    unsigned char           _pad_0x08[0x4];                  
    unsigned char           _pad_0x0c[0x4];                  
    unsigned char           _pad_0x10[0x1];                  
    unsigned char           _pad_0x11[0x7];                  
    unsigned char           _pad_0x18[0x4];                  
    unsigned char           _pad_0x1c[0x4];                  
    unsigned char           _pad_0x20[0x38];                 
    unsigned char           _pad_0x58[0x4];                  
    unsigned char           _pad_0x5c[0x4];                  
    long                    q;                               
    unsigned char           _pad_0x64[0x4];                  
    unsigned char           _pad_0x68[0x4];                  
    unsigned char           _pad_0x6c[0x4];                  
    unsigned char           _pad_0x70[0x4];                  
    unsigned char           _pad_0x74[0x4];                  
    unsigned char           _pad_0x78[0x4];                  
    unsigned char           _pad_0x7c[0x4];                  
    unsigned char           _pad_0x80[0x4];                  
    unsigned char           _pad_0x84[0x4];                  
    unsigned char           _pad_0x88[0x4];                  
    unsigned char           _pad_0x8c[0x4];                  
    unsigned char           _pad_0x90[0x4];                  
    unsigned char           _pad_0x94[0x1];                  
    unsigned char           _pad_0x95[0x7];                  
    unsigned char           _pad_0x9c[0x4];                  
    unsigned char           _pad_0xa0[0x4];                  
    unsigned char           _pad_0xa4[0x4];                  
    unsigned char           _pad_0xa8[0x4];                  
    unsigned char           _pad_0xac[0x4];                  
    unsigned char           _pad_0xb0[0x4];                  
    unsigned char           _pad_0xb4[0x4];                  
    unsigned char           _pad_0xb8[0x4];                  
    unsigned char           _pad_0xbc[0x1];                  
    unsigned char           _pad_0xbd[0x3];                  
    unsigned char           _pad_0xc0[0x4];                  
    unsigned char           _pad_0xc4[0x4];                  
    unsigned char           _pad_0xc8[0x8];                  
    unsigned long           LastMessageEventICreatedID;      
    unsigned char           CombatCollisionDebugGraphics[0xc];  
    CDefPointeeBase*        PDef_Object;                     
    long                    ShotAccuracyPercentage;          
    float                   InitialPos_X;                    
    float                   InitialPos_Y;                    
    float                   InitialPos_Z;                    
    unsigned char           PLastAttackedByCreature[0x8];    
    unsigned long           WFLastAttackedByCreature;        
    CCreatureActionBase*    PCurrentAction_Data;             
    CCPPointerInfo*         PCurrentAction_Info;             
    unsigned char           PQueuedActions[0x4];             
    float                   MovementVector_X;                
    float                   MovementVector_Y;                
    float                   MovementVector_Z;                
    float                   HeadPosOffset_X;                 
    float                   HeadPosOffset_Y;                 
    float                   HeadPosOffset_Z;                 
    long                    IdleCounter;                     
    float                   TurnSpeed;                       
    CCreatureInteraction*   PCreatureInteraction_Data;       
    CCPPointerInfo*         PCreatureInteraction_Info;       
    CTCCreatureModeManager* PTCModeManager;                  
    unsigned char           PreviousActionHandedness[0x4];   
    long                    PreviousActionHandednessWF;      
    CThingBodyReorienter*   BodyReorienter_ptr;              
    unsigned char           CombatDebugGraphics[0xc];        
    unsigned char           PItemToUnsheatheAfterCutscene[0x8]; 
    void*                   DebugText;                       
    unsigned char           HeadPosCached[0x1];              
    unsigned char           HiddenOnMiniMap[0x1];            
    unsigned char           IsShowingDebugInfo[0x2];         
};
#pragma pack(pop)

FABLE_STATIC_ASSERT(sizeof(CThingCreatureBase) == 0x160);
FABLE_STATIC_ASSERT(offsetof(CThingCreatureBase, __vftable) == 0x0);
FABLE_STATIC_ASSERT(offsetof(CThingCreatureBase, q) == 0x60);
FABLE_STATIC_ASSERT(offsetof(CThingCreatureBase, LastMessageEventICreatedID) == 0xd0);
FABLE_STATIC_ASSERT(offsetof(CThingCreatureBase, CombatCollisionDebugGraphics) == 0xd4);
FABLE_STATIC_ASSERT(offsetof(CThingCreatureBase, PDef_Object) == 0xe0);
FABLE_STATIC_ASSERT(offsetof(CThingCreatureBase, ShotAccuracyPercentage) == 0xe4);
FABLE_STATIC_ASSERT(offsetof(CThingCreatureBase, InitialPos_X) == 0xe8);
FABLE_STATIC_ASSERT(offsetof(CThingCreatureBase, InitialPos_Y) == 0xec);
FABLE_STATIC_ASSERT(offsetof(CThingCreatureBase, InitialPos_Z) == 0xf0);
FABLE_STATIC_ASSERT(offsetof(CThingCreatureBase, PLastAttackedByCreature) == 0xf4);
FABLE_STATIC_ASSERT(offsetof(CThingCreatureBase, WFLastAttackedByCreature) == 0xfc);
FABLE_STATIC_ASSERT(offsetof(CThingCreatureBase, PCurrentAction_Data) == 0x100);
FABLE_STATIC_ASSERT(offsetof(CThingCreatureBase, PCurrentAction_Info) == 0x104);
FABLE_STATIC_ASSERT(offsetof(CThingCreatureBase, PQueuedActions) == 0x108);
FABLE_STATIC_ASSERT(offsetof(CThingCreatureBase, MovementVector_X) == 0x10c);
FABLE_STATIC_ASSERT(offsetof(CThingCreatureBase, MovementVector_Y) == 0x110);
FABLE_STATIC_ASSERT(offsetof(CThingCreatureBase, MovementVector_Z) == 0x114);
FABLE_STATIC_ASSERT(offsetof(CThingCreatureBase, HeadPosOffset_X) == 0x118);
FABLE_STATIC_ASSERT(offsetof(CThingCreatureBase, HeadPosOffset_Y) == 0x11c);
FABLE_STATIC_ASSERT(offsetof(CThingCreatureBase, HeadPosOffset_Z) == 0x120);
FABLE_STATIC_ASSERT(offsetof(CThingCreatureBase, IdleCounter) == 0x124);
FABLE_STATIC_ASSERT(offsetof(CThingCreatureBase, TurnSpeed) == 0x128);
FABLE_STATIC_ASSERT(offsetof(CThingCreatureBase, PCreatureInteraction_Data) == 0x12c);
FABLE_STATIC_ASSERT(offsetof(CThingCreatureBase, PCreatureInteraction_Info) == 0x130);
FABLE_STATIC_ASSERT(offsetof(CThingCreatureBase, PTCModeManager) == 0x134);
FABLE_STATIC_ASSERT(offsetof(CThingCreatureBase, PreviousActionHandedness) == 0x138);
FABLE_STATIC_ASSERT(offsetof(CThingCreatureBase, PreviousActionHandednessWF) == 0x13c);
FABLE_STATIC_ASSERT(offsetof(CThingCreatureBase, BodyReorienter_ptr) == 0x140);
FABLE_STATIC_ASSERT(offsetof(CThingCreatureBase, CombatDebugGraphics) == 0x144);
FABLE_STATIC_ASSERT(offsetof(CThingCreatureBase, PItemToUnsheatheAfterCutscene) == 0x150);
FABLE_STATIC_ASSERT(offsetof(CThingCreatureBase, DebugText) == 0x158);
FABLE_STATIC_ASSERT(offsetof(CThingCreatureBase, HeadPosCached) == 0x15c);
FABLE_STATIC_ASSERT(offsetof(CThingCreatureBase, HiddenOnMiniMap) == 0x15d);
FABLE_STATIC_ASSERT(offsetof(CThingCreatureBase, IsShowingDebugInfo) == 0x15e);

#endif 

#endif

#ifndef FABLE_ENGINE_CTHINGBUILDING_H
#define FABLE_ENGINE_CTHINGBUILDING_H


class CThingBuilding : public CThingGameObject {
protected:
    long ForceOpenCount;
    bool OverrideAutomaticLocking;
public:
#ifdef FABLE_RETAIL_CTHING_CONSTRUCTORS
    CThingBuilding();
#endif
    static long __fastcall StatGetType() { return 3; }
    bool IsPointInside(const C3DVector&) const;
};
FABLE_STATIC_ASSERT(sizeof(CThingBuilding)==0xd8);
#endif

#ifdef FABLE_RETAIL_CTCPHYSICSBASE
#ifndef FABLE_ENGINE_CTCPHYSICSBASE_H
#define FABLE_ENGINE_CTCPHYSICSBASE_H
class CPhysicsMeshObject;

#ifndef FABLE_ENGINE_CTCBASE_H
#define FABLE_ENGINE_CTCBASE_H
















#ifdef FABLE_RETAIL_CTCBASE
#include <stddef.h>

#ifndef FABLE_ENGINE_CBASECLASSNONCOPYABLE_H
#define FABLE_ENGINE_CBASECLASSNONCOPYABLE_H






class CBaseClassNonCopyable : public CBaseClass {
protected:
    CBaseClassNonCopyable();
    virtual ~CBaseClassNonCopyable();                                    
private:
    CBaseClassNonCopyable(const CBaseClassNonCopyable&);
    CBaseClassNonCopyable& operator=(const CBaseClassNonCopyable&);
};
FABLE_STATIC_ASSERT(sizeof(CBaseClassNonCopyable) == 0x4);

#endif 




class CEnginePrimitivePrepareInfo;
class CInterfaceType;
class CPersistContext;
class CTCParamsBase;
namespace NTCMessages { class CMessageBase; }
class CTCBase : public CBaseClassNonCopyable {
public:
#ifdef FABLE_RETAIL_CTHING_CONSTRUCTORS
    CTCBase(CThing&);
#endif
protected:
    virtual void OnSerialise(CPersistContext &,bool);
public:
    virtual ~CTCBase();
public:
    virtual void OnRemove();
public:
    virtual void ConstructFromParams(const CTCParamsBase &);
public:
    virtual void OnCreate();
public:
    virtual void InitialActivate();
public:
    virtual void OnInitialActivate();
public:
    virtual void RenderUpdate(float);
public:
    virtual bool HasFrameUpdate() const;
public:
    virtual void FrameUpdate();
public:
    virtual void BulletTimeFrameUpdate();
public:
    virtual void DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo &);
public:
    virtual void Copy(const CTCBase &);
public:
    virtual void OnDie();
public:
    virtual void OnKill();
public:
    virtual void OnLevelUnload();
public:
    virtual void OnSetInLimbo(bool);
public:
    virtual bool Validate(CCharString &) const;
public:
    virtual bool IsFreeForDeletion() const;
public:
    virtual bool IsSerialiseCreatable() const;
public:
    virtual bool IsSerialisableForHeroState() const;
public:
    virtual ETCInterfaceType GetInterfaceType() const=0;
public:
    virtual CCharString GetClassTypeName() const=0;
public:
    virtual bool IsComponentSerialisable() const;
public:
    virtual bool IsDriver() const;
public:
    virtual bool IsCopyable() const;
public:
    virtual void GlobalUpdate(bool);
public:
    virtual void HandleMessage(const NTCMessages::CMessageBase &);
protected:
    CThing& ParentThing;
private:
    bool ToBeRemoved;
    bool IsInGlobalUpdateTCs;
};
FABLE_STATIC_ASSERT(sizeof(CTCBase)==12);
#else 
#include <stddef.h>


class CThing;

#pragma pack(push, 1)
class CTCBase {
public:
    void*         __vftable;                                 
    CThing*       ParentThing;                               
    bool          ToBeRemoved;                               
    bool          IsInGlobalUpdateTCs;                       
    unsigned char _pad_0x0a[0x2];                            
};
#pragma pack(pop)

FABLE_STATIC_ASSERT(sizeof(CTCBase) == 0xc);
FABLE_STATIC_ASSERT(offsetof(CTCBase, __vftable) == 0x0);
FABLE_STATIC_ASSERT(offsetof(CTCBase, ParentThing) == 0x4);
FABLE_STATIC_ASSERT(offsetof(CTCBase, ToBeRemoved) == 0x8);
FABLE_STATIC_ASSERT(offsetof(CTCBase, IsInGlobalUpdateTCs) == 0x9);

#endif 

#endif 




class C2DVector;
class CInterfaceType;
class CPersistContext;
class CPhysicsCollideResponse;
class CPhysicsDebugInfo;
class CRightHandedSet;
class CTCParamsBase;
class CThingPhysicalDebugInfo;
class CThingPhysicsMeshInfo;
class CTCPhysicsBase : public CTCBase {
#ifdef FABLE_AUTOPLACE_TEST_ACCESS
    friend class CAutoPlaceFixture;
#endif
public:
#ifdef FABLE_RETAIL_CTHING_CONSTRUCTORS
    CTCPhysicsBase(CThing&);
#endif
    virtual ETCInterfaceType GetInterfaceType() const;
protected:
    virtual C2DVector GetOffsetVector() const;
protected:
    virtual C3DVector GetOffsetVector3D() const;
public:
    virtual void SetPosition(const C3DVector &);
public:
    virtual void ResetPosition(const C3DVector &);
public:
    virtual void ResetRHSet(const CRightHandedSet &);
public:
    virtual void SetPositionToNavigateTo(const C3DVector &);
public:
    virtual void SetForcedToMovePos(const C3DVector &,bool);
public:
    virtual void ResetMovementState();
public:
    virtual void EnablePhysics(bool);
public:
    virtual bool IsPhysicsEnabled() const;
public:
    virtual void EnableGravity(bool);
public:
    virtual bool IsGravityEnabled() const;
public:
    virtual void EnableNavigator(bool);
public:
    virtual bool IsNavigationEnabled() const;
public:
    virtual bool HasNavigationLayers() const;
public:
    virtual void SetInteractionFlags(long);
public:
    virtual void ReturnToDefaultInteractionFlags();
public:
    virtual void OnDroppedFromUpdateList();
public:
    virtual void OnAddedToUpdateList();
public:
    virtual bool IsAlwaysFacingMovementDirection() const;
public:
    virtual bool IsAffectedByGravity() const;
public:
    virtual bool IsAffectedByAir() const;
public:
    virtual bool IsAffectedByWind() const;
public:
    virtual bool IsAngledWithGround() const;
public:
    virtual bool IsImmoveable() const;
public:
    virtual bool IsOnAnySurface() const;
public:
    virtual bool IsOnAnyStationarySurface() const;
public:
    virtual bool IsOnMeshSurface() const;
public:
    virtual bool IsOnGroundSurface() const;
public:
    virtual bool CanCollideWithThings() const;
public:
    virtual bool CanCollideWithThingsOneWay() const;
public:
    virtual bool CanCollideWithLandscape() const;
public:
    virtual bool CanInteractWithThings() const;
public:
    virtual bool IsExcludingCreatures() const;
public:
    virtual bool IsPhysicsMeshImpassable() const;
public:
    virtual float GetFacingAngleXY() const;
public:
    virtual float GetFacingAngleYZ() const;
public:
    virtual float GetFacingAngleXZ() const;
public:
    virtual void SetFacingAngleXY(float);
public:
    virtual void SetFacingAngleYZ(float);
public:
    virtual void SetFacingAngleXZ(float);
public:
    virtual C3DVector GetFacingVector(float) const;
public:
    virtual C3DVector GetFacingVector() const;
public:
    virtual const CRightHandedSet & GetRHSet() const;
public:
    virtual CRightHandedSet GetRHSet(float) const;
public:
    virtual const CRightHandedSet & GetLastRHSet() const;
public:
    virtual void SetFacingAnglesFromRHSet(const CRightHandedSet &);
public:
    virtual void SetRHSet(const CRightHandedSet &);
public:
    virtual C3DVector GetImpulse() const;
public:
    virtual void SetRHSetAsFlushOnNormal(const C3DVector &);
public:
    virtual void SetRHSetAsFlushOnGround();
public:
    virtual float GetRenderZOffset(float) const;
public:
    virtual void ResetInterpolation();
public:
    virtual void ApplyControlledImpulse(const C3DVector &);
public:
    virtual void ApplyControlledAcceleration(const C3DVector &,float);
public:
    virtual bool IsPushableByHero() const;
public:
    virtual void SetAsPushableByHero(bool);
public:
    virtual void FrameUpdate();
public:
    virtual bool HasFrameUpdate() const;
public:
    virtual void SetPhysicsMeshBankIndex(long);
public:
    virtual void SetScaleFromOwner();
public:
    virtual void ConstructFromParams(const CTCParamsBase &);
public:
    virtual void OnCreate();
public:
    virtual void InitialActivate();
public:
    virtual void Move();
public:
    virtual bool DrawGetPhysicsDebugInfo(CThingPhysicalDebugInfo &) const;
public:
    virtual void OnSerialise(CPersistContext &,bool);
public:
    virtual bool IsComponentSerialisable() const;
public:
    virtual bool IsSerialisableForHeroState() const;
public:
    virtual bool IsCopyable() const;
public:
    virtual void FillPhysicsDebugInfoForSelf(CPhysicsDebugInfo &) const;
public:
    virtual bool HasCollidedWithThingFromTo(const CTCPhysicsBase &,const C3DVector &,const C3DVector &,CPhysicsCollideResponse &) const;
public:
    virtual bool HasCollidedWithThingAt(const CTCPhysicsBase &,const C3DVector &,CPhysicsCollideResponse &) const;
public:
    virtual bool HasCollidedWithLandscapeAt(const C3DVector &,CPhysicsCollideResponse &) const;
public:
    virtual const CThingPhysicsMeshInfo * GetPhysicsMeshInfo() const;
protected:
    virtual void UpdateInterpolation();
protected:
    virtual void UpdateOldRHSet();
protected:
    virtual void UpdateOldPos();
public:
    virtual ~CTCPhysicsBase();
protected:
    C3DVector Position;
    C3DVector OldPosition;
    long OldPositionLastFrameSet;
    C3DVector Velocity;
    float Radius;
    unsigned char PhysicsDef[4];
    bool Moving:1,OldMoving:1;
public:
    void GetBoundingInfo(C3DVector&,float&) const;
    const C3DVector& GetPosition() const { return Position; }
    float GetRadius() const { return Radius; }
    CPhysicsMeshObject GetPhysicsMeshObject() const;
};
FABLE_STATIC_ASSERT(sizeof(CTCPhysicsBase)==0x40);
#endif
#else
#ifndef FABLE_ENGINE_CTCPHYSICSBASE_H
#define FABLE_ENGINE_CTCPHYSICSBASE_H





#include <stddef.h>


class CDefPointeeBase;

#pragma pack(push, 1)
class CTCPhysicsBase {
public:
    void*            __vftable;                              
    long             sub;                                    
    unsigned char    _pad_0x08[0x4];                         
    float            Position_X;                             
    float            Position_Y;                             
    float            Position_Z;                             
    float            OldPosition_X;                          
    float            OldPosition_Y;                          
    float            OldPosition_Z;                          
    long             OldPositionLastFrameSet;                
    float            Velocity_X;                             
    float            Velocity_Y;                             
    float            Velocity_Z;                             
    float            Radius;                                 
    CDefPointeeBase* PhysicsDef_Object;                      
    unsigned char    OldMoving[0x4];                         
};
#pragma pack(pop)

FABLE_STATIC_ASSERT(sizeof(CTCPhysicsBase) == 0x40);
FABLE_STATIC_ASSERT(offsetof(CTCPhysicsBase, __vftable) == 0x0);
FABLE_STATIC_ASSERT(offsetof(CTCPhysicsBase, sub) == 0x4);
FABLE_STATIC_ASSERT(offsetof(CTCPhysicsBase, Position_X) == 0xc);
FABLE_STATIC_ASSERT(offsetof(CTCPhysicsBase, Position_Y) == 0x10);
FABLE_STATIC_ASSERT(offsetof(CTCPhysicsBase, Position_Z) == 0x14);
FABLE_STATIC_ASSERT(offsetof(CTCPhysicsBase, OldPosition_X) == 0x18);
FABLE_STATIC_ASSERT(offsetof(CTCPhysicsBase, OldPosition_Y) == 0x1c);
FABLE_STATIC_ASSERT(offsetof(CTCPhysicsBase, OldPosition_Z) == 0x20);
FABLE_STATIC_ASSERT(offsetof(CTCPhysicsBase, OldPositionLastFrameSet) == 0x24);
FABLE_STATIC_ASSERT(offsetof(CTCPhysicsBase, Velocity_X) == 0x28);
FABLE_STATIC_ASSERT(offsetof(CTCPhysicsBase, Velocity_Y) == 0x2c);
FABLE_STATIC_ASSERT(offsetof(CTCPhysicsBase, Velocity_Z) == 0x30);
FABLE_STATIC_ASSERT(offsetof(CTCPhysicsBase, Radius) == 0x34);
FABLE_STATIC_ASSERT(offsetof(CTCPhysicsBase, PhysicsDef_Object) == 0x38);
FABLE_STATIC_ASSERT(offsetof(CTCPhysicsBase, OldMoving) == 0x3c);

#endif 

#endif

#ifdef FABLE_RETAIL_CTCCREATURENAVIGATION
#ifndef FABLE_ENGINE_CTCCREATURENAVIGATION_H
#define FABLE_ENGINE_CTCCREATURENAVIGATION_H

class CNavigatorBase;
class CTCCreatureNavigation : public CTCBase {
private:
    CNavigatorBase* PNavigator;                              
    unsigned char   PossibleNavigators[0xc];                 
    unsigned char   NavigationTarget[0x18];                  
    float           Speed;                                   
    unsigned char   PThingToNotAvoid[0x8];                   
    unsigned char   Agents[0xc];                             
    long            NextUpdate;                              
    long            FrameLastNavigated;                      
    bool            CalculatingNewRoute;                     
    bool            CanMoveThroughSolidObjects;              
    bool            GotNavigationTarget;                     
    unsigned char   _pad_0x57[0x1];                          
    float           ProximityForDirectApproach;              
    float           CurrentSpeed;                            
public:
#ifdef FABLE_RETAIL_CTHING_CONSTRUCTORS
    CTCCreatureNavigation(CThing&);
#endif
    void InterruptNavigation();
    bool GetNearbyWanderPosition(const C3DVector&,float,C3DVector&,bool) const;
    bool GetNearestNavigablePosTo(const C3DVector&,float,C3DVector&) const;
};
FABLE_STATIC_ASSERT(sizeof(CTCCreatureNavigation)==0x60);
#endif
#else
#ifndef FABLE_ENGINE_CTCCREATURENAVIGATION_H
#define FABLE_ENGINE_CTCCREATURENAVIGATION_H






#include <stddef.h>


class CNavigatorBase;

#pragma pack(push, 1)
class CTCCreatureNavigation {
public:
    void*           __vftable;                               
    unsigned char   _pad_0x04[0x4];                          
    unsigned char   _pad_0x08[0x4];                          
    CNavigatorBase* PNavigator;                              
    unsigned char   PossibleNavigators[0xc];                 
    unsigned char   NavigationTarget[0x18];                  
    float           Speed;                                   
    unsigned char   PThingToNotAvoid[0x8];                   
    unsigned char   Agents[0xc];                             
    long            NextUpdate;                              
    long            FrameLastNavigated;                      
    bool            CalculatingNewRoute;                     
    bool            CanMoveThroughSolidObjects;              
    bool            GotNavigationTarget;                     
    unsigned char   _pad_0x57[0x1];                          
    float           ProximityForDirectApproach;              
    float           CurrentSpeed;                            
};
#pragma pack(pop)

FABLE_STATIC_ASSERT(sizeof(CTCCreatureNavigation) == 0x60);
FABLE_STATIC_ASSERT(offsetof(CTCCreatureNavigation, __vftable) == 0x0);
FABLE_STATIC_ASSERT(offsetof(CTCCreatureNavigation, PNavigator) == 0xc);
FABLE_STATIC_ASSERT(offsetof(CTCCreatureNavigation, PossibleNavigators) == 0x10);
FABLE_STATIC_ASSERT(offsetof(CTCCreatureNavigation, NavigationTarget) == 0x1c);
FABLE_STATIC_ASSERT(offsetof(CTCCreatureNavigation, Speed) == 0x34);
FABLE_STATIC_ASSERT(offsetof(CTCCreatureNavigation, PThingToNotAvoid) == 0x38);
FABLE_STATIC_ASSERT(offsetof(CTCCreatureNavigation, Agents) == 0x40);
FABLE_STATIC_ASSERT(offsetof(CTCCreatureNavigation, NextUpdate) == 0x4c);
FABLE_STATIC_ASSERT(offsetof(CTCCreatureNavigation, FrameLastNavigated) == 0x50);
FABLE_STATIC_ASSERT(offsetof(CTCCreatureNavigation, CalculatingNewRoute) == 0x54);
FABLE_STATIC_ASSERT(offsetof(CTCCreatureNavigation, CanMoveThroughSolidObjects) == 0x55);
FABLE_STATIC_ASSERT(offsetof(CTCCreatureNavigation, GotNavigationTarget) == 0x56);
FABLE_STATIC_ASSERT(offsetof(CTCCreatureNavigation, ProximityForDirectApproach) == 0x58);
FABLE_STATIC_ASSERT(offsetof(CTCCreatureNavigation, CurrentSpeed) == 0x5c);

#endif 

#endif

#ifndef FABLE_ENGINE_CTCTHINGOWNER_H
#define FABLE_ENGINE_CTCTHINGOWNER_H





class CTCThingOwner : public CTCBase {
private:
    std::list<CIntelligentPointer<CThing> > ObjectList;
    bool CallOnDie;
public:
#ifdef FABLE_RETAIL_CTHING_CONSTRUCTORS
    CTCThingOwner(CThing&);
#endif
    const std::list<CIntelligentPointer<CThing> >& GetObjectList() const { return ObjectList; }
    std::list<CIntelligentPointer<CThing> >& GetObjectList() { return ObjectList; }
};
FABLE_STATIC_ASSERT(sizeof(CTCThingOwner)==0x14);
#endif

#ifndef FABLE_ENGINE_CPHYSICSMESHOBJECT_H
#define FABLE_ENGINE_CPHYSICSMESHOBJECT_H

class CCharString;


class CPhysicsMeshObject : public CBaseClass {
private:
    unsigned char Mesh[4]; 
    unsigned char PositionMatrix[48]; 
    float Scale;
    float OO_Scale;
public:
#ifdef FABLE_RETAIL_CTHING_CONSTRUCTORS
    CPhysicsMeshObject();
#endif
    virtual ~CPhysicsMeshObject();
    bool HasContainmentVolume(const CCharString&) const;
};
FABLE_STATIC_ASSERT(sizeof(CPhysicsMeshObject)==0x40);
#endif

#ifndef FABLE_ENGINE_CTHINGSEARCHTOOLS_H
#define FABLE_ENGINE_CTHINGSEARCHTOOLS_H






#include <stddef.h>


class CThingManager;
struct CThingMapManager;
class CWorldMap;

#pragma pack(push, 1)
class CThingSearchTools {
public:
#ifdef FABLE_RETAIL_CTHINGSEARCHTOOLS
#ifdef FABLE_RETAIL_VISIBLE_THINGSEARCH
    template<class Filter,class T> __declspec(noinline) bool GetPFirstLeafThing(const Filter& filter,T** out) const {
        const std::list<CClass*>& things=PeekTypeList(T::StatGetType());
        for(std::list<CClass*>::const_iterator it=things.begin();it!=things.end();++it) {
            T* thing=static_cast<T*>(*it);
            if(thing->IsIncludedInThingScans() && filter(*thing)) {
                *out=thing;
                return true;
            }
        }
        return false;
    }
#else
    template<class Filter,class T> bool GetPFirstLeafThing(const Filter&,T**) const;
#endif
protected:
    const std::list<CClass*>& PeekTypeList(long) const;
public:
#endif
    CWorldMap*        WorldMap;                              
    CThingMapManager* PThingMapManager;                      
    CThingManager*    ThingManager;                          
    unsigned char     AreaMaps[0xc];                         
};
#pragma pack(pop)

FABLE_STATIC_ASSERT(sizeof(CThingSearchTools) == 0x18);
FABLE_STATIC_ASSERT(offsetof(CThingSearchTools, WorldMap) == 0x0);
FABLE_STATIC_ASSERT(offsetof(CThingSearchTools, PThingMapManager) == 0x4);
FABLE_STATIC_ASSERT(offsetof(CThingSearchTools, ThingManager) == 0x8);
FABLE_STATIC_ASSERT(offsetof(CThingSearchTools, AreaMaps) == 0xc);

#endif 

#ifndef FABLE_ENGINE_CTHINGFILTER_OVERLAPPINGSPHERE_H
#define FABLE_ENGINE_CTHINGFILTER_OVERLAPPINGSPHERE_H


class CThing;


class CThingFilter_OverlappingSphere : public std::unary_function<CThing,bool> {
private:
    C3DVector SpherePos;
    float Radius;
public:
    CThingFilter_OverlappingSphere(const C3DVector& p,float r):SpherePos(p),Radius(r){}
#ifdef FABLE_RETAIL_VISIBLE_THINGSEARCH
    bool operator()(const CThing& test_thing) const {
        float radius;
        C3DVector test_pos;
        test_thing.GetPhysicsTC()->GetBoundingInfo(test_pos,radius);
        if ((test_pos-SpherePos).GetSquaredMagnitude() <
            (radius+Radius)*(radius+Radius))
            return true;
        return false;
    }
#else
    bool operator()(const CThing&) const;
#endif
};
#endif

#ifndef FABLE_ENGINE_CTHINGFILTER_POSINCONTAINMENTVOLUME_H
#define FABLE_ENGINE_CTHINGFILTER_POSINCONTAINMENTVOLUME_H



class CThing;

class CThingFilter_PosInContainmentVolume : public std::unary_function<CThing,bool> {
private:
    C3DVector Pos;
    CCharString VolumeName;
public:
    CThingFilter_PosInContainmentVolume(const C3DVector& p,const CCharString& n):Pos(p),VolumeName(n){}
    bool operator()(const CThing&) const;
};
#endif

#ifndef FABLE_ENGINE_CFILTER_BIND2_H
#define FABLE_ENGINE_CFILTER_BIND2_H

template<class First,class Second> class CFilter_Bind2 {
protected:
    const First& F1;
    const Second& F2;
public:
    CFilter_Bind2(const First& a,const Second& b):F1(a),F2(b){}
    template<class T> bool operator()(const T& thing) const { return F1(thing) && F2(thing); }
};
template<class First,class Second>
CFilter_Bind2<First,Second> __fastcall GFFilter_Bind2(const First& a,const Second& b) {
    return CFilter_Bind2<First,Second>(a,b);
}
#endif

#ifdef FABLE_RETAIL_CWORLD
#ifndef FABLE_ENGINE_CWORLD_H
#define FABLE_ENGINE_CWORLD_H

class CThingSearchTools; class CProceduralTexture; class CIDrawEnvironment; class CIDrawWorldMap;

class CIDrawWorld {
public:
    virtual const CProceduralTexture* DrawGetProceduralTexture() const=0;
    virtual const CIDrawEnvironment* DrawGetEnvironment() const=0;
    virtual const CIDrawWorldMap* DrawGetWorldMap() const=0;
    virtual ~CIDrawWorld();
};
namespace boost {
template<class T> class scoped_ptr {
private:
    T* ptr;
public:
    T* get() const { return ptr; }
    T& operator*() const { return *ptr; }
};
}
class CMainGameComponent; class CPlayerManager; class CGameDefinitionManager;
class CWorldMap; class CEnvironment; class CGameTimeManager;


class CWorld : public CBaseClassNonCopyable, public CIDrawWorld {
protected:
    CMainGameComponent& Component;
    CPlayerManager& PlayerManager;
    const CGameDefinitionManager& DefinitionManager;
    boost::scoped_ptr<CWorldMap> PWorldMap;
    boost::scoped_ptr<CEnvironment> PEnvironment;
    boost::scoped_ptr<CGameTimeManager> PGameTimeManager;
    boost::scoped_ptr<CThingSearchTools> PThingSearchTools;
    unsigned char _pad_0x24[0x174];
public:
    CThingSearchTools& GetThingSearchTools() { return *PThingSearchTools; }
};
FABLE_STATIC_ASSERT(sizeof(CWorld)==0x198);
#endif
#else
#ifndef FABLE_ENGINE_CWORLD_H
#define FABLE_ENGINE_CWORLD_H






#include <stddef.h>


class C3DAnimationManager;
class CAIGameCameraBase;
class CAtmosProcess;
class CBulletTimeManager;
class CCPPointerInfo;
class CCharStringData;
class CCombatManager;
class CEnvironment;
class CFactionManager;
class CGameDefinitionManager;
class CGameScriptInterface;
class CGameTimeManager;
class CMainGameComponent;
class CMeshDataBank;
class CMessageEventManager;
class CMusicManager;
class CNavigatorManager;
class COpinionReactionManager;
class CPlayerManager;
class CScriptConversationManager;
class CScriptInfoManager;
class CThingManager;
class CThingSearchTools;
class CWideStringData;
class CWorldMap;

#pragma pack(push, 1)
class CWorld {
public:
    void*                       __vftable;                   
    unsigned char               _base_0x04[0x4];             
    CMainGameComponent*         Component;                   
    CPlayerManager*             PlayerManager;               
    CGameDefinitionManager*     DefinitionManager;           
    CWorldMap*                  PWorldMap_ptr;               
    CEnvironment*               PEnvironment_ptr;            
    CGameTimeManager*           PGameTimeManager_ptr;        
    CThingSearchTools*          PThingSearchTools_ptr;       
    CAtmosProcess*              PAtmosProcessor_Data;        
    CCPPointerInfo*             PAtmosProcessor_Info;        
    CAIGameCameraBase*          PGameCamera_ptr;             
    CAIGameCameraBase*          PGameCameraManager_ptr;      
    CAIGameCameraBase*          PCurrentGameCamera;          
    CGameScriptInterface*       PGameScriptInterface_ptr;    
    CMeshDataBank*              PMainMeshBank_Data;          
    CCPPointerInfo*             PMainMeshBank_Info;          
    C3DAnimationManager*        PAnimationManager;           
    CNavigatorManager*          PNavigationManager_ptr;      
    CCombatManager*             PThingCombatManager_ptr;     
    CThingManager*              PThingManager_ptr;           
    CFactionManager*            PFactionManager_ptr;         
    CScriptInfoManager*         PScriptInfoManager_Data;     
    CCPPointerInfo*             PScriptInfoManager_Info;     
    CMessageEventManager*       PMessageEventManager_Data;   
    CCPPointerInfo*             PMessageEventManager_Info;   
    CBulletTimeManager*         PBulletTimeManager_ptr;      
    CMusicManager*              PMusicManager_Data;          
    CCPPointerInfo*             PMusicManager_Info;          
    COpinionReactionManager*    POpinionReactionManager_Data; 
    CCPPointerInfo*             POpinionReactionManager_Info; 
    CScriptConversationManager* PScriptConversationManager_ptr; 
    bool                        JustLoaded;                  
    unsigned char               _pad_0x81[0x3];              
    CCharStringData*            CurrentWorldName_PStringData; 
    long                        ConsolePauseAtFrameNumber;   
    long                        FrameStarted3DRendering;     
    double                      LastUpdateTimeLength;        
    double                      LastUpdateTime;              
    long                        CountdownTimer;              
    bool                        Paused;                      
    bool                        SlowMotion;                  
    bool                        ShowDebugText;               
    bool                        ShowFPSText;                 
    bool                        ShowProfileText;             
    unsigned char               _pad_0xa9[0x3];              
    unsigned char               InitialActiveQuests[0xc];    
    unsigned char               RegisteredQuests[0xc];       
    unsigned char               ActiveTestQuests[0xc];       
    unsigned long               CreatureGenerationDisabledGroups; 
    bool                        CreatureGenerationEnabled;   
    bool                        TeleportingEnabled;          
    bool                        ExperienceSpendingEnabled;   
    bool                        SavingEnabled;               
    bool                        DontPopulateNextLoadedRegion; 
    bool                        HeroSleepingEnabled;         
    bool                        MapTableShowQuestCardsOnUsed; 
    bool                        ScreenToFadeInOnNextRegionChange; 
    bool                        DoneExtraFrameUpdateBeforeRegionLoadScreenFadeIn; 
    bool                        MiniMapEnabled;              
    bool                        MiniMapActiveBeforeDisabled; 
    bool                        RegionLoadDisplayRegionName; 
    bool                        GuildMasterMessagesEnabled;  
    bool                        SummonerDeathExplosionAffectsHero; 
    bool                        WaitingForInventoryTutorialToFinish; 
    unsigned char               _pad_0xe3[0x1];              
    unsigned char               HeroInformationScreenModeAfterTutorial[0x4]; 
    long                        FrameCachedLODCentre;        
    float                       CachedLODCentre_X;           
    float                       CachedLODCentre_Y;           
    float                       CachedLODCentre_Z;           
    unsigned char               SaveGameLoadStatus[0x4];     
    CWideStringData*            SaveGamePathName_PStringData; 
    bool                        AutoSaveLocked;              
    bool                        SerialisingAboutToLoadHeroState; 
    bool                        SerialisingHeroState;        
    bool                        SerialisingNonPersistentQuestItems; 
    unsigned char               RegionLoadStatus[0x4];       
    float                       RegionLoadStartPos_X;        
    float                       RegionLoadStartPos_Y;        
    float                       RegionLoadStartPos_Z;        
    float                       RegionLoadStartAngleXY;      
    unsigned char               RegionLoadFollowers[0xc];    
    long                        PreviousRegion;              
    long                        NumberOfTimesFreezeControlsModeAddedDuringRegionLoad; 
    bool                        RegionLoadViaTeleporter;     
    bool                        RegionLoadViaDoor;           
    bool                        PutIntoPauseModeOnRegionChange; 
    bool                        DisplayMiniMapOnRegionLoad;  
    bool                        RegionLoadWaitingForConfirmation; 
    bool                        RegionLoadScreenFullyFaded;  
    bool                        RegionLoadScreenWasFadedOut; 
    bool                        WaitingForResetToFrontEndConfirmation; 
    unsigned char               MostRecentSaveType[0x4];     
    unsigned char               MostRecentSaveTypeBeforeManualSave[0x4]; 
    CWideStringData*            MostRecentManualSaveName_PStringData; 
    bool                        AutoSaveCheckPointExists;    
    unsigned char               SaveGameMarkerPos[0xf];      
    float                       SaveGameMarkerAngleXY;       
    float                       GuildSealRecallPos_X;        
    float                       GuildSealRecallPos_Y;        
    float                       GuildSealRecallPos_Z;        
    float                       GuildSealRecallAngleXY;      
    bool                        WeatherMaskingPrimitivesSent; 
    unsigned char               _pad_0x165[0x3];             
    unsigned char               WeatherMaskingPrimitiveHandles[0xc];  
    unsigned char               AtmosBanksWaitingToCopy[0x4]; 
    unsigned char               PlayerSpawnStatus[0x4];      
    long                        VillagerReactionDebug;       
    double                      StartTime;                   
    double                      TimePlayed;                  
    bool                        HasInitialisedStartTime;     
    unsigned char               _pad_0x191[0x7];             
    static long __fastcall GetFrame();                       
    unsigned long* GetWorldSeed() const;                     
};
#pragma pack(pop)

FABLE_STATIC_ASSERT(sizeof(CWorld) == 0x198);
FABLE_STATIC_ASSERT(offsetof(CWorld, __vftable) == 0x0);
FABLE_STATIC_ASSERT(offsetof(CWorld, Component) == 0x8);
FABLE_STATIC_ASSERT(offsetof(CWorld, PlayerManager) == 0xc);
FABLE_STATIC_ASSERT(offsetof(CWorld, DefinitionManager) == 0x10);
FABLE_STATIC_ASSERT(offsetof(CWorld, PWorldMap_ptr) == 0x14);
FABLE_STATIC_ASSERT(offsetof(CWorld, PEnvironment_ptr) == 0x18);
FABLE_STATIC_ASSERT(offsetof(CWorld, PGameTimeManager_ptr) == 0x1c);
FABLE_STATIC_ASSERT(offsetof(CWorld, PThingSearchTools_ptr) == 0x20);
FABLE_STATIC_ASSERT(offsetof(CWorld, PAtmosProcessor_Data) == 0x24);
FABLE_STATIC_ASSERT(offsetof(CWorld, PAtmosProcessor_Info) == 0x28);
FABLE_STATIC_ASSERT(offsetof(CWorld, PGameCamera_ptr) == 0x2c);
FABLE_STATIC_ASSERT(offsetof(CWorld, PGameCameraManager_ptr) == 0x30);
FABLE_STATIC_ASSERT(offsetof(CWorld, PCurrentGameCamera) == 0x34);
FABLE_STATIC_ASSERT(offsetof(CWorld, PGameScriptInterface_ptr) == 0x38);
FABLE_STATIC_ASSERT(offsetof(CWorld, PMainMeshBank_Data) == 0x3c);
FABLE_STATIC_ASSERT(offsetof(CWorld, PMainMeshBank_Info) == 0x40);
FABLE_STATIC_ASSERT(offsetof(CWorld, PAnimationManager) == 0x44);
FABLE_STATIC_ASSERT(offsetof(CWorld, PNavigationManager_ptr) == 0x48);
FABLE_STATIC_ASSERT(offsetof(CWorld, PThingCombatManager_ptr) == 0x4c);
FABLE_STATIC_ASSERT(offsetof(CWorld, PThingManager_ptr) == 0x50);
FABLE_STATIC_ASSERT(offsetof(CWorld, PFactionManager_ptr) == 0x54);
FABLE_STATIC_ASSERT(offsetof(CWorld, PScriptInfoManager_Data) == 0x58);
FABLE_STATIC_ASSERT(offsetof(CWorld, PScriptInfoManager_Info) == 0x5c);
FABLE_STATIC_ASSERT(offsetof(CWorld, PMessageEventManager_Data) == 0x60);
FABLE_STATIC_ASSERT(offsetof(CWorld, PMessageEventManager_Info) == 0x64);
FABLE_STATIC_ASSERT(offsetof(CWorld, PBulletTimeManager_ptr) == 0x68);
FABLE_STATIC_ASSERT(offsetof(CWorld, PMusicManager_Data) == 0x6c);
FABLE_STATIC_ASSERT(offsetof(CWorld, PMusicManager_Info) == 0x70);
FABLE_STATIC_ASSERT(offsetof(CWorld, POpinionReactionManager_Data) == 0x74);
FABLE_STATIC_ASSERT(offsetof(CWorld, POpinionReactionManager_Info) == 0x78);
FABLE_STATIC_ASSERT(offsetof(CWorld, PScriptConversationManager_ptr) == 0x7c);
FABLE_STATIC_ASSERT(offsetof(CWorld, JustLoaded) == 0x80);
FABLE_STATIC_ASSERT(offsetof(CWorld, CurrentWorldName_PStringData) == 0x84);
FABLE_STATIC_ASSERT(offsetof(CWorld, ConsolePauseAtFrameNumber) == 0x88);
FABLE_STATIC_ASSERT(offsetof(CWorld, FrameStarted3DRendering) == 0x8c);
FABLE_STATIC_ASSERT(offsetof(CWorld, LastUpdateTimeLength) == 0x90);
FABLE_STATIC_ASSERT(offsetof(CWorld, LastUpdateTime) == 0x98);
FABLE_STATIC_ASSERT(offsetof(CWorld, CountdownTimer) == 0xa0);
FABLE_STATIC_ASSERT(offsetof(CWorld, Paused) == 0xa4);
FABLE_STATIC_ASSERT(offsetof(CWorld, SlowMotion) == 0xa5);
FABLE_STATIC_ASSERT(offsetof(CWorld, ShowDebugText) == 0xa6);
FABLE_STATIC_ASSERT(offsetof(CWorld, ShowFPSText) == 0xa7);
FABLE_STATIC_ASSERT(offsetof(CWorld, ShowProfileText) == 0xa8);
FABLE_STATIC_ASSERT(offsetof(CWorld, InitialActiveQuests) == 0xac);
FABLE_STATIC_ASSERT(offsetof(CWorld, RegisteredQuests) == 0xb8);
FABLE_STATIC_ASSERT(offsetof(CWorld, ActiveTestQuests) == 0xc4);
FABLE_STATIC_ASSERT(offsetof(CWorld, CreatureGenerationDisabledGroups) == 0xd0);
FABLE_STATIC_ASSERT(offsetof(CWorld, CreatureGenerationEnabled) == 0xd4);
FABLE_STATIC_ASSERT(offsetof(CWorld, TeleportingEnabled) == 0xd5);
FABLE_STATIC_ASSERT(offsetof(CWorld, ExperienceSpendingEnabled) == 0xd6);
FABLE_STATIC_ASSERT(offsetof(CWorld, SavingEnabled) == 0xd7);
FABLE_STATIC_ASSERT(offsetof(CWorld, DontPopulateNextLoadedRegion) == 0xd8);
FABLE_STATIC_ASSERT(offsetof(CWorld, HeroSleepingEnabled) == 0xd9);
FABLE_STATIC_ASSERT(offsetof(CWorld, MapTableShowQuestCardsOnUsed) == 0xda);
FABLE_STATIC_ASSERT(offsetof(CWorld, ScreenToFadeInOnNextRegionChange) == 0xdb);
FABLE_STATIC_ASSERT(offsetof(CWorld, DoneExtraFrameUpdateBeforeRegionLoadScreenFadeIn) == 0xdc);
FABLE_STATIC_ASSERT(offsetof(CWorld, MiniMapEnabled) == 0xdd);
FABLE_STATIC_ASSERT(offsetof(CWorld, MiniMapActiveBeforeDisabled) == 0xde);
FABLE_STATIC_ASSERT(offsetof(CWorld, RegionLoadDisplayRegionName) == 0xdf);
FABLE_STATIC_ASSERT(offsetof(CWorld, GuildMasterMessagesEnabled) == 0xe0);
FABLE_STATIC_ASSERT(offsetof(CWorld, SummonerDeathExplosionAffectsHero) == 0xe1);
FABLE_STATIC_ASSERT(offsetof(CWorld, WaitingForInventoryTutorialToFinish) == 0xe2);
FABLE_STATIC_ASSERT(offsetof(CWorld, HeroInformationScreenModeAfterTutorial) == 0xe4);
FABLE_STATIC_ASSERT(offsetof(CWorld, FrameCachedLODCentre) == 0xe8);
FABLE_STATIC_ASSERT(offsetof(CWorld, CachedLODCentre_X) == 0xec);
FABLE_STATIC_ASSERT(offsetof(CWorld, CachedLODCentre_Y) == 0xf0);
FABLE_STATIC_ASSERT(offsetof(CWorld, CachedLODCentre_Z) == 0xf4);
FABLE_STATIC_ASSERT(offsetof(CWorld, SaveGameLoadStatus) == 0xf8);
FABLE_STATIC_ASSERT(offsetof(CWorld, SaveGamePathName_PStringData) == 0xfc);
FABLE_STATIC_ASSERT(offsetof(CWorld, AutoSaveLocked) == 0x100);
FABLE_STATIC_ASSERT(offsetof(CWorld, SerialisingAboutToLoadHeroState) == 0x101);
FABLE_STATIC_ASSERT(offsetof(CWorld, SerialisingHeroState) == 0x102);
FABLE_STATIC_ASSERT(offsetof(CWorld, SerialisingNonPersistentQuestItems) == 0x103);
FABLE_STATIC_ASSERT(offsetof(CWorld, RegionLoadStatus) == 0x104);
FABLE_STATIC_ASSERT(offsetof(CWorld, RegionLoadStartPos_X) == 0x108);
FABLE_STATIC_ASSERT(offsetof(CWorld, RegionLoadStartPos_Y) == 0x10c);
FABLE_STATIC_ASSERT(offsetof(CWorld, RegionLoadStartPos_Z) == 0x110);
FABLE_STATIC_ASSERT(offsetof(CWorld, RegionLoadStartAngleXY) == 0x114);
FABLE_STATIC_ASSERT(offsetof(CWorld, RegionLoadFollowers) == 0x118);
FABLE_STATIC_ASSERT(offsetof(CWorld, PreviousRegion) == 0x124);
FABLE_STATIC_ASSERT(offsetof(CWorld, NumberOfTimesFreezeControlsModeAddedDuringRegionLoad) == 0x128);
FABLE_STATIC_ASSERT(offsetof(CWorld, RegionLoadViaTeleporter) == 0x12c);
FABLE_STATIC_ASSERT(offsetof(CWorld, RegionLoadViaDoor) == 0x12d);
FABLE_STATIC_ASSERT(offsetof(CWorld, PutIntoPauseModeOnRegionChange) == 0x12e);
FABLE_STATIC_ASSERT(offsetof(CWorld, DisplayMiniMapOnRegionLoad) == 0x12f);
FABLE_STATIC_ASSERT(offsetof(CWorld, RegionLoadWaitingForConfirmation) == 0x130);
FABLE_STATIC_ASSERT(offsetof(CWorld, RegionLoadScreenFullyFaded) == 0x131);
FABLE_STATIC_ASSERT(offsetof(CWorld, RegionLoadScreenWasFadedOut) == 0x132);
FABLE_STATIC_ASSERT(offsetof(CWorld, WaitingForResetToFrontEndConfirmation) == 0x133);
FABLE_STATIC_ASSERT(offsetof(CWorld, MostRecentSaveType) == 0x134);
FABLE_STATIC_ASSERT(offsetof(CWorld, MostRecentSaveTypeBeforeManualSave) == 0x138);
FABLE_STATIC_ASSERT(offsetof(CWorld, MostRecentManualSaveName_PStringData) == 0x13c);
FABLE_STATIC_ASSERT(offsetof(CWorld, AutoSaveCheckPointExists) == 0x140);
FABLE_STATIC_ASSERT(offsetof(CWorld, SaveGameMarkerPos) == 0x141);
FABLE_STATIC_ASSERT(offsetof(CWorld, SaveGameMarkerAngleXY) == 0x150);
FABLE_STATIC_ASSERT(offsetof(CWorld, GuildSealRecallPos_X) == 0x154);
FABLE_STATIC_ASSERT(offsetof(CWorld, GuildSealRecallPos_Y) == 0x158);
FABLE_STATIC_ASSERT(offsetof(CWorld, GuildSealRecallPos_Z) == 0x15c);
FABLE_STATIC_ASSERT(offsetof(CWorld, GuildSealRecallAngleXY) == 0x160);
FABLE_STATIC_ASSERT(offsetof(CWorld, WeatherMaskingPrimitivesSent) == 0x164);
FABLE_STATIC_ASSERT(offsetof(CWorld, WeatherMaskingPrimitiveHandles) == 0x168);
FABLE_STATIC_ASSERT(offsetof(CWorld, AtmosBanksWaitingToCopy) == 0x174);
FABLE_STATIC_ASSERT(offsetof(CWorld, PlayerSpawnStatus) == 0x178);
FABLE_STATIC_ASSERT(offsetof(CWorld, VillagerReactionDebug) == 0x17c);
FABLE_STATIC_ASSERT(offsetof(CWorld, StartTime) == 0x180);
FABLE_STATIC_ASSERT(offsetof(CWorld, TimePlayed) == 0x188);
FABLE_STATIC_ASSERT(offsetof(CWorld, HasInitialisedStartTime) == 0x190);

#endif 

#endif

extern const CCharString BUILDING_BOUNDRY;
long __fastcall GFROR13(unsigned long);
namespace {
class RandFunc {
    unsigned long* Seed;
public:
    RandFunc(unsigned long* seed) : Seed(seed) { }
    long operator()(long range) {
        *Seed=*Seed*0x24a1UL+0x24dfUL;
        *Seed=GFROR13(*Seed);
        return range ? *Seed % range : 0;
    }
};
}

namespace std {
template<> inline void __fastcall random_shuffle<CThing**,RandFunc>(
    CThing** first,CThing** last,RandFunc& random)
{
    if(first!=last) {
        for(CThing** i=first+1;i!=last;++i) {
            CThing** j=first+random((i-first)+1);
            CThing* value=*i;
            *i=*j;
            *j=value;
        }
    }
}
}
bool CThingCreatureBase::AutoPlaceNearBuilding(const CThingBuilding* building,float distance)
{
    if (building && !building->IsDead()) {
    for (long tries=0;tries<10;++tries) {
        if (!building->GetPhysicsTC()->GetPhysicsMeshInfo())
            continue;
        bool containment=building->GetPhysicsTC()->GetPhysicsMeshObject().HasContainmentVolume(BUILDING_BOUNDRY);
        if (building->HasTC(TCI_THING_OWNER)) {
            const CTCThingOwner& owner=static_cast<const CTCThingOwner&>(building->GetTC(TCI_THING_OWNER));
            if (!owner.GetObjectList().empty()) {
                std::list<CIntelligentPointer<CThing> >::const_iterator it=owner.GetObjectList().begin();
                std::vector<CThing*> points;
                for (;it!=owner.GetObjectList().end();++it) {
                    if (it->Get() && !it->Get()->IsDead() && it->Get()->GetType()==5 &&
                        !it->Get()->HasTC(TCI_SWITCHABLE_NAVIGATION))
                    {
                        points.push_back(it->Get());
                    }
                }
                {
                    RandFunc random(GetPWorldSeed());
                    std::random_shuffle(points.begin(),points.end(),random);
                }
                {
                unsigned int count=points.size();
                for (unsigned int index=0;index<count;++index) {
                    CTCCreatureNavigation* navigation;
                    if (HasTC(TCI_NAVIGATION))
                        navigation=&static_cast<CTCCreatureNavigation&>(GetTC(TCI_NAVIGATION));
                    C3DVector position=points[index]->GetPos();
                    C3DVector original=position;
                    CThingBuilding* found=0;
                    GetWorld().GetThingSearchTools().GetPFirstLeafThing(
                        GFFilter_Bind2(CThingFilter_OverlappingSphere(position,0.0f),
                                      CThingFilter_PosInContainmentVolume(position,BUILDING_BOUNDRY)),&found);
                    if (found==building && navigation->GetNearestNavigablePosTo(original,distance,position) &&
                        !(original.X==position.X && original.Y==position.Y) &&
                        (!containment || building->IsPointInside(position))) {
                        navigation->InterruptNavigation();
                        PhysicsTC->ResetPosition(position);
                        navigation->InterruptNavigation();
                        return true;
                    }
                }
                }
            }
        }
        CTCCreatureNavigation* navigation=0;
        if (HasTC(TCI_NAVIGATION))
            navigation=&static_cast<CTCCreatureNavigation&>(GetTC(TCI_NAVIGATION));
        C3DVector position=building->GetPhysPos();
        C3DVector original=position;
        float radius=building->GetPhysicsTC()->GetRadius();
        if (containment)
            radius*=0.5f;
        else
            radius+=distance;
        if (navigation->GetNearbyWanderPosition(original,radius,position,false) &&
            (!containment || building->IsPointInside(position))) {
            navigation->InterruptNavigation();
            PhysicsTC->ResetPosition(position);
            navigation->InterruptNavigation();
            return true;
        }
    }
    }
    return false;
}

#include <assert.h>
#include <stdio.h>
#include <string.h>
void CTCBase::OnSerialise(CPersistContext &,bool) { assert(false); abort(); }
void CTCBase::OnRemove() { assert(false); abort(); }
void CTCBase::ConstructFromParams(const CTCParamsBase &) { assert(false); abort(); }
void CTCBase::OnCreate() { assert(false); abort(); }
void CTCBase::InitialActivate() { assert(false); abort(); }
void CTCBase::OnInitialActivate() { assert(false); abort(); }
void CTCBase::RenderUpdate(float) { assert(false); abort(); }
bool CTCBase::HasFrameUpdate() const { assert(false); abort(); }
void CTCBase::FrameUpdate() { assert(false); abort(); }
void CTCBase::BulletTimeFrameUpdate() { assert(false); abort(); }
void CTCBase::DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo &) { assert(false); abort(); }
void CTCBase::Copy(const CTCBase &) { assert(false); abort(); }
void CTCBase::OnDie() { assert(false); abort(); }
void CTCBase::OnKill() { assert(false); abort(); }
void CTCBase::OnLevelUnload() { assert(false); abort(); }
void CTCBase::OnSetInLimbo(bool) { assert(false); abort(); }
bool CTCBase::Validate(CCharString &) const { assert(false); abort(); }
bool CTCBase::IsFreeForDeletion() const { assert(false); abort(); }
bool CTCBase::IsSerialiseCreatable() const { assert(false); abort(); }
bool CTCBase::IsSerialisableForHeroState() const { assert(false); abort(); }
bool CTCBase::IsComponentSerialisable() const { assert(false); abort(); }
bool CTCBase::IsDriver() const { assert(false); abort(); }
bool CTCBase::IsCopyable() const { assert(false); abort(); }
void CTCBase::GlobalUpdate(bool) { assert(false); abort(); }
void CTCBase::HandleMessage(const NTCMessages::CMessageBase &) { assert(false); abort(); }
ETCInterfaceType CTCPhysicsBase::GetInterfaceType() const { assert(false); abort(); }
C2DVector CTCPhysicsBase::GetOffsetVector() const { assert(false); abort(); }
C3DVector CTCPhysicsBase::GetOffsetVector3D() const { assert(false); abort(); }
void CTCPhysicsBase::SetPosition(const C3DVector &) { assert(false); abort(); }
void CTCPhysicsBase::ResetRHSet(const CRightHandedSet &) { assert(false); abort(); }
void CTCPhysicsBase::SetPositionToNavigateTo(const C3DVector &) { assert(false); abort(); }
void CTCPhysicsBase::SetForcedToMovePos(const C3DVector &,bool) { assert(false); abort(); }
void CTCPhysicsBase::ResetMovementState() { assert(false); abort(); }
void CTCPhysicsBase::EnablePhysics(bool) { assert(false); abort(); }
bool CTCPhysicsBase::IsPhysicsEnabled() const { assert(false); abort(); }
void CTCPhysicsBase::EnableGravity(bool) { assert(false); abort(); }
bool CTCPhysicsBase::IsGravityEnabled() const { assert(false); abort(); }
void CTCPhysicsBase::EnableNavigator(bool) { assert(false); abort(); }
bool CTCPhysicsBase::IsNavigationEnabled() const { assert(false); abort(); }
bool CTCPhysicsBase::HasNavigationLayers() const { assert(false); abort(); }
void CTCPhysicsBase::SetInteractionFlags(long) { assert(false); abort(); }
void CTCPhysicsBase::ReturnToDefaultInteractionFlags() { assert(false); abort(); }
void CTCPhysicsBase::OnDroppedFromUpdateList() { assert(false); abort(); }
void CTCPhysicsBase::OnAddedToUpdateList() { assert(false); abort(); }
bool CTCPhysicsBase::IsAlwaysFacingMovementDirection() const { assert(false); abort(); }
bool CTCPhysicsBase::IsAffectedByGravity() const { assert(false); abort(); }
bool CTCPhysicsBase::IsAffectedByAir() const { assert(false); abort(); }
bool CTCPhysicsBase::IsAffectedByWind() const { assert(false); abort(); }
bool CTCPhysicsBase::IsAngledWithGround() const { assert(false); abort(); }
bool CTCPhysicsBase::IsImmoveable() const { assert(false); abort(); }
bool CTCPhysicsBase::IsOnAnySurface() const { assert(false); abort(); }
bool CTCPhysicsBase::IsOnAnyStationarySurface() const { assert(false); abort(); }
bool CTCPhysicsBase::IsOnMeshSurface() const { assert(false); abort(); }
bool CTCPhysicsBase::IsOnGroundSurface() const { assert(false); abort(); }
bool CTCPhysicsBase::CanCollideWithThings() const { assert(false); abort(); }
bool CTCPhysicsBase::CanCollideWithThingsOneWay() const { assert(false); abort(); }
bool CTCPhysicsBase::CanCollideWithLandscape() const { assert(false); abort(); }
bool CTCPhysicsBase::CanInteractWithThings() const { assert(false); abort(); }
bool CTCPhysicsBase::IsExcludingCreatures() const { assert(false); abort(); }
bool CTCPhysicsBase::IsPhysicsMeshImpassable() const { assert(false); abort(); }
float CTCPhysicsBase::GetFacingAngleXY() const { assert(false); abort(); }
float CTCPhysicsBase::GetFacingAngleYZ() const { assert(false); abort(); }
float CTCPhysicsBase::GetFacingAngleXZ() const { assert(false); abort(); }
void CTCPhysicsBase::SetFacingAngleXY(float) { assert(false); abort(); }
void CTCPhysicsBase::SetFacingAngleYZ(float) { assert(false); abort(); }
void CTCPhysicsBase::SetFacingAngleXZ(float) { assert(false); abort(); }
C3DVector CTCPhysicsBase::GetFacingVector(float) const { assert(false); abort(); }
C3DVector CTCPhysicsBase::GetFacingVector() const { assert(false); abort(); }
const CRightHandedSet & CTCPhysicsBase::GetRHSet() const { assert(false); abort(); }
CRightHandedSet CTCPhysicsBase::GetRHSet(float) const { assert(false); abort(); }
const CRightHandedSet & CTCPhysicsBase::GetLastRHSet() const { assert(false); abort(); }
void CTCPhysicsBase::SetFacingAnglesFromRHSet(const CRightHandedSet &) { assert(false); abort(); }
void CTCPhysicsBase::SetRHSet(const CRightHandedSet &) { assert(false); abort(); }
C3DVector CTCPhysicsBase::GetImpulse() const { assert(false); abort(); }
void CTCPhysicsBase::SetRHSetAsFlushOnNormal(const C3DVector &) { assert(false); abort(); }
void CTCPhysicsBase::SetRHSetAsFlushOnGround() { assert(false); abort(); }
float CTCPhysicsBase::GetRenderZOffset(float) const { assert(false); abort(); }
void CTCPhysicsBase::ResetInterpolation() { assert(false); abort(); }
void CTCPhysicsBase::ApplyControlledImpulse(const C3DVector &) { assert(false); abort(); }
void CTCPhysicsBase::ApplyControlledAcceleration(const C3DVector &,float) { assert(false); abort(); }
bool CTCPhysicsBase::IsPushableByHero() const { assert(false); abort(); }
void CTCPhysicsBase::SetAsPushableByHero(bool) { assert(false); abort(); }
void CTCPhysicsBase::FrameUpdate() { assert(false); abort(); }
bool CTCPhysicsBase::HasFrameUpdate() const { assert(false); abort(); }
void CTCPhysicsBase::SetPhysicsMeshBankIndex(long) { assert(false); abort(); }
void CTCPhysicsBase::SetScaleFromOwner() { assert(false); abort(); }
void CTCPhysicsBase::ConstructFromParams(const CTCParamsBase &) { assert(false); abort(); }
void CTCPhysicsBase::OnCreate() { assert(false); abort(); }
void CTCPhysicsBase::InitialActivate() { assert(false); abort(); }
void CTCPhysicsBase::Move() { assert(false); abort(); }
bool CTCPhysicsBase::DrawGetPhysicsDebugInfo(CThingPhysicalDebugInfo &) const { assert(false); abort(); }
void CTCPhysicsBase::OnSerialise(CPersistContext &,bool) { assert(false); abort(); }
bool CTCPhysicsBase::IsComponentSerialisable() const { assert(false); abort(); }
bool CTCPhysicsBase::IsSerialisableForHeroState() const { assert(false); abort(); }
bool CTCPhysicsBase::IsCopyable() const { assert(false); abort(); }
void CTCPhysicsBase::FillPhysicsDebugInfoForSelf(CPhysicsDebugInfo &) const { assert(false); abort(); }
bool CTCPhysicsBase::HasCollidedWithThingFromTo(const CTCPhysicsBase &,const C3DVector &,const C3DVector &,CPhysicsCollideResponse &) const { assert(false); abort(); }
bool CTCPhysicsBase::HasCollidedWithThingAt(const CTCPhysicsBase &,const C3DVector &,CPhysicsCollideResponse &) const { assert(false); abort(); }
bool CTCPhysicsBase::HasCollidedWithLandscapeAt(const C3DVector &,CPhysicsCollideResponse &) const { assert(false); abort(); }
void CTCPhysicsBase::UpdateInterpolation() { assert(false); abort(); }
void CTCPhysicsBase::UpdateOldRHSet() { assert(false); abort(); }
void CTCPhysicsBase::UpdateOldPos() { assert(false); abort(); }
static int TestBoundingCalls;
static float TestBoundRadius;
static C3DVector TestBoundPosition(0.0f,0.0f,0.0f);
static int TestMeshCalls,TestWanderCalls,TestInsideCalls,TestInterruptCalls,TestResetCalls;
static bool TestHasMesh,TestContainment,TestInside,TestWanderResult;
static float TestRadius,TestDistance;
static C3DVector TestBuildingPosition(2.0f,4.0f,8.0f),TestNextPosition(4.0f,8.0f,16.0f),TestResetPosition(0.0f,0.0f,0.0f);
static unsigned long TestSeed=3;
const CCharString BUILDING_BOUNDRY;
CBaseClass::CBaseClass(){} CBaseClass::~CBaseClass(){}
CBaseClassNonCopyable::CBaseClassNonCopyable(){} CBaseClassNonCopyable::~CBaseClassNonCopyable(){}
CBaseObject::CBaseObject():IntelligentPointer(0){} CBaseObject::~CBaseObject(){}
void CBaseObject::Validate()const{} void CClass::Save(CCharString&){}
CCharString::CCharString():PStringData(0){} CCharString::~CCharString(){}
CCharString::CCharString(const CCharString& x):PStringData(x.PStringData){}
CCharString::CCharString(const char*,long):PStringData(0){}
CBaseIntelligentPointer::~CBaseIntelligentPointer(){}
CBaseObject* CBaseIntelligentPointer::GetPItem()const{assert(false);abort();}
CThing::CThing(unsigned long type):UniqueID(0),PhysicsTC(0),PTCDriver(0),PTCGraphicAppearance(0),WorldFrameCreatedAt(0),ThingTypeFlags(type){Type=(unsigned char)type;Dead=0;memset(&UsingComponents,0,sizeof(UsingComponents));}
CThingPhysical::CThingPhysical(unsigned long type):CThing(type){}
CThingGameObject::CThingGameObject(unsigned long type):CThingPhysical(type){}
void CThingGameObject::OnHit(CHitParameters&){}
CThingBuilding::CThingBuilding():CThingGameObject(3){}
CThingCreatureBase::CThingCreatureBase(unsigned long type):CThingGameObject(type){}
void CThingCreatureBase::OnHit(CHitParameters&){}
const C3DVector& CThing::GetPos()const{assert(false);abort();}
C3DVector CThing::GetPhysPos()const{return TestBuildingPosition;}
unsigned long* CThing::GetPWorldSeed()const{return &TestSeed;}
CWorld& CThing::GetWorld(){assert(false);abort();}
long __fastcall GFROR13(unsigned long value){return (long)((value>>13)|(value<<19));}
namespace std {
void __stl_throw_out_of_range(const char*){assert(false);abort();}
template<class T,class A>void _List_base<T,A>::clear(){
 _List_node_base* current=_M_node._M_data->_M_next;
 while(current!=_M_node._M_data){_List_node_base* next=current->_M_next;free(current);current=next;}
 _M_node._M_data->_M_next=_M_node._M_data;_M_node._M_data->_M_prev=_M_node._M_data;
}
}
CTCBase::CTCBase(CThing& parent):ParentThing(parent),ToBeRemoved(false),IsInGlobalUpdateTCs(false){}
CTCBase::~CTCBase(){}
CTCPhysicsBase::CTCPhysicsBase(CThing& parent):CTCBase(parent),Position(0.0f,0.0f,0.0f),OldPosition(0.0f,0.0f,0.0f),Velocity(0.0f,0.0f,0.0f),Radius(8.0f){}
CTCPhysicsBase::~CTCPhysicsBase(){}
const CThingPhysicsMeshInfo* CTCPhysicsBase::GetPhysicsMeshInfo()const{++TestMeshCalls;return TestHasMesh?reinterpret_cast<const CThingPhysicsMeshInfo*>(this):0;}
void CTCPhysicsBase::ResetPosition(const C3DVector& position){assert(TestInterruptCalls==1);++TestResetCalls;TestResetPosition=position;}
void CTCPhysicsBase::GetBoundingInfo(C3DVector& position,float& radius)const{++TestBoundingCalls;position=TestBoundPosition;radius=TestBoundRadius;}
CPhysicsMeshObject::CPhysicsMeshObject(){memset(Mesh,0,sizeof(Mesh));memset(PositionMatrix,0,sizeof(PositionMatrix));Scale=OO_Scale=1.0f;}
CPhysicsMeshObject::~CPhysicsMeshObject(){}
CPhysicsMeshObject CTCPhysicsBase::GetPhysicsMeshObject()const{return CPhysicsMeshObject();}
bool CPhysicsMeshObject::HasContainmentVolume(const CCharString& name)const{assert(&name==&BUILDING_BOUNDRY);return TestContainment;}
CTCCreatureNavigation::CTCCreatureNavigation(CThing& parent):CTCBase(parent){}
void CTCCreatureNavigation::InterruptNavigation(){++TestInterruptCalls;assert(TestInterruptCalls<=2);}
bool CTCCreatureNavigation::GetNearbyWanderPosition(const C3DVector& original,float radius,C3DVector& out,bool flag)const{
 ++TestWanderCalls;assert(!flag);assert(original==TestBuildingPosition);TestRadius=radius;out=TestNextPosition;return TestWanderResult;
}
bool CTCCreatureNavigation::GetNearestNavigablePosTo(const C3DVector&,float,C3DVector&)const{assert(false);abort();}
bool CThingBuilding::IsPointInside(const C3DVector& position)const{++TestInsideCalls;assert(position==TestNextPosition);return TestInside;}
const std::list<CClass*>& CThingSearchTools::PeekTypeList(long)const{assert(false);abort();}
bool CThingFilter_PosInContainmentVolume::operator()(const CThing&)const{assert(false);abort();}
class CTestPhysics:public CTCPhysicsBase{
public: CTestPhysics(CThing& thing):CTCPhysicsBase(thing){}
 CCharString GetClassTypeName()const{return CCharString();}
};
class CTestNavigation:public CTCCreatureNavigation{
public: CTestNavigation(CThing& thing):CTCCreatureNavigation(thing){}
 ETCInterfaceType GetInterfaceType()const{return TCI_NAVIGATION;}
 CCharString GetClassTypeName()const{return CCharString();}
};
class CAutoPlaceFixture{
public:
 static void SetPhysics(CThing& thing,CTCPhysicsBase* physics){thing.PhysicsTC=physics;}
 static void SetDead(CThing& thing,bool dead){thing.Dead=dead;}
 static void AddNavigation(CThing& thing,CTCCreatureNavigation* navigation){
  std::pair<ETCInterfaceType,CTCBase*> item;item.first=TCI_NAVIGATION;item.second=navigation;thing.Components.push_back(item);
  thing.UsingComponents._M_w[TCI_NAVIGATION/(8*sizeof(unsigned long))]|=1UL<<(TCI_NAVIGATION%(8*sizeof(unsigned long)));
 }
};
static void ResetFixture(){TestMeshCalls=TestWanderCalls=TestInsideCalls=TestInterruptCalls=TestResetCalls=0;TestRadius=0;TestHasMesh=true;TestContainment=false;TestInside=true;TestWanderResult=true;}
int main(){
 CThingCreatureBase creature(2);CThingBuilding building;CTestPhysics creaturePhysics(creature),buildingPhysics(building);CTestNavigation navigation(creature);
 CAutoPlaceFixture::SetPhysics(creature,&creaturePhysics);CAutoPlaceFixture::SetPhysics(building,&buildingPhysics);CAutoPlaceFixture::AddNavigation(creature,&navigation);
 ResetFixture();assert(!creature.AutoPlaceNearBuilding(0,2.0f));assert(TestMeshCalls==0);
 CAutoPlaceFixture::SetDead(building,true);assert(!creature.AutoPlaceNearBuilding(&building,2.0f));assert(TestMeshCalls==0);CAutoPlaceFixture::SetDead(building,false);
 ResetFixture();TestHasMesh=false;assert(!creature.AutoPlaceNearBuilding(&building,2.0f));assert(TestMeshCalls==10&&TestWanderCalls==0&&TestResetCalls==0);
 ResetFixture();TestWanderResult=false;assert(!creature.AutoPlaceNearBuilding(&building,2.0f));assert(TestMeshCalls==10&&TestWanderCalls==10&&TestInsideCalls==0&&TestResetCalls==0&&TestRadius==10.0f);
 ResetFixture();assert(creature.AutoPlaceNearBuilding(&building,2.0f));assert(TestMeshCalls==1&&TestWanderCalls==1&&TestInsideCalls==0&&TestRadius==10.0f);assert(TestInterruptCalls==2&&TestResetCalls==1&&TestResetPosition==TestNextPosition);
 ResetFixture();TestContainment=true;TestInside=false;assert(!creature.AutoPlaceNearBuilding(&building,2.0f));assert(TestMeshCalls==10&&TestWanderCalls==10&&TestInsideCalls==10&&TestRadius==4.0f&&TestResetCalls==0);
 ResetFixture();TestContainment=true;assert(creature.AutoPlaceNearBuilding(&building,2.0f));assert(TestMeshCalls==1&&TestWanderCalls==1&&TestInsideCalls==1&&TestRadius==4.0f&&TestResetCalls==1&&TestInterruptCalls==2);
 ResetFixture();TestContainment=true;TestWanderResult=false;assert(!creature.AutoPlaceNearBuilding(&building,4.0f));assert(TestMeshCalls==10&&TestWanderCalls==10&&TestInsideCalls==0&&TestResetCalls==0);
 const float shifts[2][3]={{0.0f,0.0f,0.0f},{2.0f,-4.0f,8.0f}};
 const float offsets[9][3]={{0.0f,0.0f,0.0f},{1.0f,0.0f,0.0f},{0.0f,-2.0f,0.0f},{0.0f,0.0f,3.0f},{3.0f,4.0f,0.0f},{1.0f,2.0f,2.0f},{-4.0f,0.0f,0.0f},{2.0f,-2.0f,1.0f},{-1.0f,-2.0f,-2.0f}};
 const float radii[3]={0.0f,1.0f,3.0f};
 int checked=0;
 for(int centre_index=0;centre_index<2;++centre_index)
  for(int offset_index=0;offset_index<9;++offset_index)
   for(int filter_index=0;filter_index<3;++filter_index)
    for(int bound_index=0;bound_index<3;++bound_index){
     C3DVector sphere_pos(shifts[centre_index][0],shifts[centre_index][1],shifts[centre_index][2]);
     TestBoundPosition=C3DVector(sphere_pos.X+offsets[offset_index][0],sphere_pos.Y+offsets[offset_index][1],sphere_pos.Z+offsets[offset_index][2]);
     TestBoundRadius=radii[bound_index];TestBoundingCalls=0;
     CThingFilter_OverlappingSphere filter(sphere_pos,radii[filter_index]);
     float sum_radius=radii[filter_index]+radii[bound_index];
     float distance_squared=offsets[offset_index][0]*offsets[offset_index][0]+offsets[offset_index][1]*offsets[offset_index][1]+offsets[offset_index][2]*offsets[offset_index][2];
     assert(filter(building)==(distance_squared<sum_radius*sum_radius));
     assert(TestBoundingCalls==1);++checked;
    }
 assert(checked==162);
 printf("00666ef0_TEST PASS\n");return 0;
}
