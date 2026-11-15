## NAMING.md

# 00A495F0 naming and ABI

True public leaf is `?ReadFromFile@CLipSyncSampleData@@QAEXAAVCDataInputStream@@@Z`,1261 whole bytes at00A495F0. All11 supplied row files were consumed (10 unique plus1 exact SHA alias), including every39-record handback row; PRIOR_READ_LEDGER.json records hashes and actual read/alias states. No historical budget or stopping instruction was inherited.

Original raw Ego_r schemas preserve class keys V and private fields: CLipSyncPhoneme has Symbol/ID/Weight at0/4/8; CLipSyncFrameData has CArray<CLipSyncPhoneme> Phonemes; CLipSyncSampleData has FrameData and FramesPerSecond. Native vectors are12 bytes rather than the donor16, yielding frame12 and sample16, frame rate+12. Original locals are is,phoneme_map,id,symbol,c_frame,phoneme. The source uses genuine std::map<unsigned long,CCharString>; no CNameMap or sink remains. Current CCharString uses the original protected PStringData. Native copy ctor0099EC63 increments Data->NoRefs at+13, physically agreeing with the packed17-byte string-data/13-byte basic-string model.

Private cross-class serialization accesses require the minimal CLipSyncSampleData friendship in phoneme/frame headers. PDB does not encode friend declarations; this is an explicit interpretation from native accesses to original private fields, not a claimed PDB friend record. An unnecessary extra friendship was removed before final grading. Original GetSymbol returns CCharString by value; no invented reference-return getter is substituted.

CDataInputStream::ReadNullTerminatedString is the genuine public introducing virtual, returns CCharString by value and has PDB slot offset24; the native and rebuilt call use[eax+0x18]. VIRTUAL_CALL_PROOF.json preserves the complete method record. All99 direct/data relocation identities resolve, including float bits4F800000,3B808081,00000000,3F800000. The full original STLport source establishes private _M_erase, protected _M_insert_overflow, and public _M_fill_insert; no ABI is guessed from the later Dinkumware donor.

NAME_SEARCH.json searches all814 supplied naming files/all963750 records, including PDB names,data,label/string/FSE tables and inventories; all3 raw Anniversary PDB name sets are also searched. NAME_FIXES.tsv adds11 original typed binding records supported by complete native bodies and their exact typed callers. Existing binding discrepancies are recorded for review; the selected typed graph has one symbol per native address. No identical-code-folding claim, linker alias or forced renaming is used. The original std::_Rb_global<bool>::_M_decrement spelling is already established in landed sources, naming decisions and bindings.


## MAIN_REVIEW.md

# Independent main review

1261 target bytes plus25 selected helper bodies3177 bytes,99 exact relocation bindings. Fresh VC71 /Od /W3 /GR- and GCC64 C++03 complete-loader tests passed480 scenarios/960loads; original vendor57-byte tree predecessor was freshly built, selected by real link-map/PE checks and executed over11826topologies/80638calls, with fresh GCC counterpart. Retained64-byte speed-TU copy is explicitly unselected. Integration REQUIRES original vendor object first; unmodified source/hashes and build recipe are provided. Main removed unsupported folding language and extra fixture stdout; no production logic changed.

Combined59-row/179-header regression preserves all emitted C++ code and relocation names, including explicitly unselected copies. Helpers do not add assigned match progress.
