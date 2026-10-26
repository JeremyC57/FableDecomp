// CAnimationSet::AnimExists(const CCharString& name, const CAnimFilterBase& filter) -- true when one of the entries filed under the name's CRC passes the animation filter
// Leaf: AnimExists
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005DC2E0, 81 bytes (+ padding to 96), `ret 8` (__fastcall, bool in al).
//   Ego_r.pdb: `?AnimExists@CAnimationSet@@QBE_NABVCCharString@@ABVCAnimFilterBase@@@Z`.
// Callees: 0x005DC020 CAnimationSet::FindFirstAnimWithName (manifest "CVectorMap_LookupOrGetEnd": CRC of the name,
//   LowerBound, end() unless the key matches), CAnimFilterBase virtual slot 0 `operator()(const CAnimationEntry&)`.
// Callers: 33 sites (CThingCreatureBase::HasAnim, the creature AI / mode code; landed 0x00830F20 calls it as a table find).
// Shape: `it = FindFirstAnimWithName(name); for (i = it; i != Anims.end() && i->first == it->first; ++i) if (filter(i->second))
//   return true; return false;` -- rotated: the first end test before the loop, the key re-compared through both pointers
//   in memory, end() re-read after every virtual call; two separate `xor al,al` / `mov al,1` exits.
// Label correction: catalogued as `CAnimationDef_FindAnimationByName`; the PDB name is CAnimationSet::AnimExists.
class CCharString {
public:
    void* PStringData;
};

class CDefString { public:                                        // one word: the def string table position
    long TablePos;                                         // +0x00
};

class CAnimationEntry { public:                                   // Ego_r.pdb, 0x10 bytes
    long       BankIndex;                                  // +0x00
    CDefString AnimName;                                   // +0x04
    CDefString GroupName;                                  // +0x08
    void*      Components;                                 // +0x0c CIVCountedPointer<CAnimComponentMap>
};

class CAnimFilterBase { public:                                   // vtable 0x0122FFCC
    virtual bool operator()(const CAnimationEntry& entry) const = 0;   // slot 0
    virtual CAnimFilterBase* Clone() const = 0;            // slot 1
    virtual ~CAnimFilterBase() { }                         // slot 2
};

template <class K, class V> struct pair {
    K first;
    V second;
};

template <class K, class V> class CVectorMap { public:            // retail: a 12-byte STLport vector + comparator/dirty bytes
    typedef pair<K, V> value_type;
    value_type* m_first;                                   // +0x00
    value_type* m_last;                                    // +0x04
    value_type* m_end;                                     // +0x08
    unsigned char Compare;                                 // +0x0c
    bool Dirty;                                            // +0x0d
    const value_type* end() const { return m_last; }
};

class CAnimationSet { public:                                     // layout of the landed ctor 0x005DD2E0 / TransferBinaryOut 0x005DC070
    typedef CVectorMap<unsigned long, CAnimationEntry>::value_type value_type;
    CVectorMap<unsigned long, CAnimationEntry> Anims;      // +0x00 (CRC of the name -> entry; duplicates allowed)
    long       DefaultFlags;                               // +0x10
    long       DefaultTransitionTime;                      // +0x14
    long       DefaultDelay;                               // +0x18
    CDefString DefaultGroup;                               // +0x1c
    const value_type* FindFirstAnimWithName(const CCharString& name) const;   // 0x005DC020
    bool AnimExists(const CCharString& name, const CAnimFilterBase& filter) const;
    const CAnimationEntry& GetAnim(const CCharString& name, long index, const CAnimFilterBase& filter) const;
};

bool CAnimationSet::AnimExists(const CCharString& name, const CAnimFilterBase& filter) const
{
    const value_type* it = FindFirstAnimWithName(name);
    for (const value_type* i = it; i != Anims.end() && i->first == it->first; ++i)
        if (filter(i->second))
            return true;
    return false;
}