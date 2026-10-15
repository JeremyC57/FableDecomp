// CTCEmoteIcon::FrameUpdate() -- count the active timer down (-1 = forever); when it is already 0 and a primitive is shown, release the primitive handle and drop the emote to priority 8
// Leaf: FrameUpdate
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0071AF30, length 94 bytes (vtable slot 9, FrameUpdate; the manifest's vfunc_9).
// Callees: the primitive's virtual slots 3 (+0x0C, with the handle) and 1 (+0x04, Destroy when the count reaches zero).
// Layout: PDB CTCEmoteIcon (48 bytes) over CTCBase (engine/CTCBase.h): ActiveTimer +0xc, EmotePriority +0x20,
// PrimitiveHandle +0x28 (CEnginePrimitiveHandle = vptr + CIVCountedPointer<CEnginePersistentPrimitiveBase> Data at +0x2c;
// the pointee's count at +4, slot 1 Destroy, slot 3 the last-handle notification). The handle release is the
// agent-61 model of 0x004C4160 (CEnginePrimitiveHandle destructor body): `if (Data.IsUnique()) Data->OnLastHandleReleased(this);
// Data = 0;` -- the `operator->` keeps the object in eax and moves it to ecx, `Data = 0` is the release plus a
// second unconditional zero store (`mov [esi+4],0` twice).
// Shape: `this` in edi, the timer read once into eax (`cmp eax,-1 ; je` / `test eax,eax ; je` / `dec eax ; mov [edi+0Ch],eax`);
// the handle is tested with IsValid (+0x2c read) and then released (+0x2c read again by IsUnique), then the priority store.
#include "engine/CTCBase.h"

class CEnginePrimitiveHandle;

class CEnginePersistentPrimitiveBase {
public:
    virtual void v000();
    virtual void Destroy();                                          // slot 1 (+0x04)
    virtual void v002();
    virtual void OnLastHandleReleased(CEnginePrimitiveHandle* handle); // slot 3 (+0x0C)
    long RefCount;                                                   // +0x04
    void Release()
    {
        if (--RefCount == 0)
            Destroy();
    }
};

class CIVCountedPointer {                          // CIVCountedPointer<CEnginePersistentPrimitiveBase>
public:
    CEnginePersistentPrimitiveBase* Object;
    CEnginePersistentPrimitiveBase* operator->() const { return Object; }
    bool IsUnique() const { return Object != 0 && Object->RefCount == 1; }
    __forceinline CIVCountedPointer& operator=(int)
    {
        if (Object) {
            Object->Release();
            Object = 0;
        }
        Object = 0;
        return *this;
    }
};

class CEnginePrimitiveHandle {                     // 8 bytes: vptr (0x0122F598) + Data
public:
    virtual void v000();
    CIVCountedPointer Data;                        // +0x04
    bool IsValid() const { return Data.Object != 0; }
    __forceinline void Release()
    {
        if (Data.IsUnique())
            Data->OnLastHandleReleased(this);
        Data = 0;
    }
};

struct CTCEmoteIcon : CTCBase {
    long          ActiveTimer;                     // +0x0c
    void*         EmoteText;                       // +0x10 CWideString
    long          EmoteIcon;                       // +0x14
    float         EmoteScale;                      // +0x18
    long          EmoteType;                       // +0x1c EEmoteType
    long          EmotePriority;                   // +0x20 EEmotePriority
    unsigned char EmoteAlpha;                      // +0x24
    bool          Replaceable;                     // +0x25
    bool          Enabled;                         // +0x26
    CEnginePrimitiveHandle PrimitiveHandle;        // +0x28
    void FrameUpdate();
};

void CTCEmoteIcon::FrameUpdate()
{
    if (ActiveTimer == -1)
        return;
    if (ActiveTimer != 0) {
        --ActiveTimer;
        return;
    }
    if (PrimitiveHandle.IsValid()) {
        PrimitiveHandle.Release();
        EmotePriority = 8;
    }
}