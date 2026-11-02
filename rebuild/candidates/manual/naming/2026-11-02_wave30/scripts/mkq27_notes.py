# mkq27_notes.py (exec'd by mkq27.py): per-target outcomes / notes for wave 27.
EDX = {
 '00440990': ['008865c0', '00f3f5a0', '00f372b0', '00774100', '00aebaf0'],
 '00412330': ['004128a0', '005955ab'],
 '00994480': ['00994480', '009a7720', '00a60aa0'],
 '009e5c30': ['009aef60', '009cc530', '009b0470', '00ae85f0'],
 '009e5350': ['009ae5f0'],
 '00854380': ['008544c0'],
 '007196b0': ['007196b0'],
 '00a0d780': ['00a0d410'],
 '005bc527': ['00584a73'],
}
LEFT = {
 '013ca828': ['005dcf70', '00890f40', '008af850'],
 '007acc50': ['007adbf0', '007adb50', '007add50'],
 '00440990': ['006bc4a0'],
 '00412330': ['004091c0'],
 '009e3830': ['009f9ee0'],
 '00686a80': ['007345d0'],
 '009cbfe0': ['009cea70', '009d0380'],
 '00994400': ['009cd330'],
 '00994480': [],
 '00719280': ['005359a0', '00713660', '00713850', '00714900'],
 '00a0d780': ['00a0d2d0'],
 '0051df60': ['006a5230'],
 '00996b30': ['00a99840', '00a9ba70'],
 '00838a00': ['005299c0', '00529b00', '0052ac10', '0052ac90', '0052ad50', '007031c0', '007042a0', '00837c70'],
 '005bc65c': ['0056e5ec', '00581a6d', '00602f30', '005b34d5', '005c2e2a'],
 '009e7e40': ['009e7ec0'],
}
CLEAR = {}
C695 = 'own row respelled at the chunk 695 landing'
N = {
 '013ca828': ('deferred', 'GDefStringTable leftovers 0x005DCF70 / 0x008AF850 / 0x00890F40 (per-row re-author with the shared header; not reached, carried)'),
 '007acc50': ('deferred', 'not reached: 0x007ADBF0 (GFSerialiseMapBinaryIn<.., bool>: its X_007adbf0 map also carries insert 0x007AD060 and the CDataInputStream '
              'bridge); 0x007ADB50 / 0x007ADD50 HOLD (brief)'),
 '00440990': ('respelled-partial', 'family vl2 (DECISIONS_W26 D6): 0x008865C0 / 0x00F3F5A0 / 0x00F372B0 / 0x00774100 / 0x00AEBAF0 on STLport std::vector<T, '
              'allocator<T> > (CArray<T> derived where the PDB says CArray), each second target on its own row\'s / the bound STLport spelling: '
              'vector<float> 0x004408A0, vector<unsigned short> 0x005135D0, vector<CResourceEntry>::resize 0x00F385A0, vector<long>(const vector&) '
              '0x00514160, vector<CCharString> 0x00433530; 0x00774100 and 0x00AEBAF0 also take their Ego_r / FableWin Leafs. Left: 0x006BC4A0 '
              '(identity unproven, D6: its by-value push_back(long) matches no STLport member)'),
 '0076c000': ('deferred', 'not reached (scope 3): the c690 sort-row leftovers (own row on the descriptive SElement16 / SCompare sort, callers 0x0076C360 / 0x00AF59A0)'),
 '0076c1a0': ('deferred', 'not reached (scope 3): as 0x0076C000 (callers 0x0076C6B0 / 0x00AF59A0)'),
 '0099b510': ('deferred', 'not reached (scope 3): the ~CCharString-labelled ~CWideString binders, carried'),
 '0099a300': ('deferred', 'not reached (scope 3): the destructor-family stand-ins, carried'),
 '00412330': ('respelled-partial', 'family pb (DECISIONS_W26 D7): own row 0x004128A0 re-authored as the STLport member vector<CWideString, '
              'allocator<CWideString> >::push_back (explicit instantiation, the bound spelling; it calls 0x00412330 by its established spelling) and '
              '0x005955AB, the binder of the allocator-less push_back, on the allocator vector (its Leaf the Ego_r / FableWin protected '
              'GetProfileNames). Left: 0x004091C0 -- the D7 re-model moves 0x004125A0 (vector<CDateAndTime> grow-insert: 0x004091C0 instantiates the '
              '__true_type form, the own row 0x004125A0 defines the allocator-less __false_type form; which tag retail used is unproven) and '
              '0x0040E350 (bound as std::__copy<CDateAndTime*> by 0x004091C0, but its own row is global_CopyCActionInputControl and 0x0059AEAF defines '
              'the template __copy there): two targets without a settled spelling, decision asked (DECISIONS_W27 D3)'),
 '0099b800': ('deferred', 'not reached (scope 3): 0x009994A0 / 0x00999620 define the CWideString bodies in-TU on AFileSupport / ExternalRetry namespace models'),
 '009e3830': ('deferred', 'not reached: 0x009F9EE0 (CPixelFormatByteLengthView)'),
 '00686a80': ('deferred', 'not reached: 0x007345D0 (CTCCombat_Methods stand-in; its own row is CTCCombat::GetAppropriateCombatAnimationFromList by '
              'the Ego_r / FableWin name set -- see DEFERRED 0x00666B50)'),
 '009cbfe0': ('hold', 'HOLD (brief): the CBankFile_Retail binders 0x009D0380 / 0x009CEA70 wait for the engine/CBankFile.h migration'),
 '00994400': ('deferred', 'not reached: 0x009CD330 (CDiskFileCalls stand-in; its Write slot model now available from the fdosw opt-in header)'),
 '00994480': ('respelled', 'family fdosw (DECISIONS_W26 D2): engine/CFileDataOutputStream.h gains the opt-in retail layout '
              'FABLE_RETAIL_FILE_DATA_OUTPUT_STREAM (vtable 0x0129A6F8: SetPosition / GetPosition / Close / Write, ~ UAE); own row 0x00994480 is '
              'CFileDataOutputStream::Write (UAE; was a free __fastcall Write), binders 0x009A7720 / 0x00A60AA0 call it, ~ / GetPosition / '
              'SetPosition through the virtual model (direct calls on the local), WriteDateAndTime as CDataOutputStream\'s; both binders\' own Leafs '
              'take their PDB private access (D5)'),
 'CAudioMaster': ('deferred', 'not reached: the struct-key family (class -> struct CAudioMaster, U) over every g_pAudioMaster row'),
 '0052ccf0': ('deferred', 'not reached: 0x00549A00 (CTextSlider dtor) names it as ~CDraggableInto (RTTI base array not checked)'),
 '0052d5c0': ('deferred', 'not reached: own row 0x0052D5C0 and 0x0052D770 need the STLport list<CChangingStateComponent::EChangingStateComponentState> model'),
 '005ed210': ('done', C695 + '; no other landed row names a stand-in'),
 '00854380': ('respelled', 'family c695: the StayRelative ??_G stand-in 0x008544C0 calls ??1CScriptModuleFOVZoom@NCameraModes@@UAE@XZ qualified (was a free '
              '?CScriptModuleFOVZoomStayRelative_Destruct@@YIXPAX@Z); Leaf line added'),
 '00996b30': ('deferred', C695 + '; left: 0x00A99840 / 0x00A9BA70 name it on CMemoryDataOutputStreamMethods@X_<addr> stand-ins (the polymorphic '
              'CMemoryDataOutputStream model per row; not reached)'),
 '009e5c30': ('respelled', 'families dm / sym: 0x009AEF60 (the CDefinitionManager PSymbolMap / PDefinitionSymbolMap), 0x009CC530, 0x009B0470 and '
              '0x00AE85F0 call CSymbolMap::UpdateDirty() const (was the invented ?Finalise@CSymbolMap / ?Finalise@CSymbolTable)'),
 '009e5c60': ('done', 'own row and its caller 0x009E2590 respelled at the chunk 695 landing'),
 '009e5350': ('respelled', 'family sym: 0x009AE5F0 calls CSymbolMap::DeallocateUnusedMemory (was ?RemoveUnusedMemory@CSymbolMap) and its own Leaf takes '
              'the PDB protected access (?RemoveUnusedMemory@CDefinitionManager@@IAEXXZ, the spelling 0x009B08C0 binds); the target has no landed own row'),
 '009e7c70': ('deferred', 'own row 0x009E7C70 models CArray<SSymbolPair> (?DeallocateUnusedMemory@?$CArray@USSymbolPair@@..) under a bare Leaf; '
              'the pair<CCharString, CSymbolMap::CSymbolInfo> element re-model not reached'),
 '009e7e40': ('deferred', 'the c695 rows name the std::sort spelling; left: 0x009E7EC0 binds it as ?TransferBoolHelper@@YIXPAX0_N@Z (not reached)'),
 '00719280': ('deferred', 'the c695 rows name the _List_base::clear spelling and 0x007196B0 now does too (family c695); left: 0x00713660 / 0x00713850 / '
              '0x00714900 (?clear@CActionList@@: CActionBase is modelled as NTCScriptedControl::CActionBase, the PDB nests it in CTCScriptedControl -- a '
              'namespace-to-nested-class re-model of the three rows\' own Leafs) and 0x005359A0 (a list<CCountedPointer<NUISystem::CAction> > ??_G '
              'binding the same address: identical-code folding, two element types, one retail copy -- hold)'),
 '007196b0': ('respelled', 'family c695: own row re-authored as STLport ~_List_base<CCountedPointer<CTCScriptedControl::CActionBase> > (clear 0x00719280 '
              'then deallocate the node, explicit instantiation); the queue\'s hand-mangled spelling is exactly the symbol the compiler emits '
              '(RELOCATION_MATCH + PASS). Was a free __fastcall CActiveFile_OnReadFinished stand-in -- RENAMES'),
 '00713740': ('done', C695),
 '006a5d50': ('done', 'no landed own row; the c695 0x00838A00 row names it; the ?Destroy@CEngineSubPrimitiveGameMesh binding has no landed namer'),
 '0051dff0': ('done', C695 + '; 0x00838A00 names it'),
 '0051dfe0': ('done', C695),
 '0051df60': ('deferred', C695 + '; left: 0x006A5230 binds it as ?Add@BlendMgr@@ -- that row is catalogued C3DAnimationBlendState::AddCombinationBlend '
              'but its body (0x004C9690 then GThingManager->AddToPhysicalList(*this)) is a CThingPhysical member: identity re-model, not reached'),
 '0051df70': ('done', C695),
 '00a0d780': ('respelled-partial', 'family c695: the CAInputProcess ??_G 0x00A0D410 calls the protected by-reference spelling with CAInputProcess a friend of '
              'CInputProcessManager (retail calls it from CAInputProcess); Leaf line set to ??_GCAInputProcess@@UAEPAXI@Z. Left: 0x00A0D2D0 (its own row '
              'is the CAInputProcess_Methods stand-in over the flattened engine/CAInputProcess.h: the friend does not reach a derived stand-in)'),
 '00a0d3d0': ('deferred', C695 + '; left: the per-caller ??_G stand-ins (~15 rows, 0x0048B3B0 .. 0x0048CEC0) name it on invented spellings (scope 3 '
              'destructor families)'),
 '00838a00': ('deferred', C695 + '; left: eight rows bind the non-virtual ??1CThingGameObject@@QAE@XZ (each needs the virtual-dtor model of its local '
              'CThingGameObject; not reached)'),
 '005bc527': ('respelled', 'family c695: 0x00584A73 calls NInventory::CTCInventoryBase::AddCloseInventoryEvent (UAE) qualified, its own Leaf the Ego_r / '
              'FableWin virtual ?AddCloseInventoryEvent@CTCInventory@@UAEXXZ'),
 '005bc65c': ('deferred', C695 + '; left: five binders (0x0056E5EC / 0x00581A6D on a global CTCInventoryBase, 0x00602F30 _Map, 0x005B34D5 / 0x005C2E2A '
              '_Retail stand-ins with __vftable members): each needs NInventory nesting plus the virtual model (not reached)'),
 '0061b5f0': ('deferred', 'the landed own row is a Leaf-less __fastcall free stand-in (?CTCInventoryQuests_ProcessButtonBReleased@@): re-author as the virtual '
              'member (not reached)'),
}
