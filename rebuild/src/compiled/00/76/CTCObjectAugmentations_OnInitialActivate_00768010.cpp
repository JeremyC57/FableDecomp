// CTCObjectAugmentations::OnInitialActivate() -- on a fresh (not save-loaded) object, put each of the def's initial augmentations into a free slot; then attach the augmentation particles and set up the sounds
// Grade: RELOCATION_MATCH (near-miss retry agent901) -- STLport begin()/end() forms for size() and operator[] plus the initial-augmentation element indexed twice (no index local), as 0x007661C0; together they give retail's loop-end `mov ebp,[eax+40h] ; lea edx,[eax+40h] ; mov eax,[edx+4] ; sub eax,ebp`
// Leaf: ?OnInitialActivate@CTCObjectAugmentations@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00768010, 146 bytes, ends in a tail `jmp`.
// Identity: CTCObjectAugmentations vtable 0x0126A3A4 slot 6 (rtti-vtables.tsv); Ego_r.pdb
//   ?OnInitialActivate@CTCObjectAugmentations@@UAEXXZ. The catalog's "CTCObjectAugmentations_InitializeDrunkenness" is
//   an autoname.
// Callees: CGameDefinitionManager::Get() @ 0x0044C6B0, CDefinitionManager::GetDef<CThingBaseDef>(long,
//   CDefPointer<const T>*) const @ 0x0042AF3C (straight into the free slot), AttachAugmentationParticles() @ 0x007677E0,
//   SetupAugmentationSounds() @ 0x00766C00 (tail jump).
// Shape: `if (!LoadedFromSaveGame)` guards an index loop over PDef->InitialAugmentations (the def re-read through PDef
//   after every GetDef call, the vector's address CSE'd within a pass); GetFreeSlot() is inlined (the landed 0x00765D50
//   loop: size re-read each pass, the element walked by a pointer started at the first _M_start load, -1 when none),
//   then `slot >= 0` and `InitialAugmentations[i] > 0` (`jl`, `jle`) gate GetDef(InitialAugmentations[i], &Augmentations[slot]) with its arguments pushed
//   before Get() yields the manager.
// Layout: PDB CTCObjectAugmentations with retail 12-byte containers: PDef +0x0C, LoadedFromSaveGame +0x10,
//   Augmentations +0x14; CObjectAugmentationsDef (retail 12-byte map): AugmentationParticles +0x30, EmptySlotName +0x3C,
//   InitialAugmentations (vector<long>) +0x40.
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