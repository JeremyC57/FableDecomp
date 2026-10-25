// CTCGuard::ReportAllPendingCrimes(CTCVillage* village) -- hand every pending crime the guard is holding to its village (and to the village's opinion of the hero, unless it already knows of the deed), note whether any of them is a severity 2 or 3 crime by the hero's deed-log effects and, if so, have the village recalculate its security sweep; then forget the pending crimes
// Leaf: ?ReportAllPendingCrimes@CTCGuard@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007B2E20, length 432 bytes (thiscall, `ret 4`; 0x1AB bytes + int3).
// Identity: Ego_r.pdb ?ReportAllPendingCrimes@CTCGuard@@QAEXPAVCTCVillage@@@Z. The catalog label
//   "CTCVillage_ProcessDeedReports" is descriptive. Same TU as the landed CTCGuard::WitnessedCrime 0x007B2FD0.
// Callees: CWorld::PeekMyPlayer 0x0049D9D0, CPlayer::GetHeroCreature 0x00487DC0, CVectorMap<ETCInterfaceType,CTCBase*>::
//   LowerBound 0x0040F020 (interfaces 0x51 hero deed log, 0x4F creature opinion of hero), CTCVillage::ReportCrime(long)
//   0x0065CB6B, CTCCreatureOpinionOfHero::KnowsOfDeed(long, EOpinionDeedType, bool) const 0x00681AE0,
//   CTCCreatureOpinionOfHero::InformOfDeed(long, bool) 0x00683590, CTCHeroOpinionDeedLog::GetDeedEffectsDef(long)
//   0x006C5630 (CDefPointer by hidden pointer), CTCVillage::RecalculateSecuritySweep(long) 0x00659C32 (landed name),
//   deque<CCrimeWaitingForBribe>::clear() 0x007B4B00.
// Globals: the main game component 0x013B86A0 (+0x24 the world).
// Layout: CTCGuard PendingCrimes +0x14 (STLport deque: start iterator +0x14, finish +0x24; 0x80-byte nodes = 16 crimes);
//   CCrimeWaitingForBribe {DeedHandle +0, Type +4}; COpinionDeedEffectsDef severity +0x70.
// Shape: `if (village) { hero; log = 0; if (hero) GetInterface(&log, 0x51); severe = false; for (it = begin, end; it !=
//   end; ++it) { crime = *it; village->ReportCrime(..); if (village thing has 0x4F) { if (!op->KnowsOfDeed(..))
//   op->InformOfDeed(.., false); } if (log) { effects = log->GetDeedEffectsDef(it->Type); severity 2/3 -> severe; } }
//   if (severe) village->RecalculateSecuritySweep(0); } PendingCrimes.clear();`
struct CTCBase;
struct CInterfaceMap {                                  // CVectorMap<ETCInterfaceType, CTCBase*>
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);               // 0x0040F020
};
struct CThing {
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                     // +0x20 interface-present bitset
    CInterfaceMap m_interfaces;                         // +0x44
    __forceinline unsigned long HasTC(long type) const { return m_presentBits[type >> 5] & (1UL << (type & 31)); }
    template<class T> __forceinline T* GetTC(long wanted)
    {
        long type = wanted;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (T*)((it == map->last || wanted < it->type) ? map->last : it)->component;
    }
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (HasTC(interfaceType)) {
            *ppOut = GetTC<T>(interfaceType);
            return true;
        }
        return false;
    }
};
struct CPlayer {
    CThing* GetHeroCreature();                          // 0x00487DC0
};
struct CWorld {
    CPlayer* PeekMyPlayer() const;                      // 0x0049D9D0
};
struct CMainGameComponent {
    unsigned char _pad00[0x24];
    CWorld* World;                                      // +0x24
};
extern CMainGameComponent* g_pMainGameComponent;        // 0x013B86A0

struct CRefCounted {
    long m_count;
    void DecRef() { --m_count; }
    long GetRefCount() const { return m_count; }
};
struct CDefVirtual {
    virtual void v00();
    virtual void Destroy();                             // slot 1
};
struct COpinionDeedEffectsDef : CDefVirtual, CRefCounted {
    unsigned char _pad08[0x70 - 0x08];
    long Severity;                                      // +0x70 ECrimeSeverity
    void ReleaseTemporary()
    {
        DecRef();
        if (GetRefCount() == 0)
            Destroy();
    }
};
template <class T> struct CDefPointer {
    T* Object;
    ~CDefPointer()
    {
        if (Object) {
            Object->ReleaseTemporary();
            Object = 0;
        }
    }
    T* operator->() const { return Object; }
};

enum EOpinionDeedType { OPINION_DEED_TYPE_NONE };

struct CTCHeroOpinionDeedLog {                          // interface 0x51
    CDefPointer<COpinionDeedEffectsDef> GetDeedEffectsDef(long deedType);   // 0x006C5630
};
struct CTCCreatureOpinionOfHero {                       // interface 0x4F
    bool KnowsOfDeed(long deedHandle, EOpinionDeedType type, bool) const;   // 0x00681AE0
    void InformOfDeed(long deedHandle, bool);                               // 0x00683590
};
struct CTCVillage {
    virtual void v00();
    CThing* PThing;                                     // +0x04 (CTCBase)
    void ReportCrime(long deedHandle);                  // 0x0065CB6B
    void RecalculateSecuritySweep(long flags);          // 0x00659C32
};

class CCrimeWaitingForBribe {                           // 8 bytes
public:
    long DeedHandle;                                    // +0x00
    long Type;                                          // +0x04 EOpinionDeedType
};

namespace std {

template <class _Tp> class allocator {
public:
    allocator() { }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};

template <class _Tp> struct _Nonconst_traits { typedef _Tp value_type; typedef _Tp& reference; typedef _Tp* pointer; };

template <class _Tp> struct _Deque_iterator_base {
    enum _Constants {
        _blocksize = 128,                               // _MAX_BYTES
        __buffer_size = (sizeof(_Tp) < (unsigned int)_blocksize ? ((unsigned int)_blocksize / sizeof(_Tp)) : 1)
    };
    _Tp*  _M_cur;                                       // +0x00
    _Tp*  _M_first;                                     // +0x04
    _Tp*  _M_last;                                      // +0x08
    _Tp** _M_node;                                      // +0x0C
    void _M_set_node(_Tp** __new_node)
    {
        _M_last = (_M_first = *(_M_node = __new_node)) + (int)__buffer_size;
    }
    void _M_increment()
    {
        if (++_M_cur == _M_last) {
            _M_set_node(_M_node + 1);
            _M_cur = _M_first;
        }
    }
    bool _M_equal(const _Deque_iterator_base<_Tp>& __y) const { return _M_cur == __y._M_cur; }
};

template <class _Tp, class _Traits> struct _Deque_iterator : public _Deque_iterator_base<_Tp> {
    typedef _Deque_iterator<_Tp, _Traits> _Self;
    _Deque_iterator(const _Self& __x) : _Deque_iterator_base<_Tp>(__x) { }
    typename _Traits::reference operator*() const { return *this->_M_cur; }
    typename _Traits::pointer operator->() const { return this->_M_cur; }
    _Self& operator++() { this->_M_increment(); return *this; }
    bool operator!=(const _Self& __y) const { return !this->_M_equal(__y); }
};

template <class _Tp, class _Alloc> class _Deque_base {
public:
    typedef _Deque_iterator<_Tp, _Nonconst_traits<_Tp> > iterator;
    iterator _M_start;                                  // +0x00
    iterator _M_finish;                                 // +0x10
    _STLP_alloc_proxy<_Tp**, _Tp*, allocator<_Tp*> > _M_map;                  // +0x20
    _STLP_alloc_proxy<unsigned int, _Tp, allocator<_Tp> > _M_map_size;        // +0x24
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class deque : protected _Deque_base<_Tp, _Alloc> {
public:
    typedef typename _Deque_base<_Tp, _Alloc>::iterator iterator;
    iterator begin() { return this->_M_start; }
    iterator end() { return this->_M_finish; }
    void clear();                                       // 0x007B4B00
};

}

class CTCGuard {
public:
    virtual void v00();
    CThing*                              PThing;                            // +0x04 (CTCBase)
    bool                                 ToBeRemoved;                       // +0x08
    bool                                 IsInGlobalUpdateTCs;               // +0x09
    void*                                PDef;                              // +0x0C CDefPointer<const CGuardDef>
    long                                 BribePool;                         // +0x10
    std::deque<CCrimeWaitingForBribe>    PendingCrimes;                     // +0x14
    void ReportAllPendingCrimes(CTCVillage* village);
};

void CTCGuard::ReportAllPendingCrimes(CTCVillage* village)
{
    if (village) {
        CThing* hero = g_pMainGameComponent->World->PeekMyPlayer()->GetHeroCreature();
        CTCHeroOpinionDeedLog* log = 0;
        if (hero)
            hero->GetInterface(&log, 0x51);
        bool severe = false;
        std::deque<CCrimeWaitingForBribe>::iterator it = PendingCrimes.begin();
        std::deque<CCrimeWaitingForBribe>::iterator end = PendingCrimes.end();
        for (; it != end; ++it) {
            CCrimeWaitingForBribe crime = *it;
            village->ReportCrime(crime.DeedHandle);
            CTCCreatureOpinionOfHero* opinion;
            if (village->PThing->GetInterface(&opinion, 0x4F)) {
                if (!opinion->KnowsOfDeed(crime.DeedHandle, (EOpinionDeedType)crime.Type, true))
                    opinion->InformOfDeed(crime.DeedHandle, false);
            }
            if (log) {
                CDefPointer<COpinionDeedEffectsDef> effects = log->GetDeedEffectsDef(it->Type);
                if (effects->Severity == 2 || effects->Severity == 3)
                    severe = true;
            }
        }
        if (severe)
            village->RecalculateSecuritySweep(0);
    }
    PendingCrimes.clear();
}