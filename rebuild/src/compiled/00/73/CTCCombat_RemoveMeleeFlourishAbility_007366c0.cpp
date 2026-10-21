// CTCCombat::RemoveMeleeFlourishAbility(long defIndex) -- remove the melee combat ability with this def index from AbilityList and refresh the usable abilities
// Leaf: RemoveMeleeFlourishAbility
// Label: the catalog row says CTCCombat::AddMeleeFlourishAbility, but 0x00736640 is the add (labels_retry10 corrections); this body
// erases the def from AbilityList, and Ego_r.pdb has `?RemoveMeleeFlourishAbility@CTCCombat@@QAEXJ@Z` (void, one long).
//
// Retail 0x007366C0, 149 bytes (`ret 4`). Callees: CTCBase's `mov ecx,[ecx+4] ; jmp` forwarder
// @ 0x006869D0 to the thing's definition-manager getter (0x004C7990), CDefinitionManager::GetDef
// <CMeleeCombatAbilityDef>(long, CDefPointer<const CMeleeCombatAbilityDef>*) @ 0x00737F80 (labelled
// GetDef<CThingPhysicalSwitchDef>), STLport `__find` @ 0x00738810, `remove_copy` @ 0x00738720,
// `__copy` @ 0x00737CF0 and `__destroy_aux` @ 0x00738C20 (all `__fastcall` instantiations over
// CDefPointer<const CMeleeCombatAbilityDef>), CTCCombat::UpdateUsableMeleeCombatAbilities @ 0x00736590,
// the def's virtual slot 1 (release at zero).
// Layout: retail CTCCombat with 12-byte STLport vectors: AbilityList {first +0x2c, last +0x30, end +0x34}.
// Shape: `last = end(); newEnd = remove(begin(), last, def); erase(newEnd, last);` -- separate statements, so the empty
// tags of the inline remove and erase all share the dead defIndex slot; STLport's inline remove calls __find and, on a hit,
// remove_copy from the next element; the inline erase goes through STLport's inline copy() and _Destroy() wrappers (which
// load _M_finish before the tag pushes) to __copy(.., random_access_iterator_tag(), (int*)0) and __destroy_aux(..,
// __false_type()), then stores the new finish. GetDef, __find and remove_copy are same-TU noinline stand-in bodies (retail's
// LTCG saw none of them keep the def slot's address), so the def pointer's inline destructor releases with
// `mov ecx,[slot] ; dec [ecx+4] ; jne` and no re-read.
struct CDefPointeeBase {
    virtual void v00();
    virtual void Destroy();                         // slot 1
    long m_refCount;                                // +4
};

struct CMeleeCombatAbilityDef;

template <class T>
struct CDefPointer {
    CDefPointeeBase* m_pObject;
    CDefPointer() : m_pObject(0) {}
    ~CDefPointer()
    {
        if (m_pObject) {
            if (--m_pObject->m_refCount == 0)
                m_pObject->Destroy();
        }
    }
};

typedef CDefPointer<const CMeleeCombatAbilityDef> CAbilityPointer;

struct random_access_iterator_tag { random_access_iterator_tag() {} };
struct __false_type { __false_type() {} };

CAbilityPointer* __fastcall __find(CAbilityPointer* first, CAbilityPointer* last, const CAbilityPointer& val,
                                   const random_access_iterator_tag&);                                  // 0x00738810
CAbilityPointer* __fastcall remove_copy(CAbilityPointer* first, CAbilityPointer* last, CAbilityPointer* result,
                                        const CAbilityPointer& val);                                    // 0x00738720
CAbilityPointer* __fastcall __copy(CAbilityPointer* first, CAbilityPointer* last, CAbilityPointer* result,
                                   const random_access_iterator_tag&, int*);                            // 0x00737CF0
void __fastcall __destroy_aux(CAbilityPointer* first, CAbilityPointer* last, const __false_type&);      // 0x00738C20
CAbilityPointer* Find_AbilityPointer(CAbilityPointer* first, CAbilityPointer* last, const CDefPointeeBase* val);   // stand-in callees
CAbilityPointer* RemoveCopy_AbilityPointer(CAbilityPointer* first, CAbilityPointer* last, CAbilityPointer* result, const CDefPointeeBase* val);
#pragma auto_inline(off)
__declspec(noinline) CAbilityPointer* __fastcall __find(CAbilityPointer* first, CAbilityPointer* last, const CAbilityPointer& val,
                                                        const random_access_iterator_tag&)
{
    return Find_AbilityPointer(first, last, val.m_pObject);
}
__declspec(noinline) CAbilityPointer* __fastcall remove_copy(CAbilityPointer* first, CAbilityPointer* last, CAbilityPointer* result,
                                                             const CAbilityPointer& val)
{
    return RemoveCopy_AbilityPointer(first, last, result, val.m_pObject);
}
#pragma auto_inline(on)

inline CAbilityPointer* remove(CAbilityPointer* first, CAbilityPointer* last, const CAbilityPointer& val)
{
    first = __find(first, last, val, random_access_iterator_tag());
    if (first == last)
        return first;
    CAbilityPointer* next = first;
    return remove_copy(++next, last, first, val);
}

inline CAbilityPointer* copy(CAbilityPointer* first, CAbilityPointer* last, CAbilityPointer* result)
{
    return __copy(first, last, result, random_access_iterator_tag(), (int*)0);
}
inline void _Destroy(CAbilityPointer* first, CAbilityPointer* last)
{
    __destroy_aux(first, last, __false_type());
}

struct CAbilityVector {                             // STLport vector<CDefPointer<const CMeleeCombatAbilityDef>>
    CAbilityPointer* _M_start;
    CAbilityPointer* _M_finish;
    CAbilityPointer* _M_end_of_storage;
    CAbilityPointer* begin() { return _M_start; }
    CAbilityPointer* end() { return _M_finish; }
    CAbilityPointer* erase(CAbilityPointer* first, CAbilityPointer* last)
    {
        CAbilityPointer* i = copy(last, _M_finish, first);
        _Destroy(i, _M_finish);
        _M_finish = i;
        return first;
    }
};

struct CDefinitionManager {
    bool GetDef(long index, CAbilityPointer* out);  // 0x00737F80
};
CDefPointeeBase* Lookup_CDefinitionManager_GetDef(const void* manager, long index);   // stand-in callee
#pragma auto_inline(off)
__declspec(noinline) bool CDefinitionManager::GetDef(long index, CAbilityPointer* out)
{
    CDefPointeeBase* found = Lookup_CDefinitionManager_GetDef(this, index);
    if (!found)
        return false;
    out->m_pObject = found;
    return true;
}
#pragma auto_inline(on)

struct CTCCombat {
    virtual void v00();
    void*          ParentThing;                     // +0x04 (CTCBase)
    unsigned char  _pad08[0x2c - 0x08];
    CAbilityVector AbilityList;                     // +0x2c
    CDefinitionManager* GetDefinitionManager();     // 0x006869D0 (CTCBase forwarder)
    void UpdateUsableMeleeCombatAbilities();        // 0x00736590
    void RemoveMeleeFlourishAbility(long defIndex);
};

void CTCCombat::RemoveMeleeFlourishAbility(long defIndex)
{
    CAbilityPointer def;
    GetDefinitionManager()->GetDef(defIndex, &def);
    CAbilityPointer* last = AbilityList.end();
    CAbilityPointer* newEnd = remove(AbilityList.begin(), last, def);
    AbilityList.erase(newEnd, last);
    UpdateUsableMeleeCombatAbilities();
}