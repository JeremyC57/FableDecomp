// Behaviour test for 0091f990: CIntelligentPointer<const CThing>'s converting constructor from CIntelligentPointer<CThingCreatureBase>& (NF382 name fix, was the copy constructor; model copy)
#include <cstdio>
class CBaseObject { public: int x; };
static CBaseObject* g_item = 0; static int g_base = 0;
struct CBaseIntelligentPointer { virtual void v00() { } CBaseIntelligentPointer() { ++g_base; } CBaseObject* GetPItem() const { return m; } void SetPItem(CBaseObject* i) { m = i; g_item = i; } CBaseObject* m; };
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { public: virtual void v00() { } CIntelligentPointer() : CBaseIntelligentPointer() { } CIntelligentPointer(T* item) : CBaseIntelligentPointer() { SetPItem((CBaseObject*)item); } template <class U> CIntelligentPointer(CIntelligentPointer<U>& o) : CBaseIntelligentPointer() { SetPItem(o.GetPItem()); } };
class CDerivedObject : public CBaseObject { };
int main() { CBaseObject o; CIntelligentPointer<CBaseObject> a; a.SetPItem(&o); CIntelligentPointer<CBaseObject> b(&o); CIntelligentPointer<const CBaseObject> c(b); bool ok = g_base == 3 && b.GetPItem() == &o && c.GetPItem() == &o; std::printf(ok ? "0091f990_TEST PASS\n" : "0091f990_TEST FAIL\n"); return ok ? 0 : 1; }
