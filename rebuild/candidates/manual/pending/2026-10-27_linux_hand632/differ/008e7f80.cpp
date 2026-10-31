// NCreatureMode::CModeBriarRoseConcentrate::GetRandomImposter() const -- count the imposters not yet hit, pick one of them with the world seed and return its thing; null when every imposter has been hit
// Grade: DIFFER(223v223) -- register swap only: the second loop keeps the index in edi and the element address in ebp (retail ebp / edi)
// Leaf: ?GetRandomImposter@CModeBriarRoseConcentrate@NCreatureMode@@QBEPAVCThing@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008E7F80, 223 bytes (`ret`).
// Identity: Ego_r.pdb ?GetRandomImposter@CModeBriarRoseConcentrate@NCreatureMode@@QBEPAVCThing@@XZ (const; donor 0x00830F00
//   between IsAllowedToHitBriarRose and OnDeactivate). FableWin.pdb holds count_if<vector<CIntelligentPointer<CThing> >::
//   const_iterator, unary_negate<CIsImposterHit> > and not1<CIsImposterHit> (CIsImposterHit in the TU's anonymous
//   namespace): the first loop is the inlined STLport count_if over not1(CIsImposterHit()).
// Callees: CIsImposterHit::operator() const 0x008E8A10 (true for a missing/dead imposter or one no longer in creature mode
//   0x3f), CTCCreatureModeManager::CModeBase::GetWorld() const 0x0073A680, CWorld::GetWorldSeed() const 0x0049D810,
//   GFROR13 0x00497890 (__fastcall), CBaseIntelligentPointer::GetPItem() const 0x00A01B50.
// Layout (retail, ctors 0x00741EE0 / 0x008E78C0): Imposters +0x80 (12-byte STLport vector<CIntelligentPointer<CThing> >).
class CBaseObject;
class CThing {
public:
    unsigned char _pad00[0x91];
    unsigned char Dead : 1;                // +0x91 bit 0
};
long __fastcall GFROR13(unsigned long value);   // 0x00497890

class CWorld {
public:
    unsigned long* GetWorldSeed() const;   // 0x0049D810
};

class CBaseIntelligentPointer {            // Ego_r: vptr + PData, 8 bytes
public:
    virtual ~CBaseIntelligentPointer();
    CBaseObject* GetPItem() const;         // 0x00A01B50
private:
    void* PData;
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
};

inline bool IS_THING_ALIVE(const CIntelligentPointer<CThing>& p)
{
    return p.GetPItem() != 0 && !p.GetPItem()->Dead;
}

namespace std {
template <class _Arg, class _Result> struct unary_function {
    typedef _Arg argument_type;
    typedef _Result result_type;
};
template <class _Predicate> class unary_negate : public unary_function<typename _Predicate::argument_type, bool> {
protected:
    _Predicate _M_pred;
public:
    explicit unary_negate(const _Predicate& __x) : _M_pred(__x) { }
    bool operator()(const typename _Predicate::argument_type& __x) const { return !_M_pred(__x); }
};
template <class _Predicate> inline unary_negate<_Predicate> not1(const _Predicate& __pred) { return unary_negate<_Predicate>(__pred); }
template <class _InputIter, class _Predicate>
inline int count_if(_InputIter __first, _InputIter __last, _Predicate __pred)
{
    int __n = 0;
    for ( ; __first != __last; ++__first)
        if (__pred(*__first))
            ++__n;
    return __n;
}
template <class _Tp> class allocator { };
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc { public: _Value _M_data; };
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
};
template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : protected _Vector_base<_Tp, _Alloc> {
public:
    typedef unsigned int size_type;
    typedef const _Tp* const_iterator;
    const_iterator begin() const { return this->_M_start; }
    const_iterator end() const { return this->_M_finish; }
    size_type size() const { return size_type(end() - begin()); }
    const _Tp& operator[](size_type __n) const { return *(begin() + __n); }
};
}

class CIsImposterHit : public std::unary_function<CIntelligentPointer<CThing>, bool> {
public:
    CIsImposterHit() { }
    CIsImposterHit(const CIsImposterHit&) { }
    bool operator()(const CIntelligentPointer<CThing>& imposter) const;   // 0x008E8A10
};

class CTCCreatureModeManager {
public:
    class CModeBase {
    public:
        virtual ~CModeBase();
        unsigned char _body[0x64];
        CWorld& GetWorld() const;          // 0x0073A680
    };
};

inline unsigned long WorldRandomRange(unsigned long& seed, unsigned long range)
{
    seed = seed * 0x24A1 + 0x24DF;
    seed = GFROR13(seed);
    if (range == 0)
        return 0;
    return seed % range;
}

namespace NCreatureMode {
class CModeBriarRoseConcentrate : public CTCCreatureModeManager::CModeBase {
public:
    unsigned char _members[0x18];          // Filter +0x68, Timer +0x74, PTarget +0x78
    std::vector<CIntelligentPointer<CThing> > Imposters;   // +0x80
    CThing* GetRandomImposter() const;
};
}

CThing* NCreatureMode::CModeBriarRoseConcentrate::GetRandomImposter() const
{
    long usable = std::count_if(Imposters.begin(), Imposters.end(), std::not1(CIsImposterHit()));
    if (usable > 0) {
        unsigned long* seed = GetWorld().GetWorldSeed();
        long pick = WorldRandomRange(*seed, usable);
        for (unsigned long i = 0; i != Imposters.size(); ++i) {
            if (IS_THING_ALIVE(Imposters[i])) {
                if (pick-- == 0)
                    return Imposters[i].GetPItem();
            }
        }
    }
    return 0;
}
