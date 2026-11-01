# Chunk 666 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- (2026-11-01, chunk 666) Never-authored pool rows settle from caller + vtable + PDB donor order in one pass: 10/10 matched,
  7 on the first probe. Three catalog labels were propagated over different classes (CTCInventoryMap ->
  CConversation implicit dtor; CFrameData over CCompositeSurfaceCache; "__find_in_uint_array" over the
  CCountedPointer<CComponent> instantiation). For a CTC/def class whose methods sit in one TU, the Ego_r / FableWin donor
  order of `?Name@Class@@` entries equals the retail address order (CTCKraken 0x007FC210..0x007FC9D0, CTCHeroMorph
  0x0071C0C0..0x0071C7C0) -- a quick second source for every callee name.
- STLport list back() vs a named loop iterator (0x0071C260): `iterator it = end(); --it; if ((*it).Get()) back()->Kill(true);`
  gives retail's two shapes (`mov ecx,[eax+4] ; add ecx,8` for the named iterator, `mov eax,[ecx+4] ; lea ecx,[eax+8]` for
  back()'s temporary); back() at both sites or a direct `_M_prev` read at both sites gives one shape twice. When two reads of
  the same element differ only in the register that holds the node, look for two different access paths in the source.
- Inline member bodies the TU defines are reproduced for free: a same-TU non-inline member (CHealthOverTimeHelper::Update /
  IsFinished 0x006A6880, ResetTexture 0x009FE6C0, InitTentaclesOnly / InitTentaclesAndHead 0x007FC730) is auto-inlined by
  VC7.1 exactly as retail; two byte-identical inlined members in adjacent switch cases merge into one arm with two jump-table
  entries. The same TU then also reproduces the out-of-line copies (0x009FE440, 0x007FC6F0, 0x007FC710, 0x0071C1F0 all graded
  RELOCATION_MATCH from the chunk sources).
- Switch arms follow SOURCE case order (0x009E3870: PFType cases 2,5,4,7,6 give retail's arm order); relocation masking hides
  the jump table, so check the case -> arm pairing in the object (`objdump -r` + `objdump -t` of the $L labels) against the
  retail table by hand.
- Re-indexing a global table per member (`g_D3DFormatInfo[FormatIndex].X` each time) keeps retail's `shl eax,2 ; mov
  ecx,[eax+table+4]` scaled-index form; a `const T& info = table[i]` local forms the entry address with `lea` first (0x009E3870).
- A non-polymorphic class deleted with `delete` calls `??_G` (push 1) even when its dtor is inline (GOTCHAS 24m); retail's
  inline expansion of the CGraphicFrameData release is `data->~CGraphicFrameData(); operator delete(data);` (0x00B2B910).
  ~CGraphicFrame only expands in ~CCompositeSurfaceCache as `__forceinline` over a named `data` local (plain inline is over
  VC7.1's budget in every form tried: 8 probes); no ??1CGraphicFrame exists in either PDB and every retail use is expanded
  (incl. ??_GCGraphicFrame 0x00B2ED60), so this is the owner-sanctioned Lionhead-header __forceinline case.
- Def Transfer overrides (0x004615FB, 0x004DF562) are one `context.Transfer(member)` per member in declaration order, no base
  call; maps go through a member template `TransferMap<V,K>(std::map<K,V>&)` over the STLport map (12 bytes), which also gives
  the PDB template-argument order <V,K> in the mangled callee.
- STLport random-access `__find` (0x00534EB0) is matched by an explicit instantiation of the verbatim _algobase.c template with
  `__fastcall` on the definition; the element compare inlines the FableWin free `operator==(const CCountedPointer<T>&, const
  CCountedPointer<U>&)` (`??$?8...YI_N...`), which compares Data only.

Review at landing:
- 0x00B2B910 / 0x009FE6C0 carry `__forceinline ~CGraphicFrame` (evidence above; owner decision 2026-11-01 covers Lionhead header
  release helpers). If engine/CGraphicFrame.h gains methods, it should carry this dtor and UnInitialise (out of line, 0x00A00B90).
- 0x0071C260 needs a named loop iterator for the first back() read -- a source shape, not a lever; recheck if a shared STLport
  list header changes iterator text.
- NAME_FIXES.tsv: 21 rows (own-row respells for 0x007FC6F0 / 0x007FC710 / 0x009FE440 / 0x00A00B90; caller respells incl. the
  GetPItem return type, InternalModifyHealth / CreateTentacles access, the CThing::Kill and ~CCharString families).
- The table 0x0129BA40 has eight competing bound spellings (NOTES.md): pick one canonical name.
- TU harness TUHarness_DestroyCConversation in 0x00E54D80 is not a retail function.
- vland WARN RELOC_NAME (catalog labels, not bindings): 0x00461660 / 0x00461894 are labelled `_global::PersistTransfer_*`
  (this chunk's descriptive `??$TransferMap@MW4E...@CPersistContext@@QAEXAAV?$map@...` is the PDB template form minus the
  name argument); 0x00A69AD0 is labelled `CFrameDataHandle::Clear` -- Ego_r ?Release@CManagedTexture@@QAE_NXZ (NAME_FIXES).
