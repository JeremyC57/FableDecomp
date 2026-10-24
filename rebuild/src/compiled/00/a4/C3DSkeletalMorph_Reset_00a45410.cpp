// C3DSkeletalMorph::Reset() -- drop every morph parameter (erase(begin(), end())) and release the combined morph
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A45410, length 93 bytes (padded to 96).
// Leaf: Reset
// Label correction: the catalog "_Dest_val<..., std::pair<unsigned long, CCountedPointer<CFaction>>>" is propagated.
// Both callers (CTCSkeletalMorph::ForceSkeletalMorph 0x007864CD and CalculateSkeletalMorph 0x0078670E) call it on
// `[this+1Ch]` = CTCSkeletalMorph::PSkeletalMorph.Data (Ego_r.pdb, retail +0x1c with the 12-byte CArray), a
// C3DSkeletalMorph; Ego_r.pdb lists ?Reset@C3DSkeletalMorph@@QAEXXZ, and the body empties MorphParameters (+4) and
// releases CombinedMorph (+0x10) -- the layout of the landed C3DSkeletalMorph destructor 0x00A44EB0.
// Callees: _Move<CCountedPointer*>(first, last, dest, tag, bool) @ 0x00A46460 (landed), _Destroy_range(first, last, tag&)
//          @ 0x00A464E0 (landed), the count block's destroy (+4, data in ecx), operator delete @ 0x00BFE9BC.
// Shape: the VC6-era `erase(begin(), end())` without an empty-range test (GOTCHAS 24n): last read before first, both
// tags homed in the high byte of the `push ecx` slot, the moved-to end kept in edi; the counted-pointer reset nulls
// the block word BEFORE the data word (the destructor order Data, Info gives the stores swapped). One zero register.
void __cdecl operator delete(void* p);                           // 0x00BFE9BC
struct CCPPointerInfo {                                          // PDB: RefCount +0, DeleteFunc +4, Data +8
    long RefCount;
    void (__fastcall* DeleteFunc)(void* data);
    void* Data;
};
template<class T> struct CCountedPointer {
    T*              Data;                                        // +0
    CCPPointerInfo* Info;                                        // +4
    void Clear()
    {
        if (Info != 0) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                operator delete(Info);
            }
        }
        Info = 0;
        Data = 0;
    }
};
struct SMoveTag { SMoveTag() { } };
struct SNonscalarPtrIteratorTag { SNonscalarPtrIteratorTag() { } };
struct C3DSkeletalMorphParameter;
struct C3DSkeletalMorphTarget;
typedef CCountedPointer<C3DSkeletalMorphParameter> PMorphParameter;
PMorphParameter* __fastcall _Move(PMorphParameter* first, PMorphParameter* last, PMorphParameter* dest, const SMoveTag& tag, bool overlap);   // 0x00A46460
void __fastcall DestroyRange(PMorphParameter* first, PMorphParameter* last, const SNonscalarPtrIteratorTag& tag);   // 0x00A464E0
template<class T> struct CRetailArray {                          // retail CArray: {first, last, end}
    T* first; T* last; T* end_of_storage;
    void clear()
    {
        T* l = last;
        T* f = first;
        T* kept = _Move(l, l, f, SMoveTag(), false);
        DestroyRange(kept, last, SNonscalarPtrIteratorTag());
        last = kept;
    }
};
struct CBaseClassNonCopyable {
    virtual void v00();
};
struct C3DSkeletalMorph : CBaseClassNonCopyable {
    CRetailArray<PMorphParameter>           MorphParameters;   // +0x04
    CCountedPointer<C3DSkeletalMorphTarget> CombinedMorph;     // +0x10
    virtual void v00();
    void Reset();
};
void C3DSkeletalMorph::Reset()
{
    MorphParameters.clear();
    CombinedMorph.Clear();
}