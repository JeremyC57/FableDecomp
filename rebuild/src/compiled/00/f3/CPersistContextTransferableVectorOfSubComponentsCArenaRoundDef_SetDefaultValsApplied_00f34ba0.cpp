// CPersistContext_TransferableVectorOfSubComponents<CArenaRoundDef>::SetDefaultValsApplied(long index, bool applied) -- stores `applied` into element `index`'s CDefClassBase::DefaultValsApplied bit (+0x24 bit 2)
// Leaf: SetDefaultValsApplied
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00F34BA0, a slot of the no-RTTI vtable at 0x01301C68
// (slots: Size, Resize, AreDefaultValsApplied, SetDefaultValsApplied, Transfer -- the PDB method set of
// CPersistContext_TransferableVectorOfSubComponents<T>, mangled `?SetDefaultValsApplied@?$CPersistContext_TransferableVectorOfSubComponents@VCArenaRoundDef@@@@...`).
// The wrapper is the one the "ArenaRounds" vector (Transfer 0x00F349F0 builds this wrapper for its text-load arm).
// Element: CArenaRoundDef, 0x38 bytes in retail (Ego_r.pdb says 60: its STLport vector members are 12 bytes in
// retail, 16 in the PDB). The manifest label `CPersistContext_TransferableVector::vfunc_N` is propagated.
// Layout: the element is modelled locally: rebuild/include/engine/CDefClassBase.h flattens the +0x24 bitfield
// group into `unsigned char DefaultValsApplied[1]`, while the PDB (struct_layouts_egor.tsv) gives three uchar:1
// bits Setup, Template, DefaultValsApplied -- DefaultValsApplied is bit 2 (the retail `shr al,2` / `and dl,4`).
// Shape: the element is bound to a named reference first, so VC forms the element address (`add eax,edx`)
// and addresses the bitfield byte as [eax+0x24]; the direct `(*TransVector)[index].DefaultValsApplied = applied`
// folds +0x24 into a `lea` and loads the vector before the index (37 bytes v 38).
class CPersistContext;

class CArenaRoundDef {                                  // 0x38 bytes; CDefClassBase-derived sub-component def
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void Transfer(CPersistContext& context);     // slot 18 (+0x48)
    unsigned char      _resourceBase[0x18];          // +0x04 CResource / CDefPointeeBase body
    void*              PDefManager;                  // +0x1C
    unsigned long      GlobalIndex;                  // +0x20
    unsigned char      Setup : 1;                    // +0x24 bit 0
    unsigned char      Template : 1;                 // +0x24 bit 1
    unsigned char      DefaultValsApplied : 1;       // +0x24 bit 2
    unsigned char      _body[0x38 - 0x25];
};

template <class T> struct vector {                 // STLport, 12 bytes in retail
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    long size() const { return _M_finish - _M_start; }
    T& operator[](long index) { return _M_start[index]; }
};

class CPersistContext_TransferableVectorOfSubComponentsGenericType { public:
    virtual long Size() = 0;
    virtual void Resize(long count) = 0;
    virtual bool AreDefaultValsApplied(long index) = 0;
    virtual void SetDefaultValsApplied(long index, bool applied) = 0;
    virtual void Transfer(CPersistContext& context, long index) = 0;
};

template <class T> struct CPersistContext_TransferableVectorOfSubComponents : CPersistContext_TransferableVectorOfSubComponentsGenericType {
    vector<T>* TransVector;                          // +0x4
    virtual long Size();
    virtual void Resize(long count);
    virtual bool AreDefaultValsApplied(long index);
    virtual void SetDefaultValsApplied(long index, bool applied);
    virtual void Transfer(CPersistContext& context, long index);
};

template <class T>
void CPersistContext_TransferableVectorOfSubComponents<T>::SetDefaultValsApplied(long index, bool applied)
{
    T& def = (*TransVector)[index];
    def.DefaultValsApplied = applied;
}

template void CPersistContext_TransferableVectorOfSubComponents<CArenaRoundDef>::SetDefaultValsApplied(long index, bool applied);