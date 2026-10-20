// Behaviour test for CQ_HangingTreeGoodScript::UpdateWayPoint (0x00D74E80): the waypoint name is "HTG_WayPoint<n>",
// the looked-up thing replaces ActiveWayPoint (old block released and deleted, new block referenced once), the
// temporaries are released, and the result is the new waypoint's IsAlive().
#include <cstdio>
#include <cstring>

static int g_live = 0;
class CCharString {
public:
    char Text[64];
    CCharString(const char* text, long = -1) { std::strcpy(Text, text); ++g_live; }
    CCharString(const CCharString& o) { std::strcpy(Text, o.Text); ++g_live; }
    ~CCharString() { --g_live; }
};
static CCharString GFIntToCharString(long v) { char b[16]; std::sprintf(b, "%ld", v); return CCharString(b); }
static CCharString operator+(const char* l, const CCharString& r) { char b[64]; std::strcpy(b, l); std::strcat(b, r.Text); return CCharString(b); }

struct CCPPointerInfo { long RefCount; void (*DeleteFunc)(void*); void* Data; };
static int g_pointeeDeletes = 0;
static void DeletePointee(void*) { ++g_pointeeDeletes; }
template <class T> struct CCountedPointer {
    T* Data; CCPPointerInfo* Info;
    void Release()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) { Info->DeleteFunc(Info->Data); delete Info; }
        }
    }
    CCountedPointer& operator=(const CCountedPointer& other)
    {
        CCPPointerInfo* info = other.Info; T* data = other.Data;
        if (Info != info) { Release(); Data = data; Info = info; if (info) ++info->RefCount; }
        return *this;
    }
    ~CCountedPointer() { Release(); Data = 0; Info = 0; }
};
struct CBaseClass { CBaseClass() {} virtual ~CBaseClass() {} };
struct CScriptThing : CBaseClass {
    CCountedPointer<CScriptThing> PImp;
    bool alive;
    CScriptThing() { PImp.Data = 0; PImp.Info = 0; alive = false; }
    CScriptThing(const CScriptThing& o) : CBaseClass() { PImp.Data = 0; PImp.Info = 0; PImp = o.PImp; alive = o.alive; }
    ~CScriptThing() {}
    CScriptThing& operator=(const CScriptThing& other) { PImp = other.PImp; alive = other.alive; return *this; }
    virtual bool IsAlive() const { return alive && PImp.Data != 0; }
};
static CScriptThing g_pointee;
struct CGameScriptInterface {
    char lastName[64];
    virtual CScriptThing GetThingWithScriptName(const CCharString& name)
    {
        std::strcpy(lastName, name.Text);
        CScriptThing t; t.PImp.Data = &g_pointee; t.PImp.Info = new CCPPointerInfo;
        t.PImp.Info->RefCount = 1; t.PImp.Info->DeleteFunc = DeletePointee; t.PImp.Info->Data = &g_pointee; t.alive = true;
        return t;
    }
};
struct CQ_HangingTreeGoodScript {
    CGameScriptInterface* m_pInterface;
    long CurrentWayPoint;
    CScriptThing ActiveWayPoint;
    bool UpdateWayPoint();
};
bool CQ_HangingTreeGoodScript::UpdateWayPoint()
{
    CCharString name = "HTG_WayPoint" + GFIntToCharString(CurrentWayPoint);
    ActiveWayPoint = m_pInterface->GetThingWithScriptName(name);
    return ActiveWayPoint.IsAlive();
}
int main()
{
    CGameScriptInterface gi;
    CQ_HangingTreeGoodScript s; s.m_pInterface = &gi; s.CurrentWayPoint = 3;
    CCPPointerInfo* old = new CCPPointerInfo; old->RefCount = 1; old->DeleteFunc = DeletePointee; old->Data = &g_pointee;
    s.ActiveWayPoint.PImp.Data = &g_pointee; s.ActiveWayPoint.PImp.Info = old;
    bool r = s.UpdateWayPoint();
    int bad = 0;
    if (!r || std::strcmp(gi.lastName, "HTG_WayPoint3") != 0 || g_live != 0) bad = 1;
    if (g_pointeeDeletes != 1 || s.ActiveWayPoint.PImp.Info == 0 || s.ActiveWayPoint.PImp.Info->RefCount != 1) bad = 1;
    if (bad) { std::printf("00d74e80_TEST FAIL\n"); return 1; }
    std::printf("00d74e80_TEST PASS\n");
    return 0;
}
