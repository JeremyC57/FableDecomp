// std::__destroy_aux(TeleportData* first, TeleportData* last, const __false_type&) -- run the destructor of every teleport record in [first, last) (its four strings, last member first)
// Leaf: ??$__destroy_aux@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CDB640, 59 bytes, __fastcall (first ecx, last edx; the empty tag reference on the stack,
//   `ret 4`).
// Callers: 0x00CDB856, 0x00CDB9A6, 0x00CDBC9E, 0x00D18DCE (the vector<TeleportData> clear / erase / grow paths).
// Callees: CCharString::~CCharString 0x0099EAE0 (x4).
// Element: Ego_r.pdb TeleportData (0x18 bytes): CCharString defName +0x0, unsigned long appearanceSeed +0x4, float health
//   +0x8, CCharString scriptName +0xC, teleportTo +0x10, teleportMkr +0x14 -- the implicit destructor destroys the strings
//   in reverse declaration order.
// Label: catalog "vector<std::pair<CCharString ...>::_Destroy" is propagated; the body is STLport's __destroy_aux.
// Shape: STLport 4.6 `for (; __first != __last; ++__first) _Destroy(&*__first);` with `_Destroy(p) { p->~_Tp(); }`.
class CCharStringData;
class CCharString {
public:
    CCharStringData* PStringData;               // +0x0
    ~CCharString();                             // 0x0099EAE0
};

class TeleportData {                            // Ego_r.pdb, 0x18 bytes
public:
    CCharString   defName;                      // +0x00
    unsigned long appearanceSeed;               // +0x04
    float         health;                       // +0x08
    CCharString   scriptName;                   // +0x0C
    CCharString   teleportTo;                   // +0x10
    CCharString   teleportMkr;                  // +0x14
};

namespace std {
struct __false_type { __false_type() { } };

template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }

template <class _ForwardIterator>
inline void __fastcall __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&)
{
    for (; __first != __last; ++__first)
        _Destroy(&*__first);
}

template void __fastcall __destroy_aux(TeleportData*, TeleportData*, const __false_type&);
}