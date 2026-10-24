// behaviour test for 0x00C0EC90 CScriptHost::PurgeUnreferencedOwners: while purging is on, destroy the first unreferenced owner until none is left
#include <cstdio>

struct CScriptOwnerThing {
    bool m_unreferenced;
    bool IsUnreferenced() const { return m_unreferenced; }   // stand-in for 0x00C1EA20
};

static int g_destroyed = 0;

struct CScriptHost {
    CScriptOwnerThing** m_owners;
    unsigned long m_ownersCapacity;
    unsigned long m_ownerCount;
    long m_purge;
    void DestroyOwner(CScriptOwnerThing* owner)               // stand-in for 0x00C0EC10: unlink from the array
    {
        ++g_destroyed;
        for (unsigned long i = 0; i < m_ownerCount; ++i)
            if (m_owners[i] == owner) { for (unsigned long j = i + 1; j < m_ownerCount; ++j) m_owners[j - 1] = m_owners[j]; --m_ownerCount; return; }
    }
    void PurgeUnreferencedOwners();
};

void CScriptHost::PurgeUnreferencedOwners()
{
    if (m_purge) {
        for (;;) {
            CScriptOwnerThing* found = 0;
            for (unsigned long i = 0; !found && i < m_ownerCount; ++i) {
                CScriptOwnerThing* owner = m_owners[i];
                if (owner->IsUnreferenced())
                    found = owner;
            }
            if (!found)
                break;
            DestroyOwner(found);
        }
    }
}

int main()
{
    CScriptOwnerThing o[5] = { { true }, { false }, { true }, { true }, { false } };
    CScriptOwnerThing* arr[5] = { &o[0], &o[1], &o[2], &o[3], &o[4] };
    CScriptHost host; host.m_owners = arr; host.m_ownersCapacity = 5; host.m_ownerCount = 5; host.m_purge = 0;
    host.PurgeUnreferencedOwners();                               // purging off
    if (g_destroyed != 0 || host.m_ownerCount != 5) { std::printf("00c0ec90_TEST FAIL\n"); return 1; }
    host.m_purge = 1;
    host.PurgeUnreferencedOwners();
    if (g_destroyed != 3 || host.m_ownerCount != 2 || arr[0] != &o[1] || arr[1] != &o[4]) { std::printf("00c0ec90_TEST FAIL\n"); return 1; }
    std::printf("00c0ec90_TEST PASS\n");
    return 0;
}