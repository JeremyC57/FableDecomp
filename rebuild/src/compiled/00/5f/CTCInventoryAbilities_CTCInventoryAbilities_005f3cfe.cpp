// CTCInventoryAbilities::~CTCInventoryAbilities() -- destroy the ability/expression bookkeeping members in reverse order, then both bases
// Leaf: ??1CTCInventoryAbilities
#pragma optimize("s", on)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005F3CFE, length 200 bytes. Called only by the deleting destructor 0x005FA461
// (slot 0 of CTCInventoryAbilities's vtable 0x01256AD4); the catalog label
// "CPatchTesselationEdgeStrip::~CPatchTesselationEdgeStrip" is propagated. Size TU (`pop ecx` after free).
// Callees: free @ 0x00BFEA14 (x7, the POD vectors and the two vector maps' storage), the map destructors @ 0x00579616 (x2,
//          map<EHeroAbility, CComponent*>) and @ 0x005F8FE6 (map<EUISpellCategory, CComponent*>), the
//          vector<pair<long, CDefPointer<CHeroAbilityDef>>> destructor @ 0x005FA0E7 (labels "CActiveFile::OnReadFinished",
//          byte twins), NInventory::CTCInventoryBase::~CTCInventoryBase @ 0x005C00A0 (tail jump).
// Layout: Ego_r.pdb CTCInventoryAbilities at retail offsets (base 0x130, CBase vptr +0x130, 12-byte vectors/maps, 16-byte
// vector maps): SpellToAssign +0x134 .. CurrentAssignedAbilityLevel +0x144, ExperienceSpentOnAbilities +0x148,
// ReceivedAbilityOrder +0x154, Abilities +0x160, AbilityButtons +0x16c, AssignedAbilities +0x17c, AssignedAbilitiesSave
// +0x188, the three component maps +0x194/+0x1a0/+0x1ac, CurrentAssignedExpressionLevel +0x1b8, ReceivedExpressionOrder
// +0x1bc, ExpressionButtons +0x1c8.
// Shape: as CTCInventory's destructor 0x00589C55: two vtable stores, members in reverse (POD storage freed inline), the
// inline CBase destructor's vtable store, the tail jump.
extern "C" void __cdecl free(void* block);                    // 0x00BFEA14
namespace NUISystem { struct CComponent; }

template<class T> struct CPODVector {              // STLport vector of a trivially destructible element (12 bytes)
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    __forceinline ~CPODVector()
    {
        if (_M_start)
            free(_M_start);
    }
};

namespace NInventory {
class CTCInventoryBase {                                      // vtable 0x01253DBC, retail sizeof 0x130
public:
    virtual ~CTCInventoryBase();                              // 0x005C00A0
    unsigned char _body[0x12C];
};
}
namespace NHeroInformationScreens {
class CBase {                                                 // vtable 0x0123A3E0
public:
    virtual ~CBase() {}
};
}

template<class K, class V> struct CPODVectorMap {             // CVectorMap<K, V> (16 bytes: the vector, then compare/dirty)
    struct SPair { K first; V second; };
    CPODVector<SPair> m_vector;
    unsigned char     m_compare;
    bool              m_dirty;
};
struct CHeroAbilityDefEntryVector {                           // vector<pair<long, CDefPointer<const CHeroAbilityDef>>>
    void* _M_start; void* _M_finish; void* _M_end_of_storage;
    ~CHeroAbilityDefEntryVector();                            // 0x005FA0E7
};
struct CSpellCategoryComponentMap {                           // map<EUISpellCategory, NUISystem::CComponent*>
    void* _M_header; unsigned long _M_node_count; long _M_key_compare;
    ~CSpellCategoryComponentMap();                            // 0x005F8FE6
};
struct CAbilityComponentMap {                                 // map<EHeroAbility, NUISystem::CComponent*>
    void* _M_header; unsigned long _M_node_count; long _M_key_compare;
    ~CAbilityComponentMap();                                  // 0x00579616
};

class CTCInventoryAbilities : public NInventory::CTCInventoryBase, public NHeroInformationScreens::CBase {   // vtables 0x01256AD4 / 0x01256AC0
public:
    long                       SpellToAssign;                 // +0x134 EHeroAbility
    long                       SpellSlotToSwap;               // +0x138
    NUISystem::CComponent*     PComponentSlotToSwap;          // +0x13c
    bool                       WaitingToGiveTutorial;         // +0x140
    bool                       Initialised;                   // +0x141
    bool                       CurrentSpecialAbilityUsedFromQuickAccess;   // +0x142
    long                       CurrentAssignedAbilityLevel;   // +0x144
    CPODVector<long>           ExperienceSpentOnAbilities;    // +0x148
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
    virtual ~CTCInventoryAbilities();
};

CTCInventoryAbilities::~CTCInventoryAbilities()
{
}