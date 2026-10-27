// Behaviour test for 0x00C62C30 (standalone copy of the model and logic)
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

struct D : PKRASlaveObject {
    unsigned int m_count, m_size, m_offset; unsigned char* m_pBuffer; void* m_pDest;
    D(PKRACMasterObject* o) : PKRASlaveObject(o) {}
    virtual void OnComplete(bool success, int flags)
    {
        if (success) std::memcpy(m_pDest, m_pBuffer + m_offset, m_size);
        m_pOwner->ReceiveObject(success);
        delete this;
    } };
int main()
{
    PKRACMasterObject m; unsigned char buf[16]; unsigned char dst[8];
    for (int i = 0; i < 16; ++i) buf[i] = (unsigned char)(i + 1);
    std::memset(dst, 0, sizeof(dst));
    D* d = new D(&m); d->m_size = 5; d->m_offset = 3; d->m_pBuffer = buf; d->m_pDest = dst;
    PKRASlaveObject* p = d;
    p->OnComplete(true, 0);
    if (g_received != 1 || g_deleted != 1 || dst[0] != 4 || dst[4] != 8 || dst[5] != 0) { std::printf("00c62c30_TEST FAIL\n"); return 1; }
    std::memset(dst, 0, sizeof(dst));
    d = new D(&m); d->m_size = 5; d->m_offset = 0; d->m_pBuffer = buf; d->m_pDest = dst;
    d->OnComplete(false, 0);
    if (g_received != 0 || g_deleted != 2 || dst[0] != 0) { std::printf("00c62c30_TEST FAIL\n"); return 1; }
    std::printf("00c62c30_TEST PASS\n");
    return 0;
}