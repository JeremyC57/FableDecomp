# Hand chunk 631 (CTCPhysicsControlled .. CWeaponDrawState/CVirtualPlayer): 3 of 10 byte-exact

All ten rows were parked same-length retry rows (CHUNK_BRIEF.md). Each best attempt was reproduced first, then re-tested
with new levers. All three matches are RELOCATION_MATCH (no MATCH, no retail_asm). vland: WINS 3 (one RELOC_ALIAS warning:
`__CIpow` -> the import thunk 0x00BFEB40, the same as earlier landings).

## What matched (3)
- **C2DLineF::GetInfiniteLineLineIntersection 0x00A57960** (PDB `?GetInfiniteLineLineIntersection@C2DLineF@@QBE_NABV1@AAVC2DVector@@@Z`):
  intersects two infinite lines by the determinant form and returns false when they are parallel. The fix was to copy the
  four end points into `C2DVector` locals that have a memberwise user copy ctor. VC7.1 splits each copy into scalars, and the
  two that spill (p2.Y, p4.X) keep homes inside 8-byte struct slots. That gives retail's 0x20 frame with gaps at +0x14/+0x18.
  Plain float locals pack to 0x14, and an implicit copy moves all words. Then the cross terms are written `x2 * y1` / `x4 * y3`
  (VC loads the right operand first).
- **CCodaVoice::SetVibrato 0x00C59C60** (catalog `GraphicalElement_ComputeAnimationTransform`, propagated): sets the vibrato
  half-period from the rate and the per-block pitch factors 2^(+-2*depth/period). The name and class come from the landed
  caller CCodaVoice::StartVibrato 0x00C59FC0. The fix was to compute the two factors as `pow(2.0f, x)` through <math.h>'s C++
  `float pow(float, float)` overload. The inline float-returning overload lets VC7.1 put `pop edi` between the second _CIpow
  call and its `fstp`. A direct `(float)pow(2.0, ...)` or a hand-written inline float helper stores first.
- **CDSoundStreamBuffer::CreateLoopingBuffer 0x00C56D70** (descriptive method; catalog `_gapscan::sub_00c56d70`): creates the
  stream's DirectSound buffer, switches 3D off, mutes EAX direct and plays looping.
  - Class: slot 4 of the ctor's vtable 0x012B8D98, next to the landed CreateAndPlay 0x00C57380 (slot 3). The callee
    0x00C57140 uses the bound spelling `CreateBuffer(unsigned long, unsigned long, bool)`.
  - Fix: write the Play test as one more early return (`if (Play() != 0) return false; return true;`). The shared false
    block then stays right after the first test, and VC still folds the tail to `neg ; sbb al,al ; inc al`.

## Parked for Fable 5.1 (7)
- 00725b70 CTCPhysicsControlled::UpdateAdditionalImpulse DIFFER(201v201): `sub esp,28h` v `24h`; retail area box +0x24 and first for_each result temp +0x10 (over the dead vector), ours box +0x1c, temp on the box | tried: same-TU noinline GetAllThingsInRoughArea stand-in (fixed 205->201: start now cached in edi), box at function scope (0x34), box declared before the vector, area temp in the call (197), begin()/end() inline (same) | next: make VC7.1 see the vector slot dead after begin()/end() (retail reuses it), e.g. a stand-in body shape or vector model that ends the vector's lexical slot reservation
- 00aeb600 NParticleEngine::CParticleEmitterManager::CreateInterpolatedEmitter DIFFER(202v202): `mov edx,[esp+5Ch] ; mov ecx,[esp+14h] ; push edx ; ... ; mov esi,ecx` v `mov ecx,[esp+5Ch] ; mov esi,[esp+14h] ; push ecx ; ... ; mov ecx,esi` | tried: PDB respell (const templates, const ShouldSpawn/DatabaseClone, InterpolateTo(const&, float)), shared CCountedPointer header, result assigned directly, float local for interpolate, virtual ~CBaseClass | next: lane gap: register preference of the reloaded counted-pointer Data
- 00943c20 CAIStateGroup_WorkInWarehouse::IsRunnable DIFFER(206v206): first block `mov ecx,[eax+18h] ; test byte ptr [ecx+24h],8` v retail eax chain; second block edx/eax/eax v ecx/eax/edx | tried: PDB respell (PTask CIntelligentPointer<CVillageTaskCarryCrate>, GetBestCrateTaskForThing(const CThing&), template IsInWorkHours const, CTCVillageMember::GetPVillage), GetCreature() accessor (1 or 3 reads), named creature local, const GetInterface, early return (209), `!= 0` HasTC (211) | next: lane gap: scratch register colouring
- 008f99a0 LHNetworkLib::LSocket::Receive(vector<char>&) DIFFER(207v207): retail pushes `""` before the end-of-storage store and sinks start/finish below the second push | tried: (earlier 5 forms) + user ~LAddress, named block pointer in the default ctor | next: lane gap: store scheduling (scheduling-only)
- 009bbbf0 CPersistContext::CheckUnknownSymbols DIFFER(207v207): frame 0x38 v 0x40 (retail +0x10..+0x17 unused between the message temporaries and the spare string; every lea shifts) | tried: PersistError respell, std::list comment member, named item local (211), for loop (same), earlier: second unused string, named message | next: find the two 4-byte slots retail allocates (a dead address-taken pair declared after the spare string)
- 00a5d000 CMatrix4x4::SetInverseOfUniformScaleRotationTranslation DIFFER(211v211): `xor eax,eax` after the row-3 `fchs` v right after the last `fld [eax+0Ch]` | tried: k = 0..7 unused locals (inert, no calls), raw float pointer forms (s[3] for the old translation x fixed both operand orders), chained zero stores, separate row-3 local, -(d + t) | next: lane gap: zero-register scheduling (scheduling-only)
- 00c351b0 CVirtualPlayer::Start DIFFER(211v211): `mov edi,eax ; mov [esi+14h],edi` / ebx=&source->Info v retail edi=&Info, `mov [esi+14h],eax ; mov ebx,eax` | tried: inherited sample-description base, named info pointer, separate member stores (214), a length local, `Length - (loop + intro)` | next: lane gap: callee-saved assignment order

## Skipped
None (no row holds two functions).

## Label corrections
- 0x00C59C60 `GraphicalElement_ComputeAnimationTransform` -> CCodaVoice::SetVibrato (GameCODA voice; descriptive class, sibling-established).
- 0x00C56D70 `_gapscan::sub_00c56d70` -> CDSoundStreamBuffer::CreateLoopingBuffer (descriptive method; class from the vtable 0x012B8D98 siblings).
- 0x008F99A0 `LSocket_ReceiveWithTimeout` -> `?Receive@LSocket@LHNetworkLib@@QAE_NAAV?$vector@DV?$allocator@D@std@@@std@@@Z`.
  Proof: Ego_r.pdb has only two one-vector LSocket members, Receive and ReceiveBlock. This body only forwards two default
  LAddress locals to the landed 3-argument Receive 0x008F9320, so it is the one-argument Receive.
- 0x00C351B0 `CWeaponDrawState::Initialize` is propagated. The landed CSoundEvent callers (0x00C208BF / 0x00C20989 /
  0x00C20B2E, `this` = event +0x78) bind it as `?Start@CVirtualPlayer@@QAEXJPAUSEventLevel@@M@Z` (descriptive). The parked
  file uses CVirtualPlayer::Start, but with pointer parameter types. The bound `long` first parameter is wrong: the body
  dereferences it (+8 sample description), and `level` +0/+4 are read as a start fraction and a loop handle.
- 0x00725B70: the two functors were swapped in the earlier attempt. The Ego_r.pdb layouts show that 0x00726610 copies the
  0x14-byte CApplyAdditionalPlayerImpulseIfTooClose and 0x00726660 copies the 4-byte
  CApplyAdditionalPlayerImpulseToPlayerIfInsideUnpushableCreature.
- 0x00943C20: Ego_r.pdb gives CAIStateGroup_WorkInWarehouse one member, `CIntelligentPointer<CVillageTaskCarryCrate> PTask`
  (+0x1c). The callee 0x00658EA0 is CTCVillage::GetBestCrateTaskForThing(const CThing&).
- More callee spellings proven wrong: NAME_FIXES.tsv (17 rows).

## Names: proven vs descriptive
- Proven from the PDB: C2DLineF::GetInfiniteLineLineIntersection, LSocket::Receive, CPersistContext::CheckUnknownSymbols /
  PersistError, CMatrix4x4::SetInverseOfUniformScaleRotationTranslation, CTCPhysicsControlled::UpdateAdditionalImpulse and its
  callees, CParticleEmitterManager::CreateInterpolatedEmitter and its callees, CAIStateGroup_WorkInWarehouse::IsRunnable and
  its callees.
- Descriptive, from established siblings (libpk / GameCODA have no PDB): CCodaVoice::SetVibrato, CDSoundStreamBuffer::CreateLoopingBuffer, CVirtualPlayer::Start.

## Header debt
None of these classes has an engine header (CMatrix4x4, C2DLineF, CWeaponDrawState, CPersistContext, CTCPhysicsControlled,
CParticleEmitterManager, CAIStateGroup_WorkInWarehouse), so every file uses a local model. 00aeb600 uses the shared
engine/CCountedPointer.h.
