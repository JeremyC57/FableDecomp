// std::pair::~pair() -- (pair<CCharString, CCountedPointer<CConsoleInputBase> >) implicit destructor: release the counted pointer (+0x04), then ~CCharString on the name (tail jump)
// Leaf: ??1?$pair@VCCharString@@V?$CCountedPointer@VCConsoleInputBase@@@@@std@@QAE@XZ
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x009EEA50, 60 bytes (int3 padding 0x009EEA8C..8F): push esi ; mov esi,ecx ; the inline
//   ~CCountedPointer on second (+0x04, Info +0x08: dec / cmp / DeleteFunc / delete Info ; Data = 0, Info = 0) ; mov ecx,esi ;
//   pop esi ; jmp ~CCharString 0x0099EAE0 (first, +0x00). Unreferenced COMDAT (retail keeps them; no call site).
// Identity (proven): the console TU (CConsole 0x009EA5A0 .. 0x009EF360, compiled /Gr: std::_Construct 0x009EEE50 takes ecx/edx) holds
//   three byte-identical 60-byte bodies 0x009EEA50 / 0x009EEA90 / 0x009EF260 and the pair ??_G 0x009EEDC0. The TU's only pair types
//   with a CCharString at +0 and a CCountedPointer at +4 are the map value_type pair<const CCharString, CCountedPointer<CConsoleInputBase> >
//   (CConsole::RegisteredConsoleInputs; FableWin.pdb ??1 / ??_G of it) and the make_pair temporary pair<CCharString,
//   CCountedPointer<CConsoleInputBase> > that CConsole::AddInput 0x009EC5E0 builds at [esp+18h] before converting it to the value_type at
//   [esp+24h] (FableWin.pdb lists that non-const pair type in the same TU: ??$construct@U?$pair@VCCharString@@V?$CCountedPointer@..).
//   A lane compile of that AddInput shape plus the map's _M_erase under /Gr (probe/009eea50_exp1.cpp) emits, in section order,
//   ??1 pair<CCharString, ..> -> ??1 pair<const CCharString, ..> -> ??_G pair<const ..> -> _Destroy<pair<const ..> > -- the retail
//   address order 0x009EEA50 -> 0x009EEA90 -> 0x009EEDC0 -> 0x009EF260 (chunk 661's ??1 / ??_G / _Destroy triple for the value_type,
//   with the temporary's ??1 first). So this row is the make_pair temporary's destructor. The catalog label `_Dest_val<..
//   C3DSkeletalMorphTarget..>` is propagated; the landed 0x009EEA90 / 0x009EF260 / 0x009EEDC0 carry descriptive stand-ins (NAME_FIXES.tsv).
// Callees: the count block's DeleteFunc (`mov ecx,[eax+8] ; call [eax+4]`), operator delete 0x00BFE9BC (??3@YAXPAX@Z),
//   CCharString::~CCharString 0x0099EAE0 (??1CCharString@@QAE@XZ, tail jump).
// Layout: STLport 4.6 `pair` (struct, U-keyed as both PDBs mangle it) of CCharString (fable_string.h, 4 bytes) and CCountedPointer
//   (engine/CCountedPointer.h, its own Data-then-Info dtor body).
// Shape: the implicit pair destructor; the harness below (a local pair destroyed by name, ungraded) makes VC7.1 emit the ??1.
#include "fable_string.h"
#include "engine/CCountedPointer.h"

namespace std {
template <class _T1, class _T2> struct pair {           // STLport 4.6 stl/_pair.h
    typedef _T1 first_type;
    typedef _T2 second_type;
    _T1 first;
    _T2 second;
    pair() : first(_T1()), second(_T2()) { }
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) { }
    template <class _U1, class _U2> pair(const pair<_U1, _U2>& __p) : first(__p.first), second(__p.second) { }
};
}

class CConsoleInputBase;
FABLE_STATIC_ASSERT(sizeof(std::pair<CCharString, CCountedPointer<CConsoleInputBase> >) == 0xC);

// TU harness (not a retail function): a pair<CCharString, CCountedPointer<CConsoleInputBase> > destroyed by name, as the make_pair
// temporary in CConsole::AddInput 0x009EC5E0 is.
void TUHarness_ConsoleInputPair(const CCharString& name, const CCountedPointer<CConsoleInputBase>& input)
{
    std::pair<CCharString, CCountedPointer<CConsoleInputBase> > entry(name, input);
}