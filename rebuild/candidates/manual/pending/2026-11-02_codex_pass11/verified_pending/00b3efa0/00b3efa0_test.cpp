#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#define __stdcall
#define __forceinline inline
#define __declspec(x)
typedef long FableI64;
#else
typedef __int64 FableI64;
#endif
#define FABLE_STATIC_ASSERT(x)


#define FABLE_RETAIL_CENGINEMAP_READ
#define FABLE_RETAIL_CDATAINPUTSTREAM
#ifndef FABLE_ENGINE_CENGINEMAP_H
#define FABLE_ENGINE_CENGINEMAP_H
#ifndef FABLE_ENGINE_C3DBOUNDINGBOX_H
#define FABLE_ENGINE_C3DBOUNDINGBOX_H
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
#ifdef FABLE_RETAIL_CAMERA_DRAW
inline float __fastcall GFSqr(float n){return n*n;}
#endif
extern const float DELTA_NOTIONAL_ZERO;
extern unsigned int GFastOneOverSqrtTable[128];
inline float __fastcall GFFloatAbs(float n){
#ifdef _MSC_VER
 *(unsigned int*)&n &= 0x7fffffffUL;
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
 typedef unsigned int word_type;
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
#ifdef FABLE_RETAIL_VALIDPOSITIONS
#ifndef FABLE_ENGINE_C2DCOORDI_H
#define FABLE_ENGINE_C2DCOORDI_H
#define FABLE_CDECL __cdecl
#define FABLE_STDCALL __stdcall
#define FABLE_FASTCALL __fastcall
#define FABLE_THISCALL
typedef signed char fable_i8;typedef unsigned char fable_u8;typedef short fable_i16;typedef unsigned short fable_u16;typedef int fable_i32;typedef unsigned int fable_u32;


class C2DCoordI { public: int X,Y;C2DCoordI(){}C2DCoordI(int x,int y):X(x),Y(y){} };
#endif

#endif
class C3DVector {
public:
    float X,Y,Z;
    C3DVector() {}
#ifdef FABLE_RETAIL_VECTOR_SPLINE_CTOR
    C3DVector(float x,float y,float z){X=x;Y=y;Z=z;}
#else
#ifdef FABLE_RETAIL_VALIDPOSITIONS
    C3DVector(float x,float y,float z){Initialise(x,y,z);}
#else
    C3DVector(float x,float y,float z):X(x),Y(y),Z(z){}
#endif
#endif
#if defined(FABLE_RETAIL_VECTOR_GEOMETRY) || defined(FABLE_RETAIL_VECTOR_SPLINE_COPY)
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
    FABLE_VECTOR_INLINE const C3DVector& operator+=(const C3DVector& v){X+=v.X;Y+=v.Y;Z+=v.Z;return *this;}
    FABLE_VECTOR_INLINE const C3DVector& operator*=(float v){X*=v;Y*=v;Z*=v;return *this;}
    FABLE_VECTOR_INLINE const C3DVector operator/(float v)const{float reciprocal=1.0f/v;return C3DVector(X*reciprocal,Y*reciprocal,Z*reciprocal);}
    FABLE_VECTOR_INLINE const C3DVector& operator/=(float v){float reciprocal=1.0f/v;X*=reciprocal;Y*=reciprocal;Z*=reciprocal;return *this;}
    FABLE_VECTOR_INLINE float GetMagnitude()const{return (float)sqrt(GetSquaredMagnitude());}
#endif
#ifdef FABLE_RETAIL_VALIDPOSITIONS
    inline C2DCoordI ToC2DCoordI() const { return C2DCoordI((int)X,(int)Y); }
#endif
    C3DVector& AccurateNormalise();
    void Rotate(const C3DVector&,float);
};
#undef FABLE_VECTOR_INLINE
#endif



class C3DBoundingBox {
private:
    C3DVector MinCoord;
    C3DVector MaxCoord;
};
#endif

#ifndef FABLE_ENGINE_CCHARSTRING_H
#define FABLE_ENGINE_CCHARSTRING_H


#if defined(FABLE_RETAIL_SCOREBOARD) || defined(FABLE_RETAIL_PERSIST_OPINION) || defined(FABLE_RETAIL_SUBTITLE) || defined(FABLE_RETAIL_MINIMAP_TRAITS) || defined(FABLE_RETAIL_CTHINGMANAGER_SAVE)
#define FABLE_RETAIL_CCHARSTRING_DATA
#pragma once

#include <stddef.h>

#define FABLE_CDECL __cdecl
#define FABLE_STDCALL __stdcall
#define FABLE_FASTCALL __fastcall
#define FABLE_THISCALL
typedef signed char fable_i8;typedef unsigned char fable_u8;typedef short fable_i16;typedef unsigned short fable_u16;typedef int fable_i32;typedef unsigned int fable_u32;


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
    unsigned int StringLength;
    unsigned int DataLength : 31;
    bool UseFastExtend : 1;
#ifdef FABLE_RETAIL_MINIMAP_TRAITS
    static int __fastcall Compare(const T*,const T*);
#endif
public:
    const T* PeekData() const { return PData; }
#ifdef FABLE_RETAIL_SUBTITLE
    unsigned int GetLength() const { return StringLength; }
#endif
#if defined(FABLE_RETAIL_PERSIST_OPINION) || defined(FABLE_RETAIL_CTHINGMANAGER_SAVE) || defined(FABLE_RETAIL_MINIMAP_TRAITS)
    bool operator==(const T* text) const;
#endif
};
class CCharStringData {
public:
    CBasicString<char> Data;
protected:
    int NoRefs;
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
    int GetLength() const;
    int FindCharPos(wchar_t c) const;                         
    void __cdecl Sprintf(const wchar_t* format, ...);          
    void SetLength(int length);                               
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
        int length);

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
    CCharString(const char* text, int length);
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
        int length);

private:
    CCharStringData* storage_;
};

CCharString FABLE_FASTCALL operator+(
    const CCharString& left,
    const char* right);



extern fable_i32 g_CWideStringInstanceCount_013BCA20;
extern fable_i32 g_CCharStringInstanceCount_013BD800;
extern wchar_t g_FableEmptyWideString_0129A8E0[1];


#endif

#endif
class CCharStringData;
class CCharString {
public:
    CCharString();
    CCharString(const char* text, int length = -1);
    CCharString(const CCharString& other);
#ifdef FABLE_RETAIL_MINIMAP_TRAITS
    const CCharString& operator=(const CCharString&);
    bool operator!=(const char* text) const { return !(*this==text); }
#endif
#if defined(FABLE_RETAIL_SCOREBOARD) || defined(FABLE_RETAIL_SUBTITLE)
#ifdef FABLE_RETAIL_SUBTITLE
    const char* GetAsCharArray() const { return PStringData==0 ? &NullChar : PStringData->Data.PeekData(); }
#else
    const char* GetAsCharArray() const { const char* result=&NullChar; if(PStringData) result=PStringData->Data.PeekData(); return result; }
#endif
#endif
#if defined(FABLE_RETAIL_EFFECT_LOOKUP) || defined(FABLE_RETAIL_CTHINGMANAGER_SAVE)
    unsigned int GetCRC() const; 
#endif
#if defined(FABLE_RETAIL_PERSIST_OPINION) || defined(FABLE_RETAIL_MINIMAP_TRAITS)
    bool EqualsCaseInsensitive(const CCharString&) const;
    bool operator==(const char* text) const;
#endif
#ifdef FABLE_RETAIL_SUBTITLE
    int GetLength() const { return PStringData==0 ? 0 : PStringData->Data.GetLength(); }
    inline bool StartsWith(const CCharString& str) const { return strncmp(GetAsCharArray(),str.GetAsCharArray(),str.GetLength())==0; }
    inline bool EndsWith(const CCharString& str) const { int offset=GetLength()-str.GetLength();if(offset>=0)return strcmp(GetAsCharArray()+offset,str.GetAsCharArray())==0;return false; }
#endif
#ifdef FABLE_RETAIL_CTHINGMANAGER_SAVE
    bool operator==(const char* text) const { if(PStringData) return PStringData->Data==text; return strcmp("",text)==0; }
    bool operator!=(const char* text) const { if(!PStringData) return strcmp("",text)!=0; return !(PStringData->Data==text); }
    const CCharString& operator+=(const CCharString&);
    const CCharString& operator+=(const char*);
    const CCharString& operator=(const char*);
#endif
    ~CCharString();
protected:
#if defined(FABLE_RETAIL_SCOREBOARD) || defined(FABLE_RETAIL_SUBTITLE)
    static const char NullChar;
#endif
    CCharStringData* PStringData;
};
#endif

#ifndef FABLE_ENGINE_CCOUNTEDPOINTER_H
#define FABLE_ENGINE_CCOUNTEDPOINTER_H









































#define FABLE_CDECL __cdecl
#define FABLE_STDCALL __stdcall
#define FABLE_FASTCALL __fastcall
#define FABLE_THISCALL
typedef signed char fable_i8;typedef unsigned char fable_u8;typedef short fable_i16;typedef unsigned short fable_u16;typedef int fable_i32;typedef unsigned int fable_u32;


class CCPPointerInfo {
public:
    unsigned int RefCount;                                              
    void (FABLE_FASTCALL* DeleteFunc)(void*);                             
    void* Data;                                                          
    CCPPointerInfo(unsigned int refCount, void (FABLE_FASTCALL* deleteFunc)(void*), void* data)
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
    unsigned int GetReferenceCount() const { return Info ? Info->RefCount : 0; }   
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

#ifndef FABLE_ENGINE_CENGINESTATICMAPFILEBLOCKMANAGER_H
#define FABLE_ENGINE_CENGINESTATICMAPFILEBLOCKMANAGER_H
#ifndef FABLE_ENGINE_CCOUNTEDPOINTER_H
#define FABLE_ENGINE_CCOUNTEDPOINTER_H









































#define FABLE_CDECL __cdecl
#define FABLE_STDCALL __stdcall
#define FABLE_FASTCALL __fastcall
#define FABLE_THISCALL
typedef signed char fable_i8;typedef unsigned char fable_u8;typedef short fable_i16;typedef unsigned short fable_u16;typedef int fable_i32;typedef unsigned int fable_u32;


class CCPPointerInfo {
public:
    unsigned int RefCount;                                              
    void (FABLE_FASTCALL* DeleteFunc)(void*);                             
    void* Data;                                                          
    CCPPointerInfo(unsigned int refCount, void (FABLE_FASTCALL* deleteFunc)(void*), void* data)
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
    unsigned int GetReferenceCount() const { return Info ? Info->RefCount : 0; }   
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

#ifndef FABLE_STLPORT_MAP_H
#define FABLE_STLPORT_MAP_H
#ifndef FABLE_STLPORT_CONTAINERS_H
#define FABLE_STLPORT_CONTAINERS_H



#include <stddef.h>
#if defined(FABLE_STLPORT_UI_ACTIONS) || defined(FABLE_STLPORT_CREATURE_POSITIONS) || defined(FABLE_STLPORT_CAMERA_POINTS) || defined(FABLE_RETAIL_ENTITY_PERSIST)
#include <new>
#endif
#include <stdlib.h>
#if defined(FABLE_STLPORT_VISIBLE_TRUE_OVERFLOW) || defined(FABLE_RETAIL_CTHINGMANAGER_BINARY)
#include <string.h>
#endif
#if defined(FABLE_STLPORT_VISIBLE_PUSH_BACK) || defined(FABLE_STLPORT_HTTP)
#include <new>
#include <string.h>
#endif
namespace std {
#ifdef FABLE_RETAIL_CTHINGMANAGER_BINARY
inline void fill(unsigned char* first,unsigned char* last,const unsigned char& value) {
    unsigned char tmp=value;memset(first,tmp,last-first);
}
template<class Size> inline unsigned char* fill_n(unsigned char* first,Size n,const unsigned char& value) {
    fill(first,first+n,value);return first+n;
}
#endif
struct __true_type { };
#ifdef FABLE_STLPORT_CAMERA_POINTS
struct __false_type { };
#endif
#ifdef FABLE_RETAIL_CTHINGMANAGER_BINARY
struct __false_type { };
template<class T> struct __type_traits { typedef __false_type is_POD_type; };
template<> struct __type_traits<unsigned char> { typedef __true_type is_POD_type; };
template<class Iterator> struct iterator_traits;
template<class T> struct iterator_traits<T*> { typedef T value_type; };
template<class ForwardIterator,class Size,class T>
inline ForwardIterator __uninitialized_fill_n(ForwardIterator first,Size n,const T& value,const __true_type&) {
    return std::fill_n(first,n,value);
}
template<class ForwardIterator,class Size,class T>
inline ForwardIterator uninitialized_fill_n(ForwardIterator first,Size n,const T& value) {
    typedef typename iterator_traits<ForwardIterator>::value_type Value;
    typedef typename __type_traits<Value>::is_POD_type IsPOD;
    return __uninitialized_fill_n(first,n,value,IsPOD());
}
#endif
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
#ifdef FABLE_RETAIL_QUEST_COMPLETION
template<class T> struct __type_traits;
template<class I> struct iterator_traits;
template<class T> struct iterator_traits<T*> {
    typedef random_access_iterator_tag iterator_category;
    typedef T value_type;
    typedef ptrdiff_t difference_type;
    typedef T* pointer;
    typedef T& reference;
};
template<class I> inline void __destroy_aux(I first,I last,const __false_type&) {
    for(;first!=last;++first) _Destroy(&*first);
}
template<class I> inline void __destroy_aux(I,I,const __true_type&) { }
template<class I,class T> inline void __destroy(I first,I last,T*) {
    typedef typename __type_traits<T>::has_trivial_destructor Trivial;
    __destroy_aux(first,last,Trivial());
}
template<class I> inline void _Destroy(I first,I last) {
    __destroy(first,last,(typename iterator_traits<I>::value_type*)0);
}
#else
template<class I> inline void _Destroy(I first,I last) { for(;first!=last;++first) _Destroy(&*first); }
#endif
template<class I,class O> inline O __uninitialized_copy(I first,I last,O result,const __false_type&) {
    O cur=result;for(;first!=last;++first,++cur) _Construct(&*cur,*first);return cur;
}
template<class I,class N,class T> inline I __uninitialized_fill_n(I first,N n,const T& x,const __false_type&) {
    I cur=first;for(;n>0;--n,++cur) _Construct(&*cur,x);return cur;
}
#endif
#ifdef FABLE_STLPORT_HTTP
template<class T> struct __type_traits { typedef __false_type has_trivial_assignment_operator; typedef __false_type has_trivial_destructor; };
template<> struct __type_traits<char> { typedef __true_type has_trivial_assignment_operator; };
#ifdef FABLE_RETAIL_QUEST_COMPLETION
template<> struct __type_traits<int> { typedef __true_type has_trivial_assignment_operator; typedef __true_type has_trivial_destructor; };
template<> struct __type_traits<unsigned int> { typedef __true_type has_trivial_assignment_operator; typedef __true_type has_trivial_destructor; };
#endif
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
#ifdef FABLE_STLPORT_CREATURE_POSITIONS
#ifndef FABLE_STLPORT_HTTP
template<class T> inline const T& (max)(const T& a,const T& b) { return a<b?b:a; }
template<class T> inline const T& (min)(const T& a,const T& b) { return b<a?b:a; }
struct __false_type { __false_type() { } };
struct input_iterator_tag { };
struct forward_iterator_tag : public input_iterator_tag { };
struct bidirectional_iterator_tag : public forward_iterator_tag { };
struct random_access_iterator_tag : public bidirectional_iterator_tag { random_access_iterator_tag() { } };
#endif
template<class Input,class Output,class Distance>
Output __fastcall __copy(Input,Input,Output,const random_access_iterator_tag&,Distance*);
template<class Input,class Output> inline Output __copy_ptrs(Input first,Input last,Output result,const __false_type&) {
    return __copy(first,last,result,random_access_iterator_tag(),(ptrdiff_t*)0);
}
#endif
#ifdef FABLE_RETAIL_ENTITY_PERSIST
template<class Input,class Output>
Output __fastcall __uninitialized_copy(Input,Input,Output,const __false_type&);
template<class Input,class Size,class T>
Input __fastcall __uninitialized_fill_n(Input,Size,const T&,const __false_type&);
template<class T,class U> inline void _Construct(T* p,const U& value) { new(p) T(value); }
template<class T> inline void _Destroy(T* p) { p->~T(); }
template<class I> inline void _Destroy(I first,I last) { for(;first!=last;++first) _Destroy(&*first); }
template<class T> inline const T& (max)(const T& a,const T& b) { return a<b?b:a; }
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
    void deallocate(_Tp* p, size_t) {
#ifdef FABLE_RETAIL_ENTITY_PERSIST
        if(p) free(p);
#else
        free(p);
#endif
    }
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
#if defined(FABLE_STLPORT_UI) || defined(FABLE_RETAIL_CTHINGMANAGER_BINARY)
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
#ifdef FABLE_RETAIL_CTHINGMANAGER_SAVE
    vector& operator=(const vector&);
#endif
#ifdef FABLE_RETAIL_CTHINGMANAGER_BINARY
    explicit vector(size_type n) : _Vector_base<_Tp,_Alloc>(n,_Alloc()) {
        this->_M_finish=uninitialized_fill_n(this->_M_start,n,_Tp());
    }
    _Tp& front() { return *this->_M_start; }
#endif
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
#ifdef FABLE_RETAIL_ENTITY_PERSIST
    void push_back(const _Tp& x) {
        if(this->_M_finish!=this->_M_end_of_storage._M_data) {
            _Construct(this->_M_finish,x);++this->_M_finish;
        } else _M_insert_overflow(this->_M_finish,x,__false_type(),1,true);
    }
protected:
    void _M_insert_overflow(iterator position,const _Tp& x,const __false_type&,size_type fill_len,bool at_end=false) {
        const size_type old_size=size();const size_type len=old_size+(max)(old_size,fill_len);
        _Tp* new_start=this->_M_end_of_storage.allocate(len);
        _Tp* new_finish=new_start;
        new_finish=__uninitialized_copy(this->_M_start,position,new_start,__false_type());
        if(fill_len==1) { _Construct(new_finish,x);++new_finish; }
        else new_finish=__uninitialized_fill_n(new_finish,fill_len,x,__false_type());
        if(!at_end) new_finish=__uninitialized_copy(position,this->_M_finish,new_finish,__false_type());
        _M_clear();_M_set(new_start,new_finish,new_start+len);
    }
    void _M_clear() {
        _Destroy(this->_M_start,this->_M_finish);
        this->_M_end_of_storage.deallocate(this->_M_start,this->_M_end_of_storage._M_data-this->_M_start);
    }
    void _M_set(_Tp* start,_Tp* finish,_Tp* end) {
        this->_M_start=start;this->_M_finish=finish;this->_M_end_of_storage._M_data=end;
    }
public:
#endif
    _Tp& operator[](size_type n) { return *(begin()+n); }
    const _Tp& operator[](size_type n) const { return *(begin()+n); }
#ifdef FABLE_STLPORT_HTTP
    typedef typename __type_traits<_Tp>::has_trivial_assignment_operator _IsPODType;
#ifdef FABLE_RETAIL_QUEST_COMPLETION
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
#ifndef FABLE_RETAIL_QUEST_COMPLETION
    __declspec(noinline)
#endif
    void _M_insert_overflow(iterator,const _Tp&,const __true_type&,size_type,bool);
public:
#endif
    void push_back(const _Tp& x) {
        if(this->_M_finish!=this->_M_end_of_storage._M_data) {
            _Construct(this->_M_finish,x); ++this->_M_finish;
        } else _M_insert_overflow(this->_M_finish,x,_IsPODType(),1,true);
    }
#ifdef FABLE_RETAIL_QUEST_COMPLETION
    iterator insert(iterator position,const _Tp& x) {
        size_type n=position-begin();
        if(this->_M_finish!=this->_M_end_of_storage._M_data) {
            if(position==end()) {
                _Construct(this->_M_finish,x);
                ++this->_M_finish;
            } else {
                _Construct(this->_M_finish,*(this->_M_finish-1));
                ++this->_M_finish;
                _Tp x_copy=x;
                __copy_backward_ptrs(position,this->_M_finish-2,this->_M_finish-1,_IsPODType());
                *position=x_copy;
            }
        } else _M_insert_overflow(position,x,_IsPODType(),1UL,false);
        return begin()+n;
    }
#endif
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
#ifndef FABLE_RETAIL_QUEST_COMPLETION
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
#ifndef FABLE_RETAIL_QUEST_COMPLETION
    __declspec(noinline)
#endif
    void _M_insert_overflow(iterator position,const _Tp& x,const __true_type&,size_type fillLength,bool atEnd) {
        const size_type oldSize=size();const size_type len=oldSize+(max)(oldSize,fillLength);
        _Tp* newStart=this->_M_end_of_storage.allocate(len);
        _Tp* newFinish=(_Tp*)__copy_trivial(this->_M_start,position,newStart);
        newFinish=fill_n(newFinish,fillLength,x);
        if(!atEnd) newFinish=(_Tp*)__copy_trivial(position,this->_M_finish,newFinish);
        _M_clear();_M_set(newStart,newFinish,newStart+len);
    }
#endif
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
#elif defined(FABLE_STLPORT_CREATURE_POSITIONS)
    
    void push_back(const _Tp& x) {
        if(this->_M_finish!=this->_M_end_of_storage._M_data) {
            new((void*)this->_M_finish) _Tp(x); ++this->_M_finish;
        } else _M_insert_overflow(this->_M_finish,x,__false_type(),1,true);
    }
    iterator erase(iterator first,iterator last) {
        iterator i=std::__copy_ptrs(last,this->_M_finish,first,__false_type());
        for(iterator d=i;d!=this->_M_finish;++d) d->~_Tp();
        this->_M_finish=i;
        return first;
    }
    void clear() { erase(begin(),end()); }
protected:
#ifdef FABLE_STLPORT_VISIBLE_POSITIONS_OVERFLOW
    
    void _M_insert_overflow(iterator position,const _Tp& value,const __false_type&,size_type fillLength,bool atEnd) {
        const size_type oldSize=size();
        const size_type length=oldSize+(oldSize<fillLength?fillLength:oldSize);
        _Tp* newStart=(_Tp*)malloc(length*sizeof(_Tp));
        _Tp* newFinish=newStart;
        for(iterator first=this->_M_start;first!=position;++first,++newFinish) new((void*)newFinish) _Tp(*first);
        for(size_type count=fillLength;count>0;--count,++newFinish) new((void*)newFinish) _Tp(value);
        if(!atEnd) for(iterator first=position;first!=this->_M_finish;++first,++newFinish) new((void*)newFinish) _Tp(*first);
        for(iterator first=this->_M_start;first!=this->_M_finish;++first) first->~_Tp();
        if(this->_M_start) free(this->_M_start);
        this->_M_start=newStart;this->_M_finish=newFinish;this->_M_end_of_storage._M_data=newStart+length;
    }
#else
    void _M_insert_overflow(iterator,const _Tp&,const __false_type&,size_type,bool);
#endif
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
#ifndef FABLE_RETAIL_ENTITY_PERSIST
    void push_back(const _Tp&);
#endif
#endif
};
#ifdef FABLE_RETAIL_QUEST_COMPLETION
template<class _Tp,class _Alloc> inline void vector<_Tp,_Alloc>::_M_insert_overflow(iterator position,const _Tp& x,const __true_type&,size_type fillLength,bool atEnd) {
        const size_type oldSize=size();const size_type len=oldSize+(max)(oldSize,fillLength);
        _Tp* newStart=this->_M_end_of_storage.allocate(len);
        _Tp* newFinish=(_Tp*)__copy_trivial(this->_M_start,position,newStart);
        newFinish=fill_n(newFinish,fillLength,x);
        if(!atEnd) newFinish=(_Tp*)__copy_trivial(position,this->_M_finish,newFinish);
        _M_clear();_M_set(newStart,newFinish,newStart+len);
    }
#endif
template<class _T1,class _T2> struct pair {
    typedef _T1 first_type;
    typedef _T2 second_type;
    _T1 first;
    _T2 second;
#if defined(FABLE_STLPORT_CAMERA_POINTS) || defined(FABLE_RETAIL_QUEST_COMPLETION)
    pair():first(),second(){}
    pair(const _T1& a,const _T2& b):first(a),second(b) { }
    pair(const pair& p):first(p.first),second(p.second) { }
#endif
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
#if defined(FABLE_STLPORT_UI_CHANGE_STATE) || defined(FABLE_STLPORT_CAMERA_POINTS)
    _List_iterator(_List_node<_Tp>* x) : _List_iterator_base(x) { }
#else
    _List_iterator(_List_node_base* x) : _List_iterator_base(x) { }
#endif
    _List_iterator() { }
#if defined(FABLE_STLPORT_UI_CHANGE_STATE) || defined(FABLE_STLPORT_CAMERA_POINTS)
    _List_iterator(const iterator& x) : _List_iterator_base(x._M_node) { }
#endif
    reference operator*() const { return ((_List_node<_Tp>*)_M_node)->_M_data; }
    pointer operator->() const { return &operator*(); }
    _List_iterator& operator++() { this->_M_incr(); return *this; }
#ifdef FABLE_STLPORT_CAMERA_POINTS
    _List_iterator operator++(int) { _List_iterator tmp=*this; this->_M_incr(); return tmp; }
#endif
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
#ifdef FABLE_STLPORT_CAMERA_POINTS
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
#else
    void clear();
#endif
};
template<class _Tp,class _Alloc=allocator<_Tp> > class list : public _List_base<_Tp,_Alloc> {
public:
#ifdef FABLE_STLPORT_CONSTRUCTORS
    explicit list(const _Alloc& a=_Alloc()) : _List_base<_Tp,_Alloc>(a) { }
#endif
    typedef _List_iterator<_Tp,_Nonconst_traits<_Tp> > iterator;
    typedef _List_iterator<_Tp,_Const_traits<_Tp> > const_iterator;
#if defined(FABLE_STLPORT_UI_CHANGE_STATE) || defined(FABLE_STLPORT_CAMERA_POINTS)
    iterator begin() { return iterator((_List_node<_Tp>*)this->_M_node._M_data->_M_next); }
    const_iterator begin() const { return const_iterator((_List_node<_Tp>*)this->_M_node._M_data->_M_next); }
#else
    iterator begin() { return iterator(this->_M_node._M_data->_M_next); }
    const_iterator begin() const { return const_iterator(this->_M_node._M_data->_M_next); }
#endif
    iterator end() { return iterator(this->_M_node._M_data); }
    const_iterator end() const { return const_iterator(this->_M_node._M_data); }
    bool empty() const { return this->_M_node._M_data->_M_next==this->_M_node._M_data; }
#ifdef FABLE_RETAIL_CAMERA_TRANSITIONS
    _Tp& front() { return *begin(); }
    const _Tp& front() const { return *begin(); }
#endif
#ifdef FABLE_STLPORT_CAMERA_POINTS
    _Alloc get_allocator() const { return _Alloc(); }
    list(const list& x):_List_base<_Tp,_Alloc>(x.get_allocator()) {
        insert(begin(),x.begin(),x.end());
    }
    ~list() { }
    template<class Input> void insert(iterator p,Input first,Input last) {
        _M_insert_dispatch(p,first,last,__false_type());
    }
    template<class Input> __declspec(noinline) void _M_insert_dispatch(iterator position,Input first,Input last,const __false_type&) {
        for(;first!=last;++first) insert(position,*first);
    }
#endif
#if defined(FABLE_STLPORT_UI_ACTIONS) || defined(FABLE_STLPORT_CAMERA_POINTS)
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
    typedef unsigned int _WordT;
    _WordT _M_w[_Nw];
    static size_t _S_whichword(size_t n) { return n/(8*sizeof(_WordT)); }
    static size_t _S_whichbit(size_t n) { return n%(8*sizeof(_WordT)); }
    static _WordT _S_maskbit(size_t n) { return _WordT(1)<<_S_whichbit(n); }
    _WordT _M_getword(size_t n) const { return _M_w[_S_whichword(n)]; }
};
void __stl_throw_out_of_range(const char*);
template<size_t _Nb> class bitset : public _Base_bitset<(_Nb+8*sizeof(unsigned int)-1)/(8*sizeof(unsigned int))> {
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



namespace std {
template<class A,class B,class R> struct binary_function { typedef A first_argument_type;typedef B second_argument_type;typedef R result_type; };
template<class T> struct less:public binary_function<T,T,bool>{bool operator()(const T& a,const T& b)const{return a<b;}};
template<class P> struct _Select1st:public unary_function<P,typename P::first_type>{const typename P::first_type& operator()(const P& p)const{return p.first;}};
typedef bool _Rb_tree_Color_type;
struct _Rb_tree_node_base {
    _Rb_tree_Color_type _M_color;
    _Rb_tree_node_base* _M_parent;
    _Rb_tree_node_base* _M_left;
    _Rb_tree_node_base* _M_right;
};
template<class V> struct _Rb_tree_node:public _Rb_tree_node_base { V _M_value_field; };
#if defined(FABLE_RETAIL_QUEST_COMPLETION) || defined(FABLE_RETAIL_ENTITY_PERSIST)
template<class Dummy> class _Rb_global {
public:
    static _Rb_tree_node_base* __cdecl _M_increment(_Rb_tree_node_base* node) {
        if(node->_M_right!=0) {
            node=node->_M_right;
            while(node->_M_left!=0) node=node->_M_left;
        } else {
            _Rb_tree_node_base* parent=node->_M_parent;
            while(node==parent->_M_right) {node=parent;parent=parent->_M_parent;}
            if(node->_M_right!=parent) node=parent;
        }
        return node;
    }
};
typedef _Rb_global<bool> _Rb_global_inst;
#endif
struct _Rb_tree_base_iterator {
    _Rb_tree_node_base* _M_node;
    bool operator==(const _Rb_tree_base_iterator& y)const{return _M_node==y._M_node;}
    bool operator!=(const _Rb_tree_base_iterator& y)const{return _M_node!=y._M_node;}
};
template<class V,class Traits>struct _Rb_tree_iterator:public _Rb_tree_base_iterator {
    typedef typename Traits::reference reference;
    typedef typename Traits::pointer pointer;
    _Rb_tree_iterator(){_M_node=0;}
    _Rb_tree_iterator(_Rb_tree_node<V>* p){_M_node=p;}
    _Rb_tree_iterator(const _Rb_tree_iterator<V,_Nonconst_traits<V> >& p){_M_node=p._M_node;}
    reference operator*()const{return static_cast<_Rb_tree_node<V>*>(_M_node)->_M_value_field;}
    pointer operator->()const{return &operator*();}
#if defined(FABLE_RETAIL_QUEST_COMPLETION) || defined(FABLE_RETAIL_ENTITY_PERSIST)
    _Rb_tree_iterator& operator++(){this->_M_node=_Rb_global_inst::_M_increment(this->_M_node);return *this;}
#endif
};
template<class V,class A>struct _Rb_tree_base {
#if defined(FABLE_RETAIL_QUEST_COMPLETION) || defined(FABLE_RETAIL_ENTITY_PERSIST)
public:
#ifdef FABLE_RETAIL_CTHINGMANAGER_SAVE
    _Rb_tree_base(const A& a):_M_header(allocator<_Rb_tree_node<V> >(),(_Rb_tree_node<V>*)0){_M_header._M_data=_M_header.allocate(1);}
#else
    _Rb_tree_base(const A&);
#endif
    ~_Rb_tree_base();
#endif
protected:
    _STLP_alloc_proxy<_Rb_tree_node<V>*,_Rb_tree_node<V>,allocator<_Rb_tree_node<V> > > _M_header;
};
template<class K,class V,class KeyOfValue,class Compare,class A>
class _Rb_tree:public _Rb_tree_base<V,A> {
protected:
    size_t _M_node_count;
    Compare _M_key_compare;
public:
    typedef _Rb_tree_iterator<V,_Nonconst_traits<V> > iterator;
    typedef _Rb_tree_iterator<V,_Const_traits<V> > const_iterator;
#if defined(FABLE_RETAIL_QUEST_COMPLETION) || defined(FABLE_RETAIL_ENTITY_PERSIST)
#ifdef FABLE_RETAIL_CTHINGMANAGER_SAVE
    _Rb_tree(const Compare& c,const A& a):_Rb_tree_base<V,A>(a),_M_node_count(0),_M_key_compare(c){_M_empty_initialize();}
    _Rb_tree& operator=(const _Rb_tree&);
    size_t size() const { return _M_node_count; }
private:
    void _M_empty_initialize(){this->_M_header._M_data->_M_color=false;this->_M_header._M_data->_M_parent=0;this->_M_header._M_data->_M_left=this->_M_header._M_data;this->_M_header._M_data->_M_right=this->_M_header._M_data;}
public:
#else
    _Rb_tree(const Compare&,const A&);
#endif
    ~_Rb_tree();
    iterator begin(){return iterator(static_cast<_Rb_tree_node<V>*>(this->_M_header._M_data->_M_left));}
    const_iterator begin()const{return const_iterator(static_cast<_Rb_tree_node<V>*>(this->_M_header._M_data->_M_left));}
    iterator end(){return iterator(this->_M_header._M_data);}
#endif
    const_iterator find(const K&) const;
    const_iterator end()const{return const_iterator(this->_M_header._M_data);}
};
template<class K,class V,class Compare=less<K>,class A=allocator<pair<const K,V> > >
class map {
public:typedef pair<const K,V> value_type;
private:
    typedef _Rb_tree<K,value_type,_Select1st<value_type>,Compare,A> _Rep_type;
    _Rep_type _M_t;
public:
    typedef typename _Rep_type::const_iterator const_iterator;
#if defined(FABLE_RETAIL_QUEST_COMPLETION) || defined(FABLE_RETAIL_ENTITY_PERSIST)
    typedef typename _Rep_type::iterator iterator;
    explicit map(const Compare& c=Compare(),const A& a=A()):_M_t(c,a){}
    iterator begin(){return _M_t.begin();}
    const_iterator begin()const{return _M_t.begin();}
    iterator end(){return _M_t.end();}
    V& operator[](const K&);
#ifdef FABLE_RETAIL_CTHINGMANAGER_SAVE
    size_t size() const { return _M_t.size(); }
#endif
#endif
    const_iterator find(const K& key)const{return _M_t.find(key);}
    const_iterator end()const{return _M_t.end();}
};
}
#endif

class CBankFileAsyncEntry;
class CBankFileAsyncData;



class CEngineStaticMapFileBlockManager {
public:
    ~CEngineStaticMapFileBlockManager();
private:
    struct CFileBlock {
        int RefCount;
        bool LowPriority;
        CCountedPointer<CBankFileAsyncData> LoadingData;
    };
    CCountedPointer<CBankFileAsyncEntry> AsyncBankEntry;
    std::map<int,CFileBlock> Blocks;
};
#endif

#if defined(FABLE_RETAIL_CARRAY_STRIPIFIER)
#ifndef FABLE_ENGINE_CARRAY_H
#define FABLE_ENGINE_CARRAY_H

#ifndef FABLE_STLPORT46_STRIPIFIER_H
#define FABLE_STLPORT46_STRIPIFIER_H


#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <new>
namespace std {
struct __true_type{};struct __false_type{};
template<class T>struct __type_traits{typedef __false_type is_POD_type;typedef __false_type has_trivial_destructor;};
template<>struct __type_traits<int>{typedef __true_type is_POD_type;typedef __true_type has_trivial_destructor;};
template<>struct __type_traits<unsigned short>{typedef __true_type is_POD_type;typedef __true_type has_trivial_destructor;};
template<>struct __type_traits<bool>{typedef __true_type is_POD_type;typedef __true_type has_trivial_destructor;};
template<class T>class allocator{public:allocator(){};T*allocate(size_t n,const void* =0){return n?(T*)malloc(n*sizeof(T)):0;}void deallocate(T*p,size_t){if(p)free(p);}};
template<class V,class T,class A>class _STLP_alloc_proxy:public A{public:V _M_data;_STLP_alloc_proxy(const A&a,V p):A(a),_M_data(p){}};
template<class T>inline const T&(max)(const T&a,const T&b){return a<b?b:a;}
template<class T,class U>inline void _Construct(T*p,const U&x){new(p)T(x);}
template<class T>inline void _Destroy(T*p){p->~T();}
template<class I>inline void __destroy_aux(I a,I b,const __false_type&){for(;a!=b;++a)_Destroy(&*a);}
template<class I>inline void __destroy_aux(I,I,const __true_type&){}
template<class T>inline void _Destroy(T*a,T*b){__destroy_aux(a,b,typename __type_traits<T>::has_trivial_destructor());}
inline void*__copy_trivial(const void*a,const void*b,void*d){return b==a?d:(char*)memmove(d,a,(const char*)b-(const char*)a)+((const char*)b-(const char*)a);}
template<class T>inline T*__copy_ptrs(const T*a,const T*b,T*d,const __true_type&){return(T*)__copy_trivial(a,b,d);}
template<class T>inline T*__uninitialized_copy(const T*a,const T*b,T*d,const __true_type&){return(T*)__copy_trivial(a,b,d);}
template<class T>inline T*fill_n(T*p,size_t n,const T&x){for(;n>0;--n,++p)*p=x;return p;}
template<class T>inline T*__uninitialized_fill_n(T*p,size_t n,const T&x,const __true_type&){return fill_n(p,n,x);}
template<class T,class A>class _Vector_base{public:
 explicit _Vector_base(const A&a):_M_start(0),_M_finish(0),_M_end_of_storage(a,0){}
 _Vector_base(size_t n,const A&a):_M_start(0),_M_finish(0),_M_end_of_storage(a,0){_M_start=_M_end_of_storage.allocate(n);_M_finish=_M_start;_M_end_of_storage._M_data=_M_start+n;}
 ~_Vector_base(){if(_M_start)_M_end_of_storage.deallocate(_M_start,_M_end_of_storage._M_data-_M_start);}
protected:T*_M_start;T*_M_finish;_STLP_alloc_proxy<T*,T,A>_M_end_of_storage;};
template<class T,class A=allocator<T> >class vector:public _Vector_base<T,A>{
public:typedef T*iterator;typedef const T*const_iterator;typedef unsigned int size_type;typedef A allocator_type;typedef typename __type_traits<T>::is_POD_type _IsPODType;
 explicit vector(const A&a=A()):_Vector_base<T,A>(a){}
 vector(size_type n,const T&x,const A&a=A()):_Vector_base<T,A>(n,a){this->_M_finish=__uninitialized_fill_n(this->_M_start,n,x,_IsPODType());}
 ~vector(){_Destroy(this->_M_start,this->_M_finish);}
 iterator begin(){return this->_M_start;}const_iterator begin()const{return this->_M_start;}
 iterator end(){return this->_M_finish;}const_iterator end()const{return this->_M_finish;}
 size_type size()const{return size_type(this->_M_finish-this->_M_start);}
 T&back(){return *(end()-1);}const T&back()const{return *(end()-1);}
 void pop_back(){--this->_M_finish;_Destroy(this->_M_finish);}
 T&operator[](size_type n){return this->_M_start[n];}const T&operator[](size_type n)const{return this->_M_start[n];}
 void push_back(const T&x){if(this->_M_finish!=this->_M_end_of_storage._M_data){_Construct(this->_M_finish,x);++this->_M_finish;}else _M_insert_overflow(this->_M_finish,x,_IsPODType(),1,true);}
 iterator erase(iterator first,iterator last){iterator i=__copy_ptrs(last,this->_M_finish,first,_IsPODType());_Destroy(i,this->_M_finish);this->_M_finish=i;return first;}
 void clear(){erase(begin(),end());}
 void resize(size_type,const T&);
protected:
 void _M_clear(){_Destroy(this->_M_start,this->_M_finish);this->_M_end_of_storage.deallocate(this->_M_start,this->_M_end_of_storage._M_data-this->_M_start);}
 void _M_set(T*s,T*f,T*e){this->_M_start=s;this->_M_finish=f;this->_M_end_of_storage._M_data=e;}
 void _M_insert_overflow(T*position,const T&x,const __true_type&,size_type fill_len,bool at_end=false){
 const size_type old_size=size();const size_type len=old_size+(max)(old_size,fill_len);
 T*new_start=this->_M_end_of_storage.allocate(len);T*new_finish=(T*)__copy_trivial(this->_M_start,position,new_start);
 new_finish=fill_n(new_finish,fill_len,x);if(!at_end)new_finish=(T*)__copy_trivial(position,this->_M_finish,new_finish);
 _M_clear();_M_set(new_start,new_finish,new_start+len);
 }
};
}
#endif



template<class T>class CArray:public std::vector<T>{
public:
 CArray():std::vector<T>(){}
 explicit CArray(unsigned int n):std::vector<T>(n,T()){}
 CArray(unsigned int n,const T&x):std::vector<T>(n,x){}
 T&operator[](unsigned int n){return this->_M_start[n];}
 const T&operator[](unsigned int n)const{return this->_M_start[n];}
};
#endif
#else
#if defined(FABLE_RETAIL_CARRAY_FONT)
#ifndef FABLE_ENGINE_CARRAY_H
#define FABLE_ENGINE_CARRAY_H

template<class T> class CArray:public std::vector<T> {
public:
 T& operator[](unsigned int i){return this->_M_start[i];}
 T* GetAsCArray(){return this->size()>0?&this->front():GetZeroSizeArray();}
private:T* GetZeroSizeArray(){return (T*)this;}
};
#endif
#else
#if defined(FABLE_RETAIL_CARRAY_GEOMETRY)
#ifndef FABLE_ENGINE_CARRAY_H
#define FABLE_ENGINE_CARRAY_H



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

template<class T>class CArray:public std::vector<T>{
public:T&operator[](unsigned int i){return this->_M_start[i];}const T&operator[](unsigned int i)const{return this->_M_start[i];}
};
#endif
#else
#if defined(FABLE_RETAIL_CARRAY_DISPLAY)
#ifndef FABLE_ENGINE_CARRAY_H
#define FABLE_ENGINE_CARRAY_H

template<class T> class CArray : public std::vector<T> {};
#endif
#else
#if defined(FABLE_RETAIL_CARRAY)
#ifndef FABLE_ENGINE_CARRAY_H
#define FABLE_ENGINE_CARRAY_H



template<class T> class CArray : public std::vector<T> {
public:
    explicit CArray(unsigned int count) : std::vector<T>(count) {}
#ifdef FABLE_RETAIL_ENTITY_PERSIST
    CArray() : std::vector<T>() { }
    T& operator[](unsigned int n) { return this->_M_start[n]; }
    const T& operator[](unsigned int n) const { return this->_M_start[n]; }
    const T* GetAsCArray() const { return this->size()>0 ? &*this->begin() : (const T*)this; }
#endif
    T* GetAsCArray() { return this->size()>0 ? &this->front() : GetZeroSizeArray(); }
private:
    T* GetZeroSizeArray() { return (T*)this; }
};
#endif
#else
#ifndef FABLE_ENGINE_CARRAY_H
#define FABLE_ENGINE_CARRAY_H
#ifndef FABLE_STLPORT_CONTAINERS_H
#define FABLE_STLPORT_CONTAINERS_H



#include <stddef.h>
#if defined(FABLE_STLPORT_UI_ACTIONS) || defined(FABLE_STLPORT_CREATURE_POSITIONS) || defined(FABLE_STLPORT_CAMERA_POINTS) || defined(FABLE_RETAIL_ENTITY_PERSIST)
#include <new>
#endif
#include <stdlib.h>
#if defined(FABLE_STLPORT_VISIBLE_TRUE_OVERFLOW) || defined(FABLE_RETAIL_CTHINGMANAGER_BINARY)
#include <string.h>
#endif
#if defined(FABLE_STLPORT_VISIBLE_PUSH_BACK) || defined(FABLE_STLPORT_HTTP)
#include <new>
#include <string.h>
#endif
namespace std {
#ifdef FABLE_RETAIL_CTHINGMANAGER_BINARY
inline void fill(unsigned char* first,unsigned char* last,const unsigned char& value) {
    unsigned char tmp=value;memset(first,tmp,last-first);
}
template<class Size> inline unsigned char* fill_n(unsigned char* first,Size n,const unsigned char& value) {
    fill(first,first+n,value);return first+n;
}
#endif
struct __true_type { };
#ifdef FABLE_STLPORT_CAMERA_POINTS
struct __false_type { };
#endif
#ifdef FABLE_RETAIL_CTHINGMANAGER_BINARY
struct __false_type { };
template<class T> struct __type_traits { typedef __false_type is_POD_type; };
template<> struct __type_traits<unsigned char> { typedef __true_type is_POD_type; };
template<class Iterator> struct iterator_traits;
template<class T> struct iterator_traits<T*> { typedef T value_type; };
template<class ForwardIterator,class Size,class T>
inline ForwardIterator __uninitialized_fill_n(ForwardIterator first,Size n,const T& value,const __true_type&) {
    return std::fill_n(first,n,value);
}
template<class ForwardIterator,class Size,class T>
inline ForwardIterator uninitialized_fill_n(ForwardIterator first,Size n,const T& value) {
    typedef typename iterator_traits<ForwardIterator>::value_type Value;
    typedef typename __type_traits<Value>::is_POD_type IsPOD;
    return __uninitialized_fill_n(first,n,value,IsPOD());
}
#endif
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
#ifdef FABLE_RETAIL_QUEST_COMPLETION
template<class T> struct __type_traits;
template<class I> struct iterator_traits;
template<class T> struct iterator_traits<T*> {
    typedef random_access_iterator_tag iterator_category;
    typedef T value_type;
    typedef ptrdiff_t difference_type;
    typedef T* pointer;
    typedef T& reference;
};
template<class I> inline void __destroy_aux(I first,I last,const __false_type&) {
    for(;first!=last;++first) _Destroy(&*first);
}
template<class I> inline void __destroy_aux(I,I,const __true_type&) { }
template<class I,class T> inline void __destroy(I first,I last,T*) {
    typedef typename __type_traits<T>::has_trivial_destructor Trivial;
    __destroy_aux(first,last,Trivial());
}
template<class I> inline void _Destroy(I first,I last) {
    __destroy(first,last,(typename iterator_traits<I>::value_type*)0);
}
#else
template<class I> inline void _Destroy(I first,I last) { for(;first!=last;++first) _Destroy(&*first); }
#endif
template<class I,class O> inline O __uninitialized_copy(I first,I last,O result,const __false_type&) {
    O cur=result;for(;first!=last;++first,++cur) _Construct(&*cur,*first);return cur;
}
template<class I,class N,class T> inline I __uninitialized_fill_n(I first,N n,const T& x,const __false_type&) {
    I cur=first;for(;n>0;--n,++cur) _Construct(&*cur,x);return cur;
}
#endif
#ifdef FABLE_STLPORT_HTTP
template<class T> struct __type_traits { typedef __false_type has_trivial_assignment_operator; typedef __false_type has_trivial_destructor; };
template<> struct __type_traits<char> { typedef __true_type has_trivial_assignment_operator; };
#ifdef FABLE_RETAIL_QUEST_COMPLETION
template<> struct __type_traits<int> { typedef __true_type has_trivial_assignment_operator; typedef __true_type has_trivial_destructor; };
template<> struct __type_traits<unsigned int> { typedef __true_type has_trivial_assignment_operator; typedef __true_type has_trivial_destructor; };
#endif
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
#ifdef FABLE_STLPORT_CREATURE_POSITIONS
#ifndef FABLE_STLPORT_HTTP
template<class T> inline const T& (max)(const T& a,const T& b) { return a<b?b:a; }
template<class T> inline const T& (min)(const T& a,const T& b) { return b<a?b:a; }
struct __false_type { __false_type() { } };
struct input_iterator_tag { };
struct forward_iterator_tag : public input_iterator_tag { };
struct bidirectional_iterator_tag : public forward_iterator_tag { };
struct random_access_iterator_tag : public bidirectional_iterator_tag { random_access_iterator_tag() { } };
#endif
template<class Input,class Output,class Distance>
Output __fastcall __copy(Input,Input,Output,const random_access_iterator_tag&,Distance*);
template<class Input,class Output> inline Output __copy_ptrs(Input first,Input last,Output result,const __false_type&) {
    return __copy(first,last,result,random_access_iterator_tag(),(ptrdiff_t*)0);
}
#endif
#ifdef FABLE_RETAIL_ENTITY_PERSIST
template<class Input,class Output>
Output __fastcall __uninitialized_copy(Input,Input,Output,const __false_type&);
template<class Input,class Size,class T>
Input __fastcall __uninitialized_fill_n(Input,Size,const T&,const __false_type&);
template<class T,class U> inline void _Construct(T* p,const U& value) { new(p) T(value); }
template<class T> inline void _Destroy(T* p) { p->~T(); }
template<class I> inline void _Destroy(I first,I last) { for(;first!=last;++first) _Destroy(&*first); }
template<class T> inline const T& (max)(const T& a,const T& b) { return a<b?b:a; }
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
    void deallocate(_Tp* p, size_t) {
#ifdef FABLE_RETAIL_ENTITY_PERSIST
        if(p) free(p);
#else
        free(p);
#endif
    }
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
#if defined(FABLE_STLPORT_UI) || defined(FABLE_RETAIL_CTHINGMANAGER_BINARY)
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
#ifdef FABLE_RETAIL_CTHINGMANAGER_SAVE
    vector& operator=(const vector&);
#endif
#ifdef FABLE_RETAIL_CTHINGMANAGER_BINARY
    explicit vector(size_type n) : _Vector_base<_Tp,_Alloc>(n,_Alloc()) {
        this->_M_finish=uninitialized_fill_n(this->_M_start,n,_Tp());
    }
    _Tp& front() { return *this->_M_start; }
#endif
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
#ifdef FABLE_RETAIL_ENTITY_PERSIST
    void push_back(const _Tp& x) {
        if(this->_M_finish!=this->_M_end_of_storage._M_data) {
            _Construct(this->_M_finish,x);++this->_M_finish;
        } else _M_insert_overflow(this->_M_finish,x,__false_type(),1,true);
    }
protected:
    void _M_insert_overflow(iterator position,const _Tp& x,const __false_type&,size_type fill_len,bool at_end=false) {
        const size_type old_size=size();const size_type len=old_size+(max)(old_size,fill_len);
        _Tp* new_start=this->_M_end_of_storage.allocate(len);
        _Tp* new_finish=new_start;
        new_finish=__uninitialized_copy(this->_M_start,position,new_start,__false_type());
        if(fill_len==1) { _Construct(new_finish,x);++new_finish; }
        else new_finish=__uninitialized_fill_n(new_finish,fill_len,x,__false_type());
        if(!at_end) new_finish=__uninitialized_copy(position,this->_M_finish,new_finish,__false_type());
        _M_clear();_M_set(new_start,new_finish,new_start+len);
    }
    void _M_clear() {
        _Destroy(this->_M_start,this->_M_finish);
        this->_M_end_of_storage.deallocate(this->_M_start,this->_M_end_of_storage._M_data-this->_M_start);
    }
    void _M_set(_Tp* start,_Tp* finish,_Tp* end) {
        this->_M_start=start;this->_M_finish=finish;this->_M_end_of_storage._M_data=end;
    }
public:
#endif
    _Tp& operator[](size_type n) { return *(begin()+n); }
    const _Tp& operator[](size_type n) const { return *(begin()+n); }
#ifdef FABLE_STLPORT_HTTP
    typedef typename __type_traits<_Tp>::has_trivial_assignment_operator _IsPODType;
#ifdef FABLE_RETAIL_QUEST_COMPLETION
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
#ifndef FABLE_RETAIL_QUEST_COMPLETION
    __declspec(noinline)
#endif
    void _M_insert_overflow(iterator,const _Tp&,const __true_type&,size_type,bool);
public:
#endif
    void push_back(const _Tp& x) {
        if(this->_M_finish!=this->_M_end_of_storage._M_data) {
            _Construct(this->_M_finish,x); ++this->_M_finish;
        } else _M_insert_overflow(this->_M_finish,x,_IsPODType(),1,true);
    }
#ifdef FABLE_RETAIL_QUEST_COMPLETION
    iterator insert(iterator position,const _Tp& x) {
        size_type n=position-begin();
        if(this->_M_finish!=this->_M_end_of_storage._M_data) {
            if(position==end()) {
                _Construct(this->_M_finish,x);
                ++this->_M_finish;
            } else {
                _Construct(this->_M_finish,*(this->_M_finish-1));
                ++this->_M_finish;
                _Tp x_copy=x;
                __copy_backward_ptrs(position,this->_M_finish-2,this->_M_finish-1,_IsPODType());
                *position=x_copy;
            }
        } else _M_insert_overflow(position,x,_IsPODType(),1UL,false);
        return begin()+n;
    }
#endif
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
#ifndef FABLE_RETAIL_QUEST_COMPLETION
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
#ifndef FABLE_RETAIL_QUEST_COMPLETION
    __declspec(noinline)
#endif
    void _M_insert_overflow(iterator position,const _Tp& x,const __true_type&,size_type fillLength,bool atEnd) {
        const size_type oldSize=size();const size_type len=oldSize+(max)(oldSize,fillLength);
        _Tp* newStart=this->_M_end_of_storage.allocate(len);
        _Tp* newFinish=(_Tp*)__copy_trivial(this->_M_start,position,newStart);
        newFinish=fill_n(newFinish,fillLength,x);
        if(!atEnd) newFinish=(_Tp*)__copy_trivial(position,this->_M_finish,newFinish);
        _M_clear();_M_set(newStart,newFinish,newStart+len);
    }
#endif
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
#elif defined(FABLE_STLPORT_CREATURE_POSITIONS)
    
    void push_back(const _Tp& x) {
        if(this->_M_finish!=this->_M_end_of_storage._M_data) {
            new((void*)this->_M_finish) _Tp(x); ++this->_M_finish;
        } else _M_insert_overflow(this->_M_finish,x,__false_type(),1,true);
    }
    iterator erase(iterator first,iterator last) {
        iterator i=std::__copy_ptrs(last,this->_M_finish,first,__false_type());
        for(iterator d=i;d!=this->_M_finish;++d) d->~_Tp();
        this->_M_finish=i;
        return first;
    }
    void clear() { erase(begin(),end()); }
protected:
#ifdef FABLE_STLPORT_VISIBLE_POSITIONS_OVERFLOW
    
    void _M_insert_overflow(iterator position,const _Tp& value,const __false_type&,size_type fillLength,bool atEnd) {
        const size_type oldSize=size();
        const size_type length=oldSize+(oldSize<fillLength?fillLength:oldSize);
        _Tp* newStart=(_Tp*)malloc(length*sizeof(_Tp));
        _Tp* newFinish=newStart;
        for(iterator first=this->_M_start;first!=position;++first,++newFinish) new((void*)newFinish) _Tp(*first);
        for(size_type count=fillLength;count>0;--count,++newFinish) new((void*)newFinish) _Tp(value);
        if(!atEnd) for(iterator first=position;first!=this->_M_finish;++first,++newFinish) new((void*)newFinish) _Tp(*first);
        for(iterator first=this->_M_start;first!=this->_M_finish;++first) first->~_Tp();
        if(this->_M_start) free(this->_M_start);
        this->_M_start=newStart;this->_M_finish=newFinish;this->_M_end_of_storage._M_data=newStart+length;
    }
#else
    void _M_insert_overflow(iterator,const _Tp&,const __false_type&,size_type,bool);
#endif
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
#ifndef FABLE_RETAIL_ENTITY_PERSIST
    void push_back(const _Tp&);
#endif
#endif
};
#ifdef FABLE_RETAIL_QUEST_COMPLETION
template<class _Tp,class _Alloc> inline void vector<_Tp,_Alloc>::_M_insert_overflow(iterator position,const _Tp& x,const __true_type&,size_type fillLength,bool atEnd) {
        const size_type oldSize=size();const size_type len=oldSize+(max)(oldSize,fillLength);
        _Tp* newStart=this->_M_end_of_storage.allocate(len);
        _Tp* newFinish=(_Tp*)__copy_trivial(this->_M_start,position,newStart);
        newFinish=fill_n(newFinish,fillLength,x);
        if(!atEnd) newFinish=(_Tp*)__copy_trivial(position,this->_M_finish,newFinish);
        _M_clear();_M_set(newStart,newFinish,newStart+len);
    }
#endif
template<class _T1,class _T2> struct pair {
    typedef _T1 first_type;
    typedef _T2 second_type;
    _T1 first;
    _T2 second;
#if defined(FABLE_STLPORT_CAMERA_POINTS) || defined(FABLE_RETAIL_QUEST_COMPLETION)
    pair():first(),second(){}
    pair(const _T1& a,const _T2& b):first(a),second(b) { }
    pair(const pair& p):first(p.first),second(p.second) { }
#endif
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
#if defined(FABLE_STLPORT_UI_CHANGE_STATE) || defined(FABLE_STLPORT_CAMERA_POINTS)
    _List_iterator(_List_node<_Tp>* x) : _List_iterator_base(x) { }
#else
    _List_iterator(_List_node_base* x) : _List_iterator_base(x) { }
#endif
    _List_iterator() { }
#if defined(FABLE_STLPORT_UI_CHANGE_STATE) || defined(FABLE_STLPORT_CAMERA_POINTS)
    _List_iterator(const iterator& x) : _List_iterator_base(x._M_node) { }
#endif
    reference operator*() const { return ((_List_node<_Tp>*)_M_node)->_M_data; }
    pointer operator->() const { return &operator*(); }
    _List_iterator& operator++() { this->_M_incr(); return *this; }
#ifdef FABLE_STLPORT_CAMERA_POINTS
    _List_iterator operator++(int) { _List_iterator tmp=*this; this->_M_incr(); return tmp; }
#endif
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
#ifdef FABLE_STLPORT_CAMERA_POINTS
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
#else
    void clear();
#endif
};
template<class _Tp,class _Alloc=allocator<_Tp> > class list : public _List_base<_Tp,_Alloc> {
public:
#ifdef FABLE_STLPORT_CONSTRUCTORS
    explicit list(const _Alloc& a=_Alloc()) : _List_base<_Tp,_Alloc>(a) { }
#endif
    typedef _List_iterator<_Tp,_Nonconst_traits<_Tp> > iterator;
    typedef _List_iterator<_Tp,_Const_traits<_Tp> > const_iterator;
#if defined(FABLE_STLPORT_UI_CHANGE_STATE) || defined(FABLE_STLPORT_CAMERA_POINTS)
    iterator begin() { return iterator((_List_node<_Tp>*)this->_M_node._M_data->_M_next); }
    const_iterator begin() const { return const_iterator((_List_node<_Tp>*)this->_M_node._M_data->_M_next); }
#else
    iterator begin() { return iterator(this->_M_node._M_data->_M_next); }
    const_iterator begin() const { return const_iterator(this->_M_node._M_data->_M_next); }
#endif
    iterator end() { return iterator(this->_M_node._M_data); }
    const_iterator end() const { return const_iterator(this->_M_node._M_data); }
    bool empty() const { return this->_M_node._M_data->_M_next==this->_M_node._M_data; }
#ifdef FABLE_RETAIL_CAMERA_TRANSITIONS
    _Tp& front() { return *begin(); }
    const _Tp& front() const { return *begin(); }
#endif
#ifdef FABLE_STLPORT_CAMERA_POINTS
    _Alloc get_allocator() const { return _Alloc(); }
    list(const list& x):_List_base<_Tp,_Alloc>(x.get_allocator()) {
        insert(begin(),x.begin(),x.end());
    }
    ~list() { }
    template<class Input> void insert(iterator p,Input first,Input last) {
        _M_insert_dispatch(p,first,last,__false_type());
    }
    template<class Input> __declspec(noinline) void _M_insert_dispatch(iterator position,Input first,Input last,const __false_type&) {
        for(;first!=last;++first) insert(position,*first);
    }
#endif
#if defined(FABLE_STLPORT_UI_ACTIONS) || defined(FABLE_STLPORT_CAMERA_POINTS)
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
    typedef unsigned int _WordT;
    _WordT _M_w[_Nw];
    static size_t _S_whichword(size_t n) { return n/(8*sizeof(_WordT)); }
    static size_t _S_whichbit(size_t n) { return n%(8*sizeof(_WordT)); }
    static _WordT _S_maskbit(size_t n) { return _WordT(1)<<_S_whichbit(n); }
    _WordT _M_getword(size_t n) const { return _M_w[_S_whichword(n)]; }
};
void __stl_throw_out_of_range(const char*);
template<size_t _Nb> class bitset : public _Base_bitset<(_Nb+8*sizeof(unsigned int)-1)/(8*sizeof(unsigned int))> {
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



template<class T> class CArray : public std::vector<T> {
public:
    T& operator[](unsigned int i) { return this->_M_start[i]; }
    const T& operator[](unsigned int i) const { return this->_M_start[i]; }
};
#endif

#endif

#endif

#endif

#endif

#endif

#ifndef FABLE_ENGINE_C2DCOORDI_H
#define FABLE_ENGINE_C2DCOORDI_H
#define FABLE_CDECL __cdecl
#define FABLE_STDCALL __stdcall
#define FABLE_FASTCALL __fastcall
#define FABLE_THISCALL
typedef signed char fable_i8;typedef unsigned char fable_u8;typedef short fable_i16;typedef unsigned short fable_u16;typedef int fable_i32;typedef unsigned int fable_u32;


class C2DCoordI { public: int X,Y;C2DCoordI(){}C2DCoordI(int x,int y):X(x),Y(y){} };
#endif

class CEngineLandscapeMap;
class CLocalDetailCacheMap;
class CIDrawMap;
class CEngineStaticMapEdgeHeights;
class CDataInputStream;




class CEngineMap {
    friend int main();
public:
    enum STATIC_MAP_OPEN_STATE {
        STATIC_MAP_OPEN_STATE_CLOSED=0,
        STATIC_MAP_OPEN_STATE_OPEN_FOR_USE=1,
        STATIC_MAP_OPEN_STATE_OPEN_FOR_UPDATE=2
    };
    CEngineMap(const CIDrawMap*,int,int,int,int,int,CCharString,bool);
    ~CEngineMap();
private:
    
    
    struct CStaticMapInfoBlock {
        int VersionID,BankFileIndex,EdgeHeightFilePtr,EdgeHeightFileSize;
        int LandscapeMapPtr,LocalDetailMapPtr,ChecksumBlockFilePtr,ChecksumBlockFileSize;
        int MapWidth,MapHeight,WorldX,WorldY,Quality,ShorePointArraySize,ShorePointArrayStart;
        unsigned int LevelChecksum;
        C3DBoundingBox CameraMapBounds;
        int HeaderEndPtr;
    };
    struct GuardedBlock {unsigned int before;CStaticMapInfoBlock block;unsigned int after;};
    static void __fastcall ReadMapInfoBlock(CStaticMapInfoBlock&,CDataInputStream&);
    CEngineLandscapeMap* LandscapeMap;
    CLocalDetailCacheMap* LocalDetailCacheMap;
    const CIDrawMap* GameMap;
    C3DVector MapWorldPosition;
    int MapUID;
    CCharString StaticMapName;
    STATIC_MAP_OPEN_STATE StaticMapOpenState;
    C3DBoundingBox CameraMapBounds;
    CCountedPointer<CBankFileAsyncEntry> AsyncFileEntry;
    CEngineStaticMapFileBlockManager FileBlockManager;
    std::list<CEngineMap*>::iterator VisibleMapListIterator;
    unsigned short MapWidth,MapHeight,WorldPosX,WorldPosY;
    bool IsInVisibleMapList;
    CEngineStaticMapEdgeHeights* EdgeHeights;
    CArray<C2DCoordI> ShorePoints;
    bool ShorePointsLoadedForThisMap,MapChecksumPassed,DefChecksumPassed,NeedsRebuild;
    int EdgeHeightFilePtr,EdgeHeightFileSize,ShorePointFilePtr,ShorePointFileSize;
    CArray<int> StaticMapGenerationVisibilityInfoObservingMaps;
};
#endif

#if defined(FABLE_RETAIL_CDATAINPUTSTREAM)
#ifndef FABLE_ENGINE_CDATAINPUTSTREAM_H
#define FABLE_ENGINE_CDATAINPUTSTREAM_H
#ifndef FABLE_RETAIL_HEADER_CDATASTREAM
#define FABLE_RETAIL_HEADER_CDATASTREAM
#ifndef FABLE_ENGINE_CBASECLASSNONCOPYABLE_H
#define FABLE_ENGINE_CBASECLASSNONCOPYABLE_H



#define FABLE_CDECL __cdecl
#define FABLE_STDCALL __stdcall
#define FABLE_FASTCALL __fastcall
#define FABLE_THISCALL
typedef signed char fable_i8;typedef unsigned char fable_u8;typedef short fable_i16;typedef unsigned short fable_u16;typedef int fable_i32;typedef unsigned int fable_u32;

#ifndef FABLE_ENGINE_CBASECLASS_H
#define FABLE_ENGINE_CBASECLASS_H







#define FABLE_CDECL __cdecl
#define FABLE_STDCALL __stdcall
#define FABLE_FASTCALL __fastcall
#define FABLE_THISCALL
typedef signed char fable_i8;typedef unsigned char fable_u8;typedef short fable_i16;typedef unsigned short fable_u16;typedef int fable_i32;typedef unsigned int fable_u32;


class CBaseClass {
public:
    CBaseClass();                                                        
    virtual ~CBaseClass();                                               
};

#endif 


class CBaseClassNonCopyable : public CBaseClass {
protected:
    CBaseClassNonCopyable();
    virtual ~CBaseClassNonCopyable();                                    
private:
    CBaseClassNonCopyable(const CBaseClassNonCopyable&);
    CBaseClassNonCopyable& operator=(const CBaseClassNonCopyable&);
};

#endif 

class CDataStream : public CBaseClassNonCopyable {
public:
    virtual ~CDataStream();
    virtual void SetPosition(unsigned int)=0;
    virtual unsigned int GetPosition() const=0;
    virtual void Close()=0;
};
#endif

#include <string.h>
#pragma intrinsic(memcpy)
class CCharString;
class CWideString;

class CDataInputStream : public CDataStream {
    friend int main();
public:
#ifdef FABLE_RETAIL_CTHINGMANAGER_BINARY
    CDataInputStream();
#else
    virtual ~CDataInputStream();
#endif
    virtual void Close();
    unsigned int GetLength() const { return StreamSize; }
    virtual unsigned int GetPosition() const { return StreamPos; }
    virtual void SetPosition(unsigned int);
    inline void Read(void* buffer,int size) {
        if(size<=0)return;
        FableI64 newPosition=(FableI64)(StreamPos+size);
#ifdef FABLE_RETAIL_CENGINEMAP_READ
        bool valid=newPosition>=0 && newPosition<=0x7fffffff;
        if(!valid)return;
#else
        if(newPosition<0 || newPosition>0x7fffffff)return;
#endif
        if(size<=SourceChunkBytesRemaining) {
            memcpy(buffer,CurrentSourcePtr,size);
            CurrentSourcePtr+=size;
            SourceChunkBytesRemaining-=size;
            StreamPos+=size;
        } else ReadWithSrcChunkOverflow((unsigned char*)buffer,size);
    }
    template<class T> T ReadType() { T val; Read(&val,sizeof(T)); return val; }
    inline unsigned int ReadULONG() { return ReadType<unsigned int>(); }
    inline int ReadSLONG() { return ReadType<int>(); }
    virtual void ReadCompressed(void*,int);
    virtual void ReadZlibCompressed(void*,int);
    virtual CCharString ReadNullTerminatedString();
    virtual CWideString ReadNullTerminatedWString();
#ifdef FABLE_RETAIL_CTHINGMANAGER_BINARY
protected:
    void SetPositionAssumeWithinSourceChunk(unsigned int);
#endif
private:
    void ReadWithSrcChunkOverflow(unsigned char*,int);
    virtual void InternalReadNextSourceChunk(const unsigned char*&,int&);
    virtual bool NeedsBufferedRead(int);
    virtual void InternalReadUnbuffered(unsigned char*,int);
    unsigned int StreamPos;
    int StreamSize;
    const unsigned char* CurrentSourcePtr;
    unsigned int SourceChunkStreamPos;
    int SourceChunkBytesRemaining;
};
#endif
#else
#ifndef FABLE_ENGINE_CDATAINPUTSTREAM_H
#define FABLE_ENGINE_CDATAINPUTSTREAM_H





#include <stddef.h>
#define FABLE_CDECL __cdecl
#define FABLE_STDCALL __stdcall
#define FABLE_FASTCALL __fastcall
#define FABLE_THISCALL
typedef signed char fable_i8;typedef unsigned char fable_u8;typedef short fable_i16;typedef unsigned short fable_u16;typedef int fable_i32;typedef unsigned int fable_u32;


#pragma pack(push, 1)
class CDataInputStream {
public:
    void*         __vftable;                                 
    unsigned int StreamPos;                                 
    int          StreamSize;                                
    void*         CurrentSourcePtr;                          
    unsigned int SourceChunkStreamPos;                      
    int          SourceChunkBytesRemaining;                 
#ifdef FABLE_CDATAINPUTSTREAM_FRIEND
    friend FABLE_CDATAINPUTSTREAM_FRIEND;                   
#endif
private:
    void ReadWithSrcChunkOverflow(unsigned char* buffer, int size); 
};
#pragma pack(pop)


#endif 

#endif

void __fastcall CEngineMap::ReadMapInfoBlock(CStaticMapInfoBlock& block,CDataInputStream& ipstream)
{
    { int val; ipstream.Read(&val,sizeof(val)); block.VersionID=val; }
    { int val; ipstream.Read(&val,sizeof(val)); block.BankFileIndex=val; }
    { int val; ipstream.Read(&val,sizeof(val)); block.EdgeHeightFileSize=val; }
    { int val; ipstream.Read(&val,sizeof(val)); block.MapWidth=val; }
    { int val; ipstream.Read(&val,sizeof(val)); block.MapHeight=val; }
    { int val; ipstream.Read(&val,sizeof(val)); block.WorldX=val; }
    { int val; ipstream.Read(&val,sizeof(val)); block.WorldY=val; }
    { int val; ipstream.Read(&val,sizeof(val)); block.EdgeHeightFilePtr=val; }
    { int val; ipstream.Read(&val,sizeof(val)); block.LandscapeMapPtr=val; }
    { int val; ipstream.Read(&val,sizeof(val)); block.LocalDetailMapPtr=val; }
    { int val; ipstream.Read(&val,sizeof(val)); block.Quality=val; }
    { int val; ipstream.Read(&val,sizeof(val)); block.ShorePointArraySize=val; }
    { int val; ipstream.Read(&val,sizeof(val)); block.ShorePointArrayStart=val; }
    { unsigned int val; ipstream.Read(&val,sizeof(val)); block.LevelChecksum=val; }
    { int val; ipstream.Read(&val,sizeof(val)); block.ChecksumBlockFilePtr=val; }
    { int val; ipstream.Read(&val,sizeof(val)); block.ChecksumBlockFileSize=val; }
    ipstream.Read(&block.CameraMapBounds,sizeof(block.CameraMapBounds));
    { int val; ipstream.Read(&val,sizeof(val)); block.HeaderEndPtr=val; }
}

// These virtual return types are never instantiated by this fixture.
// The original wide string has one protected pointer, just like its PDB type.
class CWideStringData;
class CWideString { protected:CWideStringData* PStringData; };
CBaseClass::CBaseClass(){}
CBaseClass::~CBaseClass(){}
CBaseClassNonCopyable::CBaseClassNonCopyable(){}
CBaseClassNonCopyable::~CBaseClassNonCopyable(){}
CDataStream::~CDataStream(){}
CDataInputStream::~CDataInputStream(){}
void CDataInputStream::Close(){abort();}
void CDataInputStream::SetPosition(unsigned int){abort();}
void CDataInputStream::ReadCompressed(void*,int){abort();}
void CDataInputStream::ReadZlibCompressed(void*,int){abort();}
CCharString CDataInputStream::ReadNullTerminatedString(){abort();}
CWideString CDataInputStream::ReadNullTerminatedWString(){abort();}
void CDataInputStream::InternalReadNextSourceChunk(const unsigned char*&,int&){abort();}
bool CDataInputStream::NeedsBufferedRead(int){abort();}
void CDataInputStream::InternalReadUnbuffered(unsigned char*,int){abort();}
static unsigned char input_data[160];
static unsigned int input_end,refill_size,slow_calls;
void CDataInputStream::ReadWithSrcChunkOverflow(unsigned char* dst,int bytes){
 assert(bytes>0&&StreamPos+(unsigned int)bytes<=input_end);
 memcpy(dst,input_data+StreamPos,bytes);
 StreamPos+=(unsigned int)bytes;
 CurrentSourcePtr=input_data+StreamPos;
 SourceChunkStreamPos=StreamPos;
 unsigned int left=input_end-StreamPos;
 SourceChunkBytesRemaining=(int)(left<refill_size?left:refill_size);
 ++slow_calls;
}
int main(){
 assert(sizeof(CEngineMap::CStaticMapInfoBlock)==92);
 const unsigned int offsets[17]={0,4,12,32,36,40,44,8,16,20,48,52,56,60,24,28,88};
 const float coordinates[6]={-8.0f,-4.0f,0.0f,4.0f,8.0f,16.0f};
 unsigned int cases=0;
 for(unsigned int pattern=0;pattern<3;++pattern)
 for(unsigned int prefix_index=0;prefix_index<2;++prefix_index)
 for(unsigned int split=0;split<=96;++split){
  unsigned int prefix_bytes=prefix_index?7:0;
  memset(input_data,0xa7,sizeof(input_data));
  unsigned char expected[92];memset(expected,0,sizeof(expected));
  unsigned int cursor=prefix_bytes;
  for(unsigned int i=0;i<16;++i){
   unsigned int value=pattern==0?i+1:pattern==1?0x80000000u+i:0xffffffffu-i*17;
   memcpy(input_data+cursor,&value,4);cursor+=4;
   memcpy(expected+offsets[i],&value,4);
  }
  memcpy(input_data+cursor,coordinates,24);cursor+=24;
  memcpy(expected+64,coordinates,24);
  unsigned int tail=0x12345678u+pattern;
  memcpy(input_data+cursor,&tail,4);cursor+=4;
  memcpy(expected+88,&tail,4);
  input_end=cursor;refill_size=split?split:3;slow_calls=0;
  CDataInputStream stream;
  stream.StreamPos=prefix_bytes;stream.StreamSize=input_end;
  stream.CurrentSourcePtr=input_data+prefix_bytes;
  stream.SourceChunkStreamPos=prefix_bytes;
  stream.SourceChunkBytesRemaining=(int)split;
  CEngineMap::GuardedBlock destination;memset(&destination,0xcc,sizeof(destination));
  destination.before=0x1234abcd;destination.after=0x98765432;
  CEngineMap::ReadMapInfoBlock(destination.block,stream);
  assert(memcmp(&destination.block,expected,92)==0);
  assert(destination.before==0x1234abcd&&destination.after==0x98765432);
  assert(stream.StreamPos==prefix_bytes+92);
  if(split>=92)assert(slow_calls==0);else assert(slow_calls>0);
  for(unsigned int k=0;k<prefix_bytes;++k)assert(input_data[k]==0xa7);
  for(unsigned int k=input_end;k<sizeof(input_data);++k)assert(input_data[k]==0xa7);
  ++cases;
 }
 assert(cases==582);
 puts("00b3efa0_TEST PASS");return 0;
}
