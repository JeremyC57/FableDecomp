#include <cstdio>
struct CThing { int id; };
struct RequestMap {
    CThing* keys[4]; unsigned long m_count;
    unsigned long size() const { return m_count; }
    unsigned long erase(CThing* const& key)
    { for (unsigned long i = 0; i < m_count; ++i) if (keys[i] == key) { keys[i] = keys[--m_count]; return 1; } return 0; }
};
struct Controller {
    RequestMap m_requests; bool m_noRequests; int updates;
    void UpdateMinMultiplier() { ++updates; }
    void RemoveRequest(CThing* requester)
    {
        m_requests.erase(requester);
        if (m_requests.size() == 0)
            m_noRequests = true;
        UpdateMinMultiplier();
    }
};
int main()
{
    CThing a, b, c; Controller k; k.m_requests.keys[0] = &a; k.m_requests.keys[1] = &b; k.m_requests.m_count = 2; k.m_noRequests = false; k.updates = 0;
    k.RemoveRequest(&a);
    if (k.m_requests.size() != 1 || k.m_noRequests || k.updates != 1) { std::printf("004c6140_TEST FAIL\n"); return 1; }
    k.RemoveRequest(&c);
    if (k.m_requests.size() != 1 || k.m_noRequests || k.updates != 2) { std::printf("004c6140_TEST FAIL\n"); return 1; }
    k.RemoveRequest(&b);
    if (k.m_requests.size() != 0 || !k.m_noRequests || k.updates != 3) { std::printf("004c6140_TEST FAIL\n"); return 1; }
    std::printf("004c6140_TEST PASS\n");
    return 0;
}
