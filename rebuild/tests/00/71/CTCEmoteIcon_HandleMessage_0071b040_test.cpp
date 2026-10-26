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
class CTCMessage {
public:
    long m_type;
    virtual void v000() { }
    virtual long GetType() const { return m_type; }
};
class CTCEmoteIcon : public CTCEmoteIconBase { public:
    void HandleMessage(const CTCMessage* message)
    {
        if (message->GetType() == 1 && PrimitiveHandle.IsValid())
            PrimitiveHandle.Release();
    }
};
int main()
{
    CEnginePersistentPrimitiveBase p; p.RefCount = 1;
    CTCEmoteIcon e; e.PrimitiveHandle.Data.Object = &p;
    CTCMessage other; other.m_type = 4; CTCMessage one; one.m_type = 1;
    e.HandleMessage(&other);
    bool kept = e.PrimitiveHandle.Data.Object == &p;
    e.HandleMessage(&one);
    bool released = e.PrimitiveHandle.Data.Object == 0 && g_destroyed == 1 && g_lastHandle == 1;
    e.HandleMessage(&one);
    if (!kept || !released || g_destroyed != 1) { std::printf("0071b040_TEST FAIL\n"); return 1; }
    std::printf("0071b040_TEST PASS\n");
    return 0;
}