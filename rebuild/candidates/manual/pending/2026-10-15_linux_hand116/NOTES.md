# Hand chunk 116 (CViewport .. CWADFile): 37 of 40 byte-exact

All 37 matched sources grade MATCH/RELOCATION_MATCH (`grades.txt`) and their behaviour tests print PASS under g++ (`tall.sh`)
and the lane's VC7.1 cl.exe (`tall.py`). Many catalog labels in this chunk are propagated; the sources use the real or a
descriptive class name with a `// Leaf:` selector (checked against the object's defined symbols).

## What matched (37)
- **CVsyncCallbackTimerInfo (11 catalog rows, 1 real)**:
  - 009f7020 the real CVsyncCallbackTimerInfo ctor (PDB names CPU/GPU time stamps and frame counts): plain initialiser list; each 0.0 double is `fld [pool] ; fstp`.
  - 006815f0 / 00cb12e0 / 00c3b930 / 00c3c8a0 / 00656795 (NPlayerGui CQuestInfoElement, vtable 0x0125A2B4): record constructors; an out-of-order word (+0x0c last) is a body assignment, `ret 4` with an unused slot is an ignored parameter, `mov word ptr` stores are `short` members.
  - 00c39f10 / 00c3a110 / 00c3a310 (CTCTavernGameSpotTheAddition sub-object bases): vtable, then `xor ecx,ecx ; lea edx,[eax+4]` and stores through edx = `memset(m_slots, 0, sizeof m_slots)` followed by a body clear of the trailing word.
  - 00c90f10 `SStringBuffer::Init()` (not a constructor -- eax is the malloc result): `memset(this, 0, sizeof *this)` keeps the zero register private, so the buffer's first byte gets an immediate `mov byte [eax],0`.
  - 00af07a0 `CRightHandedSet::SetDefault()` (not a constructor -- eax is the last copied word): `Up = C3DVector(0,0,1); Forward = C3DVector(0,1,0);` -- the frame temporary's float stores are re-read as dwords for the bitwise copy.
- **CWADFile (6 rows; 1 propagated)**: 00a39b40 bank constructor (initialisers before the vtable store), 00a39b70 user copy constructor with the default base, 00a39c80 Clone = `new CWADFile(*this)` with the copy inlined, 00a39bb0 Read (size = second argument, the third unused), 00a39c40 GetPathname (`Bank->GetEntrySymbol(Index).ToWideString()`, the string temporary homed in the hidden pointer's dead slot). 00748530 is an intelligent-pointer record ctor (thing pointer read before the base ctor = converting constructor argument).
- **CVillageTask* (5 rows)**: 0065ef94 / 0065ee66 constructors under `optimize("s")` (pass-through `push dword ptr [esp+0Ch]`), member intelligent pointers with inline default constructors (base call then vtable store) next to out-of-line converting ones; 00860a00 / 00860e70 `member = CIntelligentPointer(p)` through a frame temporary (inline converting ctor + operator=); 007b85f0 a seat table's `m_occupants[seat] = CIntelligentPointer(p)` -- STLport `operator[]` as `*(begin() + n)` loads the start before GetPItem and scales the index after it.
- **CVillager**: 00ef4950 OnInterrupted -- nested ifs; `Crate.IsCarrying(Me.GetDataHandle())` fetches the crate vtable before the inner call; calls on the script-thing members stay virtual.
- **CVillagerInteractionsDef**: 0045e620 virtual Copy (slot 19, void) under `optimize("s")`, written as an explicit memberwise copy (the implicit operator= is out-lined under /Os and the slot becomes a jmp thunk).
- **CVirtualKeyboard**: 00851700 constructor, plain initialiser list (the argument store is scheduled last).
- **CVolume (6 rows; 3 propagated)**: 00a674b0 GetDimensions (`return C3DVectorI(w, h, d)` -- a value constructor gives the ecx/edx/ecx copy into the hidden result), 00a674e0 GetByteLength (source `bpp * W * H * D` emits `imul D ; imul H ; imul W`), 00a67730 operator= (release/clear, assign, null-tested AddRef), 00c336b0 a render-target owner's SetTarget (the view pointer is one named local), 00c57010 a sound device's ReleaseVolume (dllimport `__stdcall` CoUninitialize), 00ca8660 a sound stream's ReleaseBuffers (`jl` early return of the HRESULT).
- **CVolumeTexture (5 rows; 1 is CVolume's)**: 00a673e0 CVolume copy ctor, 00a67dd0 copy ctor / 00a682a0 operator= (`if (other.m_pTexture) m_pTexture->AddRef();` -- the other's pointer re-read for the test), 00a68060 Uninitialise (system manager in a named local before the chained statistics call), 00a68250 CreateTexture (device local, format local, `>= 0 ? true : false` for `setge al`).
- **CVsyncCallbackTimer**: 00f0d470 a quest-script query record (two CCharString members + three zeroed words).

## Parked for Fable 5.1 (3)
- 00669390 CViewport::ClearHandle DIFFER(58v58): retail `mov ecx,eax ; mov eax,[ecx] ; push esi ; call [eax+0Ch]`, ours `mov ecx,eax ; mov edx,[eax] ; push esi ; call [edx+0Ch]` (vtable register of the slot-3 notify only) | tried: member-only form, handle-pointer local, object local, object + handle locals (best) | next: route the notify through an inline helper so the vtable is read from ecx, else lane gap: register choice
- 00a69ad0 CFrameDataHandle::Clear (catalog CViewport::ClearHandle) DIFFER(70v73): retail re-reads the member for the second null test (`mov eax,[esi+4] ; test eax,eax ; je ; cmp [eax+4],2`) while the Close call reuses the first load in ecx; ours tests the cached ecx | tried: `m_p && count == 2`, inline bool predicate, ternary GetRefCount accessor, object local + accessor (best) | next: a volatile-cast read for the second test only, else lane gap: QFE CSE of a member re-read
- 007fe670 CVillageDef::Validate DIFFER(63v63): retail `lea ecx,[esp+10h] ; push ecx ; mov ecx,[esi+28h]`, ours `mov ecx,[esi+28h] ; lea edx,[esp+10h] ; push edx` (begin load scheduled before the value address) | tried: one-expression find, named `last` local (best), raw member operands, optimize("s") | next: begin() returning an iterator object, else lane gap: fastcall register scheduling

## Skipped
- none (no row holds two functions).

## Label corrections
- 00c39f10, 00c3a110, 00c3a310 "CVsyncCallbackTimerInfo" -> inline base ctors of three CTCTavernGameSpotTheAddition sub-objects (vtables 0x012B5494 / 0x012B54A4 / 0x012B54B4, no RTTI).
- 00656795 "CVsyncCallbackTimerInfo" -> NPlayerGui::CDrawQuestInfo::CQuestInfoElement::CQuestInfoElement(long) (vtable 0x0125A2B4).
- 00af07a0 "CVsyncCallbackTimerInfo" -> CRightHandedSet::SetDefault() (void; called on CParticleEmitter's orientation interpolator).
- 00c90f10 "CVsyncCallbackTimerInfo" -> a string-buffer Init() (returns the buffer, not `this`).
- 006815f0, 00cb12e0, 00c3b930, 00c3c8a0 "CVsyncCallbackTimerInfo" -> unrelated record constructors (persist list element, file-archive record, loading-frame record, SExpSpendUndo/TeleportData member).
- 00f0d470 "CVsyncCallbackTimer" -> a CAT_ConvoyMember::Main stack record (the PDB CVsyncCallbackTimer starts with a device pointer).
- 00748530 "CWADFile::CWADFile" -> intelligent-pointer record ctor.
- 007b85f0 "CVillageTaskCarryCrate::AssignCreatureToTask" -> a table's SetOccupant(seat, thing) (callers CAIStateGroup_ServeFood::Update, CAIStateGroup_EatAtTable::OnReplace).
- 00a673e0 "CVolumeTexture::CVolumeTexture" -> CVolume::CVolume(const CVolume&) (stores the CVolume vtable 0x0129C840).
- 00c336b0, 00ca8660 "CVolume::operator=", 00c57010 "CVolume::ReleaseVolume" -> other classes (render-target owner / sound stream / sound device).
- 00a69ad0 "CViewport::ClearHandle" -> a frame-data handle's Clear() (CFrameData callers).
- Callee labels: 0x00A0AA70 "$E2" is the render statistics' texture-removal method; 0x0099EAE0 "_Cons_val<...>" is ~CCharString; 0x00F35B30 "CCreatureAction_TrollWhackGroundBase::Initialise" is a script-base bool query; 0x0065D9EE "std::linear_search<EWeaponClass>" is SGI find (random-access, /Gr).
