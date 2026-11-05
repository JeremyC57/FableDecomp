# hdr/ -- shared-header corrections of chunk 780 (overlay, `FABLE_HDR_OVERLAY=$S/w22/agent780/hdr`)

Both edits are opt-in branches; the default path is byte-for-byte the repo text (`diff` against rebuild/include shows only the
added `#ifdef` blocks). No landed source defines either macro, so no includer was rechecked (SIBLINGS.tsv).

## fable_string.h -- `FABLE_RETAIL_CBASICSTRING_TEXT_EQ_INLINE`
- What: inside `template<class T> class CBasicString`, the declaration-only `bool operator==(const T* text) const;` gets an opt-in
  in-class body `{ return Compare(PData,text) ? false : true; }` (the `#else` keeps the declaration).
- Evidence: retail CV_BeardyBaldyScript::Main 0x00E4FB88..0x00E4FB98 expands `regionName == "BowerstoneSlumsWarehouses"` as
  `mov ecx,[eax] ; mov edx,literal ; call Compare 0x00411570 ; neg eax ; sbb al,al ; inc al` (= Compare(...) == 0), the same
  model chunk 778 used for WatchForQuestCardConditions 0x00E50910; FableWin.pdb has ??8?$CBasicString@D@@QBE_NPBD@Z (the
  out-of-line copy 0x004115A0 has the same body). The null arm (`repe cmpsb` of the literal against "") is CCharString's existing
  in-class `operator==(const char*)`.
- Used by: differ/00e4fa20.cpp (parked). Confirmation that the overlay copy is the one compiled: inverting the body to
  `? true : false` changed the row from DIFFER(1763v1756) to DIFFER(1762v1756) (tmp/g_e4fa20_overlaycheck.txt), then reverted.
- Includers of fable_string.h keep the default path (macro undefined).

## engine/CQ_SunnyvaleMasterData.h -- `FABLE_RETAIL_CQ_SUNNYVALEMASTERDATA_RELATION_STATICS`
- What: adds `static long RelationCounters[3]; // 0x0143E928` and `static CCharString RelationSuffixes[3]; // 0x0143E934`
  under the opt-in (the generated header carries the `// Retail opt-in` mark so gen_class_headers.py keeps it).
- Evidence: reloc_bindings.tsv binds 0x0143E928 as ?RelationCounters@CQ_SunnyvaleMasterData@NScript@@2PAJA (the landed Init
  0x00CDBA10 resets the three counters and the transfer 0x00CDC890 walks the three suffix strings at 0x0143E934); retail
  CV_FishermanScript::NPCAttitudeProcess 0x00E7C1D6 stores 2 into 0x0143E930 = RelationCounters[2].
- Used by: differ/00e7c010.cpp (parked; the store matches in every variant).
