# Landing nf182 (Account A, 2026-11-20): ctor / dtor names, slice 2 (classes CI..CZ + PK), 12 rows; 0x00C239B0 held

Agent NF182 (round 10) handed back early (context) with 39 entries: 12 done, 1 done-already, 2 rejected, 3 blocked, 2 deferred, 19 open
(not reached, or census only for CWideString q4284 / q4546). Its files are in `naming/2026-11-20_agentNF182/`: the overlay `hdr/`,
`CENSUS_CWideString.txt`, the held draft in `held/`, and the in-place respell script `work/dtorfix.py`. Open entries, and q11915 (its row
was locked by NF181, landed since), go back to the queue unclaimed.

- 12 rows land: 1 MATCH + 11 RELOCATION_MATCH, every test PASS.
  - CTCFlashOnHit: the ??_G 0x004D62D1 is re-authored onto engine/CTCFlashOnHit.h with the anonymous-namespace scaffolding (Account B's
    0x00BB1740 recipe). Self rename: `??_GCTCFlashOnHit@@UAEPAXI@Z`. Its ctor owner 0x004D6292 is ported the same way, so it no longer
    defines the ctor strongly. Both objects now emit identical pick-any ctor / vtable / ??_G COMDATs (the nf140 case, settled here).
  - In-place ??_G respells: 0x005297D0, 0x007CF390, 0x007ADA80, 0x00A628B0, 0x00670C50. A stand-in `Dtor()` / `X_dtor` becomes the
    class's virtual dtor called by qualified name. A header port is not possible as a single row: each class's ctor is out of line.
  - Header ports: 0x009E45B0 onto engine/CMaintainedEvent.h, 0x00B260B0 onto engine/CSimpleMemoryPool.h, 0x00BF3290's base onto
    engine/CIndexBuffer.h. 0x00CBE09A is a leaf-only self rename (PAV -> AAV, two PDBs) onto the shared header. 0x004473A0 is
    respelled in place.
- Headers (add-only opt-ins; `orig` equals HEAD for all three; landed includers define none of the macros):
  - engine/CTCFlashOnHit.h: FABLE_RETAIL_CTCFLASHONHIT_DTOR (inline ctor plus declarations). pin_asserts pinned its one assert at
    line 19.
  - engine/CSimpleMemoryPool.h: FABLE_RETAIL_CSIMPLEMEMORYPOOL_FLAT_CTOR.
  - engine/CMaintainedEvent.h: FABLE_RETAIL_CMAINTAINEDEVENT_CTOR. The default path is token-identical; the closing `};` moved to its own
    line, and the header has no asserts.
- Held: 0x00C239B0 (q4891). The draft grades RELOCATION_MATCH but moves 0x00C22840's binder from one stand-in spelling to another.
  0x00C22840 is really PKCFrallocStatic<CSoundEvent>, so it is settled first; queued, and the draft is in `naming/.../held/`.
- Coordinator calls: 0x004473A0 newly defines the implicit `??1CSavedInputEvent@@QAE@XZ` of its HEAD local model. Retail inlines it, no
  address owns it, and only its own .debug$F references it, so it is accepted. Queued:
  - ??0CScriptModuleBase is defined by 0x00C21BA0 (really CSoundEvent's ctor) and bound for 0x005ED070 by 0x005ED190: one name, two
    addresses.
  - q4568's three-spelling split on 0x00C1EB10.
  - The NF20 pair 0x004DC1D8 / 0x004DAB44 still has the strong-vs-COMDAT ctor shape.
- Pick-any (`logs/pickany_check.txt`), every new copy against its owner:
  - The CTCFlashOnHit ctor and ??_G grade RELOCATION_MATCH against 0x004D6292 / 0x004D62D1.
  - 0x00BF3290's ??1CIndexBuffer (22 bytes, owner 0x00B564E0), ??_GCIndexBuffer (42, 0x00AC2070) and ??_GCMovableResource (31, 0x009D5110)
    match byte for byte with equal relocations. Its stand-in-named ??_GCEngineDynamic3DResource matches the 0x00BF34B0 family row
    byte for byte (30 bytes; the call target is ??1CIndexBuffer against the owner's Dtor stand-in). These are hand comparisons: there
    are no table lines.
  - 0x00CBE09A's header-inline ??1CWideScreenMagicPauseEntities equals 0x00CBE0B3's (13 = 13).
- Link (`logs/link_landing.log`): 2 LNK2005 against 1 at HEAD (`logs/link_control_head.log`). The extra one is
  ??1CWideScreenMagicPauseEntities. 0x00CBE0B3 defines it strongly from a row-local model, and 0x00CBE09A now emits the identical header
  copy (the nf181 0x00485400 case). Porting 0x00CBE0B3 onto the header is queued as debt.
- Recheck (`logs/recheck.log`): rebind multi-check 1 (baseline); split gate 0 created. Includers and referencing rows 101 good, 0 bad;
  owner recheck good.
- tablecheck (`logs/tablecheck.txt`): four addresses shrink (0x00A04410 3 -> 1, 0x006A5D50, 0x00A5C570, 0x00AC1EC0 2 -> 1). Four grow
  0 -> 1, each to the self row's own Leaf: 0x00670090 / 0x007ADAA0 / 0x007CF0B0 / 0x00A628D0 `??1<Class>@@UAE@XZ`. Their ??_G rows
  bound address-less stand-ins before.
