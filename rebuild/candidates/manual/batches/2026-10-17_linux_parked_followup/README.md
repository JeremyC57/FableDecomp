# Parked-row follow-up (chunks 190-203): 2 exact wins

## Exact wins
- 00b45460 CEngineInternalPrimitiveShadowedSpotLight::Update RELOCATION_MATCH(164v164): coordinator re-grade of the final inline colour Set source confirms the alpha/B/G/R x87 schedule.
- 00687310 CMessageEvent::CMessageEvent RELOCATION_MATCH(189v189): raw eight-byte intelligent-pointer subobjects with explicit BaseInit/vtable stores defer pointer initialization until after the 0x40-byte payload copy.

## Improved but parked
- 00994180 CDataOutputStream::WriteDateAndTime DIFFER(161v163): all stream calls and byte fields align; only dword zero-extension around year/week differs. Tried unsigned-long temporaries, alias stores, union fields, and zero/OR expressions.
- 008c4ea0 CCreatureAction_ScorpionCharge::CCreatureAction_ScorpionCharge DIFFER(165v163): semantic constructor shape is exact; VC preserves the second input in EBX while retail reloads it from the stack.
- 006e7740 CGameScriptInterface::CGameScriptInterface DIFFER(148v173): body layout remains recovered but generated base/member/vtable scheduling and timer initialization remain unresolved.
- 00b4ce20 CEngineShadowRenderer::SetShadowClipVolumesToNeverShadow DIFFER(191v173): volatile/interleaved local sentinel writes reproduce the scalar operations but still miss retail destination-address/register scheduling.
- 008e78c0 NCreatureMode::CModeBriarRoseConcentrate::CModeBriarRoseConcentrate DIFFER(218v218): deferred raw P2/P3 initialization recovers the retail scalar/pointer order; residual relocation/scheduling difference remains.
