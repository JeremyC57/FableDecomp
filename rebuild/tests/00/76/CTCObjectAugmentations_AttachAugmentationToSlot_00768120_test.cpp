// Behaviour test for 00768120: src/00768120.cpp copied verbatim (the function body and its same-TU stand-ins), the remaining
// declared callees stubbed below so calls and arguments are observable. CTCObjectAugmentations::AttachAugmentationToSlot(long defIndex, long slot)
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CTCObjectAugmentations::AttachAugmentationToSlot(long defIndex, long slot) -- fetch the thing def for the global index; when it carries an augmentation sub-def, store it in the slot; then re-attach the augmentation particles and sounds
// Leaf: AttachAugmentationToSlot
// Retail: Fable.exe (TLC, VC7.1) @ 0x00768120, 166 bytes (`ret 8`; PDB ?AttachAugmentationToSlot@CTCObjectAugmentations@@QAEXJJ@Z).
// Callees: CTCBase::GetDefinitionManager() const @ 0x006869D0, CGameDefinitionManager::GetDef<T> @ 0x0042AF3C, CDefClassBase::
//   GetSubDef<CAugmentationDef> @ 0x0042B248, AttachAugmentationParticles() @ 0x007677E0, SetupAugmentationSounds() @ 0x00766C00
//   (manifest "CTCObjectAugmentations_UpdateParticleMorphs"; the body looks sounds up through CDefStringTable::GetString), the
//   def release through vtable slot 1.
// Tried: RAII out pointer + holder (extra zero store), plain out slot + holder + entry reference, non-const def types.
//   Next: the augmentation slot as a plain CDefSlot released by hand at the end (`if (p) { if (--p->m_refCount == 0) ... }`
//   on a pointer local read after SetupAugmentationSounds); lane gap otherwise (callee-saved colouring, chunk 161 0x69A8B0 family).
void ReleaseDef_s7(void* def);   // stand-in: the def's release (count decrement, Destroy on zero)
struct CDefVirtual {
    virtual void v00();
    virtual void Destroy() const;                       // slot 1
};
struct CDefBase : CDefVirtual {
    mutable long m_refCount;                            // +0x04
};
template <class T> __forceinline void ReleaseDef(T* def)
{
    if (def && --def->m_refCount == 0)
        def->Destroy();
}
template <class T> class CDefPointer { public:
    T* Object;
    CDefPointer() : Object(0) { }
    __forceinline ~CDefPointer() { T* object = Object; ReleaseDef(object); }
    CDefPointer& operator=(T* object)
    {
        if (Object != object) {
            if (Object) {
                ReleaseDef(Object);
                Object = 0;
            }
            Object = object;
            if (object)
                ++object->m_refCount;
        }
        return *this;
    }
};
template <class T> struct CDefHolder {                  // owns the reference GetDef handed over
    T* Object;
    __forceinline ~CDefHolder() { ReleaseDef(Object); }
};
class CAugmentationDef : public CDefBase { public: };
class CDefClassBase : public CDefBase { public:
    template <class T> bool GetSubDef(CDefPointer<T>& out) const;   // 0x0042B248 (T = const CAugmentationDef)
};
void* FindDef_s7r_0();
#pragma auto_inline(off)
// Same-TU body of GetSubDef (retail instantiates it in this TU): fetch the def, then assign it to the out pointer the way
// CDefPointer::operator= does (read the old pointer, release it when different, store the new one).
template <class T> __declspec(noinline) bool CDefClassBase::GetSubDef(CDefPointer<T>& out) const
{
    void* def = FindDef_s7r_0();
    if (!def)
        return false;
    void* old = *(void**)&out;
    if (old != def) {
        if (old)
            ReleaseDef_s7(old);
        *(void**)&out = def;
    }
    return true;
}
#pragma auto_inline(on)
class CThingBaseDef : public CDefClassBase { public: };
class CGameDefinitionManager { public:
    template <class T> bool GetDef(long index, CDefPointer<T>& out) const;   // 0x0042AF3C (T = const CThingBaseDef)
};
void* FindDef_s7r_1();
#pragma auto_inline(off)
// Same-TU body of GetDef (retail instantiates it in this TU): fetch the def, then assign it to the out pointer the way
// CDefPointer::operator= does (read the old pointer, release it when different, store the new one).
template <class T> __declspec(noinline) bool CGameDefinitionManager::GetDef(long p0, CDefPointer<T>& out) const
{
    void* def = FindDef_s7r_1();
    if (!def)
        return false;
    void* old = *(void**)&out;
    if (old != def) {
        if (old)
            ReleaseDef_s7(old);
        *(void**)&out = def;
    }
    return true;
}
#pragma auto_inline(on)
template <class T> struct vector {                      // STLport vector, 12 bytes
    T* _M_start; T* _M_finish; T* _M_end_of_storage;
    T& operator[](unsigned long n) const { return *(_M_start + n); }
};
class CThing;
class CTCObjectAugmentations { public:
    virtual void v00();
    CThing*       ParentThing;                          // +0x04 (CTCBase)
    bool          ToBeRemoved;                          // +0x08
    bool          IsInGlobalUpdateTCs;                  // +0x09
    unsigned char _pad0a[2];
    CDefPointer<const CDefBase> PDef;                   // +0x0c
    bool          LoadedFromSaveGame;                   // +0x10
    vector< CDefPointer<const CThingBaseDef> > Augmentations;   // +0x14
    const CGameDefinitionManager& GetDefinitionManager() const;   // 0x006869D0 (CTCBase)
    void AttachAugmentationParticles();                 // 0x007677E0
    void SetupAugmentationSounds();                     // 0x00766C00
    void AttachAugmentationToSlot(long defIndex, long slot);
};

void CTCObjectAugmentations::AttachAugmentationToSlot(long defIndex, long slot)
{
    CDefPointer<const CThingBaseDef> fetched;
    GetDefinitionManager().GetDef(defIndex, fetched);
    CDefHolder<const CThingBaseDef> thingDef = { fetched.Object };
    fetched.Object = 0;
    CDefPointer<const CAugmentationDef> augmentation;
    if (thingDef.Object->GetSubDef(augmentation))
        Augmentations[slot] = thingDef.Object;
    AttachAugmentationParticles();
    SetupAugmentationSounds();
}

// ---- stubs and driver ----
static void* g_defs[2]; static int g_find0 = 0, g_find1 = 0, g_release = 0, g_destroy = 0, g_mgr = 0, g_particles = 0, g_sounds = 0;
static CGameDefinitionManager g_manager;
void* FindDef_s7r_0() { ++g_find0; return g_defs[0]; }
void* FindDef_s7r_1() { ++g_find1; return g_defs[1]; }
void ReleaseDef_s7(void*) { ++g_release; }
void CDefVirtual::v00() {}
void CDefVirtual::Destroy() const { ++g_destroy; }
void CTCObjectAugmentations::v00() {}
const CGameDefinitionManager& CTCObjectAugmentations::GetDefinitionManager() const { ++g_mgr; return g_manager; }
void CTCObjectAugmentations::AttachAugmentationParticles() { ++g_particles; CHECK(g_particles == g_sounds + 1); }
void CTCObjectAugmentations::SetupAugmentationSounds() { ++g_sounds; }
int main()
{
    static CThingBaseDef thingDef; thingDef.m_refCount = 1;
    static CAugmentationDef aug; aug.m_refCount = 3;
    static CThingBaseDef old; old.m_refCount = 2;
    CDefPointer<const CThingBaseDef> slots[3]; slots[0].Object = 0; slots[1].Object = &old; slots[2].Object = 0;
    static unsigned char raw[sizeof(CTCObjectAugmentations)]; CTCObjectAugmentations& t = *(CTCObjectAugmentations*)raw;
    t.Augmentations._M_start = slots; t.Augmentations._M_finish = slots + 3; t.Augmentations._M_end_of_storage = slots + 3;
    g_defs[1] = &thingDef; g_defs[0] = &aug;
    t.AttachAugmentationToSlot(12, 1);
    CHECK(g_mgr == 1 && g_find1 == 1 && g_find0 == 1); CHECK(slots[1].Object == &thingDef && old.m_refCount == 1 && thingDef.m_refCount == 1);   // slot took a reference (+1), the holder dropped the fetched one (-1)
    CHECK(aug.m_refCount == 2 && g_particles == 1 && g_sounds == 1 && g_destroy == 0);
    // no augmentation sub-def: the slot is left alone, the thing def's last reference is dropped
    static CThingBaseDef other; other.m_refCount = 1; g_defs[1] = &other; g_defs[0] = 0;
    t.AttachAugmentationToSlot(13, 2); CHECK(slots[2].Object == 0 && other.m_refCount == 0 && g_destroy == 1 && g_particles == 2 && g_sounds == 2);
    // the same def again into its slot: operator= is a no-op on equal pointers
    g_defs[1] = &thingDef; g_defs[0] = &aug; thingDef.m_refCount = 2; t.AttachAugmentationToSlot(12, 1); CHECK(slots[1].Object == &thingDef && thingDef.m_refCount == 1);
    CHECK(g_release == 0);
    slots[0].Object = slots[1].Object = slots[2].Object = 0;
    if (fails == 0) printf("00768120_TEST PASS\n"); else printf("00768120_TEST FAIL\n");
    return fails;
}