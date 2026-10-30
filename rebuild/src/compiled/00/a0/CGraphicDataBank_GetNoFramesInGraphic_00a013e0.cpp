// CGraphicDataBank::GetNoFramesInGraphic(unsigned long index) const -- Return the graphic header field.
// Leaf: ?GetNoFramesInGraphic@CGraphicDataBank@@UBEKK@Z
// Identity: Own CGraphicDataBank vtable129c8cc slot20 and PDB exact const virtual overload. GraphicList+480 size44; header FrameCount read. Out-of-range access has retail non-null precondition and is not converted to a zero result.
#define CBankFile CBankFile_HeaderLayout
#include "engine/CBankFile.h"
#undef CBankFile
#include "engine/CPixelFormat.h"

#ifndef _MSC_VER
#define __forceinline inline
#endif
// Layout: shared CBankFile header is flattened. Restore its proven CBaseClassNonCopyable base and retain every shared member. Retail CBankFile size272, CBankFileAsync356; RTTI CGraphicDataBank has CResourceBank at356, GraphicList at480. The async and resource payloads are opaque here and never accessed.
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0 (slot 0), implicit copy
class CBaseClassNonCopyable:public CBaseClass{};
class CBankFileEntryDependencies;
class CCPPointerInfo;
class CCharStringData;
class CDiskFileWin32;
struct CProgressCallback;
class CWideStringData;

#pragma pack(push,1)
class CBankFile:public CBaseClassNonCopyable {public:
    unsigned long               Size;                        // +0x04
    unsigned char               Symbols[0xc];                // +0x08 CArray<CCharString>
    unsigned char               Checksums[0xc];              // +0x14 CArray<unsigned_long>
    unsigned char               RuntimeData[0xc];            // +0x20 CArray<CBankFile::CRuntimeData>
    unsigned char               UpdateData[0xc];             // +0x2c CArray<CBankFileEntryUpdateData_*>
    unsigned char               PackedDataOffset[0x10];      // +0x38 CPackedUIntArray
    unsigned char               PackedDataSize[0x10];        // +0x48 CPackedUIntArray
    unsigned char               PackedDataType[0x10];        // +0x58 CPackedUIntArray
    unsigned char               PackedValid[0x10];           // +0x68 CPackedUIntArray
    unsigned long               OpenFlags;                   // +0x78
    CDiskFileWin32*             BankFile_Data;               // +0x7c CCountedPointer<CDiskFileWin32>::Data
    CCPPointerInfo*             BankFile_Info;               // +0x80 CCountedPointer<CDiskFileWin32>::Info
    CWideStringData*            Filename_PStringData;        // +0x84 CWideString::PStringData
    CCharStringData*            BankHandle_PStringData;      // +0x88 CCharString::PStringData
    bool                        RetailMode;                  // +0x8c
    bool                        ReadOnly;                    // +0x8d
    bool                        WrittenToFile;               // +0x8e
    bool                        Solid;                       // +0x8f
    unsigned long               ChangeCount;                 // +0x90
    bool                        FileValid;                   // +0x94
    bool                        UpdatingFlag;                // +0x95
    unsigned char               _pad_0x96[0x2];              // +0x96
    unsigned long               Alignment;                   // +0x98
    unsigned long               BankFormatVersion;           // +0x9c
    unsigned long               HeaderSize;                  // +0xa0
    unsigned long               FileInformationSize;         // +0xa4
    unsigned long               FileSize;                    // +0xa8
    unsigned char               SymbolIndices[0xc];          // +0xac map<CCharString,unsigned_long,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,unsigned_long>_>_>
    unsigned char               OldSymbolIndices[0xc];       // +0xb8 map<CCharString,unsigned_long,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,unsigned_long>_>_>
    unsigned char               FilenameIndices[0xc];        // +0xc4 multimap<CCharString,unsigned_long,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,unsigned_long>_>_>
    unsigned char               SymbolCRCIndices[0x10];      // +0xd0 CVectorMap<unsigned_long,unsigned_long,CKeyPairCompareLess<unsigned_long,unsigned_long>_>
    unsigned char               BlockOrder[0xc];             // +0xe0 CArray<unsigned_long>
    CProgressCallback*          ProgressCallback;            // +0xec
    CCharStringData*            HeaderEnumerationType_PStringData; // +0xf0 CCharString::PStringData
    bool                        WarningFlag;                 // +0xf4
    unsigned char               _pad_0xf5[0x3];              // +0xf5
    unsigned char               ChecksumCache[0x18];         // +0xf8 CChecksumCacheFile
};
class CBankFileAsync:public CBankFile {public:unsigned char UnusedAsyncPayload[84];};
class CResourceBank:public CBaseClassNonCopyable {public:unsigned char ResourceList[60],LoadingQueue[60];};
class CGraphicInfoFrame;class CGraphicDataBank;enum ETextureType{TEXTURE_TYPE_UNKNOWN=0};
#pragma pack(pop)
struct CGraphicHeader {unsigned short Width,Height,Depth,FrameWidth,FrameHeight,FrameCount;CPixelFormat PixelFormat;unsigned char TransparencyType,MipmapLevels,IsFlatAnimatedGraphic:1;unsigned long FrameDataSize,CompressedMipmapSizes[1];};
// Layout: CGraphicHeader28 with natural padding at19; CGraphicInfo44. Original member names from PDB, fields confirmed by own reads.
class CGraphicInfo {public:CGraphicHeader Header;ETextureType TextureType;CGraphicDataBank* Bank;long Index;CGraphicInfoFrame* Frames;};
namespace std {
template<class T>class allocator{};
template<class T,class A>struct _STLP_alloc_proxy:A{T _M_data;};
template<class T,class A>class _Vector_base {public:T* _M_start;T* _M_finish;_STLP_alloc_proxy<T*,A> _M_end_of_storage;};
template<class T,class A=allocator<T> >class vector:public _Vector_base<T,A>{public:__forceinline unsigned int size()const{return this->_M_finish-this->_M_start;}__forceinline const T& operator[](unsigned int index)const{return this->_M_start[index];}};
}
template<class T>class CArray:public std::vector<T>{};
class CGraphicDataBank:public CBankFileAsync,public CResourceBank {public:
 CArray<CGraphicInfo> GraphicList;
 __forceinline const CGraphicInfo* GetGraphicInfo(unsigned long index)const{return index<GraphicList.size()?&GraphicList[index]:0;}
 virtual unsigned long GetNoFramesInGraphic(unsigned long)const;
 virtual unsigned long GetGraphicFrameWidth(unsigned long,unsigned long)const;
 virtual unsigned long GetGraphicFrameHeight(unsigned long,unsigned long)const;
};
unsigned long CGraphicDataBank::GetNoFramesInGraphic(unsigned long index)const{return GetGraphicInfo(index)->Header.FrameCount;}