// Behaviour test for 008967f0: src/008967f0.cpp copied verbatim (shared headers expanded inline), callees stubbed below so calls
// and arguments are observable. CGameScriptInterface::SetThingAsUsable(const CScriptThing& thing, bool usable) const
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CGameScriptInterface::SetThingAsUsable(const CScriptThing& thing, bool usable) const -- set the usable flag on the
// thing's usable component (TC 0x0D), else on its scripted-use hook (TC 0xC2), adding the hook when it has neither
// Leaf: ?SetThingAsUsable@CGameScriptInterface
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008967F0, length 208 bytes, `ret 8`.
// Callees: the script thing's virtual slot 75 IsValid (+0x12C) and slot 11 GetThing (+0x2C), the TC map lower bound
// CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound @ 0x0040F020, CCharString::CCharString(const char*, long)
// @ 0x0099EBF0, CThing::AddTC(const CCharString&, bool, CTCParamsBase*) @ 0x004C9D60, ~CCharString @ 0x0099EAE0.
// String 0x01239FE4 = "CTCActionUseScriptedHook".
// Layout: CThing interface bitset +0x20 (TC 0x0D = +0x20 bit 13, TC 0xC2 = +0x38 bit 2), TC map +0x44 {first,last};
// TC 0x0D's usable byte +0x10, CTCActionUseScriptedHook::Usable +0x0C.
// Shape: the out-parameter GetInterface template (landed 0x00896000 model) homes both lookup keys in the dead `thing`
// argument slot; the usable-TC arm returns, the hook arm is `if (!GetInterface(&hook)) { named CCharString; AddTC }`
// joined by one `hook->Usable = usable` (tail-duplicated, hook in esi on both paths). The same-TU LowerBound body
// (0x0040F020 stand-in, binary search over {type, component} pairs) tells VC7.1 the key's address does not escape,
// so the 0xC2 key and the hook name share the dead slot instead of opening a push-ecx frame word.

class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);     // 0x0099EBF0
    ~CCharString();                                 // 0x0099EAE0
};

class CTCBase;
class CTCParamsBase;

struct CInterfaceMap_Retail {                    // CVectorMap<ETCInterfaceType, CTCBase*> at CThing+0x44
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);           // 0x0040F020
};


#pragma auto_inline(off)
__declspec(noinline) CInterfaceMap_Retail::SEntry* CInterfaceMap_Retail::LowerBound(const long* type)
{
    SEntry* it = first;
    long count = last - it;
    while (count > 0) {
        long half = count / 2;
        if (it[half].type < *type) { it += half + 1; count -= half + 1; }
        else count = half;
    }
    return it;
}
#pragma auto_inline(on)
struct CTCUsable_0D {                     // the TC 0x0D component
    unsigned char _pad[0x10];
    bool Usable;                          // +0x10
};

class CTCActionUseScriptedHook { public:
    unsigned char _pad[0xc];
    bool Usable;                          // +0x0C
};

class CThing { public:
    virtual void v000();
    unsigned char _pad04[0x1c];
    unsigned long m_presentBits[9];       // +0x20 std::bitset of the interface types present
    CInterfaceMap_Retail m_interfaces;           // +0x44
    CTCBase* AddTC(const CCharString& name, bool activate, CTCParamsBase* params);   // 0x004C9D60
    unsigned long HasTC(long interfaceType) const { return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31)); }
    __forceinline CTCBase* GetTC(long interfaceType)
    {
        CInterfaceMap_Retail* map = &m_interfaces;
        CInterfaceMap_Retail::SEntry* it = map->LowerBound(&interfaceType);
        if (it == map->last || interfaceType < it->type)
            it = map->last;
        return it->component;
    }
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap_Retail* map = &m_interfaces;
            CInterfaceMap_Retail::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
};

class CScriptThing { public:
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007();
    virtual void v008(); virtual void v009(); virtual void v010();
    virtual CThing* GetThing() const;              // slot 11 (+0x2c)
    virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019();
    virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023(); virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027();
    virtual void v028(); virtual void v029(); virtual void v030(); virtual void v031(); virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035();
    virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039(); virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043();
    virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047(); virtual void v048(); virtual void v049(); virtual void v050(); virtual void v051();
    virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055(); virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059();
    virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063(); virtual void v064(); virtual void v065(); virtual void v066(); virtual void v067();
    virtual void v068(); virtual void v069(); virtual void v070(); virtual void v071(); virtual void v072(); virtual void v073(); virtual void v074();
    virtual bool IsValid() const;                  // slot 75 (+0x12c)
};

class CGameScriptInterface { public:
    virtual void SetThingAsUsable(const CScriptThing& thing, bool usable) const;
};

void CGameScriptInterface::SetThingAsUsable(const CScriptThing& thing, bool usable) const
{
    if (thing.IsValid()) {
        CThing* t = thing.GetThing();
        CTCUsable_0D* usableTC;
        CTCActionUseScriptedHook* hook;
        if (t->GetInterface(&usableTC, 0x0d)) {
            usableTC->Usable = usable;
            return;
        }
        if (!t->GetInterface(&hook, 0xc2)) {
            CCharString name("CTCActionUseScriptedHook", -1);
            hook = (CTCActionUseScriptedHook*)t->AddTC(name, false, 0);
        }
        hook->Usable = usable;
    }
}

// ---- stubs and driver ----
static int g_strCtors = 0, g_strDtors = 0, g_adds = 0;
static const char* g_lastText = 0;
CCharString::CCharString(const char* text, long length) { ++g_strCtors; g_lastText = text; CHECK(length == -1); PStringData = (void*)text; }
CCharString::~CCharString() { ++g_strDtors; }
static CTCActionUseScriptedHook g_added;
static bool g_addActivate = true; static CTCParamsBase* g_addParams = (CTCParamsBase*)1;
CTCBase* CThing::AddTC(const CCharString& name, bool activate, CTCParamsBase* params)
{ ++g_adds; CHECK(strcmp((const char*)name.PStringData, "CTCActionUseScriptedHook") == 0); g_addActivate = activate; g_addParams = params; return (CTCBase*)&g_added; }
void CThing::v000() { }
void CScriptThing::v000() { } void CScriptThing::v001() { } void CScriptThing::v002() { } void CScriptThing::v003() { } void CScriptThing::v004() { } void CScriptThing::v005() { } void CScriptThing::v006() { } void CScriptThing::v007() { } void CScriptThing::v008() { } void CScriptThing::v009() { } void CScriptThing::v010() { } void CScriptThing::v012() { } void CScriptThing::v013() { } void CScriptThing::v014() { } void CScriptThing::v015() { } void CScriptThing::v016() { } void CScriptThing::v017() { } void CScriptThing::v018() { } void CScriptThing::v019() { } void CScriptThing::v020() { } void CScriptThing::v021() { } void CScriptThing::v022() { } void CScriptThing::v023() { } void CScriptThing::v024() { } void CScriptThing::v025() { } void CScriptThing::v026() { } void CScriptThing::v027() { } void CScriptThing::v028() { } void CScriptThing::v029() { } void CScriptThing::v030() { } void CScriptThing::v031() { } void CScriptThing::v032() { } void CScriptThing::v033() { } void CScriptThing::v034() { } void CScriptThing::v035() { } void CScriptThing::v036() { } void CScriptThing::v037() { } void CScriptThing::v038() { } void CScriptThing::v039() { } void CScriptThing::v040() { } void CScriptThing::v041() { } void CScriptThing::v042() { } void CScriptThing::v043() { } void CScriptThing::v044() { } void CScriptThing::v045() { } void CScriptThing::v046() { } void CScriptThing::v047() { } void CScriptThing::v048() { } void CScriptThing::v049() { } void CScriptThing::v050() { } void CScriptThing::v051() { } void CScriptThing::v052() { } void CScriptThing::v053() { } void CScriptThing::v054() { } void CScriptThing::v055() { } void CScriptThing::v056() { } void CScriptThing::v057() { } void CScriptThing::v058() { } void CScriptThing::v059() { } void CScriptThing::v060() { } void CScriptThing::v061() { } void CScriptThing::v062() { } void CScriptThing::v063() { } void CScriptThing::v064() { } void CScriptThing::v065() { } void CScriptThing::v066() { } void CScriptThing::v067() { } void CScriptThing::v068() { } void CScriptThing::v069() { } void CScriptThing::v070() { } void CScriptThing::v071() { } void CScriptThing::v072() { } void CScriptThing::v073() { } void CScriptThing::v074() { }
static CThing* g_thing; static bool g_valid; static int g_getThings = 0;
CThing* CScriptThing::GetThing() const { ++g_getThings; return g_thing; }
bool CScriptThing::IsValid() const { return g_valid; }
int main()
{
    static CThing thing; static CScriptThing st; CGameScriptInterface gsi;
    static CTCUsable_0D usableTC; static CTCActionUseScriptedHook hook;
    static CInterfaceMap_Retail::SEntry entries[3];
    entries[0].type = 0x05; entries[0].component = 0;
    entries[1].type = 0x0d; entries[1].component = (CTCBase*)&usableTC;
    entries[2].type = 0xc2; entries[2].component = (CTCBase*)&hook;
    thing.m_interfaces.first = &entries[0]; thing.m_interfaces.last = &entries[3];
    g_thing = &thing;
    // 1) invalid script thing: nothing happens
    g_valid = false; gsi.SetThingAsUsable(st, true);
    CHECK(g_getThings == 0 && usableTC.Usable == false && hook.Usable == false && g_adds == 0);
    // 2) has TC 0x0D: its flag set, hook untouched
    g_valid = true; thing.m_presentBits[0] = 1UL << 13; thing.m_presentBits[6] = 1UL << 2;
    gsi.SetThingAsUsable(st, true);
    CHECK(g_getThings == 1 && usableTC.Usable == true && hook.Usable == false && g_adds == 0);
    gsi.SetThingAsUsable(st, false);
    CHECK(usableTC.Usable == false);
    // 3) no 0x0D but a hook: the hook flag
    thing.m_presentBits[0] = 0; usableTC.Usable = true;
    gsi.SetThingAsUsable(st, true);
    CHECK(hook.Usable == true && usableTC.Usable == true && g_adds == 0 && g_strCtors == 0);
    // 4) neither: the hook is added (inactive, no params) under its class name and flagged, the name destroyed
    thing.m_presentBits[6] = 0; g_added.Usable = false;
    gsi.SetThingAsUsable(st, true);
    CHECK(g_adds == 1 && g_strCtors == 1 && g_strDtors == 1 && g_added.Usable == true);
    CHECK(g_addActivate == false && g_addParams == 0);
    CHECK(g_lastText && strcmp(g_lastText, "CTCActionUseScriptedHook") == 0);
    gsi.SetThingAsUsable(st, false);
    CHECK(g_adds == 2 && g_added.Usable == false && hook.Usable == true);
    // 5) the stand-in lower bound: a bit set but the key missing falls back to `last`
    {
        static CInterfaceMap_Retail::SEntry e2[2]; static CTCActionUseScriptedHook tail;
        e2[0].type = 0x0d; e2[0].component = (CTCBase*)&usableTC; e2[1].type = 0x7fffffff; e2[1].component = (CTCBase*)&tail;
        long key = 0xc2;
        thing.m_interfaces.first = &e2[0]; thing.m_interfaces.last = &e2[1];
        CHECK(thing.m_interfaces.LowerBound(&key) == &e2[1]);
        key = 0x0d; CHECK(thing.m_interfaces.LowerBound(&key) == &e2[0]);
    }
    if (fails == 0) printf("008967f0_TEST PASS\n");
    return fails;
}
