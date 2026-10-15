#include <cstdio>
struct CThingCreatureDef {
    unsigned char _flags148_0 : 6;
    unsigned char Flag148Bit6 : 1;
};
template <class T> struct CDefPointer {
    T* m_pDef;
    T* operator->() const { return m_pDef; }
};
struct CThingPlayerCreature {
    CDefPointer<CThingCreatureDef>  PDef;
    unsigned char vfunc_75();
};
unsigned char CThingPlayerCreature::vfunc_75()
{
    return PDef->Flag148Bit6;
}
int main()
{
    int bad = 0;
    CThingCreatureDef d; d._flags148_0 = 63; d.Flag148Bit6 = 0;
    CThingPlayerCreature c; c.PDef.m_pDef = &d;
    if (c.vfunc_75() != 0) bad = 1;
    d.Flag148Bit6 = 1; d._flags148_0 = 0;
    if (c.vfunc_75() != 1) bad = 2;
    if (bad) { std::printf("00661fd0_TEST FAIL\n"); return 1; }
    std::printf("00661fd0_TEST PASS\n");
    return 0;
}
