// CTCHeroAttachableAppearanceModifiers::RemoveAllAppearanceModifiers(EHeroAttachableAppearanceModifierType) -- clears the modifier list of the given type (VC6-era vector::erase(begin(), end())) and marks the TC changed
// Leaf: RemoveAllAppearanceModifiers
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00706640, length 86 bytes (+ int3 padding).
// Callees: CTCBase::GetWorld @ 0x00686A80, the world frame getter @ 0x0049D870.
// Shape: `clear()` is the old Dinkumware `erase(begin(), end())` with an inline element copy `copy(last, _Mylast, first)`:
// because last IS _Mylast the count is `mov ecx,eax ; sub ecx,eax` (always 0) but the 8-byte-record copy loop is still
// emitted (ebx pushed inside the guarded arm), then `_Mylast = kept`. Inline SetAsChanged() follows.
// SetAsChanged() is inline: `Changed = true` (`mov al,[esi+14h] ; or al,1 ; mov [esi+14h],al`), then FrameLastChanged =
// GetWorld().GetFrame() -- GetWorld @ 0x00686A80 (manifest "CViewBase::GetDisplayEngine" is propagated) is called for its object
// expression and its result discarded, because the frame getter @ 0x0049D870 is a static (`call` with nothing in ecx).
// Model: CTCBase (vptr, ParentThing +4, two bools) then a secondary vptr at +0xc, FrameLastChanged +0x10, the bool:1 bitfield
// group Changed / RecreatePrimitives / AppearanceModifiersLoaded at +0x14, PrimHandles +0x18 (vector), HiddenMaterialNames +0x24
// (set), AppearanceModifiers +0x30 (vector< vector<CModifierEntry{DefIndex, Rendered}> >), Graphics +0x3c -- the Ego_r.pdb member
// order with VC7.1 release containers of 0xc bytes. engine/CTCHeroAttachableAppearanceModifiers.h carries the PDB debug-STL
// layout (0x10-byte containers: AppearanceModifiers at +0x38), which disagrees with the retail offsets, so the class is modelled
// locally; engine/CTCBase.h is a flattened POD (vptr as a data member), so the polymorphic CTCBase is local too.
// CWorld_Calls: engine/CWorld.h holds the PDB data layout of CWorld; only its member call(s) are needed here, so the
// callee is declared on a separate call-only struct.
struct CThing;
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
};
void CTCHeroAttachableAppearanceModifiers::RemoveAllAppearanceModifiers(EHeroAttachableAppearanceModifierType type)
{
    AppearanceModifiers[type].clear();
    SetAsChanged();
}