// Behaviour test for 0x007352D0 CTCCombat::AddThingToAlwaysBlock(const CThing*).
#include <stdio.h>
struct CBaseObject { int id; };
class CThing : public CBaseObject {};
static int g_live = 0;
class CBaseIntelligentPointer {
public:
    CBaseIntelligentPointer() : m_item(0) { ++g_live; }
    CBaseIntelligentPointer(const CBaseIntelligentPointer& o) : m_item(o.m_item) { ++g_live; }
    virtual ~CBaseIntelligentPointer() { --g_live; }
    CBaseObject* GetPItem() const { return m_item; }
    void SetPItem(CBaseObject* item) { m_item = item; }
    bool operator==(const CBaseIntelligentPointer& o) const { return m_item == o.m_item; }
    CBaseObject* m_item;
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    CIntelligentPointer(T* item) { SetPItem((CBaseObject*)item); }
};
typedef CIntelligentPointer<const CThing> TThingPtr;
struct random_access_iterator_tag { random_access_iterator_tag() {} };
TThingPtr* __find(TThingPtr* first, TThingPtr* last, const TThingPtr& val, const random_access_iterator_tag&)
{
    for (; first != last; ++first)
        if (*first == val)
            return first;
    return last;
}
inline TThingPtr* find(TThingPtr* first, TThingPtr* last, const TThingPtr& val)
{
    return __find(first, last, val, random_access_iterator_tag());
}
struct CThingPtrVector {
    TThingPtr* _M_start;
    TThingPtr* _M_finish;
    TThingPtr* _M_end_of_storage;
    int pushes;
    TThingPtr* begin() { return _M_start; }
    TThingPtr* end() { return _M_finish; }
    void push_back(const TThingPtr& x) { ++pushes; *_M_finish = x; ++_M_finish; }   // fixture has capacity
};
class CTCCombat {
public:
    CThingPtrVector AlwaysBlockThings;
    void AddThingToAlwaysBlock(const CThing* thing);
};
void CTCCombat::AddThingToAlwaysBlock(const CThing* thing)
{
    TThingPtr pointer(thing);
    TThingPtr* end = AlwaysBlockThings.end();
    if (find(AlwaysBlockThings.begin(), end, pointer) == end)
        AlwaysBlockThings.push_back(pointer);
}
int main()
{
    int fail = 0;
    CThing a, b;
    {
        TThingPtr storage[4] = { TThingPtr(0), TThingPtr(0), TThingPtr(0), TThingPtr(0) };
        CTCCombat c;
        c.AlwaysBlockThings._M_start = storage; c.AlwaysBlockThings._M_finish = storage;
        c.AlwaysBlockThings._M_end_of_storage = storage + 4; c.AlwaysBlockThings.pushes = 0;
        int live = g_live;
        c.AddThingToAlwaysBlock(&a);
        c.AddThingToAlwaysBlock(&b);
        c.AddThingToAlwaysBlock(&a);              // already there: not added again
        if (c.AlwaysBlockThings.pushes != 2) fail |= 1;
        if (c.AlwaysBlockThings._M_finish - c.AlwaysBlockThings._M_start != 2) fail |= 2;
        if (storage[0].GetPItem() != &a || storage[1].GetPItem() != &b) fail |= 4;
        if (g_live != live) fail |= 8;            // the local pointer is destroyed every call
    }
    if (fail) { printf("007352d0_TEST FAIL %d\n", fail); return 1; }
    printf("007352d0_TEST PASS\n");
    return 0;
}