// CTCHeroAttachableAppearanceModifiers::FrameUpdate() -- per-frame: when the Changed bit is set, clear it and push the changes to the graphics (UploadChangesToGraphics inlined)
// Leaf: FrameUpdate
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00707AD0, length 49 bytes (+ int3 padding). Vtable 0x01264F0C slot 9
// (referenced only from the vtable at 0x01264F30); Ego_r.pdb ?FrameUpdate@CTCHeroAttachableAppearanceModifiers@@UAEXXZ.
// Manifest label "EditFrameUpdate" is a guess shared with 0x00707930.
// Callees: as UploadChangesToGraphics 0x00707930 (the same four calls: 0x00706040, 0x00706510, 0x007070D0, 0x004BF9E0).
// Shape: byte-identical to UploadChangesToGraphics apart from the relative call displacements -- the virtual FrameUpdate is
// written with the same body (an inline call of a non-inline member would stay a call, so the body is spelled out).
// Model: CTCBase (vptr, ParentThing +4, two bools) then a secondary vptr at +0xc, FrameLastChanged +0x10, the bool:1 bitfield
// group Changed / RecreatePrimitives / AppearanceModifiersLoaded at +0x14, PrimHandles +0x18 (vector), HiddenMaterialNames +0x24
// (set), AppearanceModifiers +0x30 (vector< vector<CModifierEntry{DefIndex, Rendered}> >), Graphics +0x3c -- the Ego_r.pdb member
// order with VC7.1 release containers of 0xc bytes. engine/CTCHeroAttachableAppearanceModifiers.h carries the PDB debug-STL
// layout (0x10-byte containers: AppearanceModifiers at +0x38), which disagrees with the retail offsets, so the class is modelled
// locally; engine/CTCBase.h is a flattened POD (vptr as a data member), so the polymorphic CTCBase is local too.
// CWorld_Calls: engine/CWorld.h holds the PDB data layout of CWorld; only its member call(s) are needed here, so the
// callee is declared on a separate call-only struct.
namespace NGraphicAppearance {
struct CTCGraphicAppearance {
    void SetAsChanged(unsigned long flags);      // 0x004BF9E0
};
}
struct CThing {
    unsigned char _pad00[0x64];
    NGraphicAppearance::CTCGraphicAppearance* PTCGraphicAppearance;   // +0x64
};
struct CWorld_Calls {
    static long __fastcall GetFrame();          // 0x0049D870
};

struct CModifierEntry {
    long DefIndex;
    bool Rendered;
};

template<class T> inline T* CopyRange(T* first, T* last, T* result)
{
    for (long n = last - first; n > 0; --n) {
        *result = *first;
        ++first;
        ++result;
    }
    return result;
}

struct CModifierVector {
    CModifierEntry* _Myfirst;
    CModifierEntry* _Mylast;
    CModifierEntry* _Myend;
    CModifierEntry* begin() { return _Myfirst; }
    CModifierEntry* end() { return _Mylast; }
    CModifierEntry* erase(CModifierEntry* first, CModifierEntry* last)
    {
        CModifierEntry* kept = CopyRange(last, _Mylast, first);
        _Mylast = kept;
        return first;
    }
    void clear() { erase(begin(), end()); }
};

struct CModifierVectorVector {
    CModifierVector* _Myfirst;
    CModifierVector* _Mylast;
    CModifierVector* _Myend;
    CModifierVector& operator[](long i) { return _Myfirst[i]; }
    const CModifierVector& operator[](long i) const { return _Myfirst[i]; }
    CModifierVectorVector& operator=(const CModifierVectorVector& other);   // 0x00709660
};

struct CTCBase {
    virtual void v00();
    CThing* ParentThing;
    bool ToBeRemoved;
    bool IsInGlobalUpdateTCs;
    CWorld_Calls& GetWorld() const;                    // 0x00686A80
    virtual void FrameUpdate();
    virtual void Copy(const CTCBase& other);
};

typedef long EHeroAttachableAppearanceModifierType;

struct CTCHeroAttachableAppearanceModifiers : CTCBase {
    void* __vftable_0c;                          // +0x0c secondary vptr
    unsigned long FrameLastChanged;              // +0x10
    bool Changed : 1;                            // +0x14 bit 0
    bool RecreatePrimitives : 1;                 // +0x14 bit 1
    bool AppearanceModifiersLoaded : 1;          // +0x14 bit 2
    unsigned char PrimHandles[0xc];              // +0x18
    unsigned char HiddenMaterialNames[0xc];      // +0x24
    CModifierVectorVector AppearanceModifiers;   // +0x30
    unsigned char Graphics[0xc];                 // +0x3c

    void SetAsChanged()
    {
        Changed = true;
        FrameLastChanged = GetWorld().GetFrame();
    }
    bool IsWearingAppearanceModifier(long defIndex, EHeroAttachableAppearanceModifierType type) const;
    void RemoveAllAppearanceModifiers(EHeroAttachableAppearanceModifierType type);
    void UpdateHiddenModifiers();                // 0x00706040
    void UpdateHiddenMaterials();                // 0x00706510
    void UpdateGraphics();                       // 0x007070D0
    void UploadChangesToGraphics();
    virtual void FrameUpdate();
    virtual void Copy(const CTCBase& other);
};

void CTCHeroAttachableAppearanceModifiers::FrameUpdate()
{
    if (Changed) {
        Changed = false;
        UpdateHiddenModifiers();
        UpdateHiddenMaterials();
        UpdateGraphics();
        ParentThing->PTCGraphicAppearance->SetAsChanged(15);
    }
}