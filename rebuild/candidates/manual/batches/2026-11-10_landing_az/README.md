# 2026-11-10 landing az -- chunk 1020 (3 of 11 header-port near misses)

Source: `pending/2026-11-10_linux_hand1020/`. vland --land 3 RELOCATION_MATCH, tests PASS:
- 0x009E81E0 CPersistTraits<C2DVector>::TransferIn (catalog CBrainBehaviour wrong): visible CBasicString Compare / operator==(const T*)
  bodies, a real `if (parser.ReadAsSymbol() == ")") return;`, one named CPersistTraits<float> with its TransferIn body visible + noinline.
- 0x006DC6A0 CPersistTraits<CAttackHistoryCombo>::TransferBinaryIn (catalog CHeroStatIncreaseData wrong; real extent 447 bytes, the
  catalog row ran on): inline stream.ReadType<T>() / ReadULONG / ReadFloat reads + push_back(std::make_pair(type, attribute)).
- 0x005D8978 CPersistContext::TransferMap<long,unsigned long>: FABLE_RETAIL_CCHARSTRING_FORCEINLINE_TEXT_EQ + the visible generic
  CPersistTraits<T>::TransferOut body (both save-loop traits share one stack slot).
Manifest modules respelled for the two CPersistTraits rows.
Headers (merged onto HEAD, base aca6a572e; hdrcheck2 default=same, 14 files): CPersistTraits, CPersistContext, CSymbolMap,
CDataOutputStream, CCombatManager, CDefendingCombatantInfo, stlport/fable_map, CEngineCamera SCREENTOWORLD (camera helpers as
FABLE_FORCEINLINE for the parked 0x00BCF4F0 draft only -- review against the inline ladder before any row lands on it),
CEnginePrimitiveRenderer2D, CEngineInternalPrimitiveAnimatedMesh (SIZE128: retail's class is 0x128, Ego_r's 0x12C), the real
CEngineInternalPrimitive2DAnimatedMesh class; new CSoundMap, CAttackHistoryCombo, C3DPositionInterpolator.
Parked (differ/): 0x0047042A (one lea operand order), 0x005C0A98 (temporary address pushed after Get(); cut 369), 0x0067E290 (register
rotation; cut 273), 0x006E94D0 IsCreatureInAttackRingSector (one shared frame slot), 0x00BCF4F0 (Scale-block x87 order), 0x00A229E0
(frame 0x50 v 0x4C), 0x006668A0 / 0x00681E60 (not ported). Queued land1011ay: 7 name fixes.
