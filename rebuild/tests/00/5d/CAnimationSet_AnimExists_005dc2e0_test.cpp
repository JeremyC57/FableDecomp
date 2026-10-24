// Behaviour test of CAnimationSet::AnimExists(const CCharString& name, const CAnimFilterBase& filter) (retail 0x005DC2E0)
// The model and the function body are the source's own; the retail callees are given test bodies below.
#include <cstdio>
#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#define __forceinline inline
#endif

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

bool CAnimationSet::AnimExists(const CCharString& name, const CAnimFilterBase& filter) const
{
    const value_type* it = FindFirstAnimWithName(name);
    for (const value_type* i = it; i != Anims.end() && i->first == it->first; ++i)
        if (filter(i->second))
            return true;
    return false;
}

// ---- test bodies of the retail callees ----
// test FindFirstAnimWithName: the name's payload points at its CRC; the first entry with that CRC, else end()
const CAnimationSet::value_type* CAnimationSet::FindFirstAnimWithName(const CCharString& name) const   // 0x005DC020
{
    unsigned long crc = *(const unsigned long*)name.PStringData;
    const value_type* it = Anims.m_first;
    while (it != Anims.m_last && it->first < crc)
        ++it;
    return (it != Anims.m_last && it->first == crc) ? it : Anims.m_last;
}
struct TBankFilter : CAnimFilterBase {
    long Bank;
    bool operator()(const CAnimationEntry& entry) const { return entry.BankIndex == Bank; }
    CAnimFilterBase* Clone() const { return 0; }
};
static bool Exists(const CAnimationSet& set, unsigned long crc, long bank)
{
    CCharString name;
    name.PStringData = &crc;
    TBankFilter filter;
    filter.Bank = bank;
    return set.AnimExists(name, filter);
}
int main()
{
    static const unsigned long keys[5] = { 5, 7, 7, 7, 9 };
    static pair<unsigned long, CAnimationEntry> entries[5];
    for (int i = 0; i < 5; ++i) {
        entries[i].first = keys[i];
        entries[i].second.BankIndex = i + 1;
        entries[i].second.AnimName.TablePos = 0;
        entries[i].second.GroupName.TablePos = 0;
        entries[i].second.Components = 0;
    }
    CAnimationSet set;
    set.Anims.m_first = entries;
    set.Anims.m_last = entries + 5;
    set.Anims.m_end = entries + 5;
    bool ok = Exists(set, 7, 3)                             // the second of three entries named 7 passes the filter
           && !Exists(set, 7, 5)                            // the run stops at the next name (9)
           && !Exists(set, 8, 1)                            // no such name
           && Exists(set, 9, 5)                             // the last entry: the run stops at end()
           && Exists(set, 5, 1);
    if (!ok) { std::printf("005dc2e0_TEST FAIL\n"); return 1; }
    std::printf("005dc2e0_TEST PASS\n");
    return 0;
}