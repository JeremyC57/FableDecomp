# Chunk 597 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- A TU-local helper retail calls with its hidden result pointer in esi, its float argument on the stack and `ret 4`
  is a same-TU `static __declspec(noinline) __stdcall` (or __fastcall) returning the class by value: VC7.1 picks the esi
  convention itself without /GL (0x00C62370, called from 0x00C62530). A plain cdecl static gets `ret` + caller `add esp,4`.
- Retail keeping an x87 value on the FPU stack across a call (theta in st0 across `call 0x00C62350`) means the callee
  is defined EARLIER in the same TU -- give it its body (noinline) above the caller (0x00C62370).
- A named local returned by value (`C3DVector point(x, y, z); return point;`) gives retail's frame temporary + dword copy
  through ecx/edx into the hidden result; `return C3DVector(...)` builds straight into it (0x00C62370).
- `if (x <= 0.0f) return;` (or `if (!(x <= 0.0f))`) gives `fcomp ; test ah,41h ; jnp`; `if (x > 0.0f)` gives `jne`
  (0x00C62530).
- The same-TU static convention also covers a pointer + int: zlib longest_match gets s in esi and cur_match in eax
  from a reduced same-TU caller (0x00C00E60).
- An over-span catalog row (the row ends at the first `ret`, catch funclets and cold blocks follow) grades DIFFER on
  the extent alone: grade over the true end with an ad-hoc span (qx.py) before parking (0x00C298D0: 352 vs 542).
- Dinkumware `_Insert_n`: `_Xlen` defined in the TU with an always-throwing body (VC7.1 treats it as noreturn) and
  `operator delete(void*) throw()` remove the trailing residue and the state -1 store after the try (0x00C298D0).
- DXUT `bool bCheck` (not BOOL) keeps the check flag out of eax in ImeUi_ToggleLanguageBar (0x00CB3570).
- A `C3DVector` Dot as a const member returning `const float` gives retail's Y, X, Z term order where every association
  of a free Dot gives X, Y, Z (0x00CBF13C); a 2D direction is `v = a - b; v.Z = 0.0f; v.Normalise();` with Normalise an
  inline `if (GetSquaredMagnitude() > eps * eps) AccurateNormalise();` (the dead Z subtraction folds away).
- A float local declared before a class local with a ctor call gets its store scheduled before the ctor call (bestDot
  -2.0f before `CScriptThing best`, 0x00CBF13C).
- CCharString inline accessors written null-arm-first (`PStringData == 0 ? 0 : ...`, `== 0 ? "" : ...`) give retail's
  jne order and the preloaded "" (`mov ecx,offset "" ; je ; mov ecx,[eax]`); the `PStringData ? ... : 0` form adds a
  jmp/xor (0x00CD5170).
- Two different "" addresses in one function (0x0122D70E the literal, 0x0129AAF4 the inline accessor's) cannot both be
  `""` in one TU: one is the extern `g_emptyLiteral_0129aaf4` (0x00CD5170).
- Hoisting `const unsigned long n = obj.Count;` turns an up-counting `i < obj.Count` loop into retail's count-down
  loop and frees the object register; the order of named argument locals sets which value lands in ebx
  (primitiveCount, startIndex, vertexCount in 0x00B9E750).
- Retail `pushad`/`popad` around driver calls (winmm) is genuine inline asm: retail_asm with `__asm pushad` /
  `__asm popad` statements; the rest stays C++ (0x00C55DA0).

Review at landing:
- 0x00B9E750: the final local order (primitiveCount, startIndex, vertexCount) was found with compile-only probes after
  the row's 4 q.py grades; its only official grades are the final gall.sh run and vland (both RELOCATION_MATCH). Also
  switched to engine/CIndexBuffer.h + engine/CTexture.h afterwards (identical object code, vland re-run).
- 0x00C298D0: re-cut the catalog row to 542 bytes (0x00C298D0..0x00C29AEE) and drop/merge Catch@00c29a30 and
  Catch@00c29a3c; the differ/ source then lands unchanged (Flags /EHsc /Gr).
- 0x00C00E60 carries a REDUCED deflate_fast match step as its same-TU caller; it must never be landed as deflate_fast.
- 0x00C62530 carries same-TU stand-ins for 0x00C62350 (landed separately as `SetLungedThisFrame`, a propagated
  catalog name for libpk's random 0..1) and 0x00C62370 (`GetRandomPointInSphere`, not landed: its stand-in byte-matches
  retail, so it can become its own row). It also casts GetHandleTable's `CHandleTable*` to `CSoundSystem_00c37840*`:
  both descriptive names denote the same sound-system object (slot 22 stop / slot 25 volume / Play_00c13140); unify.
- The play-parameter block has three descriptive class names in landed code (CSoundPlayParams_00c37840 at 0x00C37840,
  `State` at 0x00C27C10, CBrainOutput for its setters); NAME_FIXES.tsv proposes CSoundPlayParams_00c37840 with float
  setters -- 0x00C27C10 copies raw dwords into those setters, recheck its bytes after respelling.
- 0x0129BA3C (1e-4f) has about 49 landed spellings; this chunk used `g_epsilon_0129ba3c` (the GOTCHAS chunk-200 one).
- CDataMarkerOutput::CElement default DataAsULONG is 10 (retail stores 10 after the two "" member ctors).
- Already-landed errors noticed (also in NAME_FIXES.tsv): 0x00C62440 landed as CRegion::SetMiniMapRegionExitTextOffsetY;
  0x00C13140 as CTCHeroExperience_00c13140::GetBlockParryReward; three addresses (0x00C20420/0x00C20480/0x00C204A0)
  landed under the one name CBrainOutput::SetAsPumpCombatActions; vector<CDataMarkerOutput::CElement>::_M_insert_overflow
  bound QAE (STLport 4.6 has it protected: IAE).
