#include <stdio.h>
static int g_ctors, g_dtors, g_sets; static bool g_tempDeadAtFlagStore;
struct CThing { long m_id; };
struct CBaseIntelligentPointer {
    virtual void v00() {} void* m_pItem;
    CBaseIntelligentPointer() : m_pItem(0) { ++g_ctors; }
    ~CBaseIntelligentPointer() { ++g_dtors; m_pItem = 0; }
    void SetPItem(const CThing* item) { m_pItem = (void*)item; ++g_sets; }
    const CThing* GetPItem() { return (const CThing*)m_pItem; }
};
struct CIntelligentPointer : CBaseIntelligentPointer { virtual void v00() {} CIntelligentPointer() : CBaseIntelligentPointer() { } };
struct CBrainOutput {
    unsigned long Types; long LookFlags; unsigned char _pad[0x38]; CIntelligentPointer LookAtThing;
    void SetAsLookAtThing(const CThing* thing, long lookFlags);
};
void CBrainOutput::SetAsLookAtThing(const CThing* thing, long lookFlags)
{
    Types |= 4;
    {
        CIntelligentPointer pointer;
        pointer.SetPItem(thing);
        LookAtThing.SetPItem(pointer.GetPItem());
    }
    g_tempDeadAtFlagStore = g_dtors == 2;
    LookFlags = lookFlags;
}
int main()
{
    CThing thing; thing.m_id = 5;
    CBrainOutput* o = new CBrainOutput; o->Types = 0x10; o->LookFlags = 0; g_dtors = 1;
    o->SetAsLookAtThing(&thing, 0x77);
    bool ok = o->Types == 0x14 && o->LookFlags == 0x77 && o->LookAtThing.m_pItem == &thing && g_ctors == 2 && g_sets == 2 && g_dtors == 2 && g_tempDeadAtFlagStore;
    delete o;
    if (!ok) { printf("0093e760_TEST FAIL\n"); return 1; }
    printf("0093e760_TEST PASS\n"); return 0;
}
