// Behaviour test for 0x006768F0: CCreatureDef::~CCreatureDef() -- the creature definition's destructor: free the radius-weight map, the DialogueVoices vector, the PhonemeAnim map and the two action/modifier vectors, release the PStats / PExpressions / PRandomAppearanceMorph / PWoundMorphs counted pointers, then run the CDefPointeeBase / CResource base destructors
// A copy of the source model and logic (compiled separately from src/006768f0.cpp) with test stand-ins for the callees.
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <new>
#if !defined(_MSC_VER)
#define __fastcall
#define __forceinline inline
#define __declspec(x)
#define __cdecl
#define __fastcall
#define __int64 long long
#endif

class CResourceList;
class CResource {                                      // 0x1c bytes (Ego_r.pdb)
public:
    virtual ~CResource();                              // 0x009FC550
    int            _padding_04;                        // +0x04
    CResourceList* ResourceList;                       // +0x08
    CResource*     PrevResource;                       // +0x0c
    CResource*     NextResource;                       // +0x10
    unsigned long  ResourceSize;                       // +0x14
    unsigned long  LastUsedFrame;                      // +0x18
};

class CDefPointeeBase : public CResource {             // vtable 0x01230BA0
public:
    virtual ~CDefPointeeBase() { }
};

class CDefinitionManager;
class CDefClassBase : public CDefPointeeBase {
public:
    CDefinitionManager* PDefManager;                   // +0x1c
    unsigned long       GlobalIndex;                   // +0x20
    unsigned char       DefaultValsApplied : 1;        // +0x24
    unsigned char       Setup : 1;
    unsigned char       Flag2 : 1;
};

class CCPPointerInfo { public:
    unsigned long RefCount;                            // +0x00
    void (__fastcall* DeleteFunc)(void* data);         // +0x04
    void*         Data;                                // +0x08
};

template <class T> class CCountedPointer {
public:
    T*              Data;                              // +0x00
    CCPPointerInfo* Info;                              // +0x04
    ~CCountedPointer()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
        Data = 0;
        Info = 0;
    }
};

class CCharString {
public:
    void* PStringData;
};

class CDefString {
public:
    long TablePos;
};

enum EHeroTrainableStatType { HERO_TRAINABLE_STAT_NONE };

namespace std {
template <class _Tp> class allocator {
public:
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};

template <class _Tp> struct less { };

template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
};

struct _Rb_tree_node_base {
    bool                _M_color;
    _Rb_tree_node_base* _M_parent;
    _Rb_tree_node_base* _M_left;
    _Rb_tree_node_base* _M_right;
};

template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base {
    _Value _M_value_field;
};

template <class _Tp, class _Alloc> struct _Rb_tree_base {
    typedef _Rb_tree_node<_Tp> _Node;
    ~_Rb_tree_base() { _M_header.deallocate(_M_header._M_data, 1); }
    _STLP_alloc_proxy<_Node*, _Node, allocator<_Node> > _M_header;
};

template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
class _Rb_tree : public _Rb_tree_base<_Value, _Alloc> {
public:
    typedef _Rb_tree_node_base* _Base_ptr;
    typedef _Rb_tree_node<_Value> _Node;
    typedef _Node* _Link_type;
    unsigned int _M_node_count;
    _Compare _M_key_compare;

    _Link_type& _M_root() const { return (_Link_type&)this->_M_header._M_data->_M_parent; }
    _Link_type& _M_leftmost() const { return (_Link_type&)this->_M_header._M_data->_M_left; }
    _Link_type& _M_rightmost() const { return (_Link_type&)this->_M_header._M_data->_M_right; }

    void _M_erase(_Link_type __x);
    void clear()
    {
        if (_M_node_count != 0) {
            _M_erase(_M_root());
            _M_leftmost() = this->_M_header._M_data;
            _M_root() = 0;
            _M_rightmost() = this->_M_header._M_data;
            _M_node_count = 0;
        }
    }
    ~_Rb_tree() { clear(); }
};

template <class _Pair> struct _Select1st { };

template <class _Key, class _Tp, class _Compare = less<_Key>, class _Alloc = allocator<pair<const _Key, _Tp> > >
class map {
public:
    typedef pair<const _Key, _Tp> value_type;
    _Rb_tree<_Key, value_type, _Select1st<value_type>, _Compare, _Alloc> _M_t;
};

template <class _Tp, class _Alloc> class _Vector_base {
public:
    ~_Vector_base()
    {
        if (_M_start != 0)
            _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start);
    }
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
};

template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    ~vector() { }
};
}

class CWoundMorphs;
class CRandomAppearanceMorph;
class CExpressionSet;
class CCreatureStatsDef;

class CHeroAbilityStatWeight {
public:
    std::map<EHeroTrainableStatType, float> StatWeights;   // +0x00
    float Weight;                                          // +0x0c
};

class CFloatRange {
public:
    float Min;
    float Max;
};

class CCreatureDef : public CDefClassBase {            // vtable 0x0125B43C
public:
    CCountedPointer<CWoundMorphs>           PWoundMorphs;             // +0x28
    CCountedPointer<CRandomAppearanceMorph> PRandomAppearanceMorph;   // +0x30
    CCountedPointer<CExpressionSet>         PExpressions;             // +0x38
    CCountedPointer<CCreatureStatsDef>      PStats;                   // +0x40
    std::vector<long>                       InitialActions;           // +0x48
    std::vector<long>                       InitialAppearanceModifiers;   // +0x54
    std::map<CCharString, long>             PhonemeAnim;              // +0x60
    std::vector<CDefString>                 DialogueVoices;           // +0x6c
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
    virtual ~CCreatureDef();
};

CCreatureDef::~CCreatureDef()
{
}

// ---- test stand-ins ----
static int g_resourceDtors = 0, g_erased = 0, g_deleted = 0;
static void* g_deletedData[4];
CResource::~CResource() { ++g_resourceDtors; }
namespace std {
template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
void _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::_M_erase(_Link_type __x)
{                                                         // STLport body: erase the right subtree, free, walk left
    while (__x != 0) {
        _M_erase((_Link_type)__x->_M_right);
        _Link_type __y = (_Link_type)__x->_M_left;
        ++g_erased;
        std::free(__x);
        __x = __y;
    }
}
}
static void __fastcall DeleteData(void* data) { g_deletedData[g_deleted++ & 3] = data; }

typedef std::_Rb_tree_node<std::pair<const CCharString, long> > PhonemeNode;
typedef std::_Rb_tree_node<std::pair<const EHeroTrainableStatType, float> > WeightNode;
template <class Node> Node* NewNode(Node* parent)
{
    Node* n = (Node*)std::malloc(sizeof(Node));
    n->_M_color = false; n->_M_parent = parent; n->_M_left = n->_M_right = 0;
    return n;
}
template <class T> void Counted(CCountedPointer<T>& p, unsigned long refs, void* data)
{
    p.Data = (T*)data;
    if (!refs) { p.Info = 0; return; }
    p.Info = new CCPPointerInfo;
    p.Info->RefCount = refs; p.Info->DeleteFunc = DeleteData; p.Info->Data = data;
}
template <class T> void Storage(std::vector<T>& v, unsigned int n)
{
    v._M_start = n ? (T*)std::malloc(n * sizeof(T)) : 0;
    v._M_finish = v._M_end_of_storage._M_data = v._M_start + n;
}

static int Fail(int line) { std::printf("006768f0_TEST FAIL (line %d)\n", line); return 1; }
#define CHECK(c) if (!(c)) return Fail(__LINE__)

int main()
{
    static char wound, morph, expressions, stats;
    CCreatureDef* def = new CCreatureDef;
    Counted(def->PWoundMorphs, 1, &wound);                     // last reference: deleted
    Counted(def->PRandomAppearanceMorph, 2, &morph);           // shared: survives
    Counted(def->PExpressions, 0, &expressions);               // no info block
    Counted(def->PStats, 1, &stats);
    CCPPointerInfo* sharedInfo = def->PRandomAppearanceMorph.Info;
    Storage(def->InitialActions, 3); Storage(def->InitialAppearanceModifiers, 0); Storage(def->DialogueVoices, 2);
    PhonemeNode* header = NewNode<PhonemeNode>(0);             // map with three nodes
    PhonemeNode* root = NewNode<PhonemeNode>(header);
    root->_M_left = NewNode<PhonemeNode>(root); root->_M_right = NewNode<PhonemeNode>(root);
    header->_M_parent = root; header->_M_left = root->_M_left; header->_M_right = root->_M_right;
    def->PhonemeAnim._M_t._M_header._M_data = header; def->PhonemeAnim._M_t._M_node_count = 3;
    WeightNode* weights = NewNode<WeightNode>(0);              // an empty map: only its header
    weights->_M_left = weights->_M_right = weights;
    def->RadiusWeight.StatWeights._M_t._M_header._M_data = weights; def->RadiusWeight.StatWeights._M_t._M_node_count = 0;

    delete def;                                                // virtual destructor chain
    CHECK(g_resourceDtors == 1);
    CHECK(g_erased == 3);                                      // only the populated map erases nodes
    CHECK(g_deleted == 2 && g_deletedData[0] != g_deletedData[1]);
    CHECK((g_deletedData[0] == &wound || g_deletedData[0] == &stats) && (g_deletedData[1] == &wound || g_deletedData[1] == &stats));
    CHECK(sharedInfo->RefCount == 1);
    delete sharedInfo;
    std::printf("006768f0_TEST PASS\n");
    return 0;
}