// Behaviour test for 0x00C62E40 (standalone copy of the model and logic)
#include <cstdio>
#include <cstring>

static int g_received = -1; static int g_deleted = 0;
struct PKRACMasterObject { void ReceiveObject(bool success) { g_received = success ? 1 : 0; } };
struct PKRASlaveObject {
    PKRACMasterObject* m_pOwner;
    PKRASlaveObject(PKRACMasterObject* o) : m_pOwner(o) {}
    virtual ~PKRASlaveObject() { g_deleted++; }
    virtual void OnComplete(bool success, int flags) { m_pOwner->ReceiveObject(success); delete this; }
};

int main()
{
    PKRACMasterObject m;
    PKRASlaveObject* s = new PKRASlaveObject(&m);
    s->OnComplete(true, 0);
    if (g_received != 1 || g_deleted != 1) { std::printf("00c62e40_TEST FAIL\n"); return 1; }
    s = new PKRASlaveObject(&m);
    s->OnComplete(false, 3);
    if (g_received != 0 || g_deleted != 2) { std::printf("00c62e40_TEST FAIL\n"); return 1; }
    std::printf("00c62e40_TEST PASS\n");
    return 0;
}