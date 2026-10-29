// CAIStateGroup_ScorpionKing::Update() -- the scorpion king's AI by boss phase: in the fighting phases (0, 2, 4) recover first if he is in creature mode 0x38, fight (UpdateCombat) and advance the phase when the boss is ready; in phases 1 and 3 run the phase-2 behaviour and queue phase 2 / 4; terminate once the state group stops being runnable
// Grade: DIFFER(308v383) -- re-split at 0x0097C720: the row runs on past the jump table into three other functions; the true extent (code to `ret` + `mov edi,edi` + the 5-entry table, 0x0097C5E0..0x0097C714) is byte-exact with relocations masked (tools/pfx2.py: 308 bytes, 21 relocs, 0 mismatches)
// Leaf: ?Update@CAIStateGroup_ScorpionKing@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0097C5E0, 286 bytes of code + `mov edi,edi` + the jump table 0x0097C700 (5 entries), `ret`.
//   The catalog row (383 bytes) also holds 0x0097C720 (after 12 int3: a 16-byte ctor-like body storing vtable 0x012989D4), 0x0097C730
//   (`mov eax,25h ; ret`) and 0x0097C740 (a scalar deleting dtor storing 0x0124F2C4).
// Identity: Ego_r.pdb `?Update@CAIStateGroup_ScorpionKing@@UAEXXZ`; donor order IsStillRunnable / ... / UpdateCombat /
//   UpdatePhase2 / Update (0x009A0500 / 0x009A0B40 / 0x009A14A0 / 0x009A1500) = retail 0x0097B870 / 0x0097BDA0 / 0x0097C580 /
//   0x0097C5E0 (Update - UpdatePhase2 = 0x60 in both). Jump table 0x0097C700 checked entry by entry: phases 0, 2, 4 ->
//   0x0097C641 (fight), 1 -> 0x0097C6C3, 3 -> 0x0097C6D0.
// Callees: CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound 0x0040F020 (inline CThing::GetTC<T>(T**); the boss lookup result is
//   used unchecked, the miss path reads the never-written out slot), CTCBoss::GetBossPhase() const 0x007AED10,
//   IsReadyForPhaseChange() const 0x007AEE90, IncrementBossPhase() 0x007AED20, SetNextBossPhase(long, bool) 0x007AEDA0 (Ego_r.pdb
//   donor order GetBossPhase / IncrementBossPhase (+0x10) / ... / SetNextBossPhase (+0x90) = retail), CTCCreatureModeManager::
//   IsModeActive(unsigned long) const 0x0073A8A0, CCreatureAction_ScorpionRecover::CCreatureAction_ScorpionRecover(
//   CThingCreatureBase&) 0x008E4770 (it stores vtable 0x01286724 `.?AVCCreatureAction_ScorpionRecover@@`; the catalog says
//   GoUnderground), CAIStateGroupBase::AIAction_PerformActionAndWait(const CCreatureActionBase&, bool) 0x0097DB70 (Ego_r.pdb
//   protected `IAE?AW4EAIActionReturn@@ABVCCreatureActionBase@@_N@Z`), CCombatActionBase::~CCombatActionBase 0x0062E900 (the
//   temporary's implicit dtor inlined), the private-to-family UpdateCombat 0x0097BDA0 / UpdatePhase2 0x0097C580 /
//   IsStillRunnable 0x0097B870 (Ego_r.pdb IAE), CAIStateGroupBase::Terminate 0x008FCF40.
// Interface types (vtable slot 21): CTCBoss 0xAD (0x007AFCD0), CTCCreatureModeManager 0x31.
// Layout: the 0x118-byte frame is the boss out slot (+0) and the 0x114-byte CCreatureAction_ScorpionRecover temporary (+4; the
//   retail CCombatActionBase size, no members of its own). CAIBrain Creature +0x18, read through the inline
//   CAIStateGroupBase::GetCreature() (Ego_r.pdb `IBEAAVCThingAICreature@@XZ`; the reference accessor gives retail's eax for the
//   temporary's argument, a raw `*PBrain->Creature` takes edx).
namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
};
template <class _Tp> class allocator { public: allocator() { } };
template <class _Value, class _Tp, class _MaybeReboundAlloc> struct _STLP_alloc_proxy {
    _Value _M_data;
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector {   // retail STLport vector (12 bytes)
public:
    typedef _Tp* iterator;
    _Tp* _M_start;                                            // +0x00
    _Tp* _M_finish;                                           // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;   // +0x08
    iterator end() { return this->_M_finish; }
};
template <unsigned int _Nw> struct _Base_bitset {
    typedef unsigned long _WordT;
    _WordT _M_w[_Nw];
    static unsigned int _S_whichword(unsigned int __pos) { return __pos / 32; }
    static _WordT _S_maskbit(unsigned int __pos) { return 1UL << (__pos % 32); }
};
template <unsigned int _Nb> class bitset : public _Base_bitset<(_Nb + 31) / 32> {
public:
    struct reference {
        typedef _Base_bitset<(_Nb + 31) / 32> _Bitset_base;
        unsigned long* _M_wp;
        unsigned int _M_bpos;
        reference(bitset<_Nb>& __b, unsigned int __pos)
        {
            _M_wp = &__b._M_w[_Bitset_base::_S_whichword(__pos)];
            _M_bpos = __pos % 32;
        }
        ~reference() { }
        operator bool() const { return (*(_M_wp) & _Bitset_base::_S_maskbit(_M_bpos)) != 0; }
    };
    reference operator[](unsigned int __pos) { return reference(*this, __pos); }
};
}   // namespace std

enum ETCInterfaceType { };
class CTCBase;
class CCharString { public:
    void* PStringData;                                      // +0x00
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};
template <class T> class CArray : public std::vector<T> { };
template <class K, class V> class CKeyPairCompareLess { };
template <class K, class V, class TCompare> class CVectorMap {   // retail 0x10 bytes
public:
    typedef std::pair<K, V> value_type;
    CArray<value_type> Data;                                  // +0x00
    TCompare           Compare;                               // +0x0C
    bool               Dirty;                                 // +0x0D
    value_type* LowerBound(const K& key);                     // 0x0040F020 for the TC map
};

class CThing { public:                                      // partial view, retail offsets
    virtual ~CThing();
    typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > TCMap;
    unsigned char      _pad_0x04[0x20 - 0x04];
    std::bitset<274>   UsingComponents;                     // +0x20
    TCMap              Components;                          // +0x44
    template <class T> __forceinline bool GetTC(T** out)
    {
        if (UsingComponents[T::INTERFACE_TYPE]) {
            TCMap* map = &Components;
            TCMap::value_type* it = map->LowerBound((ETCInterfaceType)T::INTERFACE_TYPE);
            if (it == map->Data.end() || T::INTERFACE_TYPE < it->first)
                it = map->Data.end();
            *out = (T*)it->second;
            return true;
        }
        return false;
    }
};
class CThingPhysical : public CThing { public: };
class CThingGameObject : public CThingPhysical { public: };
class CThingCreatureBase : public CThingGameObject { public: };
class CThingAICreature : public CThingCreatureBase { public: };

class CTCBase { public:
    virtual ~CTCBase();                                     // 0x00686830
    CThing* ParentThing;                                    // +0x04
    bool    ToBeRemoved;                                    // +0x08
    bool    IsInGlobalUpdateTCs;                            // +0x09
};
class CTCBoss : public CTCBase { public:                    // interface type 0xAD
    enum { INTERFACE_TYPE = 0xAD };
    long GetBossPhase() const;                              // 0x007AED10
    void IncrementBossPhase();                              // 0x007AED20
    void SetNextBossPhase(long phase, bool immediately);    // 0x007AEDA0
    bool IsReadyForPhaseChange() const;                     // 0x007AEE90
};
class CTCCreatureModeManager : public CTCBase { public:     // interface type 0x31
    enum { INTERFACE_TYPE = 0x31 };
    bool IsModeActive(unsigned long mode) const;            // 0x0073A8A0
};

class CCreatureActionBase { public:
    virtual ~CCreatureActionBase();                         // 0x00693EF0
};
class CCombatActionBase : public CCreatureActionBase { public:   // retail 0x114 bytes
    unsigned char _pad_0x04[0x114 - 0x04];
    virtual ~CCombatActionBase();                           // 0x0062E900
};
class CCreatureAction_ScorpionRecover : public CCombatActionBase { public:
    CCreatureAction_ScorpionRecover(CThingCreatureBase& creature);   // 0x008E4770
};

class CAIBrain { public:
    unsigned char       _pad_0x00[0x18];
    CThingAICreature*   Creature;                           // +0x18
};
enum EScriptAIPriority { };
enum EAIActionReturn { };
class CBaseClass { public:
    virtual ~CBaseClass();                                  // 0x0099A2E0
};
class CAIStateGroupBase : public CBaseClass { public:
    CAIBrain*           PBrain;                             // +0x04
    EScriptAIPriority   ScriptAIPriority;                   // +0x08
    CCharString         Name;                               // +0x0c
    bool                Terminated;                         // +0x10
    unsigned char       Priority;                           // +0x11
    bool                Enabled;                            // +0x12
    void Terminate();                                       // 0x008FCF40
protected:
    CThingAICreature& GetCreature() const { return *PBrain->Creature; }   // Ego_r.pdb ?GetCreature@CAIStateGroupBase@@IBEAAVCThingAICreature@@XZ (inline)
    EAIActionReturn AIAction_PerformActionAndWait(const CCreatureActionBase& action, bool wait);   // 0x0097DB70
};
class CAIStateGroup_ScorpionKing : public CAIStateGroupBase { public:
    virtual void Update();
protected:
    bool IsStillRunnable();                                 // 0x0097B870
    void UpdateCombat();                                    // 0x0097BDA0
    void UpdatePhase2();                                    // 0x0097C580
};

void CAIStateGroup_ScorpionKing::Update()
{
    CTCBoss* boss;
    GetCreature().GetTC(&boss);
    switch (boss->GetBossPhase()) {
    case 0:
    case 2:
    case 4: {
        CTCCreatureModeManager* modes;
        if (GetCreature().GetTC(&modes) && modes->IsModeActive(0x38))
            AIAction_PerformActionAndWait(CCreatureAction_ScorpionRecover(GetCreature()), false);
        UpdateCombat();
        if (boss->IsReadyForPhaseChange())
            boss->IncrementBossPhase();
        break;
    }
    case 1:
        UpdatePhase2();
        boss->SetNextBossPhase(2, false);
        break;
    case 3:
        UpdatePhase2();
        boss->SetNextBossPhase(4, false);
        break;
    }
    if (!IsStillRunnable())
        Terminate();
}
