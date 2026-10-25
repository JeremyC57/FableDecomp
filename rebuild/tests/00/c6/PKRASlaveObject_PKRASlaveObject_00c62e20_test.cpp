// Behaviour test for 00c62e20: PKRASlaveObject::PKRASlaveObject -- the owner is kept and the object is polymorphic
#include <cstdio>
class PKRASlaveObject {
public:
    PKRASlaveObject(void* owner) : m_pOwner(owner) { }
    virtual ~PKRASlaveObject() { }
    virtual long Kind() { return 3; }
    void* m_pOwner;
};
int main()
{
    int owner = 0; PKRASlaveObject s(&owner);
    int ok = s.m_pOwner == &owner && s.Kind() == 3;
    std::printf(ok ? "00c62e20_TEST PASS\n" : "00c62e20_TEST FAIL\n");
    return ok ? 0 : 1;
}
