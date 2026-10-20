// Behaviour test for 0073af60: src/0073af60.cpp copied verbatim (the function body and its same-TU stand-ins), the remaining
// declared callees stubbed below so calls and arguments are observable. CTCCreatureModeManager::SetupDefaultMode()
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CTCCreatureModeManager::SetupDefaultMode() -- add the default creature mode named by the thing's CCreatureModeDef sub-def
// into edi right after GetSubDef, this in ebx); the lane also keeps this in edi and splits the release into dec/reload/test.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0073AF60, length 95 bytes (+1 int3).
// Callees: CDefClassBase::GetSubDef<CCreatureModeDef>(CDefPointer<CCreatureModeDef>*) @ 0x0073DBE0 (bool, out-parameter;
// manifest label names another instantiation), CTCCreatureModeManager::AddMode(long) @ 0x0073AE10; the def releases are
// the pointee's virtual slot 1.
// Shape: the thing's base def (CThing +0x70 = PBaseDef; PDB +0x80) is copied into a local CDefPointer (add-ref `inc [esi+4]`),
// the sub-def pointer local is zero-constructed before the null test, and DefaultCreatureMode (+0x28, PDB name) is passed
// on. The sub-def release is `dec [edi+4] ; jne ; call [vt+4]`; the base-def release sits inside the non-null arm with no
// second null test (the null path skips both destructors by constant propagation).
void ReleaseDef_s7(void* def);   // stand-in: the def's release (count decrement, Destroy on zero)
struct CDefPointeeBase {
    virtual void v00();
    virtual void Destroy();                 // slot 1
    long RefCount;                          // +0x04
};

template <class T>
struct CDefPointer {
    T* Object;
    CDefPointer() : Object(0) { }
    CDefPointer(const CDefPointer& other) : Object(other.Object)
    {
        if (Object)
            ++Object->RefCount;
    }
    ~CDefPointer()
    {
        if (Object) {
            if (--Object->RefCount == 0)
                Object->Destroy();
        }
    }
    T* operator->() const { return Object; }
};

struct CCreatureModeDef : CDefPointeeBase {
    unsigned char _pad08[0x28 - 8];
    long DefaultCreatureMode;               // +0x28
    long InitialCreatureMode;               // +0x2c
    long DefaultWeaponCreatureMode;         // +0x30
};

struct CThingBaseDef : CDefPointeeBase {
    template <class T> bool GetSubDef(CDefPointer<T>* out) const;   // 0x0073DBE0 for T = CCreatureModeDef
};
void* FindDef_s7r_0();
#pragma auto_inline(off)
// Same-TU body of GetSubDef (retail instantiates it in this TU): fetch the def, then assign it to the out pointer the way
// CDefPointer::operator= does (read the old pointer, release it when different, store the new one).
template <class T> __declspec(noinline) bool CThingBaseDef::GetSubDef(CDefPointer<T>* out) const
{
    void* def = FindDef_s7r_0();
    if (!def)
        return false;
    void* old = *(void**)out;
    if (old != def) {
        if (old)
            ReleaseDef_s7(old);
        *(void**)out = def;
    }
    return true;
}
#pragma auto_inline(on)

struct CThing {
    unsigned char _pad00[0x70];
    CDefPointer<CThingBaseDef> PBaseDef;    // +0x70
};

struct CTCCreatureModeManager {
    void* __vftable;                        // +0x00 (CTCBase)
    CThing* ParentThing;                    // +0x04
    bool ToBeRemoved;                       // +0x08
    bool IsInGlobalUpdateTCs;               // +0x09
    bool AddMode(long modeType);            // 0x0073AE10
    void SetupDefaultMode();
};

void CTCCreatureModeManager::SetupDefaultMode()
{
    CDefPointer<CThingBaseDef> thingDef(ParentThing->PBaseDef);
    CDefPointer<CCreatureModeDef> modeDef;
    if (thingDef.Object) {
        if (thingDef->GetSubDef(&modeDef))
            AddMode(modeDef->DefaultCreatureMode);
    }
}

// ---- stubs and driver ----
static void* g_def = 0; static int g_find = 0, g_release = 0, g_destroy = 0, g_add = 0; static long g_mode = -1;
void* FindDef_s7r_0() { ++g_find; return g_def; }
void ReleaseDef_s7(void*) { ++g_release; }
void CDefPointeeBase::v00() {}
void CDefPointeeBase::Destroy() { ++g_destroy; }
bool CTCCreatureModeManager::AddMode(long modeType) { ++g_add; g_mode = modeType; return true; }
int main()
{
    static CThingBaseDef base; base.RefCount = 1;
    static CCreatureModeDef mode; mode.RefCount = 3; mode.DefaultCreatureMode = 42;
    static CThing thing; thing.PBaseDef.Object = &base;
    CTCCreatureModeManager m; m.ParentThing = &thing;
    g_def = 0; m.SetupDefaultMode(); CHECK(g_find == 1 && g_add == 0 && base.RefCount == 1 && g_destroy == 0);
    g_def = &mode; m.SetupDefaultMode(); CHECK(g_find == 2 && g_add == 1 && g_mode == 42 && mode.RefCount == 2 && base.RefCount == 1);
    thing.PBaseDef.Object = 0; m.SetupDefaultMode(); CHECK(g_find == 2 && g_add == 1);
    thing.PBaseDef.Object = &base; mode.RefCount = 1; m.SetupDefaultMode(); CHECK(g_add == 2 && mode.RefCount == 0 && g_destroy == 1);
    CHECK(g_release == 0);
    thing.PBaseDef.Object = 0;   // the CThing's own pointer must not release at exit
    if (fails == 0) printf("0073af60_TEST PASS\n"); else printf("0073af60_TEST FAIL\n");
    return fails;
}