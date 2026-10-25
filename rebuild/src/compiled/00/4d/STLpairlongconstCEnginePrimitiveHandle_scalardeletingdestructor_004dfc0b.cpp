// std::pair<const long, CEnginePrimitiveHandle>::`scalar deleting destructor'(unsigned int flags) -- run the pair destructor, then free the pair for flag bit 0
// Leaf: ??_G?$pair@$$CBJVCEnginePrimitiveHandle@@@std@@QAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004DFC0B, 28 bytes, thiscall `ret 4`: `push esi ; mov esi,ecx ; call 0x004DD728 ;
//   test byte [esp+8],1 ; je ; push esi ; call 0x00BFE9BC ; pop ecx ; mov eax,esi ; pop esi`.
// Identity (sweep1, 2026-10-25; found with FINDINGS chunk 455's 0x4DD728): the destructor it calls is
//   std::pair<const long, CEnginePrimitiveHandle>::~pair 0x004DD728 (renamed this pass; the landed
//   map<long, CEnginePrimitiveHandle>::operator[] 0x005BBFD6 binds ??1?$pair@$$CBJVCEnginePrimitiveHandle@@@std@@QAE@XZ ->
//   004dd728), so this is that pair's scalar deleting destructor: an unreferenced COMDAT (no code references, as the
//   CAStarState pair's ??_G 0x00A205C0 and the CCharString map pair's 0x00CD22C0), operator delete 0x00BFE9BC. The landed
//   "CLandscapeBackgroundPatch::`vector deleting destructor'" label is the propagated family name of this byte shape.
// Model: the pair with its destructor out of line; Kill_004dfc0b only forces the deleting destructor into the object.
#pragma optimize("s", on)
void __cdecl operator delete(void* block);         // 0x00BFE9BC
class CEnginePrimitiveHandle {
public:
    void* m_vtable;                                // +0x00 (vtable 0x0122F598)
    void* Data;                                    // +0x04 CIVCountedPointer
};
namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;                                     // +0x00
    _T2 second;                                    // +0x04
    ~pair();                                       // 0x004DD728 for <const long, CEnginePrimitiveHandle>
};
}

void Kill_004dfc0b(std::pair<const long, CEnginePrimitiveHandle>* object)   // forces the scalar deleting destructor into the object; not part of the retail body
{
    delete object;
}
