// _STL::swap(pair<unsigned __int64, bool>& a, pair<unsigned __int64, bool>& b) -- exchange two (64-bit key, flag) records through a temporary
// Leaf: ??$swap@U?$pair@_K_N@_STL@@@_STL@@YIXAAU?$pair@_K_N@0@0@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A7D520, 68 bytes (+12 int3 padding), __fastcall (a in ecx, b in edx), `ret`.
// Identity: the element is 16 bytes with a 64-bit key at +0 and one byte at +8 (8-byte alignment pads it): the Ego_r.pdb
//   name set instantiates the heap/sort/fill family over `pair<unsigned __int64, bool>` with
//   CKeyPairCompareLess<unsigned __int64, bool> (0x00BC2260..), and the neighbouring bodies here are that family (the
//   comparator 0x00A7EF60 compares the 64-bit first members unsigned; 0x00A7D570 is the linear insert over 16-byte
//   elements with the same key test). The free `swap` template is __fastcall (the game is built /Gr; the PDB's swap
//   instantiations are all `YIX...`). No call site survives in the binary (byte-identical twin 0x00A7EF90). The catalog
//   label CWeaponTrailEntry::CWeaponTrailEntry is propagated.
// Shape: STLport `_Tp __tmp = __a; __a = __b; __b = __tmp;`: the pair's memberwise user copy constructor copies key (esi/edi)
//   and flag (one frame byte) only, the implicit operator= copies all 16 bytes, so the second assignment carries the
//   temporary's never-written padding words.
namespace _STL {
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
    pair(const pair& __o) : first(__o.first), second(__o.second) {}
};

template <class _Tp> inline void __fastcall swap(_Tp& __a, _Tp& __b)
{
    _Tp __tmp = __a;
    __a = __b;
    __b = __tmp;
}

template void __fastcall swap<pair<unsigned __int64, bool> >(pair<unsigned __int64, bool>&, pair<unsigned __int64, bool>&);
}