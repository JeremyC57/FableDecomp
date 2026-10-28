// CCreatureAction_KrakenLowAttackLoop::FinishAction() -- base FinishAction; while the target lives and loops remain (CKrakenDef::LowStrikeAttackLoops), queue the next low-attack loop (jumping to the last two loops when the action was cut short, ending the attack beam on the second-last), else queue the low-attack out-of animation
// Leaf: ?FinishAction@CCreatureAction_KrakenLowAttackLoop@?A0x
// Retail: Fable.exe (TLC, VC7.1) @ 0x00924520, 334 bytes, thiscall, plain `ret` (virtual).
// Identity (proven): RTTI vtable 0x01290C64 (.?AVCCreatureAction_KrakenLowAttackLoop@?A0x29922355@@) slot 4 = FinishAction
//   (slot 4 of every CCreatureActionBase vtable); Ego_r.pdb ?FinishAction@CCreatureAction_KrakenLowAttackLoop@?A0xe2035276@@UAEXXZ
//   (donor 0x009477D0). The catalog's "vfunc_4" is that slot. The anonymous-namespace hash differs per build (retail RTTI
//   ?A0x29922355): the Leaf is the prefix.
// Callees: CCreatureActionBase::FinishAction 0x00694430, CBaseIntelligentPointer::GetPItem 0x00A01B50, CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound
//   0x0040F020 (inline CThing::GetTC<CTCKraken>, TC_KRAKEN = 0xE2), CTCKraken::EndAttackBeam 0x007FC800, the anonymous-namespace ctors
//   CCreatureAction_KrakenLowAttackLoop(CThingCreatureBase&, CThing&, long, bool) 0x009255E0 and
//   CCreatureAction_KrakenLowAttackOutOf(CThingCreatureBase&) 0x00925680 (Ego_r / FableWin), CThingCreatureBase::
//   SetCurrentAction(const CCreatureActionBase&) 0x006644F0, CCombatActionBase::~CCombatActionBase 0x0062E900.
// Shape: CThing::GetTC<CTCKraken>(&kraken) (Ego_r `??$GetTC@VCTCKraken@@@CThing@@QAE_NPAPAVCTCKraken@@@Z`, inline) with its
//   result ignored (retail reads the uninitialised slot when the kraken TC is missing). GetTC passes the interface key to
//   LowerBound as a temporary, compares the found key against the constant and keeps a reference to the map (&Components in
//   edi): the key temporary then shares the kraken slot [esp+10h] by lifetime (2026-10-28 retry; a named key or key copy is a
//   scoped local with its own slot, frame 0x124); the new action is a temporary bound to SetCurrentAction's reference (one 0x11C-byte
//   frame slot for either), destroyed through the implicit derived destructor (direct ~CCombatActionBase call).
// Layout: LowAttackLoop Interrupt +0x114 / LoopNum +0x118 (Ego_r +0x12C / +0x130); CCombatActionBase PTarget +0xA8;
//   CCreatureActionBase TotalTime +0x10 / FramesRunning +0x1C; CTCKraken PKrakenDef +0x0C; CKrakenDef LowStrikeAttackLoops
//   +0x40 (Ego_r +0x40; the high-attack sibling 0x00924930 reads HighStrikeAttackLoops +0x44).
// Device: KrakenLowAttackLoop_FinishAction_Use is not a retail function; it only keeps the anonymous-namespace body emitted.
class CTCBase;
enum ETCInterfaceType { TC_KRAKEN = 226 };

class CKrakenDef {
public:
    unsigned char _pad_0x00[0x40];
    long LowStrikeAttackLoops;                               // +0x40
};

class CTCKraken {
public:
    enum { InterfaceType = TC_KRAKEN };
    unsigned char _pad_0x00[0x0c];
    const CKrakenDef* PKrakenDef;                            // +0x0C CDefPointer<const CKrakenDef>
    void EndAttackBeam();                                    // 0x007FC800
};

namespace std {
template <class _T1, class _T2> struct pair { _T1 first; _T2 second; };
template <class _Tp> class allocator { };
template <class _Tp, class _Alloc = allocator<_Tp> > class vector {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;
};
}
template <class K, class V> class CKeyPairCompareLess { };
template <class K, class V, class C = CKeyPairCompareLess<K, V> >
class CVectorMap : public std::vector<std::pair<K, V> > {
public:
    typedef std::pair<K, V> value_type;
    typedef value_type* iterator;
    iterator LowerBound(const K& key);                       // 0x0040F020
    iterator end() { return this->_M_finish; }
};
class CCreatureActionBase;

class CThing {
public:
    virtual ~CThing();
    unsigned char _pad_0x04[0x20 - 0x04];
    unsigned long UsingComponents[9];                        // +0x20 bitset<274>
    CVectorMap<ETCInterfaceType, CTCBase*> Components;       // +0x44
    unsigned char _pad_0x50[0x91 - 0x50];
    bool Dead : 1;                                           // +0x91 bit 0
    bool IsDead() const { return Dead != 0; }
    template <class T> __forceinline bool GetTC(T** out)
    {
        if (!(UsingComponents[T::InterfaceType / 32] & (1UL << (T::InterfaceType % 32))))
            return false;
        CVectorMap<ETCInterfaceType, CTCBase*>& components = Components;
        CVectorMap<ETCInterfaceType, CTCBase*>::iterator p = components.LowerBound((ETCInterfaceType)T::InterfaceType);
        if (p == components.end() || (ETCInterfaceType)T::InterfaceType < p->first)
            p = components.end();
        *out = (T*)p->second;
        return true;
    }
};
class CThingCreatureBase : public CThing {
public:
    bool SetCurrentAction(const CCreatureActionBase& action);   // 0x006644F0
};

class CBaseObject;
class CBaseIntelligentPointer {
public:
    virtual ~CBaseIntelligentPointer();
    CBaseObject* GetPItem() const;                           // 0x00A01B50
    void* PData;
};

template <class T>
class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    virtual ~CIntelligentPointer();
    T* operator->() const { return (T*)GetPItem(); }
    T& operator*() const { return *(T*)GetPItem(); }
};

template <class T>
inline bool IS_THING_ALIVE(const T& thing)
{
    if (thing.GetPItem() != 0) {
        if (!thing->IsDead())
            return true;
    }
    return false;
}

class CCreatureActionBase {
public:
    virtual ~CCreatureActionBase();
    virtual void v01();
    virtual void v02();
    virtual void PerformAction();
    virtual void FinishAction();                             // 0x00694430
    unsigned char _pad_0x04[4];
    CIntelligentPointer<CThingCreatureBase> PCreature;       // +0x08
    long TotalTime;                                          // +0x10
    long ActionTime;                                         // +0x14
    long AnimDelayTime;                                      // +0x18
    long FramesRunning;                                      // +0x1C
    unsigned char _pad_0x20[0xa8 - 0x20];
};

class CCombatActionBase : public CCreatureActionBase {
public:
    virtual ~CCombatActionBase();                            // 0x0062E900
    CIntelligentPointer<CThing> PTarget;                     // +0xA8
    unsigned char _pad_0xb0[0x114 - 0xb0];
};

namespace {
class CCreatureAction_KrakenLowAttackLoop : public CCombatActionBase {
public:
    bool Interrupt;                                          // +0x114
    long LoopNum;                                            // +0x118
    CCreatureAction_KrakenLowAttackLoop(CThingCreatureBase& creature, CThing& target, long loop_num, bool interrupt);   // 0x009255E0
    virtual void FinishAction();
};

class CCreatureAction_KrakenLowAttackOutOf : public CCombatActionBase {
public:
    CCreatureAction_KrakenLowAttackOutOf(CThingCreatureBase& creature);   // 0x00925680
};

void CCreatureAction_KrakenLowAttackLoop::FinishAction()
{
    CCreatureActionBase::FinishAction();
    CTCKraken* kraken;
    PCreature->GetTC(&kraken);
    long attack_loops = kraken->PKrakenDef->LowStrikeAttackLoops;
    if (IS_THING_ALIVE(PTarget) && ++LoopNum < attack_loops) {
        if (FramesRunning < TotalTime) {
            LoopNum = attack_loops - 2;
            Interrupt = true;
        }
        if (LoopNum == attack_loops - 2)
            kraken->EndAttackBeam();
        PCreature->SetCurrentAction(CCreatureAction_KrakenLowAttackLoop(*PCreature, *PTarget, LoopNum, Interrupt));
    } else {
        PCreature->SetCurrentAction(CCreatureAction_KrakenLowAttackOutOf(*PCreature));
    }
}
}


void KrakenLowAttackLoop_FinishAction_Use(void* action)
{
    ((CCreatureAction_KrakenLowAttackLoop*)action)->CCreatureAction_KrakenLowAttackLoop::FinishAction();
}