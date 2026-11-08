#include "fable_compiler.h"
class COnHitFixture;
#ifndef _MSC_VER
#define __cdecl
#define __fastcall
#define __fastcall
#define __forceinline inline
#define __declspec(x)
#define __int64 long
#endif
#define FABLE_RETAIL_CTHING_CONSTRUCTORS
#define FABLE_STLPORT_CONSTRUCTORS
#define FABLE_RETAIL_DEF_CONSTRUCTORS
#define FABLE_ONHIT_TEST_ACCESS
#include <new>
#pragma once



#if defined(_MSC_VER)
#  define FABLE_CDECL __cdecl
#  define FABLE_STDCALL __stdcall
#  define FABLE_FASTCALL __fastcall
#  define FABLE_THISCALL __fastcall
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
    typedef char FABLE_JOIN(fable_sassert_, __LINE__)[(expression) ? 1 : -1]
#else
#define FABLE_STATIC_ASSERT(expression) typedef char FABLE_JOIN(_test_size_,__LINE__)[1]
#endif

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
#if defined(FABLE_RETAIL_CAMERA_DRAW) || defined(FABLE_RETAIL_HIT_EFFECT)
inline float __fastcall GFSqr(float n){return n*n;}
#endif
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
#if defined(FABLE_RETAIL_NAVIGATION_UPDATE) || defined(FABLE_RETAIL_VALIDPOSITIONS)
#ifndef FABLE_ENGINE_C2DCOORDI_H
#define FABLE_ENGINE_C2DCOORDI_H


class C2DCoordI { public: long X,Y;C2DCoordI(){}C2DCoordI(long x,long y):X(x),Y(y){} };
FABLE_STATIC_ASSERT(sizeof(C2DCoordI)==8);
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
#ifdef FABLE_RETAIL_VECTOR_COPY_INITIALISE
    C3DVector(const C3DVector& p){Initialise(p.X,p.Y,p.Z);}
#elif defined(FABLE_RETAIL_VECTOR_GEOMETRY) || defined(FABLE_RETAIL_VECTOR_SPLINE_COPY)
    C3DVector(const C3DVector& p){*this=p;}
#else
    C3DVector(const C3DVector& other):X(other.X),Y(other.Y),Z(other.Z){}
#endif
    FABLE_VECTOR_INLINE void Initialise(float x,float y,float z){X=x;Y=y;Z=z;}
    FABLE_VECTOR_INLINE const C3DVector operator-(const C3DVector& other)const{return C3DVector(X-other.X,Y-other.Y,Z-other.Z);}
    FABLE_VECTOR_INLINE const C3DVector operator+(const C3DVector& other)const{return C3DVector(X+other.X,Y+other.Y,Z+other.Z);}
    FABLE_VECTOR_INLINE const C3DVector operator*(float scale)const{return C3DVector(X*scale,Y*scale,Z*scale);}
    FABLE_VECTOR_INLINE bool operator==(const C3DVector& other)const{return X==other.X&&Y==other.Y&&Z==other.Z;}
#ifdef FABLE_RETAIL_ANIMATION_SEQUENCE
    FABLE_VECTOR_INLINE float GetSquaredMagnitude()const{return Z*Z+Y*Y+X*X;}
#else
    FABLE_VECTOR_INLINE float GetSquaredMagnitude()const{return X*X+Z*Z+Y*Y;}
#endif
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
#if defined(FABLE_RETAIL_NAVIGATION_UPDATE) || defined(FABLE_RETAIL_VALIDPOSITIONS)
    inline C2DCoordI ToC2DCoordI() const { return C2DCoordI((long)X,(long)Y); }
#endif
    C3DVector& AccurateNormalise();
    void Rotate(const C3DVector&,float);
};
#undef FABLE_VECTOR_INLINE
#endif

#ifndef FABLE_ENGINE_CCHARSTRING_H
#define FABLE_ENGINE_CCHARSTRING_H


#if defined(FABLE_RETAIL_NAVIGATION_UPDATE) || defined(FABLE_RETAIL_FACTIONS) || defined(FABLE_RETAIL_SCOREBOARD) || defined(FABLE_RETAIL_PERSIST_OPINION) || defined(FABLE_RETAIL_SUBTITLE) || defined(FABLE_RETAIL_MINIMAP_TRAITS) || defined(FABLE_RETAIL_CTHINGMANAGER_SAVE) || defined(FABLE_RETAIL_RELATIVE_PATH) || defined(FABLE_RETAIL_REGION_GRAPH) || defined(FABLE_RETAIL_SHAPE_PERSIST) || defined(FABLE_RETAIL_ANIMATION_SEQUENCE)
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
#if defined(FABLE_RETAIL_NAVIGATION_UPDATE) || defined(FABLE_RETAIL_MINIMAP_TRAITS) || defined(FABLE_RETAIL_RELATIVE_PATH) || defined(FABLE_RETAIL_SHAPE_PERSIST)
    static long __fastcall Compare(const T*,const T*);
#endif
public:
    const T* PeekData() const { return PData; }
#ifdef FABLE_RETAIL_FACTIONS
    bool operator<(const CBasicString&)const;
#endif
#if defined(FABLE_RETAIL_NAVIGATION_UPDATE) || defined(FABLE_RETAIL_RELATIVE_PATH) || defined(FABLE_RETAIL_SHAPE_PERSIST)
    bool operator==(const CBasicString& other) const { if(StringLength!=other.StringLength)return false; return !Compare(PData,other.PData); }
#endif
#if defined(FABLE_RETAIL_NAVIGATION_UPDATE) || defined(FABLE_RETAIL_SUBTITLE) || defined(FABLE_RETAIL_FACTIONS) || defined(FABLE_RETAIL_ANIMATION_SEQUENCE)
    unsigned long GetLength() const { return StringLength; }
#endif
#if defined(FABLE_RETAIL_NAVIGATION_UPDATE) || defined(FABLE_RETAIL_PERSIST_OPINION) || defined(FABLE_RETAIL_CTHINGMANAGER_SAVE) || defined(FABLE_RETAIL_MINIMAP_TRAITS) || defined(FABLE_RETAIL_REGION_GRAPH)
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
#ifdef FABLE_RETAIL_INVENTORY_MAP_SERIALISE
class CWideString;
#endif
#ifdef FABLE_RETAIL_FACTIONS
class CCharString;
#ifndef FABLE_ENGINE_CCRC_H
#define FABLE_ENGINE_CCRC_H


class CCharString;
extern const unsigned long CRC32TAB[];
class CCRC {
public:
#ifndef FABLE_RETAIL_FACTIONS
    static unsigned long __fastcall Calc(unsigned long,const void*,unsigned long);
#else
    static unsigned long __fastcall Calc(unsigned long seed,const void* data,unsigned long length) {
        unsigned long result=seed;
        if(length!=0)for(unsigned long index=0;index<length;++index)
            result=(result>>8)^CRC32TAB[((const unsigned char*)data)[index]^(result&255)];
        return result;
    }
 #endif
    static unsigned long __fastcall Calc(const void* data,unsigned long length){return Calc(0,data,length);}
    static unsigned long __fastcall CalcFromString(const CCharString&);
    static unsigned long __fastcall CalcFromPathname(const CCharString&);
};
#endif

#endif
class CCharString {
public:
#ifdef FABLE_RETAIL_FACTIONS
    const char* GetAsCharArray()const{return PStringData?PStringData->Data.PeekData():&NullChar;}
    long GetLength()const{return PStringData?PStringData->Data.GetLength():0;}
    unsigned long GetCRC()const{return CCRC::Calc(0,GetAsCharArray(),GetLength());}
    bool operator<(const CCharString& other)const{
        if(PStringData==other.PStringData)return false;
        if(!other.PStringData)return false;
        if(!PStringData)return true;
        return PStringData->Data<other.PStringData->Data;
    }
#endif
#ifdef FABLE_RETAIL_INVENTORY_MAP_SERIALISE
    CWideString ToWideString() const;
#endif
#ifdef FABLE_RETAIL_NAVIGATION_UPDATE
    long GetLength()const{return PStringData?PStringData->Data.GetLength():0;}
    CCharString GetSubString(unsigned long,unsigned long)const;
    bool operator==(const char* text)const {if(!PStringData){if(strcmp(text,"")==0)return true;return false;}return PStringData->Data==text;}
    bool operator==(const CCharString& other)const {
        if(PStringData==other.PStringData)return true;
        if(!PStringData||!other.PStringData)return false;
        return PStringData->Data==other.PStringData->Data;
    }
    bool operator!=(const CCharString&)const;
    const CCharString& operator=(const CCharString&);
#endif
    CCharString();
    CCharString(const char* text, long length = -1);
    CCharString(const CCharString& other);
#ifdef FABLE_RETAIL_MINIMAP_TRAITS
    const CCharString& operator=(const CCharString&);
    bool operator!=(const char* text) const { return !(*this==text); }
#endif
#if defined(FABLE_RETAIL_SCOREBOARD) || defined(FABLE_RETAIL_SUBTITLE) || defined(FABLE_RETAIL_ANIMATION_SEQUENCE)
#if defined(FABLE_RETAIL_SUBTITLE) || defined(FABLE_RETAIL_ANIMATION_SEQUENCE)
    const char* GetAsCharArray() const { return PStringData==0 ? &NullChar : PStringData->Data.PeekData(); }
#else
    const char* GetAsCharArray() const { const char* result=&NullChar; if(PStringData) result=PStringData->Data.PeekData(); return result; }
#endif
#endif
#if defined(FABLE_RETAIL_EFFECT_LOOKUP) || defined(FABLE_RETAIL_CTHINGMANAGER_SAVE)
    unsigned long GetCRC() const; 
#endif
#if defined(FABLE_RETAIL_PERSIST_OPINION) || defined(FABLE_RETAIL_MINIMAP_TRAITS)
    bool EqualsCaseInsensitive(const CCharString&) const;
    bool operator==(const char* text) const;
#endif
#ifdef FABLE_RETAIL_ANIMATION_SEQUENCE
    long GetLength() const { return PStringData==0 ? 0 : PStringData->Data.GetLength(); }
#endif
#ifdef FABLE_RETAIL_SUBTITLE
    long GetLength() const { return PStringData==0 ? 0 : PStringData->Data.GetLength(); }
    inline bool StartsWith(const CCharString& str) const { return strncmp(GetAsCharArray(),str.GetAsCharArray(),str.GetLength())==0; }
    inline bool EndsWith(const CCharString& str) const { long offset=GetLength()-str.GetLength();if(offset>=0)return strcmp(GetAsCharArray()+offset,str.GetAsCharArray())==0;return false; }
#endif
#ifdef FABLE_RETAIL_CTHINGMANAGER_SAVE
    bool operator==(const char* text) const { if(PStringData) return PStringData->Data==text; return strcmp("",text)==0; }
    bool operator!=(const char* text) const { if(!PStringData) return strcmp("",text)!=0; return !(PStringData->Data==text); }
    const CCharString& operator+=(const CCharString&);
    const CCharString& operator+=(const char*);
    const CCharString& operator=(const char*);
#endif
#if defined(FABLE_RETAIL_RELATIVE_PATH) || defined(FABLE_RETAIL_SHAPE_PERSIST)
    operator const char*() const;
    bool operator==(const CCharString& other) const {
        if(PStringData==other.PStringData)return true;
        if(!PStringData||!other.PStringData)return false;
        return PStringData->Data==other.PStringData->Data;
    }
    bool operator!=(const CCharString&) const;
    bool operator!=(const char*) const;
    const CCharString& operator=(const char*);
    const CCharString& operator=(const CCharString&);
    const CCharString& operator+=(const char*);
    const CCharString& operator+=(const CCharString&);
#endif
#ifdef FABLE_RETAIL_REGION_GRAPH
    bool operator==(const char* text) const {if(!PStringData){if(strcmp(text,"")==0)return true;return false;}return PStringData->Data==text;}
#endif
    ~CCharString();
protected:
#ifdef FABLE_RETAIL_INVENTORY_MAP_SERIALISE
    void UnassignString();
    static long NoStrings;
#endif
#if defined(FABLE_RETAIL_FACTIONS) || defined(FABLE_RETAIL_SCOREBOARD) || defined(FABLE_RETAIL_SUBTITLE) || defined(FABLE_RETAIL_ANIMATION_SEQUENCE)
    static const char NullChar;
#endif
    CCharStringData* PStringData;
};
#endif

#ifndef FABLE_ENGINE_CDEFSTRING_H
#define FABLE_ENGINE_CDEFSTRING_H


class CCharString;
#if defined(FABLE_RETAIL_SHOP) || defined(FABLE_RETAIL_FACTIONS)
#ifndef FABLE_ENGINE_CDEFSTRINGTABLE_H
#define FABLE_ENGINE_CDEFSTRINGTABLE_H




class CDefStringTable {
public:
    CCharString GetString(long) const;
#ifdef FABLE_RETAIL_FACTIONS
    unsigned long GetStringCRC(long)const;
#endif
private:
    unsigned char StringMap[12]; 
    unsigned char StringStream[24]; 
    unsigned long StringCount,SharedCount,RandomID;
    unsigned char TableFilename[4]; 
    bool Locked,ReadOnly;
};
FABLE_STATIC_ASSERT(sizeof(CDefStringTable)==0x38);
extern CDefStringTable GDefStringTable;
#endif

#endif
class CDefString {
public:
 CDefString():TablePos(-1){}
 CDefString(const CCharString&);
#if defined(FABLE_RETAIL_SHOP) || defined(FABLE_RETAIL_FACTIONS)
#ifdef FABLE_RETAIL_FACTIONS
 unsigned long GetCRC()const{return GDefStringTable.GetStringCRC(TablePos);}
#endif
 operator CCharString()const{return GDefStringTable.GetString(TablePos);}
 CCharString GetString()const{return GDefStringTable.GetString(TablePos);}
#endif
#ifdef FABLE_RETAIL_HIT_EFFECT
 CCharString GetString() const;
#endif
private:
 long TablePos;
};
#endif

#ifndef FABLE_ENGINE_CTBASESINGLETONNODEFAULTALLOC_H
#define FABLE_ENGINE_CTBASESINGLETONNODEFAULTALLOC_H
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
#ifdef FABLE_RETAIL_QUICK_ACCESS
        if (Data)
#else
        if (data)
#endif
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


template<class T> class CTBaseSingletonNoDefaultAlloc {
private: static CCountedPointer<T> PClass;
public:
    virtual ~CTBaseSingletonNoDefaultAlloc();
    static T& __fastcall Get() { return *PClass; }
};

#endif

#ifndef FABLE_ENGINE_C2DVECTOR_H
#define FABLE_ENGINE_C2DVECTOR_H
#ifdef FABLE_RETAIL_NAVIGATION_UPDATE


#endif


#ifdef FABLE_RETAIL_SUBTITLE
extern const float DELTA_NOTIONAL_ZERO;
#endif
#ifdef FABLE_RETAIL_C2DVECTOR_MASS

class C2DVector {
public:
    float X,Y;
    C2DVector() {}
#ifdef FABLE_RETAIL_TARGET_RANDOM_WALK
    C2DVector(float x,float y){Initialise(x,y);}
#else
    C2DVector(float x,float y):X(x),Y(y){}
#endif
    C2DVector(const C2DVector& p):X(p.X),Y(p.Y){}
    C2DVector operator+(const C2DVector& vector) const {return C2DVector(X+vector.X,Y+vector.Y);}
    C2DVector operator-(const C2DVector& vector) const {return C2DVector(X-vector.X,Y-vector.Y);}
    const C2DVector operator-() const {return C2DVector(-X,-Y);}
    const C2DVector operator*(float scalar) const {return C2DVector(X*scalar,Y*scalar);}
    const C2DVector& operator+=(const C2DVector& vector) {X+=vector.X;Y+=vector.Y;return *this;}
    float Dot(const C2DVector& vector) const {return X*vector.X+Y*vector.Y;}
#ifdef FABLE_RETAIL_TARGET_RANDOM_WALK
    void Initialise(float x,float y){X=x;Y=y;}
#endif
};
#else
#if defined(FABLE_RETAIL_C2DVECTOR_UI) || defined(FABLE_RETAIL_SUBTITLE)


class C2DVector {
public:
    float X,Y;
#ifdef FABLE_RETAIL_SUBTITLE
    float GetSquaredMagnitude() const { return X*X+Y*Y; }
    inline bool IsNonZero() const { return DELTA_NOTIONAL_ZERO*DELTA_NOTIONAL_ZERO<Y*Y+X*X; }
#endif
    C2DVector() { }
#ifdef FABLE_RETAIL_NAVIGATION_UPDATE
    C2DVector(const C3DVector& p):X(p.X),Y(p.Y){}
    C2DCoordI ToC2DCoordI()const{return C2DCoordI((long)X,(long)Y);}
#endif
#ifdef FABLE_RETAIL_NAV_OUTPUT
    bool operator==(const C2DVector&)const;
#endif
    C2DVector(float x,float y) : X(x),Y(y) { }
    C2DVector(const C2DVector& p) : X(p.X),Y(p.Y) { }
#ifdef FABLE_RETAIL_STATBOX
    const C2DVector& operator+=(const C2DVector& vector){X+=vector.X;Y+=vector.Y;return *this;}
    C2DVector ToC2DCoordF()const{return C2DVector(X,Y);}
#endif
    C2DVector operator+(const C2DVector& vector) const { return C2DVector(X+vector.X,Y+vector.Y); }
    C2DVector operator-(const C2DVector& vector) const { return C2DVector(X-vector.X,Y-vector.Y); }
};
#else
class C2DVector { public: float X,Y; };
#endif
#endif
#endif

#ifndef FABLE_ENGINE_CRIGHTHANDEDSET_H
#define FABLE_ENGINE_CRIGHTHANDEDSET_H



class CRightHandedSet { protected: C3DVector Up,Forward; public: const C3DVector& GetForward() const { return Forward; } };
#endif




#define FABLE_RETAIL_DEFINITION_MANAGER
#define FABLE_RETAIL_TYPED_GETTC
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CTHINGBASEDEF
#define FABLE_RETAIL_CCREATUREDEF
#define FABLE_RETAIL_CHEROCOMBATDEF
#define FABLE_RETAIL_CTCHEROEXPERIENCE
#define FABLE_RETAIL_CTCCOMBAT
#define FABLE_RETAIL_CTCBULLETTIME
#define FABLE_RETAIL_CPLAYERGUI
#define FABLE_RETAIL_CPLAYERMANAGER
#define FABLE_RETAIL_VISIBLE_LOWERBOUND
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
#if defined(FABLE_RETAIL_WASP) || defined(FABLE_RETAIL_SCRIPT_QUEST)
private:
#endif
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
#if defined(FABLE_RETAIL_WASP) || defined(FABLE_RETAIL_SCRIPT_QUEST)
private:
#endif
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
#ifdef FABLE_RETAIL_COMBAT_TYPE
    FABLE_DEFPTR_INLINE CDefPointer& operator=(const CDefPointer& other) {
        if(Object!=other.Object){Release();Object=other.Object;if(Object)Object->AddRef();}
        return *this;
    }
#else
    FABLE_DEFPTR_INLINE CDefPointer& operator=(const CDefPointer& other) { Assign(other.Object); return *this; }
#endif
    template <class U> FABLE_DEFPTR_INLINE CDefPointer& operator=(const CDefPointer<U>& other) { Assign(other.InternalGetObject()); return *this; }
#ifdef FABLE_RETAIL_FACTIONS
    template <class U> FABLE_DEFPTR_INLINE void SetDynamicCast(const CDefPointer<U>& other) {
        if(Object!=other.InternalGetObject()){Release();Object=other.InternalGetObject();if(Object)Object->AddRef();}
    }
#else
    template <class U> FABLE_DEFPTR_INLINE void SetDynamicCast(const CDefPointer<U>& other) { Assign(other.InternalGetObject()); }
#endif
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
#if defined(FABLE_STLPORT_UI_ACTIONS) || defined(FABLE_STLPORT_CREATURE_POSITIONS) || defined(FABLE_STLPORT_CAMERA_POINTS) || defined(FABLE_RETAIL_ENTITY_PERSIST)
#include <new>
#endif
#include <stdlib.h>
#ifdef FABLE_RETAIL_SHOP
#include <stdio.h>
#endif
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
#if defined(FABLE_RETAIL_NAVIGATION_UPDATE) || defined(FABLE_RETAIL_HERO_LOGBOOK) || defined(FABLE_RETAIL_INVENTORY_MAP_SERIALISE)
struct __false_type { };
#else
struct __false_type { __false_type() { } };
#endif
struct input_iterator_tag { };
#ifdef FABLE_RETAIL_NAVIGATION_UPDATE
struct forward_iterator_tag : public input_iterator_tag { };
#else
struct forward_iterator_tag : public input_iterator_tag { forward_iterator_tag() { } };
#endif
struct bidirectional_iterator_tag : public forward_iterator_tag { };
struct random_access_iterator_tag : public bidirectional_iterator_tag { };
#ifdef FABLE_RETAIL_INVENTORY_MAP_SERIALISE
template<class I,class O,class D> inline O __fastcall __copy(I first,I last,O result,const random_access_iterator_tag&,D*) {
    for(D n=last-first;n>0;--n) { *result=*first;++first;++result; } return result;
}
#endif
#if defined(FABLE_RETAIL_NAVIGATION_UPDATE) || defined(FABLE_RETAIL_REGION_GRAPH)
template<class I,class O,class D>O __fastcall __copy(I,I,O,const random_access_iterator_tag&,D*);
template<class I,class C>void __fastcall sort(I,I,C);
#endif
template<class T> inline const T& (max)(const T& a,const T& b) { return a<b?b:a; }
template<class T> inline const T& (min)(const T& a,const T& b) { return b<a?b:a; }
#ifdef FABLE_RETAIL_NAVIGATION_UPDATE
template<class T> inline T* __copy_ptrs(T* first,T* last,T* result,const __false_type&) {return __copy(first,last,result,random_access_iterator_tag(),(ptrdiff_t*)0);}
template<class T> inline T* __copy_ptrs(T* first,T* last,T* result,const __true_type&) {return last==first?result:(T*)((char*)memmove(result,first,(char*)last-(char*)first)+((char*)last-(char*)first));}
#endif
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
#if defined(FABLE_RETAIL_HERO_LOGBOOK) || defined(FABLE_RETAIL_REGION_GRAPH)
template<class I> void __fastcall __destroy_aux(I,I,const __false_type&);
#else
template<class I> inline void __destroy_aux(I first,I last,const __false_type&) {
    for(;first!=last;++first) _Destroy(&*first);
}
#endif
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
template<class T> struct __type_traits { typedef __false_type has_trivial_assignment_operator; typedef __false_type has_trivial_destructor;
#if defined(FABLE_RETAIL_NAVIGATION_UPDATE) || defined(FABLE_RETAIL_SHOP)
    typedef __false_type is_POD_type;
#endif
};
template<> struct __type_traits<char> { typedef __true_type has_trivial_assignment_operator; };
#ifdef FABLE_RETAIL_NAVIGATION_UPDATE
template<> struct __type_traits<unsigned char> { typedef __true_type has_trivial_assignment_operator;typedef __true_type has_trivial_destructor;typedef __true_type is_POD_type; };
#endif
#ifdef FABLE_RETAIL_QUEST_COMPLETION
#ifdef FABLE_RETAIL_HERO_LOGBOOK
template<> struct __type_traits<unsigned short> { typedef __true_type has_trivial_assignment_operator; typedef __true_type has_trivial_destructor; };
#endif
template<> struct __type_traits<long> { typedef __true_type has_trivial_assignment_operator; typedef __true_type has_trivial_destructor; };
template<> struct __type_traits<unsigned long> { typedef __true_type has_trivial_assignment_operator; typedef __true_type has_trivial_destructor; };
#ifdef FABLE_RETAIL_INVENTORY_MAP_SERIALISE
template<> struct __type_traits<unsigned __int64> { typedef __true_type has_trivial_assignment_operator; typedef __true_type has_trivial_destructor; };
#endif
#ifdef FABLE_RETAIL_SHOP
template<> struct __type_traits<float> { typedef __true_type has_trivial_assignment_operator; typedef __true_type has_trivial_destructor; typedef __true_type is_POD_type; };
#endif
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
#if defined(FABLE_RETAIL_NAVIGATION_UPDATE) || defined(FABLE_RETAIL_HERO_LOGBOOK) || defined(FABLE_RETAIL_INVENTORY_MAP_SERIALISE)
struct __false_type { };
#else
struct __false_type { __false_type() { } };
#endif
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
#ifdef FABLE_RETAIL_NAVIGATION_UPDATE
    template<class U> allocator(const allocator<U>&){}
#endif
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
#if defined(FABLE_RETAIL_NAVIGATION_UPDATE) || defined(FABLE_STLPORT_UI) || defined(FABLE_RETAIL_CTHINGMANAGER_BINARY) || defined(FABLE_RETAIL_SHOP)
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
    friend class ::COnHitFixture;
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
#if defined(FABLE_RETAIL_NAVIGATION_UPDATE) || defined(FABLE_RETAIL_CTHINGMANAGER_SAVE)
    vector& operator=(const vector&);
#endif
#ifdef FABLE_RETAIL_CTHINGMANAGER_BINARY
    explicit vector(size_type n) : _Vector_base<_Tp,_Alloc>(n,_Alloc()) {
        this->_M_finish=uninitialized_fill_n(this->_M_start,n,_Tp());
    }
    _Tp& front() { return *this->_M_start; }
#endif
#if defined(FABLE_RETAIL_NAVIGATION_UPDATE) || defined(FABLE_RETAIL_SHOP)
    vector(const vector& x) : _Vector_base<_Tp,_Alloc>(x.size(),x.get_allocator()) {
        this->_M_finish=__uninitialized_copy((const _Tp*)x._M_start,(const _Tp*)x._M_finish,this->_M_start,typename __type_traits<_Tp>::is_POD_type());
    }
#elif defined(FABLE_STLPORT_UI)
    vector(const vector&);
#endif
#if defined(FABLE_RETAIL_NAVIGATION_UPDATE) || defined(FABLE_STLPORT_UI) || defined(FABLE_RETAIL_SHOP)
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
#ifdef FABLE_RETAIL_REGION_GRAPH
    void reserve(size_type);
    vector& operator=(const vector&);
    _Tp& back(){return *(this->_M_finish-1);}
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
#if defined(FABLE_RETAIL_NAVIGATION_UPDATE) || defined(FABLE_RETAIL_REGION_GRAPH) || defined(FABLE_RETAIL_INVENTORY_MAP_SERIALISE)
#ifdef FABLE_RETAIL_NAVIGATION_UPDATE
        iterator result=__copy_ptrs(last,this->_M_finish,first,_IsPODType());
#else
        iterator result=std::__copy(last,this->_M_finish,first,random_access_iterator_tag(),(ptrdiff_t*)0);
#endif
#else
        iterator result=first;
        for(iterator source=last;source!=this->_M_finish;++source,++result) *result=*source;
#endif
        _Destroy(result,this->_M_finish);this->_M_finish=result;return first;
    }
    void clear() { erase(begin(),end()); }
#if defined(FABLE_RETAIL_NAVIGATION_UPDATE) || defined(FABLE_RETAIL_LHHTTP2_RECEIVE_HEADER)
    void resize(size_type n,const _Tp& x) {if(n<size())erase(begin()+n,end());else _M_fill_insert(end(),n-size(),x);}
#elif defined(FABLE_RETAIL_SCOREBOARD)
    __declspec(noinline) void resize(size_type n,const _Tp& x);
#elif defined(FABLE_RETAIL_OAKVALE) || defined(FABLE_RETAIL_HANGING_TREE)
    void resize(size_type n,const _Tp& x);
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
#if defined(FABLE_RETAIL_COMBAT_TYPE) || defined(FABLE_RETAIL_HIT_EFFECT) || defined(FABLE_STLPORT_CAMERA_POINTS) || defined(FABLE_RETAIL_QUEST_COMPLETION) || defined(FABLE_RETAIL_WASP) || defined(FABLE_RETAIL_SCRIPT_QUEST) || defined(FABLE_RETAIL_QUICK_ACCESS)
    pair():first(),second(){}
    pair(const _T1& a,const _T2& b):first(a),second(b) { }
    pair(const pair& p):first(p.first),second(p.second) { }
#if defined(FABLE_RETAIL_QUICK_ACCESS) || defined(FABLE_RETAIL_COMBAT_TYPE)
    template<class U1,class U2> pair(const pair<U1,U2>& p):first(p.first),second(p.second){}
#endif
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
#if defined(FABLE_RETAIL_NAVIGATION_UPDATE) || defined(FABLE_STLPORT_UI_CHANGE_STATE) || defined(FABLE_STLPORT_CAMERA_POINTS) || defined(FABLE_RETAIL_QUICK_ACCESS) || defined(FABLE_RETAIL_INVENTORY_MAP_SERIALISE)
    _List_iterator(_List_node<_Tp>* x) : _List_iterator_base(x) { }
#else
    _List_iterator(_List_node_base* x) : _List_iterator_base(x) { }
#endif
    _List_iterator() { }
#if defined(FABLE_RETAIL_NAVIGATION_UPDATE) || defined(FABLE_STLPORT_UI_CHANGE_STATE) || defined(FABLE_STLPORT_CAMERA_POINTS) || defined(FABLE_RETAIL_QUICK_ACCESS) || defined(FABLE_RETAIL_INVENTORY_MAP_SERIALISE)
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
#ifdef FABLE_RETAIL_NAVIGATION_UPDATE
    _Alloc get_allocator()const{return _Alloc(this->_M_node);}
#endif
#ifdef FABLE_STLPORT_CONSTRUCTORS
#ifdef FABLE_RETAIL_NAVIGATION_UPDATE
    _List_base(const _Alloc& a):_M_node(allocator<_List_node<_Tp> >(a),(_List_node<_Tp>*)0) {
#else
    _List_base(const _Alloc&) : _M_node(allocator<_List_node<_Tp> >(),0) {
#endif
        _List_node<_Tp>* n=_M_node.allocate(1);n->_M_next=n;n->_M_prev=n;_M_node._M_data=n;
    }
    ~_List_base() { clear();_M_node.deallocate(_M_node._M_data,1); }
#endif
    _STLP_alloc_proxy<_List_node<_Tp>*,_List_node<_Tp>,allocator<_List_node<_Tp> > > _M_node;
#if defined(FABLE_RETAIL_NAVIGATION_UPDATE) || defined(FABLE_STLPORT_CAMERA_POINTS) || defined(FABLE_RETAIL_INVENTORY_MAP_SERIALISE) || defined(FABLE_RETAIL_PARTICLE_SERIALISE)
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
#ifdef FABLE_RETAIL_REGION_GRAPH
    ~list();
#endif
#if defined(FABLE_RETAIL_RELATIVE_PATH) || defined(FABLE_RETAIL_QUICK_ACCESS)
    ~list();
#endif
#ifdef FABLE_STLPORT_CONSTRUCTORS
    explicit list(const _Alloc& a=_Alloc()) : _List_base<_Tp,_Alloc>(a) { }
#endif
    typedef _Tp value_type;
    typedef _List_iterator<_Tp,_Nonconst_traits<_Tp> > iterator;
    typedef _List_iterator<_Tp,_Const_traits<_Tp> > const_iterator;
#if defined(FABLE_RETAIL_NAVIGATION_UPDATE) || defined(FABLE_STLPORT_UI_CHANGE_STATE) || defined(FABLE_STLPORT_CAMERA_POINTS) || defined(FABLE_RETAIL_QUICK_ACCESS) || defined(FABLE_RETAIL_INVENTORY_MAP_SERIALISE)
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
#if defined(FABLE_RETAIL_NAVIGATION_UPDATE) || defined(FABLE_RETAIL_QUICK_ACCESS)
#ifndef FABLE_RETAIL_NAVIGATION_UPDATE
    _Alloc get_allocator()const { return _Alloc(); }
#endif
    list(const list& x):_List_base<_Tp,_Alloc>(x.get_allocator()) { insert(begin(),x.begin(),x.end()); }
#ifdef FABLE_RETAIL_NAVIGATION_UPDATE
    list& operator=(const list&);
#endif
    template<class Input> void insert(iterator position,Input first,Input last) { _M_insert_dispatch(position,first,last,__false_type()); }
    template<class Input> void _M_insert_dispatch(iterator,Input,Input,const __false_type&);
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
#if defined(FABLE_STLPORT_UI_ACTIONS) || defined(FABLE_STLPORT_CAMERA_POINTS) || defined(FABLE_RETAIL_QUICK_ACCESS)
protected:
    _List_node<_Tp>* _M_create_node(const _Tp& x) {
        _List_node<_Tp>* p=this->_M_node.allocate(1);
#ifdef FABLE_RETAIL_NAVIGATION_UPDATE
        _Construct(&p->_M_data,x);
#else
        new((void*)&p->_M_data) _Tp(x);
#endif
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
#ifdef FABLE_RETAIL_SHOP

inline void __cdecl __stl_throw_out_of_range(const char* message) { puts(message); abort(); }
#else
void __stl_throw_out_of_range(const char*);
#endif
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


#ifdef FABLE_RETAIL_CTHINGMANAGER_SAVE
#if defined(FABLE_RETAIL_ANIMATION_SEQUENCE)
#ifndef FABLE_ENGINE_CARRAY_H
#define FABLE_ENGINE_CARRAY_H



#ifndef FABLE_STLPORT46_ANIMATION_H
#define FABLE_STLPORT46_ANIMATION_H
#define FABLE_STLPORT_CONTAINERS_H
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <new>
namespace std {
struct __true_type{};struct __false_type{};
template<class T>struct __type_traits{typedef __false_type is_POD_type;typedef __false_type has_trivial_assignment_operator;typedef __false_type has_trivial_destructor;};
template<>struct __type_traits<unsigned char>{typedef __true_type is_POD_type;typedef __true_type has_trivial_assignment_operator;typedef __true_type has_trivial_destructor;};
template<class T>inline const T&(max)(const T&a,const T&b){return a<b?b:a;}
template<class T>class allocator{public:allocator(){} T*allocate(size_t n,const void* =0){return n?(T*)malloc(n*sizeof(T)):0;}void deallocate(T*p,size_t){if(p)free(p);}};
template<class V,class T,class A>class _STLP_alloc_proxy:public A{public:V _M_data;_STLP_alloc_proxy(const A&a,V p):A(a),_M_data(p){}};
template<class T,class U>inline void _Construct(T*p,const U&x){new(p)T(x);}
template<class T>inline void _Destroy(T*p){p->~T();}
template<class I>inline void __destroy_aux(I a,I b,const __false_type&){for(;a!=b;++a)_Destroy(&*a);}
template<class I>inline void __destroy_aux(I,I,const __true_type&){}
template<class T>inline void _Destroy(T*a,T*b){__destroy_aux(a,b,typename __type_traits<T>::has_trivial_destructor());}
inline void*__copy_trivial(const void*a,const void*b,void*d){return b==a?d:(char*)memmove(d,a,(const char*)b-(const char*)a)+((const char*)b-(const char*)a);}
struct input_iterator_tag{};struct forward_iterator_tag:public input_iterator_tag{};struct bidirectional_iterator_tag:public forward_iterator_tag{};struct random_access_iterator_tag:public bidirectional_iterator_tag{};
template<class I,class O,class D>inline O __copy(I a,I b,O d,const random_access_iterator_tag&,D*){for(D n=b-a;n>0;--n){*d=*a;++a;++d;}return d;}
template<class I,class O>inline O __copy_ptrs(I a,I b,O d,const __false_type&){return __copy(a,b,d,random_access_iterator_tag(),(ptrdiff_t*)0);}
template<class I,class O,class D>inline O __copy_backward(I a,I b,O d,const random_access_iterator_tag&,D*){for(D n=b-a;n>0;--n)*--d=*--b;return d;}
template<class I,class O>inline O __copy_backward_ptrs(I a,I b,O d,const __false_type&){return __copy_backward(a,b,d,random_access_iterator_tag(),(ptrdiff_t*)0);}
template<class I,class O>inline O __copy_backward_ptrs(I a,I b,O d,const __true_type&){ptrdiff_t n=b-a;return n?(O)memmove(d-n,a,n*sizeof(*a)):d;}
template<class I,class T>inline void fill(I a,I b,const T&x){for(;a!=b;++a)*a=x;}
template<class I,class O>inline O __copy_ptrs(I a,I b,O d,const __true_type&){return(O)__copy_trivial(a,b,d);}
template<class T>inline T*copy(const T*a,const T*b,T*d){return __copy_ptrs(a,b,d,typename __type_traits<T>::has_trivial_assignment_operator());}
template<class T>inline T*copy(T*a,T*b,T*d){return __copy_ptrs(a,b,d,typename __type_traits<T>::has_trivial_assignment_operator());}
template<class I,class O>inline O __uninitialized_copy(I a,I b,O d,const __false_type&){O cur=d;for(;a!=b;++a,++cur)_Construct(&*cur,*a);return cur;}
template<class I,class O>inline O __uninitialized_copy(I a,I b,O d,const __true_type&){return(O)__copy_trivial(a,b,d);}
template<class T>inline T*fill_n(T*p,size_t n,const T&x){for(;n>0;--n,++p)*p=x;return p;}
template<class I,class N,class T>inline I __uninitialized_fill_n(I p,N n,const T&x,const __true_type&){return fill_n(p,n,x);}
template<class I,class N,class T>inline I __uninitialized_fill_n(I p,N n,const T&x,const __false_type&){I cur=p;for(;n>0;--n,++cur)_Construct(&*cur,x);return cur;}
template<class T,class A>class _Vector_base{public:
 explicit _Vector_base(const A&a):_M_start(0),_M_finish(0),_M_end_of_storage(a,0){}
 _Vector_base(size_t n,const A&a):_M_start(0),_M_finish(0),_M_end_of_storage(a,0){_M_start=_M_end_of_storage.allocate(n);_M_finish=_M_start;_M_end_of_storage._M_data=_M_start+n;}
 ~_Vector_base(){if(_M_start)_M_end_of_storage.deallocate(_M_start,_M_end_of_storage._M_data-_M_start);}
protected:T*_M_start;T*_M_finish;_STLP_alloc_proxy<T*,T,A>_M_end_of_storage;};
template<class T,class A=allocator<T> >class vector:public _Vector_base<T,A>{
public:typedef T value_type;typedef T*pointer;typedef const T*const_pointer;typedef T*iterator;typedef const T*const_iterator;typedef size_t size_type;typedef A allocator_type;typedef typename __type_traits<T>::is_POD_type _IsPODType;
protected:
 void _M_clear(){_Destroy(this->_M_start,this->_M_finish);this->_M_end_of_storage.deallocate(this->_M_start,this->_M_end_of_storage._M_data-this->_M_start);}
 void _M_set(T*s,T*f,T*e){this->_M_start=s;this->_M_finish=f;this->_M_end_of_storage._M_data=e;}
 void _M_insert_overflow(T*position,const T&x,const __true_type&,size_type fill_len,bool at_end=false);
 void _M_insert_overflow(T*position,const T&x,const __false_type&,size_type fill_len,bool at_end=false){
 const size_type old_size=size();const size_type len=old_size+(max)(old_size,fill_len);
 T*new_start=this->_M_end_of_storage.allocate(len);T*new_finish=new_start;
 new_finish=__uninitialized_copy(this->_M_start,position,new_start,_IsPODType());
 if(fill_len==1){_Construct(new_finish,x);++new_finish;}else new_finish=__uninitialized_fill_n(new_finish,fill_len,x,_IsPODType());
 if(!at_end)new_finish=__uninitialized_copy(position,this->_M_finish,new_finish,_IsPODType());
 _M_clear();_M_set(new_start,new_finish,new_start+len);
 }
 template<class I>pointer _M_allocate_and_copy(size_type n,I first,I last){pointer result=this->_M_end_of_storage.allocate(n);__uninitialized_copy(first,last,result,_IsPODType());return result;}

public:
 explicit vector(const A&a=A()):_Vector_base<T,A>(a){}
 explicit vector(size_type n):_Vector_base<T,A>(n,A()){this->_M_finish=__uninitialized_fill_n(this->_M_start,n,T(),_IsPODType());}
 vector(size_type n,const T&x,const A&a=A()):_Vector_base<T,A>(n,a){this->_M_finish=__uninitialized_fill_n(this->_M_start,n,x,_IsPODType());}
 ~vector(){_Destroy(this->_M_start,this->_M_finish);}
 vector&operator=(const vector&);
 iterator begin(){return this->_M_start;}const_iterator begin()const{return this->_M_start;}
 iterator end(){return this->_M_finish;}const_iterator end()const{return this->_M_finish;}
 size_type size()const{return size_type(this->_M_finish-this->_M_start);}
 size_type capacity()const{return size_type(this->_M_end_of_storage._M_data-this->_M_start);}
 T&operator[](size_type n){return this->_M_start[n];}const T&operator[](size_type n)const{return this->_M_start[n];}
 void push_back(const T&x){if(this->_M_finish!=this->_M_end_of_storage._M_data){_Construct(this->_M_finish,x);++this->_M_finish;}else _M_insert_overflow(this->_M_finish,x,_IsPODType(),1,true);}
 iterator erase(iterator first,iterator last){iterator i=__copy_ptrs(last,this->_M_finish,first,typename __type_traits<T>::has_trivial_assignment_operator());_Destroy(i,this->_M_finish);this->_M_finish=i;return first;}
 void clear(){erase(begin(),end());}
 void _M_fill_insert(iterator,size_type,const T&);
 void resize(size_type n,T x){if(n<size())erase(begin()+n,end());else _M_fill_insert(end(),n-size(),x);}
 void resize(size_type n){resize(n,T());}
};
template<class T,class A>vector<T,A>&vector<T,A>::operator=(const vector& x){
 if(&x!=this){const size_type xlen=x.size();if(xlen>capacity()){pointer tmp=_M_allocate_and_copy(xlen,(const_pointer)x._M_start+0,(const_pointer)x._M_finish+0);_M_clear();this->_M_start=tmp;this->_M_end_of_storage._M_data=this->_M_start+xlen;}
 else if(size()>=xlen){pointer i=__copy_ptrs((const_pointer)x._M_start+0,(const_pointer)x._M_finish+0,(pointer)this->_M_start,typename __type_traits<T>::has_trivial_assignment_operator());_Destroy(i,this->_M_finish);}
 else{__copy_ptrs((const_pointer)x._M_start,(const_pointer)x._M_start+size(),(pointer)this->_M_start,typename __type_traits<T>::has_trivial_assignment_operator());__uninitialized_copy((const_pointer)x._M_start+size(),(const_pointer)x._M_finish+0,this->_M_finish,_IsPODType());}this->_M_finish=this->_M_start+xlen;}return *this;
}
template<class T,class A>void vector<T,A>::_M_fill_insert(iterator position,size_type n,const T&x){
 if(n!=0){if(size_type(this->_M_end_of_storage._M_data-this->_M_finish)>=n){T x_copy=x;const size_type elems_after=this->_M_finish-position;pointer old_finish=this->_M_finish;
 if(elems_after>n){__uninitialized_copy(this->_M_finish-n,this->_M_finish,this->_M_finish,_IsPODType());this->_M_finish+=n;__copy_backward_ptrs(position,old_finish-n,old_finish,typename __type_traits<T>::has_trivial_assignment_operator());fill(position,position+n,x_copy);}
 else{__uninitialized_fill_n(this->_M_finish,n-elems_after,x_copy,_IsPODType());this->_M_finish+=n-elems_after;__uninitialized_copy(position,old_finish,this->_M_finish,_IsPODType());this->_M_finish+=elems_after;fill(position,old_finish,x_copy);}}
 else _M_insert_overflow(position,x,_IsPODType(),n);}
}
template<class T,class A>void vector<T,A>::_M_insert_overflow(T*position,const T&x,const __true_type&,size_type fill_len,bool at_end){
 const size_type old_size=size();const size_type len=old_size+(max)(old_size,fill_len);pointer new_start=this->_M_end_of_storage.allocate(len);pointer new_finish=(pointer)__copy_trivial(this->_M_start,position,new_start);
 new_finish=fill_n(new_finish,fill_len,x);if(!at_end)new_finish=(pointer)__copy_trivial(position,this->_M_finish,new_finish);_M_clear();_M_set(new_start,new_finish,new_start+len);
}

}
#endif


template<class T>class CArray:public std::vector<T>{public:
 CArray():std::vector<T>(){} explicit CArray(unsigned int n):std::vector<T>(n){}
 CArray(unsigned int n,const T& x):std::vector<T>(n,x){}
 void push_back(const T& _Val){if(this->_M_finish!=this->_M_end_of_storage._M_data){std::_Construct(this->_M_finish,_Val);++this->_M_finish;}else this->_M_insert_overflow(this->_M_finish,_Val,typename std::__type_traits<T>::has_trivial_assignment_operator(),1,true);}
 T&operator[](unsigned int n){return this->_M_start[n];}const T&operator[](unsigned int n)const{return this->_M_start[n];}
};
#endif
#else
#if defined(FABLE_RETAIL_ANIMATION_SHARED)
#ifndef FABLE_ENGINE_CARRAY_H
#define FABLE_ENGINE_CARRAY_H

template<class T>class CArray:public std::vector<T>{public:T&operator[](unsigned int i){return this->_M_start[i];}const T&operator[](unsigned int i)const{return this->_M_start[i];}const T*GetAsCArray()const{return this->size()>0?this->_M_start:(const T*)this;}};
#endif
#else
#ifdef FABLE_RETAIL_NAV_REGIONS
#ifndef FABLE_ENGINE_CARRAY_H
#define FABLE_ENGINE_CARRAY_H



#ifndef FABLE_STLPORT46_NAVIGATION_H
#define FABLE_STLPORT46_NAVIGATION_H



#ifndef FABLE_STLPORT46_VECTOR_ROOTS_H
#define FABLE_STLPORT46_VECTOR_ROOTS_H

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#ifdef _MSC_VER
inline void* __cdecl operator new(size_t, void* p) { return p; }
#else
#include <new>
#endif
namespace std {
struct __true_type {};
struct __false_type {};
template<class T> struct __type_traits { typedef __false_type has_trivial_assignment_operator; typedef __false_type has_trivial_destructor; };
template<> struct __type_traits<float> { typedef __true_type has_trivial_assignment_operator; typedef __true_type has_trivial_destructor; };
#ifdef FABLE_RETAIL_NAV_OUTPUT
template<class T>struct __type_traits<T*> {typedef __true_type has_trivial_assignment_operator;typedef __true_type has_trivial_destructor;};
#endif
template<class T> inline const T& max(const T& a,const T& b) { return a < b ? b : a; }
template<class A,class B> struct pair {
    typedef A first_type; typedef B second_type;
    A first; B second;
    pair():first(A()),second(B()){}
    pair(const A& a,const B& b):first(a),second(b){}
#ifdef FABLE_RETAIL_ELLIPSE
    
    template<class U,class V> pair(const pair<U,V>& p):first(p.first),second(p.second){}
    pair(const pair<A,B>& p):first(p.first),second(p.second){}
#endif
};
template<class A,class B> inline pair<A,B> make_pair(const A& a,const B& b) {return pair<A,B>(a,b);}
template<class T> class allocator {
public:
    allocator() {}
#ifdef FABLE_RETAIL_NAV_REGIONS
    allocator(const allocator&){}
    template<class U>allocator(const allocator<U>&){}
#endif
    T* allocate(size_t n,const void* = 0) { return n ? (T*)malloc(n*sizeof(T)) : 0; }
    void deallocate(T* p,size_t) { if(p) free(p); }
};
template<class V,class T,class A> class _STLP_alloc_proxy : public A {
public: V _M_data;
    _STLP_alloc_proxy(const A& a,V p):A(a),_M_data(p){}
};
template<class T,class U> inline void _Construct(T* p,const U& x) { new(p) T(x); }
template<class T> inline void _Destroy(T* p) { p->~T(); }
template<class T> inline void __destroy_aux(T* first,T* last,const __false_type&) { for(;first!=last;++first) _Destroy(&*first); }
template<class T> inline void __destroy_aux(T*,T*,const __true_type&) {}
template<class T> inline void _Destroy(T* first,T* last) { __destroy_aux(first,last,typename __type_traits<T>::has_trivial_destructor()); }
inline void* __copy_trivial(const void* first,const void* last,void* out) {
 return last==first ? out : (char*)memmove(out,first,(const char*)last-(const char*)first)+((const char*)last-(const char*)first);
}
template<class I,class O> inline O __uninitialized_copy(I first,I last,O out,const __false_type&) {
 O cur=out; for(;first!=last;++first,++cur) _Construct(&*cur,*first); return cur;
}
template<class I,class O> inline O __uninitialized_copy(I first,I last,O out,const __true_type&) {
 return (O)__copy_trivial(first,last,out);
}
template<class T> inline T* __uninitialized_fill_n(T* first,size_t n,const T& value,const __false_type&) { T* cur=first;for(;n>0;--n,++cur)_Construct(&*cur,value);return cur;}
template<class T> inline T* fill_n(T* first,size_t n,const T& value) { for(;n>0;--n,++first)*first=value;return first; }
template<class T,class A> class _Vector_base {
public: typedef A allocator_type;
 _Vector_base(const A& a):_M_start(0),_M_finish(0),_M_end_of_storage(a,0){}
 _Vector_base(size_t n,const A& a):_M_start(0),_M_finish(0),_M_end_of_storage(a,0) {
  _M_start=_M_end_of_storage.allocate(n);_M_finish=_M_start;_M_end_of_storage._M_data=_M_start+n;
 }
 ~_Vector_base(){if(_M_start!=0)_M_end_of_storage.deallocate(_M_start,_M_end_of_storage._M_data-_M_start);}
protected:
 T* _M_start;T* _M_finish;_STLP_alloc_proxy<T*,T,A> _M_end_of_storage;
};
template<class T,class A=allocator<T> >class vector:public _Vector_base<T,A> {
public:
 typedef T value_type;typedef T* pointer;typedef const T* const_pointer;typedef T* iterator;typedef const T* const_iterator;typedef size_t size_type;typedef A allocator_type;
 typedef typename __type_traits<T>::has_trivial_assignment_operator _IsPODType;
 explicit vector(const A& a=A()):_Vector_base<T,A>(a){}
 vector(const vector& x):_Vector_base<T,A>(x.size(),x.get_allocator()){
 this->_M_finish=__uninitialized_copy((const_pointer)x._M_start,(const_pointer)x._M_finish,this->_M_start,_IsPODType());}
 ~vector(){_Destroy(this->_M_start,this->_M_finish);}
 A get_allocator()const{return (const A&)this->_M_end_of_storage;}
 iterator begin(){return this->_M_start;}const_iterator begin()const{return this->_M_start;}
 iterator end(){return this->_M_finish;}const_iterator end()const{return this->_M_finish;}
 size_type size()const{return size_type(this->_M_finish-this->_M_start);}
#ifdef FABLE_RETAIL_NAV_OUTPUT
 bool empty()const{return begin()==end();}
 T& back(){return *(end()-1);}
 void pop_back(){--this->_M_finish;_Destroy(this->_M_finish);}
#endif
#ifdef FABLE_STLPORT_CYLINDER
 void push_back(const T& x);
#else
 void push_back(const T& x){if(this->_M_finish!=this->_M_end_of_storage._M_data){_Construct(this->_M_finish,x);++this->_M_finish;}else _M_insert_overflow(this->_M_finish,x,_IsPODType(),1,true);}
#endif
#if defined(FABLE_STLPORT_SPLINE) || defined(FABLE_RETAIL_NAV_REGIONS)
 vector& operator=(const vector&);
 iterator insert(iterator,const T&);
 T& operator[](size_type n){return this->_M_start[n];}
 const T& operator[](size_type n)const{return this->_M_start[n];}
#endif
 void clear(){erase(begin(),end());}
 iterator erase(iterator pos){if(pos+1!=end()) __copy_trivial(pos+1,this->_M_finish,pos);--this->_M_finish;_Destroy(this->_M_finish);return pos;}
 iterator erase(iterator first,iterator last){pointer i=first;for(;last!=this->_M_finish;++last,++i)*i=*last;_Destroy(i,this->_M_finish);this->_M_finish=i;return first;}
protected:
 void _M_clear(){_Destroy(this->_M_start,this->_M_finish);this->_M_end_of_storage.deallocate(this->_M_start,this->_M_end_of_storage._M_data-this->_M_start);}
 void _M_set(pointer s,pointer f,pointer e){this->_M_start=s;this->_M_finish=f;this->_M_end_of_storage._M_data=e;}
#if defined(FABLE_STLPORT_SPLINE) || defined(FABLE_RETAIL_NAV_OUTPUT)
 void _M_insert_overflow(pointer,const T&,const __false_type&,size_type,bool=false);
#else
 void _M_insert_overflow(pointer pos,const T& x,const __false_type&,size_type fill_len,bool atend=false){
 const size_type old_size=size();const size_type len=old_size+(max)(old_size,fill_len);
 pointer start=this->_M_end_of_storage.allocate(len);pointer finish=start;
 finish=__uninitialized_copy(this->_M_start,pos,start,__false_type());
 if(fill_len==1){_Construct(finish,x);++finish;}else finish=__uninitialized_fill_n(finish,fill_len,x,__false_type());
 if(!atend)finish=__uninitialized_copy(pos,this->_M_finish,finish,__false_type());
 _M_clear();_M_set(start,finish,start+len);
 }
#endif
#ifdef FABLE_RETAIL_NAV_OUTPUT
 __declspec(noinline)
#endif
 void _M_insert_overflow(pointer pos,const T& x,const __true_type&,size_type fill_len,bool atend=false){
 const size_type old_size=size();const size_type len=old_size+(max)(old_size,fill_len);
 pointer start=this->_M_end_of_storage.allocate(len);pointer finish=(pointer)__copy_trivial(this->_M_start,pos,start);
 finish=fill_n(finish,fill_len,x);if(!atend)finish=(pointer)__copy_trivial(pos,this->_M_finish,finish);
 _M_clear();_M_set(start,finish,start+len);
 }
};
#ifdef FABLE_STLPORT_CYLINDER
template<class A,class B,class R>struct binary_function{typedef A first_argument_type;typedef B second_argument_type;typedef R result_type;};
template<class T>struct less:binary_function<T,T,bool>{bool operator()(const T&a,const T&b)const{return a<b;}};
template<class I,class C>void sort(I,I,C);
#endif
}
#endif

namespace std {
template<class T>struct less;
template<class K,class V,class C=less<K>,class A=allocator<pair<const K,V> > >class map;
struct _List_node_base {_List_node_base* _M_next;_List_node_base* _M_prev;};
template<class T>struct _List_node:public _List_node_base{T _M_data;};
template<class T>struct _Nonconst_traits{typedef T* pointer;typedef T& reference;};
template<class T>struct _Const_traits{typedef const T* pointer;typedef const T& reference;};
struct _List_iterator_base{
 _List_node_base* _M_node;
 _List_iterator_base(_List_node_base* x):_M_node(x){} _List_iterator_base(){}
 void _M_incr(){_M_node=_M_node->_M_next;} void _M_decr(){_M_node=_M_node->_M_prev;}
 bool operator==(const _List_iterator_base& x)const{return _M_node==x._M_node;}
 bool operator!=(const _List_iterator_base& x)const{return _M_node!=x._M_node;}
};
template<class T,class Traits>struct _List_iterator:public _List_iterator_base{
 typedef _List_iterator<T,_Nonconst_traits<T> > iterator;
 _List_iterator(_List_node<T>* x):_List_iterator_base(x){} _List_iterator(){}
 _List_iterator(const iterator& x):_List_iterator_base(x._M_node){}
 typename Traits::reference operator*()const{return ((_List_node<T>*)_M_node)->_M_data;}
 typename Traits::pointer operator->()const{return &operator*();}
 _List_iterator& operator++(){_M_incr();return *this;}
 _List_iterator& operator--(){_M_decr();return *this;}
};
template<class T,class A>class _List_base {
public:
 typedef _List_node<T> _Node;
 _List_base(const A& a):_M_node(allocator<_Node>(a),(_Node*)0){_Node* n=_M_node.allocate(1);n->_M_next=n;n->_M_prev=n;_M_node._M_data=n;}
 ~_List_base(){clear();_M_node.deallocate(_M_node._M_data,1);}
 void clear();
 _STLP_alloc_proxy<_Node*,_Node,allocator<_Node> > _M_node;
};
template<class T,class A>void _List_base<T,A>::clear(){_List_node<T>* cur=(_List_node<T>*)this->_M_node._M_data->_M_next;while(cur!=this->_M_node._M_data){_List_node<T>* tmp=cur;cur=(_List_node<T>*)cur->_M_next;_Destroy(&tmp->_M_data);this->_M_node.deallocate(tmp,1);}this->_M_node._M_data->_M_next=this->_M_node._M_data;this->_M_node._M_data->_M_prev=this->_M_node._M_data;}
template<class T,class A=allocator<T> >class list:public _List_base<T,A>{
public:
 typedef _List_node<T> _Node;typedef _List_iterator<T,_Nonconst_traits<T> > iterator;typedef _List_iterator<T,_Const_traits<T> > const_iterator;
 explicit list(const A& a=A()):_List_base<T,A>(a){}
 list(const list& x):_List_base<T,A>(x.get_allocator()){insert(begin(),x.begin(),x.end());}
 ~list(){}
 A get_allocator()const{return A((const allocator<_Node>&)this->_M_node);}
 iterator begin(){return (_Node*)this->_M_node._M_data->_M_next;}const_iterator begin()const{return (_Node*)this->_M_node._M_data->_M_next;}
 iterator end(){return this->_M_node._M_data;}const_iterator end()const{return this->_M_node._M_data;}
 T& back(){return *--end();}const T& back()const{return *--end();}
protected:
 _Node* _M_create_node(const T& x){_Node* p=this->_M_node.allocate(1);_Construct(&p->_M_data,x);return p;}
public:
 iterator insert(iterator position,const T& x){_Node* tmp=_M_create_node(x);_List_node_base* n=position._M_node;_List_node_base* p=n->_M_prev;tmp->_M_next=n;tmp->_M_prev=p;p->_M_next=tmp;n->_M_prev=tmp;return tmp;}
 void push_back(const T& x){insert(end(),x);}
 template<class I>void insert(iterator pos,I first,I last){_M_insert_dispatch(pos,first,last,__false_type());}
 template<class I>void _M_insert_dispatch(iterator pos,I first,I last,const __false_type&){for(;first!=last;++first)insert(pos,*first);}
};
}
#endif


template<class T>class CArray:public std::vector<T>{public:T&operator[](unsigned int n){return this->_M_start[n];}const T&operator[](unsigned int n)const{return this->_M_start[n];}};
#endif
#else
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
template<>struct __type_traits<long>{typedef __true_type is_POD_type;typedef __true_type has_trivial_destructor;};
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

#endif

#endif

#endif

#else
template<class T> class CArray : public std::vector<T,std::allocator<T> > {
#ifdef FABLE_RETAIL_FACTIONS
public:
    void push_back(const T& value){
        if(this->_M_finish!=this->_M_end_of_storage._M_data){std::_Construct(this->_M_finish,value);++this->_M_finish;}
        else this->_M_insert_overflow(this->_M_finish,value,std::__false_type(),1,true);
    }
#endif
#ifdef FABLE_RETAIL_HIT_EFFECT
public:void push_back(const T&);
#endif
#ifdef FABLE_RETAIL_REGION_GRAPH
public:
    
    void push_back(const T& _Val){
        if(this->_M_finish!=this->_M_end_of_storage._M_data){std::_Construct(this->_M_finish,_Val);++this->_M_finish;}
        else this->_M_insert_overflow(this->_M_finish,_Val,typename std::__type_traits<T>::has_trivial_assignment_operator(),1,true);
    }
#endif
};
#endif
template<class K,class V> class CKeyPairCompareLess {
public:
    bool operator()(const std::pair<K,V>& a,const std::pair<K,V>& b) const { return a.first<b.first; }
};
#ifdef FABLE_RETAIL_FACTIONS
namespace std {template<class Iterator,class Compare>void __fastcall sort(Iterator,Iterator,Compare);}
#endif
template<class K,class V,class CompareType=CKeyPairCompareLess<K,V> >
class CVectorMap : public CArray<std::pair<K,V> > {
public:
    typedef std::pair<K,V> value_type;
#ifdef FABLE_RETAIL_FACTIONS
    CVectorMap():Dirty(false){}
#endif
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
#if defined(FABLE_RETAIL_REGION_GRAPH) || defined(FABLE_RETAIL_FACTIONS)
    void Insert(const K& key,const V& value){this->push_back(value_type(key,value));Dirty=true;}
    void Sort(){std::sort(this->begin(),this->end(),Compare);Dirty=false;}
    void Update(){if(Dirty)Sort();}
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
#if defined(FABLE_RETAIL_TYPED_GETTC) || defined(FABLE_RETAIL_QUICK_ACCESS)


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
#ifdef FABLE_RETAIL_SHOP
    const CThingBaseDef& PeekBaseDef()const{return *PBaseDef;}
    unsigned long GetDefGlobalIndex()const{return DefGlobalIndex;}
#endif
#if defined(FABLE_RETAIL_NAVIGATION_UPDATE) || defined(FABLE_RETAIL_TARGET_RANDOM_WALK) || defined(FABLE_RETAIL_HIT_EFFECT)
    unsigned long GetTypeFlags() const { return ThingTypeFlags; }
#endif
#ifdef FABLE_RETAIL_INVENTORY_MAP_SERIALISE
    unsigned __int64 GetUniqueID() const { return UniqueID; }
    long GetCurrentMapNumber() const { return CurrentMapNumber; }
    bool IsLevelLess() const;
    template<class T> bool PeekTC(T** result) const {
        if(HasTC(T::StatGetInterfaceType())) {
            *result=static_cast<T*>(static_cast<const CTCBase*>(Components.Find(T::StatGetInterfaceType())->second));
            return true;
        }
        return false;
    }
#endif
#ifdef FABLE_RETAIL_NAVIGATION_UPDATE
    unsigned __int64 GetUniqueID()const{return UniqueID;}
    const NGraphicAppearance::CTCGraphicAppearance* GetTCGraphicAppearance()const{return PTCGraphicAppearance;}
#endif
    bool IsDead() const { return Dead!=0; }
    bool IsIncludedInThingScans() const { return IncludeInThingScans && Activated && !Dead && !InLimbo; }
    bool IsAlive() const { return !Dead; }
#ifdef FABLE_RETAIL_FORCE_HASTC
    FABLE_FORCEINLINE bool HasTC(ETCInterfaceType type) const { return UsingComponents.test(type); }
#else
    bool HasTC(ETCInterfaceType type) const { return UsingComponents.test(type); }
#endif
#ifdef FABLE_RETAIL_TYPED_GETTC
    template<class T> FABLE_FORCEINLINE bool GetTC(T** result) {
        if(HasTC(T::StatGetInterfaceType())) {
            *result=GFDebugPDynamicCast(Components.Find(T::StatGetInterfaceType())->second,(T*)0);
            return true;
        }
        return false;
    }
    template<class T> FABLE_FORCEINLINE bool PeekTC(T** result) const {
        if(HasTC(T::StatGetInterfaceType())) {
            *result=GFDebugPDynamicCast(static_cast<const CTCBase*>(Components.Find(T::StatGetInterfaceType())->second),(T*)0);
            return true;
        }
        return false;
    }
#endif
#ifdef FABLE_RETAIL_QUICK_ACCESS
    template<class T> bool PeekTC(T** result) const {
        bool present=HasTC(T::StatGetInterfaceType());
        if(present) *result=GFDebugPDynamicCast(static_cast<const CTCBase*>(Components.Find(T::StatGetInterfaceType())->second),(T*)0);
        return present;
    }
#endif
#ifdef FABLE_RETAIL_HIT_EFFECT
    template<class T> bool GetTCDriver(T**);
#endif
#ifdef FABLE_RETAIL_TYPED_GETTCDRIVER
    
    
    template<class T> FABLE_FORCEINLINE bool GetTCDriver(T** result) {
        const bool& present=HasTC(T::StatGetInterfaceType());
        if(present) *result=GFDebugPDynamicCast(&GetTC(T::StatGetInterfaceType()),(T*)0);
        return present;
    }
#endif
#ifdef FABLE_RETAIL_FORCE_GETTC
    FABLE_FORCEINLINE CTCBase& GetTC(ETCInterfaceType type) { return *Components.Find(type)->second; }
    FABLE_FORCEINLINE const CTCBase& GetTC(ETCInterfaceType type) const { return *Components.Find(type)->second; }
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
#if defined(FABLE_RETAIL_PERSIST_OPINION) || defined(FABLE_RETAIL_CAMERA_TRANSITIONS)
    CBaseIntelligentPointer();
#endif
    virtual ~CBaseIntelligentPointer();
    CBaseObject* GetPItem() const;
#ifdef FABLE_RETAIL_CAMERA_TRANSITIONS
    void SetPItem(CBaseObject*);
#endif
private:
    CBaseObjectPointer* PData;
};
template<class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
#if defined(FABLE_RETAIL_PERSIST_OPINION) || defined(FABLE_RETAIL_CAMERA_TRANSITIONS)
    CIntelligentPointer() { }
#endif
    #ifdef FABLE_RETAIL_CAMERA_TRANSITIONS
    CIntelligentPointer(T* pointer) { SetPointer(pointer); }
    void Set(T* pointer) { SetPointer(pointer); }
private:
    void SetPointer(T* pointer) { SetPItem(const_cast<CBaseObject*>(static_cast<const CBaseObject*>(pointer))); }
public:
#endif
    T* Get() const { return static_cast<T*>(GetPItem()); }
    T* operator->() const { return Get(); }
};
FABLE_STATIC_ASSERT(sizeof(CBaseIntelligentPointer)==8);
#endif



class C2DLineF;
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
#ifdef FABLE_RETAIL_NAVIGATION_UPDATE
    static unsigned long __fastcall StatGetTypeFlags(){return 3;}
    long GetNoNavCollisionLineLists()const;
    void GetNavCollisionLineList(std::vector<C2DLineF>&,long)const;
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
class CHeroCombatDef;
#ifdef FABLE_RETAIL_HIT_EFFECT
#ifndef FABLE_ENGINE_ECOMBATANIMATIONHANDEDNESS_H
#define FABLE_ENGINE_ECOMBATANIMATIONHANDEDNESS_H

enum ECombatAnimationHandedness { HANDED_RIGHT=0,HANDED_LEFT=1,HANDED_NONE=2 };
#endif

#endif
class CThingCreatureBase : public CThingGameObject {
#ifdef FABLE_RETAIL_TARGET_RANDOM_WALK
public:CDefPointer<const CHeroCombatDef> GetPHeroCombatDef() const;
#endif

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
#ifdef FABLE_RETAIL_HIT_EFFECT
    ECombatAnimationHandedness GetLastActionHandedness() const { return *reinterpret_cast<const ECombatAnimationHandedness*>(PreviousActionHandedness); }
#endif
#ifdef FABLE_RETAIL_COMBAT_TYPE
    bool IsInMelee()const;bool IsInMeleeAsAttacker()const;
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

#ifdef FABLE_RETAIL_CTHINGBASEDEF
#ifndef FABLE_ENGINE_CTHINGBASEDEF_H
#define FABLE_ENGINE_CTHINGBASEDEF_H
#ifndef FABLE_ENGINE_CPARENTDEFCLASSBASE_H
#define FABLE_ENGINE_CPARENTDEFCLASSBASE_H

























#ifdef FABLE_RETAIL_DEF_CLASS_BASE
#include <stddef.h>

#ifndef FABLE_ENGINE_CDEFCLASSBASE_H
#define FABLE_ENGINE_CDEFCLASSBASE_H























#ifdef FABLE_RETAIL_DEF_CLASS_BASE
#include <stddef.h>


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
FABLE_STATIC_ASSERT(sizeof(CDefClassBase) == 0x25);
FABLE_STATIC_ASSERT(offsetof(CDefClassBase, PDefManager) == 0x1c);
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

FABLE_STATIC_ASSERT(sizeof(CDefClassBase) == 0x25);
FABLE_STATIC_ASSERT(offsetof(CDefClassBase, __vftable) == 0x0);
FABLE_STATIC_ASSERT(offsetof(CDefClassBase, PDefManager) == 0x1c);
FABLE_STATIC_ASSERT(offsetof(CDefClassBase, GlobalIndex) == 0x20);
FABLE_STATIC_ASSERT(offsetof(CDefClassBase, DefaultValsApplied) == 0x24);

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
#ifdef FABLE_RETAIL_UI_DEF_DTOR
    
#else
    virtual ~CParentDefClassBase();                                      
#endif
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
FABLE_STATIC_ASSERT(sizeof(CParentDefClassBase) == 0x39);
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

FABLE_STATIC_ASSERT(sizeof(CParentDefClassBase) == 0x39);
FABLE_STATIC_ASSERT(offsetof(CParentDefClassBase, __vftable) == 0x0);
FABLE_STATIC_ASSERT(offsetof(CParentDefClassBase, InstantiationName_TablePos) == 0x25);
FABLE_STATIC_ASSERT(offsetof(CParentDefClassBase, SubDefInfoMap) == 0x29);

#endif 

#endif 



class CThingBaseDef : public CParentDefClassBase {
#ifdef FABLE_RETAIL_HIT_EFFECT
    unsigned char _pad_0x39[15];
public:float RenderFadeDistance;
private:unsigned char _pad_0x4c[36];
#else
    unsigned char _pad_0x39[0x37];
#endif
};
FABLE_STATIC_ASSERT(sizeof(CThingBaseDef)==0x70);

#endif
#else
#ifndef FABLE_ENGINE_CTHINGBASEDEF_H
#define FABLE_ENGINE_CTHINGBASEDEF_H






#include <stddef.h>


#pragma pack(push, 1)
class CThingBaseDef {
public:
    void*         __vftable;                                 
    unsigned char _base_0x04[0x25];                          
    unsigned char _pad_0x29[0x10];                           
    unsigned char Components[0x8];                           
    unsigned char InMapSearches[0x1];                        
    unsigned char AlwaysIncludeInObjectStrikeScans[0x1];     
    unsigned char GiveHeroStatChangesOnBeingHit[0x1];        
    long          GroupDef;                                  
    float         RenderFadeDistance;                        
    long          PersistenceFlags;                          
    float         Health;                                    
    float         MinHealth;                                 
    float         MeshHeightOffset;                          
    float         MeshHeight;                                
    float         MeshRadius;                                
    float         ApproxMaxMeshHeight;                       
    long          DefaultScriptName_TablePos;                
    long          DefaultScriptData_TablePos;                
};
#pragma pack(pop)

FABLE_STATIC_ASSERT(sizeof(CThingBaseDef) == 0x70);
FABLE_STATIC_ASSERT(offsetof(CThingBaseDef, __vftable) == 0x0);
FABLE_STATIC_ASSERT(offsetof(CThingBaseDef, Components) == 0x39);
FABLE_STATIC_ASSERT(offsetof(CThingBaseDef, InMapSearches) == 0x41);
FABLE_STATIC_ASSERT(offsetof(CThingBaseDef, AlwaysIncludeInObjectStrikeScans) == 0x42);
FABLE_STATIC_ASSERT(offsetof(CThingBaseDef, GiveHeroStatChangesOnBeingHit) == 0x43);
FABLE_STATIC_ASSERT(offsetof(CThingBaseDef, GroupDef) == 0x44);
FABLE_STATIC_ASSERT(offsetof(CThingBaseDef, RenderFadeDistance) == 0x48);
FABLE_STATIC_ASSERT(offsetof(CThingBaseDef, PersistenceFlags) == 0x4c);
FABLE_STATIC_ASSERT(offsetof(CThingBaseDef, Health) == 0x50);
FABLE_STATIC_ASSERT(offsetof(CThingBaseDef, MinHealth) == 0x54);
FABLE_STATIC_ASSERT(offsetof(CThingBaseDef, MeshHeightOffset) == 0x58);
FABLE_STATIC_ASSERT(offsetof(CThingBaseDef, MeshHeight) == 0x5c);
FABLE_STATIC_ASSERT(offsetof(CThingBaseDef, MeshRadius) == 0x60);
FABLE_STATIC_ASSERT(offsetof(CThingBaseDef, ApproxMaxMeshHeight) == 0x64);
FABLE_STATIC_ASSERT(offsetof(CThingBaseDef, DefaultScriptName_TablePos) == 0x68);
FABLE_STATIC_ASSERT(offsetof(CThingBaseDef, DefaultScriptData_TablePos) == 0x6c);

#endif 

#endif

#ifdef FABLE_RETAIL_CCREATUREDEF
#ifndef FABLE_ENGINE_CCREATUREDEF_H
#define FABLE_ENGINE_CCREATUREDEF_H
#ifndef FABLE_ENGINE_CSUBDEFCLASSBASE_H
#define FABLE_ENGINE_CSUBDEFCLASSBASE_H






#ifndef FABLE_RETAIL_DEF_CLASS_BASE
#error engine/CSubDefClassBase.h is the retail def model: define FABLE_RETAIL_DEF_CLASS_BASE before including it
#else



class CSubDefClassBase : public CDefClassBase {
public:
    CSubDefClassBase();                                                  
    virtual ~CSubDefClassBase() { }                                      
};
FABLE_STATIC_ASSERT(sizeof(CSubDefClassBase) == 0x25);

#endif 
#endif 




class CCreatureDef : public CSubDefClassBase {
public:
    unsigned char _pad_0x25[0xa3];
    bool Short,FlashOnHit,PickPocketable,ChildThatCanRegionFollow;
    bool AllowedInProtectedTowns,UseActualHitPosForHitEffects;
    unsigned char _pad_0xce[0x1a];
};
FABLE_STATIC_ASSERT(sizeof(CCreatureDef)==0xe8);
FABLE_STATIC_ASSERT(offsetof(CCreatureDef,FlashOnHit)==0xc9);

#endif
#else
#ifndef FABLE_ENGINE_CCREATUREDEF_H
#define FABLE_ENGINE_CCREATUREDEF_H






#include <stddef.h>


class CCPPointerInfo;
class CCreatureStatsDef;
class CExpressionSet;
class CRandomAppearanceMorph;
class CWoundMorphs;

#pragma pack(push, 1)
class CCreatureDef {
public:
    void*                   __vftable;                       
    unsigned char           _pad_0x04[0x4];                  
    unsigned char           _pad_0x08[0x4];                  
    unsigned char           _pad_0x0c[0x4];                  
    unsigned char           _pad_0x10[0x4];                  
    unsigned char           _pad_0x14[0x4];                  
    unsigned char           _pad_0x18[0x4];                  
    unsigned char           _pad_0x1c[0x4];                  
    unsigned char           _pad_0x20[0x4];                  
    unsigned char           _pad_0x24[0x1];                  
    unsigned char           _pad_0x25[0x3];                  
    CWoundMorphs*           PWoundMorphs_Data;               
    CCPPointerInfo*         PWoundMorphs_Info;               
    CRandomAppearanceMorph* PRandomAppearanceMorph_Data;     
    CCPPointerInfo*         PRandomAppearanceMorph_Info;     
    CExpressionSet*         PExpressions_Data;               
    CCPPointerInfo*         PExpressions_Info;               
    CCreatureStatsDef*      PStats_Data;                     
    CCPPointerInfo*         PStats_Info;                     
    unsigned char           InitialActions[0xc];             
    unsigned char           InitialAppearanceModifiers[0xc];  
    unsigned char           PhonemeAnim[0xc];                
    unsigned char           DialogueVoices[0xc];             
    long                    Inventory;                       
    long                    ClothingInventory;               
    long                    WeaponsInventory;                
    long                    HeroAbilitiesScreenInventory;    
    long                    QuestCardScreenInventory;        
    long                    MapScreenInventory;              
    long                    StatsScreenInventory;            
    long                    MagicScreenInventory;            
    long                    ExperienceScreenInventory;       
    long                    TradeScreenInventory;            
    long                    AugmentScreenInventory;          
    long                    QuestsScreenInventory;           
    long                    InGameMenuScreen;                
    unsigned long           CreatureGroup;                   
    long                    BattleCrySound_TablePos;         
    long                    OpinionSourceDef;                
    long                    RespawnWaitInDaysMin;            
    long                    RespawnWaitInDaysMax;            
    long                    RespawnWaitInFramesMin;          
    long                    RespawnWaitInFramesMax;          
    bool                    Short;                           
    bool                    FlashOnHit;                      
    bool                    PickPocketable;                  
    bool                    ChildThatCanRegionFollow;        
    bool                    AllowedInProtectedTowns;         
    bool                    UseActualHitPosForHitEffects;    
    unsigned char           _pad_0xce[0x2];                  
    unsigned char           RadiusWeight[0xc];               
    float                   RadiusSpread_HighValue;          
    float                   RadiusSpread_LowValue;           
    bool                    AGhostIsWhatIBe;                 
    unsigned char           _pad_0xe5[0x3];                  
};
#pragma pack(pop)

FABLE_STATIC_ASSERT(sizeof(CCreatureDef) == 0xe8);
FABLE_STATIC_ASSERT(offsetof(CCreatureDef, __vftable) == 0x0);
FABLE_STATIC_ASSERT(offsetof(CCreatureDef, PWoundMorphs_Data) == 0x28);
FABLE_STATIC_ASSERT(offsetof(CCreatureDef, PWoundMorphs_Info) == 0x2c);
FABLE_STATIC_ASSERT(offsetof(CCreatureDef, PRandomAppearanceMorph_Data) == 0x30);
FABLE_STATIC_ASSERT(offsetof(CCreatureDef, PRandomAppearanceMorph_Info) == 0x34);
FABLE_STATIC_ASSERT(offsetof(CCreatureDef, PExpressions_Data) == 0x38);
FABLE_STATIC_ASSERT(offsetof(CCreatureDef, PExpressions_Info) == 0x3c);
FABLE_STATIC_ASSERT(offsetof(CCreatureDef, PStats_Data) == 0x40);
FABLE_STATIC_ASSERT(offsetof(CCreatureDef, PStats_Info) == 0x44);
FABLE_STATIC_ASSERT(offsetof(CCreatureDef, InitialActions) == 0x48);
FABLE_STATIC_ASSERT(offsetof(CCreatureDef, InitialAppearanceModifiers) == 0x54);
FABLE_STATIC_ASSERT(offsetof(CCreatureDef, PhonemeAnim) == 0x60);
FABLE_STATIC_ASSERT(offsetof(CCreatureDef, DialogueVoices) == 0x6c);
FABLE_STATIC_ASSERT(offsetof(CCreatureDef, Inventory) == 0x78);
FABLE_STATIC_ASSERT(offsetof(CCreatureDef, ClothingInventory) == 0x7c);
FABLE_STATIC_ASSERT(offsetof(CCreatureDef, WeaponsInventory) == 0x80);
FABLE_STATIC_ASSERT(offsetof(CCreatureDef, HeroAbilitiesScreenInventory) == 0x84);
FABLE_STATIC_ASSERT(offsetof(CCreatureDef, QuestCardScreenInventory) == 0x88);
FABLE_STATIC_ASSERT(offsetof(CCreatureDef, MapScreenInventory) == 0x8c);
FABLE_STATIC_ASSERT(offsetof(CCreatureDef, StatsScreenInventory) == 0x90);
FABLE_STATIC_ASSERT(offsetof(CCreatureDef, MagicScreenInventory) == 0x94);
FABLE_STATIC_ASSERT(offsetof(CCreatureDef, ExperienceScreenInventory) == 0x98);
FABLE_STATIC_ASSERT(offsetof(CCreatureDef, TradeScreenInventory) == 0x9c);
FABLE_STATIC_ASSERT(offsetof(CCreatureDef, AugmentScreenInventory) == 0xa0);
FABLE_STATIC_ASSERT(offsetof(CCreatureDef, QuestsScreenInventory) == 0xa4);
FABLE_STATIC_ASSERT(offsetof(CCreatureDef, InGameMenuScreen) == 0xa8);
FABLE_STATIC_ASSERT(offsetof(CCreatureDef, CreatureGroup) == 0xac);
FABLE_STATIC_ASSERT(offsetof(CCreatureDef, BattleCrySound_TablePos) == 0xb0);
FABLE_STATIC_ASSERT(offsetof(CCreatureDef, OpinionSourceDef) == 0xb4);
FABLE_STATIC_ASSERT(offsetof(CCreatureDef, RespawnWaitInDaysMin) == 0xb8);
FABLE_STATIC_ASSERT(offsetof(CCreatureDef, RespawnWaitInDaysMax) == 0xbc);
FABLE_STATIC_ASSERT(offsetof(CCreatureDef, RespawnWaitInFramesMin) == 0xc0);
FABLE_STATIC_ASSERT(offsetof(CCreatureDef, RespawnWaitInFramesMax) == 0xc4);
FABLE_STATIC_ASSERT(offsetof(CCreatureDef, Short) == 0xc8);
FABLE_STATIC_ASSERT(offsetof(CCreatureDef, FlashOnHit) == 0xc9);
FABLE_STATIC_ASSERT(offsetof(CCreatureDef, PickPocketable) == 0xca);
FABLE_STATIC_ASSERT(offsetof(CCreatureDef, ChildThatCanRegionFollow) == 0xcb);
FABLE_STATIC_ASSERT(offsetof(CCreatureDef, AllowedInProtectedTowns) == 0xcc);
FABLE_STATIC_ASSERT(offsetof(CCreatureDef, UseActualHitPosForHitEffects) == 0xcd);
FABLE_STATIC_ASSERT(offsetof(CCreatureDef, RadiusWeight) == 0xd0);
FABLE_STATIC_ASSERT(offsetof(CCreatureDef, RadiusSpread_HighValue) == 0xdc);
FABLE_STATIC_ASSERT(offsetof(CCreatureDef, RadiusSpread_LowValue) == 0xe0);
FABLE_STATIC_ASSERT(offsetof(CCreatureDef, AGhostIsWhatIBe) == 0xe4);

#endif 

#endif

#ifndef FABLE_ENGINE_CHITPARAMETERS_H
#define FABLE_ENGINE_CHITPARAMETERS_H



namespace NHitLocations { class CHitLocation; }
class CMaterialDef;class CThing;


class CHitParameters {
#ifdef FABLE_ONHIT_TEST_ACCESS
    friend class COnHitFixture;
#endif

private:
    bool Localised,Directional;
    unsigned char Position[12]; 
    unsigned char Direction[12]; 
    bool KnockDown,Decapitate,Blockable,Flourish,EpicSpell,BlockCounter;
    bool PlayHitResponse,PlayHitResponseOverrideSet,MoveBack;
    bool CreateParticleEffectOnHit,CreateDustParticleEffectOnHit,GuaranteeHit;
    bool Blocked,HitNegated,CauseRecoil;
    float ShotBuildupTime;
    unsigned char DamageMap[12]; 
    unsigned char Augmentations[12]; 
    NHitLocations::CHitLocation* PHitLocation;
    CDefPointer<const CMaterialDef> PHitMaterialDef;
    CIntelligentPointer<CThing> PThingHitting,PWeapon;
public:
    const C3DVector& GetHitPosition() const { return *reinterpret_cast<const C3DVector*>(Position); }
    const C3DVector& GetHitDirection() const { return *reinterpret_cast<const C3DVector*>(Direction); }
    CThing* GetPThingHitting() const { return PThingHitting.Get(); }
    CThing* GetPWeapon() const { return PWeapon.Get(); }
    NHitLocations::CHitLocation* GetPHitLocation() const { return PHitLocation; }
    bool IsLocalised() const { return Localised; }
    bool IsDirectional() const { return Directional; }
    bool IsKnockDown() const { return KnockDown; }
    bool IsDecapitate() const { return Decapitate; }
    bool IsFlourish() const { return Flourish; }
    bool IsEpicSpell() const { return EpicSpell; }
    bool IsBlockCounter() const { return BlockCounter; }
    bool IsBlocked() const { return Blocked; }
    bool IsHitNegated() const { return HitNegated; }
    bool IsMoveBackOnHit() const { return MoveBack; }
    bool IsCreateDustParticleEffectOnHit() const { return CreateDustParticleEffectOnHit; }
    void SetKnockDown(bool value) { KnockDown=value; }
    void SetDecapitate(bool value) { Decapitate=value; }
    void SetPlayHitResponse(bool value) { PlayHitResponseOverrideSet=true;PlayHitResponse=value; }
    void SetMoveBackOnHit(bool value) { MoveBack=value; }
#ifdef FABLE_RETAIL_HIT_EFFECT
    CDefPointer<const CMaterialDef> GetPHitMaterialDef() const { return PHitMaterialDef; }
    bool IsCreateParticleEffectOnHit() const { return CreateParticleEffectOnHit; }
#endif
    bool IsMeleeStrike() const;
    bool IsExplosionStrike() const;
    bool IsDrainStrike() const;
    bool IsPlayGetHitResponse(const CThing*) const;
};
FABLE_STATIC_ASSERT(sizeof(CHitParameters)==0x60);

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




class CThing;
class CEnginePrimitivePrepareInfo;class CWorld;
class CInterfaceType;
class CPersistContext;
#ifdef FABLE_RETAIL_SHOP
class CGameDefinitionManager;
#endif
#ifdef FABLE_RETAIL_INVENTORY_MAP_SERIALISE
class CWorldMap;class CThingManager;
#endif
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
#ifdef FABLE_RETAIL_SHOP
    const CGameDefinitionManager& GetDefinitionManager() const;
#endif
protected:
#ifdef FABLE_RETAIL_TARGET_RANDOM_WALK
    CWorld& GetWorld() const;
#endif
#if defined(FABLE_RETAIL_INVENTORY_MAP_SERIALISE) || defined(FABLE_RETAIL_SHOP_PLACES)
    const CWorld& PeekWorld() const;
#endif
#ifdef FABLE_RETAIL_INVENTORY_MAP_SERIALISE
    CWorldMap& GetWorldMap();
    CThingManager& GetThingManager();
#endif
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


#ifndef FABLE_ENGINE_CTCFLASHONHIT_H
#define FABLE_ENGINE_CTCFLASHONHIT_H


class CTCFlashOnHit : public CTCBase {
private: long FadeInTime,FadeOutTime,LifeTime,Timer;
public:
    void SetFadeInTime(long n) { FadeInTime=n; }
    void SetFadeOutTime(long n) { FadeOutTime=n; }
    void SetLifeTime(long);
};
FABLE_STATIC_ASSERT(sizeof(CTCFlashOnHit)==0x1c);

#endif

#ifndef FABLE_ENGINE_CTCTALK_H
#define FABLE_ENGINE_CTCTALK_H

class CASoundBank;

class CTCTalk : public CTCBase {
private:
    long SpeechSample,SampleStartedFrame,SampleDurationFrames;
    unsigned char Attitude[4]; 
    float MimeStartTime,SampleLoadedTime;CASoundBank* PSpeechBank;
    unsigned long SampleID;long LastTextIndex;CCharString LastNarrator;
    bool IsPureAISpeaker,LipSynching,UseLipSynching;CCharString DialogueVoice;
public:
    static ETCInterfaceType __fastcall StatGetInterfaceType() { return TCI_TALK; }
    bool IsTalking() const;
    void StopCurrentSpeech();
};
FABLE_STATIC_ASSERT(sizeof(CTCTalk)==0x3c);

#endif

#ifndef FABLE_ENGINE_CTCSCRIPTEDCONTROL_H
#define FABLE_ENGINE_CTCSCRIPTEDCONTROL_H



class CTCScriptedControl : public CTCBase {
private:
    unsigned char PScriptHandleImp[8]; 
    unsigned char PActionList[4]; 
    bool Locked;unsigned char _pad_0x19[3];unsigned char ScriptAIPriority[4]; 
    unsigned char AttitudeTags[4]; 
public:
    static ETCInterfaceType __fastcall StatGetInterfaceType() { return TCI_SCRIPTED_CONTROL; } bool IsActive() const;
};

FABLE_STATIC_ASSERT(sizeof(CTCScriptedControl)==0x24);
#endif

#ifdef FABLE_RETAIL_CTCHEROEXPERIENCE
#ifndef FABLE_ENGINE_CTCHEROEXPERIENCE_H
#define FABLE_ENGINE_CTCHEROEXPERIENCE_H


#ifdef FABLE_RETAIL_TARGET_RANDOM_WALK
#ifndef FABLE_ENGINE_CHEROCOMBATDEF_H
#define FABLE_ENGINE_CHEROCOMBATDEF_H

#ifndef FABLE_ENGINE_CFLOATRANGE_H
#define FABLE_ENGINE_CFLOATRANGE_H
class CFloatRange{public:float HighValue,LowValue;};
#endif

#ifndef FABLE_ENGINE_CHEROABILITYSTATWEIGHT_H
#define FABLE_ENGINE_CHEROABILITYSTATWEIGHT_H
class CTCHeroExperience;
class CHeroAbilityStatWeight{public:unsigned char StatWeights[12];float GetParameter(const CTCHeroExperience&)const;};
#endif

class CHeroCombatDef:public CParentDefClassBase{
public: virtual void Copy(const CDefClassBase *);
public: virtual unsigned long GetSizeofClass();
public: virtual void Transfer(CPersistContext &);
public: virtual ~CHeroCombatDef();
public:
CFloatRange RangedAccuracySecondsForMaximumWithSkill;
CFloatRange RangedAccuracyPercentFractionWithBuildup;
CFloatRange RangedAccuracyPercentWithStats;
CHeroAbilityStatWeight RangedAccuracyWeighting;
CFloatRange RangedAccuracyShakeyCursor;
CHeroAbilityStatWeight RangedAccuracyShakeyCursorWeighting;
float RangedAccuracyShakeFractionOfRadius;
CHeroAbilityStatWeight RangedDamageMultiplierWeighting;
float RangedDamageMultiplierSpeedWithStatParameter;
float RangedDamageMultiplierSpeedWithDistance;
float RangedDamageMultiplierSpeedNominalBuildup;
float RangedDamageMultiplierScaleNominalBuildup;
float RangedDamageMultiplierSpeedSuperBuildup;
float RangedDamageMultiplierScaleSuperBuildup;
CHeroAbilityStatWeight RangedWeaponReloadSpeedWeighting;
CFloatRange MeleeDamageMultiplierWithStats;
CHeroAbilityStatWeight MeleeDamageWeighting;
CFloatRange HitDamageMultiplierWithStats;
CHeroAbilityStatWeight HitDamageWeighting;
bool MeleeFlourishValidOnMultiplierLevelIncrease;
long MeleeFlourishNumMultiplierLevelsPerFlourish;
bool MeleeFlourishAllowBreakingOfRotation;
float MeleeFlourishSecondsForAvailibility;
bool MeleeFlourishOneShot;
bool MeleeFlourishUsable;
bool MeleeCombatPlayStrikeSounds;
float TargetingPressureFractionThresholdForToggle;
float MaxAngleToTurnToShootAtTargetInThirdPersonBowMode;
long MeleeStrikeNumFramesPauseOnHeroStrike;
CFloatRange MeleeStrikeAnimationSpeedFractionWithStats;
CHeroAbilityStatWeight MeleeStrikeAnimationSpeedWeighting;
float AutoBlockSkillThreshold;
float AutoBlockStrengthThreshold;
float MeleeRecoilMaintainHandednessSkillThreshold;
float MeleeRecoilMaintainHandednessStrengthThreshold;
float MeleeRecoilMaintainComboChainSkillThreshold;
float MeleeRecoilMaintainComboChainStrengthThreshold;
std::vector<long,std::allocator<long> > DefaultCombatAbilities;
std::vector<float,std::allocator<float> > ProjectileWeaponFirstPersonModeMaxFOVValues;
bool ProjectileTargetingAcquireTargetTag;
bool ProjectileTargetingAcquireTargetsAuto;
bool ProjectileTargetingUseAnalogueZoom;
float ProjectileWeaponAnalogueZoomControlWeight;
unsigned char HeavyWeapons[12]; 
};
FABLE_STATIC_ASSERT(sizeof(CHeroCombatDef)==0x148);
FABLE_STATIC_ASSERT(offsetof(CHeroCombatDef,RangedAccuracyShakeyCursor)==0x60);
FABLE_STATIC_ASSERT(offsetof(CHeroCombatDef,RangedAccuracyShakeFractionOfRadius)==0x74);
#endif

#else
#ifdef FABLE_RETAIL_CHEROCOMBATDEF
#ifndef FABLE_ENGINE_CHEROCOMBATDEF_H
#define FABLE_ENGINE_CHEROCOMBATDEF_H




class CHeroCombatDef : public CParentDefClassBase {
public:
    unsigned char _pad_0x39[0xb3];
    long MeleeStrikeNumFramesPauseOnHeroStrike;
    unsigned char _pad_0xf0[0x58];
};
FABLE_STATIC_ASSERT(sizeof(CHeroCombatDef)==0x148);
FABLE_STATIC_ASSERT(offsetof(CHeroCombatDef,MeleeStrikeNumFramesPauseOnHeroStrike)==0xec);

#endif
#else
#ifndef FABLE_ENGINE_CHEROCOMBATDEF_H
#define FABLE_ENGINE_CHEROCOMBATDEF_H






#include <stddef.h>


#pragma pack(push, 1)
class CHeroCombatDef {
public:
    void*         __vftable;                                 
    unsigned char _pad_0x04[0x4];                            
    unsigned char _pad_0x08[0x4];                            
    unsigned char _pad_0x0c[0x4];                            
    unsigned char _pad_0x10[0x4];                            
    unsigned char _pad_0x14[0x4];                            
    unsigned char _pad_0x18[0x4];                            
    unsigned char _pad_0x1c[0x4];                            
    unsigned char _pad_0x20[0x4];                            
    unsigned char _pad_0x24[0x4];                            
    unsigned char _pad_0x28[0x1];                            
    unsigned char _pad_0x29[0x13];                           
    float         RangedAccuracySecondsForMaximumWithSkill_HighValue; 
    float         RangedAccuracySecondsForMaximumWithSkill_LowValue; 
    float         RangedAccuracyPercentFractionWithBuildup_HighValue; 
    float         RangedAccuracyPercentFractionWithBuildup_LowValue; 
    float         RangedAccuracyPercentWithStats_HighValue;  
    float         RangedAccuracyPercentWithStats_LowValue;   
    unsigned char RangedAccuracyWeighting[0xc];              
    float         RangedAccuracyShakeyCursor_HighValue;      
    float         RangedAccuracyShakeyCursor_LowValue;       
    unsigned char RangedAccuracyShakeyCursorWeighting[0xc];  
    float         RangedAccuracyShakeFractionOfRadius;       
    unsigned char RangedDamageMultiplierWeighting[0xc];      
    float         RangedDamageMultiplierSpeedWithStatParameter; 
    float         RangedDamageMultiplierSpeedWithDistance;   
    float         RangedDamageMultiplierSpeedNominalBuildup; 
    float         RangedDamageMultiplierScaleNominalBuildup; 
    float         RangedDamageMultiplierSpeedSuperBuildup;   
    float         RangedDamageMultiplierScaleSuperBuildup;   
    unsigned char RangedWeaponReloadSpeedWeighting[0xc];     
    float         MeleeDamageMultiplierWithStats_HighValue;  
    float         MeleeDamageMultiplierWithStats_LowValue;   
    unsigned char MeleeDamageWeighting[0xc];                 
    float         HitDamageMultiplierWithStats_HighValue;    
    float         HitDamageMultiplierWithStats_LowValue;     
    unsigned char HitDamageWeighting[0xc];                   
    bool          MeleeFlourishValidOnMultiplierLevelIncrease; 
    unsigned char _pad_0xd1[0x3];                            
    long          MeleeFlourishNumMultiplierLevelsPerFlourish; 
    bool          MeleeFlourishAllowBreakingOfRotation;      
    unsigned char _pad_0xd9[0x3];                            
    float         MeleeFlourishSecondsForAvailibility;       
    bool          MeleeFlourishOneShot;                      
    bool          MeleeFlourishUsable;                       
    bool          MeleeCombatPlayStrikeSounds;               
    unsigned char _pad_0xe3[0x1];                            
    float         TargetingPressureFractionThresholdForToggle; 
    float         MaxAngleToTurnToShootAtTargetInThirdPersonBowMode; 
    long          MeleeStrikeNumFramesPauseOnHeroStrike;     
    float         MeleeStrikeAnimationSpeedFractionWithStats_HighValue; 
    float         MeleeStrikeAnimationSpeedFractionWithStats_LowValue; 
    unsigned char MeleeStrikeAnimationSpeedWeighting[0xc];   
    float         AutoBlockSkillThreshold;                   
    float         AutoBlockStrengthThreshold;                
    float         MeleeRecoilMaintainHandednessSkillThreshold; 
    float         MeleeRecoilMaintainHandednessStrengthThreshold; 
    float         MeleeRecoilMaintainComboChainSkillThreshold; 
    float         MeleeRecoilMaintainComboChainStrengthThreshold; 
    unsigned char DefaultCombatAbilities[0xc];               
    unsigned char ProjectileWeaponFirstPersonModeMaxFOVValues[0xc];  
    bool          ProjectileTargetingAcquireTargetTag;       
    bool          ProjectileTargetingAcquireTargetsAuto;     
    bool          ProjectileTargetingUseAnalogueZoom;        
    unsigned char _pad_0x137[0x1];                           
    float         ProjectileWeaponAnalogueZoomControlWeight; 
    unsigned char HeavyWeapons[0xc];                         
};
#pragma pack(pop)

FABLE_STATIC_ASSERT(sizeof(CHeroCombatDef) == 0x148);
FABLE_STATIC_ASSERT(offsetof(CHeroCombatDef, __vftable) == 0x0);
FABLE_STATIC_ASSERT(offsetof(CHeroCombatDef, RangedAccuracySecondsForMaximumWithSkill_HighValue) == 0x3c);
FABLE_STATIC_ASSERT(offsetof(CHeroCombatDef, RangedAccuracySecondsForMaximumWithSkill_LowValue) == 0x40);
FABLE_STATIC_ASSERT(offsetof(CHeroCombatDef, RangedAccuracyPercentFractionWithBuildup_HighValue) == 0x44);
FABLE_STATIC_ASSERT(offsetof(CHeroCombatDef, RangedAccuracyPercentFractionWithBuildup_LowValue) == 0x48);
FABLE_STATIC_ASSERT(offsetof(CHeroCombatDef, RangedAccuracyPercentWithStats_HighValue) == 0x4c);
FABLE_STATIC_ASSERT(offsetof(CHeroCombatDef, RangedAccuracyPercentWithStats_LowValue) == 0x50);
FABLE_STATIC_ASSERT(offsetof(CHeroCombatDef, RangedAccuracyWeighting) == 0x54);
FABLE_STATIC_ASSERT(offsetof(CHeroCombatDef, RangedAccuracyShakeyCursor_HighValue) == 0x60);
FABLE_STATIC_ASSERT(offsetof(CHeroCombatDef, RangedAccuracyShakeyCursor_LowValue) == 0x64);
FABLE_STATIC_ASSERT(offsetof(CHeroCombatDef, RangedAccuracyShakeyCursorWeighting) == 0x68);
FABLE_STATIC_ASSERT(offsetof(CHeroCombatDef, RangedAccuracyShakeFractionOfRadius) == 0x74);
FABLE_STATIC_ASSERT(offsetof(CHeroCombatDef, RangedDamageMultiplierWeighting) == 0x78);
FABLE_STATIC_ASSERT(offsetof(CHeroCombatDef, RangedDamageMultiplierSpeedWithStatParameter) == 0x84);
FABLE_STATIC_ASSERT(offsetof(CHeroCombatDef, RangedDamageMultiplierSpeedWithDistance) == 0x88);
FABLE_STATIC_ASSERT(offsetof(CHeroCombatDef, RangedDamageMultiplierSpeedNominalBuildup) == 0x8c);
FABLE_STATIC_ASSERT(offsetof(CHeroCombatDef, RangedDamageMultiplierScaleNominalBuildup) == 0x90);
FABLE_STATIC_ASSERT(offsetof(CHeroCombatDef, RangedDamageMultiplierSpeedSuperBuildup) == 0x94);
FABLE_STATIC_ASSERT(offsetof(CHeroCombatDef, RangedDamageMultiplierScaleSuperBuildup) == 0x98);
FABLE_STATIC_ASSERT(offsetof(CHeroCombatDef, RangedWeaponReloadSpeedWeighting) == 0x9c);
FABLE_STATIC_ASSERT(offsetof(CHeroCombatDef, MeleeDamageMultiplierWithStats_HighValue) == 0xa8);
FABLE_STATIC_ASSERT(offsetof(CHeroCombatDef, MeleeDamageMultiplierWithStats_LowValue) == 0xac);
FABLE_STATIC_ASSERT(offsetof(CHeroCombatDef, MeleeDamageWeighting) == 0xb0);
FABLE_STATIC_ASSERT(offsetof(CHeroCombatDef, HitDamageMultiplierWithStats_HighValue) == 0xbc);
FABLE_STATIC_ASSERT(offsetof(CHeroCombatDef, HitDamageMultiplierWithStats_LowValue) == 0xc0);
FABLE_STATIC_ASSERT(offsetof(CHeroCombatDef, HitDamageWeighting) == 0xc4);
FABLE_STATIC_ASSERT(offsetof(CHeroCombatDef, MeleeFlourishValidOnMultiplierLevelIncrease) == 0xd0);
FABLE_STATIC_ASSERT(offsetof(CHeroCombatDef, MeleeFlourishNumMultiplierLevelsPerFlourish) == 0xd4);
FABLE_STATIC_ASSERT(offsetof(CHeroCombatDef, MeleeFlourishAllowBreakingOfRotation) == 0xd8);
FABLE_STATIC_ASSERT(offsetof(CHeroCombatDef, MeleeFlourishSecondsForAvailibility) == 0xdc);
FABLE_STATIC_ASSERT(offsetof(CHeroCombatDef, MeleeFlourishOneShot) == 0xe0);
FABLE_STATIC_ASSERT(offsetof(CHeroCombatDef, MeleeFlourishUsable) == 0xe1);
FABLE_STATIC_ASSERT(offsetof(CHeroCombatDef, MeleeCombatPlayStrikeSounds) == 0xe2);
FABLE_STATIC_ASSERT(offsetof(CHeroCombatDef, TargetingPressureFractionThresholdForToggle) == 0xe4);
FABLE_STATIC_ASSERT(offsetof(CHeroCombatDef, MaxAngleToTurnToShootAtTargetInThirdPersonBowMode) == 0xe8);
FABLE_STATIC_ASSERT(offsetof(CHeroCombatDef, MeleeStrikeNumFramesPauseOnHeroStrike) == 0xec);
FABLE_STATIC_ASSERT(offsetof(CHeroCombatDef, MeleeStrikeAnimationSpeedFractionWithStats_HighValue) == 0xf0);
FABLE_STATIC_ASSERT(offsetof(CHeroCombatDef, MeleeStrikeAnimationSpeedFractionWithStats_LowValue) == 0xf4);
FABLE_STATIC_ASSERT(offsetof(CHeroCombatDef, MeleeStrikeAnimationSpeedWeighting) == 0xf8);
FABLE_STATIC_ASSERT(offsetof(CHeroCombatDef, AutoBlockSkillThreshold) == 0x104);
FABLE_STATIC_ASSERT(offsetof(CHeroCombatDef, AutoBlockStrengthThreshold) == 0x108);
FABLE_STATIC_ASSERT(offsetof(CHeroCombatDef, MeleeRecoilMaintainHandednessSkillThreshold) == 0x10c);
FABLE_STATIC_ASSERT(offsetof(CHeroCombatDef, MeleeRecoilMaintainHandednessStrengthThreshold) == 0x110);
FABLE_STATIC_ASSERT(offsetof(CHeroCombatDef, MeleeRecoilMaintainComboChainSkillThreshold) == 0x114);
FABLE_STATIC_ASSERT(offsetof(CHeroCombatDef, MeleeRecoilMaintainComboChainStrengthThreshold) == 0x118);
FABLE_STATIC_ASSERT(offsetof(CHeroCombatDef, DefaultCombatAbilities) == 0x11c);
FABLE_STATIC_ASSERT(offsetof(CHeroCombatDef, ProjectileWeaponFirstPersonModeMaxFOVValues) == 0x128);
FABLE_STATIC_ASSERT(offsetof(CHeroCombatDef, ProjectileTargetingAcquireTargetTag) == 0x134);
FABLE_STATIC_ASSERT(offsetof(CHeroCombatDef, ProjectileTargetingAcquireTargetsAuto) == 0x135);
FABLE_STATIC_ASSERT(offsetof(CHeroCombatDef, ProjectileTargetingUseAnalogueZoom) == 0x136);
FABLE_STATIC_ASSERT(offsetof(CHeroCombatDef, ProjectileWeaponAnalogueZoomControlWeight) == 0x138);
FABLE_STATIC_ASSERT(offsetof(CHeroCombatDef, HeavyWeapons) == 0x13c);

#endif 

#endif

#endif

#ifdef FABLE_RETAIL_SHOP
#ifndef FABLE_ENGINE_EHEROEXPERIENCESTATCATEGORY_H
#define FABLE_ENGINE_EHEROEXPERIENCESTATCATEGORY_H
enum EHeroExperienceStatCategory { HERO_STAT_STRENGTH=0,HERO_STAT_SKILL=1,HERO_STAT_WILL=2,NUMBER_OF_HERO_STAT_CATEGORIES=3 };
#endif

#endif
class CHeroCombatDef;class CThingCreatureBase;class CHeroExperienceDef;class CHeroStatsDef;
class CTCHeroExperience:public CTCBase{
#ifdef FABLE_RETAIL_CTHING_CONSTRUCTORS
public: CTCHeroExperience(CThing&);
#endif
public:
    static ETCInterfaceType __fastcall StatGetInterfaceType(){return TCI_HERO_EXPERIENCE;}
    const CHeroCombatDef& PeekHeroCombatDef()const{return *PHeroCombatDef;}
#ifdef FABLE_RETAIL_SHOP
    void AddTrainableStatExperience(EHeroExperienceStatCategory,long);
#endif
public: virtual ETCInterfaceType GetInterfaceType() const;
public: virtual CCharString GetClassTypeName() const;
public: virtual void OnCreate();
public: virtual void OnInitialActivate();
public: virtual bool HasFrameUpdate() const;
public: virtual void FrameUpdate();
public: virtual bool IsComponentSerialisable() const;
public: virtual void DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo &);
public: virtual void Copy(const CTCBase &);
protected: virtual void OnSerialise(CPersistContext &,bool);
public: virtual ~CTCHeroExperience();
private:long FishingLevel,TotalExperiencePoints,ExperiencePointsAvailableToSpend;unsigned char StatExperiencePoints[12],TrainableStatLevels[12];float ExperienceMultiplierParam,LastExperienceMultiplierParam;long ExperienceAttenuationTimer;bool AbleToGainExperience;unsigned long Pickups;unsigned char ExperienceSpentOnStats[12];unsigned long AttackHistory;CDefPointer<const CHeroExperienceDef> PDef;CDefPointer<const CHeroCombatDef> PHeroCombatDef;CDefPointer<const CHeroStatsDef> PHeroStatsDef;
};
FABLE_STATIC_ASSERT(sizeof(CTCHeroExperience)==96);
#endif

#else
#ifndef FABLE_ENGINE_CTCHEROEXPERIENCE_H
#define FABLE_ENGINE_CTCHEROEXPERIENCE_H






#include <stddef.h>


class CDefPointeeBase;

#pragma pack(push, 1)
class CTCHeroExperience {
public:
    void*            __vftable;                              
    unsigned char    _base_0x04[0x8];                        
    long             FishingLevel;                           
    long             TotalExperiencePoints;                  
    long             ExperiencePointsAvailableToSpend;       
    unsigned char    StatExperiencePoints[0xc];              
    unsigned char    TrainableStatLevels[0xc];               
    float            ExperienceMultiplierParam;              
    float            LastExperienceMultiplierParam;          
    long             ExperienceAttenuationTimer;             
    bool             AbleToGainExperience;                   
    unsigned char    _pad_0x3d[0x3];                         
    unsigned char    Pickups[0x4];                           
    unsigned char    ExperienceSpentOnStats[0xc];            
    unsigned char    AttackHistory[0x4];                     
    CDefPointeeBase* PDef_Object;                            
    CDefPointeeBase* PHeroCombatDef_Object;                  
    unsigned char    PHeroStatsDef[0x4];                     
};
#pragma pack(pop)

FABLE_STATIC_ASSERT(sizeof(CTCHeroExperience) == 0x60);
FABLE_STATIC_ASSERT(offsetof(CTCHeroExperience, __vftable) == 0x0);
FABLE_STATIC_ASSERT(offsetof(CTCHeroExperience, FishingLevel) == 0xc);
FABLE_STATIC_ASSERT(offsetof(CTCHeroExperience, TotalExperiencePoints) == 0x10);
FABLE_STATIC_ASSERT(offsetof(CTCHeroExperience, ExperiencePointsAvailableToSpend) == 0x14);
FABLE_STATIC_ASSERT(offsetof(CTCHeroExperience, StatExperiencePoints) == 0x18);
FABLE_STATIC_ASSERT(offsetof(CTCHeroExperience, TrainableStatLevels) == 0x24);
FABLE_STATIC_ASSERT(offsetof(CTCHeroExperience, ExperienceMultiplierParam) == 0x30);
FABLE_STATIC_ASSERT(offsetof(CTCHeroExperience, LastExperienceMultiplierParam) == 0x34);
FABLE_STATIC_ASSERT(offsetof(CTCHeroExperience, ExperienceAttenuationTimer) == 0x38);
FABLE_STATIC_ASSERT(offsetof(CTCHeroExperience, AbleToGainExperience) == 0x3c);
FABLE_STATIC_ASSERT(offsetof(CTCHeroExperience, Pickups) == 0x40);
FABLE_STATIC_ASSERT(offsetof(CTCHeroExperience, ExperienceSpentOnStats) == 0x44);
FABLE_STATIC_ASSERT(offsetof(CTCHeroExperience, AttackHistory) == 0x50);
FABLE_STATIC_ASSERT(offsetof(CTCHeroExperience, PDef_Object) == 0x54);
FABLE_STATIC_ASSERT(offsetof(CTCHeroExperience, PHeroCombatDef_Object) == 0x58);
FABLE_STATIC_ASSERT(offsetof(CTCHeroExperience, PHeroStatsDef) == 0x5c);

#endif 

#endif

#ifndef FABLE_ENGINE_CTCHASTE_H
#define FABLE_ENGINE_CTCHASTE_H


class CTCHaste : public CTCBase {
private:
    float Multiplier;bool ToTerminate;long PauseFrames;
    unsigned char Multipliers[12]; 
public:
    static ETCInterfaceType __fastcall StatGetInterfaceType() { return TCI_HASTE; }
    void SetOnHitPause(long frames) { PauseFrames=frames; }
};
FABLE_STATIC_ASSERT(sizeof(CTCHaste)==0x24);

#endif

#ifdef FABLE_RETAIL_CTCBULLETTIME
#ifndef FABLE_ENGINE_CTCBULLETTIME_H
#define FABLE_ENGINE_CTCBULLETTIME_H

class CThingCreatureBase;

class CTCBulletTime : public CTCBase {
public: CTCBulletTime(CThing&);
private: unsigned char _pad_0x0c[0x4c];
public:
    static ETCInterfaceType __fastcall StatGetInterfaceType() { return TCI_BULLET_TIME; } void PlayCreatureOnHitEffect(CThingCreatureBase*);
};
FABLE_STATIC_ASSERT(sizeof(CTCBulletTime)==0x58);

#endif
#else
#ifndef FABLE_ENGINE_CTCBULLETTIME_H
#define FABLE_ENGINE_CTCBULLETTIME_H





#include <stddef.h>


class CDefPointeeBase;
struct CEnginePrimitive2DSprite;

#pragma pack(push, 1)
class CTCBulletTime {
public:
    void*                     __vftable;                     
    long                      f4;                            
    unsigned char             _pad_0x08[0x4];                
    long                      BulletTimeRemainingTicks;      
    long                      BulletTimeAbilityLevel;        
    CEnginePrimitive2DSprite* DisplacementMask;              
    unsigned char             DisplacementEffectPrimitive[0x8]; 
    CDefPointeeBase*          PDef_Object;                   
    long                      m_nLevel;                      
    long                      m_nTimeMultiplier;             
    float                     m_fDuration;                   
    unsigned char             m_tpLoopingEffect[0x8];        
    unsigned long             LoopSound;                     
    unsigned char             PItemRemoved[0x8];             
    unsigned char             HitCreatureBlur[0x8];          
    void*                     RadialBlurKey;                 
    void*                     ScreenFilterKey;               
    bool                      Pausable;                      
    bool                      Paused;                        
    bool                      CutSceneCancel;                
    unsigned char             _pad_0x57[0x1];                
};
#pragma pack(pop)

FABLE_STATIC_ASSERT(sizeof(CTCBulletTime) == 0x58);
FABLE_STATIC_ASSERT(offsetof(CTCBulletTime, __vftable) == 0x0);
FABLE_STATIC_ASSERT(offsetof(CTCBulletTime, f4) == 0x4);
FABLE_STATIC_ASSERT(offsetof(CTCBulletTime, BulletTimeRemainingTicks) == 0xc);
FABLE_STATIC_ASSERT(offsetof(CTCBulletTime, BulletTimeAbilityLevel) == 0x10);
FABLE_STATIC_ASSERT(offsetof(CTCBulletTime, DisplacementMask) == 0x14);
FABLE_STATIC_ASSERT(offsetof(CTCBulletTime, DisplacementEffectPrimitive) == 0x18);
FABLE_STATIC_ASSERT(offsetof(CTCBulletTime, PDef_Object) == 0x20);
FABLE_STATIC_ASSERT(offsetof(CTCBulletTime, m_nLevel) == 0x24);
FABLE_STATIC_ASSERT(offsetof(CTCBulletTime, m_nTimeMultiplier) == 0x28);
FABLE_STATIC_ASSERT(offsetof(CTCBulletTime, m_fDuration) == 0x2c);
FABLE_STATIC_ASSERT(offsetof(CTCBulletTime, m_tpLoopingEffect) == 0x30);
FABLE_STATIC_ASSERT(offsetof(CTCBulletTime, LoopSound) == 0x38);
FABLE_STATIC_ASSERT(offsetof(CTCBulletTime, PItemRemoved) == 0x3c);
FABLE_STATIC_ASSERT(offsetof(CTCBulletTime, HitCreatureBlur) == 0x44);
FABLE_STATIC_ASSERT(offsetof(CTCBulletTime, RadialBlurKey) == 0x4c);
FABLE_STATIC_ASSERT(offsetof(CTCBulletTime, ScreenFilterKey) == 0x50);
FABLE_STATIC_ASSERT(offsetof(CTCBulletTime, Pausable) == 0x54);
FABLE_STATIC_ASSERT(offsetof(CTCBulletTime, Paused) == 0x55);
FABLE_STATIC_ASSERT(offsetof(CTCBulletTime, CutSceneCancel) == 0x56);

#endif 

#endif

#ifndef FABLE_ENGINE_CTCCREATUREMODEMANAGER_H
#define FABLE_ENGINE_CTCCREATUREMODEMANAGER_H




class CTCCreatureModeManager : public CTCBase {
public:
    static ETCInterfaceType __fastcall StatGetInterfaceType() { return TCI_ENTITY_MODE_MANAGER; }
    class CModeBase {
    public:
        virtual ~CModeBase();
        virtual unsigned long GetType() const=0; 
    protected:
        CThing& ParentThing;
        unsigned char WeaponBasedFilter[12]; 
        long FramesBeforeRemovingSelf;bool Active;
    };
    CModeBase& GetCurrentMode() const;
    bool IsModeActive(unsigned long) const;
private:
    std::list<CCountedPointer<CModeBase> > PModes;
    long TimeLeftForCurrentMode,TimeForNextIdleMode;
    unsigned char AvailableIdleModes[12]; 
};
FABLE_STATIC_ASSERT(sizeof(CTCCreatureModeManager)==0x24);

#endif

#ifdef FABLE_RETAIL_COMBAT_TYPE
#ifndef FABLE_ENGINE_CTCCOMBAT_H
#define FABLE_ENGINE_CTCCOMBAT_H





#ifdef FABLE_RETAIL_COMBAT_TYPE
#ifndef FABLE_ENGINE_CCOMBATABILITY_H
#define FABLE_ENGINE_CCOMBATABILITY_H


class CCombatAbility {
public:CCombatAbility():RepeatTimer(-1),ActiveTimer(-1){}
private:long RepeatTimer;long ActiveTimer;
};
FABLE_STATIC_ASSERT(sizeof(CCombatAbility)==8);
#endif
#else
#ifndef FABLE_ENGINE_CCOMBATABILITY_H
#define FABLE_ENGINE_CCOMBATABILITY_H





#include <stddef.h>


#pragma pack(push, 1)
class CCombatAbility {
public:
    long RepeatTimer;                                        
    long ActiveTimer;                                        
};
#pragma pack(pop)

FABLE_STATIC_ASSERT(sizeof(CCombatAbility) == 0x8);
FABLE_STATIC_ASSERT(offsetof(CCombatAbility, RepeatTimer) == 0x0);
FABLE_STATIC_ASSERT(offsetof(CCombatAbility, ActiveTimer) == 0x4);

#endif 
#endif

#ifdef FABLE_RETAIL_COMBAT_TYPE
#ifndef FABLE_ENGINE_CCOMBATABILITYDATA_H
#define FABLE_ENGINE_CCOMBATABILITYDATA_H


#ifdef FABLE_RETAIL_COMBAT_TYPE
#ifndef FABLE_ENGINE_ECOMBATABILITYDATATYPE_H
#define FABLE_ENGINE_ECOMBATABILITYDATATYPE_H

enum ECombatAbilityDataType { COMBAT_ABILITY_DATA_NULL=0, COMBAT_ABILITY_DATA_BLOCK=1, COMBAT_ABILITY_DATA_MELEE_ATTACK=2, COMBAT_ABILITY_DATA_STRAFE=3, COMBAT_ABILITY_DATA_PROJECTILE_ATTACK=4 };
#endif
#endif

#ifdef FABLE_RETAIL_COMBAT_TYPE
#ifndef FABLE_ENGINE_ECOMBATABILITYINTERFACETYPE_H
#define FABLE_ENGINE_ECOMBATABILITYINTERFACETYPE_H

enum ECombatAbilityInterfaceType { COMBAT_ABILITY_INTERFACE_NULL=0, COMBAT_ABILITY_INTERFACE_BLOCK_UNARMED_MELEE_ATTACK=1, COMBAT_ABILITY_INTERFACE_BLOCK_LIGHT_MELEE_ATTACK=2, COMBAT_ABILITY_INTERFACE_BLOCK_HEAVY_MELEE_ATTACK=3, COMBAT_ABILITY_INTERFACE_BLOCK_PROJECTILE_ATTACK=4, COMBAT_ABILITY_INTERFACE_ATTACK_BLOCK_COUNTER=5, COMBAT_ABILITY_INTERFACE_ATTACK_FLOURISH_COUNTER=6, COMBAT_ABILITY_INTERFACE_ATTACK_PROJECTILE=7, COMBAT_ABILITY_INTERFACE_MOVEMENT_STRAFE=8, COMBAT_ABILITY_INTERFACE_ATTACK_GET_HIT_COUNTER=9 };
#endif
#endif

class CMemoryDataInputStream;class CDataOutputStream;
class CCombatAbilityData {
public:
 CCombatAbilityData(const CCombatAbilityData& p):PercentageChance(p.PercentageChance),SecondsDuration(p.SecondsDuration),SecondsToWaitBeforeRepeat(p.SecondsToWaitBeforeRepeat),Anim(p.Anim),InterfaceType(p.InterfaceType){}
 virtual ~CCombatAbilityData(){}
 long PercentageChance;float SecondsDuration;float SecondsToWaitBeforeRepeat;CDefString Anim;
 ECombatAbilityInterfaceType GetInterfaceType()const{return InterfaceType;}
private:ECombatAbilityInterfaceType InterfaceType;
public:
 virtual void TransferBinaryIn(CMemoryDataInputStream&,CCombatAbilityData&);
 virtual void TransferBinaryOut(CDataOutputStream&,const CCombatAbilityData&)const;
 virtual ECombatAbilityDataType GetCombatAbilityDataType()const{return COMBAT_ABILITY_DATA_NULL;}
};
FABLE_STATIC_ASSERT(sizeof(CCombatAbilityData)==0x18);
#endif
#else
#ifndef FABLE_ENGINE_CCOMBATABILITYDATA_H
#define FABLE_ENGINE_CCOMBATABILITYDATA_H





#include <stddef.h>


#pragma pack(push, 1)
class CCombatAbilityData {
public:
    void*         __vftable;                                 
    long          PercentageChance;                          
    float         SecondsDuration;                           
    float         SecondsToWaitBeforeRepeat;                 
    long          Anim_TablePos;                             
    unsigned char InterfaceType[0x4];                        
};
#pragma pack(pop)

FABLE_STATIC_ASSERT(sizeof(CCombatAbilityData) == 0x18);
FABLE_STATIC_ASSERT(offsetof(CCombatAbilityData, __vftable) == 0x0);
FABLE_STATIC_ASSERT(offsetof(CCombatAbilityData, PercentageChance) == 0x4);
FABLE_STATIC_ASSERT(offsetof(CCombatAbilityData, SecondsDuration) == 0x8);
FABLE_STATIC_ASSERT(offsetof(CCombatAbilityData, SecondsToWaitBeforeRepeat) == 0xc);
FABLE_STATIC_ASSERT(offsetof(CCombatAbilityData, Anim_TablePos) == 0x10);
FABLE_STATIC_ASSERT(offsetof(CCombatAbilityData, InterfaceType) == 0x14);

#endif 
#endif

#ifdef FABLE_RETAIL_COMBAT_TYPE
#ifndef FABLE_ENGINE_EUNSHEATHEWEAPONTYPE_H
#define FABLE_ENGINE_EUNSHEATHEWEAPONTYPE_H

enum EUnsheatheWeaponType { UNSHEATHE_NULL=0, UNSHEATHE_NO_WEAPON=1, UNSHEATHE_ALL_WEAPONS=2, UNSHEATHE_DEFAULT_WEAPON=3, UNSHEATHE_MELEE_WEAPON=4, UNSHEATHE_RANGED_WEAPON=5 };
#endif
#endif






#ifndef FABLE_STLPORT46_COMBAT_H
#define FABLE_STLPORT46_COMBAT_H
#ifndef FABLE_STLPORT_SET_H
#define FABLE_STLPORT_SET_H
#ifndef FABLE_STLPORT_MAP_H
#define FABLE_STLPORT_MAP_H



namespace std {
#ifdef FABLE_RETAIL_FACTIONS
template<class I>inline ptrdiff_t __fastcall __distance(const I& first,const I& last,const bidirectional_iterator_tag&){
    ptrdiff_t n=0;I it(first);while(it!=last){++it;++n;}return n;
}
template<class I>inline ptrdiff_t __fastcall distance(const I& first,const I& last){return __distance(first,last,bidirectional_iterator_tag());}
#endif
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
#if defined(FABLE_RETAIL_QUEST_COMPLETION) || defined(FABLE_RETAIL_ENTITY_PERSIST) || defined(FABLE_RETAIL_WASP) || defined(FABLE_RETAIL_SCRIPT_QUEST) || defined(FABLE_RETAIL_COMBAT_TYPE) || defined(FABLE_RETAIL_QUICK_ACCESS)
template<class Dummy> class _Rb_global {
#ifdef FABLE_RETAIL_SHOP_TREE_BODY
    typedef _Rb_tree_node_base* _Base_ptr;
#endif
public:
#if defined(FABLE_RETAIL_COMBAT_TYPE) || defined(FABLE_RETAIL_SHOP_TREE_BODY)
    static void __cdecl _Rebalance(_Rb_tree_node_base*,_Rb_tree_node_base*&);
#endif
#ifdef FABLE_RETAIL_SHOP_TREE_BODY
    static _Rb_tree_node_base* __cdecl _M_decrement(_Rb_tree_node_base* _M_node) {
  if (_M_node->_M_color == false && _M_node->_M_parent->_M_parent == _M_node)
    _M_node = _M_node->_M_right;
  else if (_M_node->_M_left != 0) {
    _Base_ptr __y = _M_node->_M_left;
    while (__y->_M_right != 0)
      __y = __y->_M_right;
    _M_node = __y;
  }
  else {
    _Base_ptr __y = _M_node->_M_parent;
    while (_M_node == __y->_M_left) {
      _M_node = __y;
      __y = __y->_M_parent;
    }
    _M_node = __y;
  }
  return _M_node;
}
#endif
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
#if defined(FABLE_RETAIL_QUEST_COMPLETION) || defined(FABLE_RETAIL_ENTITY_PERSIST) || defined(FABLE_RETAIL_WASP) || defined(FABLE_RETAIL_SCRIPT_QUEST) || defined(FABLE_RETAIL_COMBAT_TYPE) || defined(FABLE_RETAIL_QUICK_ACCESS)
#ifdef FABLE_RETAIL_SHOP_TREE_BODY
    _Rb_tree_iterator& operator--(){this->_M_node=_Rb_global_inst::_M_decrement(this->_M_node);return *this;}
#endif
    _Rb_tree_iterator& operator++(){this->_M_node=_Rb_global_inst::_M_increment(this->_M_node);return *this;}
#endif
};
template<class V,class A>struct _Rb_tree_base {
#if defined(FABLE_RETAIL_QUEST_COMPLETION) || defined(FABLE_RETAIL_ENTITY_PERSIST) || defined(FABLE_RETAIL_WASP) || defined(FABLE_RETAIL_SCRIPT_QUEST) || defined(FABLE_RETAIL_COMBAT_TYPE) || defined(FABLE_RETAIL_QUICK_ACCESS)
public:
#if defined(FABLE_RETAIL_CTHINGMANAGER_SAVE) || defined(FABLE_RETAIL_WASP) || defined(FABLE_RETAIL_SCRIPT_QUEST) || defined(FABLE_RETAIL_COMBAT_TYPE) || defined(FABLE_RETAIL_QUICK_ACCESS)
    _Rb_tree_base(const A& a):_M_header(allocator<_Rb_tree_node<V> >(),(_Rb_tree_node<V>*)0){_M_header._M_data=_M_header.allocate(1);}
#else
    _Rb_tree_base(const A&);
#endif
#if defined(FABLE_RETAIL_SCRIPT_QUEST) || defined(FABLE_RETAIL_COMBAT_TYPE) || defined(FABLE_RETAIL_QUICK_ACCESS)
    ~_Rb_tree_base(){_M_header.deallocate(_M_header._M_data,1);}
#else
    ~_Rb_tree_base();
#endif
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
#ifdef FABLE_RETAIL_COMBAT_TYPE
    iterator insert_equal(const V&);
private:
    iterator _M_insert(_Rb_tree_node_base*,_Rb_tree_node_base*,const V&,_Rb_tree_node_base* =0);
public:
#endif
#if defined(FABLE_RETAIL_QUEST_COMPLETION) || defined(FABLE_RETAIL_ENTITY_PERSIST) || defined(FABLE_RETAIL_WASP) || defined(FABLE_RETAIL_SCRIPT_QUEST) || defined(FABLE_RETAIL_COMBAT_TYPE) || defined(FABLE_RETAIL_QUICK_ACCESS)
#if defined(FABLE_RETAIL_CTHINGMANAGER_SAVE) || defined(FABLE_RETAIL_WASP) || defined(FABLE_RETAIL_SCRIPT_QUEST) || defined(FABLE_RETAIL_COMBAT_TYPE) || defined(FABLE_RETAIL_QUICK_ACCESS)
    _Rb_tree(const Compare& c,const A& a):_Rb_tree_base<V,A>(a),_M_node_count(0),_M_key_compare(c){_M_empty_initialize();}
    _Rb_tree& operator=(const _Rb_tree&);
    size_t size() const { return _M_node_count; }
private:
    void _M_empty_initialize(){this->_M_header._M_data->_M_color=false;this->_M_header._M_data->_M_parent=0;this->_M_header._M_data->_M_left=this->_M_header._M_data;this->_M_header._M_data->_M_right=this->_M_header._M_data;}
public:
#else
    _Rb_tree(const Compare&,const A&);
#endif
#if defined(FABLE_RETAIL_SCRIPT_QUEST) || defined(FABLE_RETAIL_COMBAT_TYPE) || defined(FABLE_RETAIL_QUICK_ACCESS)
    ~_Rb_tree(){clear();}
#else
    ~_Rb_tree();
#endif
    iterator begin(){return iterator(static_cast<_Rb_tree_node<V>*>(this->_M_header._M_data->_M_left));}
    const_iterator begin()const{return const_iterator(static_cast<_Rb_tree_node<V>*>(this->_M_header._M_data->_M_left));}
    iterator end(){return iterator(this->_M_header._M_data);}
#endif
#if defined(FABLE_RETAIL_SHOP) || defined(FABLE_RETAIL_SCRIPT_QUEST) || defined(FABLE_RETAIL_COMBAT_TYPE) || defined(FABLE_RETAIL_QUICK_ACCESS)
private:
    void _M_erase(_Rb_tree_node<V>*);
public:
    void clear() {
        if(_M_node_count!=0) {
            _M_erase(static_cast<_Rb_tree_node<V>*>(this->_M_header._M_data->_M_parent));
            this->_M_header._M_data->_M_left=this->_M_header._M_data;
            this->_M_header._M_data->_M_parent=0;
            this->_M_header._M_data->_M_right=this->_M_header._M_data;
            _M_node_count=0;
        }
    }
#ifndef FABLE_RETAIL_QUICK_ACCESS
    iterator find(const K&);
#endif
#endif
#if defined(FABLE_RETAIL_HERO_LOGBOOK) || defined(FABLE_RETAIL_SHOP_MAP_BODY) || defined(FABLE_RETAIL_QUICK_ACCESS) || defined(FABLE_RETAIL_FACTIONS)
protected:
    typedef _Rb_tree_node<V>* _Link_type;
#ifdef FABLE_RETAIL_SHOP_TREE_BODY
    _Link_type& _M_leftmost()const{return (_Link_type&)this->_M_header._M_data->_M_left;}
    _Link_type& _M_rightmost()const{return (_Link_type&)this->_M_header._M_data->_M_right;}
    static _Link_type& __fastcall _S_parent(_Link_type x){return (_Link_type&)x->_M_parent;}
    static _Link_type& __fastcall _S_left(_Rb_tree_node_base* x){return (_Link_type&)x->_M_left;}
    static _Link_type& __fastcall _S_right(_Rb_tree_node_base* x){return (_Link_type&)x->_M_right;}
    static const K& __fastcall _S_key(_Rb_tree_node_base* x){return KeyOfValue()(static_cast<_Link_type>(x)->_M_value_field);}
    _Link_type _M_create_node(const V& x){_Link_type p=this->_M_header.allocate(1);std::_Construct(&p->_M_value_field,x);return p;}
private:
    iterator _M_insert(_Rb_tree_node_base*,_Rb_tree_node_base*,const V&,_Rb_tree_node_base* =0);
public:
    size_t size()const{return _M_node_count;}
protected:
#endif
    _Link_type& _M_root()const { return (_Link_type&)this->_M_header._M_data->_M_parent; }
    static _Link_type& __fastcall _S_left(_Link_type x){return (_Link_type&)x->_M_left;}
    static _Link_type& __fastcall _S_right(_Link_type x){return (_Link_type&)x->_M_right;}
    static V& __fastcall _S_value(_Link_type x){return x->_M_value_field;}
    static const K& __fastcall _S_key(_Link_type x){return KeyOfValue()(_S_value(x));}
private:
    template<class Key> _Rb_tree_node<V>* _M_find(const Key& key)const {
        _Link_type y=this->_M_header._M_data;
        _Link_type x=_M_root();
        while(x!=0) {
            if(!_M_key_compare(_S_key(x),key)) {y=x;x=_S_left(x);}
            else x=_S_right(x);
        }
        if(y==this->_M_header._M_data || _M_key_compare(key,_S_key(y)))y=this->_M_header._M_data;
        return y;
    }
public:
#if defined(FABLE_RETAIL_HERO_LOGBOOK) || defined(FABLE_RETAIL_QUICK_ACCESS)
    iterator find(const K& key){return iterator(_M_find(key));}
#endif
    pair<iterator,bool> insert_unique(const V&);
    iterator insert_unique(iterator,const V&);
    Compare key_comp()const{return _M_key_compare;}
#ifdef FABLE_RETAIL_FACTIONS
    iterator lower_bound(const K& key){return iterator(_M_lower_bound(key));}
    iterator upper_bound(const K& key){return iterator(_M_upper_bound(key));}
#else
    iterator lower_bound(const K& key) {
        _Link_type y=this->_M_header._M_data;
        _Link_type x=_M_root();
        while(x!=0) {
            if(!_M_key_compare(_S_key(x),key)) {y=x;x=_S_left(x);}
            else x=_S_right(x);
        }
        return iterator(y);
    }
#endif
#endif
#ifdef FABLE_RETAIL_FACTIONS
private:
    _Link_type _M_lower_bound(const K& key)const{
        _Link_type y=this->_M_header._M_data;_Link_type x=_M_root();
        while(x!=0){if(!_M_key_compare(_S_key(x),key)){y=x;x=_S_left(x);}else x=_S_right(x);}return y;
    }
    _Link_type _M_upper_bound(const K& key)const{
        _Link_type y=this->_M_header._M_data;_Link_type x=_M_root();
        while(x!=0){if(_M_key_compare(key,_S_key(x))){y=x;x=_S_left(x);}else x=_S_right(x);}return y;
    }
public:
    pair<iterator,iterator> equal_range(const K& key){return pair<iterator,iterator>(lower_bound(key),upper_bound(key));}
    void erase(iterator,iterator);
    size_t erase(const K& key){pair<iterator,iterator> p=equal_range(key);size_t n=distance(p.first,p.second);erase(p.first,p.second);return n;}
#endif
    const_iterator find(const K&) const;
    const_iterator end()const{return const_iterator(this->_M_header._M_data);}
};
#if defined(FABLE_RETAIL_QUICK_ACCESS)
template<class K,class V,class KeyOfValue,class Compare,class A>
void _Rb_tree<K,V,KeyOfValue,Compare,A>::_M_erase(_Rb_tree_node<V>* x) {
    while(x!=0) {
        _M_erase(static_cast<_Rb_tree_node<V>*>(x->_M_right));
        _Rb_tree_node<V>* y=static_cast<_Rb_tree_node<V>*>(x->_M_left);
        _Destroy(&x->_M_value_field);
        this->_M_header.deallocate(x,1);
        x=y;
    }
}
#endif
template<class K,class V,class Compare=less<K>,class A=allocator<pair<const K,V> > >
class map {
public:typedef pair<const K,V> value_type;
private:
    typedef _Rb_tree<K,value_type,_Select1st<value_type>,Compare,A> _Rep_type;
    _Rep_type _M_t;
public:
    typedef typename _Rep_type::const_iterator const_iterator;
#if defined(FABLE_RETAIL_QUEST_COMPLETION) || defined(FABLE_RETAIL_ENTITY_PERSIST) || defined(FABLE_RETAIL_WASP) || defined(FABLE_RETAIL_SCRIPT_QUEST) || defined(FABLE_RETAIL_COMBAT_TYPE) || defined(FABLE_RETAIL_QUICK_ACCESS)
    typedef typename _Rep_type::iterator iterator;
    explicit map(const Compare& c=Compare(),const A& a=A()):_M_t(c,a){}
    iterator begin(){return _M_t.begin();}
    const_iterator begin()const{return _M_t.begin();}
    iterator end(){return _M_t.end();}
    V& operator[](const K&);
#ifdef FABLE_RETAIL_WASP
    ~map();
#endif
#if defined(FABLE_RETAIL_CTHINGMANAGER_SAVE) || defined(FABLE_RETAIL_WASP) || defined(FABLE_RETAIL_SCRIPT_QUEST) || defined(FABLE_RETAIL_COMBAT_TYPE) || defined(FABLE_RETAIL_QUICK_ACCESS)
    size_t size() const { return _M_t.size(); }
#endif
#endif
#if defined(FABLE_RETAIL_SHOP) || defined(FABLE_RETAIL_SCRIPT_QUEST) || defined(FABLE_RETAIL_COMBAT_TYPE) || defined(FABLE_RETAIL_QUICK_ACCESS)
    void clear(){_M_t.clear();}
    iterator find(const K& key){return _M_t.find(key);}
#endif
#if defined(FABLE_RETAIL_HERO_LOGBOOK) || defined(FABLE_RETAIL_SHOP_MAP_BODY) || defined(FABLE_RETAIL_QUICK_ACCESS)
#ifdef FABLE_RETAIL_HERO_LOGBOOK
    iterator find(const K& key){return _M_t.find(key);}
#endif
    pair<iterator,bool> insert(const value_type& value){return _M_t.insert_unique(value);}
    iterator insert(iterator position,const value_type& value){return _M_t.insert_unique(position,value);}
    iterator lower_bound(const K& key){return _M_t.lower_bound(key);}
    Compare key_comp()const{return _M_t.key_comp();}
#endif
    const_iterator find(const K& key)const{return _M_t.find(key);}
    const_iterator end()const{return _M_t.end();}
};
}
FABLE_STATIC_ASSERT(sizeof(std::map<unsigned long,long>)==12);
#endif

namespace std {
template<class T>struct _Identity:public unary_function<T,T>{const T& operator()(const T& value)const{return value;}};
template<class K,class Compare=less<K>,class A=allocator<K> >class set{
private:typedef _Rb_tree<K,K,_Identity<K>,Compare,A> _Rep_type;_Rep_type _M_t;
public:
    typedef typename _Rep_type::const_iterator iterator;
    set():_M_t(Compare(),A()){}
    pair<iterator,bool> insert(const K& value){typedef typename _Rep_type::iterator RepIterator;pair<RepIterator,bool> p=_M_t.insert_unique(value);return pair<iterator,bool>((const iterator&)p.first,p.second);}
    size_t erase(const K& value){return _M_t.erase(value);}
};
}
#endif

namespace std {
template<class T,class U>inline pair<T,U> __fastcall make_pair(const T& x,const U& y){return pair<T,U>(x,y);}
template<class K,class T,class Compare=less<K>,class A=allocator<pair<const K,T> > >class multimap {
public:typedef pair<const K,T> value_type;
private:typedef _Rb_tree<K,value_type,_Select1st<value_type>,Compare,A> _Rep_type;_Rep_type _M_t;
public:typedef typename _Rep_type::iterator iterator;
 multimap():_M_t(Compare(),A()){}
 iterator insert(const value_type& x){return _M_t.insert_equal(x);}
 void clear(){_M_t.clear();}
};
}
#endif

class CCombatManager;class CThingCreatureBase;class CCombatTypeDef;class CMeleeCombatAbilityDef;
class CTCCombat:public CTCBase {
public:void SetCombatType(const CDefPointer<const CCombatTypeDef>&);
private:
 long ComboChainPos;long ComboChainID;long ComboChainGracePeriodCountdown;long HandednessGracePeriodCountdown;long UnarmedCombatIdleCountdown;
 CCombatManager& CombatManager;CThingCreatureBase* ParentCreature;CDefPointer<const CCombatTypeDef> PCombatTypeDef;
 std::vector<CDefPointer<const CMeleeCombatAbilityDef> > AbilityList,UsableAbilityList;long CurrentAbilityIndex;
 CDefPointer<const CMeleeCombatAbilityDef> PBlockCounterAbility,PFlourish360Ability;long BlockCounterTimer;
 std::vector<CIntelligentPointer<const CThing> > AlwaysBlockThings;
 bool PerformingAction,PerformingStrikeAction,StartingToBlock,Blocking;long BlockHoldTimer,BlockingNumFrames;
 EUnsheatheWeaponType UnsheathingWeaponType;
 typedef std::pair<CCountedPointer<CCombatAbilityData>,CCombatAbility> TCombatAbilityData;
 typedef std::multimap<ECombatAbilityDataType,TCombatAbilityData> TCombatAbilityList;
 TCombatAbilityList CombatAbilities;long ShotOverrideType;bool ReadyForMelee;long CurFlourishTimer;
 std::vector<CIntelligentPointer<CThing> > FlourishParticles;bool IsInMiddleOfFlourishAction;
 CCombatManager& GetCombatManager(){return CombatManager;}
 CThingCreatureBase& GetParentCreature(){return *ParentCreature;}
};
FABLE_STATIC_ASSERT(sizeof(CTCCombat)==0x98);
#endif
#else
#ifdef FABLE_RETAIL_CTCCOMBAT
#ifndef FABLE_ENGINE_CTCCOMBAT_H
#define FABLE_ENGINE_CTCCOMBAT_H



class CTCCombat : public CTCBase {
private: unsigned char _pad_0x0c[0x8c];
public:
    static ETCInterfaceType __fastcall StatGetInterfaceType() { return TCI_COMBAT; } bool IsAbleToBeKnockedDown() const;
};
FABLE_STATIC_ASSERT(sizeof(CTCCombat)==0x98);

#endif
#else
#ifndef FABLE_ENGINE_CTCCOMBAT_H
#define FABLE_ENGINE_CTCCOMBAT_H






#include <stddef.h>


class CCombatManager;
class CDefPointeeBase;
class CThingCreatureBase;

#pragma pack(push, 1)
class CTCCombat {
public:
    void*               __vftable;                           
    long                obj;                                 
    unsigned char       _pad_0x08[0x4];                      
    long                ComboChainPos;                       
    long                ComboChainID;                        
    long                ComboChainGracePeriodCountdown;      
    long                HandednessGracePeriodCountdown;      
    long                UnarmedCombatIdleCountdown;          
    CCombatManager*     CombatManager;                       
    CThingCreatureBase* ParentCreature;                      
    CDefPointeeBase*    PCombatTypeDef_Object;               
    unsigned char       AbilityList[0xc];                    
    unsigned char       UsableAbilityList[0xc];              
    long                CurrentAbilityIndex;                 
    CDefPointeeBase*    PBlockCounterAbility_Object;         
    CDefPointeeBase*    PFlourish360Ability_Object;          
    long                BlockCounterTimer;                   
    unsigned char       AlwaysBlockThings[0xc];              
    bool                PerformingAction;                    
    bool                PerformingStrikeAction;              
    bool                StartingToBlock;                     
    bool                Blocking;                            
    long                BlockHoldTimer;                      
    long                BlockingNumFrames;                   
    unsigned char       UnsheathingWeaponType[0x4];          
    unsigned char       CombatAbilities[0xc];                
    long                ShotOverrideType;                    
    bool                ReadyForMelee;                       
    unsigned char       _pad_0x81[0x3];                      
    long                CurFlourishTimer;                    
    unsigned char       FlourishParticles[0xc];              
    bool                IsInMiddleOfFlourishAction;          
    unsigned char       _pad_0x95[0x3];                      
};
#pragma pack(pop)

FABLE_STATIC_ASSERT(sizeof(CTCCombat) == 0x98);
FABLE_STATIC_ASSERT(offsetof(CTCCombat, __vftable) == 0x0);
FABLE_STATIC_ASSERT(offsetof(CTCCombat, obj) == 0x4);
FABLE_STATIC_ASSERT(offsetof(CTCCombat, ComboChainPos) == 0xc);
FABLE_STATIC_ASSERT(offsetof(CTCCombat, ComboChainID) == 0x10);
FABLE_STATIC_ASSERT(offsetof(CTCCombat, ComboChainGracePeriodCountdown) == 0x14);
FABLE_STATIC_ASSERT(offsetof(CTCCombat, HandednessGracePeriodCountdown) == 0x18);
FABLE_STATIC_ASSERT(offsetof(CTCCombat, UnarmedCombatIdleCountdown) == 0x1c);
FABLE_STATIC_ASSERT(offsetof(CTCCombat, CombatManager) == 0x20);
FABLE_STATIC_ASSERT(offsetof(CTCCombat, ParentCreature) == 0x24);
FABLE_STATIC_ASSERT(offsetof(CTCCombat, PCombatTypeDef_Object) == 0x28);
FABLE_STATIC_ASSERT(offsetof(CTCCombat, AbilityList) == 0x2c);
FABLE_STATIC_ASSERT(offsetof(CTCCombat, UsableAbilityList) == 0x38);
FABLE_STATIC_ASSERT(offsetof(CTCCombat, CurrentAbilityIndex) == 0x44);
FABLE_STATIC_ASSERT(offsetof(CTCCombat, PBlockCounterAbility_Object) == 0x48);
FABLE_STATIC_ASSERT(offsetof(CTCCombat, PFlourish360Ability_Object) == 0x4c);
FABLE_STATIC_ASSERT(offsetof(CTCCombat, BlockCounterTimer) == 0x50);
FABLE_STATIC_ASSERT(offsetof(CTCCombat, AlwaysBlockThings) == 0x54);
FABLE_STATIC_ASSERT(offsetof(CTCCombat, PerformingAction) == 0x60);
FABLE_STATIC_ASSERT(offsetof(CTCCombat, PerformingStrikeAction) == 0x61);
FABLE_STATIC_ASSERT(offsetof(CTCCombat, StartingToBlock) == 0x62);
FABLE_STATIC_ASSERT(offsetof(CTCCombat, Blocking) == 0x63);
FABLE_STATIC_ASSERT(offsetof(CTCCombat, BlockHoldTimer) == 0x64);
FABLE_STATIC_ASSERT(offsetof(CTCCombat, BlockingNumFrames) == 0x68);
FABLE_STATIC_ASSERT(offsetof(CTCCombat, UnsheathingWeaponType) == 0x6c);
FABLE_STATIC_ASSERT(offsetof(CTCCombat, CombatAbilities) == 0x70);
FABLE_STATIC_ASSERT(offsetof(CTCCombat, ShotOverrideType) == 0x7c);
FABLE_STATIC_ASSERT(offsetof(CTCCombat, ReadyForMelee) == 0x80);
FABLE_STATIC_ASSERT(offsetof(CTCCombat, CurFlourishTimer) == 0x84);
FABLE_STATIC_ASSERT(offsetof(CTCCombat, FlourishParticles) == 0x88);
FABLE_STATIC_ASSERT(offsetof(CTCCombat, IsInMiddleOfFlourishAction) == 0x94);

#endif 

#endif
#endif

#ifdef FABLE_RETAIL_STATBOX
#ifndef FABLE_ENGINE_CPLAYERGUI_H
#define FABLE_ENGINE_CPLAYERGUI_H

#ifdef FABLE_RETAIL_CENTERED_TEXT
#ifndef FABLE_ENGINE_CFONTBANK_H
#define FABLE_ENGINE_CFONTBANK_H


#ifndef FABLE_RETAIL_HEADER_CWIDESTRING
#define FABLE_RETAIL_HEADER_CWIDESTRING
#ifdef FABLE_RETAIL_QUICK_ACCESS
#include <stddef.h>
#endif

#ifdef FABLE_RETAIL_HERO_LOGBOOK
#ifndef FABLE_ENGINE_CWIDESTRINGDATA_H
#define FABLE_ENGINE_CWIDESTRINGDATA_H
#ifndef FABLE_STLPORT_STRING_H
#define FABLE_STLPORT_STRING_H





#include "fable_containers.h"
#include <string.h>
#ifdef _MSC_VER
#pragma intrinsic(strlen,memcmp,memcpy,memset)
#endif
namespace std {
#ifdef FABLE_RETAIL_HERO_LOGBOOK

template<class C,class I> class __char_traits_base {
public:static int __cdecl compare(const C*,const C*,size_t);
};
template<class C> class char_traits:public __char_traits_base<C,C> {};
#else
template<class C> class char_traits;
#endif
template<> class char_traits<char> {
public:
    typedef char char_type;
    static void assign(char& a,const char& b) { a=b; }
    static size_t length(const char* p) { return strlen(p); }
    static int compare(const char* a,const char* b,size_t n) { return memcmp(a,b,n); }
    static char* copy(char* a,const char* b,size_t n) { return (char*)memmove(a,b,n); }
};
inline char* uninitialized_copy(const char* first,const char* last,char* result) {
    return (char*)__copy_trivial(first,last,result);
}
template<class T,class A> class _String_base {
public:
    typedef A allocator_type;
    T* _M_start;
    T* _M_finish;
    _STLP_alloc_proxy<T*,T,A> _M_end_of_storage;
    size_t max_size() const { return size_t(-1)/sizeof(T)-1; }
    void _M_throw_length_error() const;
    void _M_throw_out_of_range() const;
    void _M_allocate_block(size_t n) {
        if(n<=max_size()+1 && n>0) {
            _M_start=_M_end_of_storage.allocate(n);_M_finish=_M_start;_M_end_of_storage._M_data=_M_start+n;
        } else _M_throw_length_error();
    }
    void _M_deallocate_block() { _M_end_of_storage.deallocate(_M_start,_M_end_of_storage._M_data-_M_start); }
    _String_base(const A& a):_M_start(0),_M_finish(0),_M_end_of_storage(a,(T*)0) { }
    _String_base(const A& a,size_t n):_M_start(0),_M_finish(0),_M_end_of_storage(a,(T*)0) { _M_allocate_block(n); }
    ~_String_base() { _M_deallocate_block(); }
};
struct _String_reserve_t { };
template<class C,class Traits=char_traits<C>,class A=allocator<C> >
class basic_string:protected _String_base<C,A> {
    typedef basic_string<C,Traits,A> _Self;
    typedef _String_base<C,A> _Base;
public:
    typedef C* iterator;
    typedef const C* const_iterator;
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;
    typedef A allocator_type;
    typedef _String_reserve_t _Reserve_t;
    static C _M_null() { return C(); }
    enum { npos=-1 };
    allocator_type get_allocator() const { return this->_M_end_of_storage; }
    basic_string();
    basic_string(const C* s,const A& a=A());
    basic_string(const _Self& s);
    basic_string(_Reserve_t,size_t n,const A& a=A()):_Base(a,n+1) { _M_terminate_string(); }
    #ifdef FABLE_RETAIL_SCOREBOARD
    template<class I> basic_string(I f,I l,const A& a=A()):_Base(a) { _M_initialize_dispatch(f,l,__false_type()); }
#else
    template<class I> basic_string(I f,I l,const A& a=A()):_Base(a) { _M_range_initialize(f,l); }
#endif
    ~basic_string() { _Destroy(this->_M_start,this->_M_finish+1); }
    size_type size() const { return this->_M_finish-this->_M_start; }
    size_type capacity() const { return this->_M_end_of_storage._M_data-this->_M_start-1; }
    size_type max_size() const { return _Base::max_size(); }
    const C* c_str() const { return this->_M_start; }
    const C* data() const { return this->_M_start; }
    iterator begin() { return this->_M_start; }
    iterator end() { return this->_M_finish; }
    const_iterator begin() const { return this->_M_start; }
    const_iterator end() const { return this->_M_finish; }
    C& operator[](size_type n) { return *(this->_M_start+n); }
    const C& operator[](size_type n) const { return *(this->_M_start+n); }
    _Self& operator=(const _Self& s) { if(&s!=this) assign(s._M_start,s._M_finish); return *this; }
    _Self& operator=(const C* s) { return assign(s,s+Traits::length(s)); }
    _Self& assign(const C* first,const C* last);
    template<class I> _Self& assign(I first,I last) { return _M_assign_dispatch(first,last,__false_type()); }
    _Self& append(const _Self& s) { return append(s._M_start,s._M_finish); }
    _Self& append(const C* s) { return append(s,s+Traits::length(s)); }
    template<class I> _Self& append(I f,I l) { return _M_append_dispatch(f,l,__false_type()); }
    _Self& operator+=(const _Self& s) { return append(s); }
    _Self& operator+=(const C* s) { return append(s); }
    _Self& operator+=(C c) { push_back(c);return *this; }
    _Self substr(size_type pos=0,size_type n=npos) const {
        if(pos>size()) this->_M_throw_out_of_range();
        return _Self(this->_M_start+pos,this->_M_start+pos+(min)(n,size()-pos));
    }
    void reserve(size_type n=0);
    void push_back(C c) {
        if(this->_M_finish+1==this->_M_end_of_storage._M_data) reserve(size()+(max)(size(),size_type(1)));
        _M_construct_null(this->_M_finish+1);Traits::assign(*this->_M_finish,c);++this->_M_finish;
    }
private:
    void _M_construct_null(C* p) { *p=C(); }
    void _M_terminate_string() { _M_construct_null(this->_M_finish); }
    template<class I> void _M_range_initialize(I f,I l,const forward_iterator_tag&) {
        difference_type n=l-f;this->_M_allocate_block(n+1);
        this->_M_finish=uninitialized_copy(f,l,this->_M_start);_M_terminate_string();
    }
#ifdef FABLE_RETAIL_SCOREBOARD
    template<class I> void _M_initialize_dispatch(I f,I l,const __false_type&) { _M_range_initialize(f,l); }
    template<class I> void _M_range_initialize(I f,I l) { _M_range_initialize(f,l,random_access_iterator_tag()); }
#else
    template<class I> void _M_range_initialize(I f,I l) { _M_range_initialize(f,l,forward_iterator_tag()); }
#endif
    template<class I> _Self& _M_assign_dispatch(I f,I l,const __false_type&);
    template<class I> _Self& append(I f,I l,const forward_iterator_tag&);
    template<class I> _Self& _M_append_dispatch(I f,I l,const __false_type&) { return append(f,l,forward_iterator_tag()); }
};
template<class C,class T,class A> basic_string<C,T,A>::basic_string():_Base(allocator_type()) {
    this->_M_start=this->_M_end_of_storage.allocate(8);this->_M_finish=this->_M_start;
    this->_M_end_of_storage._M_data=this->_M_start+8;_M_terminate_string();
}
template<class C,class T,class A> basic_string<C,T,A>::basic_string(const C* s,const A& a):_Base(a) {
    _M_range_initialize(s,s+T::length(s));
}
template<class C,class T,class A> basic_string<C,T,A>::basic_string(const basic_string<C,T,A>& s):_Base(s.get_allocator()) {
    _M_range_initialize(s._M_start,s._M_finish);
}
template<class C,class T,class A> inline bool __fastcall operator==(const basic_string<C,T,A>& x,const basic_string<C,T,A>& y) {
    return x.size()==y.size()&&T::compare(x.data(),y.data(),x.size())==0;
}
template<class C,class T,class A> inline bool operator==(const basic_string<C,T,A>& s,const C* c) {
    size_t n=T::length(c);return s.size()==n&&T::compare(s.data(),c,n)==0;
}
template<class C,class T,class A> inline bool operator!=(const basic_string<C,T,A>& s,const C* c) { return !(s==c); }
template<class C,class T,class A> basic_string<C,T,A> operator+(const C*,const basic_string<C,T,A>&);
template<class C,class T,class A> basic_string<C,T,A> operator+(const basic_string<C,T,A>&,const C*);
typedef basic_string<char> string;
}
#endif



class CWideStringData {
protected:
    std::basic_string<unsigned short> Data;
public:
    long NoRefs;
    bool operator==(const CWideStringData& other)const { return Data==other.Data; }
};
FABLE_STATIC_ASSERT(sizeof(CWideStringData)==16);
#endif

#endif
class CCharString;
class CWideStringData;
class CWideString {
public:
#if defined(FABLE_RETAIL_CWIDESTRING_CTOR)
    CWideString();
#endif
#ifdef FABLE_RETAIL_CWIDESTRING_COPY
    CWideString(const CWideString&);
#endif
#if defined(FABLE_RETAIL_INVENTORY_MAP_SERIALISE) || defined(FABLE_RETAIL_SCOREBOARD) || defined(FABLE_RETAIL_SUBTITLE) || defined(FABLE_RETAIL_CTHINGMANAGER_SAVE)
    CCharString ToCharString() const;
#endif
#ifdef FABLE_RETAIL_CENTERED_TEXT
    operator const unsigned short*() const;
    const unsigned short* GetAsCharArray() const;
    bool operator!=(const unsigned short*) const;
    const CWideString& operator=(const CWideString&);
#endif
#ifdef FABLE_RETAIL_HERO_LOGBOOK
    bool operator==(const CWideString& other)const {
        if(PStringData==other.PStringData)return true;
        if(!PStringData || !other.PStringData)return false;
        return *PStringData==*other.PStringData;
    }
    bool Contains(const CWideString&)const;
#endif
#ifdef FABLE_RETAIL_QUICK_ACCESS
    const CWideString& operator+=(const CWideString&);
    void __cdecl Sprintf(const wchar_t*,...);
#endif
    ~CWideString();
protected:
#ifdef FABLE_RETAIL_HERO_LOGBOOK
    static long NoStrings;
    void UnassignString() { if(PStringData) { if(--PStringData->NoRefs<=0)delete PStringData;PStringData=0; } }
#endif
    CWideStringData* PStringData;
};
#endif

#ifndef FABLE_ENGINE_C2DEXTENTSF_H
#define FABLE_ENGINE_C2DEXTENTSF_H

class C2DExtentsF { public: float X,Y; C2DExtentsF(){} C2DExtentsF(float x,float y):X(x),Y(y){} C2DExtentsF(const C2DExtentsF& p):X(p.X),Y(p.Y){} };
#endif

class CFontManager;class C3DVector;class CRGBColour;class CTexture;
float __fastcall GFAXToVX(float);
float __fastcall GFAYToVY(float);

class CFontBank:public CBaseClassNonCopyable {
protected:unsigned long FontHeight;CCharString Name;CFontManager* PManager;
public:
 CFontBank(); virtual ~CFontBank();
#ifdef FABLE_RETAIL_STATBOX
 float GetStringWidth(const CWideString&)const;
#endif
 virtual C2DExtentsF GetStringExtents(const unsigned short*,bool)const=0;
 virtual float GetFontMaxHeight()const=0;
 virtual void PrepareToRender(bool)const=0;
 virtual bool Preload(const CWideString&,bool)const;
 virtual void Render(const CWideString&,const C3DVector&,float,const CRGBColour&,unsigned long,const CTexture*)const=0;
 C2DExtentsF GetStringExtents(const CWideString& text,bool b)const {return GetStringExtents(text.GetAsCharArray(),b);}
 C2DExtentsF GetVStringExtents(const CWideString& text)const { C2DExtentsF e=GetStringExtents(text,false); e.X=GFAXToVX(e.X); e.Y=GFAYToVY(e.Y); return e; }
 float GetVStringWidth(const CWideString& text)const {C2DExtentsF e=GetStringExtents(text,false);e.X=GFAXToVX(e.X);e.Y=GFAYToVY(e.Y);return e.X;}
 float GetVFontMaxHeight()const {return GFAYToVY(GetFontMaxHeight());}
};
FABLE_STATIC_ASSERT(sizeof(CFontBank)==0x10);
#endif

#else
#ifndef FABLE_ENGINE_CFONTBANK_H
#define FABLE_ENGINE_CFONTBANK_H





#include <stddef.h>


class CCharStringData;
class CFontManager;

#pragma pack(push, 1)
class CFontBank {
public:
    void*            __vftable;                              
    unsigned long    FontHeight;                             
    CCharStringData* Name_PStringData;                       
    CFontManager*    PManager;                               
};
#pragma pack(pop)

FABLE_STATIC_ASSERT(sizeof(CFontBank) == 0x10);
FABLE_STATIC_ASSERT(offsetof(CFontBank, __vftable) == 0x0);
FABLE_STATIC_ASSERT(offsetof(CFontBank, FontHeight) == 0x4);
FABLE_STATIC_ASSERT(offsetof(CFontBank, Name_PStringData) == 0x8);
FABLE_STATIC_ASSERT(offsetof(CFontBank, PManager) == 0xc);

#endif 

#endif

class CPlayerGuiDef; class CGraphicDataBank; class CWorld; class CPlayerManager;
class CGamePlayerInterface; class CMainGameComponent;


class CPlayerGui:public CTBaseSingletonNoDefaultAlloc<CPlayerGui> {
private:
 CWorld& World; CPlayerManager& PlayerManager; CGamePlayerInterface& InputInterface;
 const CMainGameComponent& Component; const CGraphicDataBank& Graphics;
 unsigned char SubtitleRenderer[0x120];
 CCountedPointer<CFontBank> PFont;
 unsigned char _pad_0x140[0x160];
 float Alpha; unsigned char _pad_0x2a4[4]; unsigned __int64 HeroCentreUID;
 unsigned char _pad_0x2b0[0x88];
public:
 CPlayerGui(const CMainGameComponent&,CWorld&,CGamePlayerInterface&,CPlayerManager&,const CGraphicDataBank&);
 virtual ~CPlayerGui();
 static const CPlayerGuiDef& __fastcall PeekGuiDef();
 const CGraphicDataBank& PeekGraphicBank()const{return Graphics;}
 const CFontBank& PeekFontBank()const{return *PFont;}
 unsigned char ScaleAlpha(unsigned char alpha)const{return (unsigned char)((float)alpha*Alpha);}
};
FABLE_STATIC_ASSERT(sizeof(CPlayerGui)==0x338);
#endif

#else
#ifdef FABLE_RETAIL_SUBTITLE
class CPlayerGuiDef;
#endif
#ifdef FABLE_RETAIL_CPLAYERGUI
#ifndef FABLE_ENGINE_CPLAYERGUI_H
#define FABLE_ENGINE_CPLAYERGUI_H

class CThing;class C3DVector;


class CPlayerGui : public CTBaseSingletonNoDefaultAlloc<CPlayerGui> {
private: unsigned char _pad_0x04[0x334];
public: void AddHitByMelee(CThing&,CThing&,const C3DVector&);
#ifdef FABLE_RETAIL_SUBTITLE
    static const CPlayerGuiDef& __fastcall PeekGuiDef();
#endif
};
FABLE_STATIC_ASSERT(sizeof(CPlayerGui)==0x338);

#endif
#else
#ifndef FABLE_ENGINE_CPLAYERGUI_H
#define FABLE_ENGINE_CPLAYERGUI_H






#include <stddef.h>


struct CAutoPickupBar;
class CCPPointerInfo;
struct CCrimeStatBar;
struct CDraw2DText;
struct CDrawAbilityButtons;
struct CDrawAutoPickups;
struct CDrawBettingHUD;
struct CDrawCombatMultiplier;
struct CDrawCrime;
struct CDrawDamage;
struct CDrawExperience;
struct CDrawFishingGame;
struct CDrawGuildSeal;
struct CDrawHealth;
struct CDrawHighlightingTargetingCursor;
struct CDrawLetterBoxBlackBars;
struct CDrawMeleeActionButtonEffects;
struct CDrawMiniMap;
struct CDrawMoney;
struct CDrawMorality;
struct CDrawMouseCursor;
struct CDrawNPCName;
struct CDrawPerceivers;
struct CDrawProjectileTargetingCentre;
struct CDrawQuestInfo;
struct CDrawQuickAccessIcons;
struct CDrawReceivedItems;
struct CDrawRenown;
struct CDrawScreenFade;
struct CDrawScriptScreenText;
struct CDrawSpiritScores;
struct CDrawSpiritTargetingCursor;
struct CDrawSpiritsHealth;
struct CDrawStamina;
struct CDrawStatUpdates;
struct CDrawTargetInfo;
struct CDrawWorldTime;
class CFontBank;
class CGamePlayerInterface;
class CGraphicDataBank;
class CMainGameComponent;
struct CManager;
struct CMultiStatBar;
class CPlayerManager;
struct CStatBar;
class CWorld;
struct IDirect3DTexture9;

#pragma pack(push, 1)
class CPlayerGui {
public:
#ifdef FABLE_RETAIL_SUBTITLE
    static const CPlayerGuiDef& __fastcall PeekGuiDef();
#endif
    void*                             __vftable;             
    CWorld*                           World;                 
    CPlayerManager*                   PlayerManager;         
    CGamePlayerInterface*             InputInterface;        
    CMainGameComponent*               Component;             
    CGraphicDataBank*                 Graphics;              
    unsigned char                     SubtitleRenderer[0x120]; 
    CFontBank*                        PFont_Data;            
    CCPPointerInfo*                   PFont_Info;            
    CDrawHealth*                      PDrawHealth_Data;      
    CCPPointerInfo*                   PDrawHealth_Info;      
    CDrawStamina*                     PDrawStamina_Data;     
    CCPPointerInfo*                   PDrawStamina_Info;     
    CDrawCombatMultiplier*            PDrawCombatMultiplier_Data; 
    CCPPointerInfo*                   PDrawCombatMultiplier_Info; 
    CDrawSpiritsHealth*               PDrawSpiritsHealth_Data; 
    CCPPointerInfo*                   PDrawSpiritsHealth_Info; 
    CDrawMiniMap*                     PDrawMiniMap_Data;     
    CCPPointerInfo*                   PDrawMiniMap_Info;     
    CDrawScreenFade*                  PDrawScreenFade_Data;  
    CCPPointerInfo*                   PDrawScreenFade_Info;  
    CDrawMoney*                       PDrawMoney_Data;       
    CCPPointerInfo*                   PDrawMoney_Info;       
    CDrawRenown*                      PDrawRenown_Data;      
    CCPPointerInfo*                   PDrawRenown_Info;      
    CDrawMorality*                    PDrawMorality_Data;    
    CCPPointerInfo*                   PDrawMorality_Info;    
    CDrawExperience*                  PDrawExperience_Data;  
    CCPPointerInfo*                   PDrawExperience_Info;  
    CDrawCrime*                       PDrawCrime_Data;       
    CCPPointerInfo*                   PDrawCrime_Info;       
    CDrawReceivedItems*               PDrawReceivedItems_Data; 
    CCPPointerInfo*                   PDrawReceivedItems_Info; 
    CDrawFishingGame*                 PDrawFishingGame_Data; 
    CCPPointerInfo*                   PDrawFishingGame_Info; 
    CDrawStatUpdates*                 PDrawStatUpdates_Data; 
    CCPPointerInfo*                   PDrawStatUpdates_Info; 
    CDrawAutoPickups*                 PDrawAutoPickups_Data; 
    CCPPointerInfo*                   PDrawAutoPickups_Info; 
    CDrawPerceivers*                  PDrawPerceivers_Data;  
    CCPPointerInfo*                   PDrawPerceivers_Info;  
    CDrawDamage*                      PDrawDamage_Data;      
    CCPPointerInfo*                   PDrawDamage_Info;      
    CDrawQuestInfo*                   PDrawQuestInfo_Data;   
    CCPPointerInfo*                   PDrawQuestInfo_Info;   
    CDrawGuildSeal*                   PDrawGuildSeal_Data;   
    CCPPointerInfo*                   PDrawGuildSeal_Info;   
    CDrawQuickAccessIcons*            PDrawQuickAccessIcons_Data; 
    CCPPointerInfo*                   PDrawQuickAccessIcons_Info; 
    CDrawScriptScreenText*            PDrawScriptScreenText_Data; 
    CCPPointerInfo*                   PDrawScriptScreenText_Info; 
    CDrawTargetInfo*                  PDrawTargetInfo_Data;  
    CCPPointerInfo*                   PDrawTargetInfo_Info;  
    CDrawHighlightingTargetingCursor* PDrawHighlightingTargetingCursor_Data; 
    CCPPointerInfo*                   PDrawHighlightingTargetingCursor_Info; 
    CDrawWorldTime*                   PDrawWorldTime_Data;   
    CCPPointerInfo*                   PDrawWorldTime_Info;   
    CDrawAbilityButtons*              PDrawAbilityButtons_Data; 
    CCPPointerInfo*                   PDrawAbilityButtons_Info; 
    CDrawLetterBoxBlackBars*          PDrawLetterBoxBlackBars_Data; 
    CCPPointerInfo*                   PDrawLetterBoxBlackBars_Info; 
    CDrawBettingHUD*                  PDrawBettingHUD_Data;  
    CCPPointerInfo*                   PDrawBettingHUD_Info;  
    CDrawProjectileTargetingCentre*   PDrawProjectileTargetingCentre_Data; 
    CCPPointerInfo*                   PDrawProjectileTargetingCentre_Info; 
    CDrawMeleeActionButtonEffects*    PDrawMeleeActionButtonEffects_Data; 
    CCPPointerInfo*                   PDrawMeleeActionButtonEffects_Info; 
    CDrawSpiritScores*                PDrawSpiritScores_Data; 
    CCPPointerInfo*                   PDrawSpiritScores_Info; 
    CDrawSpiritTargetingCursor*       PDrawSpiritTargetingCursor_Data; 
    CCPPointerInfo*                   PDrawSpiritTargetingCursor_Info; 
    CDraw2DText*                      PDraw2DText_Data;      
    CCPPointerInfo*                   PDraw2DText_Info;      
    CDrawMouseCursor*                 PDrawMouseCursor_Data; 
    CCPPointerInfo*                   PDrawMouseCursor_Info; 
    CDrawNPCName*                     PDrawNPCName_Data;     
    CCPPointerInfo*                   PDrawNPCName_Info;     
    bool                              ShowSpecialAbilityButtons; 
    bool                              ShowSpecialAbilityButtonsAbilities; 
    bool                              SpecialSelected;       
    bool                              SpecialDeselected;     
    unsigned char                     AbilityButtons[0xc];   
    unsigned char                     AbilityButtonsAlpha[0xc];  
    unsigned char                     QuickAccessButtonsAlpha[0xc];  
    float                             AbilityButtonScale;    
    bool                              NormalToSpecial;       
    bool                              SpecialToNormal;       
    unsigned char                     _pad_0x27e[0x2];       
    float                             VWindow_TLX;           
    float                             VWindow_TLY;           
    float                             VWindow_BRX;           
    float                             VWindow_BRY;           
    bool                              DrawGui;               
    bool                              OldDrawGui;            
    unsigned char                     _pad_0x292[0x2];       
    unsigned char                     GuiMode[0x4];          
    float                             FadeInTimer;           
    bool                              ForcedClosed;          
    bool                              ShowTime;              
    bool                              ShowMoneyBag;          
    bool                              ShowStatBars;          
    float                             Alpha;                 
    unsigned char                     _pad_0x2a4[0x4];       
    unsigned __int64                  HeroCentreUID;         
    bool                              DrawingSafeAreas;      
    bool                              DrawingDebugTGA;       
    unsigned char                     _pad_0x2b2[0x2];       
    unsigned char                     DebugTGATexture[0x8];  
    bool                              ScreenMessagingEnabled; 
    bool                              ForceAllowSubtitleToRender; 
    bool                              LiveGuiMode;           
    unsigned char                     _pad_0x2bf[0x1];       
    unsigned char                     Illustrators[0xc];     
    CStatBar*                         PMoney_Data;           
    CCPPointerInfo*                   PMoney_Info;           
    CStatBar*                         PRenown_Data;          
    CCPPointerInfo*                   PRenown_Info;          
    CStatBar*                         PMorality_Data;        
    CCPPointerInfo*                   PMorality_Info;        
    CStatBar*                         PExperience_Data;      
    CCPPointerInfo*                   PExperience_Info;      
    CCrimeStatBar*                    PCrime_Data;           
    CCPPointerInfo*                   PCrime_Info;           
    CMultiStatBar*                    PStatUpdates_Data;     
    CCPPointerInfo*                   PStatUpdates_Info;     
    CAutoPickupBar*                   PAutoPickups_Data;     
    CCPPointerInfo*                   PAutoPickups_Info;     
    unsigned char                     TextPrimitiveHandles[0xc];  
    unsigned char                     SafeAreaPrimitiveHandles[0xc];  
    unsigned char                     DebugTGAPrimitiveHandle[0x8]; 
    unsigned char                     ScriptScreenText[0x4]; 
    unsigned char                     ScreenMessages[0x4];   
    CManager*                         PHeroInfoScreensManager_ptr; 
    bool                              DisplayBottomHalfGUI;  
    unsigned char                     _pad_0x331[0x7];       
};
#pragma pack(pop)

FABLE_STATIC_ASSERT(sizeof(CPlayerGui) == 0x338);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, __vftable) == 0x0);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, World) == 0x4);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PlayerManager) == 0x8);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, InputInterface) == 0xc);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, Component) == 0x10);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, Graphics) == 0x14);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, SubtitleRenderer) == 0x18);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PFont_Data) == 0x138);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PFont_Info) == 0x13c);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PDrawHealth_Data) == 0x140);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PDrawHealth_Info) == 0x144);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PDrawStamina_Data) == 0x148);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PDrawStamina_Info) == 0x14c);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PDrawCombatMultiplier_Data) == 0x150);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PDrawCombatMultiplier_Info) == 0x154);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PDrawSpiritsHealth_Data) == 0x158);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PDrawSpiritsHealth_Info) == 0x15c);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PDrawMiniMap_Data) == 0x160);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PDrawMiniMap_Info) == 0x164);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PDrawScreenFade_Data) == 0x168);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PDrawScreenFade_Info) == 0x16c);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PDrawMoney_Data) == 0x170);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PDrawMoney_Info) == 0x174);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PDrawRenown_Data) == 0x178);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PDrawRenown_Info) == 0x17c);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PDrawMorality_Data) == 0x180);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PDrawMorality_Info) == 0x184);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PDrawExperience_Data) == 0x188);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PDrawExperience_Info) == 0x18c);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PDrawCrime_Data) == 0x190);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PDrawCrime_Info) == 0x194);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PDrawReceivedItems_Data) == 0x198);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PDrawReceivedItems_Info) == 0x19c);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PDrawFishingGame_Data) == 0x1a0);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PDrawFishingGame_Info) == 0x1a4);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PDrawStatUpdates_Data) == 0x1a8);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PDrawStatUpdates_Info) == 0x1ac);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PDrawAutoPickups_Data) == 0x1b0);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PDrawAutoPickups_Info) == 0x1b4);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PDrawPerceivers_Data) == 0x1b8);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PDrawPerceivers_Info) == 0x1bc);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PDrawDamage_Data) == 0x1c0);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PDrawDamage_Info) == 0x1c4);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PDrawQuestInfo_Data) == 0x1c8);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PDrawQuestInfo_Info) == 0x1cc);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PDrawGuildSeal_Data) == 0x1d0);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PDrawGuildSeal_Info) == 0x1d4);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PDrawQuickAccessIcons_Data) == 0x1d8);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PDrawQuickAccessIcons_Info) == 0x1dc);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PDrawScriptScreenText_Data) == 0x1e0);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PDrawScriptScreenText_Info) == 0x1e4);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PDrawTargetInfo_Data) == 0x1e8);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PDrawTargetInfo_Info) == 0x1ec);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PDrawHighlightingTargetingCursor_Data) == 0x1f0);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PDrawHighlightingTargetingCursor_Info) == 0x1f4);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PDrawWorldTime_Data) == 0x1f8);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PDrawWorldTime_Info) == 0x1fc);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PDrawAbilityButtons_Data) == 0x200);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PDrawAbilityButtons_Info) == 0x204);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PDrawLetterBoxBlackBars_Data) == 0x208);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PDrawLetterBoxBlackBars_Info) == 0x20c);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PDrawBettingHUD_Data) == 0x210);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PDrawBettingHUD_Info) == 0x214);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PDrawProjectileTargetingCentre_Data) == 0x218);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PDrawProjectileTargetingCentre_Info) == 0x21c);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PDrawMeleeActionButtonEffects_Data) == 0x220);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PDrawMeleeActionButtonEffects_Info) == 0x224);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PDrawSpiritScores_Data) == 0x228);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PDrawSpiritScores_Info) == 0x22c);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PDrawSpiritTargetingCursor_Data) == 0x230);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PDrawSpiritTargetingCursor_Info) == 0x234);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PDraw2DText_Data) == 0x238);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PDraw2DText_Info) == 0x23c);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PDrawMouseCursor_Data) == 0x240);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PDrawMouseCursor_Info) == 0x244);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PDrawNPCName_Data) == 0x248);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PDrawNPCName_Info) == 0x24c);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, ShowSpecialAbilityButtons) == 0x250);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, ShowSpecialAbilityButtonsAbilities) == 0x251);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, SpecialSelected) == 0x252);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, SpecialDeselected) == 0x253);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, AbilityButtons) == 0x254);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, AbilityButtonsAlpha) == 0x260);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, QuickAccessButtonsAlpha) == 0x26c);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, AbilityButtonScale) == 0x278);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, NormalToSpecial) == 0x27c);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, SpecialToNormal) == 0x27d);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, VWindow_TLX) == 0x280);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, VWindow_TLY) == 0x284);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, VWindow_BRX) == 0x288);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, VWindow_BRY) == 0x28c);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, DrawGui) == 0x290);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, OldDrawGui) == 0x291);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, GuiMode) == 0x294);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, FadeInTimer) == 0x298);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, ForcedClosed) == 0x29c);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, ShowTime) == 0x29d);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, ShowMoneyBag) == 0x29e);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, ShowStatBars) == 0x29f);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, Alpha) == 0x2a0);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, HeroCentreUID) == 0x2a8);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, DrawingSafeAreas) == 0x2b0);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, DrawingDebugTGA) == 0x2b1);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, DebugTGATexture) == 0x2b4);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, ScreenMessagingEnabled) == 0x2bc);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, ForceAllowSubtitleToRender) == 0x2bd);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, LiveGuiMode) == 0x2be);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, Illustrators) == 0x2c0);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PMoney_Data) == 0x2cc);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PMoney_Info) == 0x2d0);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PRenown_Data) == 0x2d4);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PRenown_Info) == 0x2d8);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PMorality_Data) == 0x2dc);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PMorality_Info) == 0x2e0);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PExperience_Data) == 0x2e4);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PExperience_Info) == 0x2e8);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PCrime_Data) == 0x2ec);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PCrime_Info) == 0x2f0);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PStatUpdates_Data) == 0x2f4);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PStatUpdates_Info) == 0x2f8);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PAutoPickups_Data) == 0x2fc);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PAutoPickups_Info) == 0x300);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, TextPrimitiveHandles) == 0x304);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, SafeAreaPrimitiveHandles) == 0x310);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, DebugTGAPrimitiveHandle) == 0x31c);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, ScriptScreenText) == 0x324);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, ScreenMessages) == 0x328);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PHeroInfoScreensManager_ptr) == 0x32c);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, DisplayBottomHalfGUI) == 0x330);

#endif 

#endif

#endif

#ifdef FABLE_RETAIL_CPLAYERMANAGER
#ifndef FABLE_ENGINE_CPLAYERMANAGER_H
#define FABLE_ENGINE_CPLAYERMANAGER_H

class CThing; class CPlayer;class CMainGameComponent;class CGameDefinitionManager;

class CPlayerManager : public CBaseClassNonCopyable {
private: unsigned char _pad_0x04[0x28];
public: bool IsThingMainPlayer(const CThing&) const;
#if defined(FABLE_RETAIL_CTHINGMANAGER_SAVE) || (defined(FABLE_RETAIL_SHOP) || defined(FABLE_RETAIL_HERO_LOGBOOK) || defined(FABLE_RETAIL_INVENTORY_MAP_SERIALISE))
    CPlayerManager(CMainGameComponent&,const CGameDefinitionManager&);
    CPlayer& GetMainPlayer();
#endif
};
FABLE_STATIC_ASSERT(sizeof(CPlayerManager)==0x2c);

#endif
#else
#ifndef FABLE_ENGINE_CPLAYERMANAGER_H
#define FABLE_ENGINE_CPLAYERMANAGER_H








#include <stddef.h>


class CGameDefinitionManager;
class CMainGameComponent;
class CPlayer;

#pragma pack(push, 1)
class CPlayerManager {
public:
    void*                   __vftable;                       
    CMainGameComponent*     Component;                       
    CGameDefinitionManager* DefinitionManager;               
    unsigned char           Players[0xc];                    
    long                    PlayerNeutral;                   
    long                    MainPlayer;                      
    unsigned char           HeroSwapPlayerScriptNames[0xc];  
    
    
    
    
    
    
    
    
    CPlayer&       GetPlayer(long number);                   
    const CPlayer& GetPlayer(long number) const;             
    CPlayer&       GetMainPlayer();                          
    const CPlayer& GetMainPlayer() const;                    
    long           GetMainPlayerNumber() const;              
};
#pragma pack(pop)

FABLE_STATIC_ASSERT(sizeof(CPlayerManager) == 0x2c);
FABLE_STATIC_ASSERT(offsetof(CPlayerManager, __vftable) == 0x0);
FABLE_STATIC_ASSERT(offsetof(CPlayerManager, Component) == 0x4);
FABLE_STATIC_ASSERT(offsetof(CPlayerManager, DefinitionManager) == 0x8);
FABLE_STATIC_ASSERT(offsetof(CPlayerManager, Players) == 0xc);
FABLE_STATIC_ASSERT(offsetof(CPlayerManager, PlayerNeutral) == 0x18);
FABLE_STATIC_ASSERT(offsetof(CPlayerManager, MainPlayer) == 0x1c);
FABLE_STATIC_ASSERT(offsetof(CPlayerManager, HeroSwapPlayerScriptNames) == 0x20);

#endif 

#endif

#ifndef FABLE_ENGINE_NCREATUREMODE_H
#define FABLE_ENGINE_NCREATUREMODE_H

namespace NCreatureMode { enum EMode {
    MODE_NULL=0,
    MODE_STAND=1,
    MODE_STAND_HAPPY=2,
    MODE_STAND_BORED=3,
    MODE_CARRYING_WEAPON_STAND=4,
    MODE_CARRYING_PLATE_STAND=5,
    MODE_CARRYING_GOODS_STAND=6,
    MODE_SNEAK=7,
    MODE_SIT_CHAIR=8,
    MODE_SIT_BENCH=9,
    MODE_SIT_FLOOR=10,
    MODE_SIT_TAVERN_STOOL=11,
    MODE_BLOCK=12,
    MODE_SLEEP_BED=13,
    MODE_COOK=14,
    MODE_SCARED=15,
    MODE_OUTRAGED=16,
    MODE_CREEPING=17,
    MODE_CHEERING=18,
    MODE_HECKLING=19,
    MODE_SAD=20,
    MODE_ATTRACTED=21,
    MODE_HERO_IMITATION=22,
    MODE_CURIOUS=23,
    MODE_BULLY=24,
    MODE_STRAFE=25,
    MODE_HOVER=26,
    MODE_CONVERSATION=27,
    MODE_STAND_IN_ALL_DIRECTIONS=28,
    MODE_STUNNED=29,
    MODE_UNDERGROUND=30,
    MODE_KNOCKED_DOWN=31,
    MODE_FORCE_PUSHED=32,
    MODE_FISHING=33,
    MODE_CARRYING_GENERIC_BOX=34,
    MODE_BATTLE_CHARGE=35,
    MODE_BERSERK=36,
    MODE_ASSASSIN_RUSH=37,
    MODE_WADING=38,
    MODE_DRINKING_FROM_TANKARD=39,
    MODE_EATING_AT_TABLE=40,
    MODE_HOLD_JUG=41,
    MODE_SERVE_CUSTOMER=42,
    MODE_FILL_JUG=43,
    MODE_DRUNK=44,
    MODE_ARMS_OFF_TABLE=45,
    MODE_HANDS_BOUND=46,
    MODE_SCORPION_KING_TAIL_PLUNGE=47,
    MODE_JACK_OF_BLADES_SUMMON=48,
    MODE_JACK_OF_BLADES_FLOAT=49,
    MODE_JACK_OF_BLADES_FLASH=50,
    MODE_JACK_OF_BLADES_SCREAM=51,
    MODE_JACK_OF_BLADES_VULNERABLE=52,
    MODE_JACK_OF_BLADES_DEATH=53,
    MODE_THUNDER_LIGHTNING_STORM=54,
    MODE_SCORPION_KING_VULNERABLE=55,
    MODE_SCORPION_KING_EXHAUSTED=56,
    MODE_CREATURE_IS_PROJECTILE=57,
    MODE_ROCK_TROLL_VULNERABLE=58,
    MODE_GUILDMASTER_CONCENTRATE=59,
    MODE_GUILDMASTER_POWERUP_HEAL=60,
    MODE_GUILDMASTER_POWERUP_SHIELD=61,
    MODE_GUILDMASTER_POWERUP_SPEED=62,
    MODE_BRIAR_ROSE_CONCENTRATE=63,
    MODE_BRIAR_ROSE_INVULNERABLE=64,
    MODE_BRIAR_ROSE_INTANGIBLE=65,
    MODE_EXPLOSIVE_FORCE_PUSHED=66,
    MODE_DAZED=67,
    MODE_SCREAMER_AGGRESSIVE=68,
    MODE_DRAINED=69,
    MODE_ROLL=70,
    MODE_DRAGON_HOVER=71,
    MODE_DRAGON_SWOOP=72,
    MODE_DRAGON_DEFAULT=73,
    MODE_DRAGON_REAR_UP=74,
    MODE_DRAGON_FIRE_BEAM=75,
    MODE_DRAGON_STRAFE=76,
    MODE_DRAGON_STRAFE_BURNINATE=77,
    MODE_DRAGON_LANDED=78,
    MODE_DRAGON_GET_HIT=79,
    MODE_DRAGON_FLYING=80,
    MODE_INTANGIBLE=81,
    MODE_DRAGON_HOVER_NAPALM_GROUND=82,
    MODE_VULNERABLE=83,
    MODE_UNDERWATER=84,
    MODE_DRAGON_HOVER_CHARGE_UP_NAPALM_GROUND=85,
    MODE_CAGED=86,
    MODE_SCRIPTED=87,
    MODE_BASIC=88,
    MODE_SUMMONER_SUMMON=89,
    MODE_SUMMONER_LIGHTNING_ORB=90,
    MODE_CHARGE_FORCE_PUSH=91,
    MODE_SUMMONER_DIE=92,
    MODE_ICE_TROLL_ICICLE_ASSAULT=93,
    MODE_SCREAMER_SUPER_ATTACK=94,
    MODE_HIT_BY_EPIC_SPELL=95
}; }
#endif

#ifdef FABLE_RETAIL_DEFINITION_MANAGER
#ifndef FABLE_ENGINE_CDEFINITIONMANAGER_H
#define FABLE_ENGINE_CDEFINITIONMANAGER_H
#ifdef FABLE_RETAIL_DEFINITION_MANAGER
#ifndef FABLE_ENGINE_CRESOURCELIST_H
#define FABLE_ENGINE_CRESOURCELIST_H
#ifndef FABLE_ENGINE_CFAILEDALLOCATIONHANDLER_H
#define FABLE_ENGINE_CFAILEDALLOCATIONHANDLER_H


class CFailedAllocationHandler {
public:
    virtual ~CFailedAllocationHandler();
    virtual bool HandleFailedAllocation(unsigned long)=0;
};
FABLE_STATIC_ASSERT(sizeof(CFailedAllocationHandler)==4);
#endif




class CResourceList : public CFailedAllocationHandler {
public:
    virtual ~CResourceList();
    virtual bool HandleFailedAllocation(unsigned long);
private:
    CResource Head;
    unsigned long ResourceCount;
    long AllocatedMemory,MaximumMemory;
    unsigned long CurrentFrame,DebugStatsFrame,UnloadDelay,UnloadedThisFrame;
};
FABLE_STATIC_ASSERT(sizeof(CResourceList)==0x3c);
#endif
#else
#ifndef FABLE_ENGINE_CRESOURCELIST_H
#define FABLE_ENGINE_CRESOURCELIST_H





#include <stddef.h>


#pragma pack(push, 1)
class CResourceList {
public:
    void*         __vftable;                                 
    unsigned char Head[0x1c];                                
    unsigned long ResourceCount;                             
    long          AllocatedMemory;                           
    long          MaximumMemory;                             
    unsigned long CurrentFrame;                              
    unsigned long DebugStatsFrame;                           
    unsigned long UnloadDelay;                               
    unsigned long UnloadedThisFrame;                         
};
#pragma pack(pop)

FABLE_STATIC_ASSERT(sizeof(CResourceList) == 0x3c);
FABLE_STATIC_ASSERT(offsetof(CResourceList, __vftable) == 0x0);
FABLE_STATIC_ASSERT(offsetof(CResourceList, Head) == 0x4);
FABLE_STATIC_ASSERT(offsetof(CResourceList, ResourceCount) == 0x20);
FABLE_STATIC_ASSERT(offsetof(CResourceList, AllocatedMemory) == 0x24);
FABLE_STATIC_ASSERT(offsetof(CResourceList, MaximumMemory) == 0x28);
FABLE_STATIC_ASSERT(offsetof(CResourceList, CurrentFrame) == 0x2c);
FABLE_STATIC_ASSERT(offsetof(CResourceList, DebugStatsFrame) == 0x30);
FABLE_STATIC_ASSERT(offsetof(CResourceList, UnloadDelay) == 0x34);
FABLE_STATIC_ASSERT(offsetof(CResourceList, UnloadedThisFrame) == 0x38);

#endif 

#endif




class CDefClassBase;
#ifdef FABLE_RETAIL_SHOP
class CDefString;
#endif
class CDefinitionManager : public CResourceList {
    friend class CDefClassBase;
public:
    virtual ~CDefinitionManager();
#if defined(FABLE_RETAIL_HERO_LOGBOOK) || defined(FABLE_RETAIL_FACTIONS)
    long GetGlobalIndexFromClassIndex(const CCharString&,long) const;
    template<class T> bool GetDef(long,CDefPointer<const T>*) const;
#endif
#ifdef FABLE_RETAIL_SHOP
    CDefString GetDefNameFromGlobalIndex(long) const;
    long GetDefGlobalIndexFromName(const CCharString&) const;
#endif
#ifdef FABLE_RETAIL_HIT_EFFECT
    long GetDefGlobalIndexFromName(const CCharString&) const;
#endif
#ifdef FABLE_RETAIL_FACTIONS
    long GetNoInstantiatedDefsOfType(const CCharString&)const;
#endif
    CDefPointer<const CDefClassBase> GetPDefFromGlobalIndex(long) const;
protected:
    CDefPointer<CDefClassBase> GetPDefFromGlobalIndex(long);
    unsigned char PDefTypeDebugInfoMap[4];
    unsigned char SymbolPathList[12],CompilePathList[12];
    unsigned char PSymbolMap[8],PDefinitionSymbolMap[8];
    unsigned char NameCRCToIndexMap[16],DefClassInfo[16];
    unsigned char CompressedDefData[12],CompressedOffsets[16],InstantiatedDefs[12];
    unsigned char ErrorCallback[4],CurrentFilename[4],CompiledFileName[4];
    unsigned long DependencyCheckCRC;
    bool UseSafeBinaryFormat;
    unsigned char _pad_0xc1[3];
    unsigned char RecompiledDefs[12];
};
FABLE_STATIC_ASSERT(sizeof(CDefinitionManager)==0xd0);
#endif
#else
#ifndef FABLE_ENGINE_CDEFINITIONMANAGER_H
#define FABLE_ENGINE_CDEFINITIONMANAGER_H






#include <stddef.h>


class CCPPointerInfo;
class CCharStringData;
class CSymbolMap;
class CWideStringData;
struct _func___cdecl_void_CCharString_ptr;

#pragma pack(push, 1)
class CDefinitionManager {
public:
    void*                               __vftable;           
    unsigned char                       _pad_0x04[0x4];      
    unsigned char                       _pad_0x08[0x4];      
    unsigned char                       _pad_0x0c[0x4];      
    unsigned char                       _pad_0x10[0x4];      
    unsigned char                       _pad_0x14[0x4];      
    void*                               m_classArray;        
    unsigned char                       _pad_0x1c[0x4];      
    unsigned char                       _pad_0x20[0x4];      
    unsigned char                       _pad_0x24[0x4];      
    unsigned char                       _pad_0x28[0x4];      
    unsigned char                       _pad_0x2c[0x4];      
    unsigned char                       _pad_0x30[0x4];      
    unsigned char                       _pad_0x34[0x4];      
    unsigned char                       _pad_0x38[0x4];      
    void*                               PDefTypeDebugInfoMap; 
    unsigned char                       SymbolPathList[0xc]; 
    unsigned char                       CompilePathList[0xc]; 
    CSymbolMap*                         PSymbolMap_Data;     
    CCPPointerInfo*                     PSymbolMap_Info;     
    CSymbolMap*                         PDefinitionSymbolMap_Data; 
    CCPPointerInfo*                     PDefinitionSymbolMap_Info; 
    unsigned char                       NameCRCToIndexMap[0x10]; 
    unsigned char                       DefClassInfo[0x10];  
    unsigned char                       CompressedDefData[0xc]; 
    unsigned char                       CompressedOffsets[0x10]; 
    unsigned char                       InstantiatedDefs[0xc]; 
    _func___cdecl_void_CCharString_ptr* ErrorCallback;       
    CCharStringData*                    CurrentFilename_PStringData; 
    CWideStringData*                    CompiledFileName_PStringData; 
    unsigned long                       DependencyCheckCRC;  
    bool                                UseSafeBinaryFormat; 
    unsigned char                       _pad_0xc1[0x1];      
    unsigned char                       _pad_0xc2[0x2];      
    unsigned char                       RecompiledDefs[0xc]; 
};
#pragma pack(pop)

FABLE_STATIC_ASSERT(sizeof(CDefinitionManager) == 0xd0);
FABLE_STATIC_ASSERT(offsetof(CDefinitionManager, __vftable) == 0x0);
FABLE_STATIC_ASSERT(offsetof(CDefinitionManager, m_classArray) == 0x18);
FABLE_STATIC_ASSERT(offsetof(CDefinitionManager, PDefTypeDebugInfoMap) == 0x3c);
FABLE_STATIC_ASSERT(offsetof(CDefinitionManager, SymbolPathList) == 0x40);
FABLE_STATIC_ASSERT(offsetof(CDefinitionManager, CompilePathList) == 0x4c);
FABLE_STATIC_ASSERT(offsetof(CDefinitionManager, PSymbolMap_Data) == 0x58);
FABLE_STATIC_ASSERT(offsetof(CDefinitionManager, PSymbolMap_Info) == 0x5c);
FABLE_STATIC_ASSERT(offsetof(CDefinitionManager, PDefinitionSymbolMap_Data) == 0x60);
FABLE_STATIC_ASSERT(offsetof(CDefinitionManager, PDefinitionSymbolMap_Info) == 0x64);
FABLE_STATIC_ASSERT(offsetof(CDefinitionManager, NameCRCToIndexMap) == 0x68);
FABLE_STATIC_ASSERT(offsetof(CDefinitionManager, DefClassInfo) == 0x78);
FABLE_STATIC_ASSERT(offsetof(CDefinitionManager, CompressedDefData) == 0x88);
FABLE_STATIC_ASSERT(offsetof(CDefinitionManager, CompressedOffsets) == 0x94);
FABLE_STATIC_ASSERT(offsetof(CDefinitionManager, InstantiatedDefs) == 0xa4);
FABLE_STATIC_ASSERT(offsetof(CDefinitionManager, ErrorCallback) == 0xb0);
FABLE_STATIC_ASSERT(offsetof(CDefinitionManager, CurrentFilename_PStringData) == 0xb4);
FABLE_STATIC_ASSERT(offsetof(CDefinitionManager, CompiledFileName_PStringData) == 0xb8);
FABLE_STATIC_ASSERT(offsetof(CDefinitionManager, DependencyCheckCRC) == 0xbc);
FABLE_STATIC_ASSERT(offsetof(CDefinitionManager, UseSafeBinaryFormat) == 0xc0);
FABLE_STATIC_ASSERT(offsetof(CDefinitionManager, RecompiledDefs) == 0xc4);

#endif 

#endif

extern const float DELTA_NOTIONAL_ZERO;
template<class T> __declspec(noinline) bool __fastcall IS_THING_ALIVE(const T& thing) { return thing && !thing->IsDead(); }
namespace {
void __fastcall CreateWeaponHitTargetEffect(const CThing*,const C3DVector&,const C3DVector&,bool,bool,bool);
}
template<> __declspec(noinline) bool CDefClassBase::GetSubDef<CCreatureDef>(CDefPointer<const CCreatureDef>* out) const
{
    const CSubDefInfo* index;
    { CCharString name("CCreatureDef",-1); index=GetPSubDefInfo(name); }
    if(index) {
        CDefPointer<const CCreatureDef> result(PDefManager->GetPDefFromGlobalIndex(index->DefIndex));
        *out=result;
        return true;
    }
    return false;
}
void CThingCreatureBase::OnHit(CHitParameters& hit)
{
    CThing* weapon=0;
    CThingGameObject::OnHit(hit);
    if(hit.GetPHitLocation() && IsAlive()) {
        CTCPhysicsBase& physics=*GetPhysicsTC();
        CThing* hitter=hit.GetPThingHitting();
        weapon=hit.GetPWeapon();
        const C3DVector* position=hit.IsLocalised()?&hit.GetHitPosition():&physics.GetPosition();
        const C3DVector* direction=hit.IsDirectional()?&hit.GetHitDirection():&physics.GetRHSet().GetForward();
        if(!hit.IsBlocked() && !hit.IsHitNegated()) {
            CDefPointer<const CCreatureDef> creatureDef;
            bool flash=GetPBaseDef()->GetSubDef(&creatureDef) && creatureDef->FlashOnHit;
            if(flash) {
                CTCFlashOnHit* effect=static_cast<CTCFlashOnHit*>(AddTC("CTCFlashOnHit",true,0));
                effect->SetFadeInTime(0);
                effect->SetFadeOutTime(0);
                effect->SetLifeTime(2);
            }
            CTCTalk* talk;
            if(GetTC(&talk)) {
                const CTCScriptedControl* script;
                if(talk->IsTalking() && (!PeekTC(&script) || !script->IsActive()))
                    talk->StopCurrentSpeech();
            }
            if(IS_THING_ALIVE(hitter) && hitter->GetType()==2 && hit.IsMeleeStrike() && GetPlayerManager().IsThingMainPlayer(*hitter)) {
                if(hit.IsLocalised() && hit.IsDirectional())
                    CreateWeaponHitTargetEffect(weapon,*position,*direction,hit.IsFlourish(),hit.IsKnockDown(),hit.IsBlockCounter());
                CPlayerGui::Get().AddHitByMelee(*this,*hitter,*position);
                {
                const CTCHeroExperience* experience=0;
                hitter->PeekTC(&experience);
                const CHeroCombatDef& combat=experience->PeekHeroCombatDef();
                CTCHaste* haste;
                if(!GetTC(&haste)) haste=static_cast<CTCHaste*>(AddTC("CTCHaste",false,0));
                haste->SetOnHitPause(combat.MeleeStrikeNumFramesPauseOnHeroStrike);
                if(!hitter->GetTC(&haste)) haste=static_cast<CTCHaste*>(hitter->AddTC("CTCHaste",false,0));
                haste->SetOnHitPause(combat.MeleeStrikeNumFramesPauseOnHeroStrike);
            }
            }
            CTCBulletTime* bullet;
            if(IS_THING_ALIVE(hitter) && hitter->GetTC(&bullet))
                bullet->PlayCreatureOnHitEffect(this);
            else if(GetTC(&bullet))
                bullet->PlayCreatureOnHitEffect(this);
            const CTCCreatureModeManager* mode;
            if(PeekTC(&mode) && mode->GetCurrentMode().GetType()==NCreatureMode::MODE_DAZED && hit.IsKnockDown())
                hit.SetDecapitate(true);
            const CTCCombat* combatComponent;
            if(PeekTC(&combatComponent) && !combatComponent->IsAbleToBeKnockedDown())
                hit.SetKnockDown(false);
            PQueuedActions.clear();
            if(PeekTC(&mode) && mode->IsModeActive(NCreatureMode::MODE_SLEEP_BED)) {
                hit.SetKnockDown(false);
                hit.SetPlayHitResponse(false);
                hit.SetDecapitate(false);
                hit.SetMoveBackOnHit(false);
            }
            if(GetHealth()<DELTA_NOTIONAL_ZERO) {
                hit.SetKnockDown(true);
                hit.SetPlayHitResponse(true);
            }
            ReactToGenericStrike(hit);
        }
    }
}

#ifndef FABLE_ENGINE_CPHYSICSMESHOBJECT_H
#define FABLE_ENGINE_CPHYSICSMESHOBJECT_H

class CCharString;class C3DVector;class CMeshIntersectionData;


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
#ifdef FABLE_RETAIL_NAVIGATION_UPDATE
    void GetAllHelperPointNames(std::vector<CCharString>&)const;
    C3DVector GetHelperPointPositionStatic(const CCharString&,long)const;
#endif
    bool HasContainmentVolume(const CCharString&) const;
#ifdef FABLE_RETAIL_VALIDPOSITIONS
    bool GetPointIntersectionFromTo(const C3DVector&,const C3DVector&,CMeshIntersectionData&) const;
#endif
};
FABLE_STATIC_ASSERT(sizeof(CPhysicsMeshObject)==0x40);
#endif



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
void CTCPhysicsBase::ResetPosition(const C3DVector &) { assert(false); abort(); }
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
const CThingPhysicsMeshInfo * CTCPhysicsBase::GetPhysicsMeshInfo() const { assert(false); abort(); }
void CTCPhysicsBase::UpdateInterpolation() { assert(false); abort(); }
void CTCPhysicsBase::UpdateOldRHSet() { assert(false); abort(); }
void CTCPhysicsBase::UpdateOldPos() { assert(false); abort(); }
void CIVCountedPointeeBase::Delete()const{}
CResource::CResource():ResourceList(0),PrevResource(0),NextResource(0),ResourceSize(0),LastUsedFrame(0){IVRefCount=100;}
CResource::~CResource(){}
bool CResource::UnloadResource(){return true;}
unsigned long CResource::GetUnloadableMemory()const{return 0;}
bool CResource::IsResourceLoaded()const{return true;}
void CResource::OnResourceLoaded(){}bool CResource::CanAbortResourceLoad(long)const{return false;}
void CResource::OnResourceLoadAborted(){}unsigned long CResource::GetDebugMemoryInUse()const{return 0;}void CResource::ResetDebugMemoryInUse(){}
CDefClassBase::CDefClassBase():PDefManager(0),GlobalIndex(0){Setup=0;Template=0;DefaultValsApplied=0;}
bool CDefClassBase::UnloadResource(){return true;}
CDefString CDefClassBase::GetInstantiationName()const{return CDefString();}
void CDefClassBase::SetInstantiationName(CDefString){}
void CDefClassBase::AddSubDef(const CCharString&,long,long){}void CDefClassBase::RemoveSubDef(const CCharString&){}
CDefClassBase::CSubDefInfo* CDefClassBase::GetPSubDefInfo(const CCharString&){return 0;}
const CDefClassBase::CSubDefInfo* CDefClassBase::GetPSubDefInfo(const CCharString&)const{return 0;}
bool CDefClassBase::IsParentDef()const{return false;}void CDefClassBase::PostTransfer(){}
void CDefClassBase::InternalSaveBinary(CDataOutputStream&){}void CDefClassBase::InternalLoadBinary(CDataInputStream&){}
void CDefClassBase::Validate(const CValidateDefinitionInitBase&,const CCharString&)const{}
CVectorMap<unsigned long,CDefClassBase::CSubDefInfo,CKeyPairCompareLess<unsigned long,CDefClassBase::CSubDefInfo> >* CDefClassBase::GetPSubDefInfoMap(){return 0;}
CParentDefClassBase::CParentDefClassBase():InstantiationName_TablePos(-1){memset(SubDefInfoMap,0,sizeof(SubDefInfoMap));}
CParentDefClassBase::~CParentDefClassBase(){}
CDefString CParentDefClassBase::GetInstantiationName()const{return CDefString();}void CParentDefClassBase::SetInstantiationName(CDefString){}
void CParentDefClassBase::AddSubDef(const CCharString&,long,long){}void CParentDefClassBase::RemoveSubDef(const CCharString&){}
CDefClassBase::CSubDefInfo* CParentDefClassBase::GetPSubDefInfo(const CCharString&){return 0;}
const CDefClassBase::CSubDefInfo* CParentDefClassBase::GetPSubDefInfo(const CCharString&)const{return 0;}
bool CParentDefClassBase::IsParentDef()const{return true;}
void CParentDefClassBase::InternalSaveBinary(CDataOutputStream&){}void CParentDefClassBase::InternalLoadBinary(CDataInputStream&){}
CVectorMap<unsigned long,CDefClassBase::CSubDefInfo,CKeyPairCompareLess<unsigned long,CDefClassBase::CSubDefInfo> >* CParentDefClassBase::GetPSubDefInfoMap(){return 0;}

static int TestBaseCalls,TestReactCalls,TestClearCalls,TestRHCalls;
static int TestBulletCalls;static CTCBulletTime* TestBulletReceiver;static CThingCreatureBase* TestBulletVictim;
static const CBaseIntelligentPointer* TestHitterHandle;
static CThing* TestHitter;
#include <string.h>
static float native_epsilon(){unsigned int bits=0x38d1b717;float result;memcpy(&result,&bits,4);return result;}
const float DELTA_NOTIONAL_ZERO=native_epsilon();
CBaseClass::CBaseClass(){} CBaseClass::~CBaseClass(){}
CBaseClassNonCopyable::CBaseClassNonCopyable(){} CBaseClassNonCopyable::~CBaseClassNonCopyable(){}
CBaseObject::CBaseObject():IntelligentPointer(0){} CBaseObject::~CBaseObject(){}
void CBaseObject::Validate()const{} void CClass::Save(CCharString&){}
CCharString::CCharString():PStringData(0){} CCharString::~CCharString(){}
CCharString::CCharString(const CCharString& x):PStringData(x.PStringData){}
CCharString::CCharString(const char*,long):PStringData(0){}
CBaseIntelligentPointer::~CBaseIntelligentPointer(){}
CBaseObject* CBaseIntelligentPointer::GetPItem()const{return this==TestHitterHandle?TestHitter:0;}
CThing::CThing(unsigned long type):UniqueID(0),PhysicsTC(0),PTCDriver(0),PTCGraphicAppearance(0),WorldFrameCreatedAt(0),ThingTypeFlags(type){Type=(unsigned char)type;Dead=0;memset(&UsingComponents,0,sizeof(UsingComponents));}
CThingPhysical::CThingPhysical(unsigned long type):CThing(type){Health=1.0f;}
CThingGameObject::CThingGameObject(unsigned long type):CThingPhysical(type){}
CThingCreatureBase::CThingCreatureBase(unsigned long type):CThingGameObject(type){}
void CThingGameObject::OnHit(CHitParameters&){++TestBaseCalls;}
void CThingCreatureBase::ReactToGenericStrike(const CHitParameters&){++TestReactCalls;}
CTCBase::CTCBase(CThing& parent):ParentThing(parent),ToBeRemoved(false),IsInGlobalUpdateTCs(false){}
CTCBase::~CTCBase(){}
CTCPhysicsBase::CTCPhysicsBase(CThing& parent):CTCBase(parent),Position(1.0f,2.0f,4.0f),OldPosition(0.0f,0.0f,0.0f),Velocity(0.0f,0.0f,0.0f),Radius(8.0f){}
CTCPhysicsBase::~CTCPhysicsBase(){}
class CTestAxes:public CRightHandedSet{public:CTestAxes(){Up=C3DVector(0.0f,0.0f,1.0f);Forward=C3DVector(1.0f,0.0f,0.0f);}};
static CTestAxes TestAxes;
const CRightHandedSet& CTCPhysicsBase::GetRHSet()const{++TestRHCalls;return TestAxes;}
namespace std {
void __stl_throw_out_of_range(const char*){assert(false);abort();}
template<class T,class A>void _List_base<T,A>::clear(){++TestClearCalls;assert(_M_node._M_data->_M_next==_M_node._M_data);}
}
CDefPointer<CDefClassBase> CDefinitionManager::GetPDefFromGlobalIndex(long){assert(false);abort();}
CTCBase* CThing::AddTC(const CCharString&,bool,const CTCParamsBase*){assert(false);abort();}
CPlayerManager& CThing::GetPlayerManager(){assert(false);abort();}
bool CPlayerManager::IsThingMainPlayer(const CThing&)const{assert(false);abort();}
bool CHitParameters::IsMeleeStrike()const{assert(false);abort();}
void CTCFlashOnHit::SetLifeTime(long){assert(false);abort();}
bool CTCTalk::IsTalking()const{assert(false);abort();}
void CTCTalk::StopCurrentSpeech(){assert(false);abort();}
bool CTCScriptedControl::IsActive()const{assert(false);abort();}
CTCBulletTime::CTCBulletTime(CThing& parent):CTCBase(parent){}
void CTCBulletTime::PlayCreatureOnHitEffect(CThingCreatureBase* victim){++TestBulletCalls;TestBulletReceiver=this;TestBulletVictim=victim;}
class CTestBullet:public CTCBulletTime{public:CTestBullet(CThing& parent):CTCBulletTime(parent){} ETCInterfaceType GetInterfaceType()const{return TCI_BULLET_TIME;} CCharString GetClassTypeName()const{return CCharString();}};
CTCCreatureModeManager::CModeBase& CTCCreatureModeManager::GetCurrentMode()const{assert(false);abort();}
bool CTCCreatureModeManager::IsModeActive(unsigned long)const{assert(false);abort();}
bool CTCCombat::IsAbleToBeKnockedDown()const{assert(false);abort();}
void CPlayerGui::AddHitByMelee(CThing&,CThing&,const C3DVector&){assert(false);abort();}
template<class T> CCountedPointer<T> CTBaseSingletonNoDefaultAlloc<T>::PClass;
namespace {void __fastcall CreateWeaponHitTargetEffect(const CThing*,const C3DVector&,const C3DVector&,bool,bool,bool){assert(false);abort();}}
class CTestThingDef:public CThingBaseDef{
public:void Transfer(CPersistContext&){}void Copy(const CDefClassBase*){}unsigned long GetSizeofClass(){return sizeof(*this);}
};
class CTestPhysics:public CTCPhysicsBase{
public:CTestPhysics(CThing& thing):CTCPhysicsBase(thing){}
 CCharString GetClassTypeName()const{return CCharString();}
};
class COnHitFixture{
public:
 static void Setup(CThingCreatureBase& creature,CTCPhysicsBase& physics,CThingBaseDef& def){creature.PhysicsTC=&physics;creature.PBaseDef.Object=&def;}
 static void SetDead(CThing& thing,bool dead){thing.Dead=dead;}
 static void SetHealth(CThingPhysical& thing,float health){thing.Health=health;}
 static void ResetHit(CHitParameters& hit){
  hit.Localised=true;hit.Directional=true;memset(hit.Position,0,sizeof(hit.Position));memset(hit.Direction,0,sizeof(hit.Direction));
  hit.KnockDown=false;hit.Decapitate=false;hit.Blockable=false;hit.Flourish=false;hit.EpicSpell=false;hit.BlockCounter=false;
  hit.PlayHitResponse=false;hit.PlayHitResponseOverrideSet=false;hit.MoveBack=true;hit.CreateParticleEffectOnHit=false;hit.CreateDustParticleEffectOnHit=false;hit.GuaranteeHit=false;
  hit.Blocked=false;hit.HitNegated=false;hit.CauseRecoil=false;hit.ShotBuildupTime=0;memset(hit.DamageMap,0,sizeof(hit.DamageMap));memset(hit.Augmentations,0,sizeof(hit.Augmentations));
  static int location;hit.PHitLocation=reinterpret_cast<NHitLocations::CHitLocation*>(&location);TestHitterHandle=&hit.PThingHitting;TestHitter=0;
  TestBaseCalls=TestReactCalls=TestClearCalls=TestRHCalls=TestBulletCalls=0;TestBulletReceiver=0;TestBulletVictim=0;
 }
 static void SetBullet(CThing& thing,CTCBulletTime* component){
  typedef std::pair<ETCInterfaceType,CTCBase*> Pair;
  if(thing.Components._M_start){thing.Components._M_start->~Pair();free(thing.Components._M_start);}
  thing.Components._M_start=thing.Components._M_finish=thing.Components._M_end_of_storage._M_data=0;
  memset(&thing.UsingComponents,0,sizeof(thing.UsingComponents));
  if(component){Pair* pair=(Pair*)malloc(sizeof(Pair));new(pair) Pair;pair->first=TCI_BULLET_TIME;pair->second=component;thing.Components._M_start=pair;thing.Components._M_finish=thing.Components._M_end_of_storage._M_data=pair+1;unsigned n=TCI_BULLET_TIME;thing.UsingComponents._M_w[n/(8*sizeof(unsigned long))]|=((unsigned long)1)<<(n%(8*sizeof(unsigned long)));}
 }
 static void NoLocation(CHitParameters& hit){hit.PHitLocation=0;}
 static void Block(CHitParameters& hit){hit.Blocked=true;}
 static void Negate(CHitParameters& hit){hit.HitNegated=true;}
 static void WorldVectors(CHitParameters& hit){hit.Localised=false;hit.Directional=false;}
 static bool ResponseIsForced(const CHitParameters& hit){return hit.PlayHitResponseOverrideSet&&hit.PlayHitResponse;}
};
int main(){
 CTestThingDef definition;CThingCreatureBase creature(2);CTestPhysics physics(creature);CHitParameters hit;
 COnHitFixture::Setup(creature,physics,definition);
 COnHitFixture::ResetHit(hit);COnHitFixture::NoLocation(hit);creature.OnHit(hit);assert(TestBaseCalls==1&&TestReactCalls==0&&TestClearCalls==0);
 COnHitFixture::ResetHit(hit);COnHitFixture::SetDead(creature,true);creature.OnHit(hit);assert(TestBaseCalls==1&&TestReactCalls==0&&TestRHCalls==0);COnHitFixture::SetDead(creature,false);
 COnHitFixture::ResetHit(hit);COnHitFixture::Block(hit);creature.OnHit(hit);assert(TestBaseCalls==1&&TestReactCalls==0&&TestClearCalls==0);
 COnHitFixture::ResetHit(hit);COnHitFixture::Negate(hit);creature.OnHit(hit);assert(TestBaseCalls==1&&TestReactCalls==0&&TestClearCalls==0);
 COnHitFixture::ResetHit(hit);creature.OnHit(hit);assert(TestBaseCalls==1&&TestReactCalls==1&&TestClearCalls==1&&!hit.IsKnockDown()&&!COnHitFixture::ResponseIsForced(hit));
 COnHitFixture::ResetHit(hit);COnHitFixture::SetHealth(creature,0.0f);creature.OnHit(hit);assert(TestReactCalls==1&&TestClearCalls==1&&hit.IsKnockDown()&&COnHitFixture::ResponseIsForced(hit));
 COnHitFixture::ResetHit(hit);COnHitFixture::SetHealth(creature,DELTA_NOTIONAL_ZERO);creature.OnHit(hit);assert(TestReactCalls==1&&!hit.IsKnockDown()&&!COnHitFixture::ResponseIsForced(hit));
 COnHitFixture::ResetHit(hit);COnHitFixture::SetHealth(creature,DELTA_NOTIONAL_ZERO*0.5f);creature.OnHit(hit);assert(TestReactCalls==1&&hit.IsKnockDown()&&COnHitFixture::ResponseIsForced(hit));
 COnHitFixture::ResetHit(hit);COnHitFixture::SetHealth(creature,DELTA_NOTIONAL_ZERO*2.0f);creature.OnHit(hit);assert(TestReactCalls==1&&!hit.IsKnockDown()&&!COnHitFixture::ResponseIsForced(hit));
 COnHitFixture::ResetHit(hit);COnHitFixture::SetHealth(creature,1.0f);COnHitFixture::WorldVectors(hit);creature.OnHit(hit);assert(TestReactCalls==1&&TestRHCalls==1&&TestClearCalls==1);
 CThingGameObject hitter(1);CTestBullet own_bullet(creature),hitter_bullet(hitter);
 for(int scenario=0;scenario<6;++scenario){
  COnHitFixture::ResetHit(hit);TestHitter=scenario==4?0:&hitter;COnHitFixture::SetDead(hitter,scenario==2);
  bool own=scenario!=3&&scenario!=5;bool other=scenario==0||scenario==2||scenario==5;
  COnHitFixture::SetBullet(creature,own?&own_bullet:0);COnHitFixture::SetBullet(hitter,other?&hitter_bullet:0);
  creature.OnHit(hit);assert(TestBaseCalls==1&&TestReactCalls==1&&TestClearCalls==1);
  assert(TestBulletCalls==(scenario==3?0:1));if(scenario!=3){assert(TestBulletVictim==&creature);assert(TestBulletReceiver==((scenario==0||scenario==5)?&hitter_bullet:&own_bullet));}
 }
 COnHitFixture::SetBullet(creature,0);COnHitFixture::SetBullet(hitter,0);
 printf("00667d30_TEST PASS\n");return 0;
}