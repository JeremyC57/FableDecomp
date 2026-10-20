// CTCHeroMorph::GetTattoos(vector<CDefPointer<const CTattooDef> >& tattoos) const -- refill the caller's vector with the tattoo sub-defs of the hero's tattoo thing defs
// Leaf: GetTattoos
// Retail: Fable.exe (TLC, VC7.1) @ 0x0071DD50, 196 bytes (`ret 4`). The catalog row says "CTCObjectAugmentations::GetDamageMultiplier"
//   (propagated): `this` holds a vector of CThingBaseDef pointers at +0x1c (CTCHeroMorph::Tattoos, PDB +0x2c: the two lists
//   before it are 4 bytes in retail), the callers are CTCHeroMorph/CTCInventoryTrade code (0x0057D42F, 0x0059FCE6) and the PDB
//   has ?GetTattoos@CTCHeroMorph@@QBEXAAV?$vector@V?$CDefPointer@$$CBVCTattooDef@@@@...@@@Z.
// Callees: Dinkumware vector clear = __fastcall _Move @ 0x0071FB80 + __fastcall _Destroy_range @ 0x00580A0B (labels propagated),
//   reserve @ 0x00721800, CDefClassBase::GetSubDef<const CTattooDef> @ 0x005B1EFC (labelled GetSubDef<CAbilityDef>), InsertGrow
//   @ 0x00721B10, the def release through vtable slot 1.
// Tried: holder-released def (speed), the same under optimize("s") (everything out of line, frame pointer).
//   Next: lane gap: one dead parameter slot shared by the push_back value and its tag (GOTCHAS chunks 203-204).
void ReleaseDef_s7(void* def);   // stand-in: the def's release (count decrement, Destroy on zero)
struct CDefVirtual {
    virtual void v00();
    virtual void Destroy() const;                       // slot 1
};
struct CDefBase : CDefVirtual {
    mutable long m_refCount;                            // +0x04
};
template <class T> struct CDefPointer {
    T* Object;
    CDefPointer() : Object(0) { }
    CDefPointer(const CDefPointer& other) : Object(other.Object) { if (Object) ++Object->m_refCount; }
    T* operator->() const { return Object; }
};
template <class T> struct CDefHolder {                  // owns the reference GetSubDef handed over
    T* Object;
    __forceinline ~CDefHolder()
    {
        if (Object && --Object->m_refCount == 0)
            Object->Destroy();
    }
};
struct CTattooDef : CDefBase { };
struct CDefClassBase : CDefBase {
    template <class T> bool GetSubDef(CDefPointer<T>& out) const;   // 0x005B1EFC (T = const CTattooDef)
};
void* FindDef_s7r_0();
#pragma auto_inline(off)
// Same-TU body of GetSubDef (retail instantiates it in this TU): fetch the def, then assign it to the out pointer the way
// CDefPointer::operator= does (read the old pointer, release it when different, store the new one).
template <class T> __declspec(noinline) bool CDefClassBase::GetSubDef(CDefPointer<T>& out) const
{
    void* def = FindDef_s7r_0();
    if (!def)
        return false;
    void* old = *(void**)&out;
    if (old != def) {
        if (old)
            ReleaseDef_s7(old);
        *(void**)&out = def;
    }
    return true;
}
#pragma auto_inline(on)
struct CThingBaseDef : CDefClassBase { };
struct SMoveTag { SMoveTag() { } };
struct SScalarTag { SScalarTag() { } };
typedef CDefPointer<const CTattooDef> CTattooDefPointer;
CTattooDefPointer* __fastcall Move_0071fb80(CTattooDefPointer* first, CTattooDefPointer* last, CTattooDefPointer* dest,
                                            const SMoveTag& tag, bool overlap);   // 0x0071FB80
void __fastcall Destroy_00580a0b(CTattooDefPointer* first, CTattooDefPointer* last, const SScalarTag& allocator);   // 0x00580A0B
inline void* operator new(unsigned int, void* where) { return where; }
struct CTattooDefVector {                               // VC6-era Dinkumware vector {first, last, end}
    CTattooDefPointer* m_first;
    CTattooDefPointer* m_last;
    CTattooDefPointer* m_end;
    void reserve(unsigned long count);                  // 0x00721800
    void InsertGrow(CTattooDefPointer* where, const CTattooDefPointer& value, const SScalarTag& tag,
                    unsigned long count, bool noTail);   // 0x00721B10
    void clear()
    {
        CTattooDefPointer* last = m_last;
        CTattooDefPointer* first = m_first;
        CTattooDefPointer* kept = Move_0071fb80(last, last, first, SMoveTag(), false);
        Destroy_00580a0b(kept, m_last, SScalarTag());
        m_last = kept;
    }
    void push_back(const CTattooDefPointer& value)
    {
        if (m_last != m_end) {
            new (m_last) CTattooDefPointer(value);
            ++m_last;
        } else
            InsertGrow(m_last, value, SScalarTag(), 1, true);
    }
};
void* GrowVectorStorage_s7(void* vector, unsigned long count);

#pragma auto_inline(off)
// Same-TU body of the vector growth member (InsertGrow, instantiated in retail's TU): the new storage receives the value
// read through its const reference; the empty tag is never touched.
__declspec(noinline) void CTattooDefVector::InsertGrow(CTattooDefPointer* where, const CTattooDefPointer& value, const SScalarTag& tag, unsigned long count, bool noTail)
{
    CTattooDefPointer* p = (CTattooDefPointer*)GrowVectorStorage_s7(this, count);
    if (p)
        new (p) CTattooDefPointer(value);
}
#pragma auto_inline(on)
template <class T> struct vector {                      // STLport vector, 12 bytes
    T* _M_start; T* _M_finish; T* _M_end_of_storage;
    unsigned long size() const { return (unsigned long)(_M_finish - _M_start); }
    T& operator[](unsigned long n) const { return *(_M_start + n); }
};
struct CThing;
struct CTCHeroMorph {
    virtual void v00();
    CThing*       ParentThing;                          // +0x04 (CTCBase)
    unsigned char _pad08[0x1c - 0x08];
    vector< CDefPointer<const CThingBaseDef> > Tattoos; // +0x1c (PDB +0x2c)
    void GetTattoos(CTattooDefVector& tattoos) const;
};

void CTCHeroMorph::GetTattoos(CTattooDefVector& tattoos) const
{
    tattoos.clear();
    tattoos.reserve(Tattoos.size());
    for (unsigned long i = 0; i < Tattoos.size(); ++i) {
        CTattooDefPointer slot;
        Tattoos[i]->GetSubDef(slot);
        CDefHolder<const CTattooDef> tattoo = { slot.Object };
        tattoos.push_back(slot);
    }
}