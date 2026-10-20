// Behaviour test for 00839080: body copied from src/00839080.cpp (CThingGameObject::OnCollision), the callees stubbed.
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
class CTCBase;
struct C3DVector { float X, Y, Z; };
enum ECollideType { ECT_DUMMY = 0, ECT_TWO = 2 };
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type) { SEntry* it = first; while (it != last && it->type < *type) ++it; return it; }
};
static int g_baseCalls = 0, g_removeCalls = 0; static long g_removedType = -1; static const void* g_baseOther = 0;
class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];
    CInterfaceMap m_interfaces;
    bool HasTC(long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31)))
            return true;
        return false;
    }
    CTCBase* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return ((it == map->last || interfaceType < it->type) ? map->last : it)->component;
    }
    void OnCollision(ECollideType type, const C3DVector& position, const C3DVector& normal, CThing* other) { ++g_baseCalls; g_baseOther = other; }
    void RemoveTC(long interfaceType) { ++g_removeCalls; g_removedType = interfaceType; }
};
static int g_handlerCalls = 0; static ECollideType g_handlerType; static const C3DVector* g_handlerPos = 0; static const C3DVector* g_handlerNormal = 0; static CThing* g_handlerOther = 0;
struct CTCCollisionHandler {
    virtual void v000() {} virtual void v001() {} virtual void v002() {} virtual void v003() {} virtual void v004() {} virtual void v005() {} virtual void v006() {} virtual void v007() {} virtual void v008() {} virtual void v009() {} virtual void v010() {} virtual void v011() {} virtual void v012() {} virtual void v013() {} virtual void v014() {} virtual void v015() {} virtual void v016() {} virtual void v017() {} virtual void v018() {} virtual void v019() {} virtual void v020() {} virtual void v021() {} virtual void v022() {} virtual void v023() {} virtual void v024() {} virtual void v025() {} virtual void v026() {} virtual void v027() {}
    virtual void OnCollision(ECollideType type, const C3DVector& position, const C3DVector& normal, CThing* other) { ++g_handlerCalls; g_handlerType = type; g_handlerPos = &position; g_handlerNormal = &normal; g_handlerOther = other; }
};
class CThingGameObject : public CThing {
public:
    void OnCollision(ECollideType type, const C3DVector& position, const C3DVector& normal, CThing* other);
};
void CThingGameObject::OnCollision(ECollideType type, const C3DVector& position, const C3DVector& normal, CThing* other)
{
    CThing::OnCollision(type, position, normal, other);
    if (HasTC(0x77))
        ((CTCCollisionHandler*)GetTC(0x77))->OnCollision(type, position, normal, other);
    if (HasTC(0x2a))
        RemoveTC(0x2a);
}
int main()
{
    CThingGameObject g; memset(&g, 0, sizeof(g)); CTCCollisionHandler h; CThing other;
    CInterfaceMap::SEntry entries[2] = { { 0x10, (CTCBase*)0x99 }, { 0x77, (CTCBase*)&h } };
    g.m_interfaces.first = entries; g.m_interfaces.last = entries + 2;
    C3DVector pos = { 1, 2, 3 }, nrm = { 0, 0, 1 };
    // 1. no components: only the base handler runs
    g.OnCollision(ECT_DUMMY, pos, nrm, &other); CHECK(g_baseCalls == 1); CHECK(g_baseOther == &other); CHECK(g_handlerCalls == 0); CHECK(g_removeCalls == 0);
    // 2. collision handler present: forwarded with the same arguments
    g.m_presentBits[0x77 >> 5] |= 1UL << (0x77 & 31);
    g.OnCollision(ECT_TWO, pos, nrm, &other); CHECK(g_baseCalls == 2); CHECK(g_handlerCalls == 1); CHECK(g_handlerType == ECT_TWO); CHECK(g_handlerPos == &pos); CHECK(g_handlerNormal == &nrm); CHECK(g_handlerOther == &other); CHECK(g_removeCalls == 0);
    // 3. kind-0x2a component present: removed
    g.m_presentBits[0x2a >> 5] |= 1UL << (0x2a & 31);
    g.OnCollision(ECT_TWO, pos, nrm, 0); CHECK(g_handlerCalls == 2); CHECK(g_handlerOther == 0); CHECK(g_removeCalls == 1); CHECK(g_removedType == 0x2a);
    // 4. only 0x2a: removed without the handler
    g.m_presentBits[0x77 >> 5] = 0;
    g.OnCollision(ECT_DUMMY, pos, nrm, &other); CHECK(g_handlerCalls == 2); CHECK(g_removeCalls == 2);
    if (fails == 0) printf("00839080_TEST PASS\n"); else printf("00839080_TEST FAIL\n");
    return fails;
}