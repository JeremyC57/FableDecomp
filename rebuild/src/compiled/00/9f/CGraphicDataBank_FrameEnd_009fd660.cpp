// CGraphicDataBank::FrameEnd() -- unless the graphics are frozen: end the resource bank's frame, the texture manager's frame, then update the available memory
// Leaf: ?FrameEnd@CGraphicDataBank@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x009FD660, 91 bytes. Name: Ego_r.pdb ?FrameEnd@CGraphicDataBank@@QAEXXZ, the binding of its
//   caller 0x00B2B820. Callees: CResourceBank::FrameEnd @ 0x009FC430 (the base at +0x164), CTextureManager::FrameEnd @
//   0x00A698B0 (through the CCountedPointer at +0x280), CResourceList::GetUnloadableMemory @ 0x009FC070 (inlined
//   UpdateAvailableMemory, out of line at 0x009FD3A0).
// Layout: CResourceBank base at +0x164 (its ResourceList at +0x168: AllocatedMemory +0x18C, MaximumMemory +0x190),
//   ReduceSizeFrameDelay +0x26C, AvailableMemory +0x278, FreezeGraphics +0x27C, TextureManager +0x280 (the 0x009FD3A0 model).
// Shape: UpdateAvailableMemory (Ego_r.pdb ?UpdateAvailableMemory@CGraphicDataBank@@AAEXXZ) inlined:
//   `AvailableMemory = Max(ResourceList.GetFreeMemory(), 0L); AvailableMemory += ResourceList.GetUnloadableMemory(ReduceSizeFrameDelay);`
//   The clamp through a BY-VALUE `Max(T a, T b) { return a > b ? a : b; }` leaves the result in the value register
//   (`setle dl ; dec edx ; and eax,edx`, then the store before the ReduceSizeFrameDelay load); a named local with a
//   ternary / if-assign / explicit mask, and std::max (by reference: the 0 goes to memory) all give `and edx,eax` and
//   hoist the load (the earlier parked residue). GetFreeMemory (PDB QBEJXZ) is the raw difference; GetUnloadableMemory
//   is const (PDB QBEKK).
#define CResourceList CResourceListLayout
#include "engine/CResourceList.h"
#undef CResourceList
class CResourceList : public CResourceListLayout {          // retail 0x3C bytes (engine/CResourceList.h)
public:
    unsigned long GetUnloadableMemory(unsigned long minUnusedFrames) const;   // 0x009FC070
    long GetFreeMemory() const { return MaximumMemory - AllocatedMemory; }
};

template <class T> inline T Max(T a, T b) { return a > b ? a : b; }   // inline by-value maximum (fully inlined: no symbol to prove its name)
class CResourceBank {                                        // RTTI base of CGraphicDataBank at +0x164 (356)
public:
    virtual ~CResourceBank();
    CResourceList ResourceList;                              // +0x04
    CResourceList LoadingQueue;                              // +0x40
    void FrameEnd();                                         // 0x009FC430
};

#define CTextureManager CTextureManagerLayout
#include "engine/CTextureManager.h"
#undef CTextureManager
class CTextureManager : public CTextureManagerLayout {      // engine/CTextureManager.h
public:
    void FrameEnd();                                         // 0x00A698B0
};

#include "engine/CCountedPointer.h"

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

void CGraphicDataBank::FrameEnd()
{
    if (!FreezeGraphics) {
        CResourceBank::FrameEnd();
        TextureManager->FrameEnd();
        UpdateAvailableMemory();
    }
}