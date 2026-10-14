#include <cstdio>
#include <cstring>
#include <new>

struct CWorld { int x; };
struct CGameNavigatorManagerExternalInterface {
    virtual ~CGameNavigatorManagerExternalInterface() {}
    virtual void Initialise() {}
    virtual CGameNavigatorManagerExternalInterface* Clone() const { return new CGameNavigatorManagerExternalInterface(*this); }
    CWorld* World;
};
int main()
{
    CWorld world; CGameNavigatorManagerExternalInterface src; src.World = &world;
    CGameNavigatorManagerExternalInterface* copy = src.Clone();
    if (copy == 0 || copy == &src || copy->World != &world || (sizeof(long) == 4 && sizeof(src) != 8)) { std::printf("004ab210_TEST FAIL\n"); return 1; }
    delete copy;
    std::printf("004ab210_TEST PASS\n");
    return 0;
}