// CTCInventoryAbilities::CTCInventoryAbilities(CThing* thing) -- construct the abilities screen: the inventory base, the screen base at +0x130, the ability bookkeeping (20 experience slots, 20 empty ability entries), the button maps and component maps, and the two assigned-level cursors
// Leaf: ??0CTCInventoryAbilities
// Retail 0x005F3BCF, 303 bytes, `ret 4`. Frame-pointer size TU: optimize("y", off) + ("s", on).
// Callees: NInventory::CTCInventoryBase::CTCInventoryBase(CThing*) 0x005BFF07, NHeroInformationScreens::CBase::CBase()
// 0x0066E734, vector<long>(size_type, const long&, const allocator&) 0x00512630, vector<pair<long, CDefPointer<const
// CHeroAbilityDef>>>(size_type, const value_type&, const allocator&) 0x005F9BFE, ~pair<long, CDefPointer> 0x005F8736,
// map<EUISpellCategory, CComponent*>() 0x005F9452, map<EHeroAbility, CComponent*>() 0x00579842 (x2),
// GetNumAbilitiesPerRow() const 0x005EDCF0 (x2; manifest "_Dest_val<...>" is propagated).
// Vtables: 0x01256AD4 (+0) and 0x01256AC0 (+0x130) (rtti-vtables.tsv).
// Shape: initialiser list in declaration order; the vector fill value and the default allocator temporaries live in
// the dead `thing` argument slot (`mov [ebp+8],ebx`, `lea eax,[ebp+0Bh]`); the ability entry fill value {-1, null}
// is a frame temporary destroyed by its out-of-line pair destructor; the CVectorMap ctors zero the vector and the
// dirty byte (+0xD); both level cursors start at -GetNumAbilitiesPerRow() (ctor body).
// The fill-value slot needs the same-TU stand-in for the vector<long> fill ctor (the const-ref taker of the `0`):
// extern, VC7.1 keeps the escaped temporary in its own frame word [ebp-4] (8 lines).
// Layout: as the landed destructor 0x005F3CFE (Ego_r.pdb names at retail offsets).
#pragma optimize("y", off)
#pragma optimize("s", on)

class CThing;
namespace NUISystem { class CComponent; }
struct SAllocator { SAllocator() { } };

template<class T> struct SAllocProxy { T* _M_data; SAllocProxy(T* p) : _M_data(p) {} };
template<class T> struct CPODVector {              // STLport vector of a trivially destructible element (12 bytes)
    T* _M_start;
    T* _M_finish;
    SAllocProxy<T> _M_end_of_storage;
    CPODVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) {}
};
struct CLongFillVector : CPODVector<long> {        // vector<long>(n, value, allocator)
    CLongFillVector(unsigned long n, const long& value, const SAllocator& allocator = SAllocator());   // 0x00512630
};
template<class K, class V> struct CPODVectorMap {  // CVectorMap<K, V> (16 bytes: the vector, then compare/dirty)
    struct SPair { K first; V second; };
    CPODVector<SPair> m_vector;
    unsigned char     m_compare;
    bool              m_dirty;
    CPODVectorMap() : m_dirty(false) {}
};
class CHeroAbilityDef;
template<class T> class CDefPointer { public:
    T* Object;
    CDefPointer() : Object(0) {}
};
struct SAbilityEntry {                             // pair<long, CDefPointer<const CHeroAbilityDef>>
    long Level;
    CDefPointer<CHeroAbilityDef> PDef;
    SAbilityEntry(long level, const CDefPointer<CHeroAbilityDef>& def) : Level(level), PDef(def) {}
    ~SAbilityEntry();                              // 0x005F8736
};
struct CHeroAbilityDefEntryVector {                // vector<pair<long, CDefPointer<const CHeroAbilityDef>>>
    void* _M_start; void* _M_finish; void* _M_end_of_storage;
    CHeroAbilityDefEntryVector(unsigned long n, const SAbilityEntry& value, const SAllocator& allocator = SAllocator());   // 0x005F9BFE
};
struct CSpellCategoryComponentMap {                // map<EUISpellCategory, NUISystem::CComponent*>
    void* _M_header; unsigned long _M_node_count; long _M_key_compare;
    CSpellCategoryComponentMap();                  // 0x005F9452
};
struct CAbilityComponentMap {                      // map<EHeroAbility, NUISystem::CComponent*>
    void* _M_header; unsigned long _M_node_count; long _M_key_compare;
    CAbilityComponentMap();                        // 0x00579842
};

namespace NInventory {
class CTCInventoryBase {                           // vtable 0x01253DBC, retail sizeof 0x130
public:
    CTCInventoryBase(CThing* thing);               // 0x005BFF07
    virtual ~CTCInventoryBase();
    unsigned char _body[0x12C];
};
}
namespace NHeroInformationScreens {
class CBase {                                      // vtable stored by its out-of-line ctor
public:
    CBase();                                       // 0x0066E734
    virtual void v00();
};
}

class CTCInventoryAbilities : public NInventory::CTCInventoryBase, public NHeroInformationScreens::CBase {   // vtables 0x01256AD4 / 0x01256AC0
public:
    long                       SpellToAssign;                 // +0x134 EHeroAbility
    long                       SpellSlotToSwap;               // +0x138
    NUISystem::CComponent*     PComponentSlotToSwap;          // +0x13c
    bool                       WaitingToGiveTutorial;         // +0x140
    bool                       Initialised;                   // +0x141
    bool                       CurrentSpecialAbilityUsedFromQuickAccess;   // +0x142
    long                       CurrentAssignedAbilityLevel;   // +0x144
    CLongFillVector            ExperienceSpentOnAbilities;    // +0x148
    CPODVector<long>           ReceivedAbilityOrder;          // +0x154
    CHeroAbilityDefEntryVector Abilities;                     // +0x160
    CPODVectorMap<long, long>  AbilityButtons;                // +0x16c
    CPODVector<long>           AssignedAbilities;             // +0x17c
    CPODVector<long>           AssignedAbilitiesSave;         // +0x188
    CSpellCategoryComponentMap SpellCategoryComponents;       // +0x194
    CAbilityComponentMap       UnassignedAbilityComponents;   // +0x1a0
    CAbilityComponentMap       AbilityComponents;             // +0x1ac
    long                       CurrentAssignedExpressionLevel;   // +0x1b8
    CPODVector<long>           ReceivedExpressionOrder;       // +0x1bc
    CPODVectorMap<long, long>  ExpressionButtons;             // +0x1c8
    CTCInventoryAbilities(CThing* thing);
    virtual ~CTCInventoryAbilities();
    virtual void v00();
    long GetNumAbilitiesPerRow() const;                       // 0x005EDCF0
};

// Same-TU stand-in for vector<long>(n, const long&, const allocator&) 0x00512630 (body is not the retail one): the fill
// value's address no longer escapes, so VC7.1 homes the `0` temporary in the dead `thing` slot like retail.
void FillLongs_00512630(CPODVector<long>* vector, unsigned long n, long value);
#pragma auto_inline(off)
__declspec(noinline) CLongFillVector::CLongFillVector(unsigned long n, const long& value, const SAllocator& allocator)
{
    FillLongs_00512630(this, n, value);
}
#pragma auto_inline(on)

CTCInventoryAbilities::CTCInventoryAbilities(CThing* thing)
    : NInventory::CTCInventoryBase(thing),
      SpellToAssign(0), SpellSlotToSwap(0), PComponentSlotToSwap(0),
      WaitingToGiveTutorial(false), Initialised(false), CurrentSpecialAbilityUsedFromQuickAccess(false),
      ExperienceSpentOnAbilities(20, 0),
      Abilities(20, SAbilityEntry(-1, CDefPointer<CHeroAbilityDef>()))
{
    CurrentAssignedAbilityLevel = -GetNumAbilitiesPerRow();
    CurrentAssignedExpressionLevel = -GetNumAbilitiesPerRow();
}