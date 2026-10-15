# Hand chunk 126 (CCombatManager .. CCrateTeamManager): 36 of 40 byte-exact

## What matched (36)
- **CCombatSequence_* OnCreate family** (00906db0, 00906d20, 00907ae0, 00907b60, 00907930, 0090a550, 00909140): the landed
  TrollRockPursue counted-pointer shape (`CCountedPointer action; action = new X(...); AddAction(action); Base::OnCreate();`).
  BanditKing rows: the action is a thin derived class whose INLINE constructor calls the out-of-line base ctor then stores its
  own vtable (new block held in edi). WillForcePush: `CCombatActionBase(owner, 150, CSoundPair())` inlined + `m_bPushInto = true`,
  one zero register (ebx) for all zero stores. TrollOpeningGroundPound: a leading body store `m_28 = 0`. TrollBurrowPursue x2 /
  WillLightning: reading the owner through an inline `GetOwner() const` accessor fixes the AddAction/delete argument colouring
  (direct member read gives edx/eax, retail eax/ecx).
- **0069e3d0** (label "CCombatSequence_WillLightning::OnCreate", really a camera-manager helper): new camera mode, counted
  pointer, passed BY VALUE to SetCameraMode. Retail caches the count block in esi across the call -- only reproduced when
  `Reset` (0x0069C2F0, landed body) is defined EARLIER in the TU under `#pragma auto_inline(off)`.
- **00a17480** (label "CCombatSequence_WillForcePush::OnCreate", really STLport `list<SEntry>::insert(where)` =
  `insert(where, SEntry())`): malloc'd node, placement copy (out-of-line T copy ctor), link before `where`, hidden iterator.
- **CCombatant** 00c41990 / 00c41bb0: bit-19 `(unsigned char)(flags >> 19) & 1` gate, if/else clamp into a float local (moved
  through integer registers), `pow` intrinsic result through a named float local.
- **CCombatantInfo** 006f4990: memberwise copy ctor; retail 12-byte vector puts DefenderInfo at +0x34.
- **CCompare_FurthestFromInitPos2D::Median** 00748760: /Gr `__fastcall` SGI `__median` over CThing pointers, 4-byte stateful comparator by value.
- **CComponent** 0052e9c0 (TargetPosition copy written in each arm: X store hoisted, Y per arm), 005303f0 (`this` passed as
  the CObserver base at +4 = null-tested conversion), 005304c0 / 00530530 / 00530610 (retail vector via the iterator-object
  `*(begin()+i)`; MATCH).
- **CComponentContainer::AddTextLine** 00546140: GetInstance first, counted font-bank copy lives in registers, CRGBColour by value
  via its inline `Set(r,g,b,a)` copy ctor (D3DCOLOR order), CCharString by-value parameter destroyed by the callee.
- **00cafce0** (label "CCompressedMemoryDataInputStream" ctor, really DirectShow `CRendererPosPassThru(name, pUnk, phr, pPin)`):
  AM_NOVTABLE CMediaPosition + CBaseDispatch member (+0x14 zero before the CPosPassThru vtables), E_POINTER on a null pin,
  CCritSec member, two REFERENCE_TIMEs at +0x38/+0x40, m_bReset = TRUE.
- **CCompressorZlib** 00a3a840 DecompressStream, 00a3a8c0 FinishCompressStream (finished stored twice), 00a3ab00
  InternalDecompress (static inflate state global), 00a3ac40 Compress (info written from values, compressed size first on
  the header-only path; `size <= 0` for `test ; ja`), 00a3add0 InternalCompress(src, size, CArray&) (`__aulldiv` 64-bit bound,
  `size ? first : this` data accessor). 0086c620 (label "SetupStream", really STLport `__linear_insert` for {long key; 12-byte POD}).
- **CConfigOptionsDefaultsDef::operator=** 00625350: explicit memberwise copy (DDK out-lines the implicit one).
- **CConsoleCommandParameters::GetParameterTypeAsString** 009ea740: dense switch with jump table; case bodies in source order bool, slong, ulong, float, string, default.
- **NUISystem::CControllerDisconnect::ChangeState** 0042d6b2: `optimize("s")`; `AddInputHandler(*this)` (reference, no null test) vs `RemoveInputHandler(this)` (pointer, `neg ; sbb ; and`).
- **CCombatManager** 006ea050 (two finds through an inline helper taking the u64 key BY VALUE), 006eabb0, 006ed9e0
  (`__forceinline` GetInterface template; plain inline was emitted out of line). **CCombatRing** 006e8b60 (per-prong
  `POccupier = CIntelligentPointer(0)`), 006ebee0 (unsheathe call duplicated per arm).
- **CCounter::operator=(float)** 00d90550: clamp tests the argument but stores the local copy.
- **~CCrateTeamManager** 00cdb3c0: non-polymorphic class; inline ~CScriptThing (vtable store, PImp release, CBase dtor) x2, then ~CTimer releasing the index through thing-manager slot 88.

## Parked for Fable 5.1 (4)
- 00531000 CComponent::FindAllChildrenOfType DIFFER(139v139): retail `push ebx ; push ebp ; push esi` in the prologue, lane pushes ebp only in the loop arm and aligns the loop with `lea ebx,[ebx]` instead of `jmp ; lea ecx,[ecx]` | tried: if/else, early return, reversed arms, function-scope index | next: lane gap: callee-saved register shrink-wrapping
- 00994a80 CCompressedMemoryDataInputStream::SetPosition DIFFER(138v141): position/chunk in edi/ebp (retail) vs ebp/edi (lane); `mov edx,[14h] ; lea eax,[edx+ecx]` vs `mov eax,[14h] ; add eax,edx`; pointer advance `ptr + (pos - StreamPos)` vs `ptr - StreamPos + pos` | tried: plain body, locals in store order, declaration order swap, inlined InternalReadUnbuffered (fixed the push order) | next: an inline SetPositionAssumeWithinSourceChunk-style helper taking the chunk end, or lane gap: register colouring
- 0076aae0 CCompressorZlib::SetupStream (STLport __linear_insert) DIFFER(105v105): by-value copy for __unguarded_linear_insert stores the key `[eax]=edi` first in retail, last in the lane | tried: init-list, body-assignment, split copy ctors, key+POD12 member (gives the 0086c620 shape) | next: lane gap: scheduling
- 006e5790 CConversation::AddPersonToConversation DIFFER(94v100): retail keeps `person` in ebx, stores it back into its own argument slot for find's const-ref value and homes the iterator-tag temporary in that same slot; lane binds the slot directly and gives the tag its own frame byte (`sub esp,0Ch`, `lea eax,[esp+0Bh]`) | tried: STLport __find(first, end, person, random_access_iterator_tag()) with end in a local | next: lane gap: tag temporary over a live parameter slot (GOTCHAS 24l)

## Skipped
- none (no row spans two functions).

## Label corrections
- 0069e3d0 "CCombatSequence_WillLightning::OnCreate" -> a CGameCameraManager helper (calls SetCameraMode 0x00697E20 on `this`; not in any vtable).
- 00a17480 "CCombatSequence_WillForcePush::OnCreate" -> STLport `list<SEntry>::insert(iterator)` (`ret 8`, hidden iterator).
- 00cafce0 "CCompressedMemoryDataInputStream::CCompressedMemoryDataInputStream" -> DirectShow `CRendererPosPassThru::CRendererPosPassThru` (no-RTTI vtables 0x012BF9F0/0x012BF9A8/0x012BF994; base CPosPassThru 0x012BF940/F8F8/F8E4).
- 0076aae0, 0086c620 "CCompressorZlib::SetupStream" -> STLport `__linear_insert` (/Gr) for 16-byte records.
- 00a3add0 "CCompressorZlib::InternalCompress" is the CArray overload; the real 4-argument InternalCompress is 0x00A3AA40.
- Callees: 0x00908160 (CCreatureAction_BattleChargeTurnLeft), 0x00908000 (CCombatAction_StabThing), 0x008B0650 (CXMVPlaybackInfo = base OnCreate), 0x008B0B10 (std::vector::push_back = AddAction), 0x009CCDC0 (CQuestManager::OnQuestStartScreenDisplayed = font-bank GetFont), 0x006E8400 (CAIStateGroup_MeleeCombatNearbyBase::UpdateCurrentAction, called on CCombatManager), 0x00662410/0x00662430 (C3DAnimationBlendState::AddCombinationBlend = creature combat-state bool tests) are propagated labels.
- 00625350: the Ego_r CConfigOptionsDefaultsDef layout (floats from +0x58) does not fit the copy; the def is modelled by its bytes (label may be propagated).
