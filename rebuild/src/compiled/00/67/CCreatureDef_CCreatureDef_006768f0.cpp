// CCreatureDef::~CCreatureDef() -- the creature definition's destructor: free the radius-weight map, the DialogueVoices vector, the PhonemeAnim map and the two action/modifier vectors, release the PStats / PExpressions / PRandomAppearanceMorph / PWoundMorphs counted pointers, then run the CDefPointeeBase / CResource base destructors
// Leaf: ??1CCreatureDef@
// Retail: Fable.exe (TLC, VC7.1) @ 0x006768F0, 352 bytes (349 without padding), thiscall, ends in `jmp` to the base dtor.
//   Vtables: CCreatureDef 0x0125B43C (rtti-vtables.tsv; slot 0 is the landed scalar deleting dtor 0x006768D0, which
//   calls this), CDefPointeeBase 0x01230BA0 (the inline base dtor's store; CDefClassBase's store is dead and dropped).
// Callees: _Rb_tree::_M_erase(root) for map<EHeroTrainableStatType, float> 0x004506ED and map<CCharString, long>
//   0x0043020A (both thiscall; catalog labels "LTextTreeWalkThrough::BuildTreeArray" are propagated across folded
//   bodies), free 0x00BFEA14 (the map header nodes and the vector buffers), operator delete 0x00BFE9BC (the
//   CCPPointerInfo blocks), the CCPPointerInfo::DeleteFunc pointer (__fastcall, ecx = Data), CResource::~CResource
//   0x009FC550 (landed as "~CMemoryBlock": its body unlinks the resource from ResourceList +8, CResource's member).
// Label: catalog "CMemoryBlock_Destructor" -- this is CCreatureDef::~CCreatureDef (non-deleting, Ego_r.pdb members).
// Layout: as the landed CCreatureDef ctor 0x006765A0 (retail STLport vectors and maps are 12 bytes, so the PDB offsets
//   after PhonemeAnim run high): CDefClassBase root, PWoundMorphs +0x28, PRandomAppearanceMorph +0x30, PExpressions
//   +0x38, PStats +0x40, InitialActions +0x48, InitialAppearanceModifiers +0x54, PhonemeAnim +0x60, DialogueVoices
//   +0x6c, ..., RadiusWeight {StatWeights +0xd0, ..}.
// Shape: the implicit member-wise destructor over STLport 4.6 containers: ~_Rb_tree = clear() (count test, _M_erase,
//   leftmost/root/rightmost re-read through the header) + ~_Rb_tree_base (deallocate the header: `if (p) free(p)`);
//   ~_Vector_base frees a non-null buffer; ~CCountedPointer re-reads Info after the decrement, then Data = 0, Info = 0.
extern "C" void __cdecl free(void* p);                 // 0x00BFEA14
void __cdecl operator delete(void* p);                 // 0x00BFE9BC

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

    protected:
    _Link_type& _M_root() const { return (_Link_type&)this->_M_header._M_data->_M_parent; }
    public:
    protected:
    _Link_type& _M_leftmost() const { return (_Link_type&)this->_M_header._M_data->_M_left; }
    public:
    protected:
    _Link_type& _M_rightmost() const { return (_Link_type&)this->_M_header._M_data->_M_right; }
    public:

private:
    void _M_erase(_Link_type __x);
public:
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