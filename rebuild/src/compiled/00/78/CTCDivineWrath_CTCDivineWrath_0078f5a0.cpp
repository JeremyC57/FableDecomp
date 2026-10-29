// CTCDivineWrath::~CTCDivineWrath() -- divine-wrath spell component dtor: return the environment to its default theme unless the spell finished, kill a live obstruction effect, then the member dtors in reverse order (four thing pointers, the hit-effect vector, the creatures-hit map, the target pointer, the will-button timer, the dropped-item pointer, the def pointer) and ~CTCBase
// Leaf: ??1CTCDivineWrath@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0078F5A0, 316 bytes, tail jump to the base destructor.
// Identity: the first store is vtable 0x0126BE84, RTTI `.?AVCTCDivineWrath@@` (rtti_bases: CTCDivineWrath : CTCBase); its
//   slot 0 is the scalar deleting dtor 0x007934D0, the only caller (call at 0x007934D3). Ego_r.pdb has
//   `??1CTCDivineWrath@@UAE@XZ`.
// Callees: CTCBase::GetWorld 0x00686A70 (Ego_r `?GetWorld@CTCBase@@IBEAAVCWorld@@XZ`), CEnvironment::
//   TransitionToDefaultThemeAllChannels(long, float) 0x006B3950 (Ego_r `?TransitionToDefaultThemeAllChannels@CEnvironment@@QAEXJM@Z`;
//   ecx = CWorld +0x18, the scoped_ptr<CEnvironment> PEnvironment behind the inline CWorld::GetEnvironment()),
//   CBaseIntelligentPointer::GetPItem 0x00A01B50, CThing::Kill(bool) 0x004C9B80, ~CBaseIntelligentPointer 0x00A01C10,
//   _Rb_tree<CThingCreatureBase*, pair<CThingCreatureBase* const, PerCreatureInfo>, ...>::_M_erase 0x00792300 (bound so), free
//   0x00BFEA14 (vector blocks and the map header node), CTCBase::~CTCBase 0x00686830 (tail jump); the def's slot 1 on its last
//   release.
// Layout: Ego_r.pdb CTCDivineWrath with retail STLport sizes (vector/map 0xC, CWillButtonHelperTimer 0x34): PDef +0x0C,
//   PItemRemoved +0x10, Timer +0x1C (PCreature +0x24, timings vector +0x34), Finished +0x64, PTarget +0x6D (packed),
//   LastFrameCreaturesHit +0x78, HitEffects +0x84 (0x14-byte PerEffectInfo), BuildUpEffect +0x9C, HandEffect1 +0xAC,
//   HandEffect2 +0xB4, PObstruction +0xC0. No engine header.
// Shape: user-written dtor (body: the `if (!Finished)` theme reset with 0.001f, then IS_THING_ALIVE + Kill(true)); STLport
//   ~_Rb_tree is the inline clear() (_M_erase(root), header links reset) then the header node free.
extern "C" void __cdecl free(void* block);                  // 0x00BFEA14

namespace std {
template <class _Tp> class allocator { public:
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};
template <class _Tp, class _Alloc> class _STLP_alloc_proxy : public _Alloc { public:
    _Tp _M_data;
};
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _ForwardIterator> inline void _Destroy(_ForwardIterator __first, _ForwardIterator __last)
{
    for (; __first != __last; ++__first)
        _Destroy(&*__first);
}
template <class _Tp, class _Alloc> class _Vector_base { public:
    _Tp* _M_start;                                          // +0x0
    _Tp* _M_finish;                                         // +0x4
    _STLP_alloc_proxy<_Tp*, _Alloc> _M_end_of_storage;      // +0x8
    ~_Vector_base()
    {
        if (_M_start != 0)
            _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start);
    }
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> { public:
    ~vector() { _Destroy(this->_M_start, this->_M_finish); }
};
template <class _Tp> struct less { };
template <class _T1, class _T2> struct pair { _T1 first; _T2 second; };
template <class _Pair> struct _Select1st { };
struct _Rb_tree_node_base {
    bool                _M_color;                           // +0x0
    _Rb_tree_node_base* _M_parent;                          // +0x4
    _Rb_tree_node_base* _M_left;                            // +0x8
    _Rb_tree_node_base* _M_right;                           // +0xC
};
template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base { _Value _M_value_field; };
template <class _Tp, class _Alloc> class _Rb_tree_base { public:
    ~_Rb_tree_base() { _M_header.deallocate(_M_header._M_data, 1); }
    _STLP_alloc_proxy<_Rb_tree_node<_Tp>*, allocator<_Rb_tree_node<_Tp> > > _M_header;   // +0x0
};
template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
class _Rb_tree : public _Rb_tree_base<_Value, _Alloc> { public:
    typedef _Rb_tree_node<_Value>* _Link_type;
    ~_Rb_tree() { clear(); }
    _Link_type& _M_root() const { return (_Link_type&)this->_M_header._M_data->_M_parent; }
    _Link_type& _M_leftmost() const { return (_Link_type&)this->_M_header._M_data->_M_left; }
    _Link_type& _M_rightmost() const { return (_Link_type&)this->_M_header._M_data->_M_right; }
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
    unsigned int _M_node_count;                             // +0x4
    _Compare     _M_key_compare;                            // +0x8
private:
    void _M_erase(_Link_type __x);                          // out of line (0x00792300 for CTCDivineWrath)
};
template <class _Key, class _Tp, class _Compare = less<_Key>, class _Alloc = allocator<pair<const _Key, _Tp> > >
class map { public:
    typedef pair<const _Key, _Tp> value_type;
    _Rb_tree<_Key, value_type, _Select1st<value_type>, _Compare, _Alloc> _M_t;
};
} // namespace std

class CBaseClass { public:
    virtual ~CBaseClass();                                  // 0x0099A2E0
};
class CBaseClassNonCopyable : public CBaseClass { public:
protected:
    virtual ~CBaseClassNonCopyable();                       // 0x0099A300
};
class CBaseObjectPointer;
class CBaseObject;
class CThing { public:                                      // retail flag byte +0x91 (Dead bit 0)
    unsigned char _pad_0x00[0x91];
    bool          Dead : 1;                                 // +0x91 bit 0
    bool IsDead() const { return Dead; }
    void Kill(bool immediately);                            // 0x004C9B80
};

#pragma pack(push, 1)
class CBaseIntelligentPointer { public:                     // vtable 0x0129C95C, 8 bytes
    virtual ~CBaseIntelligentPointer();                     // 0x00A01C10
    CBaseObject* GetPItem() const;                          // 0x00A01B50
private:
    CBaseObjectPointer* PData;                              // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:
    T* Get() const { return GetPointer(); }
    T* operator->() const { return GetPointer(); }
private:
    T* GetPointer() const { return (T*)GetPItem(); }
};
#pragma pack(pop)

template <class T> inline bool IS_THING_ALIVE(const CIntelligentPointer<T>& p)
{
    if (p.Get() != 0) {
        if (!p->IsDead())
            return true;
    }
    return false;
}

class CIVCountedPointeeBase { public:
    virtual ~CIVCountedPointeeBase();
    virtual void Delete() const;                            // slot 1 (+0x04)
    mutable unsigned long IVRefCount;                       // +0x04
};
class CResource : public CIVCountedPointeeBase { public:
};
class CDefPointeeBase : public CResource { public:
};
template <class T> class CDefPointer { public:
    CDefPointeeBase* Object;                                // +0x00
    void Release()
    {
        CDefPointeeBase* object = Object;
        if (object) {
            if (--object->IVRefCount == 0)
                object->Delete();
            Object = 0;
        }
    }
    ~CDefPointer() { Release(); }
};

#pragma pack(push, 1)
class C3DVector { public:                                  // retail C3DVector is 1-aligned
    float X;
    float Y;
    float Z;
};
#pragma pack(pop)

class CEnvironment { public:
    void TransitionToDefaultThemeAllChannels(long channel_type, float time);   // 0x006B3950
};
template <class T> class scoped_ptr { public:
    T* ptr;
    T& operator*() const { return *ptr; }
};
class CWorld { public:
    unsigned char           _pad_0x00[0x18];
    scoped_ptr<CEnvironment> PEnvironment;                  // +0x18
    CEnvironment& GetEnvironment() { return *PEnvironment; }
};

class CTCBase : public CBaseClassNonCopyable { public:
    CThing* ParentThing;                                    // +0x04
    bool    ToBeRemoved;                                    // +0x08
    bool    IsInGlobalUpdateTCs;                            // +0x09
    virtual ~CTCBase();                                     // 0x00686830
protected:
    CWorld& GetWorld() const;                               // 0x00686A70
};

class CThingCreatureBase;
class CSpecialAbilitiesDivineWrathDef;
enum EHeroAbility { };                                      // values not needed here
enum EButtonHeldState { };                                  // values not needed here
class CWillButtonHelperTimer { public:                      // retail 0x34 bytes
    long                                    ButtonHeldDownTimer;            // +0x00
    long                                    WillAccumulatedLevel;           // +0x04
    CIntelligentPointer<CThing>             PCreature;                      // +0x08
    EHeroAbility                            MyAbility;                      // +0x10
    EButtonHeldState                        CurrentState;                   // +0x14
    std::vector<float>                      WillPowerUpAccumulatedTimings;  // +0x18
    float                                   MaxNumberLevels;                // +0x24
    float                                   CurMaxLevels;                   // +0x28
    bool                                    WillIncrementedFlag;            // +0x2C
    bool                                    UpdateAccLevel;                 // +0x2D
    bool                                    IsAICasting;                    // +0x2E
    bool                                    IsAIHoldingDown;                // +0x2F
    long                                    LevelToCast;                    // +0x30
};
class CEnginePrimitiveThemeChannelDesc { public:
    unsigned char ChannelIndex0;                            // +0x0
    unsigned char ChannelIndex1;                            // +0x1
    unsigned char ChannelBlendFactor;                       // +0x2
    unsigned char Pad;                                      // +0x3
};

class CTCDivineWrath : public CTCBase { public:            // vtable 0x0126BE84
    struct PerCreatureInfo {
        CIntelligentPointer<CThingCreatureBase> ActualCreature;     // +0x00
        long                                    FramesToGetHit;     // +0x08
        CIntelligentPointer<CThing>             Effect;             // +0x0C
        CEnginePrimitiveThemeChannelDesc        DefaultChannel;     // +0x14
    };
    struct PerEffectInfo {
        C3DVector                               Position;           // +0x00
        CIntelligentPointer<CThing>             Effect;             // +0x0C
    };
    CDefPointer<const CSpecialAbilitiesDivineWrathDef>          PDef;                           // +0x0C
    CIntelligentPointer<CThing>                                 PItemRemoved;                   // +0x10
    bool                                                        DivineWrathActionInitialised;   // +0x18
    CWillButtonHelperTimer                                      Timer;                          // +0x1C
    long                                                        SpellLevel;                     // +0x50
    long                                                        NumFramesBetweenDamageHit;      // +0x54
    long                                                        DurationOfSpellFrames;          // +0x58
    long                                                        NumFramesForEnvChange;          // +0x5C
    long                                                        CurFramesForEnvChange;          // +0x60
    bool                                                        Finished;                       // +0x64
    long                                                        NumFramesToWaitForFadeOut;      // +0x68
    bool                                                        CutSceneCancel;                 // +0x6C
    CIntelligentPointer<CThingCreatureBase>                     PTarget;                        // +0x6D
    bool                                                        IsZTargetting;                  // +0x75
    bool                                                        PerformedReleaseAction;         // +0x76
    std::map<CThingCreatureBase*, PerCreatureInfo>              LastFrameCreaturesHit;          // +0x78
    std::vector<PerEffectInfo>                                  HitEffects;                     // +0x84
    float                                                       PerFrameStaminaModify;          // +0x90
    float                                                       AccModifyStamina;               // +0x94
    long                                                        StaminaModify;                  // +0x98
    CIntelligentPointer<CThing>                                 BuildUpEffect;                  // +0x9C
    CEnginePrimitiveThemeChannelDesc                            HeroDefaultChannel;             // +0xA4
    CEnginePrimitiveThemeChannelDesc                            SpellExclusionChannel;          // +0xA8
    CIntelligentPointer<CThing>                                 HandEffect1;                    // +0xAC
    CIntelligentPointer<CThing>                                 HandEffect2;                    // +0xB4
    unsigned long                                               LoopingSound;                   // +0xBC
    CIntelligentPointer<CThing>                                 PObstruction;                   // +0xC0
    virtual ~CTCDivineWrath();
};

CTCDivineWrath::~CTCDivineWrath()
{
    if (!Finished)
        GetWorld().GetEnvironment().TransitionToDefaultThemeAllChannels(2, 0.001f);
    if (IS_THING_ALIVE(PObstruction))
        PObstruction->Kill(true);
}