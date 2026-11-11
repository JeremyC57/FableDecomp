// NUISystem::CIconText::AddIcon(long graphic, long charIndex, EIconFlags flags) -- build an embedded icon; a set graphic takes its frame size and starts loading/fetching its frame (else the size is zero), then append it to Icons
// Grade: RELOCATION_MATCH(682v682) on PARTLY shared headers -- parked for header debt, not landable yet: CIconText (engine/NUISystem_CIconText.h, retail layout already), std::vector push_back/_M_insert_overflow (stlport/fable_containers.h, new FABLE_RETAIL_STLP_VECTOR_PUSHBACK_NOINLINE), NUISystem::CManager::GetGraphicsBank and CCountedPointer are shared; the graphics-bank stack (CResource/CResourceList TouchResource, CGraphicFrame/CGraphicFrameData refcounting, CManagedTexture touch, CGraphicInfoFrame::CFrameData, the inline CGraphicInfoFrame::GetFrame, CGraphicInfo frame size, CGraphicDataBank::PeekGraphicInfoFrame) is still local because the shared retail forms (FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS) have no inline bodies for GetFrame/TouchResource/the refcount paths and keep the members private; next: add those inline bodies as opt-ins (FableWin.pdb names: ?GetFrame@CGraphicInfoFrame@@QBE_NAAVCGraphicFrame@@KK@Z, ?TouchResource@CResource@@QAEXXZ, ?ListInsert@CResource@@AAEXPAV1@0@Z, ?PeekGraphicInfoFrame@CGraphicDataBank@@QBEPBVCGraphicInfoFrame@@KK@Z) -- the landed 0x0041C260 / 0x0041AC20 use the same local models
// Leaf: ?AddIcon@CIconText@NUISystem@@UAEXJJW4EIconFlags@2@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00542780, 682 bytes, `ret 0xc`, thiscall, virtual (Ego_r.pdb
//   ?AddIcon@CIconText@NUISystem@@UAEXJJW4EIconFlags@2@@Z; EIconFlags is NUISystem::EIconFlags).
// Callees: NUISystem::CManager::GetInstance 0x0041E5F2 (GetGraphicsBank inline), CGraphicInfoFrame::BeginLoading 0x009FE620, CResource::ListRemove 0x009FBFB0 /
//   ListInsert 0x009FBF90, CFrameData::BeginLoading 0x009FDE80, CFrameData::CFrameData 0x009FE380, InternalGetFrameForcedLoading
//   0x009FF6B0, CGraphicFrameData::Uninitialise 0x009FCBA0, operator new 0x00BFEA1A, operator delete 0x00BFE9BC,
//   vector<CEmbeddedIcon>::push_back 0x00543C90.
// Lever: retail's TU holds the out-of-line STLport push_back (0x00543C90) and its _M_insert_overflow; with both bodies visible
//   (auto_inline off) c2 knows the icon's address is not kept, which fixes the FrameWidth/FrameHeight load pair (edx, eax) before
//   the fild. push_back alone was not enough; the whole chain must be visible.
// Header debt: see line 2. The (graphic, charIndex, flags) CEmbeddedIcon constructor is an inline opt-in (FABLE_RETAIL_CICONTEXT_EMBEDDEDICON_ARGS_CTOR);
//   the PDBs only show the out-of-line default and copy constructors (the default-ctor + assignment form gives a different prologue).
#include "fable_compiler.h"
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CICONTEXT_VSLOTS
#define FABLE_RETAIL_CMANAGER
#define FABLE_RETAIL_CICONTEXT_EMBEDDEDICON_ARGS_CTOR
#define FABLE_RETAIL_STLP_VECTOR_PUSHBACK_NOINLINE
#define FABLE_STLP_OUTOFLINE_UNINIT_COPY
#define FABLE_STLP_OUTOFLINE_UNINIT_FILL_N
#include "engine/NUISystem_CIconText.h"
#include "engine/CCountedPointer.h"
#include "engine/NUISystem_CManager.h"
void __cdecl operator delete(void* block);             // 0x00BFE9BC

struct CResourceList;
struct CResource {                                     // Ego_r.pdb, 0x1C bytes
    virtual void v00();
    virtual void Release();                            // slot 1: the CIVCountedPointer release on the last reference
    unsigned long  RefCount;                           // +0x04 (CIVCountedPointer count)
    CResourceList* ResourceList;                       // +0x08
    CResource*     PrevResource;                       // +0x0c
    CResource*     NextResource;                       // +0x10
    unsigned long  ResourceSize;                       // +0x14
    unsigned long  LastUsedFrame;                      // +0x18
    void ListRemove();                                 // 0x009FBFB0
    void ListInsert(CResource* prev, CResource* next); // 0x009FBF90
    FABLE_FORCEINLINE void Touch();
};
struct CResourceList {                                 // Ego_r.pdb, 0x3C bytes
    unsigned long _pad00;
    CResource     Head;                                // +0x04
    unsigned long ResourceCount;                       // +0x20
    long          AllocatedMemory;                     // +0x24
    long          MaximumMemory;                       // +0x28
    unsigned long CurrentFrame;                        // +0x2c
};
FABLE_FORCEINLINE void CResource::Touch()
{
    if (ResourceList && LastUsedFrame != ResourceList->CurrentFrame) {
        ListRemove();
        ListInsert(ResourceList->Head.PrevResource, &ResourceList->Head);
        LastUsedFrame = ResourceList->CurrentFrame;
    }
}

class CTexture;
struct CGraphicFrameData {                             // Ego_r.pdb, 27 bytes
    unsigned long   ReferenceCount;                    // +0x00
    void*           Data;                              // +0x04
    CCountedPointer<CTexture> PTexture;                // +0x08
    unsigned short  TLX, TLY, Width, Height, Pitch;    // +0x10
    unsigned char   Alpha : 1;                         // +0x1a
    unsigned char   BooleanAlpha : 1;
    unsigned char   UsesSurface : 1;
    unsigned char   SectionOfTPage : 1;
    unsigned char   IsInitialised : 1;
    unsigned char   ScreenGraphic : 1;
    void Uninitialise();                               // 0x009FCBA0
    FABLE_FORCEINLINE ~CGraphicFrameData() { if (IsInitialised) Uninitialise(); }
    FABLE_FORCEINLINE void DecRef()
    {
        if (--ReferenceCount == 0) {
            this->~CGraphicFrameData();
            operator delete(this);
        }
    }
};
struct CGraphicFrame {                                 // intrusive pointer to CGraphicFrameData
    CGraphicFrameData* FrameData;
    FABLE_FORCEINLINE CGraphicFrame() : FrameData(0) { }
    FABLE_FORCEINLINE ~CGraphicFrame() { if (FrameData) FrameData->DecRef(); }
    FABLE_FORCEINLINE CGraphicFrame& operator=(const CGraphicFrame& other)
    {
        if (FrameData)
            FrameData->DecRef();
        FrameData = other.FrameData;
        if (FrameData)
            ++FrameData->ReferenceCount;
        return *this;
    }
};
struct CManagedTexture {                               // Ego_r.pdb, 8 bytes
    unsigned long _pad00;
    CResource*    Data;                                // +0x04 CIVCountedPointer<CManagedTextureData>
    FABLE_FORCEINLINE void Touch() { if (Data) Data->Touch(); }
};
struct CGraphicInfoFrame;
struct CFrameData : CResource {                        // Ego_r.pdb, 0x44 bytes
    CGraphicFrame   Frame;                             // +0x1c
    CManagedTexture ManagedTexture;                    // +0x20
    CManagedTexture LoadingManagedTexture;             // +0x28
    void*           FileData[2];                       // +0x30 CCountedPointer<CBankFileAsyncData>
    signed char     CurrentMipmapLevel;                // +0x38
    signed char     LoadingMipmapLevel;                // +0x39
    signed char     TouchedMipmapLevel;                // +0x3a
    unsigned long   TouchedFrame;                      // +0x3c
    CGraphicInfoFrame* GraphicInfoFrame;               // +0x40
    CFrameData(CGraphicInfoFrame* frame);              // 0x009FE380
    void BeginLoading(unsigned long mipmapLevel, unsigned long flags);   // 0x009FDE80
    FABLE_FORCEINLINE void DecRef() { if (--RefCount == 0) Release(); }
};
struct CGraphicInfoFrame;
struct CGraphicInfo {                                  // Ego_r.pdb, 0x2c bytes
    unsigned short     Width;                          // +0x00 CGraphicHeader
    unsigned short     Height;                         // +0x02
    unsigned short     Depth;                          // +0x04
    unsigned short     FrameWidth;                     // +0x06
    unsigned short     FrameHeight;                    // +0x08
    unsigned char      _pad0a[0x20 - 0x0a];
    struct CGraphicDataBank* Bank;                     // +0x20
    long               Index;                          // +0x24
    CGraphicInfoFrame* Frames;                         // +0x28
};
struct CGraphicInfoArray {                             // CArray<CGraphicInfo> (12 bytes retail)
    CGraphicInfo* _M_start;
    CGraphicInfo* _M_finish;
    CGraphicInfo* _M_end_of_storage;
    unsigned long size() const { return (unsigned long)(_M_finish - _M_start); }
};
// Layout: retail CGraphicDataBank has its current frame at +0x194 and GraphicList at +0x1e0 (PDB 0x254; the retail
// containers before it are smaller).
struct CGraphicDataBank {
    unsigned char _pad00[0x194];
    unsigned long CurrentFrame;                        // +0x194
    unsigned char _pad198[0x1e0 - 0x198];
    CGraphicInfoArray GraphicList;                     // +0x1e0
    FABLE_FORCEINLINE CGraphicInfo* GetGraphicInfo(long index) { return (unsigned long)index < GraphicList.size() ? &GraphicList._M_start[index] : 0; }
    FABLE_FORCEINLINE CGraphicInfoFrame* GetGraphicInfoFrame(long index) { return GetGraphicInfo(index) ? GetGraphicInfo(index)->Frames : 0; }
};
struct CFrameDataPointer {                             // CIVCountedPointer<CFrameData>
    CFrameData* P;
    FABLE_FORCEINLINE CFrameDataPointer& operator=(CFrameData* p)
    {
        if (P) {
            P->DecRef();
            P = 0;
        }
        P = p;
        return *this;
    }
    CFrameData* operator->() const { return P; }
    operator bool() const { return P != 0; }
};
struct CGraphicInfoFrame {                             // Ego_r.pdb, 16 bytes
    unsigned long     _pad00;
    unsigned short    FrameNum;                        // +0x04
    signed char       SmallMipmapLevel;                // +0x06
    CFrameDataPointer Data;                            // +0x08
    CGraphicInfo*     GraphicInfo;                     // +0x0c
    bool InternalGetFrameForcedLoading(unsigned long flags);   // 0x009FF6B0
    void BeginLoading(unsigned long mipmapLevel, unsigned long flags);   // 0x009FE620
    FABLE_FORCEINLINE bool GetFrame(CGraphicFrame& out, unsigned long mipmapLevel, unsigned long flags);
};
FABLE_FORCEINLINE bool CGraphicInfoFrame::GetFrame(CGraphicFrame& out, unsigned long mipmapLevel, unsigned long flags)
{
    if (Data) {
        if (Data->Frame.FrameData) {
            if (!(flags & 8)) {
                if (Data->TouchedFrame != GraphicInfo->Bank->CurrentFrame || mipmapLevel <= (unsigned long)Data->TouchedMipmapLevel) {
                    Data->TouchedFrame = GraphicInfo->Bank->CurrentFrame;
                    Data->ManagedTexture.Touch();
                    if (mipmapLevel <= (unsigned long)Data->CurrentMipmapLevel)
                        Data->Touch();
                    if ((unsigned long)Data->TouchedMipmapLevel > mipmapLevel)
                        Data->TouchedMipmapLevel = (signed char)mipmapLevel;
                }
            }
            if (mipmapLevel < (unsigned long)Data->CurrentMipmapLevel) {
                Data->BeginLoading(mipmapLevel, flags);
                if (!(flags & 0x10) || !Data->FileData[0]) {
                    out = Data->Frame;
                    return true;
                }
            } else {
                out = Data->Frame;
                return true;
            }
        }
        Data->BeginLoading(mipmapLevel, flags);
    } else {
        if (flags & 4)
            return false;
        Data = new CFrameData(this);
        Data->BeginLoading(mipmapLevel, flags);
    }
    if (!(flags & 1) || !InternalGetFrameForcedLoading(flags))
        return false;
    out = Data->Frame;
    return true;
}
namespace NUISystem {
void CIconText::AddIcon(long graphic, long charIndex, EIconFlags flags)
{
    CEmbeddedIcon icon(graphic, charIndex, flags);
    CCountedPointer<CGraphicDataBank> bank = CManager::GetInstance()->GetGraphicsBank();
    if (graphic > 0) {
        CGraphicInfoFrame* frame = bank->GetGraphicInfoFrame(graphic);
        CGraphicInfo* info = frame->GraphicInfo;
        icon.Size.X = (float)info->FrameWidth;
        icon.Size.Y = (float)info->FrameHeight;
        frame->BeginLoading(0, 0x10);
        CGraphicFrame graphicFrame;
        frame->GetFrame(graphicFrame, 0, 0x11);
    } else {
        icon.Size.X = 0.0f;
        icon.Size.Y = 0.0f;
    }
    Icons.push_back(icon);
}
}
