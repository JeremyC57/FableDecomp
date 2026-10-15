#include <cstdio>
// Behaviour model of 0x00C19E40: root polymorphic class keeping its one argument.
struct CVtable_012b2f98 {
    virtual long Kind() const { return 1; }
    void* m_pOwner;
    CVtable_012b2f98(void* owner) : m_pOwner(owner) {}
};
int main()
{
    int owner = 5;
    CVtable_012b2f98 o(&owner);
    if (o.m_pOwner != &owner || o.Kind() != 1) { std::printf("00c19e40_TEST FAIL\n"); return 1; }
    std::printf("00c19e40_TEST PASS\n");
    return 0;
}
