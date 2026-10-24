// CASoundManager::CASoundManager(const CSoundManagerInit& init, bool flag) -- sound manager base: copy the init block's settings (and the inverse of its float scale), default the flags, empty the bank vector, then create the resource bank and give its resource list the init block's memory budget with no unload delay
// Leaf: ??0CASoundManager@@QAE@ABUCSoundManagerInit@@_N@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00991910, length 176 bytes (173 + int3 padding), `ret 8`.
// Vtable 0x0129A574 (RTTI "CASoundManager"); only caller the derived manager's constructor @ 0x00A3EF00 (call
// 0x00A3EF14, base-constructing with its own two arguments).
// Callees: CBaseClassNonCopyable::CBaseClassNonCopyable @ 0x0099A2F0, operator new @ 0x00BFEA1A (0x7C bytes),
// CResourceBank::CResourceBank() @ 0x009FC5F0, CCountedPointer<CResourceBank>::Reset(CResourceBank*) @ 0x00992170
// (manifest "CCountedPointer<CIPhysicsFacesSegmentBase const>::Reset", byte twin; as the landed CLipSyncDataBank ctor
// 0x00A42B80), CResourceList::SetMaximumMemory(unsigned long) @ 0x009FC520 (Ego_r ?SetMaximumMemory@CResourceList@@
// QAEXK@Z; the manifest's "CInputProcessGameBase::AddToInterface" is propagated); 1.0f @ 0x0122DED8.
// Names: the sound module is absent from the Ego PDB (as the landed CSoundBankRecord dtor 0x00992510 notes), so the
// class name comes from RTTI and the member / init-block names are descriptive. The manifest's
// "NHeroInformationScreens_CItemDisplay_ctor" is propagated.
// Layout: init block +0x00 -> +0x18, +0x04 -> +0x14, +0x08 -> +0x04, +0x0C -> +0x08, float +0x10 -> +0x0C (and its
// reciprocal -> +0x10), memory budget +0x1C; +0x1C = 1.0f, +0x20 the flag argument, +0x24..+0x26 true, the bank
// CCountedPointer {Data, Info} +0x28, the bank-record vector (STLport, 12 bytes) +0x30, +0x3D false. Ego_r CResourceBank:
// ResourceList +0x04 (CResourceList: MaximumMemory +0x28, UnloadDelay +0x34).
// Shape: member initialisers in declaration order (one zero register, ebx; the shared `true` in al); the new-expression's
// null test feeds Reset; the counted pointer's operator-> re-reads Data for each use.
#include <stddef.h>

void* __cdecl operator new(size_t size);       // 0x00BFEA1A

class CBaseClassNonCopyable {
public:
    CBaseClassNonCopyable();                    // 0x0099A2F0
    virtual ~CBaseClassNonCopyable();
};

class CResourceList {
public:
    void*         __vftable;                    // +0x00
    unsigned char Head[0x1c];                   // +0x04 CResource
    unsigned long ResourceCount;                // +0x20
    long          AllocatedMemory;              // +0x24
    long          MaximumMemory;                // +0x28
    unsigned long CurrentFrame;                 // +0x2c
    unsigned long DebugStatsFrame;              // +0x30
    unsigned long UnloadDelay;                  // +0x34
    unsigned long UnloadedThisFrame;            // +0x38
    void SetMaximumMemory(unsigned long maximum);   // 0x009FC520
};

class CResourceBank {
public:
    void*         __vftable;                    // +0x00
    CResourceList ResourceList;                 // +0x04
    CResourceList LoadingQueue;                 // +0x40
    CResourceBank();                            // 0x009FC5F0
};

class CCPPointerInfo;
template <class T> class CCountedPointer {
public:
    T*              Data;                       // +0x00
    CCPPointerInfo* Info;                       // +0x04
    CCountedPointer() : Data(0), Info(0) { }
    void Reset(T* item);                        // 0x00992170
    T* operator->() const { return Data; }
};

template <class T> class CSTLportVector {      // STLport vector<T>: _Vector_base layout, 12 bytes
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    CSTLportVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
};

struct CSoundManagerInit {                      // descriptive: the derived manager's settings block
    long          Setting00;                    // +0x00
    long          Setting04;                    // +0x04
    long          Setting08;                    // +0x08
    long          Setting0C;                    // +0x0c
    float         Scale;                        // +0x10
    long          Setting14;                    // +0x14
    long          Setting18;                    // +0x18
    unsigned long ResourceMemory;               // +0x1c
};

struct CSoundBankEntry;

class CASoundManager : public CBaseClassNonCopyable {
public:
    long          Setting08;                    // +0x04
    long          Setting0C;                    // +0x08
    float         Scale;                        // +0x0c
    float         InverseScale;                 // +0x10
    long          Setting04;                    // +0x14
    long          Setting00;                    // +0x18
    float         Volume;                       // +0x1c
    bool          Flag;                         // +0x20
    bool          Unknown21[3];                 // +0x21 (not set by the constructor)
    bool          Enabled24;                    // +0x24
    bool          Enabled25;                    // +0x25
    bool          Enabled26;                    // +0x26
    CCountedPointer<CResourceBank> ResourceBank;   // +0x28
    CSTLportVector<CSoundBankEntry> Banks;      // +0x30
    bool          Unknown3C;                    // +0x3c
    bool          Busy;                         // +0x3d
    CASoundManager(const CSoundManagerInit& init, bool flag);
    virtual ~CASoundManager();
};

CASoundManager::CASoundManager(const CSoundManagerInit& init, bool flag)
    : Setting08(init.Setting08)
    , Setting0C(init.Setting0C)
    , Scale(init.Scale)
    , InverseScale(1.0f / init.Scale)
    , Setting04(init.Setting04)
    , Setting00(init.Setting00)
    , Volume(1.0f)
    , Flag(flag)
    , Enabled24(true)
    , Enabled25(true)
    , Enabled26(true)
    , Busy(false)
{
    ResourceBank.Reset(new CResourceBank);
    ResourceBank->ResourceList.SetMaximumMemory(init.ResourceMemory);
    ResourceBank->ResourceList.UnloadDelay = 0;
}