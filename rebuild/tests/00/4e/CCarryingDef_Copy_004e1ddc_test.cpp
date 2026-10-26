#include <cstdio>
static int g_baseCopies = 0, g_vecCopies = 0;
class CDefClassBase { public: virtual void v00() {} unsigned char _body[0x24]; void Copy(const CDefClassBase& o) { ++g_baseCopies; _body[3] = o._body[3]; } };
struct CLongVector { long* m_pFirst; long* m_pLast; long* m_pEnd; CLongVector& operator=(const CLongVector& o) { ++g_vecCopies; m_pFirst = o.m_pFirst; m_pLast = o.m_pLast; m_pEnd = o.m_pEnd; return *this; } };
class CCarryingDef : public CDefClassBase { public: CLongVector AvailableCarrySlots; long OverriddenDummyObject_TablePos;
    void Copy(const CCarryingDef& o) { CDefClassBase::Copy(o); AvailableCarrySlots = o.AvailableCarrySlots; OverriddenDummyObject_TablePos = o.OverriddenDummyObject_TablePos; } };

int main()
{
    long slots[2] = { 4, 5 };
    CCarryingDef a, b; a._body[3] = 0x42; a.AvailableCarrySlots.m_pFirst = slots; a.AvailableCarrySlots.m_pLast = slots + 2; a.AvailableCarrySlots.m_pEnd = slots + 2; a.OverriddenDummyObject_TablePos = 77;
    b._body[3] = 0; b.AvailableCarrySlots.m_pFirst = 0; b.OverriddenDummyObject_TablePos = 0;
    b.Copy(a);
    if (g_baseCopies != 1 || g_vecCopies != 1 || b._body[3] != 0x42 || b.AvailableCarrySlots.m_pLast != slots + 2 || b.OverriddenDummyObject_TablePos != 77) { std::printf("004e1ddc_TEST FAIL\n"); return 1; }
    if ((unsigned char*)&b.OverriddenDummyObject_TablePos - (unsigned char*)&b != 0x34) { std::printf("004e1ddc_TEST FAIL\n"); return 1; }
    std::printf("004e1ddc_TEST PASS\n");
    return 0;
}