# retry1 (wave 1) -- notes
Rows touched: 247 (STATUS.tsv is the complete record; anything not in it was not attempted).
Status counts: DIFFER 61, GAP 11, HELD 2, IMPROVED 46, MATCH 31, RELOCATION_MATCH 83, RELOCATION_MATCH_NEW 10, SKIP 3.
Payload: payload.json / oracle_final.tsv hold all 124 matched rows (MATCH + RELOCATION_MATCH + RELOCATION_MATCH_NEW). The retail_asm rows are in the same payload (vland grades them retail_asm via crawl/purity.py); no separate retail_asm payload was needed. oracle_extra.tsv holds the new-row oracle lines (the five vorbisfile callees and other NEW rows).

## Per-lever win tally (matched rows)
| lever | wins |
|---|---|
| same-TU static, private convention (static body + its real caller in one TU; C TUs /Gr where retail is fastcall) | 88 |
| same-TU callee bodies defined first (VC7.1 callee register tracking / mod-ref) | 22 |
| retail_asm: landed engine helpers (CeilToLong/FloorToLong/FloatToLong verbatim) or the original asm (wxutil memmove, MMX/SSE2 mix kernels) | 10 |
| parameter / fastcall-callee declaration order | 3 |
| named shared local (x87 order) | 1 |

Improved (not matched) rows use the same levers plus: the in-place box (OverlappingArea), the GetCrime()/CInterfaceMap::Find accessors, STLport spellings, retail statement order, named fraction locals (constant folding), vector operator[]/size().

## retail_asm carriers (matched)
00c5d1f0 00c5bef0 00c6d410 00c5d2e0 00cb0310 00516830 00516200 005115e0 00511a10 00774610

Each carries a `// Retail-asm: <address> <instruction>` line verified against the retail bytes; no naked/_emit/ret-in-asm.

## HELD
- 00b05e10, 00b1ad90: bytes RELOCATION_MATCH, held -- per-TU triplicates of the landed CPSCLight::SmoothFadeValue 0x00AF9000; one mangled name cannot bind three addresses (owner decision needed: per-TU static spelling or keep parked).

## Improved rows (best source in differ/<addr>.cpp, // Grade: line = residue + next step)
- 00c95580 (1 grades): 901v910: frame-slot colouring of the codes/codep loop; needs the full sharedbook.c TU (_make_words/_book_unquantize statics)
- 00c9bc10 (2 grades): 30v30: drftf1 gets c in ESI / ifac in EDX vs retail EDI / ECX; needs dradf2/4/g bodies in the TU
- 00a10a60 (1 grades): 281v281 (was 301v281): prologue register colouring, tokens in ebp vs eax
- 00a10b80 (1 grades): 458v450 (was 476): this/length register colouring ebx<->esi
- 009c33a0 (3 grades): 313v315: L"." temporary homed in a frame slot vs retail's dead path-argument slot
- 009c38c0 (1 grades): 409v409: GetFileTime/SetFileTime argument scheduling
- 00c74ee0 (3 grades): 96v95: shared return-true block placement
- 00c62370 (3 grades): 124v118: r spilled to the dead radius slot vs kept on the x87 stack
- 00ac7750 (3 grades): 497v495: commutative operand order in 2 determinant products + temporaries' frame slots
- 00c6be60 (3 grades): 226v226: only the column -= 20 placement relative to the call
- 009d5a30 (3 grades): 89v89: empty tag in its own frame byte vs retail homed on the bank arg slot
- 00a5a9d0 (3 grades): 209v203: the X dot product still lands in a separate local r (frame 0x14 v 0x0c); retail builds it in the push_back temporary
- 0097af70 (3 grades): 142v142: the argument-area pointer is now eax as retail; one residue: retail loads words 0 and 1 (ecx, edx) before sub esp,10h, ours loads word 1 after the first store
- 00c1bf00 (3 grades): 172v174: up and forward swap edi/ebx (retail up=edi); the two 1.0 diagonal stores go before the [0] store (retail after)
- 00c1c330 (3 grades): 192v192: only the x87 stack order in the second transform: ours keeps y on top after the mirror test (fld st(0) for the Row[1] terms), retail z (fld st(1)); the term-pair adds then pair differently
- 0041db84 (3 grades): 276v281: frame 4 v 0xc -- the three CCharString temporaries (dialog name, child name, text symbol) share one frame word [ebp-4] where retail gives each its own (-4/-8/-0xc)
- 00c755b0 (2 grades): 178v178: callee-saved colouring only -- retail keeps this in edi (reused as the stage pointer, add edi,40h) and config in ebp, counters in ebx; ours this in ecx (spilled), config in ebx, counters edi/ebp
- 00c76ab0 (1 grades): DIFFER(171v175): the truncation block now matches; left is callee-saved colouring around the inline memset (retail keeps pMemory in ebx from before the if and length*4 in ebp; ours reloads pMemory and keeps length*4 in e
- 00c76dd0 (1 grades): DIFFER(182v186): the truncation block now matches; left is callee-saved colouring around the inline memset (retail keeps pMemory in ebx from before the if and length*4 in ebp; ours reloads pMemory and keeps length*4 in e
- 00c6ebe0 (3 grades): 106v106: ramp pointer colour ecx (retail) vs esi (ours) in both arms + the two stores; member-function helper not inlined (88v106)
- 00c77b30 (3 grades): 190v193: retail keeps params in ebp (extra push ebp) and runs two pointer IVs esi/edi over state+0x38/+0x3c with the table offset from 4; VC7.1 here keeps params in ebx and one IV edx
- 00b84720 (3 grades): 146v142: fistp block now byte-equal; residue: sub esp,8 scheduled after the first fld in retail, push_back copy reloads SubPrimitive from the escaped frame temp (retail stores esi), manager through eax (retail ecx), __fa
- 006378e0 (3 grades): 108v106: only the level search -- retail loads g_NoMapwhoLevels into edx and runs an unrotated loop with no bound compare (cmp esi,[ecx] ; jl ; inc ; add ; jmp); every C form rotates it or keeps the bound test
- 0057b26f (3 grades): 156v156: only the GetInterface key slot -- retail homes it in the dead fraction slot [ebp+8], VC7.1 here in the helper result / 0.0f temp slot [ebp-8] (frame-slot sharing order)
- 00b947e0 (3 grades): 251v246: the fistp block is byte-equal; residue: retail holds span in esi from the helper read (push esi in the prologue, inc esi in place for HighestSetBit), VC7.1 here keeps span in eax and pushes esi on the slow branc
- 007e2df0 (3 grades): 252v257: the asm tail and frame now match; residue: retail assigns ParentRHSet through a pointer copy (lea ecx,[esi+0Ch] ; mov edx,ecx ; six dword moves through edi, push edi) and then reads the set copy through ecx; VC7
- 00f36540 (3 grades): 321v321: both floor blocks and the frame match; residue: the hash sum's register roles -- retail lea edx,[eax+edi] keeps h0 in edx and masks a copy in eax (inc edx in place for the +1 corner), VC7.1 here add eax,edi and 
- 00b94bf0 (3 grades): 297v311: the floor blocks now assemble, but VC7.1 packs the first expansion's slots differently (retail: value [ebp-0Ch], result in the dead position slot [ebp+8]; ours half at [ebp-0Ch], result [ebp-4]) and the area box
- 00b84580 (3 grades): 349v340: both fistp blocks byte-equal; residue (shared with 0x00B84720): push_back's element copy reloads SubPrimitive from the escaped frame temporary (retail stores esi), the __false_type temp slot, and eax/edx rotatio
- 00b1bc80 (3 grades): 282v280: code shape now retail's (loop, two fistp blocks, both multiplies); residue: frame 0x14 v 0x10 -- retail homes the fistp result at [esp] and reuses slots, VC7.1 here gives the result its own word
- 00b18820 (2 grades): 297v304: the fistp block matches; residue: the early-return block laid at the end in retail (je far) vs inline here, LightLifeTimer kept in ecx (retail) v edi + push edi, and retail's fild timer ; fmul st(1) with the int
- 005ee77c (3 grades): 259v267: fistp and the interface hit test now match; residue: retail converts Morality through eax into the dead ability slot (mov eax,[ecx+28h] ; mov [ebp+8],eax ; fild [ebp+8]) where VC7.1 here filds the member directl
- 006b2960 (1 grades): 250v250 (from 231v250): every instruction now in retail's order; residue: frame 0x38 v 0x30 -- retail homes the first two helper values in the dead box slot [ebp+8] and the floor helpers' 1.0f there too, VC7.1 here gives
- 00654438 (2 grades): 347v347: one x87 operand choice left -- retail duplicates the width (fld st(1) ; fadd [pos.X]) for the box's right edge, VC7.1 here loads pos.X (fld [pos.X] ; fadd st(2)) whatever the sum order
- 0066cd50 (3 grades): 382v423: fistp block and colour shuffles now match; left: retail keeps two size slots (inner-loop guard) and recomputes the cell index after the __ftol2 calls, ours CSEs both (same as w3 with width/height locals)
- 00439bb0 (3 grades): 592v592: frame and fistp match; left: eax/edx/ecx rotation of the PMoney/PeekGuiDef loads in the UpdateStat args and the uninitialised-bar stores (unnamed float conversion changes nothing)
- 00b51ad0 (3 grades): 965v969: the three floor blocks, the snap (two stores) and the unrolled 9-entry fill match; left: retail runs the outer loop on a separate down counter (edx=9, row pointer +0x20C, column in [ebp-4] via edi) and keeps ebx
- 0056ef2d (3 grades): 66v66: list address in eax and first in esi now match; left: retail loads statIndicatorMax into edi right after the first compare and reuses eax for setl, the lane loads it late into eax and takes ebx for setl
- 004385f0 (3 grades): 379v379: the GetCrime() accessor pins the bar pointer in ebp before GetNormalisedTimeLeftOnFines as retail; left: the same one-step eax/edx/ecx rotation as 0x00439BB0 in the UpdateStat argument loads and the initialise b
- 00f36010 (3 grades): 444v442 (mnemonic similarity 0.17 -> 0.55): both floor blocks match; left: frame packing (0x24 v 0x38, an ebx push) and the hash/gradient load interleave -- retail copies only X/Y of g10/g01/g11 and fetches g00 through a
- 00a88d10 (3 grades): 182v196: the fistp blocks and both multiplies match; left: the lane merges TableCos's position parameter with the caller's local and CSEs position-0.5 across the two calls; retail keeps the parameter as its own frame wor
- 007727e0 (1 grades): 467v433 (base 497v433; mnemonic similarity 0.27 -> 0.35): the ceil block is reproducible; residue is the def-pointer handling -- retail holds the thing def as a raw counted pointer in ebx with inline release paths and an
- 00a76e00 (3 grades): 298v298: every instruction matches; left: frame packing only -- retail reserves 0x28 (corner.Y at [ebp-24h], rel.X at [ebp-18h]), the lane 0x20 ([ebp-14h]/[ebp-20h]); the vector temporaries' frame homes differ
- 00914650 (3 grades): 341v340 (mnemonic similarity 0.75 -> 0.84): the ceil block, the interface select and the tail match; left: frame 0x38 v 0x30 (CeilToLong's locals one slot lower) and the order of the four box stores
- 00b16220 (3 grades): 372v372 (mnemonic similarity 0.75 -> 0.92): fistp, both pointer IVs and every lerp but one match; left: the Type select's address is [edx+edi] (retail [edi+edx]) and the Radius lerp evaluates this*s before other*t (retai
- 006cc800 (3 grades): 373v399 (mnemonic similarity 0.48 -> 0.74): the ceil block and the first stagger match; left: retail keeps clear()'s erase alive (cmp eax,eax on two loads of _M_finish, then memmove) where the lane folds last == _M_finis

## Names changed (per row; two sources each unless marked descriptive)
- 009c33a0 parked caller spelling MakeBackupName -> GetMarkerFileName (one address, one name)
- 00c1c3f0	CPhysicsMeshObject::GetHelperPointPositionStatic -> TransformHelperPoint_00c1c3f0 (descriptive): PDB GetHelperPointPositionStatic takes (const CCharString&, long) const; reloc_bindings binds it to 0x00A6C9C0
- 00ac7750	CQuadric members A2/B2/C2/D2 -> AA/BB/CC/DD (+Weight), Ego_r.pdb struct_layouts
- 00ac70f0	parked name CQuadric::GetMatrixPart -> GetInverseMatrixPart: body is the cofactor inverse (1/det); PDB declares both ?GetMatrixPart and ?GetInverseMatrixPart with the same signature; the caller 0x00AC7750 already spells GetInverseMatrixPart
- 00c6be60	callee LookupDirectionIndex -> GetFrequencyTableEntry (the landed name at 0x00C6BDC0)
- 00c17260	CAIStateGroup_MeleeCombatNearbyBase::UpdateCurrentAction (propagated from 0x0093D140) -> LoadRecordAndReport_00c17260 (descriptive)
- 00c17400/00c16ce0: types CScriptObject->CLoadLog, CTrackNodeArgs->CScriptHost, CTrackNodeExtra->SProcessorSettings_00c16ce0, CWaypointData->CBankData, CWaypointHolder->CAutoDeletePointer (landed 0x00C17740), Report->CDebugStream::operator<< (landed spelling), CProcessor_00c16ce0->CBankProcessor (landed)
- 00780d80 (not landed): parked CSoundDef model of GetSubDef 0x0065F604 is landed as CDefClassBase::GetSubDef<CEntitySoundDef>
- 00c16c10: CString_00c16c10 -> CString (landed 0x00C0BFE0/0x00C0BA40), CSegmentFile_00c16c10::OpenSegment int -> CSegmentFile::OpenSegment long (landed leaf)
- 00c16d90: SProcessorSettings_00c16d90::ProcessEntry (member) -> static ProcessEntry(settings, ...); CEntry_00c16d90 -> CBankData (ctor 0x00C2EED0, dtor 0x00C2EF30 landed CBankData); CBankFile_00c16d90::IsOpen -> CLoadLog::IsOk (landed 0x00C307C0)
- 0041db84: new same-TU static GetTextBySymbol 0x0041CFB2 (g_pMainGameComponent 0x013B86A0 + PTextBank per Ego_r CMainGameComponent +0x14; g_emptyWideString 0x013BCA24; CWideString::CreateFromCharString 0x0099B6A0; NGameText::CDataBank::GetTextBySymbol 0x009C95E0)
- 00c5d1f0/00c6e290/00c6ed70: ScaleAccumulate16Ramp parameter order (step, initial) -> (initial, step); the function becomes a file static (mangling ?ScaleAccumulate16Ramp@Row<addr>@@... internal)
- 00c5bef0/00c6aae0/00c6f1a0/00c6f800/00c6fc80: CUnprovenMixKernel_<addr> kept; parameters retyped (short start, const __int64* source, int quadPairs, __int64* accumulator, short step) -> (const short* src, unsigned long count, long* dst, short initial, short step) to match the stack/ecx use
- 0057b26f: CTCBase::GetDefinitions->GetDefinitionManager, CDefinitionHolder->CGameDefinitionManager, CTCStaminaListener::OnStaminaExhausted->CTCPhysicalShieldSpell::CancelMove (landed sibling 0x0057B1DB spellings)
- 0072aaf0: CThing::AddTC third parameter CTCParamsBase* -> const CTCParamsBase* (PDB ?AddTC@CThing@@QAEPAVCTCBase@@ABVCCharString@@_NPBVCTCParamsBase@@@Z, landed 0x0072DD60); struct->class for CTCParamsBase/CCharString
- 00516830/00516200: CThing/CThingPhysical/CThingAICreature/C3DVector/CThingFilter_IsInQuestOrNullQuest struct->class; CTCPhysicsBase::GetBoundingInfo(C3DVector*, float*) const -> (C3DVector&, float&) const = Ego_r.pdb ?GetBoundingInfo@CTCPhysicsBase@@QBEXAAVC3DVector@@AAM@Z; IsInQuestOrNullQuest operator() now binds the landed ??RCThingFilter_IsInQuestOrNullQuest@@QBE_NABVCThing@@@Z
- 005115e0/00511a10: parked CThingFilter_OverlappingArea::operator()(CThing*) -> CFilter_Bind3<...>/CFilter_Bind2<...>::operator()<CThing>(const CThing&) const (unreferenced instantiations; layout = the inlined Bind copies in 0x00516200/0x00516830); 0x006A7C60 now the landed ??RCThingFilter_IsInQuestOrNullQuest@@QBE_NABVCThing@@@Z (was ?? R CThingFilter(CThing*)); GetBoundingInfo PDB spelling
- 00b84580: _M_insert_overflow respelled to the bound ?_M_insert_overflow@?$vector@VCSortElement@CEngineSubPrimitiveRenderer@@...ABU__false_type@2@I_N@Z (was CSortList::_M_insert_overflow with __true_type); 00b84720's differ still uses CArray<>/__false_type -- name debt
- 006b2960: modelled the retail no-arg slot-13 call as CNavRegionSource::GetNavRegion (descriptive; same vtable slot as CNavManager::Query) -- needs a name source
- 004385f0: CCriminalRecord -> CTCVillage; GetCriminalRecord -> CPlayer::GetPNearestTCVillage (0x00488620); IsWanted -> CTCVillage::IsHeroCriminal (0x0065BFCE); GetBounty -> CTCVillage::GetFineAmountForCurrentCrimes (0x0065A41C); GetBountyFraction -> CTCVillage::GetNormalisedTimeLeftOnFines (0x0065A28F, landed); CCrimeStatBar::SetBounty -> SetFine (0x00643FF6, PDB ?SetFine@CCrimeStatBar@NPlayerGui@@QAEXJ@Z, stores CurrentFine); BountyFraction -> NormalisedTimeLeft via inline SetNormalisedTimeLeft; accessor CPlayerGui::GetCrime (PDB). Sources: reloc_bindings.tsv + FableWin/Ego_r PDB names.
- 00774610: map<long,long> copy ctor now the out-of-line ??0?$map@JJ...@QAE@ABV01@@Z (0x00778100, landed leaf ??0?$map@) instead of an inline call to the _Rb_tree copy ctor; operator[] keeps the PDB ??A?$map@JJ... (NAME_FIXES: caller binding ??ACLongMap)
- 006cc800: GFGetCurrentFrame -> CWorld::GetFrame (static __fastcall, PDB ?GetFrame@CWorld@@SIJXZ; reloc_bindings 0065a686)
- earlier in this run: 009c33a0 MakeBackupName -> GetMarkerFileName; g_dwWordWrapFlags -> g_WordWrapOptions (word-wrap rows 00a0fe80..00a10720).

### Descriptive names still in use (need a name source)
- 006b2960 CNavRegionSource::GetNavRegion (vtable slot 13)
- 00c1c3f0 TransformHelperPoint_00c1c3f0
- 00c17260 LoadRecordAndReport_00c17260; SProcessorSettings_00c16ce0 (00c16ce0/00c17400)
- 00c5bef0 family CUnprovenMixKernel_<addr>
- 00c37c40 CCreateEntryVisitor_00c37c40 / CSoundEmitter_00c37c40 (vland RELOC_NAME warns against the label CVtable_012b528c)
- PeekTypeList/PeekAllThings use the landed CThingSearchToolsMethods wrapper spellings; the PDB has ?PeekTypeList@CThingSearchTools@@IBE... / PeekClassList (name debt on 00516200)

## Header debt
- 00cbe583: header debt -- local CScriptThing vtable stub, engine/CScriptThing.h exists
- 005ee77c: header debt -- local CTCHeroStats model, engine/CTCHeroStats.h exists
- 0066cd50: local CRegionMinimap/CSurface stubs; engine/CRegionMinimap.h and engine/CSurface.h exist (no CRGBColour header)
- general: many rows still carry local struct models of classes that have engine/<Class>.h headers (vland SHARED_HEADER_AVAILABLE warnings: CTCPhysicsBase, CCountedPointer, CGameJoystickManager, CManager); not converted in this wave.
