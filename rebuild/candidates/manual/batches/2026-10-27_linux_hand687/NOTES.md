# Hand chunk 687 (ten never-attempted rows, 520-672 bytes, all `_global::` stand-in labels)

## What matched (7 of 10)
| address | grade | bytes | identity | evidence |
|---|---|---|---|---|
| 0x00C04020 | MATCH | 563 | zlib 1.2.1 trees.c `gen_bitlen` (static) | vendor text verbatim; landed trees.c TU (pqdownheap 0x00C03F50, build_tree 0x00C052C0) |
| 0x00CB09A0 | RELOCATION_MATCH | 520 | DirectShow strmbase wxutil.cpp `WaitDispatchingMessages` | L"AXWaitDispatchingMessages" 0x012BFA68; caller CBaseRenderer::GetState 0x00CA6620 |
| 0x00C68D70 | RELOCATION_MATCH | 542 | `CEaxContext::SetReverbAllParameters(unsigned long, const EAXREVERBPROPERTIES*)` | EAXREVERB_ALLPARAMETERS arm of 0x00C68F90; twin of landed SetAllParameters 0x00C67AE0; EAX 4.0 SDK field order |
| 0x00CA2480 | RELOCATION_MATCH | 542 | libvorbis 1.0.1 floor1.c `inspect_error` (static) | vendor text verbatim; vorbis_dBquant 0x00CA1E50; caller floor1_fit 0x00CA26C0 |
| 0x00A3EF00 | RELOCATION_MATCH | 561 | `CSoundManagerLH::CSoundManagerLH(const CSoundManagerInit&, bool)` | RTTI vtable 0x0129D2C4; base ctor 0x00991910; dtor 0x00A3EDE0 layout; caller 0x009A6610 |
| 0x00A0F4B0 | RELOCATION_MATCH (true extent) | 1462 | `std::deque<C2DVector>::_M_insert_aux(iterator, unsigned int, const C2DVector&)` | STLport 4.6 _deque.c; landed 0x00A0FAA0 deque TU. **EXTENT FIX** below |
| 0x00ACA5F0 | RELOCATION_MATCH | 570 | `QuadricOptimiserInternals::COptimisedVertex::LimitGroupCount(unsigned long)` (/Gr) | PDB symbol; COptimisedVertex::Groups map at +0x9C; CGroup::CompareWeight 0x00AC6B40 |

Tests: `test/<addr>.cpp` for all seven, PASS under g++ (tall.sh) and VC7.1 (tall.py, log tmp/tall_vc.log).

### EXTENT FIX: 0x00A0F4B0
The catalog carries three rows for one function: 0x00A0F4B0 (570), 0x00A0F6EA and 0x00A0F98B. The true
body is 0x00A0F4B0..0x00A0FA66 (1462 bytes, one epilogue, STLport `_M_insert_aux` with both the
front-half and back-half branches). The source matches the full extent (`tmp/00a0f4b0_oracle_full.tsv`
via `bin/qo.py`); against the chunk oracle gall/vland report DIFFER(1462v570) which is the extent, not
the code. Coordinator: merge the three rows into one 1462-byte row and re-grade.

## Parked (3), all with >=12 probes and a documented residue in differ/<addr>.cpp line 2
- 0x00C61D10 `CAmbientEvent::Trigger(CAmbientEmitter*)` DIFFER(579v581), 31 probes + flag sweep.
  Residue: the Play call's three argument pushes are interleaved with the height x87 chain in retail; VC
  from my body emits the pushes after the chain. Everything else (array probability test, scene
  lookup, origin add, jitter, CPKVoiceParams setters, volume compare) is byte-equal.
- 0x00C733D0 `CAudioSource::ApplySoftware(CAudioObject*, unsigned long)` DIFFER(610v610), 13 probes.
  Residue: register choice only (retail keeps `flags` in ebx and re-reads `sample` from its slot; mine
  the reverse). Same instruction stream.
- 0x00C73AF0 `CreateDecoderStream_00c73af0(IStreamSource_00c62920*, const unsigned short*, const SStreamSegment&)`
  (__fastcall, /EHsc /GS) DIFFER(671v672), 13 probes + flag sweep. Residue: block layout only (retail
  places the epilogue after the type-0 arm; mine at the end).

## Skipped
None.

## Label corrections (all ten catalog labels are stand-ins; see NAME_FIXES.tsv)
00c04020 keeps gen_bitlen; the other nine labels are propagated/descriptive and wrong (steering, lights,
spline, shader, mouse cursor, hero experience ...). Corrected spellings in NAME_FIXES.tsv.

## Header debt
- `CSoundManagerLH` / `CSoundManagerInit` / `CSoundSystemConfig` modelled locally (libpk has no PDB types);
  engine header exists for CSoundManagerLH only partially (vtable+base), offsets documented in src.
- `COptimisedVertex` used from PDB layout (Groups at +0x9C); `CGroup` {unsigned long BoneIndex; float BoneWeight}.
- libpk audio (CPKArray, CPKVoiceParams, CAmbientScene/Zone/Emitter, CAudioContext) are local models
  consistent with the landed siblings; coordinator should consolidate under one header set.
- Vtable 0x012B1E24 is named both CSoundSystem and CAudioOwner in landed code; one name needed.

## Callee names (proven, used in sources)
See NAME_FIXES.tsv; key: 0x00C096E0 libpk sound-system factory (__stdcall), 0x00C08CF0/D00/D10 allocator
setters, 0x00C08800 CSoundSystemConfig ctor, 0x00C087C0 SetChannel, 0x00AAC530 CSoundEmitterLH ctor,
0x00AD14E0 _Rb_tree::_M_erase, 0x00AD6420 map::operator[], 0x00AD4E20 vector<CGroup>::_M_insert_overflow,
0x00AD7550 std::sort<CGroup*,...>, 0x00C61BC0/0x00C61C60/0x00C61C00 CPKArray, 0x00C10A30 CPKVoiceParams,
0x00C20480/A0/20 float setters, 0x00C13140 CPKVoiceTable::Play, 0x00C73A00 ClassifyAudioExtension.
