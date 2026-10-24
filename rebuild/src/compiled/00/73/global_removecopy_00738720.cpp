// _global::remove_copy(CDefPointer<const CMeleeCombatAbilityDef>* first, CDefPointer<const CMeleeCombatAbilityDef>* last, CDefPointer<const CMeleeCombatAbilityDef>* result, const CDefPointer<const CMeleeCombatAbilityDef>& val) -- STLport remove_copy over melee-ability def pointers: assign every pointer that is not `val` down to `result` (releasing / add-ref'ing the defs), return the new end
// Leaf: remove_copy
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00738720, 92 bytes (96 with padding), `ret 8`.
// Label correction: the catalog "_Move<CDefPointer<CMeleeCombatAbilityDef const>*,...>" is propagated (the real
//   __copy / _Move bodies are 0x0071F430 / 0x0071FA60); this body skips the elements equal to a by-reference value:
//   STLport `remove_copy(first, last, result, val) { for (; first != last; ++first) if (!(*first == val)) {
//   *result = *first; ++result; } return result; }`.
// Callees: none besides the def's virtual release (CDefPointeeBase slot 1, `call [eax+4]`).
// Element: CDefPointer<T> = { CDefPointeeBase* Object } (count at +4 of the pointee).  Its operator= is
//   `if (Object != o.Object) { if (Object) { Object->RemoveRef(); Object = 0; } Object = o.Object; if (Object)
//   ++Object->RefCount; }`.  The decrement must live in a pointee member (`if (--RefCount == 0) Release();`):
//   retail tests the `dec [ecx+4]` flags directly; writing `--Object->RefCount` in the pointer's operator= makes
//   VC7.1 reload Object through `this` after the store (may-alias) and re-test the count (99 v 92 bytes).
// Convention: /Gr (link-time __fastcall): first in ecx (esi), last in edx (ebx), result and the value reference on
//   the stack; the empty-range exit returns result straight from its slot.
struct CDefPointeeBase {
    virtual void v00();
    virtual void Release();                             // slot 1 (+0x04)
    long RefCount;                                      // +0x04
    void RemoveRef()
    {
        if (--RefCount == 0)
            Release();
    }
};
struct CMeleeCombatAbilityDef;
template <class T> struct CDefPointer {                 // 4 bytes
    CDefPointeeBase* Object;
    CDefPointer& operator=(const CDefPointer& o)
    {
        if (Object != o.Object) {
            if (Object) {
                Object->RemoveRef();
                Object = 0;
            }
            Object = o.Object;
            if (Object)
                ++Object->RefCount;
        }
        return *this;
    }
    bool operator==(const CDefPointer& o) const { return Object == o.Object; }
};
typedef CDefPointer<const CMeleeCombatAbilityDef> TAbilityPtr;

TAbilityPtr* __fastcall remove_copy(TAbilityPtr* first, TAbilityPtr* last, TAbilityPtr* result, const TAbilityPtr& val)
{
    for (; first != last; ++first)
        if (!(*first == val)) {
            *result = *first;
            ++result;
        }
    return result;
}