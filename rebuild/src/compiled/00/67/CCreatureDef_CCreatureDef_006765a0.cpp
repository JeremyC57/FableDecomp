// CCreatureDef::CCreatureDef() -- the creature definition's default constructor: the def-class root (CResource, no manager, flag bits cleared), four empty counted pointers, two empty long vectors, the empty PhonemeAnim map, the empty DialogueVoices vector, BattleCrySound = -1, the radius-weight map and range zeroed, then PWoundMorphs / PRandomAppearanceMorph / PExpressions / PStats each reset to a freshly built default object
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006765A0, length 381 bytes (+ int3 padding), plain `ret`. Vtable 0x0125B43C (CCreatureDef,
// rtti-vtables.tsv); the stats object's vtable 0x012321AC is CCreatureStatsDef.
// Callees: CResource::CResource @ 0x009FBEC0 (the def root's base, twice), malloc @ 0x00BFEA0E (the two STLport map header nodes,
// 24 bytes each), operator new @ 0x00BFEA1A (0x10 CWoundMorphs, 0x3c CRandomAppearanceMorph, 0x10 CExpressionSet, 0x48 CCreatureStatsDef),
// CRandomAppearanceMorph::CRandomAppearanceMorph @ 0x0086C4D0, CCountedPointer<T>::Reset(T*) @ 0x00675060 (CWoundMorphs), @ 0x00676840
// (CRandomAppearanceMorph), @ 0x006750D0 (CExpressionSet), @ 0x00673500 (CCreatureStatsDef).
// Layout (retail, Ego_r.pdb CCreatureDef minus the 4-byte-smaller STLport containers): CDefClassBase root {vptr, CResource +4.., PDefManager
// +0x1c, GlobalIndex +0x20, three flag bits in the byte +0x24}; PWoundMorphs +0x28, PRandomAppearanceMorph +0x30, PExpressions +0x38,
// PStats +0x40, InitialActions +0x48, InitialAppearanceModifiers +0x54, PhonemeAnim +0x60 (header, count, compare), DialogueVoices +0x6c,
// the inventory longs +0x78.., BattleCrySound +0xb0, RadiusWeight {map +0xd0, weight +0xdc}, RadiusSpread +0xe0.
// Shape: the root ctor is inline over the out-of-line CResource ctor (PDefManager = 0, `and byte,0F8h`, then the vtable); the map ctor
// is the inlined STLport `_Rb_tree()` (header node from malloc, colour red, no parent, left/right self, zero count, each store through
// the re-read member); each Reset argument is a null-checked `new` with its inline or out-of-line ctor.
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E
class CResource { public:                                     // the def root's base
    virtual void v00();
    unsigned char _body[0x1c - 4];
    CResource();                                       // 0x009FBEC0
};
class CDefinitionManager;
struct __declspec(novtable) CDefClassBase : CResource {
    CDefinitionManager* PDefManager;                   // +0x1c
    unsigned long GlobalIndex;                         // +0x20
    unsigned char DefaultValsApplied : 1;              // +0x24
    unsigned char Flag1 : 1;
    unsigned char Flag2 : 1;
    CDefClassBase() : CResource(), PDefManager(0), DefaultValsApplied(0), Flag1(0), Flag2(0) {}
};
template <class T> struct CRetailVector {              // STLport vector (12 bytes)
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    CRetailVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) {}
};
template <class Node> struct CRetailMap {              // STLport _Rb_tree (12 bytes: header, node count, compare)
    Node* _M_header;
    unsigned long _M_node_count;
    unsigned char _M_compare[4];
    CRetailMap() : _M_header(0)                        // the alloc proxy's null start, then the header node
    {
        _M_header = (Node*)malloc(sizeof(Node));
        _M_node_count = 0;
        _M_header->_M_color = 0;
        _M_header->_M_parent = 0;
        _M_header->_M_left = _M_header;
        _M_header->_M_right = _M_header;
    }
};
struct CPhonemeAnimNode {                              // pair<CCharString, long> node, 24 bytes
    unsigned char _M_color;
    CPhonemeAnimNode* _M_parent;
    CPhonemeAnimNode* _M_left;
    CPhonemeAnimNode* _M_right;
    void* _M_key;
    long _M_value;
};
struct CStatWeightNode {                               // 24-byte node of the radius-weight map
    unsigned char _M_color;
    CStatWeightNode* _M_parent;
    CStatWeightNode* _M_left;
    CStatWeightNode* _M_right;
    long _M_key;
    float _M_value;
};
class CHeroAbilityStatWeight { public:                        // retail 16 bytes
    CRetailMap<CStatWeightNode> Weights;               // +0x00
    float Weight;                                      // +0x0c (zeroed by the def ctor body)
};
class CFloatRange { public:
    float Min;
    float Max;
};
class CDefString { public:
    long m_tablePos;
    CDefString() : m_tablePos(-1) {}
};
class CWoundMorphs { public:                                  // 0x10 bytes: a vector and two flags (only the second is set)
    CRetailVector<long> Morphs;
    bool Applied;                                      // +0x0c (left uninitialised)
    bool Dirty;                                        // +0x0d
    CWoundMorphs() : Dirty(false) {}
};
class CRandomAppearanceMorph { public:                        // 0x3c bytes
    unsigned char _body[0x3c];
    CRandomAppearanceMorph();                          // 0x0086C4D0
};
class CExpressionSet { public:                                // 0x10 bytes: a vector and two flags (only the second is set)
    CRetailVector<long> Expressions;
    bool Applied;                                      // +0x0c (left uninitialised)
    bool Dirty;                                        // +0x0d
    CExpressionSet() : Dirty(false) {}
};
class CCreatureStatsDef : public CDefClassBase { public:             // 0x48 bytes, vtable 0x012321AC
    unsigned char _body[0x48 - 0x28];
    CCreatureStatsDef() : CDefClassBase() {}
};
struct SCountBlock;
template <class T> class CCountedPointer { public:
    T*           m_pObject;
    SCountBlock* m_pCount;
    CCountedPointer() : m_pObject(0), m_pCount(0) {}
    void Reset(T* object);
};
class CCreatureDef : public CDefClassBase { public:                  // vtable 0x0125B43C
    CCountedPointer<CWoundMorphs>           PWoundMorphs;             // +0x28
    CCountedPointer<CRandomAppearanceMorph> PRandomAppearanceMorph;   // +0x30
    CCountedPointer<CExpressionSet>         PExpressions;             // +0x38
    CCountedPointer<CCreatureStatsDef>      PStats;                   // +0x40
    CRetailVector<long>                     InitialActions;           // +0x48
    CRetailVector<long>                     InitialAppearanceModifiers;   // +0x54
    CRetailMap<CPhonemeAnimNode>            PhonemeAnim;              // +0x60
    CRetailVector<CDefString>               DialogueVoices;           // +0x6c
    long Inventory;                                    // +0x78
    long ClothingInventory;                            // +0x7c
    long WeaponsInventory;                             // +0x80
    long HeroAbilitiesScreenInventory;                 // +0x84
    long QuestCardScreenInventory;                     // +0x88
    long MapScreenInventory;                           // +0x8c
    long StatsScreenInventory;                         // +0x90
    long MagicScreenInventory;                         // +0x94
    long ExperienceScreenInventory;                    // +0x98
    long TradeScreenInventory;                         // +0x9c
    long AugmentScreenInventory;                       // +0xa0
    long QuestsScreenInventory;                        // +0xa4
    long InGameMenuScreen;                             // +0xa8
    unsigned long CreatureGroup;                       // +0xac
    CDefString BattleCrySound;                         // +0xb0
    long OpinionSourceDef;                             // +0xb4
    long RespawnWaitInDaysMin;                         // +0xb8
    long RespawnWaitInDaysMax;                         // +0xbc
    long RespawnWaitInFramesMin;                       // +0xc0
    long RespawnWaitInFramesMax;                       // +0xc4
    bool Short;                                        // +0xc8
    bool FlashOnHit;
    bool PickPocketable;
    bool ChildThatCanRegionFollow;
    bool AllowedInProtectedTowns;                      // +0xcc
    bool UseActualHitPosForHitEffects;
    CHeroAbilityStatWeight RadiusWeight;               // +0xd0
    CFloatRange RadiusSpread;                          // +0xe0
    bool AGhostIsWhatIBe;                              // +0xe8
    CCreatureDef();
};
template <> void CCountedPointer<CWoundMorphs>::Reset(CWoundMorphs* object);                     // 0x00675060
template <> void CCountedPointer<CRandomAppearanceMorph>::Reset(CRandomAppearanceMorph* object); // 0x00676840
template <> void CCountedPointer<CExpressionSet>::Reset(CExpressionSet* object);                 // 0x006750D0
template <> void CCountedPointer<CCreatureStatsDef>::Reset(CCreatureStatsDef* object);           // 0x00673500

CCreatureDef::CCreatureDef()
    : CDefClassBase()
{
    RadiusWeight.Weight = 0.0f;
    RadiusSpread.Min = 0.0f;
    PWoundMorphs.Reset(new CWoundMorphs());
    PRandomAppearanceMorph.Reset(new CRandomAppearanceMorph());
    PExpressions.Reset(new CExpressionSet());
    PStats.Reset(new CCreatureStatsDef());
}