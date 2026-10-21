// behaviour test for 0x00419680
#include <stdio.h>
class CBulletTimeInfoMap {                               // STLport map<void*, CBulletTimeInfo>
public:
    void*         _M_header;                             // +0x00
    unsigned long _M_node_count;                         // +0x04
    unsigned long _M_key_compare_pad;                    // +0x08
    bool empty() const { return _M_node_count == 0; }
};
class CBulletTimeManager {
public:
    CBulletTimeInfoMap BulletTimeInfoMap;                // +0x00
    bool               Override;                         // +0x0c
    bool IsBulletTimeActive() const;
};
bool CBulletTimeManager::IsBulletTimeActive() const
{
    return Override || !BulletTimeInfoMap.empty();
}
int main()
{
    CBulletTimeManager m; m.BulletTimeInfoMap._M_node_count = 0; m.Override = false; int ok = 1;
    ok &= !m.IsBulletTimeActive();
    m.Override = true; ok &= m.IsBulletTimeActive();
    m.Override = false; m.BulletTimeInfoMap._M_node_count = 2; ok &= m.IsBulletTimeActive();
    printf(ok ? "00419680_TEST PASS\n" : "00419680_TEST FAIL\n");
    return ok ? 0 : 1;
}