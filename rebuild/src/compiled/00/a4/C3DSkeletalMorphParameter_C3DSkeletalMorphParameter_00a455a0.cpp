// C3DSkeletalMorphParameter::C3DSkeletalMorphParameter(const C3DSkeletalMorphTarget& target) -- CBase(), vtable, a new shared block whose positive target is a copy of `target` (unsigned), value 0, no owning morph
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A455A0, length 141 bytes.
// Leaf: ??0C3DSkeletalMorphParameter
// Callees: 0x0099A2F0 CBaseClassNonCopyable::CBaseClassNonCopyable, 0x00BFEA1A operator new(0x24), 0x0099E4B0 CCharString() (x2, the two target
// names), 0x00A47060 CCountedPointer<CShared>::Reset(CShared*), 0x0099EFB0 CCharString::operator=, 0x00A46790
// CArray<C3DSkeletalBoneMorph>::operator= (propagated label CCombatWheel::operator_assign).
// Vtable 0x0129D490 = C3DSkeletalMorphParameter (rtti-vtables.tsv).
// Layout (PDB, retail 12-byte CArray): C3DSkeletalMorphParameter {CurrentValue +4, SkeletalMorph +8, SharedData
// +0xc}; CShared (0x24) {PositiveTarget +0, NegativeTarget +0x10, SignedFlag +0x20}; C3DSkeletalMorphTarget
// (16 bytes) {Name +0, BoneMorphs +4}.
// Shape: SharedData is addressed once (`lea ebp,[edi+0Ch]`), default-constructed and Reset to the new block (the
// CShared constructor inlined: two targets, each a CCharString plus a zeroed CArray; SignedFlag left alone);
// the CArray must be the real STLport vector (_Vector_base + _STLP_alloc_proxy over an allocator temporary): with
// three plain zeroing words the second name's `lea ecx,[esi+10h]` is scheduled before the +8 store (the "24i" fix);
// the positive target is assigned through C3DSkeletalMorphTarget's inline memberwise operator= on the block read
// once, then SignedFlag, CurrentValue and SkeletalMorph are body assignments from the zero register.
class CCharString { public:
    char* m_pString;
    CCharString();                                       // 0x0099E4B0
    const CCharString& operator=(const CCharString& other);    // 0x0099EFB0
};
class C3DSkeletalBoneMorph;
struct SAllocator { SAllocator() {} };                   // empty allocator (user constructor: no zero fill of the temporary)
template<class T> struct SAllocProxy : SAllocator {      // STLport _STLP_alloc_proxy: the end-of-storage pointer over the allocator
    T* m_data;
    SAllocProxy(const SAllocator& a, T* data) : SAllocator(a), m_data(data) {}
};
template<class T> struct CVectorBase {                   // STLport _Vector_base
    T*             m_begin;
    T*             m_end;
    SAllocProxy<T> m_capacity;
    CVectorBase(const SAllocator& a) : m_begin(0), m_end(0), m_capacity(a, 0) {}
};
template<class T> class CArray : public CVectorBase<T> { public:       // retail CArray<T> = STLport vector (12 bytes)
    CArray() : CVectorBase<T>(SAllocator()) { }
    CArray& operator=(const CArray& other);              // 0x00A46790
};
class C3DSkeletalMorphTarget { public:                          // 16 bytes (retail)
    CCharString                  Name;                   // +0x00
    CArray<C3DSkeletalBoneMorph> BoneMorphs;             // +0x04
    C3DSkeletalMorphTarget& operator=(const C3DSkeletalMorphTarget& other)
    {
        Name = other.Name;
        BoneMorphs = other.BoneMorphs;
        return *this;
    }
};
template<class T> class CCountedPointer { public:
    T*    Data;                                          // +0
    void* Info;                                          // +4
    CCountedPointer() : Data(0), Info(0) { }
    void Reset(T* object);                               // 0x00A47060
    T* operator->() const { return Data; }
};

class CBaseClassNonCopyable {
protected:
    virtual ~CBaseClassNonCopyable();   // 0x0099A300
    CBaseClassNonCopyable();                                             // 0x0099A2F0
};

class C3DSkeletalMorph;
class C3DSkeletalMorphParameter : public CBaseClassNonCopyable { public:
    class CShared { public:                                     // 0x24 bytes
        C3DSkeletalMorphTarget PositiveTarget;           // +0x00
        C3DSkeletalMorphTarget NegativeTarget;           // +0x10
        bool                   SignedFlag;               // +0x20
    };
    float                     CurrentValue;              // +0x04
    C3DSkeletalMorph*         SkeletalMorph;             // +0x08
    CCountedPointer<CShared>  SharedData;                // +0x0c   sizeof == 0x14
    C3DSkeletalMorphParameter(const C3DSkeletalMorphTarget& target);
};

C3DSkeletalMorphParameter::C3DSkeletalMorphParameter(const C3DSkeletalMorphTarget& target)
    : CBaseClassNonCopyable()
{
    SharedData.Reset(new CShared);
    SharedData->PositiveTarget = target;
    SharedData->SignedFlag = false;
    CurrentValue = 0.0f;
    SkeletalMorph = 0;
}