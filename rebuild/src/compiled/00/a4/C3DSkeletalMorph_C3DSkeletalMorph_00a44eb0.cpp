// C3DSkeletalMorph::~C3DSkeletalMorph() -- detaches every morph parameter from this morph, then releases the combined morph, destroys and frees the parameter array and runs the base destructor
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A44EB0, length 138 bytes.
// Leaf: ??1C3DSkeletalMorph
// Callees: the counted block's destroy thunk (`call [info+4]`, data in ecx) and 0x00BFE9BC operator delete,
// 0x00A464E0 _Destroy_range(first, last, tag&) (`__fastcall`, releases each counted pointer; the by-reference
// tag is a 1-byte temporary in the high byte of the `push ecx` slot, `lea ecx,[esp+0Bh]`), 0x00BFEA14 free
// (null-tested), 0x0099A300 ~CBaseClassNonCopyable (manifest label "CBase").
// Layout: as the landed constructor 0x00A44E80 (PDB with the retail 12-byte CArray): MorphParameters +4
// (CArray<CCountedPointer<C3DSkeletalMorphParameter>>, 8-byte elements), CombinedMorph +0x10 (CCountedPointer:
// data +0x10, count block +0x14). C3DSkeletalMorphParameter (PDB) keeps its owning SkeletalMorph at +8.
// Shape: the body loop clears each parameter's back pointer (index in eax, size re-read every iteration);
// the members are destroyed in reverse order: the counted pointer's inline destructor (release, then both
// words nulled -- kept because code follows), then the CArray's (the landed 0x00AA8490 CRetailArray form).
extern "C" void __cdecl free(void* block);                      // 0x00BFEA14
void __cdecl operator delete(void* p);                           // 0x00BFE9BC

struct CCPPointerInfo {                                          // PDB: RefCount +0, DeleteFunc +4, Data +8
    long RefCount;
    void (__fastcall* DeleteFunc)(void* data);
    void* Data;
};
template<class T> struct CCountedPointer {
    T*              Data;                                        // +0
    CCPPointerInfo* Info;                                        // +4
    ~CCountedPointer()
    {
        if (Info != 0) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                operator delete(Info);
            }
        }
        Data = 0;
        Info = 0;
    }
    T* operator->() const { return Data; }
};

struct SNonscalarPtrIteratorTag { SNonscalarPtrIteratorTag() { } };
struct C3DSkeletalMorph;
struct C3DSkeletalMorphTarget;
struct C3DSkeletalMorphParameter {
    virtual void v00();
    float             CurrentValue;                              // +0x04
    C3DSkeletalMorph* SkeletalMorph;                             // +0x08
};
void __fastcall DestroyRange(CCountedPointer<C3DSkeletalMorphParameter>* first, CCountedPointer<C3DSkeletalMorphParameter>* last,
                             const SNonscalarPtrIteratorTag& tag);   // 0x00A464E0

template<class T> struct CRetailArray {                          // retail CArray: {first, last, end}
    T* first; T* last; T* end_of_storage;
    unsigned long Size() const { return (unsigned long)(last - first); }
    T& operator[](unsigned long index) { return first[index]; }
    ~CRetailArray()
    {
        DestroyRange(first, last, SNonscalarPtrIteratorTag());
        if (first != 0)
            free(first);
    }
};

struct CBaseClassNonCopyable {
    virtual void v00();
    ~CBaseClassNonCopyable();                                    // 0x0099A300
};

struct C3DSkeletalMorph : CBaseClassNonCopyable {
    CRetailArray<CCountedPointer<C3DSkeletalMorphParameter> > MorphParameters;   // +0x04
    CCountedPointer<C3DSkeletalMorphTarget>                   CombinedMorph;     // +0x10   sizeof == 0x18
    virtual void v00();                                          // own vtable 0x0129D498
    ~C3DSkeletalMorph();
};

C3DSkeletalMorph::~C3DSkeletalMorph()
{
    for (unsigned long i = 0; i < MorphParameters.Size(); ++i)
        MorphParameters[i]->SkeletalMorph = 0;
}