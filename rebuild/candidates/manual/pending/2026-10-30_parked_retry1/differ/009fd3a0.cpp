// CGraphicDataBank::UpdateAvailableMemory() -- available memory = the resource list's free headroom (clamped at 0) plus what it could unload after ReduceSizeFrameDelay frames
// Grade: DIFFER(59v59) retry1 -- 3 more clamp spellings; residue: select destination register + store/push order
// Leaf: ?UpdateAvailableMemory@CGraphicDataBank@@AAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x009FD3A0, 59 bytes. Name: Ego_r.pdb ?UpdateAvailableMemory@CGraphicDataBank@@AAEXXZ
//   (private); the same body is inlined at the end of FrameEnd 0x009FD660 (bound to ?FrameEnd@CGraphicDataBank@@QAEXXZ by
//   its caller 0x00B2B820). Callee: CResourceList::GetUnloadableMemory(ulong) @ 0x009FC070 on ResourceList (+0x168).
// Layout: RTTI CGraphicDataBank : CBankFileAsync@0, CResourceBank@0x164 (ResourceList at bank+4 = +0x168, AllocatedMemory
//   +0x18C, MaximumMemory +0x190); members after the bases from the landed Initialise 0x009FD4E0 (PDB offsets - 0x7C):
//   ReduceSizeFrameDelay +0x26C, AvailableMemory +0x278, FreezeGraphics +0x27C, TextureManager +0x280.
// Shape: an inline CResourceList getter (ecx = &ResourceList computed first) returning `x > 0 ? x : 0` is retail's `setle ; dec ; and` clamp; the store then the `add [mem],eax` of the call result.
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
