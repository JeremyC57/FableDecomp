// _STL::fill(CSharerParserOutputToDebugContextMapper::CEntry* first, CSharerParserOutputToDebugContextMapper::CEntry* last, const CSharerParserOutputToDebugContextMapper::CEntry& value) -- assign one line-mapping entry to every element of a range
// Leaf: ??$fill@PAVCEntry@CSharerParserOutputToDebugContextMapper@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00527C10, 52 bytes, `__fastcall` (first ecx, last edx, value
//   pointer on the stack), `ret 4`.
// Identity: the manifest label agrees with the bytes: 12-byte stride and the element is Ego_r.pdb
//   CSharerParserOutputToDebugContextMapper::CEntry {long SrcLine; long DestLine; CCharString DebugContext}.
//   One caller, 0x0052BADA (the vector's fill-insert).
// Callees: CCharString::operator=(const CCharString&) @ 0x0099EFB0.
// Shape: STLport `for (; first != last; ++first) *first = val;` with CEntry's implicit operator= inline (the two
//   longs by register, then the string assignment); &val.DebugContext is hoisted out of the loop.

class CCharString {
public:
    void* PStringData;
    CCharString& operator=(const CCharString& other);    // 0x0099EFB0
};

class CSharerParserOutputToDebugContextMapper {
public:
    class CEntry {                                       // 12 bytes
    public:
        long        SrcLine;                             // +0x00
        long        DestLine;                            // +0x04
        CCharString DebugContext;                        // +0x08
    };
};

namespace _STL {
// STLport 4.6 fill; the algorithm templates are __fastcall here (first/last in ecx/edx, callee pops the value pointer).
template <class _ForwardIter, class _Tp>
void __fastcall fill(_ForwardIter __first, _ForwardIter __last, const _Tp& __val)
{
    for ( ; __first != __last; ++__first)
        *__first = __val;
}
}

template void __fastcall _STL::fill<CSharerParserOutputToDebugContextMapper::CEntry*, CSharerParserOutputToDebugContextMapper::CEntry>(
    CSharerParserOutputToDebugContextMapper::CEntry*, CSharerParserOutputToDebugContextMapper::CEntry*,
    const CSharerParserOutputToDebugContextMapper::CEntry&);