// _STL::pair::pair(const pair& other) -- copy constructor of the criminal-status map node value: copy the region name, block-copy the 0x44-byte status
// Leaf: ??0?$pair@$$CBVCCharString@@UMirroredCriminalStatus@CTCHeroStats@@@_STL@@QAE@ABU01@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0057F30A, 34 bytes, __fastcall `ret 4`, returns `this` (unaligned: a size-optimised TU).
// Identity: the value_type of CTCHeroStats::OutstandingCrimes (map<CCharString, CTCHeroStats::MirroredCriminalStatus>);
//   its callers are the TU's `_Construct(p, v)` helper 0x0057F9A5 (`if (p) new (p) value_type(v)`) and 0x0058041D.
//   Twin of the value constructor 0x0057ED6C. FableWin.pdb lists the Dinkumware twin ??0?$pair@$$CBVCCharString@@UMirroredCriminalStatus@CTCHeroStats@@@std@@...
// Pragma: #pragma optimize("s", on) -- the `push 11h ; pop ecx ; rep movsd` block copy.
// Callee: CCharString::CCharString(const CCharString&) @ 0x0099EC30.
// Shape: STLport's memberwise `pair(const pair& __o) : first(__o.first), second(__o.second)`; the status is a POD with an implicit
//   copy (17 dwords, `rep movsd` under /Os), its user default constructor (landed 0x0057A5DD) does not affect the copy.
#pragma optimize("s", on)

class CCharString {
public:
    void* PData;                                        // +0x00
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
};

class CTCHeroStats {
public:
    struct MirroredCriminalStatus {
        long  Crimes[10];                               // +0x00
        long  LastTimeSeenByGuard;                      // +0x28
        long  LastTimeExited;                           // +0x2C
        bool  ExitedUsingTeleporter;                    // +0x30
        float EntrancePosX;                             // +0x34
        float EntrancePosY;                             // +0x38
        float EntrancePosZ;                             // +0x3C
        float EntranceAngle;                            // +0x40
        MirroredCriminalStatus();                       // 0x0057A5DD
    };
};

namespace _STL {
template <class _T1, class _T2> struct pair {
    _T1 first;                                          // +0x00
    _T2 second;                                         // +0x04
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) {}
    pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) {}
};
template pair<const CCharString, CTCHeroStats::MirroredCriminalStatus>::pair(
    const pair<const CCharString, CTCHeroStats::MirroredCriminalStatus>&);
}