// Behaviour test for 007bd2a7: src/007bd2a7.cpp copied verbatim (the function body and its same-TU stand-ins), the remaining
// declared callees stubbed below so calls and arguments are observable. NThingShot::CTCShotDefinitionMultiArrow::GetWeaponTrailGraphic()
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// NThingShot::CTCShotDefinitionMultiArrow::GetWeaponTrailGraphic() -- loads the shot's multi-arrow sub-def into a local def pointer and returns its weapon-trail graphic id
#pragma optimize("y", off)
#pragma optimize("s", on)
// Leaf: GetWeaponTrailGraphic
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007BD2A7, length 42 bytes. Vtable 0x0126DF34
// (NThingShot::CTCShotDefinitionMultiArrow) slot 32, referenced from 0x0126DFB4; Ego_r.pdb names the virtual
// `?GetWeaponTrailGraphic@CTCShotDefinitionMultiArrow@NThingShot@@UBEJXZ` (const, returns long). The catalog label
// "CTCTavernGame::GetBanter" is propagated over this body and rejected.
// Callee: CDefClassBase::GetSubDef<T>(CDefPointer<const T>*) @ 0x007BE7EA (bool, result unused; manifest names a
// byte-identical instantiation "GetSubDef<CAbilityDef>"), called on the def object held at +0x38; the sub-def's virtual
// slot 1 destroys it when the last reference goes.
// Layout: the def object pointer at +0x38 (CTCShotDefinition), the graphic id at +0x3c of the sub-def (descriptive).
// Shape: a frame-pointer size TU (`push ebp ; mov ebp,esp`, `and dword ptr [ebp-4],0`, `leave`): `optimize("y",off)` +
// `("s",on)`. The local def pointer's release reads the count through the object with no null test (the def is known
// valid after the read of +0x3c): the destructor is an unconditional release through a LOCAL copy of the pointer (the
// member form re-reads the slot for the compare: `dec [eax+4] ; mov ecx,[ebp-4] ; cmp [ecx+4],0`).
struct CDefPointee {                         // counted def: count at +4, slot 1 destroys
    virtual void v00();
    virtual void Destroy() const;            // slot 1 (+4)
    mutable long m_refCount;                 // +0x04
    __forceinline void Release() const
    {
        if (--m_refCount == 0)
            Destroy();
    }
};

struct CMultiArrowDef : CDefPointee {
    unsigned char _pad_0x08[0x34];
    long          WeaponTrailGraphic;        // +0x3c
};

template<class T> struct CDefPointer {
    T* Object;
    CDefPointer() : Object(0) { }
    __forceinline ~CDefPointer()
    {
        Object->Release();
    }
    T* operator->() const { return Object; }
};

struct CDefClassBase {
    bool GetSubDef(CDefPointer<const CMultiArrowDef>* out);   // 0x007BE7EA
};
void* FindDef_CDefClassBase_GetSubDef_0();   // stand-in callee of the stand-in body below
// Stand-in body (the real one is out of line in retail): it writes the out slot only inline, so VC7.1 knows the def
// pointer's address does not escape the call (the landed 0x00848730 idiom).
#pragma auto_inline(off)
__declspec(noinline) bool CDefClassBase::GetSubDef(CDefPointer<const CMultiArrowDef>* out)
{
    void* def = FindDef_CDefClassBase_GetSubDef_0();
    if (!def)
        return false;
    *(void**)out = def;
    return true;
}
#pragma auto_inline(on)

struct CTCShotDefinitionMultiArrow {
    virtual void v00();
    unsigned char  _pad_0x04[0x34];
    CDefClassBase* PDef;                     // +0x38
    long GetWeaponTrailGraphic() const;
};

long CTCShotDefinitionMultiArrow::GetWeaponTrailGraphic() const
{
    CDefPointer<const CMultiArrowDef> def;
    PDef->GetSubDef(&def);
    return def->WeaponTrailGraphic;
}

// ---- stubs and driver ----
static void* g_def = 0; static int g_destroyed = 0;
void* FindDef_CDefClassBase_GetSubDef_0() { return g_def; }
void CDefPointee::v00() {}
void CDefPointee::Destroy() const { ++g_destroyed; }
void CTCShotDefinitionMultiArrow::v00() {}
int main()
{
    CMultiArrowDef def; def.m_refCount = 2; def.WeaponTrailGraphic = 0x4321; g_def = &def;
    CDefClassBase base;
    CTCShotDefinitionMultiArrow s; memset(s._pad_0x04, 0, sizeof(s._pad_0x04)); s.PDef = &base;
    CHECK(s.GetWeaponTrailGraphic() == 0x4321); CHECK(def.m_refCount == 1); CHECK(g_destroyed == 0);
    def.WeaponTrailGraphic = 7; CHECK(s.GetWeaponTrailGraphic() == 7); CHECK(def.m_refCount == 0); CHECK(g_destroyed == 1);
    if (fails == 0) printf("007bd2a7_TEST PASS\n"); else printf("007bd2a7_TEST FAIL\n");
    return fails;
}