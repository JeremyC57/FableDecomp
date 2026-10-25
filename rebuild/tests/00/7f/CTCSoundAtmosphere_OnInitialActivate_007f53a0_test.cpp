// behaviour test for 0x007F53A0 CTCSoundAtmosphere::OnInitialActivate(), linked against the built member: the parent
// thing's def (+0x70) is asked for its CSoundAtmospheresDef sub-def into the component's PDef (+0x10)
#include <cstdio>
#include <cstring>
#include <cstddef>
class CSoundAtmospheresDef;
template <class T> class CDefPointer { public: T* Data; };
static const void* g_self = 0; static void* g_out = 0; static int g_calls = 0;
class CThingBaseDef { public: bool GetSubDef(CDefPointer<const CSoundAtmospheresDef>* out) const; };
bool CThingBaseDef::GetSubDef(CDefPointer<const CSoundAtmospheresDef>* out) const
{
    ++g_calls; g_self = this; g_out = out; out->Data = (const CSoundAtmospheresDef*)0x5A5A; return true;
}
class CThing { public: unsigned char _pad00[0x70]; CThingBaseDef* PDef; };
class CTCBase { public: virtual ~CTCBase(); CThing* ParentThing; bool ToBeRemoved; bool IsInGlobalUpdateTCs; };
class CTCSoundAtmosphere : public CTCBase {
public:
    float TimeUntilNextUpdate; CDefPointer<const CSoundAtmospheresDef> PDef;
    virtual void OnInitialActivate();
};
int main()
{
    if (offsetof(CTCSoundAtmosphere, PDef) != 0x10 || offsetof(CThing, PDef) != 0x70) { std::printf("007f53a0_TEST FAIL layout\n"); return 1; }
    CThingBaseDef def; CThing thing; std::memset(&thing, 0, sizeof(thing)); thing.PDef = &def;
    static unsigned char mem[sizeof(CTCSoundAtmosphere)];
    std::memset(mem, 0, sizeof(mem));
    CTCSoundAtmosphere* c = (CTCSoundAtmosphere*)mem;
    c->ParentThing = &thing;
    c->CTCSoundAtmosphere::OnInitialActivate();
    bool ok = g_calls == 1 && g_self == &def && g_out == &c->PDef && c->PDef.Data == (const CSoundAtmospheresDef*)0x5A5A;
    if (!ok) { std::printf("007f53a0_TEST FAIL\n"); return 1; }
    std::printf("007f53a0_TEST PASS\n");
    return 0;
}
