# Identity and selected reconstruction

The selected row is public, non-static `bool NScript::CV_BodyGuardScript::MakeHenchmanComment(const CCharString&, long, EHENCHMAN_COMMENT, const CScriptThing&)`. The exact emitted leaf is `?MakeHenchmanComment@CV_BodyGuardScript@NScript@@QAE_NABVCCharString@@JW4EHENCHMAN_COMMENT@12@ABVCScriptThing@@@Z`; objdump finds it exactly once.

Evidence is independent of the manifest label: Ego_r public at donor00eb4e60 and FableWin public at donor039a0260 give exactly this symbol. Both raw TPI classes and original module scopes corroborate the signature, original parameter names and local names. Fresh native xrefs find18 callers in the BodyGuard translation unit, including the separately assigned idle-comment method00e5e5a0. Calls pass the parent script in ecx and the string/state/enum/specific CScriptThing parameters; native `ret16` agrees.

Original locals retained: `HenchmanID`, `Henchman`, `HenchmanBuddy`, `ResponseAllowed`, `HenchmanMorality`, and separately scoped `HLStyleConversationIndex`. The unused return-token local is not needed to emit any native instruction.

# Complete layouts and access

`CV_BodyGuardScript` derives from the original `CLevelScriptBase<CQ_SunnyvaleMasterData>`, whose Game and MasterScriptData are references. Native accesses independently establish +0x40 Game, +0x44 master data, +0x5c CommentaryTimer, +0x64/+0x68 vector begin/end, and +0x70 ConversationIndex. All original members are preserved through the two final four-element arrays, giving0x94 bytes. The original two PDB class records independently list those fields; the donor base92 shrinks to retail72 and donor vector16 shrinks to retail12.

`CQ_SunnyvaleMasterData` preserves every original member. The five unconsumed enum fields retain their exact original member names as four-byte opaque storage with original type/offset comments. Its native Init00cdba10 writes SingingStonesInSync at+0x140, corroborating the full0x144-byte model and the original BoastRegions vector at+0x134. Native MakeHenchmanComment independently accesses HenchmanCommentaryState at+8. Ego_r and FableWin type records are saved; their different donor sizes reflect donor library/string layouts and are not retail sizes.

The new CCounter has its original four-byte private RelevantWorldFrame, corroborated by BodyGuard idle code at+0x50. CScriptDataBase derives from original CBaseClassNonCopyable with virtual OnPersist and Init; the PDB type records and native master-data Init field accesses prove its4-byte base.

The inherited CScriptThing header had public NullString. Raw Ego_r symbol-stream8 at0x19090 proves `?NullString@CScriptThing@@1VCCharString@@A`; raw TPI proves protected static access. The BodyGuard opt-in corrects this symbol, private PImp, and original private CTimer::TimerIndex while preserving all default branches. The selected seventh build still matches every native instruction. The emitted vtable symbol is independently present in raw PDB globals and the native79-slot table at01238c8c.

# Operands and extent

RELOCATIONS.json checks every one of49 relocation operands against its exact native target. String literals match their native NUL-terminated bytes. All direct helpers have exact original PDB symbols; `_rand` is the native CRT thunk; pDSTGame uses the established spelling. Incorrect old binding aliases are reported in NAME_FIXES.tsv rather than copied. The original CGameScriptInterfaceBase virtual slots used are GetHero70, SetTimer89, AddNewConversation364, AddPersonToConversation365, AddLineToConversation366, IsConversationActive368. CScriptThing virtual IsAlive75 and protected IsEqualTo78 have native vtable offsets0x12c and0x138. These slots and class keys use the complete existing original interface headers.

The authorized1064-byte extent starts with `sub esp,0x24`, has all branch targets inside it, and ends with the complete `ret16` at00e5eff5..00e5eff7. The selected object has exactly1064 bytes; masked comparison has no differing instruction lines. There is no byte padding or inserted native data.

# Behaviour

The self-contained fixture pastes the selected function unchanged and expands only active original header branches. It uses real class interfaces, real CScriptThing reference-counted ownership, real STLport vector bodies, instrumented external string operations and a derived test interface.3888 combinations cover empty/non-empty lists, each of four comment modes, deterministic random speaker/response branches, active-conversation/master-state rejection, absent/same/different requested speaker, dead chosen/requested speakers, dialogue text/participant order, timer/state mutations and ownership cleanup. GNU64 C++03/noRTTI and actual VC7.1 RTM `/GR-` pass. Test stand-ins are limited to external dependencies; the function under test is the production body.

Private evidence paths are under `workers/scripts_b/evidence/`: `initial_types_*.json`, `initial_locals_*.json`, `bodyguard_aux_types.json`, `bodyguard_aux_FableWin.json`, `bodyguard_enum.json`, `CScriptThing_types.json`, `script_static_original_symbols.json`, and `xref_all.txt`. Native disassembly is private and excluded from handback.

The helper parameter names are now also original raw-PDB spellings: CTimer seconds, implicit CScriptThing copy __that, and equality rhs. Raw module evidence is comment_inline_parameter_names.txt and the copied pass12 evidence/function_locals_CTimer_operator=.json. The final build/test source hashes reflect these name-only changes.
