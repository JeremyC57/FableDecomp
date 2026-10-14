#include <cstdio>
struct CLink_012b402c {
    virtual void v00() { }
    void* m_pOwner;
    CLink_012b402c(void* owner) : m_pOwner(owner) { }
};
struct CWorker_00c27070 {
    CLink_012b402c* m_pLink;
    unsigned char _body[0x2c];
    CWorker_00c27070(CLink_012b402c* link) : m_pLink(link) { }
};
struct CVtable_012b4030 {
    virtual void v00() { }
    CLink_012b402c   m_link;
    CWorker_00c27070 m_worker;
    void*            m_pOwner;
    CVtable_012b4030(void* owner);
};
CVtable_012b4030::CVtable_012b4030(void* owner)
    : m_link(owner), m_worker(&m_link), m_pOwner(owner)
{
}
int main()
{
    int owner;
    CVtable_012b4030 o(&owner);
    if (o.m_link.m_pOwner != &owner || o.m_worker.m_pLink != &o.m_link || o.m_pOwner != &owner
        || reinterpret_cast<unsigned char*>(&o.m_pOwner) - reinterpret_cast<unsigned char*>(&o) != 0x3c) { std::printf("00c26e90_TEST FAIL\n"); return 1; }
    std::printf("00c26e90_TEST PASS\n");
    return 0;
}
