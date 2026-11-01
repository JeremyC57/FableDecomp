# Chunk 670 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- 2026-11-01 (chunk 670) A virtual call written in a destructor (`~CFontDataBankEntry() { Unload(); }`) is devirtualised by
  VC7.1 and expanded from a same-TU body defined AFTER the dtor, as retail's file order has it (0x00A63480 / 0x00A5F150 Unload,
  0x00A0B9A0 CSoundBankLH slot 2); no inline helper or qualified call is needed. A release written before the member's own
  ~CCountedPointer leaves `mov eax,edi ; cmp eax,edi ; je` (VC7.1 propagates the zero Info but keeps the dead block).
- 2026-11-01 (chunk 670) VC7.1's compiler-generated destructor converts `this` to a non-primary base with the null-checked
  adjustment (`test esi,esi ; je -> xor ecx,ecx ; call ~Base2`) and duplicates the tail jump; a user destructor uses the plain
  `lea` (0x00B8C7C0 ~CEngineBillboardGenerator vs 0x00B84390 ~CEngineSubPrimitiveRenderer, both with CEngineComponent at +4).
  That null test is the sign the destructor is implicit even before checking for the vtable store.
- 2026-11-01 (chunk 670) The real STLport 4.6 text again decides per-container inlining in one dtor with no hints: in
  ~CSoundManagerLH vector<pair<CCharString,T*>> expands inline, vector<pair<pair<CCharString,CCharString>,T*>> and
  list<T*> (clear() out of class, as _list.c) stay out-of-line `??1?$vector` / `??1?$_List_base` COMDATs (0x00A40BD0 /
  0x00A3FF60), exactly as retail.
- 2026-11-01 (chunk 670) Masked-identical destructor twins with duplicated catalog labels (CFontDataBankEntry 0x00A5F150 /
  0x00A63480; C3DMeshLODInfo 0x00A24470 / 0x00A0B9A0) are separated by the vtable each stores; the RTTI COL at vtable-4 gives
  the class and its base-class array (contained counts, mdisp) without any manifest help.
- 2026-11-01 (chunk 670) A delete loop `for (unsigned long i = 0; i < arr.size(); ++i) delete arr[i];` over a STLport
  CArray of non-virtual-class pointers gives retail's `sar ; jbe`/`jb` re-read loop with an inline null test + ??1 + operator
  delete (0x00B93E40).
- 2026-11-01 (chunk 670) Lionhead free / static functions are __fastcall (Ego_r `YI` / `SI` manglings): a one-instruction
  `mov [g],ecx ; ret` body is a static setter, and `push ecx ; mov ecx,[g] ; call X ; ret` is a fastcall forwarder passing its
  argument to a method of the global (0x00B84220, 0x00BEB540 -> CSimpleMemoryPool::Allocate).

Review at landing:
- NAME_FIXES.tsv (29 rows): six own-row respells, four own rows whose landed `??_G` callers call them under invented
  spellings (`@<Class>_dtor@4` free fastcall functions, `?Dtor@<Class>@@QAEXXZ`), ten `??_G` rows landed as free __fastcall /
  `ScalarDeletingDestructor` stand-ins (all slot 0, each calling this chunk's row), and neighbours 0x00A40BD0, 0x00A3FF60,
  0x00A66F80, 0x0099A300, 0x009D5F80, plus three descriptive-but-proven-class respells (0x00A0B6C0, 0x009918F0, 0x0143BAC4).
- 0x00A0BAC0 (landed ??0CSoundBankLH(long bankIndex)): the argument is a sound-manager pointer, not a long -- slot 1 reads
  [+0xC]->+0x40 (CSoundManagerLH::m_pSoundSystem) and the callers pass CSoundManagerLH `this` (0x00A3E444) and PSoundManager
  (0x0042E1A0). Whether the declared type is CASoundManager* or CSoundManagerLH* is not proven, so it is not in NAME_FIXES.
- 0x00B8C7C0's TU defines ??0CEngineBillboardGenerator@@QAE@XZ with an empty body only to emit vtable -> ??_G -> implicit ??1
  (not retail's body; not graded), as chunks 664/665/667 did.
- Same-TU bodies defined only for inlining (not graded): CFontDataBankEntry::Unload / CStreamingFontDataBankEntry::Unload
  (landed separately at 0x00A63580 / 0x00A5F250), CSoundBankLH::Unload_00a0b6c0 (0x00A0B6C0, landed as a free fastcall stand-in).
- Landed rows with poor models met on the way: the ten `??_G` stand-ins above; 0x00A5F290 ??0CStreamingFontDataBank calls its base
  as N_009d5f80::CBankFile (it is ??0CBankFileAsync); 0x00A0B6C0 landed as free `ClearAllChildPrimitives(CEnginePrimitiveRenderer*,
  CEnginePrimitiveHandle*)`; 0x00A40BD0 / 0x00A3FF60 landed as CRetailVector / TreeList stand-ins.
- No row needed more than 3 probes or 4 grades; nothing parked, nothing int3-after-noreturn.
