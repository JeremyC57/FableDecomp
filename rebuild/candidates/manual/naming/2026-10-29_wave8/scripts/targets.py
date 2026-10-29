# wave 8 target table: retail address -> final symbol settled in wave 8 (DECISIONS_W8.tsv merges these with the proposals)
FINAL = {   # scope 1: the three pass-9 RELOC_CONFLICTs
    '004a96f0': '??0?$CIntelligentPointer@VCThing@@@@QAE@PAVCThing@@@Z',
    '0056c860': '??0?$CIntelligentPointer@$$CBVCThing@@@@QAE@PBVCThing@@@Z',
    '009d5530': '?GetData@CBankFileAsyncData@@QAEPAXXZ',
    '009d5580': '?PeekData@CBankFileAsyncData@@QBEPBXXZ',
    '006860f0': '?_M_fill_assign@?$vector@_NV?$allocator@_N@std@@@std@@QAEXIAB_N@Z',
    '00467686': '?resize@?$vector@_NV?$allocator@_N@std@@@std@@QAEXIAB_N@Z',
}
FINAL.update({   # GPT conflicts file / disagreements with earlier waves
    '004c6b60': '?erase@?$_Rb_tree@PAXU?$pair@QAXVCBulletTimeInfo@CBulletTimeManager@@@std@@U?$_Select1st@U?$pair@QAXVCBulletTimeInfo@CBulletTimeManager@@@std@@@2@U?$less@PAX@2@V?$allocator@U?$pair@QAXVCBulletTimeInfo@CBulletTimeManager@@@std@@@2@@std@@QAEIABQAX@Z',
    '007e9d80': '?erase@?$_Rb_tree@PAVCThing@@U?$pair@QAVCThing@@M@std@@U?$_Select1st@U?$pair@QAVCThing@@M@std@@@3@U?$less@PAVCThing@@@3@V?$allocator@U?$pair@QAVCThing@@M@std@@@3@@std@@QAEIABQAVCThing@@@Z',
    '004fc180': '?GetRegion@CWorldMap@@QBEABVCRegion@@J@Z',
    '00686d90': '?PeekThingSearchTools@CTCBase@@IBEABVCThingSearchTools@@XZ',
    '00686d20': '?PeekWorldMap@CTCBase@@IBEABVCWorldMap@@XZ',
    '0049d9d0': '?PeekMyPlayer@CWorld@@QBEPBVCPlayer@@XZ',
})
CONF = {
    '004a96f0': 'scope 1: stores vtable 0x01238C6C, RTTI .?AV?$CIntelligentPointer@VCThing@@@@ (labels_* RTTI notes; 004a9730 / 006a2ba0 same vtable); both PDBs carry the spelling. Landed 0072cba0 and 0071c9f0 bound the const-CThing ctor here; both now construct CIntelligentPointer<CThing> (0071c9f0: Ego_r CTCHeroMorph::IdleParticleEmitters is list<CIntelligentPointer<CThing> >)',
    '0056c860': 'scope 1: stores vtable 0x01237F1C, RTTI .?AV?$CIntelligentPointer@$$CBVCThing@@@@; pass-9 row 0064a2db binds it here (retail call 0x0064A2DB -> 0x0056C860)',
    '009d5530': 'scope 1: Ego_r order IsFinished 0xB29550, GetData 0xB295A0, PeekData 0xB295F0, CanRelocateData 0xB29640; FableWin order WaitUntilFinished, GetData, PeekData; retail IsFinished 0x9D54E0, CanRelocateData 0x9D55D0 (catalog) bracket the twins 0x9D5530 / 0x9D5580; callers by retail call target: 009c8700 009c8fb0 00a25d20 00a42ea0 00bdc510 00bf3b60 009fd9b0 -> 0x9D5530',
    '009d5580': 'see 009d5530; the only caller 00be04c0 -> 0x9D5580 = PeekData() const',
    '006860f0': 'scope 1: body is STLport 4.6 vector::_M_fill_assign verbatim (n > capacity: fill-construct a temporary 0x00451583 and swap; else fill + uninitialized_fill_n / fill_n + erase); landed defining row STLvectorbool_Mfillassign_006860f0 (public); caller 006821e0 reaches it through the inline assign(0x5C, true)',
    '00467686': 'scope 1: body is STLport vector::resize (size compare, erase 0x004676BB or insert 0x004676EF); pass-9 row 00cdd0a0 binds it here',
    '004c6b60': 'GPT conflicts file: back-reference numbering settled by compiling the declaration with the lane cl 13.10.3077 (key void* has no name: @2@)',
    '007e9d80': 'GPT conflicts file: back-reference numbering settled by compiling the declaration with the lane cl 13.10.3077 (key CThing* adds a name: @3@)',
    '004fc180': 'GPT proposal QAEAAV (one external agent) rejected: W1/W6/W7 proven const GetRegion (0064e019 reaches 0x4FC180 through PeekWorldMap, slot 13); the non-const GetRegion is 0x004FB490 (bound, KEEP)',
    '00686d90': 'GPT proposal GetThingSearchTools IAEAAV rejected: that spelling is bound to 0x00686D80 (007ee5a0); W1/W7 proven PeekThingSearchTools here',
    '00686d20': 'GPT conflicts file: W7 settled PeekWorldMap here (GetWorldMap 0x00686D30)',
    '0049d9d0': 'retail body `mov ecx,[ecx+0Ch] ; jmp 0x00449970` (PlayerManager->GetMainPlayer); retail order GetSampleBank 0x49D9B0, (0x49D9C0), 0x49D9D0 = Ego_r order GetSampleBank 0x4A6D00, PeekSampleBank 0x4A6D10, PeekMyPlayer 0x4A6D20; both PDBs; W1 proven; W7 "descriptive GetCurrentPlayer" (no PDB symbol) was wrong',
}
REJECT = {
    '004fc180': 'see evidence',
    '00686d90': 'see evidence',
    '00686d20': 'see evidence',
    '006d4d70': 'malformed proposal ("manifest IsAIFollower"); W1/W7 proven ?IsFollower@CTCFollowed@@QBE_NPBVCThing@@@Z stands',
    '007fef40': 'malformed proposal ("FreeActionPoint/UseActionPoint"); W1 proven ?SetActionPointAsFree@CTCUserActionPointBase@@QAEXJ@Z stands',
}
MANUAL_PROVEN = set(FINAL)
APPLIED = {}

# ---- applied in wave 8 (families / hand scripts; every final spelling checked present in BOTH name lists unless noted) ----
_B = 'Ego_r.pdb + FableWin.pdb (exact, both lists); '
_APP = {   # address: (final, family, evidence)
    '0099f0a0': ('??YCCharString@@QAEABV0@ABV0@@Z', 'opy8', _B + 'Ego_r order operator+=(const CCharString&), (const char*), (char) = retail 0x99F0A0, 0x99F100, 0x99F180; only the const return differs'),
    '0099f100': ('??YCCharString@@QAEABV0@PBD@Z', 'opy8', _B + 'see 0099f0a0'),
    '0099f180': ('??YCCharString@@QAEABV0@D@Z', 'opy8', _B + 'see 0099f0a0'),
    '006e0be0': ('?AttachToThing@CTCDParticleEmitter@@QAEXAAVCThing@@JABVCCharString@@JM@Z', 'att8 + att8j', _B + 'the only AttachToThing in both PDBs; pointer -> reference (att8, 43 rows); att8j: 8 rows that modelled the last parameter as long (every call passes literal 0) respelled to float'),
    '0099a380': ('??0CBaseObject@@IAE@XZ', 'cbo8', _B + 'protected default ctor; access only'),
    '0099a3b0': ('??4CBaseObject@@IAEAAV0@ABV0@@Z', 'cbo8', _B + 'protected copy assignment; access only'),
    '004c9d60': ('?AddTC@CThing@@QAEPAVCTCBase@@ABVCCharString@@_NPBVCTCParamsBase@@@Z', 'atc8', _B + 'const CTCParamsBase* only'),
    '00405f00': ('?GFIntToWideString@@YI?AVCWideString@@J@Z', 'i2w8', _B + 'the only `CWideString __fastcall (long)` GF*ToWideString in both PDBs (Ego_r 0x00404F90); retail body formats L"%d"'),
    '00999230': ('?PathExists@CAFile@@SI_NABVCWideString@@@Z', 'sm8', _B + 'the only PathExists in both PDBs; free fastcall -> CAFile static fastcall (same code)'),
    '0051ed80': ('?GetPThingWithUID@CThingManager@@QAEPAVCThing@@_K@Z', 'sm8', _B + 'the only CThingManager member taking unsigned __int64 and returning CThing*'),
    '00a22f80': ('?Rotate@CRightHandedSet@@QAEXABVC3DVector@@M@Z', 'sm8', _B + 'the only CRightHandedSet (const C3DVector&, float) member'),
    '0049d810': ('?GetWorldSeed@CWorld@@QBEPAKXZ', 'sm8', _B + 'body returns [world+8] + 0x1613C = CMainGameComponent::WorldSeed (Ego_r +90428)'),
    '0073aba0': ('?RemoveMode@CTCCreatureModeManager@@QAEXK@Z', 'sm8', _B + 'unsigned long parameter only'),
    '008fcfd0': ('?GetPWorldSeed@CAIStateGroupBase@@IBEPAKXZ', 'aisg8', _B + 'Ego_r accessor order IsRunnableMain .. IsStopping = retail 0x008FCF10 .. 0x008FD010 one slot each'),
    '008fcf90': ('?GetPlayerManager@CAIStateGroupBase@@IBEAAVCPlayerManager@@XZ', 'aisg8', _B + 'see 008fcfd0'),
    '008fcf80': ('?PeekPlayerManager@CAIStateGroupBase@@IBEABVCPlayerManager@@XZ', 'aisg8', _B + 'see 008fcfd0'),
    '009bedc0': ('?GetRenderTargetDimensions@CDisplayManager@@QAE?BVC2DExtentsI@@XZ', 'rtd8', _B + 'non-const, const C2DExtentsI by value'),
    '006644f0': ('?SetCurrentAction@CThingCreatureBase@@QAE_NABVCCreatureActionBase@@@Z', 'sca8', _B + 'pointer -> const reference only'),
    '00438570': ('?PeekGuiDef@CPlayerGui@@SIABVCPlayerGuiDef@@XZ', 'pgd8', _B + 'static fastcall member returning const&; already bound here by 0066e742'),
    '007aef10': ('?CountEffects@CTCBoss@@QAEJXZ', 'sn8a', _B + 'self-name: drops const'),
    '007f61b0': ('?AddThingToNotAttack@CTCTargetingAI@@QAEXAAVCThing@@@Z', 'sn8a', _B + 'self-name: CThing&'),
    '008e5270': ('?UpdateIsMoving@CTCPhysicsStandard@@IAEXXZ', 'sn8a', _B + 'self-name: protected'),
    '008e5670': ('?PerformForceUpdates@CTCPhysicsStandard@@IAEXXZ', 'sn8a', _B + 'self-name: protected'),
    '008f6fc0': ('?ZeroAllUnusedThemePaletteEntries@CMap@@IAEXXZ', 'sn8a', _B + 'self-name: protected'),
    '008fa120': ('?ClearData@LSocket@LHNetworkLib@@AAEXXZ', 'sn8a', _B + 'self-name: private'),
    '0097fa00': ('?AIAction_MinionIdle@CAIStateGroupBase@@IAE?AW4EAIActionReturn@@XZ', 'sn8a', _B + 'self-name: protected (00917380 already calls this spelling)'),
    '00a48ac0': ('?WriteToFile@CSpeechFrame@@QBEXAAVCDataOutputStream@@@Z', 'sn8a', _B + 'self-name: CDataOutputStream& (00a48b50 already calls this spelling)'),
    '00ab5d00': ('??0CMouseDX@@QAE@AAVCInputManagerDX@@_N@Z', 'sn8a', _B + 'self-name: CInputManagerDX&; base call respelled to ??0CMouse@@QAE@AAVCInputManager@@_N@Z (defined so by 00a66f20)'),
    '00ab64e0': ('??0CKeyboardDX@@QAE@AAVCInputManagerDX@@_N@Z', 'sn8a', _B + 'self-name: CInputManagerDX&; base call respelled to ??0CKeyboard@@QAE@AAVCInputManager@@_N@Z (defined so by 00a66930)'),
    '00ab7850': ('??0CJoystickDX@@QAE@AAVCInputManagerDX@@_N@Z', 'sn8a', _B + 'self-name: CInputManagerDX&; base call respelled to ??0CJoystick@@QAE@AAVCInputManager@@_N@Z'),
    '009e43b0': ('??0CJoystick@@QAE@AAVCInputManager@@_N@Z', 'sn8a', _B + 'self-name: CInputManager&'),
    '00a66f20': ('??0CMouse@@QAE@AAVCInputManager@@_N@Z', 'sn8a', _B + 'defining row already right; its caller 00ab5d00 respelled'),
    '00a66930': ('??0CKeyboard@@QAE@AAVCInputManager@@_N@Z', 'sn8a', _B + 'defining row already right; its caller 00ab64e0 respelled'),
    '0049dd50': ('?CopyFileInternal@CWorld@@IBEXABVCWideString@@0@Z', 'sn8b', _B + 'self-name + callers 004a0180 004a0ae0 004a0cd0: protected'),
    '004a00e0': ('?SaveHeroStateInternal@CWorld@@IAE_NABVCWideString@@_N@Z', 'sn8b', _B + 'self-name + callers 004a0180 004a05c0: protected'),
    '004b00c0': ('?IsQuestRegistered@CQuestManager@@QBE_NABVCCharString@@@Z', 'sn8b', _B + 'self-name + caller 00892f50: const'),
    '0063dfd0': ('?UpdateAllEdges@CWorldMap@@IAEXXZ', 'sn8b', _B + 'self-name + callers 004ff080 004ff440: protected'),
    '006cb620': ('?IsAbleToNavigateFromPosToPos@CTCCreatureNavigation@@QBE_NABVC3DVector@@0M@Z', 'sn8b', _B + 'self-name + callers 0074a260 008c1600 (const), 006ea770 00970930 (pointer -> reference)'),
    '00a01920': ('?GetSoundIndexFromSymbol@CASoundBank@@QBEJABVCCharString@@@Z', 'sn8b', _B + 'self-name + callers 006e2a50 0065c781: const (9 other callers already const)'),
}
for _a, (_s, _f, _e) in _APP.items():
    FINAL.setdefault(_a, _s); CONF.setdefault(_a, _e); MANUAL_PROVEN.add(_a)
    APPLIED[_a] = 'applied: ' + _f
for _a in ('004a96f0', '0056c860', '009d5530', '009d5580', '006860f0', '00467686'):
    APPLIED[_a] = 'applied: c8a (scope 1)'
CONF['006860f0'] += ('. Spelling evidence is structural, not PDB: Ego_r/FableWin carry the Dinkumware bit-vector vector<bool> (_Assign_n, '
                     'resize(unsigned,bool)); retail runs the generic STLport vector<T> body for bool (byte-matched callers pass const bool&)')
CONF['00467686'] += '; spelling structural (see 006860f0)'
for _a in ('004c6b60', '007e9d80', '0049d9d0'):
    APPLIED.setdefault(_a, 'decided, not applied (no row edit needed / structural: see DEFERRED.tsv)')
