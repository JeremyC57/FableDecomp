// CTCObjectAugmentations::GetAugmentationName(long defIndex) -- the display name of an augmentation def (its Name text from the game text bank), or "Missing augmentation name!" when the def has no text
// Leaf: ?GetAugmentationName@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00765F90, 186 bytes (+int3 pad to 192), plain `ret` (static __fastcall:
//   the hidden CWideString result in ecx, the def index in edx; eax = result).
// Identity: Ego_r.pdb ?GetAugmentationName@CTCObjectAugmentations@@SI?AVCWideString@@J@Z. Callers 0x005C6E55,
//   0x005C736F, 0x005C939D (inventory UI).
// Callees: CGameDefinitionManager::Get() @ 0x0044C6B0, CDefinitionManager::GetDef<CThingBaseDef> @ 0x0042AF3C,
//   CDefClassBase::GetSubDef<CAugmentationDef> @ 0x0042B248, NGameText::CDataBank::GetPGroupEntry(unsigned long, bool)
//   const @ 0x009C9070 (the bank at g_pMainGameComponent 0x013B86A0 +0x14), the group entry's GetText() @ 0x009C7A40,
//   CWideString::CWideString(const CWideString&) @ 0x0099B720 on g_CWideString_013bad98 (L"Missing augmentation name!",
//   dynamic initializer 0x0121EE90 / atexit 0x01229AD0).
// Shape: the def lookups as GetDamageMultiplierFromAugmentationSet 0x007661C0 (GetDef's arguments pushed before Get(),
//   each def-pointer local zeroed after its address is pushed, the thing def read once into edi); `if (Name > 0)`
//   (unsigned, `jbe`) reads the text bank into a block local BEFORE the argument pushes (`mov edx,[g] ; mov ecx,
//   [edx+14h]`; the Name re-read inline, not a named local, keeps it in eax for the push) and returns
//   GetPGroupEntry(Name, true)->GetText() straight into the result (the entry's CIVCountedPointer temporary takes the
//   dead augmentation slot and is released with `Object = 0` through the landed CItem 0x005BD8A0 DecRef/GetRefCount
//   helpers), else copies the missing-name string; both exits share the augmentation release (no null test: it was
//   dereferenced) and the thing def release (null-tested).
// Layout: CAugmentationDef Name (text index) +0x28 (as the landed 0x00766140 model); def reference count +4, release
//   slot 1; CGroupEntry: vptr, IVRefCount +4 (second base), Delete slot 1.
class CWideString { public:
    void* PStringData;
    CWideString(const CWideString& other);       // 0x0099B720
};

extern CWideString g_CWideString_013bad98;       // 0x013BAD98 L"Missing augmentation name!"

class CDefClassBase { public:
    virtual void Destroy() const;
    virtual void Release() const;                // slot 1
    mutable long RefCount;                       // +0x04
    void ReleaseReference() const
    {
        if (--RefCount == 0)
            Release();
    }
};

template <class T> class CDefPointer { public:
    T* Object;
    CDefPointer() : Object(0) { }
    ~CDefPointer()
    {
        if (Object)
            Object->ReleaseReference();
    }
    T* operator->() const { return Object; }
};

class CAugmentationDef : public CDefClassBase { public:
    unsigned char _pad08[0x28 - 0x08];
    unsigned long Name;                          // +0x28
    long  Type;                                  // +0x2c
    unsigned long InventoryGraphic;              // +0x30
    float DamageMultiplier;                      // +0x34
    float ExperienceMultiplier;                  // +0x38
};

void* FindDef_GetSubDef_00765f90();              // stand-in callees of the stand-in bodies below
void* FindDef_GetDef_00765f90(long index);

class CThingBaseDef : public CDefClassBase { public:
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
    void* def = FindDef_GetSubDef_00765f90();
    if (!def)
        return false;
    *(void**)out = def;
    return true;
}

__declspec(noinline) bool CDefinitionManager::GetDef(long index, CDefPointer<const CThingBaseDef>* out) const
{
    void* def = FindDef_GetDef_00765f90(index);
    if (!def)
        return false;
    *(void**)out = def;
    return true;
}
#pragma auto_inline(on)

struct CIVCountedPointeeVirtual {
    virtual void Destroy(int flags);             // slot 0
    virtual void Delete();                       // slot 1
};
struct CIVRefCounted {                           // second base: the count at +4 behind the vptr
    unsigned long IVRefCount;
    __forceinline void DecRef() { --IVRefCount; }
    __forceinline unsigned long GetRefCount() const { return IVRefCount; }
};
class CIVCountedPointeeBase : public CIVCountedPointeeVirtual, public CIVRefCounted { public:
    __forceinline void ReleaseTemporary()
    {
        DecRef();
        if (GetRefCount() == 0)
            Delete();
    }
};
template <class T> class CIVCountedPointer { public:
    T* Object;
    ~CIVCountedPointer()
    {
        if (Object) {
            Object->ReleaseTemporary();
            Object = 0;
        }
    }
    T* operator->() const { return Object; }
};

namespace NGameText {
class CGroupEntry : public CIVCountedPointeeBase { public:
    CWideString GetText() const;                 // 0x009C7A40
};
class CDataBank { public:
    CIVCountedPointer<CGroupEntry> GetPGroupEntry(unsigned long index, bool load) const;   // 0x009C9070
};
}

class CMainGameComponent { public:
    unsigned char _pad00[0x14];
    NGameText::CDataBank* TextBank;              // +0x14
};
extern CMainGameComponent* g_pMainGameComponent; // 0x013B86A0

class CTCObjectAugmentations {
public:
    static CWideString __fastcall GetAugmentationName(long defIndex);
};

CWideString __fastcall CTCObjectAugmentations::GetAugmentationName(long defIndex)
{
    CDefPointer<const CThingBaseDef> def;
    CGameDefinitionManager::Get().GetDef(defIndex, &def);
    CDefPointer<const CAugmentationDef> augmentation;
    def->GetSubDef(&augmentation);
    if (augmentation->Name > 0) {
        NGameText::CDataBank* bank = g_pMainGameComponent->TextBank;
        return bank->GetPGroupEntry(augmentation->Name, true)->GetText();
    }
    return g_CWideString_013bad98;
}