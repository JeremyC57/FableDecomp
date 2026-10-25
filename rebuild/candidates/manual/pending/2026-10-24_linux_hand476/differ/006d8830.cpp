// CTCHeroExperience::AddSuccessfulMultistrikeHit(const CHitParameters& hit) -- for every (damage attribute, amount) of the hit's damage map: roll the attack history (drop the oldest, append this attribute at the current frame), restart the experience attenuation timer and, when the active combo data has a multiplier for the attribute, raise the experience multiplier param by variation * |amount| * multiplier and update the combo multiplier
// Grade: DIFFER(321v321) -- scheduling only: retail stores |amount| (`fst [esp+10h]`) before UpdateComboMultiplier's `push 1`, ours pushes first. 4 grades: plain find, __forceinline find, STLport _M_find, STLport list/map iterators with user copy constructors (fixed the shrink-wrapped ebp/edi pushes)
// Leaf: ?AddSuccessfulMultistrikeHit@CTCHeroExperience@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006D8830, 321 bytes (+ int3 padding to 336), thiscall, `ret 4`.
// Identity: the catalog's "Experience_ProcessMultiHitCombo" is a descriptive auto label. FableWin.pdb lists
//   ?AddSuccessfulMultistrikeHit@CTCHeroExperience@@QAEXABVCHitParameters@@@Z; the body walks CHitParameters::DamageMap
//   (+0x30, Ego_r.pdb) and inlines the private AddAttackOfTypeToAttackHistory. Single caller 0x007CCD4C.
// Callees: free 0x00BFEA14 (pop_front), CTCBase::GetWorld 0x00686A70 + the static CWorld::GetFrame 0x0049D870, malloc
//   0x00BFEA0E (push_back), GetActiveComboMultiplierData 0x006D72D0, __ftol2 0x00BFEA70, GetAttackVariationMultiplier
//   0x006D7120, UpdateComboMultiplier(bool) 0x006D7F20 (landed). Global: g_framesPerSecond 0x01375550 (`fild`).
// Layout: CTCHeroExperience in its retail layout (12-byte STLport vectors, 4-byte lists; engine/CTCHeroExperience.h has
//   the Ego_r.pdb debug-STL offsets), as in the landed 0x006D7F20: ExperienceMultiplierParam +0x30,
//   ExperienceAttenuationTimer +0x38, AttackHistory list<pair<EDamageAttribute,long> > +0x50; CComboMultiplierData
//   {ComboMultiplier map +0, ExperienceMultiplierNumSecondsForAttenuation +0xC}.
// Shape: the map entry copied to two frame words; STLport list pop_front (erase(begin())) and push_back (insert(end(), x)
//   with the null-tested placement construct); the inline map::find on the combo map; |amount| kept in a named float.
extern "C" void* __cdecl malloc(unsigned int size);         // 0x00BFEA0E
extern "C" void __cdecl free(void* block);                  // 0x00BFEA14
extern "C" double __cdecl fabs(double x);
#pragma intrinsic(fabs)
inline void* operator new(unsigned int, void* p) { return p; }

extern long g_framesPerSecond;                              // 0x01375550

enum EDamageAttribute { EDamageAttribute_Force32 = 0x7fffffff };

namespace std {
template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
    pair(const T1& a, const T2& b) : first(a), second(b) { }
};
template <class T1, class T2> inline pair<T1, T2> make_pair(const T1& a, const T2& b) { return pair<T1, T2>(a, b); }

struct _Rb_tree_node_base {
    unsigned char       _M_color;                           // +0x00
    _Rb_tree_node_base* _M_parent;                          // +0x04
    _Rb_tree_node_base* _M_left;                            // +0x08
    _Rb_tree_node_base* _M_right;                           // +0x0C
};
template <class K, class V> struct _Rb_tree_node : public _Rb_tree_node_base {
    K first;                                                // +0x10
    V second;                                               // +0x14
};
template <class Dummy> struct _Rb_global {
    static _Rb_tree_node_base* _M_increment(_Rb_tree_node_base* x)
    {
        if (x->_M_right != 0) {
            x = x->_M_right;
            while (x->_M_left != 0)
                x = x->_M_left;
        }
        else {
            _Rb_tree_node_base* y = x->_M_parent;
            while (x == y->_M_right) {
                x = y;
                y = y->_M_parent;
            }
            if (x->_M_right != y)
                x = y;
        }
        return x;
    }
};
template <class K, class V> struct _Rb_tree_iterator {
    _Rb_tree_node_base* _M_node;
    _Rb_tree_iterator(_Rb_tree_node_base* x) : _M_node(x) { }
    _Rb_tree_iterator(const _Rb_tree_iterator& x) : _M_node(x._M_node) { }
    _Rb_tree_iterator& operator++() { _M_node = _Rb_global<bool>::_M_increment(_M_node); return *this; }
    bool operator==(const _Rb_tree_iterator& x) const { return _M_node == x._M_node; }
    bool operator!=(const _Rb_tree_iterator& x) const { return _M_node != x._M_node; }
    _Rb_tree_node<K, V>* operator->() const { return (_Rb_tree_node<K, V>*)_M_node; }
};
template <class K, class V> class map {
public:
    typedef _Rb_tree_iterator<K, V> const_iterator;
    _Rb_tree_node_base* _M_header;                          // +0x00
    unsigned long       _M_node_count;                      // +0x04
    unsigned char       _M_key_compare[4];                  // +0x08
    const_iterator begin() const { return const_iterator(_M_header->_M_left); }
    const_iterator end() const { return const_iterator(_M_header); }
    _Rb_tree_node_base* _M_find(const K& k) const
    {
        _Rb_tree_node_base* y = _M_header;                  // last node which is not less than k
        _Rb_tree_node_base* x = _M_header->_M_parent;       // current node
        while (x != 0)
            if (!(((_Rb_tree_node<K, V>*)x)->first < k))
                y = x, x = x->_M_left;
            else
                x = x->_M_right;
        if (y == _M_header || k < ((_Rb_tree_node<K, V>*)y)->first)
            y = _M_header;
        return y;
    }
    const_iterator find(const K& k) const { return const_iterator(_M_find(k)); }
};

struct _List_node_base {
    _List_node_base* _M_next;                               // +0x00
    _List_node_base* _M_prev;                               // +0x04
};
template <class T> struct _List_node : public _List_node_base {
    T _M_data;                                              // +0x08
};
template <class T> struct _List_iterator {
    _List_node<T>* _M_node;
    _List_iterator(_List_node<T>* x) : _M_node(x) { }
    _List_iterator(const _List_iterator& x) : _M_node(x._M_node) { }
};
template <class T> class list {
public:
    typedef _List_iterator<T> iterator;
    _List_node<T>* _M_node;                                 // +0x00 (the header node)
    iterator begin() { return iterator((_List_node<T>*)_M_node->_M_next); }
    iterator end() { return iterator(_M_node); }
    _List_node<T>* _M_create_node(const T& x)
    {
        _List_node<T>* p = (_List_node<T>*)malloc(sizeof(_List_node<T>));
        new (&p->_M_data) T(x);
        return p;
    }
    iterator erase(iterator position)
    {
        _List_node_base* next_node = position._M_node->_M_next;
        _List_node_base* prev_node = position._M_node->_M_prev;
        prev_node->_M_next = next_node;
        next_node->_M_prev = prev_node;
        free(position._M_node);
        return iterator((_List_node<T>*)next_node);
    }
    iterator insert(iterator position, const T& x)
    {
        _List_node<T>* tmp = _M_create_node(x);
        _List_node_base* n = position._M_node;
        _List_node_base* p = n->_M_prev;
        tmp->_M_next = n;
        tmp->_M_prev = p;
        p->_M_next = tmp;
        n->_M_prev = tmp;
        return iterator(tmp);
    }
    void pop_front() { erase(begin()); }
    void push_back(const T& x) { insert(end(), x); }
};
}

struct CComboMultiplierData {
    std::map<EDamageAttribute, float> ComboMultiplier;      // +0x00
    float ExperienceMultiplierNumSecondsForAttenuation;     // +0x0C
    float ExperienceMultiplierParamDecayDeltaPerSecond;     // +0x10
};

class CHitParameters {
public:
    unsigned char _pad00[0x30];
    std::map<EDamageAttribute, float> DamageMap;            // +0x30
};

class CWorld {
public:
    static long __fastcall GetFrame();                      // 0x0049D870
};

class CTCHeroExperience {
public:
    virtual void v00();
    void* ParentThing;                                      // +0x04
    bool ToBeRemoved;                                       // +0x08
    bool IsInGlobalUpdateTCs;                               // +0x09
    long FishingLevel;                                      // +0x0C
    long TotalExperiencePoints;                             // +0x10
    long ExperiencePointsAvailableToSpend;                  // +0x14
    long* StatExperiencePoints[3];                          // +0x18
    long* TrainableStatLevels[3];                           // +0x24
    float ExperienceMultiplierParam;                        // +0x30
    float LastExperienceMultiplierParam;                    // +0x34
    long ExperienceAttenuationTimer;                        // +0x38
    bool AbleToGainExperience;                              // +0x3C
    void* Pickups;                                          // +0x40
    long* ExperienceSpentOnStats[3];                        // +0x44
    std::list<std::pair<EDamageAttribute, long> > AttackHistory;   // +0x50

    CWorld* GetWorld() const;                               // 0x00686A70 (CTCBase)
    const CComboMultiplierData& GetActiveComboMultiplierData() const;   // 0x006D72D0
    float GetAttackVariationMultiplier() const;             // 0x006D7120
    void UpdateComboMultiplier(bool increase);              // 0x006D7F20
    void AddAttackOfTypeToAttackHistory(EDamageAttribute type)
    {
        AttackHistory.pop_front();
        AttackHistory.push_back(std::make_pair(type, GetWorld()->GetFrame()));
    }
    void AddSuccessfulMultistrikeHit(const CHitParameters& hit);
};

void CTCHeroExperience::AddSuccessfulMultistrikeHit(const CHitParameters& hit)
{
    for (std::map<EDamageAttribute, float>::const_iterator it = hit.DamageMap.begin(); it != hit.DamageMap.end(); ++it) {
        EDamageAttribute type = it->first;
        float damage = it->second;
        AddAttackOfTypeToAttackHistory(type);
        const CComboMultiplierData& data = GetActiveComboMultiplierData();
        ExperienceAttenuationTimer = (long)(g_framesPerSecond * data.ExperienceMultiplierNumSecondsForAttenuation);
        std::map<EDamageAttribute, float>::const_iterator found = data.ComboMultiplier.find(type);
        if (found != data.ComboMultiplier.end()) {
            float strength = (float)fabs(damage);
            ExperienceMultiplierParam += GetAttackVariationMultiplier() * strength * found->second;
            UpdateComboMultiplier(true);
        }
    }
}
