# Chunk 994 (retail-asm / volatile verification) -- lessons from the authoring agent (fold into GOTCHAS at landing)

## Inline-asm helper shapes (all proven by byte matches in retail_asm/)
- The engine's ceil/floor helpers are `__forceinline long CeilToLong(float value)` / `FloorToLong` whose WHOLE 10-instruction block is `__asm`
  (`fld value ; fadd/fsub half ; fistp result ; fild result ; fsub/fadd one ; fstp back ; mov eax,value ; cmp eax,back ; jne ; dec/inc result`)
  over locals declared `long result; float one; float half; float back;` with `half = 0.5f; one = 1.0f;` assigned in C++ before the block;
  the result is read back by C++ (0048d940, 0057a60a, 006a68f0, 0078bf00, 00785b40, 00b52990 ...). A C++ `--result` after the block hoists the
  load into a register (`mov eax ; jne ; inc eax`), unlike retail's memory decrement.
- Inline asm does NOT force an ebp frame; the ceil/floor rows' ebp frames come from `#pragma optimize("y", off)`. An asm block with a *label*
  does give an ebp frame (00c5d370 v1 vs. the label-free 00c70a60).
- The bare `fld value ; fistp result` helper (rasm1 family (a)) is `__forceinline long FloatToLong(float value) { long result; __asm fld value __asm fistp result; return result; }`;
  the by-value parameter copy is aliased to a dead argument slot when the caller passes a dead variable (`fstp [esp+14h] ; fld [esp+14h] ; fistp [esp]`).
  The pointer-out form (landed 0xB1B5E0: `__asm fld value __asm mov edx,result __asm fistp dword ptr [edx]`) is a different helper (009fca40 uses it twice).
- libpk's truncation helper is `value = value - 0.5f; __asm fld value __asm fistp result; if (result < 0) ++result;` (00c59b00, 00c6bdc0, 00c6c000, 00c77b30):
  the `fsub 0.5 ; fstp [slot] ; fld [slot] ; fistp ; mov ; test ; jge ; inc` signature.
- libpk's 10^x block is `fld log2of10 ; fmul x ; frndint ; fst whole ; wait ; fld1 ; fscale ; fxch st(1) ; fstp scale ; fld log2of10 ; fmul x ; fsub whole ; f2xm1 ; fadd one ; fmulp st(1),st ; fstp result ; wait`
  over `float scale, result, whole, log2of10 = 3.32192802f, one = 1.0f` (declared in that order to hit [esp+10h]/[esp+0Ch]/[esp+8]/[esp+4]/[esp]); seven
  byte-identical standalone copies (00c53b10 + six) and two inlined uses (MillibelsToAmplitude 00c77700/00c40ee0, COnePoleLowPass::SetLevel 00c6c000).
- `align 16` IS accepted inside `__asm` by VC7.1 and reproduces retail's loop-head padding (`mov edi,edi`, `lea ecx,[ecx]`, and for long pads a
  `jmp` over `lea esp,[esp] ; mov edi,edi`): 00c5d370/3f0/4b0, 00c6b3a0, 00c6d370, 00c6f0f0. Without it the asm label is not aligned.
- The compiler moves C++ statements across `__asm` blocks (a `long count = 8;` before the block was emitted after it), and a C++ variable live
  across an asm block is kept in a callee-saved register (esi) with its `push esi` placed lazily after the first asm statement (00c5d2e0 residue).
- A `/GS` function with an `__declspec(align(16))` local gets the `push ebx ; mov ebx,esp ; push ecx ; and esp,-16 ; add esp,4 ; push ebp` frame and reads its
  arguments through ebx; the asm block names the arguments and the aligned local (`movdqa xmm7, gains`) -- 00c6d370 / 00c6f0f0 (Flags: /GS).
- An `unsigned char` return gives `test eax,eax ; setne al` with no `xor eax,eax` (00c52aa0); the pushfd/popfd EFLAGS.ID probe is `__asm` over a `long supported = 0` local.
- A class returned through the hidden pointer with four byte stores in R,G,B,A order is a constructor `CRGBColour(r,g,b,a) { R = r; G = g; B = b; A = a; }`
  built in the return slot (00a44250); the plain POD was returned in eax instead.
- VC7.1 re-associates `(float)rate * seconds / blockFrames` into `rate / blockFrames * seconds`; naming the product (`float samples = (float)rate * seconds;`)
  keeps `fmul ; fdiv` (00c59b00). `(float)kConst` of a `static const __int64` is NOT folded: it is `mov [esp+4],imm ; mov [esp+8],0 ; fild qword` (00c59b00's 128 default).
- Operand order: a pool constant is always the `fmul` memory operand (`fld x ; fmul [__real]`), a *global variable* is loaded first (`fld [g] ; fmul [x]`)
  whatever the source order -- when retail shows `fld [arg] ; fmul [pool]`, the multiplier is a literal (00c6bdc0: 1/3, 1/6, -90.0f; 00c6c000: 2*pi/44100).
- VC7.1 unrolls a simple `for (i = 0; i < 128; ++i) dst[i] = (float)src[i];` eight times (the x87 fallback of 00c6b3a0); an already 8-way hand-unrolled loop is unrolled again.
- Self-assignments `rec.x = rec.x` and un-hoisted loads around stores are `volatile` members (00c34630: only m_channels and m_sampleRate; the fields whose loads
  retail hoists above earlier stores are NOT volatile).
- Stagger test `(UID + frame) % interval` is a signed `__int64` remainder (__allrem 0x00BFEC30), UID modelled as `__int64` (00785b40).

## Review at landing
- SHARED_HEADER_AVAILABLE warnings (non-strict) on 0050ef30 (CTCPhysicsBase), 0057a60a (CTCBase), 006a68f0 (CThingPhysical), 00766260/00766330 (CWorld),
  00b52990 (CEngineWeatherRenderer): the local models are callee/base stubs; convert to engine headers if strict mode is required (regrade needed).
- 0050ef30's leaf `operator()` matches no symbol name; the verifier graded the only function (WARN, MATCH).
- Twin naming per address: Pow10_<addr> (6 copies of 00c53b10), MillibelsToAmplitude_00c40ee0, AddInt128Block_00c5d3f0/_00c5d4b0 (the __stdcall copies).
- Rows graded more than five times across strategies (all logged in GRADES.tsv): 0050ef30, 005115e0, 00511a10 (shape fixes), 007e2df0 (6), 00c5d370 (8, matched), 00c5d2e0 (8, parked), 00c59b00 (6, matched).
- 00785b40 was first authored with `unsigned __int64` (RELOC_CONFLICT on __aullrem): fixed to `__int64` / __allrem before hand-over.
- The `_asm` spelling of the wxutil.h block was changed to `__asm` so purity.classify sees it (00cb0160).
