// 0057f9e9 CPersistTraits<CHeroMarriageInfo>::TransferBinaryIn
// Leaf: ?TransferBinaryIn@?$CPersistTraits@VCHeroMarriageInfo@@@@QAEXAAVCMemoryDataInputStream@@AAVCHeroMarriageInfo@@@Z
// Flags: /O1 /Oy-
#include "fable_compiler.h"
#include <stddef.h>
#include "engine/CBaseClassNonCopyable.h"
#define CDataInputStream CDataInputStreamFields
#include "engine/CDataInputStream.h"
#undef CDataInputStream
#define CMemoryDataInputStream CMemoryDataInputStreamFields
#include "engine/CMemoryDataInputStream.h"
#undef CMemoryDataInputStream
#define CHeroMarriageInfo CHeroMarriageInfoFields
#include "engine/CHeroMarriageInfo.h"
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
 FABLE_FORCEINLINE long ReadSLONG(){return ReadType<long>();}
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
FABLE_STATIC_ASSERT(sizeof(CHeroMarriageInfo)==24);
FABLE_STATIC_ASSERT(sizeof(CDataInputStream)==24);
FABLE_STATIC_ASSERT(sizeof(CMemoryDataInputStream)==28);