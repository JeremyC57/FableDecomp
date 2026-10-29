// C3DSkeletalMorph::C3DSkeletalMorph() -- CBase(), this class's vtable, empty MorphParameters array and empty CombinedMorph pointer
// Leaf: ??0C3DSkeletalMorph@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A44E80, oracle length 35 bytes.
// Callees: CBaseClassNonCopyable::CBaseClassNonCopyable @ 0x0099A2F0 (landed). Vtable 0x0129D498 = C3DSkeletalMorph (rtti-vtables.tsv).
//
// Retail bytes:  push esi ; mov esi,ecx ; call CBaseClassNonCopyable::CBaseClassNonCopyable ; xor eax,eax ; mov [esi],offset vftable ; mov [esi+4],eax ;
//                mov [esi+8],eax ; mov [esi+0Ch],eax ; mov [esi+10h],eax ; mov [esi+14h],eax ; mov eax,esi ; pop esi ; ret
// PDB layout (struct_layouts_egor.tsv): +4 MorphParameters = CArray<CCountedPointer<C3DSkeletalMorphParameter>>,
// +0x10 CombinedMorph = CCountedPointer<C3DSkeletalMorphTarget>. The retail CArray is the 12-byte
// {first, last, end} vector (the PDB's 16-byte CArray is the debug build's), so the object is 0x18 bytes:
// five zero stores from one cleared register, all behind the vtable store because both members are
// sub-objects with inline zeroing constructors.
class CBaseClassNonCopyable {
protected:
    virtual ~CBaseClassNonCopyable();   // 0x0099A300
    CBaseClassNonCopyable();                               // 0x0099A2F0
};

struct CArray {                            // retail CArray<T>: {first, last, end}
    void* m_pFirst;
    void* m_pLast;
    void* m_pEnd;
    CArray() : m_pFirst(0), m_pLast(0), m_pEnd(0) { }
};

struct CCountedPointer {
    void* m_pObject;
    void* m_pCount;
    CCountedPointer() : m_pObject(0), m_pCount(0) { }
};

class C3DSkeletalMorph : public CBaseClassNonCopyable { public:
    CArray          MorphParameters;       // +0x4
    CCountedPointer CombinedMorph;         // +0x10
    C3DSkeletalMorph();
};

C3DSkeletalMorph::C3DSkeletalMorph()
    : CBaseClassNonCopyable(), MorphParameters(), CombinedMorph()
{
}