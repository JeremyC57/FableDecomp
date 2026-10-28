// CGraphicDataBank::~CGraphicDataBank() native009FEBC0 / PDB00B5D2A0, FW03117820.
// Leaf: ~CGraphicDataBank
#define CGraphicDataBank CGraphicDataBankDonor
#include "engine/_quarantine/CGraphicDataBank.h"
#undef CGraphicDataBank
#include "engine/CBankFile.h"
#include "engine/CResourceList.h"
#include "engine/CGraphicBankStateBlock.h"
#define CTexture CTexturePdb
#include "engine/CTexture.h"
#undef CTexture
#include "engine/CPixelFormat.h"
#include "fable_string.h"
#include <stdlib.h>
struct CharArray;
// RTTI129C8CC/129C938: primary Async->BankFile->NonCopy->Base; secondary ResourceBank->NonCopy->Base.
// Native BankFile110 (ctor9CD480 final ChecksumCacheF8..110), Async164 (ctor9D5F80); donor174/1D8.
class CBaseClass { public:CBaseClass();virtual ~CBaseClass();};
struct CBaseClassNonCopyable:CBaseClass {CBaseClassNonCopyable();virtual ~CBaseClassNonCopyable();};
struct BankState;struct OutputStream;struct BankTypeCount;struct PointerInfo;
template<class T>struct BankArrayView {T*First,*Last,*End;};
struct StatePointer {BankState*Data;PointerInfo*Info;StatePointer(const StatePointer&);~StatePointer();}; // genuine non-POD counted pointer passed by value
class CDateAndTime { public:fable_i32 Year;fable_u32 Month,Day,Hour,Minute,Second,Millisecond;CDateAndTime();}; // genuine defaultFW0308B860; hidden-result class
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
class CBankFileAsync:public CBankFileNative { public:CBankFileAsync();virtual ~CBankFileAsync();virtual bool OpenReadOnly(const CCharString&,unsigned long);virtual void OpenReadWrite(const CCharString&,unsigned long,unsigned long);virtual bool OpenFileReadOnly(const CWideString&,unsigned long);virtual void OpenFileReadWrite(const CWideString&,unsigned long,unsigned long);virtual void Close();unsigned char UntouchedAsyncState[0x54];}; // defaultFW02FF5CA0; dtor9D6490
class CResourceBank:public CBaseClassNonCopyable { public:CResourceBank();virtual ~CResourceBank();unsigned char UntouchedResourceLists[0x78];}; // defaultFW03178950/9FC5F0, dtor9FC690; native124

struct GraphicInfo;
struct GraphicInfoFrame:CBaseClass {GraphicInfoFrame();virtual ~GraphicInfoFrame();unsigned short FrameNum;signed char SmallMipmapLevel;void*Data;GraphicInfo*GraphicInfoPtr;}; // VT129C878->A009A0; true BaseClass hierarchy; FW03126720/031267D0
struct GraphicHeader {unsigned short Width,Height,Depth,FrameWidth,FrameHeight,FrameCount;fable_i32 PixelFormat;unsigned char TransparencyType,MipmapLevels,Flags,Padding;fable_u32 FrameDataSize,CompressedMipmapSizes;};
struct GraphicInfo {GraphicInfo();~GraphicInfo(){Bank=0;Frames=0;}GraphicHeader Header;fable_i32 TextureType;void*Bank;fable_i32 Index;GraphicInfoFrame*Frames;}; // actual defaultFW03125A40,dtor03125C20;44 native
struct GraphicArray {GraphicInfo*First,*Last,*End;GraphicArray():First(0),Last(0),End(0){}GraphicInfo*erase(GraphicInfo*,GraphicInfo*);void clear(){erase(First,Last);}~GraphicArray(){for(GraphicInfo*end=Last,*p=First;p!=end;++p)p->~GraphicInfo();if(First)free(First);}};
struct FrameArray {GraphicInfoFrame*First,*Last,*End;FrameArray():First(0),Last(0),End(0){}GraphicInfoFrame*erase(GraphicInfoFrame*,GraphicInfoFrame*);void clear(){erase(First,Last);}~FrameArray(){for(GraphicInfoFrame*end=Last,*p=First;p!=end;++p)p->~GraphicInfoFrame();if(First)free(First);}};
// Retail VC7 vector iterators are raw pointers, returned in EAX, RET8; A00E60/A00ED0.
// Newer PDB FW0312A980/0312AE40 confirms vector erase(range), but has newer checked iterator classes.
struct BankState {BankState();virtual ~BankState(){}virtual void GetDefault(BankState&)=0;virtual void Set(const BankState*)=0;virtual unsigned long CreateCRC()=0;}; // genuine defaultFW02D6BDA0; native VT129C82C four slots
struct GraphicBankState:BankState {GraphicBankState();virtual ~GraphicBankState(){}virtual void GetDefault(BankState&);virtual void Set(const BankState*);virtual unsigned long CreateCRC();unsigned char State[96];}; // ctorFW02D8ACE0; VT129C888[A00920,9FCEF0,A00940,9FD020]
struct CTexture:CTexturePdb {CTexture();void Uninitialise();~CTexture(){Uninitialise();}}; // CTexture actual defaultFW018BB0B0, Uninitialise 9F9F70 (?Uninitialise@CTexture@@QAEXXZ);8
struct PointerInfo {fable_u32 ReferenceCount;void(__fastcall*DeleteFunction)(void*);void*Data;};
struct TextureManagerPointer {void*Data;PointerInfo*Info;TextureManagerPointer():Data(0),Info(0){}~TextureManagerPointer(){if(Info){--Info->ReferenceCount;if(Info->ReferenceCount==0){Info->DeleteFunction(Info->Data);delete Info;}}Data=0;Info=0;}};
class CGraphicDataBank:public CBankFileAsync,public CResourceBank { public:CGraphicDataBank();void CloseGraphics(){GraphicList.clear();GraphicFrameBuffer.clear();CBankFileNative::Close();}
 virtual ~CGraphicDataBank();
 virtual void Close();virtual unsigned long GetDataFormatVersion();virtual void NotifyBankSize(unsigned long,const BankArrayView<BankTypeCount>&);virtual void NotifyBankOpened();
 virtual void NotifyEntryAdded(unsigned long,unsigned long,const CharArray&);virtual void NotifyEntryUpdated(unsigned long,unsigned long,const CharArray&);virtual void NotifyEntryRemoved(unsigned long);
 virtual void PrepareDataForInclusion(unsigned long,const BankArrayView<CCharString>&,unsigned long,StatePointer,OutputStream&,OutputStream&);
 virtual unsigned long GetNoFramesInGraphic(unsigned long)const;
 virtual unsigned long GetGraphicFrameWidth(unsigned long,unsigned long)const;
 virtual unsigned long GetGraphicFrameHeight(unsigned long,unsigned long)const;
 virtual unsigned long GetGraphicMaxWidth(unsigned long)const;
 virtual unsigned long GetGraphicMaxHeight(unsigned long)const;
 virtual bool IsUsingSurfaces()const;

 GraphicArray GraphicList;FrameArray GraphicFrameBuffer;bool Initialised;GraphicBankState StateBlock;
 fable_i32 TextureSizeWarningMaxWidth,TextureSizeWarningMaxHeight;fable_u32 UnloadFrameDelay,ReduceSizeFrameDelay,TexturePointersValidForFrames;fable_i32 MinAvailablePreloadMemory,AvailableMemory;bool FreezeGraphics;
 TextureManagerPointer TextureManager;fable_i32 PixelFormats[11];CTexture BlankTextures[11];
};
CGraphicDataBank::~CGraphicDataBank(){CloseGraphics();}