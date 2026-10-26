// CV_AssassinAttacksScript::InitialiseRegionSpecificInfo() -- for each of the five regions, either load its kill count from RegionKillCounts (once activated) or clear the saved count
// Retail: Fable.exe @ 0x00E37CF0, 84 bytes (+ int3 padding). Called once (0x00E37A4C).
// Callees: CScriptBase::IsActiveThreadTerminating @ 0x00CB7940 (the terminate check before each action).
// Layout: Ego_r.pdb CV_AssassinAttacksScript - 0x14: Activated +0x50, RegionInfo (RegionData[5], 16 bytes: name, count,
// max, assassinActive) +0x58, RegionKillCounts (STLport vector<long>, first pointer) +0xa8.
// Shape: a counted `for` over 5 regions (the index strength-reduced to a byte offset in esi, `&RegionInfo[i].count`
// in ebx); each arm does `if (terminating) return;` first; the vector element is reached as `*(begin() + i)`
// (STLport operator[]), re-reading the first pointer in each arm.
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (wrapped, struct->class). Same bytes.
class CCharString { public:
    void* m_pData;
};

struct RegionData {
    CCharString name;                                    // +0x00
    long count;                                          // +0x04
    long max;                                            // +0x08
    bool assassinActive;                                 // +0x0c
};

template <class T>
struct CRetailVector {                                   // STLport vector: three pointers
    T* m_pFirst;
    T* m_pLast;
    T* m_pEnd;
    T* begin() { return m_pFirst; }
    T& operator[](unsigned long n) { return *(begin() + n); }
};

namespace NScript {
class CV_AssassinAttacksScript {
public:
    unsigned char _base[0x48];
    long RegionIndex;                                    // +0x48 (AA_LocationIdentifier)
    long AssassinObjectCount;                            // +0x4c
    bool Activated;                                      // +0x50
    CCharString RegionOfMostRecentKill;                  // +0x54
    RegionData RegionInfo[5];                            // +0x58
    CRetailVector<long> RegionKillCounts;                // +0xa8
    bool IsActiveThreadTerminating();                    // 0x00CB7940
    void InitialiseRegionSpecificInfo();
};
} // namespace NScript
using NScript::CV_AssassinAttacksScript;

void NScript::CV_AssassinAttacksScript::InitialiseRegionSpecificInfo()
{
    for (long i = 0; i < 5; ++i) {
        if (Activated) {
            if (IsActiveThreadTerminating())
                return;
            RegionInfo[i].count = RegionKillCounts[i];
        } else {
            if (IsActiveThreadTerminating())
                return;
            RegionKillCounts[i] = 0;
        }
    }
}