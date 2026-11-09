# Chunk 892 header changes (overlay, copies of the CURRENT rebuild/include)

Every change to an existing header is behind a new `FABLE_RETAIL_*` opt-in (or an existing opt-in's `#if` widened by `||`); with
no opt-in defined each header preprocesses to the same text as the repo copy, so the default path stays byte-identical. New
headers (marked NEW) declare only PDB-backed classes; where they carry bodies those are opt-in too. Each block's own comment in
the header cites the retail addresses; this file is the index. Land by copying the files over rebuild/include (except
engine/LHNetworkTypes.h, an unchanged neighbour copy).

## stlport/fable_containers.h
- `FABLE_STLP_NOINLINE` / `FABLE_STLP_FASTCALL` (`__declspec(noinline)` / `__fastcall` under `_MSC_VER`, empty elsewhere), defined
  only when one of the opt-ins below that needs them is on.
- `FABLE_RETAIL_STLP_LIST_DTOR_OUTOFLINE`, `FABLE_RETAIL_STLP_LIST_INSERT_OUTOFLINE` (row 0x005A887D): list's ~list() and
  insert(iterator, const T&) with bodies visible but kept out of line (retail calls 0x0042ABCA / the insert COMDAT).
- `FABLE_RETAIL_STLP_SORT_INTERNALS` (rows 0x00807250, 0x00808AA0): STLport 4.6 iter_swap, __copy_backward (random-access, out of
  line `__fastcall`, 0x00806310) / copy_backward, __unguarded_partition, __unguarded_linear_insert (declaration, 0x008074A0),
  __linear_insert -- the /Gr algorithm TU of the first-person targeting sort. Also enables the existing swap() (the
  `FABLE_RETAIL_STLP_VECTOR_SWAP` block's guard widened with `|| defined(FABLE_RETAIL_STLP_SORT_INTERNALS)`).
- `FABLE_RETAIL_STLP_HEAP` (row 0x00BEEDA0): STLport 4.6 _heap.c __pop_heap (out of line, `__fastcall`) and __adjust_heap
  (declaration, 0x00BEEA00).
- `FABLE_RETAIL_STLP_UNINIT_OUTOFLINE` (row 0x0076E4D0): the non-POD __uninitialized_copy / __uninitialized_fill_n out of line with the
  DEFAULT convention (that TU is not /Gr; 0x0076F780 / 0x0076F7D0), a third branch next to the existing `__fastcall`
  FABLE_STLP_OUTOFLINE_UNINIT_COPY / _FILL_N ones.
- `FABLE_RETAIL_STLP_ADVANCE` (row 0x0076E4D0): STLport 4.6 advance() with the bidirectional __advance for list iterators.

## engine/CVectorCompare_BestForTargeting.h (NEW; rows 0x00807250, 0x00808AA0)
- Ego_r.pdb layout (0x2c, class key V), FableWin access (GetCompareValueForPoint public 0x007F9660, GetCompareValue private
  0x007F9480). `FABLE_RETAIL_CVECTORCOMPARE_BESTFORTARGETING`: the FableWin operator()(const C3DVector&, const C3DVector&) const,
  inline (two named values with parenthesised initialisers -- the PAREN tuple lever of 0x00807250).
  `FABLE_RETAIL_CVECTORCOMPARE_BESTFORTARGETING_BODIES`: both scoring bodies as INLINE (COMDAT) definitions with the landed rows'
  logic: c2 knows the comparator is not captured but keeps the standard register kills at the calls (0x00808AA0's lever).

## engine/CTCSmashableBase.h (NEW; row 0x008A8010)
- CTCSmashableBase : CTCBase (Ego_r.pdb, 0x18: LastWorldFrameHit +0x0C, Smashed +0x10, Definition CDefPointer<const CSmashableDef>
  +0x14) and CTCSmashableWindow : CTCSmashableBase. Declarations only, no opt-in needed (a new file changes no other TU): the
  public static __fastcall StatGetInterfaceType() = TCI_SMASHABLE (78, FableWin.pdb; inline in release, 0x008A80A7), OnDie /
  GetInterfaceType overrides, protected OnCreate / OnInitialActivate and the protected virtual Smash (slot 28,
  ?Smash@CTCSmashableBase@@MAEXXZ 0x007EED20), public CTCSmashableWindow::Smash override (?Smash@CTCSmashableWindow@@UAEXXZ,
  0x007F4E10 landed). Defines FABLE_RETAIL_CTCBASE (the polymorphic CTCBase with its 28 slots).

## engine/C3DVector.h
- `FABLE_RETAIL_GFFASTONEOVERSQRT_NEWTON_PAREN` (rows 0x00963880, 0x0096E830): GFFastOneOverSqrt's Newton step written
  `(3.0f-(r*r*value))*r*0.5f` -- same instructions, one more no-code PAREN tuple (81-tuple scheduling-region cap).

## AI state groups / combat (rows 0x00963880, 0x0096E830)
- engine/CAIStateGroup_VersusEnemyBase.h `FABLE_RETAIL_CAISTATEGROUP_VERSUSENEMYBASE`: class on CAIStateGroupBase, protected
  `CIntelligentPointer<CThingCreatureBase> PTargetThing` +0x14 (Ego_r name).
- engine/CAIStateGroup_VersusScorpionKing.h `FABLE_RETAIL_CAISTATEGROUP_VERSUSSCORPIONKING`: GoClockwise +0x1C, PArenaCentreThing
  +0x1D, private FindChargeEvent (0x00963810) / IsScorpionChargingAtMe.
- engine/CAIStateGroup_MeleeCombatNearbyBase.h `FABLE_RETAIL_CAISTATEGROUP_MELEECOMBATNEARBYBASE`: protected PEnemy +0x14.
- engine/CAIStateGroup_MeleeCombatNearby.h `FABLE_RETAIL_CAISTATEGROUP_MELEECOMBATNEARBY`: private const
  IsEnemyCreatureInCombatPosition (0x0096E700) / IsEnemyCreatureNearCombatPosition, bool Archer +0x1C; nested
  `..._COMBATMANAGER`: the Ego_r private inline GetCombatManager() = GetWorld().GetCombatManager().
- engine/CCombatManager.h `FABLE_RETAIL_CCOMBATMANAGER_PREFERREDPOS`: GetPreferredPos(const CThingCreatureBase&) const 0x006EA770.
- engine/CWorld.h `FABLE_RETAIL_CWORLD_COMBATMANAGER`: inline GetCombatManager() (FableWin 0x01D8B290).

## Rendering
- engine/CEngineInternalPrimitiveBase.h `FABLE_RETAIL_CENGINEINTERNALPRIMITIVEBASE_CLASS` and
  engine/CEngineInternalPrimitive2DViewport.h `FABLE_RETAIL_CENGINEINTERNALPRIMITIVE2DVIEWPORT_MEMBERS` (row 0x00BE1C10): the
  polymorphic base and the typed 2D viewport primitive (Ego_r members, retail offsets); NEW engine/C2DAlphaMaskInfo.h and
  engine/CEnginePrimitive2DViewport.h (Ego_r layouts) for it.
- engine/CEngineScreenEffectRadialBlurRenderer.h `FABLE_RETAIL_CENGINESCREENEFFECTRADIALBLURRENDERER_MEMBERS` (row 0x00B5E0F0).
- engine/CRenderStateManager.h `FABLE_RETAIL_CRENDERSTATEMANAGER_OUTLINE_INLINE` (row 0x00BF01E0): SetColourWriteEnable(bool),
  SetStencilOpPass, SetStencilWriteMask inline (no retail copy).
- engine/CEnginePrimitiveRenderUtil.h `FABLE_RETAIL_CENGINEPRIMITIVERENDERUTIL_OUTLINESTENCIL` and NEW engine/CEngineMeshEffect.h
  `FABLE_RETAIL_CENGINEMESHEFFECT` (row 0x00BF01E0).
- engine/CEngineWaterRenderer.h `FABLE_RETAIL_CENGINEWATERRENDERER_SEAMIDGROUND`, engine/CEngineWaterSettings.h
  `FABLE_RETAIL_CENGINEWATERSETTINGS_COLOUR` (parked row 0x00B6FA40).
- engine/CEngineLightingManager.h `FABLE_RETAIL_CENGINELIGHTINGMANAGER_MEMBERS`, engine/CEnginePrimitiveRenderer.h
  `FABLE_RETAIL_CENGINEPRIMITIVERENDERER_SCENELIST`, engine/CEnginePrimitiveTypeInfo.h `FABLE_RETAIL_CENGINEPRIMITIVETYPEINFO_REGISTER`,
  engine/CMemoryAllocator.h `FABLE_RETAIL_CMEMORYALLOCATOR_ALIGNED` (row 0x00B482A0).

## Strings / CRC (row 0x00B21A50)
- fable_string.h `FABLE_RETAIL_CCHARSTRING_GETCRC_INLINE`: GetCRC() as retail's LTCG callers expand it.
- NEW engine/CCRC.h `FABLE_RETAIL_CCRC_CALC_BODY`: the Calc body (landed 0x004014A0) visible, so VC7.1 inlines it per site.

## Inventory / UI (rows 0x005A887D, 0x005B6767)
- engine/CTCInventoryTrade.h `FABLE_RETAIL_CTCINVENTORYTRADE_BUYPOSSIBILITYPC`; engine/NUISystem_CDeletion.h
  `FABLE_RETAIL_CDELETION_BODIES` (the two constructors' bodies visible, kept out of line).
- engine/CTCInventoryClothing.h `FABLE_RETAIL_CTCINVENTORYCLOTHING_ARMOUR` (nested ArgPasser + the two PDB methods).

## engine/LSocket.h (row 0x008F99A0 LHNetworkLib::LSocket::Receive(vector<char>&))
- `FABLE_RETAIL_LSOCKET_LADDRESS`: `LAddress() { ClearData(); }` and `void ClearData()` (Ego_r.pdb / FableWin.pdb
  `??0LAddress@LHNetworkLib@@QAE@XZ`, `?ClearData@LAddress@LHNetworkLib@@QAEXXZ`, public; inline in retail: `ServerName = ""`,
  then ServerPort, ServerIP, TransportProtocol, TransportFlags zeroed -- retail 0x008F99B4..0x008F99E6).
- `FABLE_RETAIL_LSOCKET_RECEIVE`: declarations of the LSocket.cpp members the one-argument Receive reaches, with Ego_r.pdb
  access: public static `ResolveHost(const std::string&)` 0x008F86A0 / `ResolveHost(LAddress&, unsigned long&)` 0x008F86F0
  (`SI`), public `CheckReadSize()` 0x008F8370 and `Receive(vector<char>&, LAddress&, const LAddress&)` 0x008F9320 (`QAE`),
  private static `CreateSocket` 0x008F87A0 / `ConvertSockaddr2LAddress` 0x008F8BB0 (`CI`), private `SetNonBlocking` 0x008F8870
  (`AAE`). `FABLE_FASTCALL` (rebuild_abi.h, included only under the opt-in) for the static members.
- engine/LHNetworkTypes.h: an UNCHANGED copy, present only so the overlay's LSocket.h resolves its relative include (cl.exe under
  wine hangs instead of reporting C1083 when it does not). Do not land it.
