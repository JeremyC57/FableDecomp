# mkq30_notes.py (exec'd by mkq30.py): per queue row (1-based index into QUEUE_W30_input.tsv, header excluded) outcome / note for
# wave 30; rows not listed are carried (not reached).
_d = lambda *f: sorted(set(x for x in sum([_rows(x) for x in f], [])))
CST_DEF = ('deferred this wave (runs/cst_deferred.tsv): the CScriptThingRuntime / CScriptThingStored and CScriptThingCopy / CScriptThingParam / '
           'InlineCopy two-view rows (one view with an inline copy, one with the out-of-line 0x004ABE90 / 0x004AA840: retail had one class, so the '
           'header model needs a per-site lever), the ScriptMethods rows whose own signature needs a second fix (NativeCharString: fable_string.h '
           'lacks the out-of-line ??0CCharString@@QAE@ABV0@@Z; ScriptVector: the retail std::vector<CScriptThing>&; CThingNative : CThing / '
           '_quarantine CThing models) and the quest-script ScriptMethods rows (0x00CDFF50, 0x00D1E310, 0x00D408B0, 0x00D864B0, 0x00D9C2B0, 0x00F00AE0)')
EDX_I = {
 13: ['0052d5c0', '0052d770'], 23: _d('cst', 'csm'), 32: ['00569c30'], 33: ['00569c30'], 34: ['00569940', '00569c30'], 47: ['00b43bb0'],
 66: ['00a9d3a0'], 67: ['00a9d3a0'], 68: ['00b09e40'], 69: ['0051ed60', '0057abd6', '0057ae53'], 71: ['006868e0', '00713740', '007f0890'],
}
LEFT_I = {13: [], 32: [], 33: [], 47: [], 66: [], 67: ['00aa8490'], 68: [], 69: [], 71: []}
N_I = {
 13: ('respelled', 'family stl: own row 0x0052D5C0 is the STLport ??1?$_List_base@W4EChangingStateComponentState@CChangingStateComponent@NUISystem@@...'
      '@std@@QAE@XZ (was the free __fastcall ?SortTreeRecursively_00436900@@ stand-in); the ??_G 0x0052D770 calls it through the one-word '
      'std::list<EChangingStateComponentState> of the landed 0x0052CCF0 model (was ??1CStateList@NUISystem@@QAE@XZ). Left: 0x00549A00 '
      '(~CDraggableInto stand-in, queue row 12)'),
 23: ('respelled-partial', 'families cst (15) + csm (18): the ~CScriptThing binders on engine/CScriptThing.h\'s retail class (virtual '
      '~CScriptThing -> ??1CScriptThing@@UAE@XZ): the local non-virtual stand-ins (QAE, 10 of 11), the derived second views ScriptThingCopy / '
      'NScript::ScriptThing / Row00e15500::Handle, Row00e1e530::Handle, and 18 CTCAIScratchpad / script-API ScriptMethods rows re-authored on '
      'the retail CGameScriptInterface (own rows now ?X@CGameScriptInterface@@U..., was the invented CGameScriptInterfaceMethods; the landed '
      'sibling 0x008A1E50 is the model; CThing::GetScriptThing takes its PDB const). 33 of 65 binders; ' + CST_DEF),
 32: ('respelled', 'family lhn: own row ??1LHHttpHeaders@LHNetworkLib@@QAE@XZ (Ego_r + FableWin; was the descriptive CBlockVectorOwner_00569c30): '
      'Reset 0x0083FBC0, HeaderString freed, the tail into ~vector<std::string> 0x00569940'),
 33: ('respelled', 'family lhn: the only wrong namer (own row 0x00569C30, ?Clear@CBlockVectorOwner_00569c30@@) now calls '
      '?Reset@LHHttpHeaders@LHNetworkLib@@QAEXXZ'),
 34: ('respelled-partial', 'family lhn: own row 0x00569940 is the STLport ~vector<basic_string<char> > (was CRetailVector_00569940::'
      'Destroy_CRetailVector_00569940) and 0x00569C30 calls it by that name. Left: the LHStringVector rows 0x00561450 .. and 0x008410D0 '
      '(CRetailStringVector): per-row STLport basic_string / vector models (not reached)'),
 47: ('respelled', 'family stl: own row 0x00B43BB0 is the STLport ??1?$_List_base@PAVCEngineMap@@...@std@@QAE@XZ (was the free __fastcall '
      '?SortTreeRecursively_00436900@@ stand-in; the ~CEngineWorldMap 0x00B42E70 TU emits the same COMDAT)'),
 66: ('respelled', 'family c699: own row 0x00A9D3A0 is the implicit ??1C3DMeshFilePrimitiveChunk@@UAE@XZ (FableWin.pdb donor pair with '
      '~C3DMeshFileClothChunk; sole caller ??_G 0x00ABEEB0 of vtable 0x0129ED74), modelled as the sibling 0x00A9D3C0; was "CTCInventoryItem::'
      'OnDie" over an invented BaseItem'),
 67: ('respelled-partial', 'family c699: the wrong binding ?OnDie@BaseItem@@QAEXXZ (from 0x00A9D3A0) is gone -- 0x00A9D3A0 now calls '
      '??1CChunkedFileSuperChunk@@UAE@XZ. Left: the own row 0x00AA8490 still defines the invented CChunkedFileSuperChunk_Retail / '
      'CChunkedFileChunk_Retail / CRetailArray model (not reached)'),
 68: ('respelled', 'family stl: own row 0x00B09E40 is the STLport ??1?$_List_base@VCSpriteTrailEntry@CEnginePrimitiveSpriteTrail@@...@std@@QAE@XZ '
      '(was the free __fastcall ?SortTreeRecursively_00436900@@ stand-in; the ~CPSCSingleSprite 0x00B0A160 TU emits the same COMDAT)'),
 69: ('respelled', 'family c699: own row ?PeekPThingWithUID@CThingManager@@QBEPBVCThing@@_K@Z (both PDBs; the const twin of the landed '
      'GetPThingWithUID 0x0051ED80; was the free __fastcall "CStopWatch::GetTicks"); its two const CTCHeroStats binders 0x0057ABD6 / '
      '0x0057AE53 call it through the PDB const GetThingManager() returning const CThingManager& (was CWorld::GetThingFromUID)'),
 71: ('respelled', 'family c699: own row ?OnDie@CTCBase@@UAEXXZ (both PDBs; slot 13 = OnDie; was C006868e0::OnDie); its binders are '
      're-authored as their PDB names: 0x00713740 ?OnDie@CTCScriptedControl@@UAEXXZ (was Leaf vfunc_13, catalog OnRemove) and 0x007F0890 '
      '?OnDie@CTCKrakenTentacle@@UAEXXZ (private KillRippleFX + RemoveObstruction; was the free Dest_val with Dtor_first / Dtor_node)'),
}
