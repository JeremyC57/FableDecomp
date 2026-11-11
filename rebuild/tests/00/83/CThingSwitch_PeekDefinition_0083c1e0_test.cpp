// Behaviour test for 0083c1e0 CThingSwitch::PeekDefinition: the row body verbatim over fixture models of the shared-header interfaces
// (CThing::PeekDefinitionManager, GetDef<CThingSwitchDef>, counted CDefPointer with copy). The returned pointer holds one reference.
#include <stdio.h>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
struct Counted { mutable int Refs; Counted() : Refs(1) { } };
template <class T> class CDefPointer {
public:
    T* Object;
    CDefPointer() : Object(0) { }
    CDefPointer(const CDefPointer& o) : Object(o.Object) { if (Object) ++Object->Refs; }
    ~CDefPointer() { if (Object) --Object->Refs; }
    void Set(T* p) { Object = p; if (p) ++p->Refs; }
};
class CThingSwitchDef : public Counted { };
class CGameDefinitionManager { public:
    const CThingSwitchDef* Def; mutable long Index;
    bool GetDef(long index, CDefPointer<const CThingSwitchDef>* out) const { Index = index; out->Set(Def); return Def != 0; }
};
static CGameDefinitionManager g_manager;
class CThingSwitch { public:
    unsigned short DefGlobalIndex;
    const CGameDefinitionManager& PeekDefinitionManager() const { return g_manager; }
    CDefPointer<const CThingSwitchDef> PeekDefinition() const;
};
// ---- row body (verbatim) ----
CDefPointer<const CThingSwitchDef> CThingSwitch::PeekDefinition() const
{
    CDefPointer<const CThingSwitchDef> def;
    PeekDefinitionManager().GetDef(DefGlobalIndex, &def);
    return def;
}
// ---- harness ----
int main()
{
    CThingSwitchDef def; g_manager.Def = &def; g_manager.Index = -1;
    CThingSwitch sw; sw.DefGlobalIndex = 0x1F;
    {
        CDefPointer<const CThingSwitchDef> p = sw.PeekDefinition();
        CHECK(p.Object == &def && g_manager.Index == 0x1F && def.Refs == 2);
    }
    CHECK(def.Refs == 1);
    g_manager.Def = 0;
    { CDefPointer<const CThingSwitchDef> p = sw.PeekDefinition(); CHECK(p.Object == 0); }
    if (fails == 0) printf("0083c1e0_TEST PASS\n");
    return fails;
}
