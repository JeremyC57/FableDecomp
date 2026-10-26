// Behaviour test for 00766050: body copied from src/00766050.cpp (CTCObjectAugmentations::HasCombinationOfAugmentations), callees stubbed.
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
class CAugmentationDef;
static int g_released = 0;
class CDefClassBase { public:
    virtual void Destroy() {}
    virtual void Release() { ++g_released; }
    long RefCount;
    void ReleaseReference()
    {
        if (--RefCount == 0)
            Release();
    }
};
template<class T> class CDefPointer { public:
    T* Object;
    CDefPointer() : Object(0) { }
    ~CDefPointer()
    {
        Object->ReleaseReference();
    }
    T* operator->() const { return Object; }
};
static CAugmentationDef* g_subDefs[4]; static int g_getSubDef = 0;
class CThingBaseDef : public CDefClassBase { public:
    int index;
    bool GetSubDef(CDefPointer<CAugmentationDef>* out) const;
};
class CAugmentationDef : public CDefClassBase { public:
    unsigned char _pad08[0x28 - 0x08];
    unsigned long Name;
    long  Type;
    unsigned long InventoryGraphic;
    float DamageMultiplier;
    float ExperienceMultiplier;
};
bool CThingBaseDef::GetSubDef(CDefPointer<CAugmentationDef>* out) const
{
    ++g_getSubDef; CAugmentationDef* d = g_subDefs[index]; ++d->RefCount; out->Object = d; return true;
}
struct CThingBaseDefPointer { const CThingBaseDef* Object; };
template<class T> struct CRetailVector {
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    T* begin() const { return _M_start; }
    unsigned long size() const { return (unsigned long)(_M_finish - _M_start); }
    T& operator[](unsigned long n) const { return *(begin() + n); }
};
static CThingBaseDef g_defs[3]; static CAugmentationDef g_augs[3]; static CThingBaseDefPointer g_entries[4];
static void Setup()
{
    for (int i = 0; i < 3; ++i) { g_defs[i].index = i; g_defs[i].RefCount = 1; g_augs[i].RefCount = 1; g_subDefs[i] = &g_augs[i]; g_entries[i].Object = &g_defs[i]; }
    g_entries[3].Object = 0;
    g_augs[0].Type = 1; g_augs[0].DamageMultiplier = 2.0f; g_augs[1].Type = 4; g_augs[1].DamageMultiplier = 0.5f; g_augs[2].Type = 4; g_augs[2].DamageMultiplier = 3.0f;
}
class CTCObjectAugmentations { public:
    virtual void v00() {}
    void* ParentThing;
    unsigned char _base08[4];
    void* PDef;
    bool LoadedFromSaveGame;
    CRetailVector<CThingBaseDefPointer> Augmentations;
    bool HasCombinationOfAugmentations(long typeMask) const;
};
bool CTCObjectAugmentations::HasCombinationOfAugmentations(long typeMask) const
{
    long combined = 0;
    for (unsigned long i = 0; i < Augmentations.size(); ++i) {
        const CThingBaseDef* def = Augmentations[i].Object;
        if (def) {
            CDefPointer<CAugmentationDef> augmentation;
            def->GetSubDef(&augmentation);
            combined |= augmentation->Type;
        }
    }
    return (combined & typeMask) == typeMask;
}
int main()
{
    Setup();
    CTCObjectAugmentations tc; tc.Augmentations._M_start = g_entries; tc.Augmentations._M_finish = g_entries + 4; tc.Augmentations._M_end_of_storage = g_entries + 4;
    CHECK(tc.HasCombinationOfAugmentations(5)); CHECK(g_getSubDef == 3); CHECK(g_augs[0].RefCount == 1 && g_augs[1].RefCount == 1 && g_released == 0);
    CHECK(tc.HasCombinationOfAugmentations(1)); CHECK(!tc.HasCombinationOfAugmentations(2)); CHECK(!tc.HasCombinationOfAugmentations(7)); CHECK(tc.HasCombinationOfAugmentations(0));
    tc.Augmentations._M_finish = g_entries; CHECK(!tc.HasCombinationOfAugmentations(1)); CHECK(tc.HasCombinationOfAugmentations(0));
    g_augs[0].RefCount = 0; tc.Augmentations._M_finish = g_entries + 1; CHECK(tc.HasCombinationOfAugmentations(1)); CHECK(g_released == 1);
    if (fails == 0) printf("00766050_TEST PASS\n"); else printf("00766050_TEST FAIL\n");
    return fails;
}