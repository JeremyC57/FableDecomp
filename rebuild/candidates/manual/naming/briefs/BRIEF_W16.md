# Naming pass, wave 16 (one agent beside two chunk agents on the lane)

Owner rules 2026-11-01, in force for every decision: consider how retail did it and recreate it faithfully; and **when a catalog name is
wrong, fix it** -- a proven wrong spelling in a landed source is respelled in this wave, not held. Resolved decisions:
`naming/2026-11-01_wave15/DECISIONS_W15_RESOLVED.tsv` (+ waves 13/14); canonical globals / members in `naming/ESTABLISHED_SPELLINGS.tsv`.
Rules, mechanics and hand-back as `briefs/BRIEF_W15.md` / `BRIEF_W14.md` (read both, the wave 15 README, DEFERRED.tsv, QUEUE_W15.tsv and the
wave 15 `scripts/` -- compose15.py, foldtests.py, fam_*.py, hand15/ -- reused as compose16.py etc.). Labels the chunks already proved are in
`ghidra_out/labels_chunk667..674_2026-10-27.tsv` (the catalog and manifest carry them; the landed SOURCES do not -- this wave's work).

## Scope, in this order (name-only unless noted; every family rechecked against HEAD, one commit per family)
1. **The NAME_FIXES_QUEUE rows c667-c674** (`$S/naming/w16/QUEUE_W16_input.tsv`, 183 rows; a row's wrong_spellings_seen names the landed
   sources to respell). Group by target symbol into families; the big ones first:
   - the ??_G scalar deleting destructors landed as `CLandscapeBackgroundPatch` / free `__fastcall *_scalar_deleting_destructor` /
     `?ScalarDeletingDestructor@` stand-ins (c668-c674: ~60 rows) and their `@*_dtor@4` / `?Dtor@` callee spellings -> the proven
     `??_G<Class>@@UAEPAXI@Z` + `??1<Class>@@UAE@XZ` (name-only where the class model keeps the bytes; a row whose respell moves bytes,
     e.g. a stand-in that now needs the real vptr store, goes to DEFERRED with the diff -- or re-author it when the chunk's landed
     sibling source shows the exact shape);
   - the quest-script ctors landed with `void*` / `CGameScriptInterface*` parameters -> `??0<C>@NScript@@QAE@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z`
     (c671, c672, c673), the `Create_CClass_*` Allocs -> `?Alloc@<C>@NScript@@SIPAVCScriptBase@@...`, and the landed script classes that declare a
     user destructor retail does not have (name-only if the bytes hold);
   - the STLport instantiation spellings: `_List_base<CIntelligentPointer<CThingAICreature>>::clear` 0x00510D40 (11 callers), the
     `_List_base<CIntelligentPointer<C3DAnimation2>>::clear` 0x00A53300, vector<pair<pair<CCharString,CCharString>,CSoundBankLH*>> dtor
     0x00A40BD0, `_List_base<CSoundObjectLH*>` dtor 0x00A3FF60, vector<CQuickDrawTriInfo> dtor 0x009C2F50;
   - the per-row items: IsAcceptButton const (0x00558C10), FadeRain/SnowToTarget private AAEXM (0x00B52B30 binding), Set{TL,TR,BL,BR}Node
     protected (0x00A75CA0..), CWillButtonHelperTimer ctor (EHeroAbility, CThing&) 0x008EDFF0, CTCOpinionSource ctor (CThing&) 0x00771360,
     ClearMemoryBlocks private 0x00A5C600, ~CInitBaseClass 0x0099A410, CInitBaseClass::IsValid 0x0099A330, FreeResource 0x00A62ED0,
     ~CChunkedFileChunk 0x00AA75C0, CIndexBuffer::Uninitialise 0x00AC1EE0, FreeMemoryPage 0x00F37030, ~CEntityScriptBase 0x00F35B40,
     ~CMouse 0x00A66F80, ??0CBankFileAsync 0x009D5F80, SetScriptActiveStatus 0x00CBFAB8, the CrimeText atexit dtor 0x01228ED8,
     CStringParser ctor callback `P6IX0@Z` 0x00404720, the data symbols (0x013B8A38, 0x013B8A44, 0x013B8B10 under the lane TU hash,
     0x0143E910, 0x013B89FC), GFEnterAllocatorLock / GFLeaveAllocatorLock / CMemoryAllocator::GetAllocationCount labels,
     0x00887A30's invented SetBaseWeight helper -> `SetDefaultThemeWeight(weight, true)` (c669 README), the descriptive CSoundBankLH::Unload /
     CASoundManager::ClearBanks / PFrameMemoryPool rows stay descriptive (c670 decision).
   0x00440730 stays on HOLD. 0x00C28690 (libpk CHandleBasedMemoryPool::Free) stays deferred unless you find the libpk class name
   (one mangled symbol cannot link twice; look for a second class / namespace in the Ego_r name set).
2. **Wave 15 DEFERRED.tsv, the name-only remainder**: the family remainders on free-function / destructor-view stand-ins (bo 29, scb 29,
   bcn/bip/cmb, sai 3) where a class-view respell keeps the bytes; 00861d00 OnDeactivate if a polymorphic CModeStandBase model holds
   the bytes; 00634870 (C2011 CPlayer redefinition: fix the TU, name-only).
3. **The mega families, as far as the wave reaches, biggest first** (name-only; each a family commit): ConstantFPS 0x01375550 (314 rows),
   `bitset::test` (239), `CBasicString::Compare` (138), GetPItem (903 left), ~CCharString (135 left), GetSubDef, GetInstance, `Transfer<>`, AddEvent.
Not in this wave: the byte-moving tcb/bip/cmb/cab/bcn rows (re-authoring; DEFERRED keeps their diffs), the ~40 STLport own rows that need
re-authoring (a chunk), VIRTUAL_SCREEN_* / PMainGameComponent (gated), the engine header changes (migration wave 10).

Hand back BINDINGS.tsv, LABELS.tsv, RENAMES.tsv (file renames for rows whose file name carries the old function name: the three wave 15
leftovers CTCPhysicsRigidBody_vfunc75_00759a00, CTCTurncoatSpell_FinalCleanup_007c75e0, CXMVPlaybackInfo_CXMVPlaybackInfo_007f7700 included),
DEFERRED.tsv, QUEUE_W16.tsv (every queue row with its outcome: respelled / respelled-partial / deferred / hold / already-correct), the
per-family rows files, logs and scripts, as wave 15 did.
