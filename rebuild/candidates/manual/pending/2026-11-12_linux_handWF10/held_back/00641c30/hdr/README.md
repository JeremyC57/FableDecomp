engine/CEnvironment.h: FABLE_RETAIL_CENVIRONMENT_LIGHT_ATTENUATION -- inline PeekDiffuseColour (GameTickState+4 = retail +0x58) and PeekSunlightAttenuatorAngleFadeStart/End (FableWin 0x0222AF60/AFA0/AFC0, emitted in CTCLight.obj).
engine/CGameTimeManager.h: FABLE_RETAIL_CGAMETIMEMANAGER_GETTIME -- inline GetTime() { return Time; } (retail 0x006415C4 `mov ecx,[eax+8]`).
engine/CEnginePrimitiveLight.h: FABLE_RETAIL_CENGINEPRIMITIVELIGHT_INLINE_CTOR -- the landed 0x00642000 ctor body inline (expanded in DrawLight 0x00641A7F); needs FABLE_RETAIL_CENGINEPRIMITIVE_ID_CTOR.
engine/NGraphicAppearance_CTCGraphicAppearance.h: FABLE_RETAIL_CTCGRAPHICAPPEARANCE_PEEKLIGHTINGCHANNEL -- inline PeekLightingChannel() for the MOVEMENT layout (retail 0x006419CA `mov cl,[eax+40h]`).
