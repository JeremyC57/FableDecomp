// Grade: DIFFER(176v176) -- register choice only: ParentThing pushed through ecx (retail eax), the overflow call's tag/value leas in edx/ecx order (retail ecx/edx), the release vtable in edx (retail eax); needed the same-TU _M_insert_overflow body (auto_inline off) to keep the def in esi
// CTCCombat::UpdateUsableMeleeCombatAbilities() -- rebuild the usable-ability list: clear it, then keep every ability def that is valid for the parent thing
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00736590, 176 bytes (plain `ret`).
// Callees: `__fastcall _Move(first, last, dest, tag, bool)` @ 0x00737CF0 and `__fastcall _Destroy(first, last, tag)` @ 0x00738C20
// for CDefPointer<const CMeleeCombatAbilityDef> (the inline VC6-era `erase(begin(), end())`; the latter labelled with another
// instantiation's name), CMeleeCombatAbilityDef::IsAbilityValid(CThing*) @ 0x008E7040, the vector's _M_insert_overflow
// @ 0x00739BC0 (manifest "std::vector_insert_CCountedPointer"), the def's virtual slot 1 on the last release.
// Layout: local model (engine/CTCCombat.h is Ego_r.pdb with 16-byte vectors): AbilityList +0x2c, UsableAbilityList +0x38.
// Shape: clear() and push_back() are inline members of the vector (`lea edi,[ebx+38h]` as their `this`); clear is
// `_Move(last, last, first)` + `_Destroy(kept, last)` + `last = kept` with no empty test; each element is copied into a
// CDefPointer local (add-ref, null-tested) whose address push_back passes on; the empty tags share the high byte of the
// frame word below it; the source end is re-read every pass.
struct CThing;
struct CMeleeCombatAbilityDef {
    virtual void v00();
    virtual void Delete();                              // slot 1 (+0x04)
    long m_refCount;                                    // +0x04
    bool IsAbilityValid(CThing* thing) const;           // 0x008E7040
};
struct CDefPointer {                                    // CDefPointer<const CMeleeCombatAbilityDef>
    CMeleeCombatAbilityDef* Object;
    CDefPointer(const CDefPointer& other) : Object(other.Object) { if (Object) ++Object->m_refCount; }
    ~CDefPointer() { if (Object && --Object->m_refCount == 0) Object->Delete(); }
    CMeleeCombatAbilityDef* operator->() const { return Object; }
};
struct SMoveTag { SMoveTag() { } };                     // _Nonscalar_ptr_iterator_tag
struct SAllocatorTag { SAllocatorTag() { } };
struct __false_type { __false_type() { } };
inline void* operator new(unsigned int, void* where) { return where; }
CDefPointer* __fastcall Move(CDefPointer* first, CDefPointer* last, CDefPointer* dest, const SMoveTag& tag, bool overlap);   // 0x00737CF0
void __fastcall DestroyRange(CDefPointer* first, CDefPointer* last, const SAllocatorTag& tag);                            // 0x00738C20
struct CDefPointerVector {                              // STLport vector<CDefPointer<const CMeleeCombatAbilityDef>>, 12 bytes
    CDefPointer* _M_start;
    CDefPointer* _M_finish;
    CDefPointer* _M_end_of_storage;
    void _M_insert_overflow(CDefPointer* position, const CDefPointer& x, const __false_type&, unsigned long fill_len, bool atend);   // 0x00739BC0
    void erase(CDefPointer* first, CDefPointer* last)
    {
        CDefPointer* kept = Move(last, _M_finish, first, SMoveTag(), false);
        DestroyRange(kept, _M_finish, SAllocatorTag());
        _M_finish = kept;
    }
    void clear() { erase(_M_start, _M_finish); }
    void push_back(const CDefPointer& x)
    {
        if (_M_finish != _M_end_of_storage) {
            new (_M_finish) CDefPointer(x);
            ++_M_finish;
        } else
            _M_insert_overflow(_M_finish, x, __false_type(), 1, true);
    }
};
#pragma auto_inline(off)
extern "C" void* __cdecl malloc(unsigned int size);     // 0x00BFEA0E
extern "C" void __cdecl free(void* block);              // 0x00BFEA14
CDefPointer* __fastcall Move(CDefPointer* first, CDefPointer* last, CDefPointer* dest, const SMoveTag& tag, bool overlap)
{
    for (long n = last - first; n > 0; --n, ++first, ++dest) {
        if (dest->Object != first->Object) {
            if (dest->Object) {
                if (--dest->Object->m_refCount == 0)
                    dest->Object->Delete();
                dest->Object = 0;
            }
            dest->Object = first->Object;
            if (dest->Object)
                ++dest->Object->m_refCount;
        }
    }
    return dest;
}
void __fastcall DestroyRange(CDefPointer* first, CDefPointer* last, const SAllocatorTag& tag)
{
    for (; first != last; ++first) {
        if (first->Object) {
            if (--first->Object->m_refCount == 0)
                first->Object->Delete();
            first->Object = 0;
        }
    }
}
void CDefPointerVector::_M_insert_overflow(CDefPointer* position, const CDefPointer& x, const __false_type&, unsigned long fill_len, bool atend)
{
    unsigned long oldSize = _M_finish - _M_start;
    unsigned long len = oldSize + (oldSize < fill_len ? fill_len : oldSize);
    CDefPointer* newStart = len ? (CDefPointer*)malloc(len * sizeof(CDefPointer)) : 0;
    CDefPointer* newFinish = newStart;
    for (CDefPointer* p = _M_start; p != position; ++p, ++newFinish)
        new (newFinish) CDefPointer(*p);
    for (unsigned long i = 0; i < fill_len; ++i, ++newFinish)
        new (newFinish) CDefPointer(x);
    if (!atend)
        for (CDefPointer* p = position; p != _M_finish; ++p, ++newFinish)
            new (newFinish) CDefPointer(*p);
    DestroyRange(_M_start, _M_finish, SAllocatorTag());
    if (_M_start)
        free(_M_start);
    _M_start = newStart;
    _M_finish = newFinish;
    _M_end_of_storage = newStart + len;
}
#pragma auto_inline(on)
struct CTCCombat {
    virtual void v00();
    CThing* ParentThing;                                // +0x04 (CTCBase)
    unsigned char _pad08[0x2c - 0x08];
    CDefPointerVector AbilityList;                      // +0x2c (PDB +0x2c)
    CDefPointerVector UsableAbilityList;                // +0x38 (PDB +0x3c)
    void UpdateUsableMeleeCombatAbilities();
};

void CTCCombat::UpdateUsableMeleeCombatAbilities()
{
    UsableAbilityList.clear();
    for (CDefPointer* it = AbilityList._M_start; it != AbilityList._M_finish; ++it) {
        const CDefPointer ability(*it);
        if (ability->IsAbilityValid(ParentThing))
            UsableAbilityList.push_back(ability);
    }
}
