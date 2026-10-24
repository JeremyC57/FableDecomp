// Behaviour test for 0x00768010 CTCObjectAugmentations::OnInitialActivate: on a fresh object each positive initial
// augmentation def goes into the next free slot (non-positive entries are skipped and leave the slot free; when no
// slot is free nothing is placed); a save-loaded object places nothing. Particles and sounds are set up either way.
#include <stdio.h>
#include <string.h>
#ifndef _MSC_VER
#define __fastcall
#endif
struct CThing;

struct CDefClassBase {
    virtual void Destroy() const;
    virtual void Release() const;                // slot 1
    mutable long RefCount;                       // +0x04
};

template <class T> struct CDefPointer {
    T* Object;
    T* operator->() const { return Object; }
};

struct CThingBaseDef : CDefClassBase {
};

class CDefinitionManager {
public:
    bool GetDef(long index, CDefPointer<const CThingBaseDef>* out) const;   // 0x0042AF3C (GetDef<CThingBaseDef>)
};

class CGameDefinitionManager : public CDefinitionManager {
public:
    static CGameDefinitionManager& __fastcall Get();   // 0x0044C6B0
};

namespace _STL {
template <class T> class vector {
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    T* begin() { return _M_start; }
    T* end() { return _M_finish; }
    const T* begin() const { return _M_start; }
    const T* end() const { return _M_finish; }
    unsigned int size() const { return (unsigned int)(end() - begin()); }
    T& operator[](unsigned int n) { return *(begin() + n); }
    const T& operator[](unsigned int n) const { return *(begin() + n); }
};
}

struct CObjectAugmentationsDef : CDefClassBase {
    unsigned char _pad08[0x28 - 0x08];
    long NumberOfSlots;                          // +0x28
    long MaximumNumberOfSlots;                   // +0x2C
    unsigned char AugmentationParticles[0x0c];   // +0x30 map<EObjectAugmentationType, CObjectAugmentationParticleSet>
    unsigned long EmptySlotName;                 // +0x3C
    _STL::vector<long> InitialAugmentations;     // +0x40
    const _STL::vector<long>& GetInitialAugmentations() const { return InitialAugmentations; }
};

class CTCObjectAugmentations {
public:
    virtual void v00();
    CThing* ParentThing;                                             // +0x04 (CTCBase)
    bool ToBeRemoved;                                                // +0x08
    bool IsInGlobalUpdateTCs;                                        // +0x09
    CDefPointer<const CObjectAugmentationsDef> PDef;                 // +0x0C
    bool LoadedFromSaveGame;                                         // +0x10
    _STL::vector<CDefPointer<const CThingBaseDef> > Augmentations;  // +0x14
    long GetFreeSlot() const
    {
        for (unsigned int i = 0; i < Augmentations.size(); ++i) {
            if (Augmentations[i].Object == 0)
                return i;
        }
        return -1;
    }
    void AttachAugmentationParticles();                              // 0x007677E0
    void SetupAugmentationSounds();                                  // 0x00766C00
    void OnInitialActivate();
};

void CTCObjectAugmentations::OnInitialActivate()
{
    if (!LoadedFromSaveGame) {
        for (unsigned int i = 0; i < PDef->GetInitialAugmentations().size(); ++i) {
            long slot = GetFreeSlot();
            if (slot >= 0) {
                if (PDef->GetInitialAugmentations()[i] > 0)
                    CGameDefinitionManager::Get().GetDef(PDef->GetInitialAugmentations()[i], &Augmentations[slot]);
            }
        }
    }
    AttachAugmentationParticles();
    SetupAugmentationSounds();
}


static CThingBaseDef g_defs[16];
static int g_getDef = 0, g_particles = 0, g_sounds = 0;
void CDefClassBase::Destroy() const { }
void CDefClassBase::Release() const { }
void CTCObjectAugmentations::v00() { }
static CGameDefinitionManager g_mgr;
CGameDefinitionManager& __fastcall CGameDefinitionManager::Get() { return g_mgr; }
bool CDefinitionManager::GetDef(long index, CDefPointer<const CThingBaseDef>* out) const
{ ++g_getDef; out->Object = &g_defs[index]; return true; }
void CTCObjectAugmentations::AttachAugmentationParticles() { ++g_particles; }
void CTCObjectAugmentations::SetupAugmentationSounds() { ++g_sounds; }

int main()
{
    static CObjectAugmentationsDef def;
    long initial[5] = { 5, 0, 7, -1, 9 };
    def.InitialAugmentations._M_start = initial;
    def.InitialAugmentations._M_finish = initial + 5;
    def.InitialAugmentations._M_end_of_storage = initial + 5;
    CDefPointer<const CThingBaseDef> slots[4];
    for (unsigned int k = 0; k < 4; ++k) slots[k].Object = 0;
    slots[1].Object = &g_defs[1];                       // already filled

    CTCObjectAugmentations t;
    t.PDef.Object = &def;
    t.LoadedFromSaveGame = false;
    t.Augmentations._M_start = slots;
    t.Augmentations._M_finish = slots + 4;
    t.Augmentations._M_end_of_storage = slots + 4;

    int ok = 1;
    t.OnInitialActivate();
    if (g_getDef != 3 || g_particles != 1 || g_sounds != 1) ok = 0;
    if (slots[0].Object != &g_defs[5] || slots[1].Object != &g_defs[1] || slots[2].Object != &g_defs[7]
        || slots[3].Object != &g_defs[9]) ok = 0;

    t.OnInitialActivate();                              // every slot full: nothing more is placed
    if (g_getDef != 3 || g_particles != 2 || g_sounds != 2) ok = 0;

    slots[2].Object = 0;
    t.LoadedFromSaveGame = true;                        // save-loaded: no placement even with a free slot
    t.OnInitialActivate();
    if (g_getDef != 3 || slots[2].Object != 0 || g_particles != 3 || g_sounds != 3) ok = 0;

    printf(ok ? "00768010_TEST PASS\n" : "00768010_TEST FAIL\n");
    return ok ? 0 : 1;
}