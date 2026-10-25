// std::_Construct(CVsyncCallbackTimer::CPendingQuery* p, const CVsyncCallbackTimer::CPendingQuery& val) -- STLport: placement-copy one 16-byte pending vsync query into a list node (unoptimised TU)
// Leaf: ??$_Construct@UCPendingQuery@CVsyncCallbackTimer@@U12@@std@@YIXPAUCPendingQuery@CVsyncCallbackTimer@@ABU12@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x009F7890, 82 bytes (+14 int3), `__fastcall` (p in ecx, &val in edx), `ret`.
// Identity: CVsyncCallbackTimer (Ego_r.pdb) holds two list<CPendingQuery> (FreeQueoryObjects +0x04, PendingQueories +0x10);
//   a list node's value is built by STLport's `_Construct(&node->_M_data, x)`. CPendingQuery is a 16-byte POD (copied as
//   four dwords). Label correction: the catalog's "_Cons_val<allocator<CPendingQuery>,...>" is the propagated Dinkumware
//   spelling; retail STL is STLport 4.x.
// Callees: placement operator new(unsigned int, void*) @ 0x00405C10 (out of line here: `mov eax,[esp+8] ; ret`).
// Layout: CPendingQuery {four dwords}; member names are not in the PDB layouts (descriptive: the D3D query object and
//   the frame it was issued for, plus two words).
// Shape: this TU region (0x009F77xx-0x009F78xx, see the landed 0x009F77F0/0x009F7810) is built with optimisation off:
//   both register arguments are homed (edx to [ebp-0Ch], ecx to [ebp-8]), the placement new is a real call whose result
//   is null-tested, the implicit copy constructor is the dword-by-dword block copy, and the new-expression's value is
//   stored to a dead temporary. `#pragma optimize("", off)` alone still expands the inline placement new (the pragma
//   does not reach /Ob); `inline_depth(0)` stands for the TU's /Ob0, which keeps the call. The /Gr calling convention is
//   spelled `__fastcall`.
#pragma optimize("", off)
#pragma inline_depth(0)

inline void* __cdecl operator new(unsigned int, void* where) { return where; }   // 0x00405C10 (STLport <new>)

struct IDirect3DQuery9;

class CVsyncCallbackTimer {
public:
    struct CPendingQuery {                               // 16 bytes
        IDirect3DQuery9* Query;                          // +0x00
        long             Frame;                          // +0x04
        long             Data0;                          // +0x08
        long             Data1;                          // +0x0c
    };
};

namespace std {

template <class _T1, class _T2>
inline void __fastcall _Construct(_T1* __p, const _T2& __val)
{
    new (__p) _T1(__val);
}

template void __fastcall _Construct<CVsyncCallbackTimer::CPendingQuery, CVsyncCallbackTimer::CPendingQuery>(
    CVsyncCallbackTimer::CPendingQuery*, const CVsyncCallbackTimer::CPendingQuery&);

}