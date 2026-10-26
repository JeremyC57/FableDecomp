#include <cstdio>
class CBaseObject { public: int x; };
static CBaseObject* g_item = 0; static int g_base = 0;
struct CBaseIntelligentPointer { virtual void v00() { } CBaseIntelligentPointer() { ++g_base; } CBaseObject* GetPItem() const { return m; } void SetPItem(CBaseObject* i) { m = i; g_item = i; } CBaseObject* m; };
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { public: virtual void v00() { } CIntelligentPointer() : CBaseIntelligentPointer() { } CIntelligentPointer(T* item) : CBaseIntelligentPointer() { SetPItem((CBaseObject*)item); } CIntelligentPointer(const CIntelligentPointer<T>& o) : CBaseIntelligentPointer() { SetPItem(o.GetPItem()); } };
int main() { CBaseObject o; CIntelligentPointer<CBaseObject> a; a.SetPItem(&o); CIntelligentPointer<CBaseObject> b(&o); CIntelligentPointer<CBaseObject> c(b); bool ok = g_base == 3 && b.GetPItem() == &o && c.GetPItem() == &o; std::printf(ok ? "00936750_TEST PASS\n" : "00936750_TEST FAIL\n"); return ok ? 0 : 1; }
