# 009bb110 final handoff

Final status: **VERIFIED_MATCH**. Selected grade: 5. 

ROW.json and root STATUS.tsv are authoritative. Earlier status claims below are historical; they may have been superseded.

## rows/009bb110/NAMING_EVIDENCE.md

# Original symbol/type and relocation audit

The exact Leaf is `?TransferVectorLoadText@CPersistContext@@IAEXPBDAAVCPersistContext_TransferableVectorGenericType@@@Z`: protected nonvirtual void, const char* name and generic-vector reference. Both exact original name sets, both CPersistContext complete TPI records (`evidence/009bbcc0_{Ego_r,FableWin}_types.json`) and both original procedure records (`evidence/009bb110_locals.json`) prove it independently. The locals also name CStringParser parser, CCharString t, long text_start/name_length/index/array_size/array_index and CPersistTraits<long> int_traits.

Both independent original generic-interface TPI records in `evidence/009bb110_{Ego_r,FableWin}_types.json` prove class keyV, vptr-only size4, no virtual destructor and public pure virtual slots Size0, Resize4, TransferOut8 and TransferIn12. Size returns long; Resize takes long; TransferIn takes CPersistContext&, CStringParser&, long. Every native indirect call uses those slots and arguments. The full original interface is in the private header, without substitute fields or invented vtable entries.

Both original CPersistTraits<long> TPI records prove the class key, public long DefaultVal and its member TransferIn overloads. Both original CPersistTraitsSLONG records (`009bb110_*_slong.json`) prove the stateless original class and static fastcall functions with context/parser/value references. Both exact original name sets contain `?TransferIn@CPersistTraitsSLONG@@SIXAAVCPersistContext@@AAVCStringParser@@AAJ@Z`. Both original procedure records are also preserved in `009bb110_callee_locals.json`. Native009e2590 takes context inecx, parser inedx and a value reference on the stack. This establishes the original spelling over older stand-in functions and the queue's obsolete uncertainty. The selected source uses the real local CPersistTraits<long> and its value-copy wrapper; it does not rename the static family to a member ABI.

Both original CStringParser/CCharString/CBasicString<char>/CBaseClass TPI records are preserved in `009bbcc0_*_types.json` and `00501d30_*_dependent_types.json`. They prove the parser copy constructor, public parser reads and accessors, protected parser field types, CCursorPos members, CCharString assignment/compare/operator types and class keys. The current private headers retain complete original fields and default branches. CPersistContext fields and access are original, including bool SingleTransfer/Finished and const parser reference to PersistError. The source's ordinary inline parser copy follows those exact original members; no stronger-inline directive was introduced. Native0046381b is the copy constructor called by this target; both original names and copy procedure signatures confirm it.

All163 COFF operands are individually enumerated in RELOCATIONS.json and checked against the native target or the complete terminated literal by `workers/engine/009bb110_review.py`. All ordinary called decorated names occur exactly in BOTH original name files and have matching original class or procedure types. Each string/error literal is compared directly to the native bytes. CStringParser::Comments is list<CStringParserCommentBoundaries,allocator<CStringParserCommentBoundaries> > in both independent PDBs. Original STLport4.6.2 _list.h defines its _List_base destructor; native004046b0 performs sentinel cleanup. The newer cx11 priority correction (NAME_FIXES_QUEUE.tsv4195) explicitly supersedes the old list-destructor spelling. The selected object emits the exact _List_base Leaf.

The native extent is the assigned2340 bytes, ending with ret8 at009bba31; twelve followingCC bytes are alignment before009bba40. Both complete raw object/native disassemblies are retained. The audit compares the complete extent, all relocation masks and exact Leaf. NAME_FIXES.json records stale/conflicting bindings without editing the shared tables. Main independent acceptance is pending; this agent writes LOCAL_REVIEW.json only.

Main independent acceptance: attempt3, exact assigned extent and all relocation symbols verified; current source tests rerun under GNU C++03 and actual VC7.1 /GR-, both PASS.

Eligibility correction04: original CStringParser copy is generated0x103 in both PDBs. The prior authored copy body was removed, retaining only its original external declaration. Old03 source is ineligible and preserved under reviews/before_generated_copy_correction. New04 still RELOCATION_MATCH2340, all actual operands independently rechecked and exact-body GNU64/VC7.1 tests PASS. No change to Main logic.
