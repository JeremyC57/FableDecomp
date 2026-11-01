# mkq17_notes.py (exec'd by mkq17.py): per-target outcome overrides / notes, and skipsum() -- the fam_q skip reasons of a target.
import glob as _g
ABASE = dict((r[1], r) for r in A)
def skipsum(t):
    ls = sorted(_g.glob(os.path.join(W, 'logs', 'q_*_%s.log' % t)) + _g.glob(os.path.join(W, 'logs', 'compose_ov_final_*_q%s.log' % t)), key=os.path.getmtime)
    if not ls:
        a = ABASE.get(t)
        if a and a[5] and a[6].startswith('N'):
            return 'not driven this wave: the own row defines a stand-in (%s) -- own-row re-author needed before the callers' % a[6][2:].split()[0][:90]
        return 'not driven this wave: the remaining rows name per-row stand-in spellings (vtable / data / free-function views) -- per-row'
    c = collections.Counter()
    for l in open(ls[-1]):
        p = l.split()
        if len(p) >= 4 and p[0] == 'skip':
            k = ' '.join(p[2:4]).rstrip(':')
            k = {'free stand-in': 'free-function stand-in (no class to move)', 'no calls': 'no call site found (address bound through a view / data)',
                 'call of': 'qualified / indirect call', 'non-public': 'non-public member reached from an unrelated class'}.get(' '.join(p[2:4]), k)
            if p[2] == 'moved' or p[2] == 'inplace': k = 'compile error after moving %s' % p[3]
            if p[2].isdigit(): k = 'stand-in %s (definition / signature mismatch)' % p[3]
            if p[3] == 'defined': k = 'row defines its own %s model' % p[2]
            c[k] += 1
    a = ABASE.get(t)
    own = ('; the own row defines a stand-in (%s) -- own-row re-author needed' % a[6][2:].split()[0][:90]) if a and a[5] and a[6].startswith('N') else ''
    if not c: return ('callers respelled' + own) if own else 'fam_q: every driven row edited; the rows left are outside its reach (re-authoring)'
    return 'fam_q skips: ' + '; '.join('%d %s' % (n, k) for k, n in c.most_common(3)) + ' -- per-row re-modelling' + own
EDX = {'0143be00': ['00c72af0']}   # rows edited for a target that has no mangled final (descriptive slot)
N = {
 '0040f020': (None, 'CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound: own row already correct; the callers use free / table stand-ins (EntVec_LowerBound, MazeTable::Find, JackTable::FindEntry) of different template instantiations -- per-row'),
 '004bfac0': (None, 'CTCGraphicAppearance moved into namespace NGraphicAppearance in 0x0080FAD0, 0x008C64F0 (SetVisibleFlag -> SetAlpha), 0x008CCB00, 0x008CCD70'),
 '005baa41': ('deferred', 'template ??_G?$CDefPointer@$$CBVCAppearanceModifierDef@@@@QAEPAXI@Z: the own row models the class as a CUnproven placeholder; the ??_G emitter (empty-ctor pattern) does not cover template classes -- re-authoring'),
 '00c28690': ('deferred', 'libpk CHandleBasedMemoryPool::Free (second copy): Ego_r / FableWin carry one CHandleBasedMemoryPool class only (258 names searched, no second class or namespace), and ?Free@CHandleBasedMemoryPool@@QAEXJ@Z is bound to the engine copy 0x00A02B30 -- one mangled symbol cannot link twice: owner decision (carried from wave 15)'),
 '00c052c0': ('already-correct', 'zlib C-linkage static: the landed row already emits _build_tree (catalog name build_tree is the undecorated spelling)'),
 '00ca20a0': ('already-correct', 'libvorbis C static accumulate_fit: the landed row already defines it'),
 '00ca3f60': ('already-correct', 'libvorbis _Newton_Raphson@12: the landed row already defines it'),
 '00ad3a10': (None, 'own row already correct; 00513700 / 00ae7880 name the set<long> copy ctor (an ICF-shared instantiation: KEEP); 00ad77f0 (set<unsigned long> U-class model), 00ad62b0 (CIndexList@N00ad62b0), 00ad7e80 (RetailSet@N00ad7e80), 00ad6ae0 need per-row STLport set re-modelling'),
 '00514c10': (None, 'own row already correct (iterator traits fixed in 0x004FE7F0); 00a385a0 calls it through a set::insert stand-in with a U-class allocator and 00ad3940 through ?helper2@CTCLook -- per-row STLport set re-modelling'),
 '005d8ef0': (None, '0x005D92F0 now calls the private static ?GetRHSetFromOrientation@CTCInventoryItem@@CI?AVCRightHandedSet@@W4EInventoryObjectOrientation@@@Z'),
 '0137cf74': (None, '0x005D9730 now returns the anonymous-namespace DEFAULT_ORIENTATION (?DEFAULT_ORIENTATION@?A0x...@@3VCRightHandedSet@@B, hash-normalised)'),
 '00443e20': (None, 'the ??_G row 0x004942A2 re-authored (?LBP_dtor stand-in gone): its ~CSubtitleRenderer call is the real destructor'),
 '004942a2': (None, 're-authored as the compiler-emitted ??_GCSubtitleRenderer@@UAEPAXI@Z (empty-ctor emitter pattern)'),
 '004427d0': (None, 'CUnprovenElement placeholder replaced by CSubtitleRenderer::CSetTextInfo (PDB members): emits ??1?$deque@VCSetTextInfo@CSubtitleRenderer@@...'),
 '00947780': ('deferred', 'GetAllLeafThings<CThingBuilding, vector<CThing*>> reads PeekTypeList whose const std::list<CClass*>& return needs the STLport list model in the row (the row models a CThingList struct) -- re-authoring'),
 '008fd000': ('deferred', '31 rows read PeekThingSearchTools through stand-in classes / a non-const pointer return; the PDB spelling (IBE, const CThingSearchTools& return) needs the const CThingSearchTools API model per row -- re-authoring'),
 '008fcfd0': (None, 'fam_q17 (protected member reached from classes derived from CAIStateGroupBase at offset 0, RTTI): rows whose class derives in the row text respelled; 13 rows use stand-in class names that are not the row\'s RTTI class (...Methods / BaseR / Seed / Adapter), plus 0x0094EF70 (free GetObj) and 0x0095B020 (reference spelling) -- per-row'),
 '0049c780': ('deferred', '33 rows model PeekTypeList as returning CThingList* (non-const / stand-in classes); the PDB const std::list<CClass*>& return needs the STLport list model per row -- re-authoring'),
 '006a80a0': ('deferred', 'own row already correct; 11 rows call the HasTC operator() through generic filter templates passing pointers (PAUThing / PAVCThing / PBVCThing): the PDB ABVCThing needs a filter(*x) re-model of each template -- re-authoring'),
 '00c72e00': ('already-correct', 'descriptive per c675 (CAudioSource::ApplySound stays descriptive): no proven PDB spelling to respell to'),
 '00c720c0': ('already-correct', 'descriptive per c675 (StartFade)'),
 '00c880a0': ('already-correct', 'descriptive per c675 (ApplyHardware)'),
 '00c733d0': ('already-correct', 'descriptive per c675 (ApplySoftware)'),
 '0143be00': ('respelled', '0x00C72AF0 now calls slot +0x50 as alSourcef (pfnSourcef, Sourcef(SourceId, 0x1021, 0.0f)); RELOCATION_MATCH + PASS'),
 '0143bdb0': (None, 'the audio context object unified on g_AudioContext / CAudioContext in 0x00C4E1B0, 0x00C3EC30, 0x00C71CF0, 0x01224BC0 (descriptive name kept per c675)'),
 '00bebca0': ('already-correct', 'own row not landed; no landed row names a wrong spelling'),
 '00ba48f0': ('already-correct', 'own row not landed; no landed row names a wrong spelling'),
 '00c67f60': ('already-correct', 'own row not landed; no landed row names a wrong spelling'),
 '00ac8360': ('already-correct', 'own row not landed; no landed row names a wrong spelling'),
 '00ac7f90': ('already-correct', 'own row not landed; 0x00AD2790 (its find callee) respelled to the const set<unsigned long>::find'),
 '00ac8170': (None, 'own row already correct; 0x00AC8540 carries a same-TU auto_inline PaintSegment body (the U-struct COptimisedPrimitive stand-in) that needs a re-author on STLport list<long> -- re-authoring'),
 '00669b50': (None, 'own row already correct; 0x0066A1C0 is a mislabeled ~_List_base stand-in (?base_read) with a single source for its instantiation -- per-row, not respelled'),
 '006661b0': (None, 'own row already correct; binding RETIRE of ?nonvirt_dtor@CEngineSubPrimitiveGameMesh@@QAEXXZ at 0x006661B0 (no landed row emits it)'),
 '00c60410': (None, 'own row already correct; binding RETIRE of ?ClearMessages@CMessageOwner@@QAEXXZ at 0x00C60410 (no landed row emits it)'),
 '01278058': (None, '0x00881370 moved into NScreenEffect: now emits ??_7CScreenEffectPriorityStack@NScreenEffect@@6B@'),
 '00b7fde0': (None, 'own row already correct; 0x00B7FD00 (CRetailVector_elem56 resize) needs the STLport vector<CTVertexWaterBackground> model -- re-authoring'),
 '00b7fd00': (None, '0x00B7FD00 own row and 0x00B7FCE0 model the vector as CRetailVector_elem56 / CArray<CTVertexWaterBackground>: needs the STLport vector model -- re-authoring'),
 '00b7ffc0': (None, 'own row emits __false_type in the global namespace (std:: in the STLport text vs global in the landed row) and 0x00BE4F90 uses the CArray model -- owner note + re-authoring'),
 '00caf230': (None, 'own row already correct; 0x00CAF680 models the ctor as CTask_00caf680 on a landscape-patch stand-in: needs the CCmdQueue / CDeferredCommand re-author'),
 '00cf9350': (None, '0x00CFA2F0 re-authored as ?Alloc@CQ_AmbushTradersScript@NScript@@SI... calling the real ctor'),
 '00a01b50': (None, 'GetPItem mega family (scope 3): rows reach it through stand-in pointer views (CThingIntelligentPointerView, NScreen*, HomeObj, Subobject, ...)'),
 # c678 (appended by the coordinator during the wave)
 '00df10d0': (None, 'own row (landed with chunk 678) already defines the final; no landed row names a wrong spelling'),
 '00e57aa0': (None, 'own row (landed with chunk 678) already defines the final; no landed row names a wrong spelling'),
 '009c01c0': (None, 'own row (landed with chunk 678) already defines the final; no landed row names a wrong spelling'),
 '009ce180': (None, 'own row (landed with chunk 678) already defines the final; no landed row names a wrong spelling'),
 '00ca1060': (None, 'own row (landed with chunk 678) already defines the final; no landed row names a wrong spelling'),
 '007acc50': ('deferred', 'own row is a CTreeOwner stand-in (?FreeTree_CTreeOwner_007acc50) and 6+ callers use per-row tree stand-ins (CRetailTree_erase007acc50, CStringBoolTree, GossipList, List): needs the STLport map<CCharString,bool> re-author (as 0x00527910 / 0x00882BC0 this wave) -- next wave'),
 '00cd344a': ('deferred', 'own row already correct; ~13 callers name CCutsceneCastMap / CRetailTree_erase00cd344a stand-ins: per-row STLport map<CCharString,CScriptGameResourceObjectScriptedThingBase> re-modelling'),
 '00cbd510': ('deferred', '~CScriptBase destructor family: ~40 rows reach it through AutoTinyCompositeDestructor_00cbd510 / CBase_00cbd510 stand-ins (wave 15 scb remainder) -- re-authoring'),
 '00dec7f0': ('deferred', 'own row models the ctor as (void*, CGameScriptInterface*) with a CRetailMap member; the (CScriptDataBase*, CGameScriptInterfaceBase&) re-author (h17_qctor pattern) and the Alloc caller 0x00DF1080 (CHeroMorphDef stand-in) -- next wave'),
 '00a61b10': ('deferred', 'own row is the CDirectInputDeviceLists_00a61b10 stand-in: needs the CBusyResourceManager (two std::list<CPendingQuery>) re-author'),
 '00a61a80': ('deferred', 'own row and 0x00A61B10 use the CDirectInputDeviceLists_00a61b10::ReleaseAll stand-in: re-author with 0x00A61B10'),
 '009f7330': ('deferred', 'own row already correct; 0x009C0E50 declares the member as CDynamicTextureList (class-model change to CVsyncCallbackTimer, fam_q: no declaration to move) -- per-row'),
 '009c2220': ('deferred', 'own row is a SortTreeRecursively stand-in: needs the STLport _List_base<CDeviceResetCallback*> dtor re-author'),
 '0099a300': ('deferred', '~CBaseClassNonCopyable family (protected virtual): 0x0099A300 own row models ~CBaseClass; ~120 rows reach it through ??1CBaseClassNonCopyable QAE / CTCParamsPhysicsBase / namespace stand-ins (fam_dtor territory) -- re-authoring'),
 '00a358f0': ('deferred', 'own row not landed; 0x0075A530 calls a free GetMatrixFromRHSet stand-in returning C3DMatrix& (not the static hidden-result CMatrix3x4 member): per-row re-model'),
 '00aa7540': ('deferred', 'own row models the class as T (??1T, ??_7T): the CChunkedFileLoader re-author'),
 '00a60ce0': ('deferred', 'own row not landed; 0x009B05F0 declares the member as CChecksumFile (class-model change) -- per-row'),
 '00579435': ('deferred', 'own row is a CTextTreeWalkThrough stand-in; callers use CBankSymbolTree / CNameToHandleMap / CSymbolTree / CRetailTree stand-ins: STLport map<CCharString,unsigned long> re-author -- next wave'),
 '00994700': ('deferred', 'own row already correct; 0x009B08C0 / 0x009D0380 / 0x009D4C30 declare the ctor as (CDiskFileWin32*, unsigned long) / (CAFile*, long): parameter-model change per row'),
 '00994780': ('deferred', 'own row already correct; the same 3 rows declare a non-virtual ~CFileDataInputStream (QAE): needs the polymorphic stream model per row'),
 '00993bc0': ('deferred', 'own row and 0x00B42530 declare SetPosition non-virtual (QAE): the virtual CDataInputStream model is a class-model change'),
 '009a7f80': ('deferred', 'own row already correct; 0x009D56C0 models the parameters as SBankEntryInfo* / CCountedPointer<CRetailBank>* (NBankFileManager types not modelled in the row) -- per-row'),
 '009a9d60': ('deferred', 'own row is a ?DestVal@Slot stand-in; 0x009D56C0 names ??1?$CCountedPointer@UCRetailBank@@ (struct, global): NBankFileManager::CRetailBank class model per row'),
 '009cfbc0': ('deferred', 'own row not landed; 0x009D0380 calls ReadEntryList on a CBankFile_Retail stand-in (private member: needs the CBankFile class model)'),
 '00487dc0': (None, 'own row not landed; no landed row names a wrong spelling'),
 '007ca9e0': ('deferred', 'own row defines a CIntelligentPointer stand-in, not GiveItems: own-row re-author'),
 '009c95e0': ('deferred', 'own row already correct; 0x005A45F6 / 0x005A731F model CDataBank in the global namespace (NGameText namespace move per row)'),
 '004e34b2': ('deferred', 'own row is an OnReadFinished stand-in; 0x004E3C6E names ??1CMap_004e34b2: STLport vector<pair<long,CCharString> > re-author'),
 '00686d60': ('deferred', 'protected PeekComponent: 17 rows read it through per-class getters (GetDefinitionManager / GetFrameSource / GetGameSubsystem1C ...) from classes that do not derive from CTCBase in the row text, or with a different return model -- per-row'),
 '0065fba4': (None, '0x007C04A0 respelled (CSoundDef::CMap::Find -> GetPSoundPair); 0x008C1F70 (GetPSound returning long*) hits a compile error -- per-row'),
 '0066b280': ('deferred', 'own row already correct; callers use PlaySound / PlayRandomSound / ExternalRetry stand-ins without a CSoundInit model -- per-row'),
 '00ca0ba0': (None, 'own row not landed; no landed row names a wrong spelling'),
 '013b86a0': ('deferred', 'CMainGameComponent::PMainGameComponent: ~40 rows read it through row-local global spellings (g_pMainGameComponent, Game_013b86a0, CUnprovenGlobal, gModeGlobalRoot ...) typed per row; the static-member respell needs a shared CMainGameComponent model per row (like PClass 013b89fc) -- next wave'),
 '009cd850': (None, '0x009D1090 respelled (CBankFile::Close -> the private CloseEntries, evidence-best per c678)'),
 '009a7ca0': (None, '0x009CB960 respelled (GetBankPath -> GetDevBankFilePath const); own row is an exception stand-in -- own-row re-author'),
 '00a62ac0': (None, 'own row already correct'),
 '0057b327': (None, 'own row already correct'),
 # c681 (appended by the coordinator during the wave)
 '00e7ef70': ('deferred', 'own row models the unnested CFishCompOrganiser(CScriptOwner*, void*, const CScriptThing&): the nested NScript::CV_FishingCompetitionScript::CFishCompOrganiser re-author (h17_qctor pattern) -- next wave'),
 '00e1db00': ('deferred', 'own row: unnested CQR_EscortTraderScript(void*, CGameScriptInterface*): the NScript (CScriptDataBase*, CGameScriptInterfaceBase&) re-author (h17_qctor pattern) -- next wave'),
 '00e755f0': ('deferred', 'own row: NScript::CV_ExposeMayorScript(void*, CGameScriptInterface*): parameter-pair re-author (h17_qctor pattern) -- next wave'),
 '00d9e210': ('deferred', 'own row: NScript::CQ_MinionCampScript(void*, CGameScriptInterface*): parameter-pair re-author -- next wave'),
 '00cf88c0': ('deferred', 'own row: unnested CLadyGreyWifeManagerScript(void*, CGameScriptInterface*) and its Alloc caller 0x00CF91C0: re-author with 0x00CF91C0 -- next wave'),
 '00de0320': ('deferred', 'own row: NScript::CQ_RansomVictimScript(CScriptDataBase*, CGameScriptInterface&): CGameScriptInterfaceBase& re-author -- next wave'),
 '00e398f0': ('deferred', 'own row: NScript::CV_BordelloScript(void*, CGameScriptInterface*): parameter-pair re-author -- next wave'),
 '00cf91c0': ('deferred', 'own row is the free Create_CLadyGreyWifeManagerScript stand-in: the ?Alloc@...@NScript@@SI re-author (as 0x00CFA2F0 this wave) together with its ctor 0x00CF88C0 -- next wave'),
 '009f2b20': ('deferred', 'own row already correct; 0x00A5C6E0 / 0x00A5C9B0 call the free ?DeallocateAligned_009f2b20 stand-in (fam_q: free function, no class to move into) -- per-row'),
 '00995e70': (None, '11 callers respelled to PeekData const (QBEPBXXZ); 0x009D4A20 (CStringTableArray::GetData) and 0x00A99840 / 0x00A9BA70 (CMemoryDataOutputStreamMethods redefinition, C2011 against the shared header) -- per-row'),
 '00994b40': (None, '10 callers respelled to GetLength const; own row 0x00994B40 defines ?GetFramesRemaining@CCreatureActionBase (an ICF-shared body: own-row decision); 0x009D4A50 / 0x009D4F10 (access error after the PeekData respell) and 0x00A99840 / 0x00A9BA70 -- per-row'),
 '00b2ea90': ('deferred', 'own row is a CCountedPointer stand-in: the STLport _List_base<CCountedPointer<CCompositeSurfaceCache>>::clear re-author -- next wave'),
 '00b2ef20': ('deferred', 'own row and 0x00B2F1F0 / 0x00B2F520 use CRetailTree_erase00b2ef20 / SNodeValue stand-ins: STLport map<pair<long,long>,CEngineTextureMorph> re-author -- next wave'),
 '00e24c80': (None, 'own row (landed with chunk 681) already defines the final; its only caller, the ??_G stand-in, re-authored in family gdt to call ??1 by its PDB name'),
 '00e7b2e0': (None, 'own row (landed with chunk 681) already defines the final; its only caller, the ??_G stand-in, re-authored in family gdt to call ??1 by its PDB name'),
 '00da2f90': (None, 'own row (landed with chunk 681) already defines the final; its only caller, the ??_G stand-in, re-authored in family gdt to call ??1 by its PDB name'),
 '00cf9210': (None, 'own row (landed with chunk 681) already defines the final; its only caller, the ??_G stand-in, re-authored in family gdt to call ??1 by its PDB name'),
 '00debcb0': (None, 'own row (landed with chunk 681) already defines the final; its only caller, the ??_G stand-in, re-authored in family gdt to call ??1 by its PDB name'),
 '00e46a50': (None, 'own row (landed with chunk 681) already defines the final; its only caller, the ??_G stand-in, re-authored in family gdt to call ??1 by its PDB name'),
 '00e7f210': (None, 'own row (landed with chunk 681) already defines the final; its only caller, the ??_G stand-in, re-authored in family gdt to call ??1 by its PDB name'),
 '00b999f0': (None, 'own row (landed with chunk 681) already defines the final; its only caller, the ??_G stand-in, re-authored in family gdt to call ??1 by its PDB name'),
 '00b2d1b0': (None, 'own row (landed with chunk 681) already defines the final; its only caller, the ??_G stand-in, re-authored in family gdt to call ??1 by its PDB name'),
}
