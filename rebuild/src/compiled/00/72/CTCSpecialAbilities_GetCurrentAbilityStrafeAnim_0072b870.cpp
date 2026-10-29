// CTCSpecialAbilities::GetCurrentAbilityStrafeAnim() const -- the strafe animation name of the spell component behind the current ability (fireball 0x10, will lightning 0xb), else ""
// Leaf: GetCurrentAbilityStrafeAnim
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0072B870, 194 bytes (`ret 4`: the CCharString result's hidden pointer).
// Callees: the const interface LowerBound @ 0x004365B0 (inlined lookups of interfaces 0x9d and 0x94), the fireball spell's
// strafe-animation getter @ 0x007C85F0 (CTCFireballSpell area) and the will-lightning one @ 0x0079F610 (CTCWillLightning area;
// both catalogued under the propagated label CTCFireballSpell::GetReleaseAnimName -- named descriptively), CCharString(const
// char*, long) @ 0x0099EBF0 ("" @ 0x0122D70E).
// Layout: CTCSpecialAbilities CurrentAbility +0x10; CThing interface bitset +0x20 (0x9d = bit 29 and 0x94 = bit 20 of the fifth
// word), map +0x44.
// Shape: a two-case switch (tests in value order, bodies in source order: 0x10 first); each case is the out-parameter
// GetInterface (component in eax, then `mov ecx,eax`) and returns the getter's CCharString straight through the hidden
// pointer; the default builds the empty string there.
class CTCBase;
class CCharString { public:
    void* m_pData;
    CCharString(const char* text, long length);        // 0x0099EBF0
    CCharString(const CCharString& other);
    ~CCharString();
};
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    const SEntry* LowerBound(const ETCInterfaceType& type) const;  // 0x004365B0
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                    // +0x20
    CInterfaceMap m_interfaces;                        // +0x44
    template <class T> __forceinline bool GetInterface(long type, T** out) const
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31))) {
            ETCInterfaceType key = (ETCInterfaceType)(type);
            const CInterfaceMap* map = &m_interfaces;
            const CInterfaceMap::SEntry* it = map->LowerBound(key);
            if (it == map->last || type < it->first)
                it = map->last;
            *out = (T*)it->second;
            return true;
        }
        return false;
    }
};
class CTCFireballSpell { public:
    CCharString GetStrafeAnimName() const;             // 0x007C85F0
};
class CTCWillLightning { public:
    CCharString GetStrafeAnimName() const;             // 0x0079F610
};
class CTCSpecialAbilities { public:
    virtual void v00();
    CThing* ParentThing;                        // +0x04 (CTCBase)
    unsigned char _base08[4];
    bool AbilityMutexLock;                      // +0x0c
    bool UsingBed;                              // +0x0d
    long CurrentAbility;                        // +0x10
    CCharString GetCurrentAbilityStrafeAnim() const;
};

CCharString CTCSpecialAbilities::GetCurrentAbilityStrafeAnim() const
{
    switch (CurrentAbility) {
    case 0x10: {
        CTCFireballSpell* fireball;
        if (ParentThing->GetInterface(0x9d, &fireball))
            return fireball->GetStrafeAnimName();
        break;
    }
    case 0xb: {
        CTCWillLightning* lightning;
        if (ParentThing->GetInterface(0x94, &lightning))
            return lightning->GetStrafeAnimName();
        break;
    }
    }
    return CCharString("", -1);
}