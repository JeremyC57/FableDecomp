// CGraphicInfoFrame::CFrameData::CFrameData(CGraphicInfoFrame*) -- construct frame resources and register resource size with the graphic bank
// Retail009fe380181bytes. Actual flattened CResource/CResourceList/CGraphicFrame headers; absent CFrameData/CManagedTexture headers modeled from PDB.
// Bank native ResourceBank+164, CurrentFrame+194; donor bank base shrinks by STL layout differences.
// Callees CResource ctor009fbec0,CBase ctor0099a2d0,GraphicFrameData::Sizeof009fcbf0,ManagedTexture::Sizeof00a69e60,SetResourceSize009fbf20,ResourceBank::AddResource009fc380.
#include "engine/CResource.h"
#include "engine/CResourceList.h"
#include "engine/CGraphicFrame.h"
#define CGraphicDataBank CGraphicDataBankDonor
#include "engine/_quarantine/CGraphicDataBank.h"
#undef CGraphicDataBank
// Donor bank has opaque inherited bytes: native ResourceBank+164 and CurrentFrame+194 are verified by009fe380 and CResourceList+2c; donor derived fields start254 and disagree.
struct CGraphicFrameData {long Sizeof();}; //009fcbf0
extern void* g_FrameDataVtable[]; //0129c89c CGraphicInfoFrame::CFrameData
extern void* g_ManagedTextureVtable[]; //0129c880 CManagedTexture
struct CBaseNative {void* Vtable;CBaseNative();}; //0099a2d0
struct CManagedTexture : CBaseNative {
 void* Data;
 CManagedTexture(){Vtable=g_ManagedTextureVtable;Data=0;}
 long Sizeof(); //00a69e60
};
struct CResourceNative : CResource {CResourceNative();void SetResourceSize(unsigned long);}; //009fbec0,009fbf20
struct CResourceBank {void* Vtable;CResourceList ResourceList;void AddResource_009fc380(CResource*);}; //009fc380
struct CGraphicDataBank {unsigned char Base[0x164];CResourceBank Resources;};
struct CGraphicHeader {unsigned char Data[0x1c];};
struct CGraphicInfo {CGraphicHeader Header;long TextureType;CGraphicDataBank* Bank;long Index;void* Frames;};
struct CGraphicInfoFrame {long Reserved;unsigned short FrameNum;signed char SmallMipmapLevel;void* Data;CGraphicInfo* GraphicInfo;};
// CFrameData is the physical-field view of nested CGraphicInfoFrame::CFrameData (RTTI0129c89c).
struct CFrameMember : CGraphicFrame {
 long Sizeof() const{return FrameData?FrameData->Sizeof()+4:4;}
 CFrameMember(CResource* owner){owner->__vftable=g_FrameDataVtable;FrameData=0;}
};
struct CFrameData : CResourceNative {
 CFrameMember Frame;CManagedTexture ManagedTexture,LoadingManagedTexture;void* FileData[2];signed char CurrentMipmapLevel,LoadingMipmapLevel,TouchedMipmapLevel;unsigned long TouchedFrame;CGraphicInfoFrame* GraphicInfoFrame;
 long Sizeof(){long n=Frame.Sizeof()+0x44;if(LoadingManagedTexture.Data)n+=LoadingManagedTexture.Sizeof();return n;}
 CFrameData(CGraphicInfoFrame*);
};
CFrameData::CFrameData(CGraphicInfoFrame* frame):Frame(this) {
 FileData[0]=0;FileData[1]=0;GraphicInfoFrame=frame;
 CurrentMipmapLevel=-1;TouchedMipmapLevel=127;
 TouchedFrame=frame->GraphicInfo->Bank->Resources.ResourceList.CurrentFrame-1;
 SetResourceSize(Sizeof());
 frame->GraphicInfo->Bank->Resources.AddResource_009fc380(this);
}