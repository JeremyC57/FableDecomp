// CTCObjectAugmentations::GetDamageMultiplierFromAugmentationSet(const std::vector<long>& augmentations) -- product of the DamageMultiplier of every augmentation def named by a positive def index (1.0 with none)
// Grade: RELOCATION_MATCH (near-miss retry agent901) -- the element is indexed twice (`if (augmentations[i] > 0)` ... `GetDef(augmentations[i], &def)`, no index local) through STLport's `*(begin() + n)` operator[]; together (neither alone) they give retail's `mov ecx,[eax+esi*4] ... mov eax,ecx ; push eax` with the index in esi and the def in edi
// Leaf: ?GetDamageMultiplierFromAugmentationSet@CTCObjectAugmentations@@SIMABV?$vector@J@std@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007661C0, 158 bytes (+int3 pad to 160), plain `ret` (static __fastcall:
//   the vector reference in ecx, the float in st0).
// Identity: Ego_r.pdb ?GetDamageMultiplierFromAugmentationSet@CTCObjectAugmentations@@SIMABV?$vector@JV?$allocator@J@std@@@std@@@Z
//   (static, __fastcall); the catalog's name is right without the class.
// Callees: CGameDefinitionManager::Get() @ 0x0044C6B0 (static __fastcall, returns the manager by reference),
//   CDefinitionManager::GetDef<CThingBaseDef>(long, CDefPointer<const T>*) const @ 0x0042AF3C,
//   CDefClassBase::GetSubDef<CAugmentationDef>(CDefPointer<const T>*) const @ 0x0042B248, the defs' slot-1 release.
// Shape: the instance twin of the landed GetDamageMultiplier 0x00766140 over def indices: `for (unsigned i = 0; i <
//   size(); ++i)` (size re-read each pass, `jb`), `if (augmentations[i] > 0)` (`jle`); GetDef's arguments are pushed before the
//   Get() call yields the manager (`mov ecx,eax`), each def-pointer local zeroed after its address is pushed; the thing
//   def is read once into edi (the same-TU GetDef/GetSubDef stand-ins write only their out slot, so the address does
//   not escape); the augmentation def's release has no null test (it was just dereferenced), the thing def's keeps it.
// Layout: CAugmentationDef DamageMultiplier +0x34 (as the landed 0x00766140); def reference count +4, release slot 1.
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

// Stand-in bodies (both are out of line in retail): they write the out slot only, so VC7.1 knows the def pointers'
// addresses do not escape the calls (the landed 0x00766140 / 0x00848730 idiom).
#pragma auto_inline(off)
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
#pragma auto_inline(on)

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