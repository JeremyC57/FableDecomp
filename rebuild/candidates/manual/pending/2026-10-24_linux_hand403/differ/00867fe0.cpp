// CAnimationEntry::GetComponent(const CAnimComponentFollowingOnAnim**) const -- the entry's own following-on-anim component (EAnimComponent 8) or, when the entry has none, the default one
// Leaf: ??$GetComponent@VCAnimComponentFollowingOnAnim@@
// Grade: DIFFER(120v125) -- the key is re-read from its slot for the `key < it->first` compares (retail: immediate 8, `cmp [eax],8 ; jle/jg`) and its store is not hoisted above the null test (retail: one `mov [esp+4],8` before `je`, reused by the default-map lookup); tried named/const local, by-value find with an inner key copy, static-const and rvalue-function keys. Next: a same-TU LowerBound stand-in body (auto_inline(off) + noinline) so VC sees the key unchanged.
// Retail 0x00867FE0, 0x7D bytes, thiscall `ret 4`. Manifest label "CTCBase_GetInterfaceAbility" is a descriptive auto label;
// Ego_r names this instantiation ??$GetComponent@VCAnimComponentFollowingOnAnim@@@CAnimationEntry@@QBEXPAPBVCAnimComponentFollowingOnAnim@@@Z
// and CAnimComponentFollowingOnAnim::GetType (0x005DF7D0) returns 8. The byte-identical twin at 0x00867F60 is the
// CAnimComponentFollowingOnAnimFilter (type 0x0B) instantiation. Caller: CCombatAction_GenericStrikeResponseKnockdownRecoverBase::
// Initialise 0x00865EB0.
// Callee: CVectorMap<EAnimComponent, CIVCountedPointer<CAnimComponentBase> >::LowerBound @ 0x005DFA50 (8-byte pairs), on the
// entry's component map and on the default component map @ 0x013B8C38 (first) / 0x013B8C3C (last), which the anim-component
// registration code at 0x005DD7xx fills.
// Layout (Ego_r): CAnimationEntry {BankIndex +0, AnimName +4, GroupName +8, Components +0x0C (CIVCountedPointer<CAnimComponentMap>)};
// CAnimComponentMap {vptr +0, IVRefCount +4, Map +8 (STLport vector: first +8, last +0x0C)}.
// Shape: the key is one function-scope local stored before the null test (`mov [esp+4],8` ahead of `je`), shared by both
// finds. The inline find is `it == last || key < it->first ? last : it`; the entry's map result is compared with a re-read
// end() (`cmp eax,[edi+0Ch]`); the default map's find is used unchecked (`->second` of last when absent), its two select
// arms each get their own store tail.
class CAnimComponentBase;
class CIVCountedPointeeBase;

enum EAnimComponent { ANIM_COMPONENT_FOLLOWING_ON_ANIM = 8 };

template <class T> class CIVCountedPointer
{
public:
    T* Object;                                       // +0x00
};

struct SAnimComponentPair                            // pair<EAnimComponent, CIVCountedPointer<CAnimComponentBase> >
{
    EAnimComponent                        first;     // +0x00
    CIVCountedPointer<CAnimComponentBase> second;    // +0x04
};

class CAnimComponentVectorMap                        // CVectorMap<EAnimComponent, CIVCountedPointer<CAnimComponentBase> >
{
public:
    SAnimComponentPair* _M_start;                    // +0x00
    SAnimComponentPair* _M_finish;                   // +0x04
    SAnimComponentPair* _M_end_of_storage;           // +0x08

    SAnimComponentPair* LowerBound(const EAnimComponent& key) const;   // 0x005DFA50
    SAnimComponentPair* end() const { return _M_finish; }
    __forceinline SAnimComponentPair* find(const EAnimComponent& key) const
    {
        SAnimComponentPair* it = LowerBound(key);
        SAnimComponentPair* last = _M_finish;
        if (it == last || key < it->first)
            return last;
        return it;
    }
};

extern CAnimComponentVectorMap g_DefaultAnimComponents;   // 0x013B8C38

class CAnimComponentMap
{
public:
    virtual void v00();
    unsigned long           IVRefCount;              // +0x04
    CAnimComponentVectorMap Map;                     // +0x08
};

class CAnimComponentFollowingOnAnim
{
public:
    static const EAnimComponent StaticType = ANIM_COMPONENT_FOLLOWING_ON_ANIM;
};

class CDefString { public: void* PData; };

class CAnimationEntry
{
public:
    long                                 BankIndex;  // +0x00
    CDefString                           AnimName;   // +0x04
    CDefString                           GroupName;  // +0x08
    CIVCountedPointer<CAnimComponentMap> Components; // +0x0C

    template <class T> void GetComponent(const T** out) const;
};

template <class T> void CAnimationEntry::GetComponent(const T** out) const
{
    EAnimComponent type = T::StaticType;
    const CAnimComponentMap* components = Components.Object;
    if (components)
    {
        SAnimComponentPair* it = components->Map.find(type);
        if (it != components->Map.end())
        {
            *out = (const T*)it->second.Object;
            return;
        }
    }
    *out = (const T*)g_DefaultAnimComponents.find(type)->second.Object;
}

template void CAnimationEntry::GetComponent<CAnimComponentFollowingOnAnim>(const CAnimComponentFollowingOnAnim** out) const;
