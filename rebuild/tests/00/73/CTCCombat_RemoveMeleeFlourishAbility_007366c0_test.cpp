// Behaviour test for 007366c0: src/007366c0.cpp copied verbatim; RemoveMeleeFlourishAbility looks up the def with this
// index, erases every AbilityList entry holding it (remove + erase), refreshes the usable abilities and releases the def.
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
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
class CDefPointeeBase { public:
    virtual void v00();
    virtual void Destroy();                         // slot 1
    long m_refCount;                                // +4
};

class CMeleeCombatAbilityDef;

template <class T>
class CDefPointer { public:
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

class CDefinitionManager { public:
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

class CTCCombat { public:
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
// ---- harness ----
static int g_destroyed = 0; static long g_index = -1; static int g_updates = 0; static int g_leftovers = 0;
static CDefPointeeBase* g_found = 0; static CDefinitionManager g_manager;
void CDefPointeeBase::v00() { }
void CDefPointeeBase::Destroy() { ++g_destroyed; }
void CTCCombat::v00() { }
CDefinitionManager* CTCCombat::GetDefinitionManager() { return &g_manager; }
void CTCCombat::UpdateUsableMeleeCombatAbilities() { ++g_updates; }
CDefPointeeBase* Lookup_CDefinitionManager_GetDef(const void* manager, long index)
{ CHECK(manager == &g_manager); g_index = index; if (g_found) ++g_found->m_refCount; return g_found; }
CAbilityPointer* Find_AbilityPointer(CAbilityPointer* first, CAbilityPointer* last, const CDefPointeeBase* val)
{ while (first != last && first->m_pObject != val) ++first; return first; }
CAbilityPointer* RemoveCopy_AbilityPointer(CAbilityPointer* first, CAbilityPointer* last, CAbilityPointer* result, const CDefPointeeBase* val)
{ for (; first != last; ++first) if (first->m_pObject != val) { result->m_pObject = first->m_pObject; ++result; } return result; }
CAbilityPointer* __fastcall __copy(CAbilityPointer* first, CAbilityPointer* last, CAbilityPointer* result, const random_access_iterator_tag&, int*)
{ for (; first != last; ++first, ++result) result->m_pObject = first->m_pObject; return result; }
void __fastcall __destroy_aux(CAbilityPointer* first, CAbilityPointer* last, const __false_type&)
{ for (; first != last; ++first) { ++g_leftovers; first->m_pObject = 0; } }
int main()
{
    static CDefPointeeBase a, b, c;
    a.m_refCount = 1; b.m_refCount = 1; c.m_refCount = 1;
    static CAbilityPointer list[5];
    static unsigned char combatBuf[sizeof(CTCCombat)]; CTCCombat& combat = *(CTCCombat*)combatBuf;
    list[0].m_pObject = &a; list[1].m_pObject = &b; list[2].m_pObject = &c; list[3].m_pObject = &b; list[4].m_pObject = 0;
    combat.AbilityList._M_start = list; combat.AbilityList._M_finish = list + 4; combat.AbilityList._M_end_of_storage = list + 5;

    // remove b (twice in the list)
    g_found = &b;
    combat.RemoveMeleeFlourishAbility(0x42);
    CHECK(g_index == 0x42 && g_updates == 1);
    CHECK(combat.AbilityList._M_finish == list + 2 && list[0].m_pObject == &a && list[1].m_pObject == &c && g_leftovers == 2);
    CHECK(b.m_refCount == 1 && g_destroyed == 0);

    // a def that is not in the list: nothing erased
    g_found = &b; g_leftovers = 0;
    combat.RemoveMeleeFlourishAbility(7);
    CHECK(combat.AbilityList._M_finish == list + 2 && g_leftovers == 0 && g_updates == 2 && b.m_refCount == 1);

    // lookup fails: the null def matches nothing either
    g_found = 0;
    combat.RemoveMeleeFlourishAbility(8);
    CHECK(combat.AbilityList._M_finish == list + 2 && g_updates == 3 && g_destroyed == 0);

    // remove the first entry; its def's last reference goes with the local
    g_found = &a; a.m_refCount = 0;
    combat.RemoveMeleeFlourishAbility(9);
    CHECK(combat.AbilityList._M_finish == list + 1 && list[0].m_pObject == &c && g_leftovers == 1 && g_destroyed == 1);

    if (fails == 0) printf("007366c0_TEST PASS\n");
    return fails;
}