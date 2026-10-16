// CTCCombat::AddMeleeFlourishAbility(long defIndex) -- catalog label; the body REMOVES the melee combat ability with this def index from AbilityList and refreshes the usable abilities
// Grade: DIFFER(157v149) -- (1) the copy/destroy pushes use the other scratch registers (retail `lea ecx,[esp+14h] ; push ecx` / `lea edx,[esp+10h] ; push edx` around the finish load); (2) the def pointer release: retail `mov ecx,[esp+8] ; test ; dec [ecx+4] ; jne ; mov eax,[ecx] ; call [eax+4]`, our inline dtor re-reads the object and its count after the dec (an inline Release() member or a pointer-local copy give the `add eax,4 ; dec esi ; mov [eax],esi` counted form instead)
//
// Retail 0x007366C0, 149 bytes (`ret 4`). Callees: CTCBase's `mov ecx,[ecx+4] ; jmp` forwarder
// @ 0x006869D0 to the thing's definition-manager getter (0x004C7990), CDefinitionManager::GetDef
// <CMeleeCombatAbilityDef>(long, CDefPointer<const CMeleeCombatAbilityDef>*) @ 0x00737F80 (labelled
// GetDef<CThingPhysicalSwitchDef>), STLport `__find` @ 0x00738810, `remove_copy` @ 0x00738720,
// `__copy` @ 0x00737CF0 and `__destroy_aux` @ 0x00738C20 (all `__fastcall` instantiations over
// CDefPointer<const CMeleeCombatAbilityDef>), CTCCombat::UpdateUsableMeleeCombatAbilities @ 0x00736590,
// the def's virtual slot 1 (release at zero).
// Layout: retail CTCCombat with 12-byte STLport vectors: AbilityList {first +0x2c, last +0x30, end +0x34}.
// Shape: `last = end(); newEnd = remove(begin(), last, def); erase(newEnd, last);` -- separate
// statements, so the empty tags of the inline remove and erase all share the dead defIndex slot (one
// full expression gives the erase tags their own frame word); STLport's inline remove calls __find and, on a hit, remove_copy from the next element; the
// inline erase copies the tail with __copy(.., random_access_iterator_tag(), (ptrdiff_t*)0), destroys
// the leftovers with __destroy_aux(.., __false_type()) and stores the new finish; every empty tag lands
// in the dead defIndex argument slot. The def pointer's inline destructor releases at the end.
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

inline CAbilityPointer* remove(CAbilityPointer* first, CAbilityPointer* last, const CAbilityPointer& val)
{
    first = __find(first, last, val, random_access_iterator_tag());
    if (first == last)
        return first;
    CAbilityPointer* next = first;
    return remove_copy(++next, last, first, val);
}

struct CAbilityVector {                             // STLport vector<CDefPointer<const CMeleeCombatAbilityDef>>
    CAbilityPointer* _M_start;
    CAbilityPointer* _M_finish;
    CAbilityPointer* _M_end_of_storage;
    CAbilityPointer* begin() { return _M_start; }
    CAbilityPointer* end() { return _M_finish; }
    CAbilityPointer* erase(CAbilityPointer* first, CAbilityPointer* last)
    {
        CAbilityPointer* i = __copy(last, _M_finish, first, random_access_iterator_tag(), (int*)0);
        __destroy_aux(i, _M_finish, __false_type());
        _M_finish = i;
        return first;
    }
};

struct CDefinitionManager {
    bool GetDef(long index, CAbilityPointer* out);  // 0x00737F80
};

struct CTCCombat {
    virtual void v00();
    void*          ParentThing;                     // +0x04 (CTCBase)
    unsigned char  _pad08[0x2c - 0x08];
    CAbilityVector AbilityList;                     // +0x2c
    CDefinitionManager* GetDefinitionManager();     // 0x006869D0 (CTCBase forwarder)
    void UpdateUsableMeleeCombatAbilities();        // 0x00736590
    void AddMeleeFlourishAbility(long defIndex);
};

void CTCCombat::AddMeleeFlourishAbility(long defIndex)
{
    CAbilityPointer def;
    GetDefinitionManager()->GetDef(defIndex, &def);
    CAbilityPointer* last = AbilityList.end();
    CAbilityPointer* newEnd = remove(AbilityList.begin(), last, def);
    AbilityList.erase(newEnd, last);
    UpdateUsableMeleeCombatAbilities();
}
