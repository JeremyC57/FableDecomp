// CGraphicDataBank::FrameEnd() -- unless the graphics are frozen: end the resource bank's frame, the texture manager's frame, then update the available memory
// Grade: DIFFER(91v91) -- (twin of 009fd3a0, 59v59) clamp lands in edx (`and edx,eax`) and the ReduceSizeFrameDelay load + push are hoisted above the AvailableMemory store; retail `and eax,edx ; mov [esi+278h],eax ; mov eax,[esi+26Ch] ; push eax`
// Leaf: ?FrameEnd@CGraphicDataBank@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x009FD660, 91 bytes. Name: Ego_r.pdb ?FrameEnd@CGraphicDataBank@@QAEXXZ, the binding of its
//   caller 0x00B2B820. Callees: CResourceBank::FrameEnd @ 0x009FC430 (the base at +0x164), CTextureManager::FrameEnd @
//   0x00A698B0 (through the CCountedPointer at +0x280), CResourceList::GetUnloadableMemory @ 0x009FC070 (inlined
//   UpdateAvailableMemory, out of line at 0x009FD3A0).
// Layout: see 0x009FD3A0 (same model).
#define CResourceList CResourceListLayout
#include "engine/CResourceList.h"
#undef CResourceList
class CResourceList : public CResourceListLayout {          // retail 0x3C bytes (engine/CResourceList.h)
public:
    unsigned long GetUnloadableMemory(unsigned long minUnusedFrames);   // 0x009FC070
    long GetFreeMemory() const                               // inline: headroom, never negative
    {
        long freeMemory = MaximumMemory - AllocatedMemory;
        return freeMemory <= 0 ? 0 : freeMemory;
    }
};

class CResourceBank {                                        // RTTI base of CGraphicDataBank at +0x164 (356)
public:
    virtual ~CResourceBank();
    CResourceList ResourceList;                              // +0x04
    CResourceList LoadingQueue;                              // +0x40
    void FrameEnd();                                         // 0x009FC430
};

class CTextureManager {
public:
    void FrameEnd();                                         // 0x00A698B0
};

template <class T> class CCountedPointer {
public:
    T* Data;
    T* operator->() const { return Data; }
};

class CBankFileAsync {                                       // RTTI primary base (CBankFile), 0x164 bytes in retail
public:
    virtual ~CBankFileAsync();
    unsigned char _body[0x160];
};

class CGraphicDataBank : public CBankFileAsync, public CResourceBank {
public:
    unsigned char _pad0e4[0x260 - 0x164 - 0x7c];
    long          TextureSizeWarningMaxWidth;    // +0x260
    long          TextureSizeWarningMaxHeight;   // +0x264
    unsigned long UnloadFrameDelay;              // +0x268
    unsigned long ReduceSizeFrameDelay;          // +0x26C
    unsigned long TexturePointersValidForFrames; // +0x270
    long          MinAvailablePreloadMemory;     // +0x274
    long          AvailableMemory;               // +0x278
    bool          FreezeGraphics;                // +0x27C
    CCountedPointer<CTextureManager> TextureManager;   // +0x280
    void FrameEnd();
private:
    void UpdateAvailableMemory();
};

void CGraphicDataBank::UpdateAvailableMemory()
{
    AvailableMemory = ResourceList.GetFreeMemory();
    AvailableMemory += ResourceList.GetUnloadableMemory(ReduceSizeFrameDelay);
}

void CGraphicDataBank::FrameEnd()
{
    if (!FreezeGraphics) {
        CResourceBank::FrameEnd();
        TextureManager->FrameEnd();
        UpdateAvailableMemory();
    }
}
