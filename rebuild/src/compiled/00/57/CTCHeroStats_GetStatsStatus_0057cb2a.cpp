// CTCHeroStats::GetStatsStatus(std::vector<long>& status) -- append the hero's renown level and the renown points earned inside that level to status
// Leaf: ?GetStatsStatus@CTCHeroStats@@QBEXAAV?$vector@JV?$allocator@J@std@@@std@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0057CB2A, 33 bytes, thiscall const, `ret 4`; size-optimised TU (odd entry), no direct
//   caller or data reference (an unreferenced out-of-line member the link kept).
// Identity (proven): CTCHeroStats::GetStatsStatus(vector<long>&) const; evidence: Ego_r.pdb places GetStatsStatus (33 bytes, 0x0056F376 ..
//   0x0056F397) directly after UpgradeRenownLevel 0x0056F32F and ForceRenownLevelTo 0x0056F349 -- retail has the same order and the same
//   size here (UpgradeRenownLevel 0x0057CAE9, ForceRenownLevelTo 0x0057CB00 landed, this 33-byte body, then the TLC-era AddHousePosition
//   0x0057CB4B); the body appends two longs through vector<long>::push_back to its one reference argument, which is the PDB signature
//   ?GetStatsStatus@CTCHeroStats@@QBEXAAV?$vector@JV?$allocator@J@std@@@std@@@Z (FableWin.pdb and Ego_r.pdb).
// Label: the manifest's "ClipBox" is propagated from a byte-identical body elsewhere (family J080: 33-byte two-call bodies;
//   retail has no identical-code folding).
// Callees: std::vector<long>::push_back(const long&) 0x00485FF3 (agent952's landed spelling), twice.
// Layout: engine/CTCHeroStats.h (retail container sizes): RenownLevel +0x70, RenownPointsInLevel +0x78 -- the offsets the landed
//   ForceRenownLevelTo 0x0057CB00 writes (Ego_r.pdb +0x78 / +0x80, eight bytes later: two 16-byte PDB containers precede them).
// Shape: this in esi, the vector re-read from its stack slot before each call, the first address through eax, the last formed in
//   place (`add esi,78h`).
#pragma optimize("s", on)
namespace std {
template <class _Tp> class allocator { };
template <class _Tp, class _Alloc = allocator<_Tp> > class vector {
public:
    void push_back(const _Tp& __x);                       // 0x00485FF3 for long
    _Tp* _M_start;                                        // +0x00
    _Tp* _M_finish;                                       // +0x04
    _Tp* _M_end_of_storage;                               // +0x08
};
}
#define CTCHeroStats CTCHeroStatsPdb
#include "engine/CTCHeroStats.h"
#undef CTCHeroStats
class CTCHeroStats : public CTCHeroStatsPdb {             // engine/CTCHeroStats.h is data-only: the method is added here
public:
    void GetStatsStatus(std::vector<long>& status) const;
};
void CTCHeroStats::GetStatsStatus(std::vector<long>& status) const
{
    status.push_back(RenownLevel);
    status.push_back(RenownPointsInLevel);
}