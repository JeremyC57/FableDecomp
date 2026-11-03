# Original identity

`?Load@CParticleEmitterDatabase@NParticleEngine@@IAEXAAVCAFile@@ABVCEmitterDatabaseSerialiseInfo@2@@Z` is the original protected, non-const, thiscall member. Both original name sets contain the exact spelling. The original particle_system_database procedure records recover `file`, `info`, `script`, `parser`, `emitter_handle`, `pemitter`, `emitter_script`, `emitter_parser` and `context`. Native Serialise at00ae90c3 calls the assigned Load; CreateEmitterTemplate, Load and Serialise have the same order in the donor. Native Load ends with ret8 at00ae9089; its828-byte extent contains one complete function.

# Original headers and layout

The complete original CParticleEmitterDatabase header retains SymbolMap+0, UsingSymbolMap+0x24, Emitters+0x28, EmitterBank+0x38, CurrentHandle+0x40, ParFilename+0x44 and HeaderFilename+0x48, total0x4c. Original TPI3eb4b names/types, native constructor00ae8bf0 and Load member operations independently establish that layout. Original CSymbolMap retains its CBaseClassNonCopyable base and every original member; untouched SLONGMap and NamesUsed occupy their proven16-byte and12-byte container storage. These members are documented opaque storage, not substitute container implementations.

The real CVectorMap<long,CCountedPointer<NParticleEngine::CParticleEmitter> > uses original STLport copy/destruction algorithms. Native00ae9cb0 performs pair copying, including counted ownership;00ae9d50 destroys that same element type. Original STLport4.6 algorithms, template arguments recovered from the original owner, and the caller's element stride establish their emitted names. No donor STL implementation address is treated as a native binding.

The source uses the existing complete CParticleEmitter and CEmitterDatabaseSerialiseInfo headers. The latter is the original pair of symbol-map references. DatabaseSerialise at00aef1f0 is proven by its exact PDB name, donor order and the receiver/info/context call contract; the older GenerateHeader binding is wrong.

# Parser and string details

The visible CStringParser constructor is the genuine original constructor taking const CCharString&, fastcall error callback, filename and base-line offset. Original procedure locals and native constructor00404720 prove TempCharString assignment followed by Init. Its body naturally remains outlined for the outer parser and expands for the inner parser; no forwarding overload or inline-depth directive is used.

Init at009b9530 takes const char*, a fastcall callback, const CCharString& and long. LoadFromFile at0099f2e0 takes CAFile&, CASuspendableProcess* and long. Exact PDB public spellings and native parameter use prove both signatures. SkipPastString009b9ba0 and SkipPastWholeString009b9c00 are distinct original methods, as are ReadAsStringUntilString009ba330 and the different UntilPastString helper used by another row. Original protected CCharString::NullChar and the full CStringParser virtual header establish the data operands at0129aaf4 and0122d874.

The selected original GetAsCharArray implementation returns NullChar on the null branch, then returns the string data. Changing this branch shape resolves the only eight instruction-diff lines in the first successful build. No variable or function name was invented to obtain it.

# STLport destructor correction

004046b0 is the original std::_List_base<CStringParserCommentBoundaries,std::allocator<CStringParserCommentBoundaries> > destructor, which clears the elements and frees its sentinel. Native00404710 is the separate derived std::list destructor jump thunk to004046b0. Original STLport4.6 _list.h and native cleanup/thunk bodies prove the distinction. This corrects the former list-destructor label and is also applied to this pass's009bc120 row. Donor PDB list symbols from a different library implementation are not evidence for the native implementation-specific base name.

# Verification and preserved evidence

The selected source is strategy2, grade3; grade4 was a diagnostic destructor spelling experiment and was not selected. Fresh main grades5 and6 reproduce828/828 with exactly64 relocated operands. Every complete string literal, target/addend and emitted symbol was reviewed. Twenty emitted game/row spellings appear in the supplied original PDB name sets; implementation-specific STLport symbols and the two non-function symbols have the separate evidence above. All wrong older bound spellings found are recorded in NAME_FIXES.tsv.

The self-contained fixture pastes the production Load and parser constructor. It tests72 cases across emitter counts, prior database state, header presence/location and relative/absolute header paths. Assertions cover clearing, handle reset, load arguments, template creation, nested persistence defaults and info identity, and final header path. Real VC7.1 RTM with /GR- and GNU64 C++03/no RTTI both pass. Dependency implementations are test boundaries, not production substitutions.

Internal evidence: evidence/00ae8d50/{ORIGINAL_TYPES,EXACT_NAME_SETS,OPERAND_CANDIDATES}.json; original procedure-local records; native constructor/Load/Serialise and helper disassembly; immutable grade3 and main grade6 source/header/object snapshots; behavior_tests/00ae8d50/a5da038da36c/RESULT.json. Native bytes and binaries remain internal and are excluded from the hand-back.
