// CTCHeroPodium::GetCrowdPos(const C3DVector& pos, C3DVector& crowd_pos) -- claims a crowd slot (at most eight occupants): takes the best vacant position for pos (always slot 0 at the origin in this build), marks the slot occupied, bumps the occupant count
// Leaf: ?GetCrowdPos@CTCHeroPodium@@QAE_NABVC3DVector@@AAV2@@Z
// Plan phase 4 (2026-11-05), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00771D50, 85 bytes (+11 int3), thiscall `ret 8`. Single caller 0x00957D86.
// Name: Ego_r.pdb ?GetCrowdPos@CTCHeroPodium@@QAE_NABVC3DVector@@AAV2@@Z (donor order: OnInitialActivate, GetPositionForMember,
//   GetBestVacantPositionForThisPos, GetCrowdPos, ctor, FrameUpdate -- retail 0x00771BD0, 0x00771C70, (inlined), 0x00771D50,
//   0x00771DB0, 0x00771DE0, all CTCHeroPodium in the landed tree).
// Shape: the 16-byte frame is the inlined private const helper's return value
//   std::pair<C3DVector, long> GetBestVacantPositionForThisPos(const C3DVector&) const (Ego_r.pdb name, 0x20-byte donor body):
//   C3DVector's user copy constructor (??0C3DVector@@QAE@ABV0@@Z, FableWin/Ego_d) keeps the pair's 12-byte first in the frame
//   (`mov [esp],0 ; mov edx,[esp] ; mov [eax],edx` per component) and the long second in the dead fourth word (its 0
//   folds into CrowdOccupation[0]). Without the user copy constructor VC7.1 scalarises the pair and keeps only the
//   12-byte vector temporary (`sub esp,0Ch` -- the earlier parked residue).
// Layout (Ego_r.pdb CTCHeroPodium, retail STLport vector 12 bytes, as the landed GetPositionForMember 0x00771C70):
//   NumberOfCrowdOccupants +0x0c, CrowdOccupation vector<long> +0x10, E_X +0x1c, E_Y +0x28. No engine header exists.
#include "engine/C3DVector.h"
namespace std {
template <class _T1, class _T2>
struct pair {
    typedef _T1 first_type;
    typedef _T2 second_type;
    _T1 first;
    _T2 second;
    pair() : first(_T1()), second(_T2()) {}
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) {}
};
template <class _Tp, class _Alloc> class vector;
template <class _Tp> class allocator { };
template <class _Tp, class _Alloc = allocator<_Tp> >
class vector {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;
    _Tp& operator[](unsigned int __n) { return *(_M_start + __n); }
};
}
class CThing;
class CTCHeroPodium {
public:
    void* __vftable;                                    // +0x00 (CTCBase)
    CThing* ParentThing;                                // +0x04
    bool ToBeRemoved;                                   // +0x08
    bool IsInGlobalUpdateTCs;                           // +0x09
    long NumberOfCrowdOccupants;                        // +0x0c
    std::vector<long> CrowdOccupation;                  // +0x10
    C3DVector E_X;                                      // +0x1c
    C3DVector E_Y;                                      // +0x28
    bool GetCrowdPos(const C3DVector& pos, C3DVector& crowd_pos);
private:
    std::pair<C3DVector, long> GetBestVacantPositionForThisPos(const C3DVector& pos) const
    {
        return std::pair<C3DVector, long>(C3DVector(0.0f, 0.0f, 0.0f), 0);
    }
};
bool CTCHeroPodium::GetCrowdPos(const C3DVector& pos, C3DVector& crowd_pos)
{
    if (NumberOfCrowdOccupants == 8)
        return false;
    std::pair<C3DVector, long> best = GetBestVacantPositionForThisPos(pos);
    crowd_pos = best.first;
    CrowdOccupation[best.second] = 1;
    ++NumberOfCrowdOccupants;
    return true;
}