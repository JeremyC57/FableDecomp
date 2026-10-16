// CTCCombat::AddMeleeFlourishAbility(long defIndex) -- looks up a melee combat ability def, appends it to AbilityList and recomputes the usable abilities
// Leaf: AddMeleeFlourishAbility
// Grade: DIFFER(126v122) -- register allocation only: retail loads the def pointer straight into edi (copy + final release) and reuses edx for _M_end_of_storage; ours loads it into edx, copies to edi and needs ebx (extra push/pop ebx). Pushes, tag slot and control flow match.
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
struct CMeleeCombatAbilityDef {
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
struct CDefinitionManager {
    bool GetDef(long defIndex, CDefPointer* def);                 // 0x00737F80
};
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
struct CTCCombat {
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
    GetDefinitionManager()->GetDef(defIndex, &def);
    CMeleeCombatAbilityDef* object = def.Object;
    AbilityList.push_back(def);
    UpdateUsableMeleeCombatAbilities();
    if (object) {
        if (--object->RefCount == 0)
            object->Destroy();
    }
}
