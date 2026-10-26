// CTCCombat::AddMeleeFlourishAbility(long defIndex) -- looks up a melee combat ability def, appends it to AbilityList and recomputes the usable abilities
// Leaf: AddMeleeFlourishAbility
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00736640, length 128 bytes (default /O2 TU).
// Label correction: catalogued as CTCInventoryBase::AddCategory (propagated); the body is CTCCombat's: it sits between
// CTCCombat::UpdateUsableMeleeCombatAbilities (0x00736590, called here) and 0x007366C0, appends to the PDB
// CTCCombat::AbilityList (+0x2C vector<CDefPointer<const CMeleeCombatAbilityDef>>), and the PDB names exactly two
// `(long)` members for that list, AddMeleeFlourishAbility and RemoveMeleeFlourishAbility. This one is the push_back, so it is
// AddMeleeFlourishAbility; the manifest's "AddMeleeFlourishAbility" at 0x007366C0 (find + erase) is RemoveMeleeFlourishAbility.
// Callees: CTCBase::GetDefinitionManager @ 0x006869D0, CDefinitionManager::GetDef<CMeleeCombatAbilityDef>(long,
// CDefPointer*) @ 0x00737F80, vector<CDefPointer<...>>::_M_insert_overflow @ 0x00739BC0, UpdateUsableMeleeCombatAbilities @ 0x00736590.
// Shape: the def out-slot (the `push ecx` frame word) is zeroed after the argument pushes and read ONCE after GetDef into
// edi, which serves the inline push_back's add-ref copy (constructor testing its parameter's pointer) and the hand-written
// release at the end; STLport push_back inlined with a fresh `__false_type()` tag in the dead `defIndex` slot.
// Layout: CTCCombat AbilityList {first,last,end} at +0x2C (engine/CTCCombat.h offset; the retail vector is 12 bytes).
class CMeleeCombatAbilityDef { public:
    virtual void v00();
    virtual void Destroy();                                       // slot 1 (+0x04)
    long RefCount;                                                // +0x04
};
struct CDefPointer {                                              // CDefPointer<const CMeleeCombatAbilityDef> (released by hand)
    CMeleeCombatAbilityDef* Object;
    CDefPointer() : Object(0) {}
    CDefPointer(const CDefPointer& other)
    {
        CMeleeCombatAbilityDef* object = other.Object;
        Object = object;
        if (object)
            ++object->RefCount;
    }
};
class CDefinitionManager { public:
    bool GetDef_00737f80(long defIndex, CDefPointer* def);                 // 0x00737F80
};
void* FindDef_CDefinitionManager_GetDef_0();   // stand-in callee of the stand-in body below
// Stand-in body (the real one is out of line in retail): it writes the out slot only inline, so VC7.1 knows the def
// pointer's address does not escape the call (the landed 0x00848730 idiom).
#pragma auto_inline(off)
__declspec(noinline) bool CDefinitionManager::GetDef_00737f80(long defIndex, CDefPointer* out)
{
    void* def = FindDef_CDefinitionManager_GetDef_0();
    if (!def)
        return false;
    *(void**)out = def;
    return true;
}
#pragma auto_inline(on)
struct __false_type { __false_type() {} };
inline void* operator new(unsigned int, void* where) { return where; }
struct CDefPointerVector {                                        // std::vector<CDefPointer<const CMeleeCombatAbilityDef>>
    CDefPointer* _M_start;
    CDefPointer* _M_finish;
    CDefPointer* _M_end_of_storage;
    void _M_insert_overflow(CDefPointer* pos, const CDefPointer& x, const __false_type&, unsigned int fillLen, bool atEnd);   // 0x00739BC0
    void push_back(const CDefPointer& x)
    {
        if (_M_finish != _M_end_of_storage) {
            if (_M_finish)
                new (_M_finish) CDefPointer(x);
            ++_M_finish;
        } else
            _M_insert_overflow(_M_finish, x, __false_type(), 1, true);
    }
};
void* FindRef_CDefPointerVector__M_insert_overflow_0();   // stand-in callee of the stand-in body below
// Stand-in body (the real one is out of line in retail): it only writes a word through the reference parameter inline, so
// VC7.1 knows the caller's address-taken local does not escape the call.
#pragma auto_inline(off)
__declspec(noinline) void CDefPointerVector::_M_insert_overflow(CDefPointer* pos, const CDefPointer& x, const __false_type& p2, unsigned int fillLen, bool atEnd)
{
    void* p = FindRef_CDefPointerVector__M_insert_overflow_0();
    if (!p)
        return;
    *(void**)pos = p;
    if (*(void**)&x != p)
        return;
}
#pragma auto_inline(on)
class CTCCombat { public:
    void* __vftable;                                              // +0x00
    unsigned char _pad_0x04[0x28];
    CDefPointerVector AbilityList;                                // +0x2C
    CDefinitionManager* GetDefinitionManager();                   // 0x006869D0 (CTCBase)
    void UpdateUsableMeleeCombatAbilities();                      // 0x00736590
    void AddMeleeFlourishAbility(long defIndex);
};
void CTCCombat::AddMeleeFlourishAbility(long defIndex)
{
    CDefPointer def;
    GetDefinitionManager()->GetDef_00737f80(defIndex, &def);
    CMeleeCombatAbilityDef* object = def.Object;
    AbilityList.push_back(def);
    UpdateUsableMeleeCombatAbilities();
    if (object) {
        if (--object->RefCount == 0)
            object->Destroy();
    }
}