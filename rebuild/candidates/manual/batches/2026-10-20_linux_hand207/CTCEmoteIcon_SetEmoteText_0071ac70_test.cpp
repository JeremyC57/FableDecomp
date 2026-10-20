#ifndef _MSC_VER
#define __forceinline inline
#define __fastcall
#define __stdcall
#define __cdecl
#endif
#include <cstdio>
#include <cstring>
class CEnginePrimitiveHandle;

class CWideString {
public:
    void* PStringData;
    bool operator!=(const wchar_t* text) const;     // 0x0099B150
    CWideString& operator=(const CWideString& other);       // 0x0099B7D0
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

// ---- test stubs for the retail callees ----
static int g_destroyed, g_lastReleased; static float g_timer = -1.0f;
static const wchar_t g_empty[1] = { 0 };
bool CWideString::operator!=(const wchar_t* text) const { const wchar_t* p = (const wchar_t*)PStringData; return !(p[0] == text[0]); }
CWideString& CWideString::operator=(const CWideString& other) { PStringData = other.PStringData; return *this; }
const CWideString& CWideString::operator=(const wchar_t*) { PStringData = (void*)g_empty; return *this; }
struct CTestPrim : CEnginePersistentPrimitiveBase {
    virtual void Destroy() { ++g_destroyed; }
    virtual void OnLastHandleReleased(CEnginePrimitiveHandle*) { ++g_lastReleased; }
};
void CEnginePersistentPrimitiveBase::v000() {} void CEnginePersistentPrimitiveBase::Destroy() {}
void CEnginePersistentPrimitiveBase::v002() {}
void CEnginePersistentPrimitiveBase::OnLastHandleReleased(CEnginePrimitiveHandle*) {}
void CEnginePrimitiveHandle::v000() {}
void CTCEmoteIcon::v00() {}
void CTCEmoteIcon::SetActiveTimer(float time) { g_timer = time; }

int main()
{
    int ok = 1;
    static const wchar_t hello[3] = { 'h', 'i', 0 };
    CWideString text; text.PStringData = (void*)hello;
    CTestPrim prim; prim.RefCount = 1;
    CTCEmoteIcon icon;
    icon.EmoteText.PStringData = (void*)g_empty; icon.EmotePriority = 5; icon.EmoteType = 3; icon.EmoteScale = 0.0f;
    icon.PrimitiveHandle.Data.Object = &prim;
    // higher priority value than the current one: ignored
    icon.SetEmoteText(6, text, 2.0f, 1.5f);
    if (icon.EmotePriority != 5 || g_timer != -1.0f || icon.EmoteText.PStringData != (void*)g_empty) ok = 0;
    // accepted: text, scale, type reset, primitive dropped (last handle: notified and destroyed)
    icon.SetEmoteText(4, text, 2.0f, 1.5f);
    if (icon.EmotePriority != 4 || icon.EmoteText.PStringData != (void*)hello || icon.EmoteScale != 1.5f) ok = 0;
    if (icon.EmoteType != 0 || g_timer != 2.0f || icon.PrimitiveHandle.Data.Object != 0) ok = 0;
    if (g_lastReleased != 1 || g_destroyed != 1 || prim.RefCount != 0) ok = 0;
    // shared primitive: released without notification
    prim.RefCount = 2; icon.PrimitiveHandle.Data.Object = &prim;
    icon.SetEmoteText(4, text, 3.0f, 1.0f);
    if (g_lastReleased != 1 || g_destroyed != 1 || prim.RefCount != 1 || icon.PrimitiveHandle.Data.Object != 0) ok = 0;
    // empty text: idle priority, empty string, timer 0
    CWideString empty; empty.PStringData = (void*)g_empty;
    icon.SetEmoteText(4, empty, 3.0f, 1.0f);
    if (icon.EmotePriority != 8 || icon.EmoteText.PStringData != (void*)g_empty || g_timer != 0.0f) ok = 0;
    if (!ok) { std::printf("0071ac70_TEST FAIL\n"); return 1; }
    std::printf("0071ac70_TEST PASS\n");
    return 0;
}
