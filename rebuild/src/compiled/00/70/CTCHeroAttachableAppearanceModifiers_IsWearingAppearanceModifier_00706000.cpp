// CTCHeroAttachableAppearanceModifiers::IsWearingAppearanceModifier(long, EHeroAttachableAppearanceModifierType) const -- true when the modifier list of the given type holds an entry with the given def index
// Leaf: IsWearingAppearanceModifier
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00706000, length 53 bytes (+ int3 padding).
// Shape: `AppearanceModifiers[type]` is a 12-byte element (`lea eax,[eax+eax*2] ; lea ecx,[ecx+eax*4]`), first/last loaded
// once, a plain pointer loop over 8-byte CModifierEntry records comparing DefIndex (the argument in edx, loaded after the
// empty test), the aligned loop head (`lea ebx,[ebx]` filler), two exits `xor al,al ; ret 8` / `mov al,1 ; ret 8`.
// Model: CTCBase (vptr, ParentThing +4, two bools) then a secondary vptr at +0xc, FrameLastChanged +0x10, the bool:1 bitfield
// group Changed / RecreatePrimitives / AppearanceModifiersLoaded at +0x14, PrimHandles +0x18 (vector), HiddenMaterialNames +0x24
// (set), AppearanceModifiers +0x30 (vector< vector<CModifierEntry{DefIndex, Rendered}> >), Graphics +0x3c -- the Ego_r.pdb member
// order with VC7.1 release containers of 0xc bytes. engine/CTCHeroAttachableAppearanceModifiers.h carries the PDB debug-STL
// layout (0x10-byte containers: AppearanceModifiers at +0x38), which disagrees with the retail offsets, so the class is modelled
// locally; engine/CTCBase.h is a flattened POD (vptr as a data member), so the polymorphic CTCBase is local too.
// CWorld_Calls: engine/CWorld.h holds the PDB data layout of CWorld; only its member call(s) are needed here, so the
// callee is declared on a separate call-only struct.
class CThing;
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

class CTCBase { public:
    virtual void v00();
    CThing* ParentThing;
    bool ToBeRemoved;
    bool IsInGlobalUpdateTCs;
    CWorld_Calls& GetWorld() const;                    // 0x00686A80
};

typedef long EHeroAttachableAppearanceModifierType;

class CTCHeroAttachableAppearanceModifiers : public CTCBase { public:
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
bool CTCHeroAttachableAppearanceModifiers::IsWearingAppearanceModifier(long defIndex, EHeroAttachableAppearanceModifierType type) const
{
    const CModifierVector& list = AppearanceModifiers[type];
    for (const CModifierEntry* it = list._Myfirst; it != list._Mylast; ++it)
        if (it->DefIndex == defIndex)
            return true;
    return false;
}