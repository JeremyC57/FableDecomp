// behaviour test for CThingCreatureBase::SetBodyReorienterActive (0x00663490)
#include <cstdio>
class CThingCreatureBase;
static int g_live = 0, g_made = 0;
class CThingBodyReorienter { public:
    CThingCreatureBase* owner;
    CThingBodyReorienter(CThingCreatureBase* o) : owner(o) { ++g_live; ++g_made; }
    ~CThingBodyReorienter() { --g_live; }
};
template<class T> struct scoped_ptr {
    T* px;
    T* get() const { return px; }
    void reset(T* p = 0) { T* old = px; px = p; delete old; }
};
class CThingCreatureBase { public:
    scoped_ptr<CThingBodyReorienter> BodyReorienter;
    void SetBodyReorienterActive(bool active)
    {
        if (active) {
            if (BodyReorienter.get() == 0)
                BodyReorienter.reset(new CThingBodyReorienter(this));
        } else {
            BodyReorienter.reset();
        }
    }
};
int main()
{
    CThingCreatureBase c; c.BodyReorienter.px = 0;
    c.SetBodyReorienterActive(true);
    int bad = g_live != 1 || c.BodyReorienter.get() == 0 || c.BodyReorienter.get()->owner != &c;
    CThingBodyReorienter* first = c.BodyReorienter.get();
    c.SetBodyReorienterActive(true);
    if (g_made != 1 || c.BodyReorienter.get() != first) bad = 1;
    c.SetBodyReorienterActive(false);
    if (g_live != 0 || c.BodyReorienter.get() != 0) bad = 1;
    c.SetBodyReorienterActive(false);
    if (g_live != 0) bad = 1;
    if (bad) { std::printf("00663490_TEST FAIL\n"); return 1; }
    std::printf("00663490_TEST PASS\n");
    return 0;
}