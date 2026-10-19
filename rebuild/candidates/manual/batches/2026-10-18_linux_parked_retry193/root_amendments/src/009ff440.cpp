// CGraphicDataBank::InitFrames() PDB00B5CE60. Leaf: InitFrames
#define CGraphicDataBank CGraphicDataBankDonor
#include "engine/_quarantine/CGraphicDataBank.h"
#undef CGraphicDataBank
#include "engine/CBankFile.h"
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

// Native CGraphicInfo=44 and frame=16 match donor. Bank arrays retail1E0/1EC vs donor254/264.
struct GraphicInfo;
struct GraphicInfoFrame:CBaseClass {GraphicInfoFrame();virtual ~GraphicInfoFrame();unsigned short FrameNum;signed char SmallMipmapLevel;void*Data;GraphicInfo*GraphicInfoPtr;}; // genuine ctor PDB00B580A0; data-only projection
#pragma pack(push,1)
struct GraphicHeader {unsigned short Width,Height,Depth,FrameWidth,FrameHeight,FrameCount;CPixelFormat PixelFormat;unsigned char TransparencyType,MipmapLevels,Flags,Padding;unsigned long FrameDataSize,CompressedMipmapSizes;};
#pragma pack(pop)
struct GraphicInfo {GraphicInfo();GraphicHeader Header;long TextureType;void*Bank;long Index;GraphicInfoFrame*Frames;}; // genuine ctor PDB00B57D00
struct GraphicArray {GraphicInfo*First,*Last,*End;__forceinline unsigned long size()const{return Last-First;} __forceinline GraphicInfo& operator[](unsigned long i){return First[i];}};
struct FrameArray {GraphicInfoFrame*First,*Last,*End;void resize(unsigned int);}; // std::vector resize(uint), PDB00B5F1E0 native00A016F0
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
GraphicArray GraphicList;FrameArray GraphicFrameBuffer;unsigned char UntouchedTail[0x114];void InitFrames();}; // genuine ctor PDB00B5CCB0; untouched native base opaque
void CGraphicDataBank::InitFrames(){
 unsigned long total=0;unsigned long count=GraphicList.size();
 for(unsigned long i=0;i<count;++i)if(GraphicList[i].Index!=-1)total+=GraphicList[i].Header.FrameCount;
 GraphicFrameBuffer.resize(total);
 unsigned long offset=0;
 for(unsigned long i=0;i<count;++i){GraphicInfo&g=GraphicList[i];if(g.Index!=-1){g.Frames=GraphicFrameBuffer.First+offset;unsigned long n=g.Header.FrameCount;for(unsigned long j=0;j<n;++j){GraphicInfoFrame&f=g.Frames[j];f.FrameNum=(unsigned short)j;f.GraphicInfoPtr=&g;f.SmallMipmapLevel=0;unsigned long size=g.Header.FrameWidth>g.Header.FrameHeight?g.Header.FrameWidth:g.Header.FrameHeight;while(size>32){size>>=1;++f.SmallMipmapLevel;}if(f.SmallMipmapLevel>(long)g.Header.MipmapLevels-1)f.SmallMipmapLevel=g.Header.MipmapLevels-1;}offset+=n;}}
}
