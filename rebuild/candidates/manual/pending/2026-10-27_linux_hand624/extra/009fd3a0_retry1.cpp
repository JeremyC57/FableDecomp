// CGraphicDataBank::UpdateAvailableMemory() -- AvailableMemory = max(free memory, 0) + the resource list's unloadable memory for ReduceSizeFrameDelay
// Grade: RELOCATION_MATCH (chunk 624 probe, retry1 parked row, not this chunk) -- by-value Max(GetFreeMemory(), 0L) clamp; leaf ?UpdateAvailableMemory@ (PDB ?UpdateAvailableMemory@CGraphicDataBank@@AAEXXZ: make it private at landing and add a test).
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
    unsigned long GetUnloadableMemory(unsigned long minUnusedFrames) const;   // 0x009FC070
    long GetFreeMemory() const { return MaximumMemory - AllocatedMemory; }
};

namespace std {
template <class _Tp> inline const _Tp& (max)(const _Tp& __a, const _Tp& __b) { return __a < __b ? __b : __a; }
}
template <class T> inline T Max(T a, T b) { return a > b ? a : b; }
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
    AvailableMemory = Max(ResourceList.GetFreeMemory(), 0L);
    AvailableMemory += ResourceList.GetUnloadableMemory(ReduceSizeFrameDelay);
}
