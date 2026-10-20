// Behaviour test for 00965fb0: the model below is a copy of src/00965fb0.cpp (compiled separately), driven by test doubles.
#include <cstdio>
#include <cstring>
static int g_ctors = 0, g_dtors = 0, g_sets = 0, g_gets = 0, g_finds = 0;
struct CThing {
    unsigned char _pad[0x91];
    unsigned char m_flags91;
    bool IsDead() const { return (m_flags91 & 1) != 0; }
};
struct CThingObject : CThing {};
struct CBaseIntelligentPointer {
    virtual void v00() {}
    void* m_pItem;
    CBaseIntelligentPointer() : m_pItem(0) { ++g_ctors; }
    ~CBaseIntelligentPointer() { ++g_dtors; }
    CThing* GetPItem() const { ++g_gets; return (CThing*)m_pItem; }
    void SetPItem(CThing* item) { ++g_sets; m_pItem = item; }
};
struct CIntelligentPointerThing : CBaseIntelligentPointer {
    virtual void v00() {}
    CIntelligentPointerThing() : CBaseIntelligentPointer() {}
    CIntelligentPointerThing(CThing* item) : CBaseIntelligentPointer() { SetPItem(item); }
    CIntelligentPointerThing& operator=(const CIntelligentPointerThing& other) { CThing* item = other.GetPItem(); SetPItem(item); return *this; }
};
struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase() {}
    void* PBrain;
    long ScriptAIPriority;
    void* Name_PStringData;
    unsigned char Terminated;
    unsigned char Priority;
    unsigned char Enabled;
    unsigned char _pad_0x13;
};
static CThingObject* g_crate = 0;
struct CAIStateGroup_CarryCrateAndFollow : CAIStateGroupBase {
    CIntelligentPointerThing PCrate;
    CIntelligentPointerThing PPreviouslyCarriedItem;
    CIntelligentPointerThing PThingToFollow;
    bool GotActionPoint;
    long ActionPointIndex;
    long PickUpActionID;
    CThingObject* FindPCrate() { ++g_finds; return g_crate; }
    virtual bool IsRunnable();
};

bool CAIStateGroup_CarryCrateAndFollow::IsRunnable()
{
    ActionPointIndex = -1;
    GotActionPoint = false;
    PickUpActionID = -1;
    PCrate = CIntelligentPointerThing((CThing*)FindPCrate());
    if (PCrate.GetPItem() != 0) {
        if (!PCrate.GetPItem()->IsDead())
            return true;
    }
    return false;
}

static bool Run(bool hasCrate, bool dead, bool expect)
{
    static CThingObject crate; std::memset(&crate, 0, sizeof(crate)); crate.m_flags91 = dead ? 1 : 0;
    g_crate = hasCrate ? &crate : 0;
    CAIStateGroup_CarryCrateAndFollow s; s.GotActionPoint = true; s.ActionPointIndex = 5; s.PickUpActionID = 9;
    g_ctors = g_dtors = g_sets = g_gets = g_finds = 0;
    bool r = s.IsRunnable();
    if (r != expect) return false;
    if (s.GotActionPoint || s.ActionPointIndex != -1 || s.PickUpActionID != -1) return false;
    if (g_finds != 1 || s.PCrate.m_pItem != (void*)g_crate) return false;
    if (g_ctors != 1 || g_dtors != 1 || g_sets != 2) return false;        // one temporary, copied into the member
    if (g_gets != (hasCrate ? 3 : 2)) return false;                       // copy + null test (+ dead test)
    return true;
}
int main()
{
    bool ok = Run(true, false, true) && Run(true, true, false) && Run(false, false, false);
    if (!ok) { std::printf("00965fb0_TEST FAIL\n"); return 1; }
    std::printf("00965fb0_TEST PASS\n");
    return 0;
}