// CTCDExperienceOrb::OnCollected(CThing& collector) -- an experience orb is picked up: scale its points by the collector's pickup multiplier, spawn the def's pickup particle effect for that many points at the orb, play the orb's PICKUP_SOUND through the collector's sound player, then kill the orb
// Leaf: ?OnCollected@CTCDExperienceOrb@@QAEXAAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0077A180, 455 bytes, thiscall `ret 4`. Callers 0x006AD6F0 / 0x007C1991 pass the collecting
//   thing. Ego_r.pdb ?OnCollected@CTCDExperienceOrb@@QAEXAAVCThing@@@Z (the catalog label is right).
// Callees: CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound const 0x004365B0 (PeekTC of interface 0x68, CTCHeroExperience),
//   CTCHeroExperience::GetExperiencePickUpMultiplier() const 0x006D6570, CThing::GetPos() const 0x004C73D0,
//   CTCDParticleEmitter::Create(long, const C3DVector&, bool) 0x006E0880 (static __fastcall), the non-const LowerBound inline
//   (GetTC of interface 0x10, CTCSoundPlayer), CDefClassBase::GetSubDef<CEntitySoundDef>(CDefPointer<const CEntitySoundDef>*)
//   const 0x0065F604, CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0, CSoundMap::GetPSoundPair 0x0065FBA4,
//   CTCSoundPlayer::PlayAttachedSample(const CSoundPair&, const CSoundInit&, bool, bool, bool) 0x0066B280, CThing::Kill(bool)
//   0x004C9B80; the sound def's release through engine/CDefPointer.h (CResource refcount +4, Delete slot 1).
// Layout: Ego_r.pdb CTCDExperienceOrb: ParentThing +0x04, PDef +0x0C (CDefPointer<const CExperienceDef>), ExperiencePoints +0x10;
//   CExperienceDef PickupParticleEffectsPerExperiencePoints +0x38 (std::map<long, long>, STLport 12 bytes, after
//   OrbCombiningParticleEffect +0x28 and ParticleScalesPerExperiencePoints +0x2C); CThing interface bits +0x20, interface map
//   +0x44, PBaseDef +0x70 (Ego_r +0x80 less the 0x10 the retail CVectorMap saves); CEntitySoundDef SoundMap +0x28.
class CCharStringData;
class CCharString { public:
    CCharString(const char* text, long length = -1);       // 0x0099EBF0 ??0CCharString@@QAE@PBDJ@Z
    ~CCharString();                                         // 0x0099EAE0
    CCharStringData* PStringData;
};
// Shape: the same-TU CDefClassBase::GetSubDef<T> body keeps the sound def in esi across the calls (and lets the interface
//   key share the dead collector slot); the component lookups go through a map pointer; the pickup loop re-reads
//   GetDef().PickupParticleEffectsPerExperiencePoints for begin()/end(); "PICKUP_SOUND" converts implicitly (the
//   CCharString(const char*, long = -1) default), so the arg address is re-taken after the ctor.
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CSubDefClassBase.h"
#include "engine/CDefPointer.h"

class CDefinitionManager {
    friend class CDefClassBase;
protected:
    CDefPointer<CDefClassBase> GetPDefFromGlobalIndex(long index);   // 0x009ADA10
};
template <class T> bool CDefClassBase::GetSubDef(CDefPointer<const T>* out) const   // <CEntitySoundDef> 0x0065F604
{
    const CSubDefInfo* info = GetPSubDefInfo(T::StatGetTypeName());
    if (info) {
        CDefPointer<const CDefClassBase> def = PDefManager->GetPDefFromGlobalIndex(info->DefIndex);
        out->SetDynamicCast(def);
        return true;
    }
    return false;
}
class C3DVector { public: float X, Y, Z; };

namespace std {
// STLport 4.6 stl/_pair.h, stl/_function_base.h, stl/_tree.h (the members this row needs)
template <class _T1, class _T2> struct pair {
    typedef _T1 first_type;
    typedef _T2 second_type;
    _T1 first;
    _T2 second;
};
template <class _Tp> class allocator { public: allocator() { } };
template <class _Arg1, class _Arg2, class _Result> struct binary_function { };
template <class _Tp> struct less : public binary_function<_Tp, _Tp, bool> {
    bool operator()(const _Tp& __x, const _Tp& __y) const { return __x < __y; }
};
template <class _Pair> struct _Select1st {
    const typename _Pair::first_type& operator()(const _Pair& __x) const { return __x.first; }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc { public:
    _Value _M_data;
};
struct _Rb_tree_node_base {
    typedef bool _Color_type;
    typedef _Rb_tree_node_base* _Base_ptr;
    _Color_type _M_color;
    _Base_ptr   _M_parent;
    _Base_ptr   _M_left;
    _Base_ptr   _M_right;
};
template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base {
    _Value _M_value_field;
};
template <class _Dummy> class _Rb_global { public:
    typedef _Rb_tree_node_base* _Base_ptr;
    static _Base_ptr _M_increment(_Base_ptr _M_node)
    {
        if (_M_node->_M_right != 0) {
            _M_node = _M_node->_M_right;
            while (_M_node->_M_left != 0)
                _M_node = _M_node->_M_left;
        }
        else {
            _Base_ptr __y = _M_node->_M_parent;
            while (_M_node == __y->_M_right) {
                _M_node = __y;
                __y = __y->_M_parent;
            }
            if (_M_node->_M_right != __y)
                _M_node = __y;
        }
        return _M_node;
    }
};
typedef _Rb_global<bool> _Rb_global_inst;
struct _Rb_tree_base_iterator {
    typedef _Rb_tree_node_base* _Base_ptr;
    _Base_ptr _M_node;
    _Rb_tree_base_iterator() : _M_node(0) { }
    _Rb_tree_base_iterator(_Base_ptr __x) : _M_node(__x) { }
    void _M_increment() { _M_node = _Rb_global_inst::_M_increment(_M_node); }
};
template <class _Value> struct _Rb_tree_iterator : public _Rb_tree_base_iterator {
    typedef _Rb_tree_iterator<_Value> _Self;
    typedef _Rb_tree_node<_Value>* _Link_type;
    _Rb_tree_iterator() { }
    _Rb_tree_iterator(_Link_type __x) : _Rb_tree_base_iterator(__x) { }
    const _Value& operator*() const { return _Link_type(_M_node)->_M_value_field; }
    const _Value* operator->() const { return &(operator*()); }
    _Self& operator++() { _M_increment(); return *this; }
    bool operator==(const _Self& __y) const { return _M_node == __y._M_node; }
    bool operator!=(const _Self& __y) const { return _M_node != __y._M_node; }
};
template <class _Tp, class _Alloc> struct _Rb_tree_base {
    typedef _Rb_tree_node<_Tp> _Node;
    _STLP_alloc_proxy<_Node*, _Node, allocator<_Node> > _M_header;
};
template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
class _Rb_tree : public _Rb_tree_base<_Value, _Alloc> { public:
    typedef _Rb_tree_node<_Value>* _Link_type;
    typedef _Rb_tree_iterator<_Value> const_iterator;
    unsigned int _M_node_count;
    _Compare _M_key_compare;
    _Link_type& _M_leftmost() const { return (_Link_type&)this->_M_header._M_data->_M_left; }
    const_iterator begin() const { return const_iterator(_M_leftmost()); }
    const_iterator end() const { return const_iterator(this->_M_header._M_data); }
};
template <class _Key, class _Tp, class _Compare = less<_Key>, class _Alloc = allocator<pair<const _Key, _Tp> > >
class map { public:
    typedef pair<const _Key, _Tp> value_type;
    typedef _Rb_tree<_Key, value_type, _Select1st<value_type>, _Compare, _Alloc> _Rep_type;
    typedef typename _Rep_type::const_iterator const_iterator;
    const_iterator begin() const { return _M_t.begin(); }
    const_iterator end() const { return _M_t.end(); }
    _Rep_type _M_t;
};
} // namespace std

class CTCBase;
enum ETCInterfaceType { };
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class C> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:
    typedef std::pair<ETCInterfaceType, CTCBase*> value_type;
    value_type* _M_start;                                   // +0x00 (the STLport vector)
    value_type* _M_finish;                                  // +0x04
    value_type* _M_end_of_storage;                          // +0x08
    unsigned char Compare;                                  // +0x0C
    const value_type* LowerBound(const ETCInterfaceType& key) const;   // 0x004365B0
    value_type* LowerBound(const ETCInterfaceType& key)                // 0x0040F020 (inline here)
    {
        long count = _M_finish - _M_start;
        value_type* first = _M_start;
        while (count > 0) {
            long half = count / 2;
            value_type* middle = first + half;
            if (middle->first < key) {
                first = middle + 1;
                count = count - half - 1;
            } else
                count = half;
        }
        return first;
    }
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CSoundPair { public: long FirstSound; long LastSound; };
class CSoundInit { public:
    float         Volume;
    float         Pitch;
    float         Pan;
    long          NoPlays;
    long          PlaybackType;
    unsigned long GainFaderBits;
    unsigned long PitchFaderBits;
    long          OverrideID;
    CSoundInit() : Volume(1.0f), Pitch(1.0f), Pan(0.0f), NoPlays(1), GainFaderBits(0), PitchFaderBits(0), OverrideID(-1)
    {
        PlaybackType = 1;
    }
};
class CSoundMap { public:
    const CSoundPair* GetPSoundPair(const CCharString& name) const;   // 0x0065FBA4
    unsigned long Data[8];
};
class CEntitySoundDef : public CSubDefClassBase { public:
    static CCharString __fastcall StatGetTypeName() { return CCharString("CEntitySoundDef", -1); }
    CSoundMap SoundMap;                                     // +0x28
};
class CThingBaseDef : public CDefClassBase { };
class CExperienceDef : public CDefClassBase { public:
    long OrbCombiningParticleEffect;                        // +0x28
    std::map<long, float> ParticleScalesPerExperiencePoints;          // +0x2C
    std::map<long, long> PickupParticleEffectsPerExperiencePoints;    // +0x38
    std::map<long, float> LifeTimeForDistanceThresholds;              // +0x44
};

class CTCHeroExperience { public:
    long GetExperiencePickUpMultiplier() const;             // 0x006D6570
};
class CTCSoundPlayer { public:
    unsigned long PlayAttachedSample(const CSoundPair& pair, const CSoundInit& init, bool attach, bool loop, bool stream);   // 0x0066B280
};
class CThing { public:
    const C3DVector& GetPos() const;                        // 0x004C73D0
    void Kill(bool removeNow);                              // 0x004C9B80
    unsigned char _pad00[0x20];
    unsigned long Components_present[9];                    // +0x20 (std::bitset of interface types)
    CInterfaceMap Components;                               // +0x44
    unsigned char _pad54[0x70 - 0x54];
    CDefPointer<const CThingBaseDef> PBaseDef;              // +0x70
    __forceinline unsigned long HasTC(long type) const
    {
        return Components_present[type >> 5] & (1UL << (type & 31));
    }
    template <class T> __forceinline const T* PeekTC(long type) const
    {
        const CInterfaceMap* map = &Components;
        const CInterfaceMap::value_type* it = map->LowerBound((ETCInterfaceType)type);
        return (const T*)((it == map->_M_finish || type < it->first) ? map->_M_finish : it)->second;
    }
    template <class T> __forceinline T* GetTC(long type)
    {
        ETCInterfaceType key = (ETCInterfaceType)type;
        CInterfaceMap* map = &Components;
        CInterfaceMap::value_type* it = map->LowerBound(key);
        return (T*)((it == map->_M_finish || type < it->first) ? map->_M_finish : it)->second;
    }
};
class CTCDParticleEmitter { public:
    static CThing* __fastcall Create(long effect, const C3DVector& position, bool attach);   // 0x006E0880
};

class CTCDExperienceOrb { public:
    virtual ~CTCDExperienceOrb();
    CThing* ParentThing;                                    // +0x04
    unsigned char _pad08[0x4];
    CDefPointer<const CExperienceDef> PDef;                 // +0x0C
    long ExperiencePoints;                                  // +0x10
    long CombinationImmunityTimer;                          // +0x14
    long LifeTimer;                                         // +0x18
    bool DecayOverTime;                                     // +0x1C
    bool PickedUp;                                          // +0x1D
    void OnCollected(CThing& collector);
private:
    const CExperienceDef& GetDef() const { return *PDef; }
};

void CTCDExperienceOrb::OnCollected(CThing& collector)
{
    long experience = ExperiencePoints;
    if (collector.HasTC(0x68))
        experience *= collector.PeekTC<CTCHeroExperience>(0x68)->GetExperiencePickUpMultiplier();
    long effect = 0;
    for (std::map<long, long>::const_iterator it = GetDef().PickupParticleEffectsPerExperiencePoints.begin();
         it != GetDef().PickupParticleEffectsPerExperiencePoints.end(); ++it) {
        if (experience < it->first)
            break;
        effect = it->second;
    }
    CTCDParticleEmitter::Create(effect, ParentThing->GetPos(), false);
    CDefPointer<const CEntitySoundDef> soundDef;
    if (collector.HasTC(0x10)) {
        CTCSoundPlayer* player = collector.GetTC<CTCSoundPlayer>(0x10);
        if (ParentThing->PBaseDef->GetSubDef(&soundDef)) {
            const CSoundPair* pair = soundDef->SoundMap.GetPSoundPair("PICKUP_SOUND");
            if (pair)
                player->PlayAttachedSample(*pair, CSoundInit(), true, false, false);
        }
    }
    ParentThing->Kill(true);
}