// std::pair<unsigned long,C3DMeshVolume>::pair() -- default-construct an indexed mesh-volume entry: zero key, empty volume name, null counted convex volume
// Leaf: ??0?$pair@KVC3DMeshVolume@@@std@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A327E0, 39 bytes, thiscall, plain `ret` (returns `this`).
// Identity: Ego_r.pdb has `??0?$pair@KVC3DMeshVolume@@@std@@QAE@XZ`; the element of C3DMeshStats' sorted
//   vector<pair<unsigned long, C3DMeshVolume> > (the landed neighbours 0x00A326D0 ~pair, 0x00A32F40 __uninitialized_copy
//   work on the same 16-byte pair). No direct callers: the out-of-line COMDAT copy of the constructor that vector::resize
//   uses for its `T()` default value.
// Callees: CCharString::CCharString() @ 0x0099E4B0 (the volume's Name).
// Layout: pair { unsigned long first +0x0; C3DMeshVolume second +0x4 = { CCharString Name +0x4,
//   CCountedPointer<CContainmentVolume> ConvexVolume { Data +0x8, Info +0xC } } }.
// Shape: STLport `pair() : first(_T1()), second(_T2())`: the key zero store comes first, `&second` is formed once (edi) for
//   the name's constructor call and the counted pointer's two zero stores (C3DMeshVolume's implicit constructor inlined).
class CCharString {
public:
    void* PStringData;                                       // +0x0
    CCharString();                                           // 0x0099E4B0
};

struct CCPPointerInfo;
template <class T> class CCountedPointer {
public:
    T*              Data;                                    // +0x0
    CCPPointerInfo* Info;                                    // +0x4
    CCountedPointer() : Data(0), Info(0) { }
};

class CContainmentVolume;
class C3DMeshVolume {                                        // Ego_r.pdb, 0xc bytes
public:
    CCharString                         Name;                // +0x0
    CCountedPointer<CContainmentVolume> ConvexVolume;        // +0x4
};

namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;                                               // +0x0
    _T2 second;                                              // +0x4
    pair() : first(_T1()), second(_T2()) { }
};
template struct pair<unsigned long, C3DMeshVolume>;
}