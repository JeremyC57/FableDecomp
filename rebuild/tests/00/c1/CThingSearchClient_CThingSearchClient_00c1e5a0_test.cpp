// Behaviour test for 0x00C1E5A0 (CThingSearchClient constructor): the owner and the record are stored and
// the new client registers itself with the owner's host registry.
#include <cstdio>

struct SSearchRecord {
    long m_a, m_b, m_c, m_d, m_e;
};

struct CThingSearchClient;

struct CThingSearchRegistry {
    CThingSearchClient* m_last;
    int m_registrations;
    virtual ~CThingSearchRegistry() { }
    virtual void Register(CThingSearchClient* client) { m_last = client; ++m_registrations; }
};

struct CThingSearchHost {
    CThingSearchRegistry m_registry;
};

struct CThingSearchOwner {
    CThingSearchHost* m_pHost;
};

struct CThingSearchClient {
    virtual ~CThingSearchClient() { }
    CThingSearchOwner* m_pOwner;
    SSearchRecord      m_record;
    CThingSearchClient(CThingSearchOwner* owner, const SSearchRecord& record);
};

CThingSearchClient::CThingSearchClient(CThingSearchOwner* owner, const SSearchRecord& record)
    : m_pOwner(owner), m_record(record)
{
    owner->m_pHost->m_registry.Register(this);
}

int main()
{
    CThingSearchHost host;
    host.m_registry.m_last = 0;
    host.m_registry.m_registrations = 0;
    CThingSearchOwner owner;
    owner.m_pHost = &host;
    SSearchRecord rec = { 1, 2, 3, 4, 5 };
    CThingSearchClient client(&owner, rec);
    if (client.m_pOwner != &owner || client.m_record.m_a != 1 || client.m_record.m_e != 5 ||
        host.m_registry.m_last != &client || host.m_registry.m_registrations != 1) {
        std::printf("00c1e5a0_TEST FAIL\n");
        return 1;
    }
    std::printf("00c1e5a0_TEST PASS\n");
    return 0;
}