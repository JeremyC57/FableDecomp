#include <cstdio>
class CEnginePrimitiveHandle;
static int g_destroyed = 0, g_lastHandle = 0;
class CEnginePersistentPrimitiveBase {
public:
    virtual void v000() { }
    virtual void Destroy() { ++g_destroyed; }
    virtual void v002() { }
    virtual void OnLastHandleReleased(CEnginePrimitiveHandle*) { ++g_lastHandle; }
    long RefCount;
    void Release() { if (--RefCount == 0) Destroy(); }
};
class CIVCountedPointer {
public:
    CEnginePersistentPrimitiveBase* Object;
    CEnginePersistentPrimitiveBase* operator->() const { return Object; }
    bool IsUnique() const { return Object != 0 && Object->RefCount == 1; }
    CIVCountedPointer& operator=(int) { if (Object) { Object->Release(); Object = 0; } Object = 0; return *this; }
};
class CEnginePrimitiveHandle {
public:
    virtual void v000() { }
    CIVCountedPointer Data;
    bool IsValid() const { return Data.Object != 0; }
    void Release() { if (Data.IsUnique()) Data->OnLastHandleReleased(this); Data = 0; }
};
struct CTCEmoteIconBase {
    void* vptrSlot; void* ParentThing; bool m_flags[4];
    long ActiveTimer; void* EmoteText; long EmoteIcon; float EmoteScale; long EmoteType; long EmotePriority;
    unsigned char EmoteAlpha; bool Replaceable; bool Enabled;
    CEnginePrimitiveHandle PrimitiveHandle;
};
struct CTCEmoteIcon : CTCEmoteIconBase {
    void FrameUpdate()
    {
        if (ActiveTimer == -1) return;
        if (ActiveTimer != 0) { --ActiveTimer; return; }
        if (PrimitiveHandle.IsValid()) { PrimitiveHandle.Release(); EmotePriority = 8; }
    }
};
int main()
{
    CEnginePersistentPrimitiveBase p; p.RefCount = 1;
    CTCEmoteIcon e; e.PrimitiveHandle.Data.Object = &p; e.EmotePriority = 2; e.ActiveTimer = -1;
    e.FrameUpdate();
    bool forever = e.ActiveTimer == -1 && e.PrimitiveHandle.Data.Object == &p;
    e.ActiveTimer = 2; e.FrameUpdate(); e.FrameUpdate();
    bool counted = e.ActiveTimer == 0 && e.PrimitiveHandle.Data.Object == &p && e.EmotePriority == 2;
    e.FrameUpdate();
    bool expired = e.PrimitiveHandle.Data.Object == 0 && g_destroyed == 1 && g_lastHandle == 1 && e.EmotePriority == 8;
    e.EmotePriority = 3; e.FrameUpdate();
    bool idle = e.EmotePriority == 3;
    if (!forever || !counted || !expired || !idle) { std::printf("0071af30_TEST FAIL\n"); return 1; }
    std::printf("0071af30_TEST PASS\n");
    return 0;
}
