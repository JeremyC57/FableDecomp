// CGraphicDataBank::InitFrames() PDB00B5CE60. Leaf: InitFrames
#define CGraphicDataBank CGraphicDataBankDonor
#include "engine/_quarantine/CGraphicDataBank.h"
#undef CGraphicDataBank
#include "engine/CPixelFormat.h"
// Native CGraphicInfo=44 and frame=16 match donor. Bank arrays retail1E0/1EC vs donor254/264.
struct GraphicInfo;
struct GraphicInfoFrame {GraphicInfoFrame();void*VTable;unsigned short FrameNum;signed char SmallMipmapLevel;void*Data;GraphicInfo*GraphicInfoPtr;}; // genuine ctor PDB00B580A0; data-only projection
#pragma pack(push,1)
struct GraphicHeader {unsigned short Width,Height,Depth,FrameWidth,FrameHeight,FrameCount;CPixelFormat PixelFormat;unsigned char TransparencyType,MipmapLevels,Flags,Padding;unsigned long FrameDataSize,CompressedMipmapSizes;};
#pragma pack(pop)
struct GraphicInfo {GraphicInfo();GraphicHeader Header;long TextureType;void*Bank;long Index;GraphicInfoFrame*Frames;}; // genuine ctor PDB00B57D00
struct GraphicArray {GraphicInfo*First,*Last,*End;unsigned long size()const{return Last-First;}};
struct FrameArray {GraphicInfoFrame*First,*Last,*End;void resize(unsigned int);}; // std::vector resize(uint), PDB00B5F1E0 native00A016F0
struct CGraphicDataBank {CGraphicDataBank();unsigned char NativeBaseState[0x1e0];GraphicArray GraphicList;FrameArray GraphicFrameBuffer;void InitFrames();}; // genuine ctor PDB00B5CCB0; untouched native base opaque
void CGraphicDataBank::InitFrames(){
 unsigned long count=GraphicList.size(),total=0;
 for(unsigned long i=0;i<count;++i)if(GraphicList.First[i].Index!=-1)total+=GraphicList.First[i].Header.FrameCount;
 GraphicFrameBuffer.resize(total);
 unsigned long offset=0;
 for(unsigned long i=0;i<count;++i){GraphicInfo&g=GraphicList.First[i];if(g.Index!=-1){g.Frames=GraphicFrameBuffer.First+offset;unsigned long n=g.Header.FrameCount;for(unsigned long j=0;j<n;++j){GraphicInfoFrame&f=g.Frames[j];f.FrameNum=(unsigned short)j;f.GraphicInfoPtr=&g;f.SmallMipmapLevel=0;unsigned long size=g.Header.FrameWidth>g.Header.FrameHeight?g.Header.FrameWidth:g.Header.FrameHeight;while(size>32){size>>=1;++f.SmallMipmapLevel;}if(f.SmallMipmapLevel>(long)g.Header.MipmapLevels-1)f.SmallMipmapLevel=g.Header.MipmapLevels-1;}offset+=n;}}
}
