# mkq28_notes.py (exec'd by mkq28.py): per queue row (1-based index into QUEUE_W28_input.tsv, header excluded) outcome / note for
# wave 28; rows not listed are carried unchanged from the wave 27 queue (not reached).
_d = lambda *f: sorted(set(x for x in sum([_rows(x) for x in f], [])))
EDX_I = {
 8: ['004091c0'], 11: ['007345d0'], 19: ['00713660', '00713850', '00714900'], 20: ['006a5230'],
 27: ['009d4a20'], 28: ['00559600', '0055a3e0'], 30: _d('sfb'), 36: ['00ed7db0'], 38: _d('esb'),
 50: ['009b05f0'], 51: ['00b81e20'], 53: ['00bdfa10'],
}
LEFT_I = {
 3: ['006bc4a0'], 8: [], 11: [], 19: ['005359a0'], 20: [], 27: ['00a99840', '00a9ba70'], 28: [], 29: [], 30: [], 31: [],
 32: [], 33: [], 34: [], 35: [], 36: [], 37: [], 38: ['00ce0ec0', '00d2d900', '00e5de50', '00ee1be0'],
 47: [], 48: [], 49: [], 50: [], 51: [], 53: [],
}
C696 = 'own row landed at chunk 696 with this spelling'
N_I = {
 3: ('respelled-partial', '[carried] family vl2 (wave 27); left: 0x006BC4A0 -- HOLD (DECISIONS_W27 D7: owning class unknown; brief)'),
 8: ('respelled', 'family d3 (DECISIONS_W27 D3): 0x004091C0 LoadFileList on STLport vector<CWideString, allocator> / vector<CDateAndTime, allocator>: '
     'erase 0x00412450, grow-insert 0x00412330 (this spelling) and 0x004125A0 (the protected __false_type member, proven from the bytes) on '
     'their bound spellings; the CDateAndTime erase\'s __copy 0x0040E350 on 0x0059AEAF\'s template spelling (0x0040E350 itself is a folded copy: '
     'held). Rows 0x004125A0 (own Leaf, allocator + IAE) and 0x0048608C (IAE call) moved with it'),
 11: ('respelled', 'family d4 (DECISIONS_W27 D4 / W24 D7): 0x007345D0 is the CTCCombat member and reaches the world through the protected '
      'CTCBase::PeekWorld 0x00686A80 (was CTCCombat_Methods::GetWorld) -- no junk binder of 0x00686A80 is left'),
 19: ('respelled-partial', 'family d5 (DECISIONS_W27 D5): 0x00713660 / 0x00713850 / 0x00714900 nest CActionBase in CTCScriptedControl (the PDB nesting; '
      'NTCScriptedControl kept for the derived actions, which the PDB puts there) and call the c695 _List_base::clear spelling through the STLport '
      'list<CCountedPointer<CTCScriptedControl::CActionBase> >. Left: 0x005359A0 -- HOLD (identical-code folding, DECISIONS_W27 D5)'),
 20: ('respelled', 'family d6 (DECISIONS_W27 D6, second source found): 0x006A5230 is ?InitialActivate@CThingPhysical@@MAEXXZ -- slot 9 of vtable '
      '0x01244E84 (RTTI .?AVCThingPhysical@@), the slot CThing\'s vtable fills with CThing::InitialActivate 0x004C9690; body = the base call then '
      'AddToPhysicalList(*this); Ego_r / FableWin name set. Own row re-authored (RENAMES); it calls this spelling'),
 26: ('deferred', 'not reached: the nine invented spellings sit in ~66 rows, each with its own CScriptThing model (local non-virtual class, '
      'CScriptThingRuntime / ScriptMethods / ScriptThingCopy structs over engine/CScriptThing.h, X_ handle classes); the virtual destructor changes '
      'each model\'s layout, so per-row work (the Runtime / ScriptMethods groups are the next mechanical sets)'),
 27: ('respelled-partial', 'family peek: 0x009D4A20 calls PeekData on CDefStringTable::StringStream (own Leaf the Ego_r / FableWin const '
      '?GetStringCRC@CDefStringTable@@QBEKJ@Z). Left: 0x00A99840 / 0x00A9BA70 -- every external symbol of both rows is an invented X_<addr> '
      'spelling (ctor / dtor / GetLength / PeekData / WriteULONG / SaveChunkedFile): a whole-row re-model with 0x00996B30 (queue row 16), not reached'),
 28: ('respelled', 'family clk: 0x00559600 (CScrollBar::Update, which skips CDraggable\'s override) and 0x0055A3E0 (CDraggable::Update, own Leaf the Ego_r / '
      'FableWin virtual) call the base CClickable::Update qualified'),
 29: ('done', 'the only namer is 0x0055ACB0 (chunk 696), which already spells it; no binding before'),
 30: ('respelled', 'family sfb: the 150 CSpawnedFunc<T> deleting-destructor rows called it as the free __fastcall ?AutoTinyCompositeDestructor@@YIXPAX@Z; '
      'each now calls CSpawnedFuncBase::~CSpawnedFuncBase qualified (a direct thiscall, `this` in ecx as before). The CSpawnedFuncBaseRuntime '
      'declarations are unbound (virtual calls): queue row 40'),
 31: ('done', 'its binders (0x00CDD4C0 and the definer 0x00E95F10) already spell it'),
 32: ('done', C696 + '; no binder'), 33: ('done', C696 + '; no binder'), 34: ('done', C696 + '; no binder'), 35: ('done', C696 + '; no binder'),
 36: ('respelled', 'family bard: own row re-authored as the compiler\'s ??_GCTB_Bard@CV_TalentlessBardScript@NScript@@UAEPAXI@Z (emitted beside the vtable '
      'of a construction, the 0x008C37C0 idiom), calling ~CTB_Bard 0x00ED7DD0 by its chunk 696 spelling. Was the free CLandscapeBackgroundPatch '
      'stand-in (RENAMES)'),
 37: ('done', 'binding already correct; binders 0x009FE6C0 / 0x00B2B910 / 0x00B2C400 spell it'),
 38: ('respelled-partial', 'family esb: the 41 rows calling the free __fastcall ?AutoTinyCompositeDestructor_00f35b40@@YIXPAX@Z call '
      'CEntityScriptBase::~CEntityScriptBase qualified. Left: 0x00CE0EC0 / 0x00D2D900 / 0x00E5DE50 / 0x00EE1BE0 bind ??1CFrontEndScreen@NUISystem@@QAE@XZ '
      '(each models the entity on a local CFrontEndScreen class: per-row, not reached)'),
 39: ('deferred', 'not reached: the twelve invented spellings sit in 21 rows, each a bespoke derived-destructor model whose base is a non-virtual local '
      'class (Base / BlendBase / CBase_00686830 / FableDestVal outer forwarders): every row needs the polymorphic CTCBase model with its own vtable '
      'store checked'),
 40: ('deferred', 'not reached (coordinator decision 2): 8 of the 14 sources declaring virtual ~CSpawnedFuncBaseRuntime also include '
      'engine/CSpawnedFuncBase.h, so renaming the local class to CSpawnedFuncBase collides with the header class; do it with the header fix '
      '(row 41), the local models then deriving from the header class'),
 41: ('deferred', 'not reached (coordinator decision 3): engine/CSpawnedFuncBase.h has 394 landed includers (every one rechecked after the change); '
      'its own family in wave 29, together with row 40'),
 45: ('done', 'no binder; own row landed at chunk 697 with this spelling'),
 47: ('done', 'no binder; own row landed at chunk 697 with this spelling'),
 48: ('done', 'the own row (chunk 697) and its binders 0x00B59840 / 0x00B59BE0 spell it'),
 49: ('respelled', 'family c697: 0x009B05F0 CheckDependencies\' checksum file is CChecksumCacheFile (ctor 0x00A60C90 / Open 0x00A60D40 / dtor on their bound '
      'spellings, merged with its GetChecksum class). Was a separate CChecksumFile'),
 50: ('respelled', 'family c697: own row renamed from the descriptive ClearSimulations to the Ego_r / FableWin Clear (the binder 0x00BEBC50 already spells it)'),
 52: ('respelled', 'family c697: 0x00BDFA10 nests the class in CLocalDetailCacheMap (was a global QuadTreeElement)'),
 42: ('deferred', 'not reached (chunk 697 appended late): 12 rows (the C3DMeshObject subclass destructors 0x004BC4F0 ..) name it; per-row virtual models'),
 43: ('deferred', 'not reached (chunk 697 appended late): ??1ScriptResource@@QAE@XZ (5 rows) and ?Destroy@CScriptGameResourceObjectScriptedThingBase@@QAEXXZ '
      '(11 rows) are per-row script-resource destructor models'),
 44: ('deferred', 'not reached (chunk 697 appended late): the QAE form (3 rows), ~CScriptGameResourceObjectMovieFull (3), Destroy (8) and DestroyOutOfLine (10) '
      'binders are per-row script-resource models'),
 46: ('deferred', 'not reached (chunk 697 appended late): 4 rows bind the non-virtual QAE form, plus @FableMemberDtor_00550a00@4 / @SymbolPair_Dtor@4 '
      'stand-ins (0x00550A00, 0x004C3360); each needs the virtual model of its local C3DAnimationInterpolator'),
 51: ('deferred', 'not reached (chunk 697 appended late): 0x00BACD50 (QAE form) and 0x00BC67D0 (free _Destruct stand-in) need the virtual model'),
 53: ('deferred', 'not reached (chunk 697 appended late): the own row (catalog CBlockVectorOwner_00569c30) carries the LHNetworkLib class, its Reset 0x0083FBC0 '
      'and the STLport vector<std::string> dtor 0x00569940: one re-author with rows 54 / 55'),
 54: ('deferred', 'not reached: the only wrong namer is the own row of 0x00569C30 (row 53)'),
 55: ('deferred', 'not reached: 0x00569C30 (row 53), 0x008410D0 (CRetailStringVector) and the LHStringVector rows (0x00561450 ..) -- the STLport '
      'basic_string / vector templates per row'),
 56: ('deferred', 'not reached (chunk 697 appended late): destructor-family stand-ins, as the c690 0x0099A300 row'),
 57: ('deferred', 'not reached: carried with the c690 0x0099A300 row (queue row 7)'),
 58: ('deferred', 'duplicate of row 51'),
}
