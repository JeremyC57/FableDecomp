# Chunk 655 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- Never-attempted twins are cheap when the identity work is done first: all ten rows matched within 1-2 probes once the
  element type / enum / class came from the caller chain (sole caller -> its bound caller -> the PDB member type). The twin's
  NAMES were wrong for every row (find vs __find's caller, C3DGroup2/CEngineSceneGrid vs CMeshDataBankEntry, three different
  Transfer T's, EndProfile vs CameraCancelScreenEffect, Release vs a block-op stand-in, implicit ~CTCHeroPodium vs OnDie).
- `CPersistTraits<T>() : DefaultVal() {}` with T a NESTED enum (NScript::CQR_EscortTraderScript::EscortTraderQuestStage)
  fails in VC7.1 with C2065/C2064; `DefaultVal((T)0)` emits the identical zero store (0x00E1DF60, 0x00E38DD0).
- Retail STLport vector<bool> is a plain byte vector: CPersistContext::TransferVector<bool> 0x00CDCF80 matches over
  `vector<bool, allocator<bool> >` with `clear() { erase(begin(), end()); }` calling the out-of-line byte erase 0x004676BB.
- Model STLport vector as `template <class _Tp, class _Alloc = allocator<_Tp> > class vector`: a one-parameter model
  mangles `?$vector@VT@@@std@@` and misses the PDB/retail `?$vector@VT@@V?$allocator@VT@@@std@@@std@@` (several landed
  CVertexGroupBlock / vector<bool> rows carry the one-parameter spelling; NAME_FIXES).
- STLport 4.6 `vector::operator=` (_vector.c, three branches, `__copy_ptrs` + `_Destroy` / `__uninitialized_copy`) matches
  verbatim under /Gr with the three range helpers declaration-only (0x00ADCA30, 347 bytes, first probe).
- A destructor with no vtable store whose class has no PDB destructor is the IMPLICIT one: define the class's constructor
  in the TU so the vftable, `??_G` and `??1` are emitted, and let `~vector` / `~_Vector_base` inline (0x00771F30).
- `uninitialized_fill_n` vs `__uninitialized_fill_n`: `ret 4` (one stack argument after ecx/edx) is the 3-argument front
  end with the tagged helper inlined; the tagged one would be `ret 8` (0x00A95890, 0x00B96150).

Review at landing:
- NAME_FIXES.tsv holds 19 rows. The respells touch landed callers: 0x0057598A (find), 0x004B0160 / 0x004B2FE0 / 0x00AEBC90
  (__find), 0x0049F180 / 0x00416953 (EndProfile), 0x00A97F70 (whole N00a97f70 stand-in family: its other helpers
  0x00A95820 / 0x00A95B00 / 0x00A95E10 / 0x00A97CE0 are std:: helpers over CCountedPointer<C3DGroup2> too, not respelled
  here), 0x00B97E90 family (CUnprovenPointee_00b97cb0 -> CEngineSceneGrid), 0x00771F10 (free-function stand-ins for
  ??_G/??1), 0x0049B8D0 and its callers, 0x00ADC120/0x00ADC0D0 (Copy_SArrayTreePair18 stand-ins), 0x00ADCB90 / 0x004676BB
  (one-parameter vector spellings), 0x004675C5 / 0x004675F6 bindings.
- 0x00CA5510 is respelled `?Release@CBasePin@@UAGKXZ` only to follow its landed slot-1 sibling `?AddRef@CBasePin@@UAGKXZ`
  0x00CA53F0; CBasePin is AM_NOVTABLE in strmbase, so the class owning table 0x012BF3C0 (a concrete CBaseOutputPin-derived
  pin) is still to be proven -- respell both together.
- _M_fill_insert access: the landed C3DGroup2 instance 0x00A97F70 is bound QAE (public), the CEngineSceneGrid one 0x00B97E90
  IAE (protected); one is wrong -- check STLport 4.6 _vector.h.
- The landed twin 0x00529CC0 ("CTCInventoryItem::OnDie", 0 xrefs) is an implicit destructor shape (free [this+0x10], tail jump
  to ~CBaseClass 0x0099A2E0, no vtable store); its class is unproven.
- reloc_bindings.tsv binds 27 unrelated stand-in names (`_Profile_Enter@4`, `?LM_Enter@@...`, ...) to 0x00404500, which is
  CPersistContext::CheckCRC (landed CPersistContext_CheckCRC_00404500.cpp; Ego_r ?CheckCRC@CPersistContext@@IAEXPBD@Z). These
  look like mis-attributed targets in old landed rows; worth a bindings audit.
- 0x00CA9710's vland WARN SHARED_HEADER_AVAILABLE CBaseObject is a false positive (DirectShow strmbase CBaseObject, not
  Fable's engine class).
