// C3DSkeletalMorphParameter::C3DSkeletalMorphParameter(const C3DSkeletalMorphTarget& positive, const C3DSkeletalMorphTarget& negative) -- create the shared target pair, copy both targets in, mark it valid and start at value 0
// Leaf: ??0C3DSkeletalMorphParameter
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A454F0, length 172 bytes (`ret 8`). Vtable 0x0129D490 (rtti-vtables:
// C3DSkeletalMorphParameter slot 0 = 0x00A46420). Animation/mesh layer.
// Callees: CBase::CBase @ 0x0099A2F0 (polymorphic root), operator new @ 0x00BFEA1A (0x24 bytes = retail CShared),
// CCharString::CCharString() @ 0x0099E4B0, CCountedPointer<CShared>::Reset(CShared*) @ 0x00A47060 (manifest names the
// CIPhysicsFacesSegmentBase instantiation), CCharString::operator= @ 0x0099EFB0, CArray<C3DSkeletalBoneMorph>::operator=
// @ 0x00A46790 (manifest "CCombatWheel::operator_assign" is propagated).
// Retail offsets: CurrentValue +0x04, SkeletalMorph +0x08, SharedData +0x0c {Object, block}; CShared = PositiveTarget
// +0x00, NegativeTarget +0x10 (each 0x10: Name + 12-byte bone-morph array), Valid +0x20.
// Shape: the bone-morph arrays use the STLport `_Vector_base` + `_STLP_alloc_proxy` model (fixes the `lea` of the
// second target before its last zero store); one `xor ebp,ebp` zero for every zero store; the counted pointer zeroes inline before the allocation; the
// target assignment is an inline memberwise operator= (Name, then BoneMorphs); CurrentValue/SkeletalMorph are body
// assignments after the copies.
struct CCharString
{
    void* PStringData;
    CCharString();                                  // 0x0099E4B0
    CCharString& operator=(const CCharString& other);   // 0x0099EFB0
};

struct C3DSkeletalBoneMorph;

template <class T> struct _STLP_alloc_proxy        // STLport allocator proxy holding end_of_storage
{
    T* _M_data;
    _STLP_alloc_proxy(T* p) : _M_data(p) { }
};

template <class T> struct _Vector_base
{
    T* _M_start;
    T* _M_finish;
    _STLP_alloc_proxy<T> _M_end_of_storage;
    _Vector_base() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
};

struct CBoneMorphArray : _Vector_base<C3DSkeletalBoneMorph>   // CArray<C3DSkeletalBoneMorph>
{
    CBoneMorphArray& operator=(const CBoneMorphArray& other);   // 0x00A46790
};

struct C3DSkeletalMorphTarget                       // retail 0x10 bytes
{
    CCharString Name;                               // +0x00
    CBoneMorphArray BoneMorphs;                     // +0x04
    __forceinline C3DSkeletalMorphTarget& operator=(const C3DSkeletalMorphTarget& other)
    {
        Name = other.Name;
        BoneMorphs = other.BoneMorphs;
        return *this;
    }
};

struct CBase
{
    virtual void v00();
    CBase();                                        // 0x0099A2F0
};

template <class T>
struct CCountedPointer
{
    T* Object;                                      // +0x00
    void* Block;                                    // +0x04
    CCountedPointer() : Object(0), Block(0) { }
    void Reset(T* object);                          // 0x00A47060
    T* operator->() const { return Object; }
};

struct C3DSkeletalMorph;

struct C3DSkeletalMorphParameter : CBase            // vtable 0x0129D490
{
    struct CShared                                  // retail 0x24 bytes
    {
        C3DSkeletalMorphTarget PositiveTarget;      // +0x00
        C3DSkeletalMorphTarget NegativeTarget;      // +0x10
        bool Valid;                                 // +0x20
    };
    float CurrentValue;                             // +0x04
    C3DSkeletalMorph* SkeletalMorph;                // +0x08
    CCountedPointer<CShared> SharedData;            // +0x0c
    C3DSkeletalMorphParameter(const C3DSkeletalMorphTarget& positive, const C3DSkeletalMorphTarget& negative);
    virtual void v00();
};

C3DSkeletalMorphParameter::C3DSkeletalMorphParameter(const C3DSkeletalMorphTarget& positive,
                                                     const C3DSkeletalMorphTarget& negative)
{
    SharedData.Reset(new CShared);
    SharedData->PositiveTarget = positive;
    SharedData->NegativeTarget = negative;
    SharedData->Valid = true;
    CurrentValue = 0;
    SkeletalMorph = 0;
}