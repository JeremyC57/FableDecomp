#pragma optimize("s", on)
// std::pair<long, CCharString>::`scalar deleting destructor'(unsigned int flags) -- the compiler-emitted ??_G of the implicit destructor: ~CCharString on
//   second, then operator delete when bit 0 of flags is set
// Leaf: ??_G?$pair@JVCCharString@@@std@@QAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004DA68A, 31 bytes, thiscall, `ret 4`.
// Identity: catalog std::pair<long,CCharString>::`scalar deleting destructor'; called with flag 0 by __destroy_aux 0x004DD782 (the element destroy of
//   ~vector<pair<long, CCharString> > 0x004E34B2) -- VC7.1 calls the ??_G with flag 0 for a non-inlined implicit destructor (optimize("s") TU).
// Emitter: `delete` of a pair<long, CCharString>* makes the TU emit the ??_G (the Kill_ function is not part of the graded body).
// Naming wave 18: landed as the free __fastcall vector_deleting_destructor(CLandscapeBackgroundPatch*, int, unsigned int) stand-in (the
//   propagated CLandscapeBackgroundPatch label); re-authored as the compiler-emitted ??_G (name-only: the bytes are unchanged).
class CCharStringData;
class CCharString { public:
    CCharStringData* PStringData;
    ~CCharString();                             // 0x0099EAE0
};
namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
};
}
void Kill_004da68a(std::pair<long, CCharString>* p)   // forces the scalar deleting destructor into the object; not part of the retail body
{
    delete p;
}
