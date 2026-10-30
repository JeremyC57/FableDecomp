# Hand chunk 595 (CImaAdpcmCodec .. PKCTRRedblackTree): 7 of 10 byte-exact

All seven are RELOCATION_MATCH (no plain MATCH: every row has relocated calls or data); no retail_asm rows. Tests pass under
g++ and VC7.1 (tall.sh + tall.py). Read-only vland: 7 WINS, no RELOC_IDENTITY; its RELOC_NAME warnings are the wrong bound
labels listed under NAME_FIXES, and its SHARED_HEADER_AVAILABLE warning on 0x00CAED50 is a name collision (see Header debt).

## What matched (7)
- **CDeferredCommand::Invoke 0x00CAED50** (strmbase ctlutil.cpp): the SDK body, `hr = m_pQueue->Remove(this); m_pQueue = 0;
  return hr;` after ITypeInfo::Invoke. Shape lesson: CBaseDispatch::GetTypeInfo is inlined with its function-local statics
  (`?szRegTypeLib@?N@??GetTypeInfo@CBaseDispatch@@...`), so the TU must carry the landed 0x00CACD90 body, not a declaration.
- **std::vector<CTVertexSeaBackground>::_M_insert_overflow 0x00B80400** (STLport 4.6, `__false_type` overload): the real
  _Vector_base / _STLP_alloc_proxy / malloc allocator model. Shape lesson: `__uninitialized_copy` / `__uninitialized_fill_n`
  must be `__forceinline` (plain inline: 226v333); `_M_clear` 0x00B80550 stays out of line (declared only).
- **GetReadingWindowOrientation 0x00CB3420** (DX SDK ImeUi.cpp static, /GS): the SDK body. Shape lesson: it only matched as a
  TU-local `static __declspec(noinline) __stdcall` with a same-TU caller (a reduced GetReadingString); without both, 330v333.
- **PKCTRRedblackTree::DeleteFixup 0x00C18C00** and **PKCTRRedblackMap::DeleteFixup 0x00C1B560** (libpk): textbook Niemann
  deleteFixup. Shape lesson: for the Tree the RotateLeft/RotateRight bodies sit in the TU under `#pragma auto_inline(off)`
  (VC then knows they keep ecx, `this` stays in ecx); for the Map they are declared only (`this` in edi), exactly as retail.
- **CAudioContext::InitialiseListener 0x00C568F0** (libpk OpenAL/EAX wrapper): DistanceModel(0), Listener3f(AL_POSITION /
  AL_VELOCITY, 0,0,0), EAXSet ROOM -10000 and ROOMROLLOFFFACTOR 0 (both DEFERRED). The three wrappers are defined in the TU
  and inline, like the landed family.
- **CSoundSystem_00c131c0::PlaySoundList 0x00C131C0** (libpk audio, /EHsc): under the global PKCProtectionInstance lock start
  each valid list entry; consecutive started sounds are chained into one group; return the group id, the lone sound's id
  or -1. Shape lessons: `CSoundGroup* g = pool.Allocate(); g->Init(this, m_nextGroupId++, prev);` (a local, so Allocate is
  called before the arguments are evaluated; the chained `pool.Allocate()->Init(...)` evaluates the arguments first); and
  the tail as `if (!g) return a; return b;` (two returns) puts the EH state `-1` store before the branch where the ternary
  form put it after (332v332 -> RELOCATION_MATCH). 3 grades.

## Parked for Fable 5.1 (3)
- 0x00C5F6A0 CImaAdpcmCodec::DecodeM16 DIFFER(327v327): block head only: retail reads the dword header first, keeps the cBlocks-- temp in ebx and emits `lea edx,[esi+4]` before the `jl`; ours keeps the temp in edi and the lea after the step-index check | tried: narrow header loads (314), return 0 (311/316), pbBlock+= placements (316/318), declaration orders, step-size temporaries, one dword header local (best, a12) | next: the imaadpcm.cpp sample's exact block loop (`while (cBlocks--)` with the header read as two separate statements before `fSuccess` checks), or a `for` over cBlocks
- 0x00C20580 CSoundPlayParameterOverride::ApplyTo DIFFER(332v330): mask merge register allocation: retail `mov ebx,[esp+8]` (params) first, mask in di, &m_params in ebp, params.GetFadeIn() spilled to the arg slot [esp+0x14]; ours keeps params in edi, &m_params spilled | tried: whole-expression `(unsigned short)` cast (330v330 but zero-extends with `xor ebp,ebp; mov bp,ax`), named unsigned short locals (340), inline MergeMasked(dst, src, mask) helper (best: 32-bit and/not/or like retail) | next: the merge helper with the mask first / taking `const unsigned short&`, or the getters returning `unsigned short` by reference into the block
- 0x00C5E4E0 CUnprovenKsDevice_012B9148::OpenByDirectSoundGuid DIFFER(336v332): hDll in edi / hr in esi (retail esi / edi) and the bool tail `xor ecx,ecx; setne cl; mov al,cl` where retail is `setne al` | tried: if/else with the CreateInstance `!= 0` failure first and a local hDevice (best, k4), other branch orders | next: `unsigned char` return type (bool tail), or the device search as an inline helper

## Skipped
None.

## Label corrections
- 0x00C5F6A0 Audio_ExpandADPCM_Mono -> CImaAdpcmCodec::DecodeM16 (`?DecodeM16@CImaAdpcmCodec@@SIHPAE0IIIPAH1@Z`; Initialize
  0x00C5F9E0's converter table 0x012B9274 = {DecodeM16 0x00C5F6A0, DecodeS16 0x00C5F7F0}; DX/Xbox SDK imaadpcm.cpp).
- 0x00CAED50 COM_InitializeTypeLibrary -> CDeferredCommand::Invoke.
- 0x00B80400 Vector_PushBack_CActiveFile_3Floats -> std::vector<CTVertexSeaBackground>::_M_insert_overflow.
- 0x00C5E4E0 sub_00c5e4e0 -> CUnprovenKsDevice_012B9148::OpenByDirectSoundGuid (descriptive).
- 0x00C18C00 RedBlackTree_RemoveNode / 0x00C1B560 RedBlackTree_RemoveNodeVariant2 -> PKCTRRedblackTree / PKCTRRedblackMap::DeleteFixup.
- 0x00C568F0 CAudioContext_ConfigureListener -> CAudioContext::InitialiseListener.
- 0x00C20580 CTCHero_ProcessInventoryFlagMask -> CSoundPlayParameterOverride::ApplyTo (descriptive).
- 0x00C131C0 CEnginePrimitive_UpdateParticleTrail -> CSoundSystem_00c131c0::PlaySoundList (descriptive).
- 0x00CB3420 GetReadingWindowOrientation: correct.
- Callees: 0x00CAEC80 CAdviseHolder_00caec80::Unadvise -> CDeferredCommand::Cancel; 0x00B80550 CActiveFile::OnReadFinished /
  ReleaseFirstMember_00b80550 -> vector::_M_clear; 0x00C29F70 CTrailListener_00c29f70::Attach / GetWaterHeightAtPosition,
  0x00C29EC0 CSteeringGroup::AddAgent / CEngineInternalPrimitiveParticleSpriteTrailGroup::Update, 0x00C12CD0
  CombatBlockReward_ProcessWithExceptionHandler, 0x00C21F20 CIsThingAlive::operator(): all propagated labels on libpk audio code.

## Names and evidence
- Proven (two sources): GetReadingWindowOrientation (FableWin.pdb + Ego_r.pdb name sets, SDK strings); CDeferredCommand::Invoke
  (strmbase ctlutil.cpp body + landed CDeferredCommand model / IDeferredCommand vtable 0x012BF8B8); CBaseDispatch::GetTypeInfo
  statics (landed 0x00CACD90); std::vector<CTVertexSeaBackground>::_M_insert_overflow (STLport _vector.c body + Ego_r type
  CTVertexSeaBackground {X,Y,Z} and the 12-byte stride); CImaAdpcmCodec::DecodeM16 (converter table + SDK imaadpcm.cpp
  signature); PKCTRRedblackTree (".\PKCTRRedblackTree.cpp" 0x012B2E98 + landed InsertFixup); PKCTRRedblackMap (landed chunk
  593 spelling); CAudioContext (landed family) ; GUIDs CLSID_DirectSoundPrivate, IID_IClassFactory, IID_IKsPropertySet,
  DSPROPSETID_DirectSoundDevice, DSPROPSETID_EAX30_ListenerProperties (bytes vs SDK headers).
- Descriptive (libpk / private code, no PDB): DeleteFixup (both trees), CAudioContext::InitialiseListener,
  CSoundPlayParameterOverride::ApplyTo + its CSoundPlayParameters getters (mirrors of the landed setters),
  CSoundSystem_00c131c0::PlaySoundList / StartSound, CSoundGroup_00c29ec0::Init / AddMember, CSoundGroupPool_00c21f20::Allocate,
  ISoundList_00c131c0 / ISoundListSource_00c131c0 (slot names IsValid 4, GetCount 5, GetItem 7, GetList 5), CSoundInstance_00c131c0,
  CSoundRequest_00c131c0, CUnprovenKsDevice_012B9148 / CUnprovenKsHandle_012B90D4 / FindKsDeviceCallback / SKsDeviceSearch,
  PKCTRRedblackNode / PKCTRRedblackMapNode, and the reduced GetReadingString stand-in caller in 0x00CB3420's TU.
- Callee spellings reused as landed: ??0/??1PKCProtectionInstance, ?g_lock_0143bb14@@3VPKCCriticalSection@@A,
  ?g_hklCurrent@@3PAUHKL__@@A (as the landed CheckInputLocale; 0x0143E8B8 also carries `_g_hklCurrent`), GetImeId 0x00CB28F0,
  CAudioContext::GetProcAddress 0x00C4D740, CBaseDispatch statics.

## Header debt
- No engine header exists for any class here: strmbase (CDeferredCommand, CBaseDispatch, CUnknown), ImeUi statics, libpk
  (CAudioContext, PKCTRRedblackTree/Map, CSoundPlayParameters, CSoundPlayParameterOverride, the 0x00C131C0 sound classes),
  dsound KS device, CImaAdpcmCodec and CTVertexSeaBackground (Ego_r has it, but no generated header): all are local models.
- vland's SHARED_HEADER_AVAILABLE CBaseObject on 0x00CAED50 is a false positive: rebuild/include/engine/CBaseObject.h is
  Lionhead's CBaseObject (CBaseClass.cpp, vtable 0x0129A7D8, 8 bytes); strmbase's combase.h CBaseObject (release build: only
  the static object count, ctor takes a name) is a different class with the same name. Do not swap the header in.
