# Chunk 596 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- A same-TU `static __declspec(noinline)` libvorbis helper gets retail's private register convention from VC7.1 with
  no /GL. floor1_inverse2 0x00CA3420 matched on the first grade calling render_line as y1-in-ecx plus four pushes.
  hand448 parked render_line0 0x00CA2010 as an "LTCG custom regs" lane gap after compiling it as a non-static
  `__fastcall`; retest that row and render_line 0x00CA1E80 as statics with a caller in the TU.
- libvorbis info.c statics match as real 1.0.1 bodies in `extern "C"`, `static __declspec(noinline)`, with `__stdcall`
  on the callee-pop ones, under `/Gr /GS` (0x00C8E580, first grade).
- A struct copy of a 3-float C3DVector inside an STLport `_M_fill_insert` is `rep movsd` unless C3DVector has a user
  memberwise copy constructor; with one it is dword moves (0x00B8F4E0).
- strmbase rows: define the inline helper the SDK defines in the same .cpp (CBaseFilter::NotifyEvent) in the TU so it
  inlines; the landed ReceiveMultiple model plus strmif.h interface order is enough for Receive (0x00CA8B90).
- x87 scheduling: `float ramp = (float)u; float fade = ramp * k;` places the fmul after the next integer compare. The
  inline `(float)u * k` emits it straight after the 2^32 correction (0x00C75820).
- Two chunk lengths both initialised before their wrap tests (`n1 = r; n = r; if (...) n1 = ...; if (...) n = ...;`)
  gives retail's `mov ecx,ebp ; mov esi,ebp` pair (0x00C756C0).
- The libpk assert macro (CDebugStream log line in its own scope, then the CDebugBreak temporary, /EHsc) is reusable
  for any `.\File.cpp(line) : text` assertion (0x00C62A50).
- Parked: GFInterpolateArray 0x00B22F30, register allocation around the inline-asm fistp helper (best 354v340 after 4
  grades and ~25 sweep spellings).

Review at landing:
- 0x00C8E580: the TU carries four static bodies (_v_readstring, _vorbis_unpack_info/_comment/_books). Land the leaf only.
- 0x00CA3420: the TU carries the static render_line body (0x00CA1E80). Land the leaf only; FLOOR1_fromdB_LOOKUP is
  declared extern as `_FLOOR1_fromdB_LOOKUP` (0x013ABE28, new binding).
- 0x00CA8B90: the TU carries the CBaseFilter::NotifyEvent body (0x00CA7930). Land the leaf only.
- 0x00C756C0 / 0x00C75820 retype the history parameter as CRingBuffer and make UpdateGainRamped's params non-const
  (NAME_FIXES.tsv). Respell CReverbDiffuser::Process 0x00C75A20 and ProcessRamped 0x00C75AB0. The stage callees
  CReverbDiffuserStage::Process 0x00C891A0 / ProcessRamped 0x00C89350 take the same object as `const SDiffuserParams*
  target`; that is probably also the ring, but it was not checked here.
- CDspGain::Apply 0x00C74CE0 has two landed spellings. Its definition is `...PBMPAMHI@Z` (int mode, unsigned count),
  which this chunk uses; ProcessInput 0x00C69C60 binds `...PBMPAM_NK@Z`. ApplyRamps 0x00C74510 reads mode as a byte
  (`mov al,[esp+30h]`), so bool may be right. Pick one spelling for both.
- 0x00C74D30: the definition is Crossfade (used here); a caller binds `?Blend@CDspGain@@...0PBMPAM_NKK@Z`.
- 0x0099EAE0 has about 100 bindings. Three are `??1CCharString@@QAE@XZ`, which 0x00AE3670 uses; the rest look like
  ICF-folded or stand-in names. Not listed in NAME_FIXES.
- The landed CParticleProgram ??_G 0x00AE8520 calls the stand-in `@CParticleProgram_dtor@4`. Respell it to
  `??1CParticleProgram@@UAE@XZ`.
- Header debt as listed in NOTES.md.
- 0x00B22F30 is a retail_asm candidate if it is ever matched (fistp dword helper evidence is in the file).
- vland (read-only run, 9 wins, no RELOC_IDENTITY) warns SHARED_HEADER_AVAILABLE for CMemoryDataOutputStream /
  CMovableResource (0x00AE3670: the headers are flattened, local models kept, header debt), CEngineOcclusionManager
  (0x00B8F4E0: only the nested COccluderFlareEntry is modelled) and CBaseObject (0x00CA8B90: the landed strmbase
  ReceiveMultiple model's local base). RELOC_NAME warnings: CDebugStream appenders/dtor vs labels CString::operator<< /
  ~CString (the landed spellings were used), and the two vorbis statics vs C3DGenerator_* labels (labels wrong).
