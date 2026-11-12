// Behaviour test for CHitParameters::SetWeapon (0x00868CE0): a copy of the model (weapon pointer, component presence bit,
// augmentation index list, def lookup with sub-def, type list) exercising the same logic.
#include <cstdio>
#include <vector>
#include <map>
enum EObjectAugmentationType { OBJECT_AUGMENTATION_NULL = 0, OBJECT_AUGMENTATION_FLAME = 3, OBJECT_AUGMENTATION_SILVER = 7 };
class CAugmentationDef { public: long RefCount; EObjectAugmentationType Type; };
class CThingBaseDef { public: long RefCount; CAugmentationDef* PAugmentation; };   // null: the def has no CAugmentationDef sub-def
static int g_releases = 0;
template<class T> class CDefPointer { public:
    T* P;
    CDefPointer() : P(0) { }
    ~CDefPointer() { if (P && --P->RefCount == 0) ++g_releases; }
    void Set(T* p) { P = p; if (p) ++p->RefCount; }
    T* operator->() const { return P; }
};
class CDefinitionManager { public:
    std::map<long, CThingBaseDef*> Defs;
    bool GetDef(long index, CDefPointer<CThingBaseDef>* out) const
    {
        if (index > 0) {
            std::map<long, CThingBaseDef*>::const_iterator it = Defs.find(index);
            if (it != Defs.end()) { out->Set(it->second); return true; }
        }
        return false;
    }
};
static bool GetSubDef(const CThingBaseDef* def, CDefPointer<CAugmentationDef>* out)
{
    if (def->PAugmentation) { out->Set(def->PAugmentation); return true; }
    return false;
}
class CTCObjectAugmentations { public:
    std::vector<long> Slots;
    void GetAugmentations(std::vector<long>& out) const { for (unsigned i = 0; i < Slots.size(); ++i) out.push_back(Slots[i]); }
};
class CThing { public:
    unsigned long UsingComponents[9];
    CTCObjectAugmentations* PAugmentations;
    const CDefinitionManager* PManager;
    bool PeekTC(const CTCObjectAugmentations** ptc) const
    {
        if (UsingComponents[191 >> 5] & (1UL << (191 & 31))) { *ptc = PAugmentations; return true; }
        return false;
    }
    const CDefinitionManager& PeekDefinitionManager() const { return *PManager; }
};
class CHitParameters { public:
    std::vector<EObjectAugmentationType> Augmentations;
    CThing* PWeapon;
    void SetWeapon(CThing& weapon)
    {
        PWeapon = &weapon;
        const CTCObjectAugmentations* ptc;
        if (PWeapon->PeekTC(&ptc)) {
            std::vector<long> augmentations;
            ptc->GetAugmentations(augmentations);
            for (std::vector<long>::iterator it = augmentations.begin(); it != augmentations.end(); ++it) {
                long index = *it;
                CDefPointer<CThingBaseDef> pdef;
                if (weapon.PeekDefinitionManager().GetDef(index, &pdef)) {
                    CDefPointer<CAugmentationDef> paug;
                    if (GetSubDef(pdef.P, &paug))
                        Augmentations.push_back(paug->Type);
                }
            }
        }
    }
};
int main()
{
    bool ok = true;
    CAugmentationDef flame = { 1, OBJECT_AUGMENTATION_FLAME }, silver = { 1, OBJECT_AUGMENTATION_SILVER };
    CThingBaseDef defFlame = { 1, &flame }, defPlain = { 1, 0 }, defSilver = { 1, &silver };
    CDefinitionManager manager;
    manager.Defs[10] = &defFlame; manager.Defs[20] = &defPlain; manager.Defs[30] = &defSilver;
    CTCObjectAugmentations tc;
    tc.Slots.push_back(10); tc.Slots.push_back(0); tc.Slots.push_back(20); tc.Slots.push_back(99); tc.Slots.push_back(30);
    // 1: a weapon without the augmentations component only becomes the weapon.
    CThing plain = { { 0 }, &tc, &manager };
    plain.UsingComponents[5] = 0x7FFFFFFF;                        // every bit of the word but 191
    CHitParameters a; a.PWeapon = 0;
    a.SetWeapon(plain);
    if (a.PWeapon != &plain || !a.Augmentations.empty()) ok = false;
    // 2: with the component: index 10 -> flame, 0 skipped (not a def index), 20 has no augmentation sub-def, 99 unknown, 30 -> silver.
    CThing sword = { { 0 }, &tc, &manager };
    sword.UsingComponents[5] = 0x80000000UL;
    CHitParameters b; b.PWeapon = 0;
    b.SetWeapon(sword);
    if (b.PWeapon != &sword || b.Augmentations.size() != 2) ok = false;
    else if (b.Augmentations[0] != OBJECT_AUGMENTATION_FLAME || b.Augmentations[1] != OBJECT_AUGMENTATION_SILVER) ok = false;
    // every def reference taken was given back, none dropped to zero
    if (flame.RefCount != 1 || silver.RefCount != 1 || defFlame.RefCount != 1 || defPlain.RefCount != 1 || defSilver.RefCount != 1 || g_releases != 0) ok = false;
    // 3: a second call appends (the list is not cleared) and replaces the weapon.
    b.SetWeapon(sword);
    if (b.Augmentations.size() != 4 || b.Augmentations[3] != OBJECT_AUGMENTATION_SILVER) ok = false;
    b.SetWeapon(plain);
    if (b.PWeapon != &plain || b.Augmentations.size() != 4) ok = false;
    if (!ok) { std::printf("00868ce0_TEST FAIL\n"); return 1; }
    std::printf("00868ce0_TEST PASS\n");
    return 0;
}