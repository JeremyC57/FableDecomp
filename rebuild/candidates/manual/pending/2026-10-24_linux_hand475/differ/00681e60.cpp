// CTCCreatureOpinionOfHero::SetPersonalityOverride(long defIndex, bool force) -- switch the creature to an OPINION_PERSONALITY def (or back to all-enabled attitudes for 0): refill the attitude mask from the def, apply its per-attitude overrides, and rescale each transient opinion offset's run-out by the new/old TimeScalingRunOut ratio
// Grade: DIFFER(626v626) -- one SIB byte: the old-traits address is `lea ecx,[eax+ecx+10h]` in retail (scaled index as base) and `lea ecx,[ecx+eax+10h]` here; the forms that flip it (old reference declared first) also hoist the oldDef load above the new-def loads
// Leaf: ?SetPersonalityOverride@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00681E60, 626 bytes (+ padding to 640), thiscall `ret 8`. Ego_r.pdb name set:
//   ?SetPersonalityOverride@CTCCreatureOpinionOfHero@@QAEXJ_N@Z (recursive call `SetPersonalityOverride(0, true)` on a bad
//   def). Catalog label "SetPersonalityOverride_BuildOpinionTable" is descriptive.
// Callees: CTCBase::GetWorld 0x00686A70, CDefinitionManager::GetDefNameFromGlobalIndex 0x009ACCC0 and
//   GetDefClassTypeNameFromGlobalIndex 0x009ACCE0 (CDefString by hidden pointer), the personality def's slot 10 (+0x28,
//   a CDefString by hidden pointer), CDefString::operator==(const char*) 0x005B3440 ("OPINION_PERSONALITY" 0x01235D68),
//   GetDef<COpinionPersonalityDef> 0x00685E70, std::fill<bool*, bool> 0x00685430 (__fastcall), malloc 0x00BFEA0E /
//   free 0x00BFEA14, _ftol 0x00BFEA70. Constant 0.0001f 0x0129BA3C.
// Layout: retail CTCCreatureOpinionOfHero PDef +0x1C, PPersonalityDef +0x20, AttitudeMask +0x24 (12-byte vector),
//   TransientOffsets +0x338 (4-byte STLport list); COpinionOfHeroDef Personality +0x30; COpinionPersonalityDef
//   PersonalityTraits +0x3C, AttitudeEnabledDefaultForNotNone +0x40, AttitudeEnabled map +0x44 (PDB +0x40/+0x44/+0x48).
// Shape: matched everywhere else: the map iterator's operator++ must be __forceinline (the frame then keeps the scratch
//   vector at [esp+14]); the bad-def path is an early `return` (its own oldDef release + epilogue); the transient list
//   end is cached; the all-true reset is a plain fill loop over two pointer locals (VC7.1 turns it into rep stosd/stosb).
extern "C" void* __cdecl malloc(unsigned int size);  // 0x00BFEA0E
extern "C" void __cdecl free(void* p);               // 0x00BFEA14
extern "C" void* __cdecl memset(void* d, int c, unsigned int n);
#pragma intrinsic(memset)

class CDefString {
public:
    long Index;
    CDefString(long index = -1) : Index(index) { }
    bool operator==(const char* name) const;           // 0x005B3440
    bool operator!=(const CDefString& other) const { return Index != other.Index; }
};
class CDefVirtual {
public:
    virtual void v00();
    virtual void Destroy() const;                      // slot 1
};
class CDefPointeeBase : public CDefVirtual {
public:
    mutable long RefCount;                             // +0x04
};
template <class T> class CDefPointer {
public:
    T* Object;
    CDefPointer() : Object(0) { }
    CDefPointer(const CDefPointer& other) : Object(other.Object)
    {
        if (Object)
            ++Object->RefCount;
    }
    ~CDefPointer()
    {
        T* object = Object;
        if (object && --object->RefCount == 0)
            object->Destroy();
    }
    T* operator->() const { return Object; }
};

namespace std {
struct __false_type { __false_type() { } };
template <class _Tp> struct less { };
template <class _T1, class _T2> struct pair { _T1 first; _T2 second; };
template <class _Tp> class allocator { };
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};

template <class _ForwardIter, class _Tp>
void __fastcall fill(_ForwardIter __first, _ForwardIter __last, const _Tp& __val)
{
    for (; __first != __last; ++__first)
        *__first = __val;
}
template <>
void __fastcall fill<bool*, bool>(bool* __first, bool* __last, const bool& __val);   // 0x00685430

template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp* iterator;
    typedef unsigned int size_type;
    iterator begin() { return this->_M_start; }
    iterator end() { return this->_M_finish; }
    _Tp& operator[](size_type __n) { return *(this->_M_start + __n); }
    __forceinline vector(size_type __n, const _Tp& __val)
    {
        this->_M_start = (_Tp*)malloc(__n * sizeof(_Tp));
        this->_M_finish = this->_M_start;
        this->_M_end_of_storage._M_data = this->_M_start + __n;
        memset(this->_M_start, __val, __n);
        this->_M_finish = this->_M_start + __n;
    }
    __forceinline ~vector()
    {
        if (this->_M_start != 0)
            free(this->_M_start);
    }
};

struct _Rb_tree_node_base {
    char                _M_color;
    _Rb_tree_node_base* _M_parent;
    _Rb_tree_node_base* _M_left;
    _Rb_tree_node_base* _M_right;
};
template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base {
    _Value _M_value_field;
};
struct _Rb_tree_base_iterator {
    _Rb_tree_node_base* _M_node;
};
template <class _Value> struct _Rb_tree_iterator : public _Rb_tree_base_iterator {
    _Rb_tree_iterator() { }
    _Rb_tree_iterator(_Rb_tree_node_base* __x) { _M_node = __x; }
    const _Value& operator*() const { return ((_Rb_tree_node<_Value>*)_M_node)->_M_value_field; }
    const _Value* operator->() const { return &((_Rb_tree_node<_Value>*)_M_node)->_M_value_field; }
    __forceinline _Rb_tree_iterator& operator++()
    {
        _Rb_tree_node_base* __x = _M_node;
        if (__x->_M_right != 0) {
            __x = __x->_M_right;
            while (__x->_M_left != 0)
                __x = __x->_M_left;
        } else {
            _Rb_tree_node_base* __y = __x->_M_parent;
            while (__x == __y->_M_right) {
                __x = __y;
                __y = __y->_M_parent;
            }
            if (__x->_M_right != __y)
                __x = __y;
        }
        _M_node = __x;
        return *this;
    }
    bool operator!=(const _Rb_tree_iterator& __y) const { return _M_node != __y._M_node; }
};
template <class _Key, class _Tp, class _Compare = less<_Key>, class _Alloc = allocator<pair<const _Key, _Tp> > >
class map {
public:
    typedef _Rb_tree_iterator<pair<const _Key, _Tp> > const_iterator;
    _STLP_alloc_proxy<_Rb_tree_node_base*, _Rb_tree_node<pair<const _Key, _Tp> >, _Alloc> _M_header;   // +0x00
    unsigned int _M_node_count;
    _Compare _M_key_compare;
    const_iterator begin() const { return const_iterator(_M_header._M_data->_M_left); }
    const_iterator end() const { return const_iterator(_M_header._M_data); }
};

struct _List_node_base {
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};
template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;
};
template <class _Tp> struct _List_iterator {
    _List_node_base* _M_node;
    _List_iterator(_List_node_base* __x) : _M_node(__x) { }
    _Tp& operator*() const { return ((_List_node<_Tp>*)_M_node)->_M_data; }
    _List_iterator& operator++() { _M_node = _M_node->_M_next; return *this; }
    bool operator!=(const _List_iterator& __y) const { return _M_node != __y._M_node; }
};
template <class _Tp, class _Alloc = allocator<_Tp> > class list {
public:
    typedef _List_iterator<_Tp> iterator;
    _STLP_alloc_proxy<_List_node<_Tp>*, _List_node<_Tp>, _Alloc> _M_node;
    iterator begin() { return iterator(_M_node._M_data->_M_next); }
    iterator end() { return iterator(_M_node._M_data); }
};
}

static inline void FillTrue(bool* first, bool* last)
{
    for (; first != last; ++first)
        *first = true;
}

enum EOpinion { OPINION_COUNT = 4 };
enum EOpinionAttitudeType { OPINION_ATTITUDE_TYPE_COUNT = 0x12 };

class COpinionPersonalityTraits {                      // 0x24 bytes per EOpinion
public:
    float BaseOffset;                                  // +0x00
    float HeroStatScaling;                             // +0x04
    float VillageOpinionScaling;                       // +0x08
    float TimeScalingRunIn;                            // +0x0C
    float TimeScalingRunOut;                           // +0x10
    float PeakScalingIfPos;                            // +0x14
    float PeakScalingIfNeg;                            // +0x18
    float PersistScalingIfPos;                         // +0x1C
    float PersistScalingIfNeg;                         // +0x20
};
class COpinionPersonalityDef : public CDefPointeeBase {
public:
    virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06();
    virtual void v07(); virtual void v08(); virtual void v09();
    virtual CDefString GetName() const;                // slot 10 (+0x28)
    unsigned char _pad08[0x3c - 0x08];
    COpinionPersonalityTraits* PersonalityTraits;      // +0x3C (PDB +0x40)
    bool AttitudeEnabledDefaultForNotNone;             // +0x40
    unsigned char _pad41[3];
    std::map<EOpinionAttitudeType, bool> AttitudeEnabled;   // +0x44
};
class COpinionOfHeroDef : public CDefPointeeBase {
public:
    unsigned char _pad08[0x30 - 0x08];
    long Personality;                                  // +0x30
};
class COpinionTransientOffset {
public:
    EOpinion Opinion;                                  // +0x00
    float    OffsetPerFrameRunIn;                      // +0x04
    long     FramesToRunIn;                            // +0x08
    long     FramesOfCappedPeak;                       // +0x0C
    float    OffsetPerFrameRunOut;                     // +0x10
    long     FramesToRunOut;                           // +0x14
};

class CDefinitionManager {
public:
    CDefString GetDefNameFromGlobalIndex(long index) const;            // 0x009ACCC0
    CDefString GetDefClassTypeNameFromGlobalIndex(long index) const;   // 0x009ACCE0
    template <class T> bool GetDef(long index, CDefPointer<T>* out) const;   // 0x00685E70 (COMDAT-folded)
};
class CGameDefinitionManager : public CDefinitionManager { };
class CWorld {
public:
    unsigned char _pad00[0x10];
    CGameDefinitionManager* DefinitionManager;         // +0x10
};
class CThing;

class CTCCreatureOpinionOfHero {
public:
    void* __vftable;                                   // +0x00
    CThing* ParentThing;                               // +0x04
    unsigned char _pad08[0x1c - 0x08];
    CDefPointer<const COpinionOfHeroDef> PDef;         // +0x1C
    CDefPointer<const COpinionPersonalityDef> PPersonalityDef;   // +0x20
    std::vector<bool> AttitudeMask;                    // +0x24
    unsigned char _pad30[0x338 - 0x30];
    std::list<COpinionTransientOffset> TransientOffsets;   // +0x338
    CWorld* GetWorld() const;                          // 0x00686A70 (CTCBase)
    void DisableUnconfigurableAttitudes()
    {
        AttitudeMask[13] = false;
        AttitudeMask[14] = false;
        AttitudeMask[15] = false;
        AttitudeMask[16] = false;
        AttitudeMask[17] = false;
    }
    void SetPersonalityOverride(long defIndex, bool force);
};

void CTCCreatureOpinionOfHero::SetPersonalityOverride(long defIndex, bool force)
{
    CGameDefinitionManager* defManager = GetWorld()->DefinitionManager;
    if (!force && PPersonalityDef->GetName() != defManager->GetDefNameFromGlobalIndex(PDef->Personality))
        return;
    if (defIndex) {
        CDefPointer<const COpinionPersonalityDef> oldDef = PPersonalityDef;
        if (!(defManager->GetDefClassTypeNameFromGlobalIndex(defIndex) == "OPINION_PERSONALITY") || !defManager->GetDef(defIndex, &PPersonalityDef)) {
            SetPersonalityOverride(0, true);
            return;
        }
        std::fill(AttitudeMask.begin(), AttitudeMask.end(), PPersonalityDef->AttitudeEnabledDefaultForNotNone);
        DisableUnconfigurableAttitudes();
        std::vector<bool> applied(OPINION_ATTITUDE_TYPE_COUNT, false);
        std::map<EOpinionAttitudeType, bool>::const_iterator end = PPersonalityDef->AttitudeEnabled.end();
        for (std::map<EOpinionAttitudeType, bool>::const_iterator it = PPersonalityDef->AttitudeEnabled.begin(); it != end; ++it) {
            EOpinionAttitudeType type = it->first;
            if (type < OPINION_ATTITUDE_TYPE_COUNT) {
                applied[type] = true;
                AttitudeMask[type] = it->second;
            }
        }
        std::list<COpinionTransientOffset>::iterator offsetsEnd = TransientOffsets.end();
        for (std::list<COpinionTransientOffset>::iterator it = TransientOffsets.begin(); it != offsetsEnd; ++it) {
            COpinionTransientOffset& offset = *it;
            float scale = PPersonalityDef->PersonalityTraits[offset.Opinion].TimeScalingRunOut;
            const float& oldScale = oldDef->PersonalityTraits[offset.Opinion].TimeScalingRunOut;
            if (oldScale > 0.0001f)
                scale /= oldScale;
            float total = offset.FramesToRunOut * offset.OffsetPerFrameRunOut;
            long frames = (long)(offset.FramesToRunOut * scale);
            if (frames < 1)
                frames = 1;
            offset.FramesToRunOut = frames;
            offset.OffsetPerFrameRunOut = total / frames;
        }
    } else {
        FillTrue(AttitudeMask.begin(), AttitudeMask.end());
        DisableUnconfigurableAttitudes();
    }
}
