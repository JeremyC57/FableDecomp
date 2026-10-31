// CGraphicDataBank::UpdateAvailableMemory() -- AvailableMemory = max(free memory, 0) + the resource list's unloadable memory after ReduceSizeFrameDelay frames
// Grade: RELOCATION_MATCH (chunk 626 extra; chunk 624 retry1 probe) -- private as the PDB has it.
// Leaf: ?UpdateAvailableMemory@CGraphicDataBank@@AAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x009FD3A0, 59 bytes (+5 int3). Name: Ego_r.pdb ?UpdateAvailableMemory@CGraphicDataBank@@AAEXXZ
//   (donor 0x00B56B40; private -> AAE); no code xrefs (inlined into CGraphicDataBank::FrameEnd 0x009FD660, out-of-line copy kept).
// Callee: CResourceList::GetUnloadableMemory(unsigned long) const @ 0x009FC070 (PDB ?GetUnloadableMemory@CResourceList@@QBEKK@Z).
// Shape: the by-value Max(x, 0L) clamp gives "xor edx,edx ; test ; setle dl ; dec edx ; and eax,edx" with the store kept before
//   the ReduceSizeFrameDelay load; GetFreeMemory (MaximumMemory - AllocatedMemory, +0x28 - +0x24 of the list at +0x168) is an
//   inline accessor (no PDB body). The second statement is a read-modify-write "add [esi+278h],eax".
// Layout: CResourceList from engine/CResourceList.h via the #define ...Layout include pattern (0x3C bytes); CResourceBank sits at
//   +0x164 (RTTI second base of CGraphicDataBank), so ResourceList is at +0x168. No engine/CGraphicDataBank.h exists.
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