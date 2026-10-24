# Hand chunk 348 (CAppearanceModifierGraphics::CEntry .. pair<CCharString const,CArray<unsigned long>>): 39 of 40 byte-exact

All 40 catalog labels are the propagated `_Cons_val<...>` family. 33 rows really are STLport `_Construct(T* p, const T& v)`
(`if (p) new (p) T(v)`, __fastcall-shaped: ecx = slot, edx = source); the other 7 are different functions (see Label corrections).
Every `_Construct` TU wraps its model in `namespace N<addr>` so the 33 same-named helpers stay distinct symbols.

## What matched (39)
- **_Construct, string-led records** (CCharString copy 0x0099EC30 / CWideString copy 0x0099B720 first, scalars after):
  CSymbolMap::CSymbolInfo x3 (009e5f60, 00a3fb60, 00cd4e30), CMeshDataBankInfo x2 (006c3170, 009eee50; CCountedPointer copy bumps
  Info->RefCount), CActivateQuestInfo 004b8130, CQuestCard 004b82f0, COptimisedBone 00ad15c0, pair<CCharString,C3DHelperPoint> x2
  (0088ab90, 00a6ed30), pair<CCharString,C3DVector> 0077e250, pair<CCharString,CRegisteredPSCInfo> 00af2b70, CActivity 00565780,
  CClanInfo 00566180, CClanMember 005661c0, CConversationLine 006e6570, CHeroLogBookEntry 0047b5c0, pair<CCharString,CScriptInfo>
  00cb60b0, pair<CCharString,CPersistInfo> 004b8160, pair<CCharString,C3DMeshVolume> 00a6fdc0, CEntityScriptInfo 0052b4e0.
  Shape: plain `new (p) T(value)` over the PDB member list. Fixed-offset `[esi+4..0xc]` moves of a C3DVector/C2DVector need the
  vector type to have a user memberwise copy ctor (otherwise VC block-copies the sub-struct through a re-based pointer).
- **_Construct, scalar-led records** (scalar first, then an out-of-line member copy): CQuestObjectiveDesc 0047b590,
  CUIMiniMapMarker 00613d20, CResourcePage 00f38410 (CArray<long> copy 0x00514160), CEnvironmentThemePriorityStackEntry 006b6670
  (CArray copy 0x006B58F0, double via fld/fstp), CUniqueVertex 00ad62b0 (two CIndexList copies 0x00AD3A10), CNavigationLayer
  00a85360 (three vector copies; CVectorMap = vector base + empty Compare + Dirty at +0xD), CProfileTreeEntry 009f9a90,
  CGraphicDataBankInfo 009f8780 (44-byte Init via rep movsd + string + counted pointer).
- **_Construct, all-scalar records**: CQuake 0069c860 and CResourceEntry 00913590 (single eax moves, no rep movsd): needs a
  non-trivial memberwise copy (user-copy C3DVector / C2DExtentsI members + a spelled-out copy ctor).
- **_Construct, intelligent-pointer led**: CMiniMapDisplay::CMarkerInfo 0082c5e0 (uses engine/CMarkerInfo.h; the opaque PThing
  bytes are placement-built as a CIntelligentPointer<const CThing>), NSpeechGainManager::CActiveDialogueInfo 006e4bd0. The inline
  CIntelligentPointer copy ctor = base ctor 0x00A01B10, own vtable store, SetPItem(other.GetPItem()).
- **CCardPositionsDef _Construct** 004ebdf1: implicit copy of a polymorphic def (CParentDefClassBase copy 0x0046EE98, vtable
  0x012439DC store, vector<C2DVector> copy 0x004EBE1A at +0x3C).
- **CDefendingCombatantInfo::CProng::CProng(const CProng&)** 006eff60: out-of-line copy ctor over the landed CCombatRing model's
  byte-packed head (Blocked +0, C3DVector at +1 copied as three unaligned dwords because C3DVector has a user memberwise copy).
- **std::__uninitialized_fill_n** 009fffc0 (0x2C-byte record) and 00b6bba0 (0x38-byte record): `for (; n > 0; --n, ++first)
  if (first) new (first) T(x); return first;` with ecx/edx/stack, `ret 4`.
- **CEngineLandscapeMemoryManager::FreeVirtualAligned(void*)** 00bd9ca0 (PDB name): `if (p) FreeVirtual(p - ((long*)p)[-1])`,
  tail jump; method projection over engine/CEngineLandscapeMemoryManager.h.
- **NUISystem::CIME::Shutdown()** 004051c0 (PDB name): IME finalise/disable/re-init-disabled, `delete` of the non-virtual symbol
  font (bare operator delete), then the private Reset inlined (member clears + 64-byte memset + tail jump to ResetToDefaultSettings).

Recurring lesson: VC7.1 /O2 calls the IMPLICIT copy ctor out of line (11-byte `test ecx,ecx ; push edx ; call`) once a record has
about three non-trivial members or a big scalar tail; spelling the memberwise copy ctor out with `__forceinline` restores retail's
inlined body (00565780, 00566180, 00613d20, 0069c860, 009f8780, 006b6670, 006e4bd0, 00cb60b0, 00913590).

## Parked for Fable 5.1 (1)
- 00beeda0 std::__pop_heap<pair<pair<long,long>,CEngineTextureDecal>*,...> DIFFER(147v147): retail loads `value.first.second` (`mov edx,[esp+18h]`) before `sub esp,1Ch` for the by-value copy of `value`, ours loads it after (and swaps the [eax+4]/[eax+8] store order) | tried: implicit pair copy, user copy ctor on pair<long,long>, user copy ctor on the outer pair | next: lane gap: scheduling (same instructions, one load hoisted); maybe a CEngineTextureDecal user copy ctor

## Skipped
- none. No row holds two functions. Note 004ebdf1 is an unaligned start (a densely packed TU: CCardPositionsDef::GetSizeofClass
  follows at 0x004EBE16); its 37-byte bound is right.

## Label corrections
- 009fffc0, 00b6bba0 `_Cons_val<CAppearanceModifierGraphics::CEntry>` -> std::__uninitialized_fill_n over a 0x2C / 0x38-byte POD
  (callers: _M_fill_insert 0x00A01290 in CGraphicDataBank's TU / the Struct0x38 vector helpers near 0x00B6C700).
- 006eff60 `_Cons_val<CProng>` -> CDefendingCombatantInfo::CProng::CProng(const CProng&) (thiscall, returns this; the landed
  CCombatRing ctor 0x006EC380 already names it).
- 00913590 `CResourceEntry` kept, but retail stride is 0x30 (AddResourceEntry 0x00913E70) vs PDB 0x2C: one extra trailing word.
- 006e6570 `CRandomAppearanceMorph::CBodyPartMesh` -> CConversationLine (long, long, CCharString, bool).
- 00cb60b0 `CProfileManager::CProfileTreeEntry` -> std::pair<CCharString, CScriptInfo> (neighbours are pair<CCharString,CScriptInfo> helpers).
- 0047b5c0 `pair<CCharString,CPersistInfo>` -> CHeroLogBookEntry (three CWideStrings; next to CHeroLogBook's 0x0047B590).
- 004ebdf1 `pair<CCharString const,CArray<unsigned long>>` -> _Construct of CCardPositionsDef.
- 00bd9ca0 `pair<CCharString,CCharString>` -> CEngineLandscapeMemoryManager::FreeVirtualAligned(void*).
- 004051c0 `CScreenFilterEntry` -> NUISystem::CIME::Shutdown().
- 00beeda0 `pair<CCharString,C3DVector>` -> std::__pop_heap for pair<pair<long,long>,CEngineTextureDecal> (callee 0x00BEEA00 is its __adjust_heap).
- Retail vs Ego_r.pdb: STLport vector / CArray bodies are 12 bytes in retail (PDB 16), so CResourcePage, CEntityScriptInfo,
  CNavigationLayer, CProfileTreeEntry, CEnvironmentThemePriorityStackEntry offsets are the retail ones (noted in each source).
