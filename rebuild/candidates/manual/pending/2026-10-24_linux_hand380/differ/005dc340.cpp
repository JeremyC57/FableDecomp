// Grade: DIFFER(56v66) -- retail keeps the dead `this` in edi across the FindFirstAnimWithName call (four pushes, `mov edi,ecx`; filter in ebx, index in ebp), the lane saves only three registers (filter edi, index ebx); the loop body is otherwise identical
// CAnimationSet::GetAnim(const CCharString& name, long index, const CAnimFilterBase& filter) -- the index-th entry filed under the name's CRC that passes the animation filter (unchecked walk)
// Leaf: GetAnim
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005DC340, 66 bytes (+ padding to 80), `ret 0Ch` (__thiscall, returns the entry reference).
//   Ego_r.pdb: `?GetAnim@CAnimationSet@@QBEABVCAnimationEntry@@ABVCCharString@@JABVCAnimFilterBase@@@Z`.
// Callees: 0x005DC020 CAnimationSet::FindFirstAnimWithName, CAnimFilterBase virtual slot 0 `operator()(const CAnimationEntry&)`.
// Callers: 38 sites (CThingCreatureBase::GetAnimFromName and the creature code).
// Shape: `it = FindFirstAnimWithName(name); for (;;) { if (filter(it->second)) { if (index == 0) break; --index; } ++it; }
//   return it->second;` -- no end or key test (the callers check AnimExists / GetNoAnims first); filter in ebx, the
//   countdown in ebp, the result `lea eax,[esi+4]`.
// Label correction: catalogued as `Animation_FindNthValid`; the PDB name is CAnimationSet::GetAnim.
class CCharString {
public:
    void* PStringData;
};

struct CDefString {                                        // one word: the def string table position
    long TablePos;                                         // +0x00
};

struct CAnimationEntry {                                   // Ego_r.pdb, 0x10 bytes
    long       BankIndex;                                  // +0x00
    CDefString AnimName;                                   // +0x04
    CDefString GroupName;                                  // +0x08
    void*      Components;                                 // +0x0c CIVCountedPointer<CAnimComponentMap>
};

struct CAnimFilterBase {                                   // vtable 0x0122FFCC
    virtual bool operator()(const CAnimationEntry& entry) const = 0;   // slot 0
    virtual CAnimFilterBase* Clone() const = 0;            // slot 1
    virtual ~CAnimFilterBase() { }                         // slot 2
};

template <class K, class V> struct pair {
    K first;
    V second;
};

template <class K, class V> struct CVectorMap {            // retail: a 12-byte STLport vector + comparator/dirty bytes
    typedef pair<K, V> value_type;
    value_type* m_first;                                   // +0x00
    value_type* m_last;                                    // +0x04
    value_type* m_end;                                     // +0x08
    unsigned char Compare;                                 // +0x0c
    bool Dirty;                                            // +0x0d
    const value_type* end() const { return m_last; }
};

struct CAnimationSet {                                     // layout of the landed ctor 0x005DD2E0 / TransferBinaryOut 0x005DC070
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

const CAnimationEntry& CAnimationSet::GetAnim(const CCharString& name, long index, const CAnimFilterBase& filter) const
{
    const value_type* it = FindFirstAnimWithName(name);
    for (;;) {
        if (filter(it->second)) {
            if (index == 0)
                break;
            --index;
        }
        ++it;
    }
    return it->second;
}
