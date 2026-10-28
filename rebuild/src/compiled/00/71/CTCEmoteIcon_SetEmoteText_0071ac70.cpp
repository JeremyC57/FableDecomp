// CTCEmoteIcon::SetEmoteText(EEmotePriority priority, const CWideString& text, float activeTime, float scale) -- show a text emote unless a higher-priority emote is up; an empty text resets the icon to the idle priority
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0071AC70, length 162 bytes (+ int3 padding). __fastcall, `ret 10h`.
//         Ego_r.pdb: `?SetEmoteText@CTCEmoteIcon@@QAEXW4EEmotePriority@1@ABVCWideString@@MM@Z`.
// Callees: CWideString::operator!=(const wchar_t*) const @ 0x0099B150, CWideString::operator=(const CWideString&) @ 0x0099B7D0,
//          CWideString::operator=(const wchar_t*) @ 0x0099B800 (empty literal @ 0x0122D70C),
//          CTCEmoteIcon::SetActiveTimer(float) @ 0x0071AA50; the primitive's virtual slots 3 (OnLastHandleReleased) and 1.
// Shape: a lower or equal priority value wins (`cmp ; jg` to the shared exit); the text branch drops the primitive through
//        the inline CEnginePrimitiveHandle::Clear (uniqueness test, release, two zero stores) when one is attached; every
//        zero comes from one ebx register; EmoteType = 0 is written before the scale (the other order loads the handle
//        pointer once into eax instead of the retail `cmp [esi+2Ch],ebx` + reload). The empty branch pushes 0.0f as 0.
// Layout: Ego_r.pdb CTCEmoteIcon (matches retail): EmoteText +0x10, EmoteScale +0x18, EmoteType +0x1C, EmotePriority +0x20,
//         PrimitiveHandle +0x28 (CBaseClass vptr + counted Data at +0x2C).
#include <stddef.h>                                 // wchar_t (unsigned short under the era defaults)
class CEnginePrimitiveHandle;

class CWideString {
public:
    void* PStringData;
    bool operator!=(const wchar_t* text) const;     // 0x0099B150
    const CWideString& operator=(const CWideString& other);       // 0x0099B7D0
    const CWideString& operator=(const wchar_t* text);      // 0x0099B800
};

class CEnginePersistentPrimitiveBase {
public:
    virtual void v000();
    virtual void Destroy();                                          // slot 1 (+0x04)
    virtual void v002();
    virtual void OnLastHandleReleased(CEnginePrimitiveHandle* handle); // slot 3 (+0x0C)
    long RefCount;                                                   // +0x04
    void Release() { if (--RefCount == 0) Destroy(); }
};
class CIVCountedPointer {                           // CIVCountedPointer<CEnginePersistentPrimitiveBase>
public:
    CEnginePersistentPrimitiveBase* Object;
    CEnginePersistentPrimitiveBase* operator->() const { return Object; }
    bool IsUnique() const { return Object != 0 && Object->RefCount == 1; }
    __forceinline void ReleaseRef() { if (Object) { Object->Release(); Object = 0; } }
    __forceinline CIVCountedPointer& operator=(int) { ReleaseRef(); Object = 0; return *this; }
};
class CEnginePrimitiveHandle {                      // vtable 0x0122F598 (CBaseClass), 8 bytes
public:
    virtual void v000();
    CIVCountedPointer Data;                         // +0x04
    bool IsValid() const { return Data.Object != 0; }
    __forceinline void Clear()
    {
        if (Data.IsUnique())
            Data->OnLastHandleReleased(this);
        Data = 0;
    }
};

class CTCEmoteIcon {
public:
    virtual void v00();
    unsigned char _pad04[0x10 - 0x04];              // CTCBase, ActiveTimer +0x0C
    CWideString EmoteText;                          // +0x10
    long EmoteIcon;                                 // +0x14
    float EmoteScale;                               // +0x18
    long EmoteType;                                 // +0x1C EEmoteType
    long EmotePriority;                             // +0x20 EEmotePriority
    unsigned char EmoteAlpha;                       // +0x24
    bool Replaceable;                               // +0x25
    bool Enabled;                                   // +0x26
    CEnginePrimitiveHandle PrimitiveHandle;         // +0x28
    void SetActiveTimer(float time);                // 0x0071AA50
    void SetEmoteText(long priority, const CWideString& text, float activeTime, float scale);
};

void CTCEmoteIcon::SetEmoteText(long priority, const CWideString& text, float activeTime, float scale)
{
    if (priority <= EmotePriority) {
        if (text != L"") {
            EmotePriority = priority;
            EmoteText = text;
            EmoteType = 0;
            EmoteScale = scale;
            if (PrimitiveHandle.IsValid())
                PrimitiveHandle.Clear();
            SetActiveTimer(activeTime);
        } else {
            EmotePriority = 8;
            EmoteText = L"";
            SetActiveTimer(0.0f);
        }
    }
}