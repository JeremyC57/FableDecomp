# mkq26_notes.py (exec'd by mkq26.py): per-target outcomes / notes for wave 26.
EDX = {
 '007acc50': _rows('gossip') + _rows('rbt'), '00440990': _rows('vlong') + _rows('gst'), '00412330': _rows('vws'),
 '009e3830': ['009f31c0'], '00686a80': ['00744580'], '0139c8fc': _rows('nul'), '00744580': ['00744580'],
 '009cbfe0': ['009cbfe0'], '009cda70': ['009cda70'], '00994400': ['00994400'] + _rows('fdos'), '00e241b0': ['00e241b0'],
}
LEFT = {
 '007acc50': ['007adbf0', '007adb50', '007add50'],
 '00440990': ['006bc4a0', '00774100', '008865c0', '00f372b0', '00f3f5a0'],
 '00412330': ['004091c0', '004128a0'],
 '009e3830': ['009f9ee0'],
 '00686a80': ['007345d0'],
 '0139c8fc': [],
 '009cbfe0': ['009cea70', '009d0380'],
 '00994400': ['009cd330'],
 '00994480': ['00994480', '009a7720'],
}
CLEAR = {}
CBANK = 'HOLD (brief): the CBankFile rows wait for the engine-header migration'
N = {
 '00c28690': ('hold', 'HOLD (brief: 0x00C28690 not in this wave; libpk CHandleBasedMemoryPool::Free second copy, owner decision, carried)'),
 '013ca828': ('deferred', 'GDefStringTable leftovers 0x005DCF70 / 0x008AF850 / 0x00890F40 (per-row re-author with the shared header; not reached, carried)'),
 '007acc50': ('respelled-partial', 'families gossip / rbt: 0x008A78E0 / 0x008A7930 are CGameScriptInterface::ClearActiveGossipCategories / '
              'ClearIsGossipForPlayer, map<CCharString, bool>::clear() of the CTCAIScratchpad static maps on the STLport _Rb_tree model (private '
              '_M_erase AAE); 0x007AD180 is _Rb_tree::erase(iterator, iterator) re-authored on the own row\'s STLport text. Left: 0x007ADBF0 '
              '(GFSerialiseMapBinaryIn<.., bool>: its X_007adbf0 map also carries insert 0x007AD060 and the CDataInputStream bridge; not reached) and '
              '0x007ADB50 / 0x007ADD50 (HOLD, brief)'),
 '00440990': ('respelled-partial', 'families vlong / gst: 16 rows\' vector<long> stand-ins (CLongVector / CLongArray / CRetailVector_long / the '
              'allocator-less std::vector) become STLport std::vector<long, allocator<long> > with the protected IAE _M_insert_overflow (size_type I, '
              'const __true_type&; out-of-class bodies __declspec(noinline) as the template instantiates under auto_inline); 0x00BB1640 / 0x00BC7920 are '
              'the GetSupportedTypes overrides of CEnginePrimitiveManager2DSprites / CEnginePrimitiveManagerText (RTTI vtable slot 2; the J027 '
              '0x00BD28A0 model). Left: 0x008865C0 (its CArray<T> template also instantiates vector<float>::_M_insert_overflow 0x004408A0), '
              '0x00F3F5A0 (CArray<T> also carries the 0x005135D0 instantiation), 0x00F372B0 (CRetailVector<T> also carries vector<CResourceEntry>::resize '
              '0x00F385A0), 0x00774100 (_Vector_base / _STLP_alloc_proxy stand-ins plus the copy ctor 0x00514160), 0x006BC4A0 (the own row is an '
              'out-of-class non-template by-value push_back: no STLport member has that shape) -- each re-model moves a second target\'s spelling'),
 '0076c000': ('deferred', 'scope 2 not reached: the c690 sort-row leftovers (own row on the descriptive SElement16 / SCompare sort, callers 0x0076C360 / 0x00AF59A0)'),
 '0076c1a0': ('deferred', 'scope 2 not reached: as 0x0076C000 (callers 0x0076C6B0 / 0x00AF59A0)'),
 '009cec60': ('hold', CBANK), '009cbd30': ('hold', CBANK), '009cbd50': ('hold', CBANK),
 '0099b510': ('deferred', 'carried: the ~CCharString-labelled ~CWideString binders (009994A0 / 00999620 and the destructor-family stand-ins of scope 3; not reached)'),
 '0099a300': ('deferred', 'carried (wave 25 respelled-partial): the destructor-family stand-ins of scope 3 (not reached); 0x00BC9C30 / 0x009CDF50 '
              'now landed by chunk 693 on the final spellings'),
 '00412330': ('respelled-partial', 'family vws: 0x005FDAD0 / 0x0071CE80 / 0x009AEF60 / 0x009AF480 -- CWideStringVector becomes STLport '
              'std::vector<CWideString, allocator<CWideString> > with the protected _M_insert_overflow(.., const __false_type&, size_type I, bool). '
              'Left: 0x004091C0 (vector<T, Trivial> also instantiates the CDateAndTime grow-insert: a second target moves) and 0x004128A0 (the bound '
              'vector<CWideString>::push_back, but its own row is a Leaf-less free VectorPushBack over a VectorPushBackOverlay::InsertRange view: re-author as the STLport member)'),
 '0099b800': ('deferred', 'scope 2 not reached: 0x009994A0 / 0x00999620 define the CWideString bodies in-TU on AFileSupport / ExternalRetry namespace models'),
 '009e3830': ('respelled-partial', 'family wcol (DECISIONS_W25 D7): 0x009F31C0 is CSurface::SetPixel(long, long, const CRGBColour&, const '
              'CSurfaceLockedRect&) (Ego_r / FableWin), its CPixelFormat built by the inline ctor -> Initialise(_D3DFORMAT). Left: 0x009F9EE0 '
              '(CPixelFormatByteLengthView; not reached)'),
 '00686a80': ('respelled-partial', 'family tbvm: 0x00744580 binds PeekWorld as CTCPerceiveThing::GetTimeBasedVisibilityMultiplier. Left: 0x007345D0 '
              '(CTCCombat_Methods::GetWorld stand-in; not reached)'),
 '0139c8fc': ('respelled', 'family nul (DECISIONS_W25_RESOLVED D1): the three _afxDataNil binders 0x00C0B910 / 0x00C0B9F0 / 0x00C0BA40 name '
              '?_NullString@@3PAUCStringData@@A (retail\'s own assert text "stringSrc.GetData() != _NullString"); 0x0139C900 stays HOLD (D4)'),
 '00744580': ('respelled', 'family tbvm (DECISIONS_W25_RESOLVED D5): ?GetTimeBasedVisibilityMultiplier@CTCPerceiveThing@@ABEMXZ (Ego_r.pdb + '
              'FableWin.pdb; body reads CPerceivedThingDef Day/NightTimeVisibilityMultiplier through CGameTimeManager::IsDay / IsNightTime); was the '
              '__fastcall GetCtx stand-in'),
 '00bc9c30': ('done', 'own row (chunk 693) defines it; no other row names a stand-in'),
 '009cdf50': ('done', 'own row (chunk 693) defines it'),
 '009cbfe0': ('respelled-partial', 'family c693: the own row declares GetRecoveryFilePath private (AAE, Ego_r / FableWin; was QAE). Left: the '
              'CBankFile_Retail binders 0x009D0380 / 0x009CEA70 (CBankFile HOLD, brief)'),
 '009cda70': ('respelled', 'family c693: the own row\'s CBankFile_Retail is CBankFile (?InternalWriteEntryList@CBankFile@@QAEKAAVCDataOutputStream@@@Z)'),
 '00994400': ('respelled-partial', 'families c693 / fdos: the own row is CFileDataOutputStream(CAFile&) and the binders 0x009A7720 / 0x009D4CE0 / '
              '0x00A60AA0 pass the CDiskFileWin32 (RTTI 0x0122D06C: CDiskFileWin32 <- CAFile <- CBaseClassNonCopyable) by reference. Left: '
              '0x009CD330 (CDiskFileCalls stand-in; its Write slot model not settled)'),
 '00994480': ('deferred', 'needs the virtual-slot model (UAE: Write is CDataOutputStream\'s virtual), but engine/CFileDataOutputStream.h flattens the '
              'vptr: own row 0x00994480 (free __fastcall Write) and its binder 0x009A7720 stay (decision asked)'),
 '0099d8c0': ('done', 'no landed row defines or names it; nothing to respell (the spelling stands for the row that lands 0x0099D8C0)'),
 '00c013c0': ('done', 'own row defines it'), '00cf0790': ('done', 'own row (chunk 693) defines it'),
 '00d89e60': ('done', 'own row (chunk 693) defines it'), '00ddde20': ('done', 'own row (chunk 693) defines it'),
 '00ef8d30': ('done', 'own row (chunk 693) defines it'), '00dde230': ('done', 'own row (chunk 693) defines it'),
 '00e241b0': ('respelled', 'family c693: the own row is ??1CIsThingAlive@NScript@@UAE@XZ on engine/CPredicateBase.h (was CScriptThingFunctor_00e241b0); '
              'family pred\'s 0x00EE76F0 calls it out of line'),
 'CAudioMaster': ('deferred', 'not reached: the struct-key family (class -> struct CAudioMaster, U) over every g_pAudioMaster row'),
 '00ef87d0 00ef5cb0': ('other-lane', 'not a naming row: the inline_depth re-model is the chunk 694 repair lane'),
}
# the chunk 694 queue rows (appended to QUEUE_W26_input.tsv after the chunk 694 landing)
EDX.update({'00854310': ['00854500', '00854540', '00854580'], '005ed120': ['00718810', '00854450'], 'CClassTypeInit': _rows('cti')})
LEFT.update({'0052ccf0': ['00549a00'], '0052d5c0': ['0052d5c0', '0052d770'], 'CClassTypeInit': []})
N.update({
 '00854310': ('respelled', 'family c694: the ??_G stand-ins of OpenChest 0x00854500 / UseLamp 0x00854540 call it as '
              '??1CScriptModuleSelfTerminateBase@NCameraModes@@UAE@XZ (was ??1CScriptModuleBase@NCameraModes@@QAE@XZ) and OpenGate 0x00854580 '
              '(was the free ?CScriptModuleOpenGate_Destruct@@YIXPAX@Z); bases declared with their virtual destructors, the calls stay qualified '
              '(direct); Leaf lines added'),
 '005ed120': ('respelled', 'family c694: GoToLookAt 0x00718810 / TransitionBetweenPoints 0x00854450 call it as ??1CScriptModuleBase@NCameraModes@@UAE@XZ '
              '(were the free ?CScriptModule<X>_Destruct@@YIXPAX@Z); Leaf lines added'),
 '0052ccf0': ('deferred', 'own row (chunk 694) defines it; left: 0x00549A00 (CTextSlider dtor) names it as ~CDraggableInto -- the re-model renames '
              'CTextSlider\'s base chain (CDraggableInto vs CChangingStateComponent: RTTI base array not checked this wave; not reached)'),
 '0052d5c0': ('deferred', 'not reached: own row 0x0052D5C0 is the SortTreeRecursively_00436900 stand-in and 0x0052D770 names ~CStateList; both need the '
              'STLport list<CChangingStateComponent::EChangingStateComponentState> model (_List_base dtor out of line, list ctor) in one family'),
 '00cafa10': ('done', 'own row (chunk 694) defines it; no other row names it'),
 'CClassTypeInit': ('respelled', 'family cti (chunk 694 decision 2): the nine CClassTypeInfo models that spelled AllocFunc __cdecl spell CClass* '
              '(__fastcall*)() (FableWin ??0CClassTypeInit@@QAE@JP6IPAVCClass@@XZ_NABVCCharString@@@Z); no mangled symbol carries the type'),
})
