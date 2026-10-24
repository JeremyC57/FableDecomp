# Hand chunk 347 (_Adjust_heap<pair<float,EConversationAttitude>> .. _Cons_val<CAppearanceModifierGraphics::CEntry>): 39 of 40 byte-exact

## What matched (39)
- **Dinkumware `_Adjust_heap` (19 rows; all `/Gr`, first in ecx, hole in edx, bottom / value-by-value / comparator on the stack).**
  One body for every instantiation: `top = hole; idx = 2*hole+2; for (; idx < bottom; idx = 2*idx+2) { if (pred(first[idx], first[idx-1])) --idx; first[hole] = first[idx], hole = idx; } if (idx == bottom) {...} _Push_heap(...)`.
  The element model is what decides the bytes:
  - 8-byte (key, dword) pairs: 0x0088CA20 (signed key, GREATER comparator `jle`), 0x0073D7C0 / 0x009B2FF0 / 0x009B3060 (unsigned less `jae`) -- pair with a user memberwise copy ctor (value built in its slot `sub esp,8 ; mov eax,esp`); 0x00B852F0 -- POD pair with NO copy ctor (value pushed dword by dword).
  - Lexicographic `std::pair` operator< `x.first < y.first || (!(y.first < x.first) && x.second < y.second)`: 0x00AE5FB0 (long first, `jg/jl`), 0x00A6B4F0 (unsigned first, `ja/jb`).
  - 16-byte (unsigned __int64 key, X): 0x00A7F0F0 (X = bool: the slot copy writes ONE byte at +8), 0x00A7F1A0 (X = dword); 64-bit compare `cmp hi ; ja ; jb ; cmp lo ; jae`.
  - 0x00769D50 (ulong, C3DVector) and 0x00A32440 pair<ulong, C3DHelperPoint>: C3DVector needs a memberwise USER copy ctor (an implicit one copies it as a block via `add eax,4`).
  - 0x004C2C70 / 0x004C2D10: NGraphicAppearance::CMeshInfo {Index, SizeX, PrimaryMesh:1} with CSortPrimaryMeshesFirst/Last (`pred(a,b) = a.PrimaryMesh` / `!a.PrimaryMesh`); CMeshInfo must have the IMPLICIT copy ctor (a user one re-merges the bitfield with xor/and).
  - 0x0076A4E0 pair<ulong, CParticleUpdateParamDesc<ulong,ulong>> (vptr at +4 skipped by implicit operator=, vtable stored by implicit copy, out-of-line CBase dtor on the by-value pair).
  - 0x006A3300 pair<long, CIntelligentPointer<CThing>> (operator= = GetPItem/SetPItem, copy = base ctor + vtable + Get/Set, base dtor at the end).
  - 0x00992B80 pair<long, sound-bank record> with out-of-line record copy/assign/dtor (landed 0x009922F0 / 0x00992630 / 0x00992510).
  - 0x006FBD60 pair<ulong, CSmallVector<CEvent,32>> (`&other != this` around Copy; copy ctor zeroes then Copy; `delete[] Start`).
  - 0x00593E3E: /Os TU (`#pragma optimize("s", on)`), every helper out of line (comparator thiscall on its own stack slot).
  - 0x009D3750: unsigned long indices with CBankFile::CompareDataOffset, the retail bank model of the landed 0x009D2FC0.
- **Placement-copy `_Cons_val` / STL construct helpers (`/Gr`, dest in ecx, source in edx, null-tested placement new):**
  0x004AA030 / 0x00511F50 / 0x0074CC90 / 0x00A53520 (CIntelligentPointer<T>, four vtables), 0x007696A0 ParticleAndEndPoints,
  0x00786D60 {long, CCharString, long}, 0x009EE180 CConsoleCommandLine (user copy ctor that default-constructs CBase through its
  out-of-line ctor 0x0099A2D0), 0x009EDFE0 polymorphic CGenericVar (implicit copy), 0x00A31E70 C3DGeneratorObject (52-byte POD block
  = one `rep movsd`), 0x00A49B70 CLipSyncPhonemeSample (counted pointer copy).
- **Rows that are really something else (label = only the inlined construct step):**
  - Copy constructors (thiscall, `ret 4`, return this): 0x006C5460 CWitnessesSearchingDeed (base ctor + vtable BEFORE the leading dword
    copies = Recipient default-constructed and body `*this = other`), 0x00769040 AttackParticleAndEndPoints, 0x009BDE00 CSubTitle,
    0x009EA2E0 CConsoleCommandLine.
  - STLport list `_M_create_node` (malloc node, construct `_M_data` at +8, /Os): 0x005BAC52 (CDefPointer), 0x00578F1A (28-byte POD).
  - `std::__uninitialized_fill_n` 0x00A31F20 (C3DGeneratorObject) and `std::__uninitialized_copy` 0x00673D30 (pair<long, 40-byte POD>,
    STLport explicit pair copy ctor = key by register + POD `rep movsd`; plain MATCH).
  - 0x009C6640: a counted-pointer deleter (`if (p) { p->~T(); operator delete(p); }`) of a record of two CWideStrings.
  - 0x00CA1750: zero a 0x460-byte block (`rep stosd`) and free it through the MSVCR71 import (dllimport `free`).

## Parked for Fable 5.1 (1)
- 00c969b0 _global::FFT_InitFactorsUnlessTrivial_00c969b0 DIFFER(23v17): retail takes n in EAX, table in EDX, factors in ECX (`cmp eax,1 ; je ; push ecx ; lea ecx,[edx+eax*4] ; push ecx ; push eax ; call 0xC96800 ; ret`); ours loads n from the stack and `ret 4` | tried: __fastcall stand-in with a __stdcall callee | next: lane gap: ltcg (custom register-argument convention; the target itself receives register args, so ltcg_grade.py cannot grade it either -- GOTCHAS b84)

## Skipped
- none.  Bounds: all 40 rows hold exactly one function (trailing int3 only).  Note 0x00C969B0 is a jmp target (reached only by the
  tail `jmp` at 0x00C9BC80 of the FFT state ctor 0x00C9BC50), not a normal call entry.

## Label corrections
- 0x0088CA20 pair<float,EConversationAttitude>: the key is a signed long and the comparator is "greater" (min-heap).
- 0x0073D7C0 / 0x009B2FF0 / 0x009B3060 / 0x00B852F0 pair<ulong, CParticleUpdateParamDesc<ulong,ulong>>: 8-byte stride; the descriptor is 12 bytes, so the payload is one dword.
- 0x0076A4E0 pair<ulong, CRandomAppearanceMorph::CTextureMorph>: really pair<ulong, NParticleEngine::CParticleUpdateParamDesc<ulong,ulong>> (vtable 0x0125FEE4).
- 0x004C2C70 / 0x004C2D10 (CTextureMorph pair): really NGraphicAppearance::CMeshInfo with CSortPrimaryMeshesFirst / CSortPrimaryMeshesLast (callee labels 0x004C1F80 / 0x004C2000 agree).
- 0x00992B80 pair<long,EHeroAbility>: 36-byte pair<long, sound-bank record>.
- 0x00A7F0F0 / 0x00A7F1A0 pair<pair<long,long>,CEngineTextureDecal>: 16-byte (unsigned __int64, bool / dword) pairs.
- 0x00769D50 pair<ulong,C3DHelperPoint>: 16-byte stride, 12-byte payload (the 20-byte pair is 0x00A32440).
- 0x00786D60 _Cons_val<CNavRegion>: record {long, CCharString, long}; CNavRegion has no string.
- 0x009C6640 _Cons_val<CIntelligentPointer<const CThing>>: counted-pointer deleter of a two-CWideString record.
- 0x00C969B0 _Cons_val<CCharString, const char(&)[5]>: FFT factor step (LTCG register args).
- 0x00CA1750 _Cons_val<C3DGeneratorObject>: zero-and-free of a 0x460-byte block.
- 0x006C5460 / 0x00769040 / 0x009BDE00 / 0x009EA2E0: copy constructors, not _Cons_val.  0x005BAC52 / 0x00578F1A: list::_M_create_node.
  0x00A31F20: __uninitialized_fill_n.  0x00673D30: __uninitialized_copy.
- Landed naming noise noticed: the sound-bank record's three out-of-line members are landed under three different names
  (SRecord::SRecord 0x009922F0, "CCamera::UpdateCleanupFlags" 0x00992630, CSoundBankRecord::~CSoundBankRecord 0x00992510).
