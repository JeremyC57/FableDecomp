# Landing nf192 (Account A, 2026-11-20): engine / render / resource / data-stream names, second pass (16 rows)

Agent NF192 (round 10) handed back early (context) with 50 entries: 10 done, 3 done-already, 4 blocked, 1 deferred and 32 open
(q6437 onward). Its files are in `naming/2026-11-20_agentNF192/`, with the July-row probe (`work/b1_glyph_julyprobe.log`) and the
withdrawn libpk draft (`work/c266e0/`). The open entries and q11864 (see below) go back to the queue unclaimed.

- 16 rows land, all RELOCATION_MATCH, every test PASS.
- Streaming-font glyph cache (q4201-q4204, plus 0x013BDA24 `textureCacheMem` from NAME_FIXES): 0x009A1660 and 0x009A14E0 move from
  the invented `g_pStreamingFontCache*` / `g_pGlyphAdjustments` globals to the Ego_d PUB32 anonymous-namespace statics
  (`?glyphTextureCacheGlyphDataIndices@?A0xfe46a155@@3PAKA`, ...GlyphData, ...GlyphDataCache, ...GlyphAdjustment, textureCacheMem).
  The second source is the landed 0x009A3640 InitializeStreamingFontTextureCache body (new[] element sizes and order). In RENAMES,
  `?A0x<tu>` stands for the per-TU hash.
  - Advisor decision (owner delegated; EXTRA_RULES 2026-11-20, new section): per-TU `?A0x` hashes of one anonymous-namespace PDB name
    count as one spelling. VC7.1 derives the hash from the compile path, so no row text can reproduce retail's. Normalised (hash
    stripped), each of 0x013BDA24 / 34 / 38 / 3C / 40 keeps one spelling with its type code; the stand-ins are gone.
  - The queue lines carried `?A0xd4716bce`, a hash found in no PDB name set (it came from a rebuild object). They now carry Ego_d's
    `?A0xfe46a155`, which the Ego_r / FableWin name sets also use for that TU's other symbols.
  - The same ruling clears q11864 (0x0122A600, GFontTextureCache, held at nf142 for a third TU hash); it returns to the queue.
  - Queued: gate.py `collapse()` / tablecheck should strip the hash, and reloc_bindings.tsv should normalise it on write.
- 0x00AC0C00: a self rename (catalog CEngineWorldMap::CreateNewBankMap -> C3DMeshFileMaterialChunk::CreateMap, both PDBs) and a port
  onto the shared headers. It now binds ??0CChunkedFileSuperChunk (q4362) and emits the C3DMeshFileMapChunk vtable, ??_G and DeleteData.
  - Pick-any: the owner rows are stand-ins (0x00ABFE50 has a 36-byte stand-in vtable; 0x00AC1460's Leaf is bare, hence pickany_check's
    MISSING). Graded at landing with q.py: DeleteData MATCH at 0x00AC1460, ??_G RELOCATION_MATCH at 0x00AC1960; the agent checked the
    vtable bytes and slot names against retail 0x0129EE34. Under the nf181 ownership rule copies equal to retail land; no ??_7 LNK2005
    appears, and 0x00ABFE50's port is queued.
- Other ports: 0x00AFF0A0 (leaf-only self rename, binds the CEnginePrimitiveParticleMeshGroup ctor, q4795), 0x00A8EDA0 (self leaf
  respelled from a third spelling) with binders 0x00A914B0 / 0x00A931F0 (q6242), the C3DVector->C3DAngles CRightHandedSet ctor binders
  (q5209), 0x009A0EC0 (CStreamingFontBank::AlphaMaskUVMul / AlphaMaskUVAdd, q6313; AlphaMaskUVAdd from NAME_FIXES on two sources:
  Ego_r's dynamic-initializer name and the landed engine/CStreamingFontBank.h spelling), the three
  CEnginePrimitiveManager setters, 0x00B3ADC0 (q5164's ported binder), and 0x0073A690 GetTimeToBlendIn (self, onto
  NAnimationEntryBodge_CAnimEntryLite.h) with its only referencer 0x00861A80 (q6370; this also closes q6396).
  - Old self leaves: no landed row still binds one (tablecheck counts referencers only; checked against the table and the HEAD objects).
- Headers (add-only, `orig` equals HEAD, default paths token-identical, asserts pinned with FABLE_STATIC_ASSERT_AT, no landed includer
  defines the macros): CChunkedFileSuperChunk.h ADDSUBCHUNK (declaration only), C3DMeshFileMaterialChunk.h CREATEMAP,
  C3DMeshFileMapChunk.h TYPE_CTOR (inline ctor, both PDBs), NAnimationEntryBodge_CAnimEntryLite.h INLINE_ACCESSORS.
- Blocked or deferred:
  - q4251 / q6204: their rows left (0x00BDC510 0x00BE7D70 0x00BE8920 0x00BF3B60) are July candidates-header rows that grade DIFFER at
    HEAD, so a port cannot be verified.
  - q5164: 0x00B3A850 / 0x00B3A8C0 are byte-identical ~_List_base / ~list twins with no callers; COMDAT order is one source, so no
    rename (advisor decision). 0x00B3ADC0's copies equal both twins, so its landing does not depend on this.
  - q6367: 0x00CDBD00's class is unproven.
  - q5193: libpk vendor code; the port needs an inline opt-in heavier than light renaming.
- Pick-any (`logs/pickany_check.txt`): the STLport list `clear` copy equals its owner 0x00B3A760 (58 = 58); 0x00AC0C00 as above. The
  strong ~CEnginePrimitiveAttachmentManager copy in 0x00B3ADC0 is unchanged from HEAD (existing debt).
- Link (`logs/link_landing.log`): 4 LNK2005, the same symbol set as HEAD (`logs/link_control_head.log`).
- Recheck (`logs/recheck.log`): rebind multi-check 1 (baseline); split gate 0 created. Includers and referencing rows 61 good, 0 bad;
  owner recheck good.
- tablecheck (`logs/tablecheck.txt`): eight addresses shrink (0x00AA8360, 0x012A1AB0, 0x00B3A760, 0x00A229E0, 0x013CA2F8, 0x013CA300,
  0x00AFEAD0, 0x00A8EDA0, each 2 -> 1). One grows, 0x004C5FA0 1 -> 2 `?GetTimeScale@CBulletTime@@QAEMXZ` from 0x0073A690: SAME (an
  unchanged binding the table lacked). No out-of-image oracle line added.
