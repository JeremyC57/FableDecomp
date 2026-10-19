// CGraphicDataBank::NotifyEntryAdded(ulong,ulong,const CArray<char>&), PDB00B5D420
// Leaf: NotifyEntryAdded@CGraphicDataBank
#define CGraphicDataBank CGraphicDataBankDonor
#include "engine/_quarantine/CGraphicDataBank.h"
#undef CGraphicDataBank
#include "engine/CBankFile.h"
#include "engine/CDataInputStream.h"
#include "engine/CMemoryDataInputStream.h"
#include "engine/CPixelFormat.h"
struct CharString;struct WideString;
struct DataStream {DataStream();virtual ~DataStream();}; // genuine ctor00AFBB90, dtor00AFBBB0, native99A2F0/99A300
struct InputStream:DataStream {InputStream();virtual ~InputStream();virtual void SetPosition(unsigned long);virtual unsigned long GetPosition()const;virtual void Close();virtual void ReadCompressed(void*,long);virtual void ReadZlibCompressed(void*,long);virtual CharString ReadNullTerminatedString();virtual WideString ReadNullTerminatedWString();virtual void InternalReadNextSourceChunk(const unsigned char*&,long&);virtual bool NeedsBufferedRead(long);virtual void InternalReadUnbuffered(unsigned char*,long);unsigned long StreamPos;long StreamSize;unsigned char*CurrentSourcePtr;unsigned long SourceChunkStreamPos;long SourceChunkBytesRemaining;};
struct MemoryInput:InputStream {MemoryInput(const void*,unsigned long);virtual CharString ReadNullTerminatedString();virtual WideString ReadNullTerminatedWString();virtual void ReadCompressed(void*,long);unsigned char*StreamSource;}; // ctor994930/PDBAFCA20; implicit dtor calls InputStream dtor993B10; nativeVT123675C has all11 genuine slots
struct CharArray {char*First,*Last,*End;unsigned long size()const{return Last-First;}const char*data()const{return size()?First:reinterpret_cast<const char*>(this);}};
struct PixelFormatValue:CPixelFormat {PixelFormatValue(){FormatIndex=-1;}};
struct GraphicHeader {void Load(InputStream&);unsigned short Width,Height,Depth,FrameWidth,FrameHeight,FrameCount;PixelFormatValue PixelFormat;unsigned char TransparencyType,MipmapLevels;unsigned char IsFlatAnimatedGraphic:1;unsigned long FrameDataSize,CompressedMipmapSizes;GraphicHeader():Width(0),Height(0),Depth(0),FrameWidth(0),FrameHeight(0),FrameCount(1),TransparencyType(0),MipmapLevels(0),IsFlatAnimatedGraphic(false),FrameDataSize(0),CompressedMipmapSizes(0){}}; // genuine ctorPDBB57C50; byte18 remaining bits untouched
struct CGraphicDataBank;
struct GraphicInfo {GraphicInfo(unsigned long,CGraphicDataBank*,const GraphicHeader&);GraphicHeader Header;long TextureType;CGraphicDataBank*Bank;long Index;void*Frames;}; // ctor9FD700/PDBB57C80
struct GraphicArray {GraphicInfo*First,*Last,*End;unsigned long size()const{return Last-First;}void Add();}; //native A01650 appends one default GraphicInfo
struct BankMethods {void NotifyEntryAdded(unsigned long,unsigned long,const CharArray&);}; //CBankFile qualified nonvirtual call9CBC20, PDBB19970
struct CGraphicDataBank {CGraphicDataBank();unsigned char NativeBase[0x1e0];GraphicArray GraphicList;void NotifyEntryAdded(unsigned long,unsigned long,const CharArray&);};
void CGraphicDataBank::NotifyEntryAdded(unsigned long type,unsigned long index,const CharArray&data){
 while(index>=GraphicList.size())GraphicList.Add();
 MemoryInput input(data.data(),data.size());
 GraphicHeader header;header.Load(input);
 GraphicInfo* entries=GraphicList.First;entries[index]=GraphicInfo(index,this,header);
 reinterpret_cast<BankMethods*>(this)->NotifyEntryAdded(type,index,data);
}
