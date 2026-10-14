#include <cstdio>
struct CLink_012b4018 {
    virtual void v00() { }
    void* m_pOwner;
    CLink_012b4018(void* owner) : m_pOwner(owner) { }
};
struct CWorker_00c26c10 {
    CLink_012b4018* m_pLink;
    unsigned char _body[0x2c];
    CWorker_00c26c10(CLink_012b4018* link) : m_pLink(link) { }
};
struct CVtable_012b401c {
    virtual void v00() { }
    CLink_012b4018   m_link;
    CWorker_00c26c10 m_worker;
    void*            m_pOwner;
    CVtable_012b401c(void* owner);
};
CVtable_012b401c::CVtable_012b401c(void* owner)
    : m_link(owner), m_worker(&m_link), m_pOwner(owner)
{
}
int main()
{
    int owner;
    CVtable_012b401c o(&owner);
    if (o.m_link.m_pOwner != &owner || o.m_worker.m_pLink != &o.m_link || o.m_pOwner != &owner
        || reinterpret_cast<unsigned char*>(&o.m_pOwner) - reinterpret_cast<unsigned char*>(&o) != 0x3c) { std::printf("00c26a30_TEST FAIL\n"); return 1; }
    std::printf("00c26a30_TEST PASS\n");
    return 0;
}