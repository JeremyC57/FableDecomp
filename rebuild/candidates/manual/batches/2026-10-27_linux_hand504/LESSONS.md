# Chunk 504 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- A libpk queue Pop that ends `mov eax,[arg] ; mov edi,eax ; rep movsd` returns its pointer argument: the slot is `T* Pop(T* item) { ...; return item; }`. A `void` Pop loads the argument straight into edi, which costs 2 bytes (0x00C1B370, 0x00C1DD70).
- When a libpk queue Push compares the count with `jb` and wraps with `div`, the count, capacity and head are `unsigned long`. A `long` member gives `jl` / `idiv` (0x00C1AEC0).
- A flag store scheduled between the loads of a struct copy (`mov ecx,[esi+4] ; mov eax,[arg] ; mov [esi+0Ch],0 ; mov [eax],ecx`) comes from the flag being cleared BEFORE the copy in source; after the copy it lands last (0x00C1DD70).
- A destructor whose only statement is a call still emits the implicit destruction of `std::list<T*> m[3]` members. Each array is a counted `dec ebx` reverse loop through an inline STLport ~_List_base (clear() inline, then a null-tested sentinel free), and the inline empty virtual base dtors give the trailing vtable stores (0x00BD9ED0).
- In a /Gr STLport __partial_sort, model make_heap as the inline wrapper `__make_heap(first, last, comp, (T*)0, (int*)0)`, which is `push 0 ; push 0 ; push comp`. The by-value element for __pop_heap needs a same-TU noinline __uninitialized_copy body for the inline vector copy (0x00BEF700).
- STLport `swap<T>` with a counted-pointer member matches as the 3-line temp swap once the CCountedPointer members are __forceinline (0x00BED690).
- Identify a vector element from the caller that passes `this+offset` and the Ego_r.pdb layout, not from the catalog. BSim labelled 0x00BDB750 `C2DLineF`, but the operator= 0x00BDB9F0 that calls it is fed CVertexShaderConstantLayout::ReservedPresets (+0x140) (0x00BDB9F0).
- x87: a member at zero displacement (`[ecx]`) can be ranked cheaper and evaluated first. In a row-wise rotation the first row comes out `c*X` before `s*Y` while the displaced rows keep the retail order, and swapping source operands did not move it (0x00C1CA20, parked).
- `GetTickCount() - m_tick` compared with a global: retail folds both memory operands (`sub eax,[edi+4] ; cmp esi,[g]`); VC7.1 /O2 here loads both into registers first, whatever the statement form (0x00C0FFE0, parked).
- Lane tooling: q.py ignores the source's `// Flags:` line, so pass `--flags /Gr` or `--flags /EHsc` on every probe (gall.sh reads the line itself). One grade was spent learning this on 0x00BEF700.

Review at landing:
- vland RELOC_NAME warns `??6CString@@QAIAAV0@PBD@Z -> 00c0dec0 labelled 'CString::operator<<'` even though the class is spelled exactly as the label: the check seems not to demangle `operator<<` (false positive).
- 0x00C0DEC0 / 0x00C0DF00 / 0x00C0BA40 / 0x0139C900 / 0x0143BB2C carry many spellings in landed rows: CDebugStream, CLogLine_<addr>, CString_<addr>, g_pDebugBuffer / g_pchNil_0139c900 / _afxPchNil, g_pfnErrorReport / g_pfnAssertHook. This chunk uses the CString / _afxPchNil / g_pfnAssertHook / CDebugBreak spelling of the sweep1 PKCDefaultFileManager 0x00C1A230; unify the rest.
- 0x00C0FFC0 / 0x00C0FFE0 are spelled PKCProtectionInstance, CScopedLock (CSoundEventManager::DispatchToEntry 0x00C115D0), CProfileLock_00c119d0 and CQueueLock (CMailboxQueue_00c39830) in landed rows.
- The 16-byte handle 0x00C25C50 / 0x00C25C70 / 0x00C25CB0 / 0x00C25CA0 has four unrelated landed spellings:
  - `T::Init` (C3DGroup2)
  - `CParticleRef_00c25c70::Bind`
  - `CParticleHandle::~CParticleHandle` for 0x00C25CB0, which is really a Release: the true dtor is the empty 0x00C25CA0
  - `C00c25ca0::M` ($E2)
- Landed std::sort_heap 0x00BEF350 is a non-template global `sort_heap(SDecalEntry*, ...)` declaring a global `__pop_heap` for 0x00BEEDA0, with an invented SDecalEntry. This chunk's 0x00BEF700 references the real `std::sort_heap<pair<pair<long,long>,CEngineTextureDecal>*, CKeyPairCompareLess<...>>` / `std::__pop_heap<...>` templates, so corpus-wide those addresses now have two spellings.
- Landed 0x00BDB750 is instantiated as `__uninitialized_copy<C2DLineF*, C2DLineF*>`; it is `<const CVertexShaderConstant*, CVertexShaderConstant*>` (this chunk's 0x00BDB9F0 references that spelling).
- Landed 0x00A6B420 / 0x00A6BBB0 declare `class` pair / less (V); the PDB keys are U (struct).
- Landed 0x00AD75F0's leaf spells the const instantiation (PBV); 0x00AD83D0 calls it as `<T*, T*>`.
- Invented names on landed helpers: 0x00B0F670 / 0x00B0F6B0 (CParticleUpdateParamDesc pair), 0x00B6BAB0 / 0x00B6B700 (CVertexShader), 0x00BF8F30 / 0x00BF8F70 (SPairLongCSecond_0099ec30 = CEngineTexturePalette::CPaletteEntry) and 0x0051D400.
- 00ac8720's `_M_insert` stand-in shares the symbol of landed 0x00AD3440; check that the bodies agree.
- 0x0129BA3C (the float epsilon used by 00ac7200) has 49 spellings in landed rows.
- Grade budget:
  - 00ac7200 went over 4 through probe compiles (counted as grades).
  - 00ac8720, 00c0ffe0, 00c11230 and 00c1ca20 used exactly 4.
- Local base models next to existing engine headers: 00bd9ed0 (CEngineLandscapeMemoryManager.h flattens CFailedAllocationHandler / CMovableResource), 00af1e90 (CIVCountedPointeeBase).
- libpk descriptive class names (PKCStaticQueue, PKCOneShotQueue, PKCProtectionInstance, CSoundEventManager) follow the sweep1 rows without an address suffix, as those rows chose; the method names (Push / Pop / Slot00 / CanStartEvent / RotateC) are slot- or behaviour-descriptive.
- The PKCStaticQueue interface's Pop must become `T* Pop(T*)`, and count/capacity unsigned, in the landed models: ctor 0x00C1B340, PKCDefaultFileManager 0x00C1A230, PKCOneShotQueue 0x00C1DD50 / 0x00C1DF30.
