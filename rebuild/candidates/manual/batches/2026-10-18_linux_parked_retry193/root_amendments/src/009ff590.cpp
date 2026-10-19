// CGraphicDataBank::NotifyEntryAdded(ulong,ulong,const CArray<char>&), PDB00B5D420
// Leaf: NotifyEntryAdded@CGraphicDataBank
#define CGraphicDataBank CGraphicDataBankDonor
#include "engine/_quarantine/CGraphicDataBank.h"
#undef CGraphicDataBank
#include "engine/CBankFile.h"
#include "engine/CDataInputStream.h"
#include "engine/CMemoryDataInputStream.h"
#include "engine/CPixelFormat.h"
#include "fable_string.h"
struct CharArray;
// RTTI129C8CC/129C938: primary Async->BankFile->NonCopy->Base; secondary ResourceBank->NonCopy->Base.
// Native BankFile110 (ctor9CD480 final ChecksumCacheF8..110), Async164 (ctor9D5F80); donor174/1D8.
struct CBaseClass {CBaseClass();virtual ~CBaseClass();};
struct CBaseClassNonCopyable:CBaseClass {CBaseClassNonCopyable();virtual ~CBaseClassNonCopyable();};
struct BankState;struct OutputStream;struct BankTypeCount;struct PointerInfo;
template<class T>struct BankArrayView {T*First,*Last,*End;};
struct StatePointer {BankState*Data;PointerInfo*Info;StatePointer(const StatePointer&);~StatePointer();}; // genuine non-POD counted pointer passed by value
struct CDateAndTime {fable_i32 Year;fable_u32 Month,Day,Hour,Minute,Second,Millisecond;CDateAndTime();}; // genuine defaultFW0308B860; hidden-result class
struct CBankFileNative:CBaseClassNonCopyable {
 CBankFileNative();virtual ~CBankFileNative();
 virtual bool OpenReadOnly(const CCharString&,unsigned long);
 virtual void OpenReadWrite(const CCharString&,unsigned long,unsigned long);
 virtual bool OpenFileReadOnly(const CWideString&,unsigned long);
 virtual void OpenFileReadWrite(const CWideString&,unsigned long,unsigned long);
 virtual void Close();
 virtual void AddEntryMultipleFiles(const CCharString&,unsigned long,const BankArrayView<CCharString>&,StatePointer);
 virtual void Update();
 virtual unsigned long GetDataFormatVersion()=0;
 virtual unsigned long AdjustUpdateEntries();
 virtual void NotifyBankSize(unsigned long,const BankArrayView<BankTypeCount>&);
 virtual void NotifyBankOpened();
 virtual void NotifyEntryAdded(unsigned long,unsigned long,const CharArray&);
 virtual void NotifyEntryUpdated(unsigned long,unsigned long,const CharArray&);
 virtual void NotifyEntryRemoved(unsigned long);
 virtual void NotifyPostEntriesLoaded();
 virtual void PrepareDataForInclusion(unsigned long,const BankArrayView<CCharString>&,unsigned long,StatePointer,OutputStream&,OutputStream&);
 virtual bool IsDiskFile(unsigned long,unsigned long);
 virtual CDateAndTime GetLastModifiedTime(const CCharString&);
 virtual CCharString GetEnumerationType(unsigned long)const;
 unsigned char UntouchedBankState[0x10c];
};
struct CBankFileAsync:CBankFileNative {CBankFileAsync();virtual ~CBankFileAsync();virtual bool OpenReadOnly(const CCharString&,unsigned long);virtual void OpenReadWrite(const CCharString&,unsigned long,unsigned long);virtual bool OpenFileReadOnly(const CWideString&,unsigned long);virtual void OpenFileReadWrite(const CWideString&,unsigned long,unsigned long);virtual void Close();unsigned char UntouchedAsyncState[0x54];}; // defaultFW02FF5CA0; dtor9D6490
struct CResourceBank:CBaseClassNonCopyable {CResourceBank();virtual ~CResourceBank();unsigned char UntouchedResourceLists[0x78];}; // defaultFW03178950/9FC5F0, dtor9FC690; native124

#include "fable_string.h"
// String-returning virtuals use genuine non-POD classes and VC7.1 hidden-result ABI.
struct DataStream:CBaseClassNonCopyable {DataStream();virtual ~DataStream();}; // genuine ctor00AFBB90, dtor00AFBBB0, native99A2F0/99A300
struct InputStream:DataStream {InputStream();virtual ~InputStream();virtual void SetPosition(unsigned long);virtual unsigned long GetPosition()const;virtual void Close();virtual void ReadCompressed(void*,long);virtual void ReadZlibCompressed(void*,long);virtual CCharString ReadNullTerminatedString();virtual CWideString ReadNullTerminatedWString();virtual void InternalReadNextSourceChunk(const unsigned char*&,long&);virtual bool NeedsBufferedRead(long);virtual void InternalReadUnbuffered(unsigned char*,long);unsigned long StreamPos;long StreamSize;unsigned char*CurrentSourcePtr;unsigned long SourceChunkStreamPos;long SourceChunkBytesRemaining;};
struct MemoryInput:InputStream {MemoryInput(const void*,unsigned long);virtual CCharString ReadNullTerminatedString();virtual CWideString ReadNullTerminatedWString();virtual void ReadCompressed(void*,long);unsigned char*StreamSource;}; // ctor994930/PDBAFCA20; implicit dtor calls InputStream dtor993B10; nativeVT123675C has all11 genuine slots
struct CharArray {char*First,*Last,*End;unsigned long size()const{return Last-First;}const char*data()const{return size()?First:reinterpret_cast<const char*>(this);}};
struct PixelFormatValue:CPixelFormat {PixelFormatValue(){FormatIndex=-1;}};
struct GraphicHeader {void Load(InputStream&);unsigned short Width,Height,Depth,FrameWidth,FrameHeight,FrameCount;PixelFormatValue PixelFormat;unsigned char TransparencyType,MipmapLevels;unsigned char IsFlatAnimatedGraphic:1;unsigned long FrameDataSize,CompressedMipmapSizes;GraphicHeader():Width(0),Height(0),Depth(0),FrameWidth(0),FrameHeight(0),FrameCount(1),TransparencyType(0),MipmapLevels(0),IsFlatAnimatedGraphic(false),FrameDataSize(0),CompressedMipmapSizes(0){}}; // genuine ctorPDBB57C50; byte18 remaining bits untouched
struct CGraphicDataBank;
struct GraphicInfo {GraphicInfo(); /* genuine default PDB00B57D00 */ GraphicInfo(unsigned long,CGraphicDataBank*,const GraphicHeader&);GraphicHeader Header;long TextureType;CGraphicDataBank*Bank;long Index;void*Frames;}; // ctor9FD700/PDBB57C80
struct GraphicArray {GraphicInfo*First,*Last,*End;unsigned long size()const{return Last-First;}void Add();}; //native A01650 appends one default GraphicInfo
struct BankMethods {void NotifyEntryAdded(unsigned long,unsigned long,const CharArray&);}; //CBankFile qualified nonvirtual call9CBC20, PDBB19970
struct CGraphicDataBank:CBankFileAsync,CResourceBank {CGraphicDataBank(); virtual ~CGraphicDataBank();
 virtual void Close();virtual unsigned long GetDataFormatVersion();virtual void NotifyBankSize(unsigned long,const BankArrayView<BankTypeCount>&);virtual void NotifyBankOpened();
 virtual void NotifyEntryAdded(unsigned long,unsigned long,const CharArray&);virtual void NotifyEntryUpdated(unsigned long,unsigned long,const CharArray&);virtual void NotifyEntryRemoved(unsigned long);
 virtual void PrepareDataForInclusion(unsigned long,const BankArrayView<CCharString>&,unsigned long,StatePointer,OutputStream&,OutputStream&);
 virtual unsigned long GetNoFramesInGraphic(unsigned long)const;
 virtual unsigned long GetGraphicFrameWidth(unsigned long,unsigned long)const;
 virtual unsigned long GetGraphicFrameHeight(unsigned long,unsigned long)const;
 virtual unsigned long GetGraphicMaxWidth(unsigned long)const;
 virtual unsigned long GetGraphicMaxHeight(unsigned long)const;
 virtual bool IsUsingSurfaces()const;
GraphicArray GraphicList;unsigned char UntouchedTail[0x120];};
void CGraphicDataBank::NotifyEntryAdded(unsigned long type,unsigned long index,const CharArray&data){
 while(index>=GraphicList.size())GraphicList.Add();
 MemoryInput input(data.data(),data.size());
 GraphicHeader header;header.Load(input);
 GraphicInfo* entries=GraphicList.First;entries[index]=GraphicInfo(index,this,header);
 CBankFileNative::NotifyEntryAdded(type,index,data);
}
