// 00ba9cb0 CEnginePrimitiveManagerMeshBase::GetAppropriateMipmap
// Leaf: ?GetAppropriateMipmap@CEnginePrimitiveManagerMeshBase@@SIKABVC3DVector@@ABVCMatrix3x4@@ABVC3DPrimitive2@@MPBVC3DMeshMaterial@@PBVCGraphicDataBank@@@Z
// Retail-asm-helper: GFFloatToLongFloor
// Retail-asm: 0x00BA9DE2..0x00BA9DFC uses fistp dword for a rounded conversion and compares the next integral float.
// Flags: /d2inlT250
// Inline-budget: retail expands CGraphicDataBank::PeekGraphic at the width lookup. Plain /O2 emits a call.
#define FABLE_RETAIL_MESH_MIPMAP_PASS30
#define FABLE_RETAIL_C3DMESHMATERIAL_MAPBANKINDICES
#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_CBANKFILE_ENTRYQUERY
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CMOVABLERESOURCE
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CTEXTURE
#define FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS
#define FABLE_RETAIL_CGRAPHICINFO_CONSTRUCT
#define FABLE_RETAIL_CGRAPHICINFO_GETWIDTH
#define FABLE_RETAIL_CGRAPHICDATABANK_PEEKGRAPHIC
#define FABLE_RETAIL_CENGINEPRIMITIVEMANAGERMESHBASE_GETAPPROPRIATEMIPMAP
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERUTIL_MESHPRIMITIVE
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERERSHAREDDATA_MIPMAPSIZEMULTIPLIER
#define FABLE_RETAIL_C3DPRIMITIVE2_HIERARCHY
#define FABLE_RETAIL_C3DPRIMITIVE2_RENDER
#define FABLE_RETAIL_CMATRIX3X4
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_MAGNITUDE_NAMED_SQUARED
#include "rebuild_abi.h"
#include "engine/CMatrix3x4.h"
#include "engine/C3DPrimitive2.h"
#include "engine/C3DMeshMaterial.h"
#include "engine/CGraphicDataBank.h"
#include "engine/CGraphicInfo.h"
#include "engine/CEnginePrimitiveManagerMeshBase.h"
#include "engine/CEnginePrimitiveRenderUtil.h"
#include "engine/GFFloatToLongFloor.h"
#include "engine/GFGetTopBitInByte.h"

extern const float DELTA_NOTIONAL_ZERO;

#if !defined(MIPMAP_LINK_PRODUCTION)
unsigned long FABLE_FASTCALL CEnginePrimitiveManagerMeshBase::GetAppropriateMipmap(
    const C3DVector& cam_pos, const CMatrix3x4& object_transform, const C3DPrimitive2& prim,
    float object_scale, const C3DMeshMaterial* material, const CGraphicDataBank* graphic_bank)
{
    C3DBoundingSphere bsphere = prim.GetBoundingSphere();
    object_transform.TransformPoint(bsphere.Center);
    float distance = (cam_pos - bsphere.Center).GetMagnitude();
    unsigned long mipmap_level = 0;
    float min_distance = distance - bsphere.GetRadius() * object_scale;
    float dtexdpos = prim.GetAverageTextureStretch();
    float divisor = min_distance * dtexdpos;
    if (divisor > DELTA_NOTIONAL_ZERO) {
        long diffuse_index = material->GetDiffuseMapBankIndex();
        if (diffuse_index > 0) {
            unsigned long texture_size = graphic_bank->PeekGraphic(diffuse_index)->GetWidth();
            float ideal_size = CEnginePrimitiveRenderUtil::GetMipmapSizeMultiplier() * object_scale / divisor;
            long ratio = GFFloatToLongFloor(texture_size / ideal_size);
            if (ratio < 1)
                ratio = 1;
            else if (ratio > 255)
                ratio = 255;
            mipmap_level = GFGetTopBitInByte((unsigned char)ratio);
        }
    }
    return mipmap_level;
}
#endif

#include <stdio.h>
#include "engine/CDateAndTime.h"
const float DELTA_NOTIONAL_ZERO = 0.0001f;
unsigned char GTopBitInByteTable[256];
namespace NEngineInternal { CEnginePrimitiveRendererSharedData* GPPrimitiveRendererSharedData; }
class MipmapFixture : public CEnginePrimitiveManagerMeshBase {
public:
    using CEnginePrimitiveManagerMeshBase::GetAppropriateMipmap;
};
// Only the banks' storage and texture metadata participate in this computation.
CGraphicInfo::CGraphicInfo(unsigned long index, CGraphicDataBank* bank, const CGraphicDataBank::CGraphicHeader& header)
    : Header(header), TextureType(CGraphicDataBank::TEXTURE_TYPE_OPAQUE), Bank(bank), Index(index), Frames(0)
{
    bank->GraphicList.push_back(*this);
}
C3DPrimitive2::~C3DPrimitive2() { }
void C3DPrimitive2::RelocateData(long, void*, void*, long) { }
bool CMovableResource::CanRelocateData(long) const { return false; }
unsigned long CMovableResource::Verify() const { return 0; }
CBaseClass::~CBaseClass() { }
CBaseClassNonCopyable::~CBaseClassNonCopyable() { }
CResourceBank::CResourceBank() { }
CResourceBank::~CResourceBank() { }
CResourceList::~CResourceList() { }
CBankFile::~CBankFile() {  }
bool CBankFile::OpenReadOnly(const CCharString&,unsigned long) { return false; }
void CBankFile::OpenReadWrite(const CCharString&,unsigned long,unsigned long) {  }
bool CBankFile::OpenFileReadOnly(const CWideString&,unsigned long) { return false; }
void CBankFile::OpenFileReadWrite(const CWideString&,unsigned long,unsigned long) {  }
void CBankFile::Close() {  }
void CBankFile::AddEntryMultipleFiles(const CCharString&,unsigned long,const CArray<CCharString>&,const CCountedPointer<CBankStateBlock>) {  }
void CBankFile::Update() {  }
unsigned long CBankFile::AdjustUpdateEntries() { return 0; }
void CBankFile::NotifyBankSize(unsigned long,const CArray<CBankFileDataTypeCount>&) {  }
void CBankFile::NotifyBankOpened() {  }
void CBankFile::NotifyEntryAdded(unsigned long,unsigned long,const CArray<char>&) {  }
void CBankFile::NotifyEntryUpdated(unsigned long,unsigned long,const CArray<char>&) {  }
void CBankFile::NotifyEntryRemoved(unsigned long) {  }
void CBankFile::NotifyPostEntriesLoaded() {  }
void CBankFile::PrepareDataForInclusion(unsigned long,const CArray<CCharString>&,unsigned long,const CCountedPointer<CBankStateBlock>,CDataOutputStream&,CDataOutputStream&) {  }
bool CBankFile::IsDiskFile(unsigned long,unsigned long) { return false; }
CDateAndTime CBankFile::GetLastModifiedTime(const CCharString&) { return CDateAndTime(); }
CCharString CBankFile::GetEnumerationType(unsigned long) const { return CCharString(); }
CBankFileAsync::~CBankFileAsync() {  }
bool CBankFileAsync::OpenReadOnly(const CCharString&,unsigned long) { return false; }
void CBankFileAsync::OpenReadWrite(const CCharString&,unsigned long,unsigned long) {  }
bool CBankFileAsync::OpenFileReadOnly(const CWideString&,unsigned long) { return false; }
void CBankFileAsync::OpenFileReadWrite(const CWideString&,unsigned long,unsigned long) {  }
void CBankFileAsync::Close() {  }
CGraphicDataBank::~CGraphicDataBank() {  }
unsigned long CGraphicDataBank::GetNoFramesInGraphic(unsigned long) const { return 0; }
unsigned long CGraphicDataBank::GetGraphicFrameWidth(unsigned long,unsigned long) const { return 0; }
unsigned long CGraphicDataBank::GetGraphicFrameHeight(unsigned long,unsigned long) const { return 0; }
unsigned long CGraphicDataBank::GetGraphicMaxWidth(unsigned long) const { return 0; }
unsigned long CGraphicDataBank::GetGraphicMaxHeight(unsigned long) const { return 0; }
bool CGraphicDataBank::IsUsingSurfaces() const { return false; }
void CGraphicDataBank::Close() {  }
unsigned long CGraphicDataBank::GetDataFormatVersion() { return 0; }
void CGraphicDataBank::NotifyBankSize(unsigned long,const CArray<CBankFileDataTypeCount>&) {  }
void CGraphicDataBank::NotifyBankOpened() {  }
void CGraphicDataBank::NotifyEntryUpdated(unsigned long,unsigned long,const CArray<char>&) {  }
void CGraphicDataBank::NotifyEntryAdded(unsigned long,unsigned long,const CArray<char>&) {  }
void CGraphicDataBank::NotifyEntryRemoved(unsigned long) {  }
void CGraphicDataBank::PrepareDataForInclusion(unsigned long,const CArray<CCharString>&,unsigned long,const CCountedPointer<CBankStateBlock>,CDataOutputStream&,CDataOutputStream&) {  }
CBaseClass::CBaseClass() { }
CBaseClassNonCopyable::CBaseClassNonCopyable() { }
bool CResourceList::HandleFailedAllocation(unsigned long) { return false; }
CResource::CResource() { }
CResource::~CResource() { }
CFailedAllocationHandler::~CFailedAllocationHandler() { }
CCharString::CCharString() { }
CCharString::~CCharString() { }
void CTexture::Uninitialise() { }
void CIVCountedPointeeBase::Delete() const { }
bool CResource::UnloadResource() { return false; }
unsigned long CResource::GetUnloadableMemory() const { return 0; }
bool CResource::IsResourceLoaded() const { return false; }
void CResource::OnResourceLoaded() {  }
bool CResource::CanAbortResourceLoad(long) const { return false; }
void CResource::OnResourceLoadAborted() {  }
unsigned long CResource::GetDebugMemoryInUse() const { return 0; }
void CResource::ResetDebugMemoryInUse() {  }
static int check(unsigned long actual, unsigned long expected, unsigned long line)
{
    if (actual == expected) return 0;
    printf("line %lu expected %lu got %lu\n", line, expected, actual);
    return 1;
}
int main()
{
    CEnginePrimitiveRendererSharedData renderer;
    NEngineInternal::GPPrimitiveRendererSharedData = &renderer;
    renderer.MipmapSizeMultiplier = 1.0f;
    CGraphicDataBank bank;
    CGraphicDataBank::CGraphicHeader header;
    header.Width = 123;
    header.FrameWidth = 0;
    CGraphicInfo dummy(0, &bank, header);
    header.FrameWidth = 1;
    CGraphicInfo first(1, &bank, header);
    header.FrameWidth = 256;
    CGraphicInfo second(2, &bank, header);
    C3DPrimitive2 prim;
    prim.BoundingSphere.Center = C3DVector(1,2,3);
    prim.BoundingSphere.Radius = 1;
    prim.AverageTextureStretch = 1;
    CMatrix3x4 matrix;
    matrix.Set(0,1,0, -1,0,0, 0,0,1, 6,4,-3);
    C3DVector camera(0,0,0);
    C3DMeshMaterial material;
    material.DiffuseMapBankIndex = 1;
    unsigned int n;
    for (n=0;n<256;++n) GTopBitInByteTable[n] = (unsigned char)((n*73+31)&255);
    int bad = 0;
    for (n=0;n<=513;++n) {
        camera = C3DVector(4.25f+(float)n,5,0);
        unsigned long expected = n==0 ? 0 : GTopBitInByteTable[n<2 ? 1 : n>256 ? 255 : n-1];
        bad += check(MipmapFixture::GetAppropriateMipmap(camera,matrix,prim,1,&material,&bank), expected,__LINE__);
    }
    camera = C3DVector(7,9,0);
    bad += check(MipmapFixture::GetAppropriateMipmap(camera,matrix,prim,1,&material,&bank),GTopBitInByteTable[4],__LINE__);
    renderer.MipmapSizeMultiplier = 8;
    prim.AverageTextureStretch = 2;
    material.DiffuseMapBankIndex = 2;
    bad += check(MipmapFixture::GetAppropriateMipmap(camera,matrix,prim,2,&material,&bank),GTopBitInByteTable[96],__LINE__);
    material.DiffuseMapBankIndex = 1;
    bad += check(MipmapFixture::GetAppropriateMipmap(camera,matrix,prim,2,&material,&bank),GTopBitInByteTable[1],__LINE__);
    material.DiffuseMapBankIndex = 0;
    bad += check(MipmapFixture::GetAppropriateMipmap(camera,matrix,prim,1,&material,0),0,__LINE__);
    material.DiffuseMapBankIndex = ~0UL;
    bad += check(MipmapFixture::GetAppropriateMipmap(camera,matrix,prim,1,&material,0),0,__LINE__);
    material.DiffuseMapBankIndex = 1;
    prim.BoundingSphere.Radius = 0;
    camera = C3DVector(5,5,0);
    prim.AverageTextureStretch = DELTA_NOTIONAL_ZERO;
    bad += check(MipmapFixture::GetAppropriateMipmap(camera,matrix,prim,1,0,0),0,__LINE__);
    prim.AverageTextureStretch = -2;
    bad += check(MipmapFixture::GetAppropriateMipmap(camera,matrix,prim,1,0,0),0,__LINE__);
    bad += check(bank.PeekGraphic(3) == 0,1,__LINE__);
    // The original initializer sets zero to 255 and stores floor(log2(n)) for values 1..255.
    GTopBitInByteTable[0] = 255;
    for (n=1;n<256;++n) {
        unsigned int value = n, bit = 0;
        while (value > 1) { value >>= 1; ++bit; }
        GTopBitInByteTable[n] = (unsigned char)bit;
    }
    prim.AverageTextureStretch = 1;
    renderer.MipmapSizeMultiplier = 1;
    material.DiffuseMapBankIndex = 1;
    for (n=0;n<=8;++n) {
        camera = C3DVector(4.25f+(float)(1u<<n),5,0);
        bad += check(MipmapFixture::GetAppropriateMipmap(camera,matrix,prim,1,&material,&bank),n<8?n:7,__LINE__);
    }
    if (bad) return 1;
    puts("00ba9cb0_TEST PASS");
    return 0;
}