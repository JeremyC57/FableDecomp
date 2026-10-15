// Behaviour test for 0x006988B0 CGameCameraManager::PushCurrentManualMode.
#include <cstdio>
static int g_destroyed = 0;
struct CThing { unsigned char m_statusFlags; };
struct CDef { int m_count; void Destroy() { ++g_destroyed; } };
struct CDefPointer {
    CDef* m_p;
    CDefPointer(CDef* p) : m_p(p) {}
    CDefPointer(const CDefPointer& o) : m_p(o.m_p) { if (m_p) ++m_p->m_count; }
    ~CDefPointer() { if (m_p) { if (--m_p->m_count == 0) m_p->Destroy(); } }
};
struct CScriptModule { bool allows; bool AllowsManualControl() { return allows; } };
struct M {
    CScriptModule* PCurrentScriptModule; CScriptModule* PPendingScriptModule; CThing* PObservedThing;
    CDefPointer* CameraListIndex; CDefPointer* SpecialCameraList_first; CDefPointer* SpecialCameraListIndex;
    bool UsingCameraManually, BlockModeChange; CDef* prepared; int seenCount;
    void PreparePendingMode(CDefPointer def) { prepared = def.m_p; seenCount = def.m_p->m_count; }
    void PushCurrentManualMode()
    {
        BlockModeChange = false;
        UsingCameraManually = true;
        if (PObservedThing && !(PObservedThing->m_statusFlags & 1)) {
            if (!PPendingScriptModule) {
                if (!PCurrentScriptModule || PCurrentScriptModule->AllowsManualControl()) {
                    CDefPointer def = *CameraListIndex;
                    PreparePendingMode(def);
                    SpecialCameraListIndex = SpecialCameraList_first;
                }
            }
        }
    }
};
static int Fail() { std::printf("006988b0_TEST FAIL\n"); return 1; }
int main()
{
    CDef d; d.m_count = 1;
    CDefPointer list[2] = { CDefPointer(&d), CDefPointer(0) };
    CDefPointer special[2] = { CDefPointer(0), CDefPointer(0) };
    CThing t; t.m_statusFlags = 0;
    CScriptModule cur; cur.allows = false;
    M m; m.PCurrentScriptModule = &cur; m.PPendingScriptModule = 0; m.PObservedThing = &t; m.CameraListIndex = &list[0];
    m.SpecialCameraList_first = &special[0]; m.SpecialCameraListIndex = &special[1]; m.UsingCameraManually = false;
    m.BlockModeChange = true; m.prepared = 0;
    m.PushCurrentManualMode();
    if (!m.UsingCameraManually || m.BlockModeChange || m.prepared) return Fail();
    cur.allows = true; m.PushCurrentManualMode();
    if (m.prepared != &d || m.seenCount != 3 || d.m_count != 1 || m.SpecialCameraListIndex != &special[0]) return Fail();
    m.prepared = 0; t.m_statusFlags = 1; m.PushCurrentManualMode();
    if (m.prepared) return Fail();
    list[0].m_p = 0;
    std::printf("006988b0_TEST PASS\n");
    return 0;
}