// NUISystem::CSwappingStateComponent::Update(float deltaTime) -- the changing-state update, then cycle the swap states: after a state change remember when a swap state was entered; when settled in a swap state whose dwell time has run out, change to the next swap state (or a random different one) and restart the dwell clock
// Leaf: ?Update@CSwappingStateComponent@NUISystem@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00547380, 311 bytes (+ int3 padding), `ret 4`. Slot 1 of the primary vtable
//   0x012485AC (rtti-vtables.tsv); Ego_r.pdb ?Update@CSwappingStateComponent@NUISystem@@UAEXM@Z.
// Callees: NUISystem::CChangingStateComponent::Update(float) @ 0x0052C7E0 (qualified), the virtual slots 137 (+0x224)
//   ChangedStateLastUpdate @ 0x0041C5E0, 142 (+0x238) IsRandomSwap @ 0x00547970 (FlagsSetTwo bit 3) and 48 (+0xC0)
//   ChangeState(unsigned long) @ 0x0052CF40, the STLport `__find_if(first, last, pred, const random_access_iterator_tag&)`
//   instance @ 0x00547790 (__fastcall; manifest "std::find_int_array"; the tag temporary homed in the dead deltaTime
//   slot) and GFRandomNoRepeat @ 0x005472A0 (landed, __fastcall).
// Layout: CChangingStateComponent as the landed InternalChanged 0x0052C780 (CurrentState +0x144, TargetState +0x148)
//   and the component clock at +0x30; CSwappingStateComponent (retail) SwappingStates +0x15C (the STLport vector of
//   (state, dwell time) pairs, as the landed GetDataFromDef 0x00547500), then the time of the last swap +0x168 and the
//   random seed +0x16C (descriptive names).
// Shape: the pair predicate is a 4-byte functor holding the state (pushed by value; the out-of-line __find_if
//   compares `[eax]` with it); the dwell test is `Time - LastSwapTime >= dwell` (`test ah,1 ; jne` to the exit);
//   the next index is `index < size() - 1 ? index + 1 : 0` (unsigned, `jae`); the empty iterator tag needs a
//   user default constructor (else its temporary is zero-filled, `mov byte ptr [esp+14h],0`).
struct random_access_iterator_tag { random_access_iterator_tag() {} };

template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
};

template <class _Tp> struct vector {                   // STLport vector, 12 bytes
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;
    _Tp* begin() { return _M_start; }
    _Tp* end() { return _M_finish; }
    unsigned long size() const { return (unsigned long)(_M_finish - _M_start); }
    bool empty() const { return _M_start == _M_finish; }
    _Tp& operator[](unsigned long n) { return *(_M_start + n); }
};

template <class _RandomAccessIter, class _Predicate>
_RandomAccessIter __fastcall __find_if(_RandomAccessIter __first, _RandomAccessIter __last, _Predicate __pred,
                                       const random_access_iterator_tag&);     // 0x00547790

template <class _InputIter, class _Predicate>
inline _InputIter find_if(_InputIter __first, _InputIter __last, _Predicate __pred)
{
    return __find_if(__first, __last, __pred, random_access_iterator_tag());
}

unsigned long __fastcall GFRandomNoRepeat(unsigned long range, unsigned long previous, unsigned long* seed);   // 0x005472A0

namespace NUISystem {

typedef pair<unsigned long, float> CSwappingState;     // (state, dwell time)

class CSwappingStateIs {                               // 4 bytes, by value
public:
    unsigned long State;
    explicit CSwappingStateIs(unsigned long state) : State(state) {}
    bool operator()(const CSwappingState& entry) const { return entry.first == State; }
};

class CChangingStateComponent {
public:
    virtual ~CChangingStateComponent();
    virtual void Update(float deltaTime);                      // slot 1; 0x0052C7E0
    virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007();
    virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013();
    virtual void v014(); virtual void v015(); virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019();
    virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023(); virtual void v024(); virtual void v025();
    virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029(); virtual void v030(); virtual void v031();
    virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037();
    virtual void v038(); virtual void v039(); virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043();
    virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047();
    virtual void ChangeState(unsigned long state);              // slot 48 (+0xC0); 0x0052CF40
    virtual void v049(); virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053(); virtual void v054();
    virtual void v055(); virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059(); virtual void v060();
    virtual void v061(); virtual void v062(); virtual void v063(); virtual void v064(); virtual void v065(); virtual void v066();
    virtual void v067(); virtual void v068(); virtual void v069(); virtual void v070(); virtual void v071(); virtual void v072();
    virtual void v073(); virtual void v074(); virtual void v075(); virtual void v076(); virtual void v077(); virtual void v078();
    virtual void v079(); virtual void v080(); virtual void v081(); virtual void v082(); virtual void v083(); virtual void v084();
    virtual void v085(); virtual void v086(); virtual void v087(); virtual void v088(); virtual void v089(); virtual void v090();
    virtual void v091(); virtual void v092(); virtual void v093(); virtual void v094(); virtual void v095(); virtual void v096();
    virtual void v097(); virtual void v098(); virtual void v099(); virtual void v100(); virtual void v101(); virtual void v102();
    virtual void v103(); virtual void v104(); virtual void v105(); virtual void v106(); virtual void v107(); virtual void v108();
    virtual void v109(); virtual void v110(); virtual void v111(); virtual void v112(); virtual void v113(); virtual void v114();
    virtual void v115(); virtual void v116(); virtual void v117(); virtual void v118(); virtual void v119(); virtual void v120();
    virtual void v121(); virtual void v122(); virtual void v123(); virtual void v124(); virtual void v125(); virtual void v126();
    virtual void v127(); virtual void v128(); virtual void v129(); virtual void v130(); virtual void v131(); virtual void v132();
    virtual void v133(); virtual void v134(); virtual void v135(); virtual void v136();
    virtual bool ChangedStateLastUpdate();                      // slot 137 (+0x224); 0x0041C5E0
    virtual void v138(); virtual void v139(); virtual void v140(); virtual void v141();
    virtual bool IsRandomSwap() const;                          // slot 142 (+0x238); 0x00547970
    unsigned char _pad004[0x30 - 4];
    float         Time;                                 // +0x30 (the component clock)
    unsigned char _pad034[0x144 - 0x34];
    unsigned long CurrentState;                         // +0x144
    unsigned long TargetState;                          // +0x148
    unsigned char _pad14c[0x15c - 0x14c];
};

class CSwappingStateComponent : public CChangingStateComponent {
public:
    vector<CSwappingState> SwappingStates;              // +0x15C
    float                  LastSwapTime;                // +0x168
    unsigned long          RandomSeed;                  // +0x16C
    virtual void Update(float deltaTime);
};

void CSwappingStateComponent::Update(float deltaTime)
{
    CChangingStateComponent::Update(deltaTime);
    if (ChangedStateLastUpdate()) {
        for (unsigned long i = 0; i < SwappingStates.size(); ++i) {
            if (SwappingStates[i].first == CurrentState) {
                LastSwapTime = Time;
                return;
            }
        }
    } else if (TargetState == CurrentState) {
        CSwappingState* it = find_if(SwappingStates.begin(), SwappingStates.end(), CSwappingStateIs(TargetState));
        if (it != SwappingStates.end() && Time - LastSwapTime >= it->second) {
            unsigned long next;
            if (IsRandomSwap() && !SwappingStates.empty())
                next = GFRandomNoRepeat(SwappingStates.size(), TargetState, &RandomSeed);
            else {
                unsigned long index = it - SwappingStates.begin();
                next = index < SwappingStates.size() - 1 ? index + 1 : 0;
            }
            ChangeState(SwappingStates[next].first);
            LastSwapTime = Time;
        }
    }
}
}