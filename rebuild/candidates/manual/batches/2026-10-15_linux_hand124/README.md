# Hand chunk 124 (CBinaryTree_Alt .. CCharString): 33 of 40 byte-exact

All 33 sources in `src/` grade MATCH / RELOCATION_MATCH (`grades.txt`) and their behaviour tests print
`<addr>_TEST PASS` under g++ (`tall.sh`) and the lane's VC7.1 cl.exe (`tall.py`). 7 rows are parked in `differ/`.

## What matched (33)
- **STL instantiations behind placeholder labels** (CBinaryTree_Alt, CBlendedEnvironmentTheme rows):
  - 004f9960 STLport `_Rb_tree(const _Rb_tree&)` (8-byte values): size TU (`#pragma optimize("s", on)`, `pop ecx`
    after malloc), the `_Rb_tree_base` constructor `__forceinline`, and `_M_root()/_M_leftmost()/_M_rightmost()`
    accessors returning REFERENCES so `_M_root() = _M_copy(...)` loads the header before the argument pushes.
  - 006f04c0 STLport list `_S_merge(list&, list&, CThingCompare_Nearest)`: /Gr `__fastcall` free template, 12-byte
    comparator by value (its slot is `this` of the out-of-line operator()), `_M_transfer` inlined.
  - 00a70280 SGI `__pop_heap<pair<CCharString, C3DDummyObject>>` (named by its callee's manifest label): the pair's
    IMPLICIT assignment / copy constructor inline (CCharString members out of line, the 0x34-byte dummy object a
    13-dword `rep movsd`).
- **CBrainOutput** 006d6110 copy constructor: memberwise user copy constructors, the counted pointer's count bump
  and the family-44 intelligent-pointer copy inline.
- **CBriarRose / entity scripts** (quest-script module, names from the factory-name table and vtables):
  - 00d9e930 CBriarRose (minion camp) and 00eaed90 CMurderTwistSheriff constructors: the landed CVtable_012efee8
    model (Game and ParentClass plain initialisers before their vtables, member objects after, PMasterData body
    assignment last) plus `CTimer() : TimerIndex(g_pScriptTimerManager->NewTimer())`.
  - 00d9e9e0 CBriarRose::Init: the by-value CScriptThing argument must use the IMPLICIT copy constructor (reads the
    source words `this`-relative, `[esi+0Ch]`); a user copy constructor reads them through the reference.
  - 00da2610 CBriarRose::DoSpeech: `Game->Slot(speech, Game->GetHero())` nested calls fetch the outer vtable first.
  - 00d69f50 / 00d6a0d0 CHTE_Prisoner::Main / CHTE_Executioner::Main (entity slot 1): the landed
    CFadeinSummoner shape with a `do { Update(); } while (!IsDone());` wait loop.
- **CBufferChunk** 009c6020 `~CBufferChunk`: `if (Mem) GFFreeFromGlobalAllocator(Mem);` with the landed free function
  `__forceinline` (plain `inline` stays out of line); Leaf `??1CBufferChunk`.
- **CBulletTimeManager** 004c5ea0 UpdateCombatRelatedEffects: local model (the engine header is 4 bytes off).
- **CCRC** 00669d30 CalcAndLookup: two null-case-first CCharString accessors (length evaluated first), the hash
  homed in the dead argument slot, STLport `find(iterator* result, const key&)`.
- **CCamera**: 0049d380 copy constructor (memberwise, float members copied as raw dwords); 00a0c130 the 5-argument
  constructor (`FOVFlags(1)` in the init list, a body store schedules +0x2c early).
- **LSocket** 008f8c00 Attach (label CCamera::SetView propagated): two POD sockaddr_in copies + two
  `ConvertSockaddr2LAddress` fastcalls; retail LAddress is 0x1c bytes.
- **CCard** 008f0660 Move: the PDB packs the three C3DVectors at odd offsets (`#pragma pack(1)` = the real layout).
- **CCarriedReadableDef** 007e1e30 Transfer: `#include "engine/CCarriedReadableDef.h"` + a `_Methods` struct, an
  inline `CPersistContext::Transfer(CDefString&)` (CheckCRC("") then the two-case switch) applied twice.
- **CCategory** (retail-offset view, the header is 4 bytes off after the 12-byte vector):
  - 005bcf61 / 005bcfe2 Get{Next,Previous}OccupiedSlot (size TU): unsigned up-scan / signed down-scans, the
    WrapHighlightCursor read through the scoped CDefPointer reference.
  - 005bd9b2 SelectHighlightedItem: the item type and the inventory taken into named locals (in that order) before
    the selected/deselected if/else; written inline per call the lane jump-threads the flag (108v107).
  - 005bdac5 GetCategoryName and 005bf24a RemoveSelectedItem: frame-pointer size TUs (`optimize("y", off)` +
    `("s", on)`); the condition evaluated under a scoped def reference lands in a frame byte.
  - 005bf103 AddInventoryItem: the STLport `*(begin() + n)` element access keeps the scaled slot offset in edi.
  - 005bfcd9 the constructor: the def arrives as a by-value CDefPointer (`__forceinline` copy constructor and
    destructor under /Os), `PAssociatedThing = (CThing*)0` (an IMPLICIT conversion temporary re-addressed with lea).
- **CChangingStateComponent** 0052c7e0 Update: `InitialZoom.X = InitialZoom.Y = 1.0f` (one eax load, Y first);
  zeroes as separate immediate stores.
- **CCharString / CWideString**:
  - 0099ea60 CCharString::AllocStringData and 0099b3c0 CWideString::AllocStringData: one operator new hoisted above
    the length test; the wide one needs the `?:` form for the block order and STLport `_String_base` over an
    `_STLP_alloc_proxy` (else the tag `lea` schedules above the zero stores).
  - 0099b560 CWideString::MakeOwnCopyOfData (the narrow landed shape; tag temp in the high byte of the frame word).
  - 0099b860 CWideString::operator=(wchar_t): {c, 0} homed in the argument slot, the landed inline unassign.
  - 0099e520 CCharString::operator!=(char): `!(*this == c)` over a `__forceinline` operator== written
    null-test-positive (block order) with the strcmp intrinsic.
  - 0099f100 CCharString::operator+=(const char*): the block read into a local after MakeOwnCopyOfData, guarded
    do/while strlen.
  - 0099f450 CCharString::RemoveWhiteSpaceBegin: `GetText()[i]` (its null test folds against the loop test) and a
    NAMED GetSubString local for the inline operator= self test.

## Parked for Fable 5.1 (7)
- 00a0c1b0 CCamera::MoveSideways DIFFER(158v152): free arm loads every cross-product product with the operands reversed (`fld [ecx+0Ch] ; fmul [ecx+20h]` v retail `fld [ecx+20h] ; fmul [ecx+0Ch]`), so `Forward.Y*Up.Z` is not hoisted above the HeightLocked test | tried: inline GetRight (CrossProduct(Forward, Up) by constructor), assignment-built cross products in three operand orders, an argument-swapped helper, a SetCrossProduct local (shared and per arm) | next: a CRightHandedSet member returning the right vector through a hidden-pointer named local, or the locked arm from a separate source expression
- 0091a670 CAIStateGroupBase::WaitForMessageEvent<Bind3> DIFFER(120v120): `mov eax,4` sits before the loop-back `je` in retail, after it in the lane | tried: do/while, for(;;) with the return inside, a result local assigned in the loop, optimize("s") | next: lane gap: latch constant scheduling
- 00969760 CAIStateGroupBase::WaitForMessageEvent<Bind2> DIFFER(120v120): same `mov eax,4` placement as 0091a670 (byte-identical twin but for the find callee) | tried: the 0091a670 forms | next: lane gap: latch constant scheduling
- 00a5b020 CCachedFile::SetPosition DIFFER(124v128): retail `sub esp,8` keeps the thrown CBBBFileException in its own frame word, the lane homes it in the dead `position` slot (`push ecx` frame), shifting every esp offset | tried: exception named before the message, message in its own block, an inline ThrowFileError helper | next: lane gap: frame-slot sharing (QFE homes the throw object in a dead argument slot)
- 00ab15f0 CBoneInfo::FindChildIndex DIFFER(109v109 with /Oa; 102v109 without): only register choice differs under /Oa (retail eax for the child chain and the recomputed count, edx for the length compare; lane edx/ecx) | tried: index loop, iterator-pointer loop, a data-pointer compare helper, swapped length compare (all /Oa) | next: keep `// Flags: /Oa`; try the name block read into a named local before the loop
- 005bd0f7 CCategory::HighlightPreviousItem DIFFER(132v140): retail `mov eax,[esi+24h] ; dec eax ; js join ; test eax,eax ; mov [esi+24h],eax ; jge join` keeps an unreachable wrap/clamp block; the lane emits `dec [esi+24h] ; jns` (or folds the dead block, 87v140 / 95v140) | tried: `if (--h < 0) {wrap}`, a literal `h-1 >= 0` transcription, an inline setter | next: lane gap: RTM keeps dead code the DDK compiler folds
- 0099f4d0 CCharString::RemoveWhiteSpaceEnd DIFFER(120v135): the lane jump-threads GetLength's null arm (exits straight to the end), drops the per-iteration GetText null test and caches StringLength in edi; retail keeps all three | tried: if/return accessor, for/break loop, optimize("s") (103v135) | next: lane gap: jump threading across the inlined accessors

## Skipped
None: no row of this chunk spans two functions (every int3 run ends its row).

## Label corrections
- 008f8c00 "CCamera::SetView" is LSocket::Attach(SOCKET, const sockaddr_in&, const sockaddr_in&) (caller LSocket::Accept 0x008F8EA0).
- 0091a670 / 00969760 "CCOE_AltarEvil::Main" are CAIStateGroupBase::WaitForMessageEvent template instances (Bind3 / Bind2 message filters).
- 00d69f50 / 00d6a0d0 "CCOE_AltarEvil::Main" are CHTE_Prisoner::Main / CHTE_Executioner::Main (factory names "HTE_Prisoner" / "HTE_Executioner", vtables 0x012D20D8 / 0x012D211C, entity slot 1).
- 00eaed90 "CBriarRose::CBriarRose" is CMurderTwistSheriff::CMurderTwistSheriff (factory 0x00EAED10 "MurderTwistSheriff", vtable 0x012EFA0C).
- 00a70280 "CBlendedEnvironmentTheme::CBlendedEnvironmentTheme" is `__pop_heap<pair<CCharString,C3DDummyObject>*, ...>`.
- 004f9960 "CBinaryTree_Alt::CopyConstructor" is the STLport `_Rb_tree` copy constructor; 006f04c0 "CBinaryTree_Alt::Rebalance" is STLport list `_S_merge<CThingCompare_Nearest>`.
- 0099b3c0 / 0099b560 / 0099b860 (labelled CCharString AllocStringData / MakeOwnCopyOfData / UnassignString) are CWideString::AllocStringData / MakeOwnCopyOfData / operator=(wchar_t).
- Callee labels seen wrong: 0x0044FC00 "CPersistTraits<float>::TransferBinaryIn" (a CDefString read), 0x00993EB0 "WriteCBYTE" (a long write), 0x005BD4CE "CTCWeapon::IsAnalogueLoading" (a CTCInventoryBase def flag), 0x005C2AA9 "CTCInventoryStats::OnSerialise" (vector<CItem>::resize), 0x00F35B10 "CWorldMap::AllocateMapThingUID" and 0x00F35B30 "CCreatureAction_TrollWhackGroundBase::Initialise" (entity-script filter registration / IsDone), 0x00487DC0 / 0x00449970 / 0x0099A330 (CPlayer::GetPlayerCreature / CPlayerManager::GetMainPlayer / IsInitialised).
