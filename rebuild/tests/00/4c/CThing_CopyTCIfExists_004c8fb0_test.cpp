// Behaviour test for 004c8fb0: body copied from src/004c8fb0.cpp (CThing::CopyTCIfExists), the callees stubbed.
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
static int g_putsCalls = 0, g_abortCalls = 0;
static int my_puts(const char* text) { ++g_putsCalls; return (int)strlen(text); }
static void my_abort() { ++g_abortCalls; }
#define puts my_puts
#define abort my_abort
class CTCBase {
public:
    virtual void v00() {} virtual void v01() {} virtual void v02() {} virtual void v03() {} virtual void v04() {} virtual void v05() {}
    virtual void v06() {} virtual void v07() {} virtual void v08() {} virtual void v09() {} virtual void v10() {} virtual void v11() {}
    virtual void CopyFrom(const CTCBase& source) { copiedFrom = &source; ++copyCalls; }
    virtual void v13() {} virtual void v14() {} virtual void v15() {} virtual void v16() {} virtual void v17() {} virtual void v18() {}
    virtual void v19() {} virtual void v20() {}
    virtual long GetInterfaceType() const { return type; }
    long type; const CTCBase* copiedFrom; int copyCalls;
    CTCBase(long t) : type(t), copiedFrom(0), copyCalls(0) {}
};
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type) { SEntry* it = first; while (it != last && it->type < *type) ++it; return it; }
};
struct CComponentBits {
    unsigned long _M_w[9];
    bool test(unsigned long pos) const
    {
        if (pos >= 274) {
            puts("bitset");
            abort();
        }
        return (_M_w[pos >> 5] & (1UL << (pos & 31))) != 0;
    }
};
class CThing {
public:
    virtual void v00() {}
    unsigned char _pad04[0x20 - 0x04];
    CComponentBits UsingComponents;
    CInterfaceMap Components;
    void CopyTCIfExists(const CTCBase& source);
};
__forceinline CTCBase* GetTC(CThing* thing, long type)
{
    long key = type;
    CInterfaceMap* map = &thing->Components;
    CInterfaceMap::SEntry* it = map->LowerBound(&key);
    return ((it == map->last || type < it->type) ? map->last : it)->component;
}
void CThing::CopyTCIfExists(const CTCBase& source)
{
    long type = source.GetInterfaceType();
    if (UsingComponents.test(type))
        GetTC(this, type)->CopyFrom(source);
}
int main()
{
    CThing thing; memset(&thing.UsingComponents, 0, sizeof(thing.UsingComponents));
    CTCBase mine(0x33), other(0x10), src(0x33), srcMissing(0x40), srcBig(274);
    CInterfaceMap::SEntry entries[2] = { { 0x10, &other }, { 0x33, &mine } };
    thing.Components.first = entries; thing.Components.last = entries + 2;
    // 1. bit clear: no copy
    thing.CopyTCIfExists(src); CHECK(mine.copyCalls == 0);
    // 2. bit set: the matching component copies from the source
    thing.UsingComponents._M_w[0x33 >> 5] = 1UL << (0x33 & 31);
    thing.CopyTCIfExists(src); CHECK(mine.copyCalls == 1); CHECK(mine.copiedFrom == &src); CHECK(other.copyCalls == 0);
    // 3. a type whose bit is clear: nothing
    thing.CopyTCIfExists(srcMissing); CHECK(mine.copyCalls == 1);
    // 4. out-of-range type (274, word 8 bit 18, clear): the bitset guard fires (puts + abort); the stubbed abort returns and the clear bit means no lookup
    thing.CopyTCIfExists(srcBig); CHECK(g_putsCalls == 1); CHECK(g_abortCalls == 1);
    if (fails == 0) printf("004c8fb0_TEST PASS\n"); else printf("004c8fb0_TEST FAIL\n");
    return fails;
}