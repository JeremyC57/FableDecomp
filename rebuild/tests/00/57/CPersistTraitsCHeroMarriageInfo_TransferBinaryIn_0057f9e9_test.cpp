// Behavior-only fixture for the parked source; target body is unchanged.
// Native ABI assertions remain active on VC7.1. GNU64 uses its native long;
// ReadSLONG uses ReadType<int> only on GNU64 to retain the native four-byte
// serialized width; its native method body remains unchanged.
// Access macros expose fields only for deterministic fixture initialization.
#include "fable_compiler.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifndef _MSC_VER
#define __int64 long
#define __forceinline inline
#define __fastcall
#define __cdecl
#define __declspec(x)
#endif
#define private public
#define protected public
static int cases,overflowCalls;static long expectedOverflowSize;
#define CHECK(x)do{if(!(x)){fprintf(stderr,"case %d line %d\n",cases,__LINE__);abort();}}while(0)
// 0057f9e9 CPersistTraits<CHeroMarriageInfo>::TransferBinaryIn
// Leaf: ?TransferBinaryIn@?$CPersistTraits@VCHeroMarriageInfo@@@@QAEXAAVCMemoryDataInputStream@@AAVCHeroMarriageInfo@@@Z
// Flags: /O1 /Oy-
#include <stddef.h>
// Begin shared header engine/CBaseClassNonCopyable.h
#ifndef FABLE_ENGINE_CBASECLASSNONCOPYABLE_H
#define FABLE_ENGINE_CBASECLASSNONCOPYABLE_H
// Retail layout (hand-written; no members). RTTI CBaseClassNonCopyable : CBaseClass; vtable 0x0129A7C4, slot 0 scalar
// deleting dtor 0x0099A470. Ego_r.pdb / FableWin.pdb: ??0CBaseClassNonCopyable@@IAE@XZ (protected),
// ??1CBaseClassNonCopyable@@MAE@XZ (protected virtual). The copy operations are private and undefined (non-copyable).
// Begin shared header rebuild_abi.h
#ifndef TEST_EMBED_3AC84EBD4026
#define TEST_EMBED_3AC84EBD4026


// ABI primitives for reconstructed FableTLC modules. Keep this header valid
// under VC7.1; modern-only language/library features belong outside Tier 0.
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
#define FABLE_STATIC_ASSERT(expression) typedef char FABLE_JOIN(fable_sassert_, __LINE__)[(expression) ? 1 : -1]

#endif

// End shared header rebuild_abi.h
// Begin shared header engine/CBaseClass.h
#ifndef FABLE_ENGINE_CBASECLASS_H
#define FABLE_ENGINE_CBASECLASS_H
// Retail layout (hand-written; Ego_r.pdb has no members): the root polymorphic base. RTTI .?AVCBaseClass@@ (no bases);
// vtable 0x01231710, slot 0 scalar deleting dtor 0x004371F0, which calls ~CBaseClass 0x0099A2E0 (one vtable store).
// The default ctor is out of line (0x0099A2D0: `mov eax,ecx; mov [eax],0x01231710; ret`): Ego_r.pdb places
// ??0CBaseClass@@QAE@XZ / ??1CBaseClass@@UAE@XZ side by side (0x00B01F20 / 0x00B01F30, one .cpp) and FableWin.pdb does the
// same (0x03044700 / 0x03044720), and 1,225 landed rows call 0x0099A2D0. The copy ctor is the implicit one: FableWin.pdb
// emits ??0CBaseClass@@QAE@ABV0@@Z beside the compiler-generated ??_E (0x018CD7E0), and retail expands it inline (a bare
// vtable store in a derived copy ctor, landed chunk 577 0x00DCEDF0 / chunk 578 0x00F05F30: write `: CBaseClass(other)`).
// Begin shared header rebuild_abi.h
#ifndef TEST_EMBED_3AC84EBD4026
#define TEST_EMBED_3AC84EBD4026


// ABI primitives for reconstructed FableTLC modules. Keep this header valid
// under VC7.1; modern-only language/library features belong outside Tier 0.
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
#define FABLE_STATIC_ASSERT(expression) typedef char FABLE_JOIN(fable_sassert_, __LINE__)[(expression) ? 1 : -1]

#endif

// End shared header rebuild_abi.h

class CBaseClass {
public:
    CBaseClass();                                                        // 0x0099A2D0
    virtual ~CBaseClass();                                               // slot 0  0x0099A2E0
};
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(sizeof(CBaseClass) == 0x4);
#endif
#endif

#endif // FABLE_ENGINE_CBASECLASS_H

// End shared header engine/CBaseClass.h

class CBaseClassNonCopyable : public CBaseClass {
protected:
    CBaseClassNonCopyable();
    virtual ~CBaseClassNonCopyable();                                    // slot 0
private:
    CBaseClassNonCopyable(const CBaseClassNonCopyable&);
    CBaseClassNonCopyable& operator=(const CBaseClassNonCopyable&);
};
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(sizeof(CBaseClassNonCopyable) == 0x4);
#endif
#endif

#endif // FABLE_ENGINE_CBASECLASSNONCOPYABLE_H

// End shared header engine/CBaseClassNonCopyable.h
#define CDataInputStream CDataInputStreamFields
// Begin shared header engine/CDataInputStream.h
#ifndef FABLE_ENGINE_CDATAINPUTSTREAM_H
#define FABLE_ENGINE_CDATAINPUTSTREAM_H
// GENERATED by tools/decomp_pipeline/gen_class_headers.py -- do not edit by hand.
// Donor: Ego_r.pdb (ghidra_out/struct_layouts_egor.tsv) sizeof CDataInputStream = 0x18.
// Retail evidence: 0 trusted landed function(s); reconciliation clean.
// Members are original Lionhead names; `_pad_*` are unnamed bytes; opaque
// `unsigned char X[N]` members carry their real (composite/template) type in a comment.
#include <stddef.h>
// Begin shared header rebuild_abi.h
#ifndef TEST_EMBED_3AC84EBD4026
#define TEST_EMBED_3AC84EBD4026


// ABI primitives for reconstructed FableTLC modules. Keep this header valid
// under VC7.1; modern-only language/library features belong outside Tier 0.
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
#define FABLE_STATIC_ASSERT(expression) typedef char FABLE_JOIN(fable_sassert_, __LINE__)[(expression) ? 1 : -1]

#endif

// End shared header rebuild_abi.h

#pragma pack(push, 1)
class CDataInputStream {
public:
    void*         __vftable;                                 // +0x00 vptr, or first dword of a flattened base subobject
    unsigned long StreamPos;                                 // +0x04
    long          StreamSize;                                // +0x08
    void*         CurrentSourcePtr;                          // +0x0c uchar *
    unsigned long SourceChunkStreamPos;                      // +0x10
    long          SourceChunkBytesRemaining;                 // +0x14
#ifdef FABLE_CDATAINPUTSTREAM_FRIEND
    friend FABLE_CDATAINPUTSTREAM_FRIEND;                   // the including row's inline-helper class
#endif
private:
    void ReadWithSrcChunkOverflow(unsigned char* buffer, long size); // 0x00993CA0 ?ReadWithSrcChunkOverflow@CDataInputStream@@AAEXPAEJ@Z
};
#pragma pack(pop)

#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(sizeof(CDataInputStream) == 0x18);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CDataInputStream, __vftable) == 0x0);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CDataInputStream, StreamPos) == 0x4);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CDataInputStream, StreamSize) == 0x8);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CDataInputStream, CurrentSourcePtr) == 0xc);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CDataInputStream, SourceChunkStreamPos) == 0x10);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CDataInputStream, SourceChunkBytesRemaining) == 0x14);
#endif
#endif

#endif // FABLE_ENGINE_CDATAINPUTSTREAM_H

// End shared header engine/CDataInputStream.h
#undef CDataInputStream
#define CMemoryDataInputStream CMemoryDataInputStreamFields
// Begin shared header engine/CMemoryDataInputStream.h
#ifndef FABLE_ENGINE_CMEMORYDATAINPUTSTREAM_H
#define FABLE_ENGINE_CMEMORYDATAINPUTSTREAM_H
// GENERATED by tools/decomp_pipeline/gen_class_headers.py -- do not edit by hand.
// Donor: Ego_r.pdb (ghidra_out/struct_layouts_egor.tsv) sizeof CMemoryDataInputStream = 0x1c.
// Retail evidence: 1 trusted landed function(s); reconciliation clean.
// Members are original Lionhead names; `_pad_*` are unnamed bytes; opaque
// `unsigned char X[N]` members carry their real (composite/template) type in a comment.
#include <stddef.h>
// Begin shared header rebuild_abi.h
#ifndef TEST_EMBED_3AC84EBD4026
#define TEST_EMBED_3AC84EBD4026


// ABI primitives for reconstructed FableTLC modules. Keep this header valid
// under VC7.1; modern-only language/library features belong outside Tier 0.
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
#define FABLE_STATIC_ASSERT(expression) typedef char FABLE_JOIN(fable_sassert_, __LINE__)[(expression) ? 1 : -1]

#endif

// End shared header rebuild_abi.h

#pragma pack(push, 1)
class CMemoryDataInputStream {
public:
    void* __vftable;                                         // +0x00 vptr, or first dword of a flattened base subobject
    long  position;                                          // +0x04 retail-only long (previous header)
    long  sourceLength;                                      // +0x08 retail-only long (previous header)
    long  chunkBase;                                         // +0x0c retail-only long (previous header)
    long  chunkPosition;                                     // +0x10 retail-only long (previous header)
    long  chunkLength;                                       // +0x14 retail-only long (previous header)
    void* StreamSource;                                      // +0x18 uchar *
};
#pragma pack(pop)

#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(sizeof(CMemoryDataInputStream) == 0x1c);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CMemoryDataInputStream, __vftable) == 0x0);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CMemoryDataInputStream, position) == 0x4);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CMemoryDataInputStream, sourceLength) == 0x8);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CMemoryDataInputStream, chunkBase) == 0xc);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CMemoryDataInputStream, chunkPosition) == 0x10);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CMemoryDataInputStream, chunkLength) == 0x14);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CMemoryDataInputStream, StreamSource) == 0x18);
#endif
#endif

#endif // FABLE_ENGINE_CMEMORYDATAINPUTSTREAM_H

// End shared header engine/CMemoryDataInputStream.h
#undef CMemoryDataInputStream
#define CHeroMarriageInfo CHeroMarriageInfoFields
// Begin shared header engine/CHeroMarriageInfo.h
#ifndef FABLE_ENGINE_CHEROMARRIAGEINFO_H
#define FABLE_ENGINE_CHEROMARRIAGEINFO_H
// GENERATED by tools/decomp_pipeline/gen_class_headers.py -- do not edit by hand.
// Donor: Ego_r.pdb (ghidra_out/struct_layouts_egor.tsv) sizeof CHeroMarriageInfo = 0x18.
// Retail evidence: 0 trusted landed function(s); reconciliation clean.
// Members are original Lionhead names; `_pad_*` are unnamed bytes; opaque
// `unsigned char X[N]` members carry their real (composite/template) type in a comment.
#include <stddef.h>
// Begin shared header rebuild_abi.h
#ifndef TEST_EMBED_3AC84EBD4026
#define TEST_EMBED_3AC84EBD4026


// ABI primitives for reconstructed FableTLC modules. Keep this header valid
// under VC7.1; modern-only language/library features belong outside Tier 0.
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
#define FABLE_STATIC_ASSERT(expression) typedef char FABLE_JOIN(fable_sassert_, __LINE__)[(expression) ? 1 : -1]

#endif

// End shared header rebuild_abi.h

#pragma pack(push, 1)
class CHeroMarriageInfo {
public:
    unsigned __int64 WifeUID;                                // +0x00
    unsigned __int64 MaritalHomeUID;                         // +0x08
    unsigned char    State[0x4];                             // +0x10 EMarriageState
    bool             GayMarriage;                            // +0x14
    unsigned char    _pad_0x15[0x3];                         // +0x15
};
#pragma pack(pop)

#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(sizeof(CHeroMarriageInfo) == 0x18);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CHeroMarriageInfo, WifeUID) == 0x0);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CHeroMarriageInfo, MaritalHomeUID) == 0x8);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CHeroMarriageInfo, State) == 0x10);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CHeroMarriageInfo, GayMarriage) == 0x14);
#endif
#endif

#endif // FABLE_ENGINE_CHEROMARRIAGEINFO_H

// End shared header engine/CHeroMarriageInfo.h
#undef CHeroMarriageInfo
extern "C" void*__cdecl memcpy(void*,const void*,size_t);
#pragma intrinsic(memcpy)
class CCharString;class CWideString;
class CDataStream:public CBaseClassNonCopyable{public:virtual~CDataStream();virtual void SetPosition(unsigned long)=0;virtual unsigned long GetPosition()const=0;virtual void Close()=0;};
// Layout: original CDataInputStream24, shared fields StreamPos4/StreamSize8/CurrentSourcePtrc/SourceChunkStreamPos10/SourceChunkBytesRemaining14.
class CDataInputStream:public CDataStream{
private:unsigned long StreamPos;long StreamSize;const unsigned char*CurrentSourcePtr;unsigned long SourceChunkStreamPos;long SourceChunkBytesRemaining;
 void ReadWithSrcChunkOverflow(unsigned char*,long);
public:virtual~CDataInputStream();virtual void Close();virtual unsigned long GetPosition()const;virtual void SetPosition(unsigned long);
 virtual void ReadCompressed(void*,long);virtual void ReadZlibCompressed(void*,long);virtual CCharString ReadNullTerminatedString();virtual CWideString ReadNullTerminatedWString();
private:virtual void InternalReadNextSourceChunk(const unsigned char*&,long&);virtual bool NeedsBufferedRead(long);virtual void InternalReadUnbuffered(unsigned char*,long);
public:template<class T>FABLE_FORCEINLINE T ReadType(){T val;Read(&val,sizeof(T));return val;}
 FABLE_FORCEINLINE unsigned __int64 ReadUHUGE(){return ReadType<unsigned __int64>();}
 FABLE_FORCEINLINE long ReadSLONG(){
#ifdef _MSC_VER
return ReadType<long>();
#else
return ReadType<int>();
#endif
}
 FABLE_FORCEINLINE unsigned char ReadUBYTE(){return ReadType<unsigned char>();}
 FABLE_FORCEINLINE bool ReadEBOOL(){return ReadUBYTE()!=0;}
 FABLE_FORCEINLINE void Read(void*dst,long no_bytes){if(no_bytes){
 __int64 overflow_check=static_cast<__int64>(StreamPos+no_bytes);
 if(overflow_check<0||overflow_check>0x7fffffff)return;
 if(SourceChunkBytesRemaining>=no_bytes){memcpy(dst,CurrentSourcePtr,no_bytes);CurrentSourcePtr+=no_bytes;SourceChunkBytesRemaining-=no_bytes;StreamPos+=no_bytes;}
 else ReadWithSrcChunkOverflow(static_cast<unsigned char*>(dst),no_bytes);
 }}

};
// Layout: shared memory stream adds original private StreamSource at18, total1c.
class CMemoryDataInputStream:public CDataInputStream{private:const unsigned char*StreamSource;};
enum EMarriageState{};
// Layout: original class V and typed enum State10, shared fields WifeUID0/MaritalHomeUID8/GayMarriage14, total18.
class CHeroMarriageInfo{public:unsigned __int64 WifeUID,MaritalHomeUID;EMarriageState State;bool GayMarriage;CHeroMarriageInfo():WifeUID(0),MaritalHomeUID(0),State(static_cast<EMarriageState>(0)),GayMarriage(false){}};
template<class T>class CPersistTraits{public:T DefaultVal;FABLE_FORCEINLINE void TransferBinaryIn(CMemoryDataInputStream&stream,T&value){T number;stream.Read(&number,sizeof(number));value=number;}};
template<>class CPersistTraits<unsigned __int64>{public:unsigned __int64 DefaultVal;FABLE_FORCEINLINE void TransferBinaryIn(CMemoryDataInputStream&stream,unsigned __int64&value){value=stream.ReadUHUGE();}};
template<>class CPersistTraits<EMarriageState>{public:EMarriageState DefaultVal;FABLE_FORCEINLINE void TransferBinaryIn(CMemoryDataInputStream&stream,EMarriageState&value){value=static_cast<EMarriageState>(stream.ReadSLONG());}};
template<>class CPersistTraits<bool>{public:bool DefaultVal;FABLE_FORCEINLINE void TransferBinaryIn(CMemoryDataInputStream&stream,bool&value){value=stream.ReadEBOOL();}};
template<>class CPersistTraits<CHeroMarriageInfo>{public:CHeroMarriageInfo DefaultVal;void TransferBinaryIn(CMemoryDataInputStream&,CHeroMarriageInfo&);};
void CPersistTraits<CHeroMarriageInfo>::TransferBinaryIn(CMemoryDataInputStream&stream,CHeroMarriageInfo&value){
 value=CHeroMarriageInfo();CPersistTraits<unsigned __int64>uidTraits;uidTraits.TransferBinaryIn(stream,value.WifeUID);uidTraits.TransferBinaryIn(stream,value.MaritalHomeUID);CPersistTraits<EMarriageState>stateTraits;stateTraits.TransferBinaryIn(stream,value.State);CPersistTraits<bool>boolTraits;boolTraits.TransferBinaryIn(stream,value.GayMarriage);
}
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(sizeof(CHeroMarriageInfo)==24);
#endif
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(sizeof(CDataInputStream)==24);
#endif
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(sizeof(CMemoryDataInputStream)==28);
#endif

#undef private
#undef protected
CBaseClass::CBaseClass(){}
CBaseClass::~CBaseClass(){}
CBaseClassNonCopyable::CBaseClassNonCopyable(){}
CBaseClassNonCopyable::~CBaseClassNonCopyable(){}
CDataStream::~CDataStream(){}
CDataInputStream::~CDataInputStream(){}
void CDataInputStream::SetPosition(unsigned long){abort();}
unsigned long CDataInputStream::GetPosition()const{return StreamPos;}
void CDataInputStream::Close(){abort();}
void CDataInputStream::ReadCompressed(void*,long){abort();}
void CDataInputStream::ReadZlibCompressed(void*,long){abort();}
class CCharString{};class CWideString{};
CCharString CDataInputStream::ReadNullTerminatedString(){abort();return CCharString();}
CWideString CDataInputStream::ReadNullTerminatedWString(){abort();return CWideString();}
void CDataInputStream::InternalReadNextSourceChunk(const unsigned char*&,long&){abort();}
bool CDataInputStream::NeedsBufferedRead(long){abort();return false;}
void CDataInputStream::InternalReadUnbuffered(unsigned char*,long){abort();}
void CDataInputStream::ReadWithSrcChunkOverflow(unsigned char*out,long bytes){
 CHECK(bytes==expectedOverflowSize);CHECK(StreamPos+(unsigned long)bytes<=(unsigned long)StreamSize);++overflowCalls;
 const CMemoryDataInputStream&memory=static_cast<const CMemoryDataInputStream&>(*this);
 memcpy(out,memory.StreamSource+StreamPos,bytes);StreamPos+=(unsigned long)bytes;
 CurrentSourcePtr=memory.StreamSource+StreamPos;SourceChunkBytesRemaining=StreamSize-StreamPos;
}
int main(){
 unsigned char raw[64];const unsigned char flags[]={0,1,2,255};
 for(int pattern=0;pattern<4;++pattern)for(int state=0;state<7;++state)for(int flag=0;flag<4;++flag)for(int chunk=0;chunk<=21;++chunk)for(int offset=0;offset<2;++offset){
  memset(raw,0xa5,sizeof raw);unsigned char*p=raw+offset;
  unsigned __int64 wife=((unsigned __int64)(0x12340000U+pattern*0x10001U)<<32)|(0x87654321U+pattern);
  unsigned __int64 home=((unsigned __int64)(0xfedc0000U-pattern*0x10001U)<<32)|(0x13572468U-pattern);
  EMarriageState marriage=(EMarriageState)state;
  memcpy(p,&wife,8);memcpy(p+8,&home,8);memcpy(p+16,&marriage,4);p[20]=flags[flag];
  CMemoryDataInputStream stream;stream.StreamSource=raw;stream.StreamPos=offset;stream.StreamSize=sizeof raw;stream.SourceChunkStreamPos=offset;stream.CurrentSourcePtr=p;stream.SourceChunkBytesRemaining=chunk;
  CHeroMarriageInfo result;result.WifeUID=~wife;result.MaritalHomeUID=~home;result.State=(EMarriageState)99;result.GayMarriage=flags[flag]==0;
  CPersistTraits<CHeroMarriageInfo>traits;traits.DefaultVal.WifeUID=77;traits.DefaultVal.State=(EMarriageState)33;
  overflowCalls=0;expectedOverflowSize=chunk<16?8:(chunk<20?4:1);
  traits.TransferBinaryIn(stream,result);
  CHECK(result.WifeUID==wife&&result.MaritalHomeUID==home&&result.State==marriage&&result.GayMarriage==(flags[flag]!=0));
  CHECK(stream.StreamPos==(unsigned long)(offset+21)&&stream.CurrentSourcePtr==p+21);CHECK(overflowCalls==(chunk<21?1:0));
  CHECK(stream.SourceChunkBytesRemaining==(chunk<21?(long)sizeof raw-offset-21:0));
  CHECK(traits.DefaultVal.WifeUID==77&&traits.DefaultVal.State==(EMarriageState)33);
  CHECK(p[21]==0xa5&&raw[63]==0xa5);++cases;
 }
 CHECK(cases==4928);
 // Test Read's invalid-position guard separately; Transfer would copy an
 // uninitialized scalar after such a rejected read in the original code.
 CMemoryDataInputStream guard;guard.StreamPos=0x80000000UL;guard.SourceChunkBytesRemaining=8;guard.CurrentSourcePtr=raw;
 unsigned int sentinel=0x12345678;guard.Read(&sentinel,4);CHECK(sentinel==0x12345678&&guard.StreamPos==0x80000000UL);
 puts("0057f9e9_TEST PASS");return 0;
}