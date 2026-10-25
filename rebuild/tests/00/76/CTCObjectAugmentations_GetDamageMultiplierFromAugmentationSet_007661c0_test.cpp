// Behaviour test for 0x007661C0 CTCObjectAugmentations::GetDamageMultiplierFromAugmentationSet: the product of the
// DamageMultiplier of each augmentation def named by a positive index (1.0 with none); zero / negative indices are
// skipped, and every def reference taken is released again.
#include <stdio.h>
#include <string.h>
#include <math.h>
#ifndef _MSC_VER
#define __fastcall
#define __declspec(x)
#endif
struct CDefClassBase {
    virtual void Destroy() const;
    virtual void Release() const;                // slot 1
    mutable long RefCount;                       // +0x04
    void ReleaseReference() const
    {
        if (--RefCount == 0)
            Release();
    }
};

template <class T> struct CDefPointer {
    T* Object;
    CDefPointer() : Object(0) { }
    ~CDefPointer()
    {
        if (Object)
            Object->ReleaseReference();
    }
    T* operator->() const { return Object; }
};

struct CAugmentationDef : CDefClassBase {
    unsigned char _pad08[0x28 - 0x08];
    unsigned long Name;                          // +0x28
    long  Type;                                  // +0x2c
    unsigned long InventoryGraphic;              // +0x30
    float DamageMultiplier;                      // +0x34
    float ExperienceMultiplier;                  // +0x38
};

void* FindDef_GetSubDef_007661c0();              // stand-in callees of the stand-in bodies below
void* FindDef_GetDef_007661c0(long index);

struct CThingBaseDef : CDefClassBase {
    bool GetSubDef(CDefPointer<const CAugmentationDef>* out) const;   // 0x0042B248 (GetSubDef<CAugmentationDef>)
};

class CDefinitionManager {
public:
    bool GetDef(long index, CDefPointer<const CThingBaseDef>* out) const;   // 0x0042AF3C (GetDef<CThingBaseDef>)
};

class CGameDefinitionManager : public CDefinitionManager {
public:
    static CGameDefinitionManager& __fastcall Get();   // 0x0044C6B0
};

__declspec(noinline) bool CThingBaseDef::GetSubDef(CDefPointer<const CAugmentationDef>* out) const
{
    void* def = FindDef_GetSubDef_007661c0();
    if (!def)
        return false;
    *(void**)out = def;
    return true;
}

__declspec(noinline) bool CDefinitionManager::GetDef(long index, CDefPointer<const CThingBaseDef>* out) const
{
    void* def = FindDef_GetDef_007661c0(index);
    if (!def)
        return false;
    *(void**)out = def;
    return true;
}

namespace std {
template <class T> class vector {
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    const T* begin() const { return _M_start; }
    const T* end() const { return _M_finish; }
    unsigned int size() const { return (unsigned int)(end() - begin()); }
    const T& operator[](unsigned int n) const { return *(begin() + n); }
};
}

class CTCObjectAugmentations {
public:
    static float __fastcall GetDamageMultiplierFromAugmentationSet(const std::vector<long>& augmentations);
};

float __fastcall CTCObjectAugmentations::GetDamageMultiplierFromAugmentationSet(const std::vector<long>& augmentations)
{
    float multiplier = 1.0f;
    for (unsigned int i = 0; i < augmentations.size(); ++i) {
        if (augmentations[i] > 0) {
            CDefPointer<const CThingBaseDef> def;
            CGameDefinitionManager::Get().GetDef(augmentations[i], &def);
            CDefPointer<const CAugmentationDef> augmentation;
            def->GetSubDef(&augmentation);
            multiplier *= augmentation->DamageMultiplier;
        }
    }
    return multiplier;
}


struct TThingDef : CThingBaseDef { };
static CAugmentationDef g_aug[4];
static TThingDef g_thing[4];
static int g_released = 0;
static long g_lastSubDefFor = -1;
static long g_current = -1;
void CDefClassBase::Destroy() const { }
void CDefClassBase::Release() const { ++g_released; }
static CGameDefinitionManager g_mgr;
CGameDefinitionManager& __fastcall CGameDefinitionManager::Get() { return g_mgr; }
void* FindDef_GetDef_007661c0(long index)
{
    if (index < 1 || index > 3) return 0;
    g_current = index;
    ++g_thing[index].RefCount;
    return &g_thing[index];
}
void* FindDef_GetSubDef_007661c0()
{
    g_lastSubDefFor = g_current;
    ++g_aug[g_current].RefCount;
    return &g_aug[g_current];
}

int main()
{
    g_aug[1].DamageMultiplier = 2.0f;
    g_aug[2].DamageMultiplier = 0.5f;
    g_aug[3].DamageMultiplier = 3.0f;
    for (unsigned int k = 0; k < 4; ++k) { g_aug[k].RefCount = 0; g_thing[k].RefCount = 0; }
    long data[6] = { 1, 0, 3, -2, 2, 3 };
    std::vector<long> v;
    v._M_start = data; v._M_finish = data + 6; v._M_end_of_storage = data + 6;

    int ok = 1;
    float m = CTCObjectAugmentations::GetDamageMultiplierFromAugmentationSet(v);
    if (fabs(m - 2.0f * 3.0f * 0.5f * 3.0f) > 1e-5f) ok = 0;
    if (g_released != 8) ok = 0;                        // 4 thing defs + 4 augmentation defs back to zero
    for (unsigned int k = 0; k < 4; ++k)
        if (g_aug[k].RefCount != 0 || g_thing[k].RefCount != 0) ok = 0;
    v._M_finish = data;                                 // empty set
    if (fabs(CTCObjectAugmentations::GetDamageMultiplierFromAugmentationSet(v) - 1.0f) > 1e-6f) ok = 0;
    long negatives[2] = { 0, -5 };
    v._M_start = negatives; v._M_finish = negatives + 2;
    if (fabs(CTCObjectAugmentations::GetDamageMultiplierFromAugmentationSet(v) - 1.0f) > 1e-6f) ok = 0;
    if (g_released != 8) ok = 0;

    printf(ok ? "007661c0_TEST PASS\n" : "007661c0_TEST FAIL\n");
    return ok ? 0 : 1;
}