// Behaviour test for CTCEmoteIcon::~CTCEmoteIcon @ 0x0071b120 (implicit): the primitive handle is cleared first (the last
// holder notifies the primitive, then the reference is dropped and the primitive deleted at zero), then the emote text, then
// the CTCBase part.
#include <stdio.h>
static char g_log[64];
static void logc(char c) { int n = 0; while (g_log[n]) ++n; g_log[n] = c; g_log[n + 1] = 0; }
class CEnginePrimitiveHandle;
class CPrimitive { public:
    unsigned int IVRefCount;
    int notified, deleted;
    void NotifyReleased(const CEnginePrimitiveHandle&) { notified++; logc('n'); }
    void Delete() const { const_cast<CPrimitive*>(this)->deleted++; logc('x'); }
};
class CEnginePrimitiveHandle { public:
    CPrimitive* Object;
    unsigned int GetReferenceCount() const { return Object ? Object->IVRefCount : 0; }
    void Release() { CPrimitive* o = Object; if (o) { if (--o->IVRefCount == 0) o->Delete(); Object = 0; } }
    void Clear() { if (GetReferenceCount() == 1) Object->NotifyReleased(*this); Release(); }
    virtual ~CEnginePrimitiveHandle() { Clear(); Release(); logc('h'); }
};
class CWideString { public: ~CWideString() { logc('w'); } };
class CTCBase { public: virtual ~CTCBase() { logc('T'); } };
class CTCEmoteIcon : public CTCBase { public:
    long ActiveTimer;
    CWideString EmoteText;
    long EmoteIcon;
    float EmoteScale;
    CEnginePrimitiveHandle PrimitiveHandle;
};
int main()
{
    CPrimitive prim; prim.IVRefCount = 1; prim.notified = prim.deleted = 0;
    CTCEmoteIcon* icon = new CTCEmoteIcon; icon->PrimitiveHandle.Object = &prim;
    CTCBase* b = icon; delete b;
    const char* want = "nxhwT";
    for (int i = 0; want[i] || g_log[i]; ++i) if (want[i] != g_log[i]) { printf("0071b120_TEST FAIL\n"); return 1; }
    g_log[0] = 0;
    prim.IVRefCount = 2; prim.notified = prim.deleted = 0;
    icon = new CTCEmoteIcon; icon->PrimitiveHandle.Object = &prim;
    b = icon; delete b;
    if (prim.notified != 0 || prim.deleted != 0 || prim.IVRefCount != 1) { printf("0071b120_TEST FAIL\n"); return 1; }
    g_log[0] = 0;
    icon = new CTCEmoteIcon; icon->PrimitiveHandle.Object = 0;
    b = icon; delete b;
    if (g_log[0] != 'h') { printf("0071b120_TEST FAIL\n"); return 1; }
    printf("0071b120_TEST PASS\n");
    return 0;
}