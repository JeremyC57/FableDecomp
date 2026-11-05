# Chunk 782 header overlay

Every change is an opt-in branch behind a new `FABLE_RETAIL_*` macro, or a new file. No default path changed, so no landed
includer can change bytes. As a sample, 10 landed includers and siblings were regraded read-only with the final overlay, and all
are still RELOCATION_MATCH (SIBLINGS.tsv). No row edits engine/CBankFile.h or rebuild_abi.h.

| header | change | opt-in macro | evidence | used by |
|---|---|---|---|---|
| engine/CV_MurderTwistFinalScript.h | CMurderTwistAssassin members and functions at retail offsets | FABLE_RETAIL_CV_MURDERTWISTFINALSCRIPT_ASSASSIN | Ego_r.pdb member list; retail offsets read from 0x00EB4760 | 0x00EB4760 (RELOCATION_MATCH) |
| engine/CEngineScreenEffectGlowRenderer.h | full Ego_r member list at retail offsets +0x0C..+0x220, CVertexShader / CPixelShader / CTVertexColTex1 members, private `bool InitialiseTextures()` 0x00B5ECE0. The old opaque class stays in the `#else` | FABLE_RETAIL_CENGINESCREENEFFECTGLOWRENDERER_MEMBERS (inside the existing FABLE_RETAIL_CENGINESCREENEFFECTGLOWRENDERER branch) | Ego_r.pdb layout; every store offset in 0x00B5F090 | 0x00B5F090 (RELOCATION_MATCH) |
| engine/CEngineComponent.h | members `protected` instead of `private` (access only, no layout or byte change) | FABLE_RETAIL_CENGINECOMPONENT_PROTECTED | derived ctors store RenderEnabled themselves (0x00B5F10C `mov [esi+8],bl`, also 0x00B86C00); neither PDB has a setter | 0x00B5F090 |
| engine/CTVertexColTex1.h | NEW: X, Y, Z, DiffuseColour, U1, V1 (0x18 bytes) | new file | Ego_r.pdb CTVertexColTex1; the glow quads' 0x18 stride | 0x00B5F090 |
| stlport/fable_containers.h | `vector::erase(iterator)` declared only, so it is called out of line | FABLE_RETAIL_STLP_VECTOR_ERASE_OUTOFLINE | retail AttackPeople calls erase 0x00CD3152 | 0x00DFD600 (parked) |
| engine/CEnvironment.h | `void TransitionToTheme(long channel, long priority, long theme, float time)` 0x006B3800, declaration only | FABLE_RETAIL_CENVIRONMENT_TRANSITIONTOTHEME (inside FABLE_RETAIL_CENVIRONMENT) | Ego_r ?TransitionToTheme@CEnvironment@@QAEXJJJM@Z; `ret 10h` | 0x007941D0 (RELOCATION_MATCH) |
| engine/NGraphicAppearance_CTCGraphicAppearance.h | inline `PeekLightingChannel()`, declared `SetLightingChannel(const CEnginePrimitiveThemeChannelDesc&, bool, bool)` 0x004BFB40, and LightingChannel at +0x40 (+0x0C..+0x3F kept opaque) | FABLE_RETAIL_CTCGRAPHICAPPEARANCE_LIGHTINGCHANNEL | FableWin ?PeekLightingChannel@..QBEABVCEnginePrimitiveThemeChannelDesc@@XZ (retail inlines it: `mov edx,[ecx+40h]`); Ego_r layout with retail STLport sizes (two 0xC maps, PTCAnimation +0x24, CachedStaticFocalPosOffset +0x28, Scale +0x34, two 4-byte lists, then LightingChannel +0x40) | 0x007941D0 |
| engine/CTCVolumeContainmentTracker.h | Ego_r members after PThingInside (CurrentThemeChannel +0x14, PreviouslyUpdated +0x18, PDef +0x1C, UpdateLightingChannels +0x20) and inline `SetUpdateLightingChannels(bool)` | FABLE_RETAIL_CTCVOLUMECONTAINMENTTRACKER_LIGHTING | Ego_r layout (no container, so the offsets are retail's); FableWin ?SetUpdateLightingChannels@..QAEX_N@Z; retail `mov [eax+20h],bl` | 0x007941D0 |
| engine/CEnginePrimitiveThemeChannelDesc.h | inline 3-byte constructor `(index0, index1, blend_factor)` | FABLE_RETAIL_CENGINEPRIMITIVETHEMECHANNELDESC_INDICES | FableWin ??0CEnginePrimitiveThemeChannelDesc@@QAE@EEE@Z; retail stores +0xB0 = 5, +0xB1 = 5, +0xB2 = 0xFF | 0x007941D0 |
| engine/CSpecialEffectsDef.h | (a) inline `static CCharString __fastcall StatGetTypeName()`; (b) an `inline __declspec(noinline)` GetEffectFromString body (CRC of the name, then the map lookup), plus a `CCRC { static Calc }` declaration | (a) FABLE_RETAIL_CSPECIALEFFECTSDEF_TYPENAME; (b) FABLE_RETAIL_CSPECIALEFFECTSDEF_GETEFFECT_BODY | (a) FableWin ?StatGetTypeName@CSpecialEffectsDef@@SI?AVCCharString@@XZ (literal of the landed 0x00669580); (b) EXTRA_RULES chunk 422 same-TU noinline body: without it VC7.1 reloads the hand-effect index from its slot, retail keeps it in edi (0x0079476B) | 0x007941D0 |
| engine/CSpecialAbilitiesUnholyPowerDef.h | NEW: CParentDefClassBase-derived def, the 22 Ego_r members in order with 12-byte STLport vectors (StartPowerupSound +0x3C .. LargeRingRadius +0xF8) | new file (needs FABLE_RETAIL_CDEFSTRINGTABLE + FABLE_RETAIL_CCHARSTRING for engine/CDefString.h) | Ego_r.pdb struct layout; nine offsets read by 0x007941D0 | 0x007941D0 |

Notes for landing:
- engine/CEnginePrimitiveThemeChannelDesc.h is also included by the repo headers CEngineLightingManager.h and CEnginePrimitive.h.
  A row that includes one of those before this header and defines FABLE_RETAIL_CENGINEPRIMITIVETHEMECHANNELDESC_INDICES gets the
  repo copy, because MSVC searches the including file's directory first. That holds only while the overlay is not merged; after the
  merge there is a single copy.
- The overlay copies of CTCVolumeContainmentTracker.h and NGraphicAppearance_CTCGraphicAppearance.h include
  CEnginePrimitiveThemeChannelDesc.h from the same hdr/engine directory, so they get the overlay copy.
- The GetEffectFromString body in (b) is a same-TU stand-in for the out-of-line 0x00669D30. It follows the landed 0x00669D30 shape,
  but this TU does not grade it.
