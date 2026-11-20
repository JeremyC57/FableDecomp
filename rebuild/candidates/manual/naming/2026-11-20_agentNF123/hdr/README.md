# NF123 header overlay (round 10, PRIORITY slice 8)

Base copies of every overlaid header are in `../orig/` (byte-equal to the repo at the start of this pass, re-checked at hand-back).
Every addition is an `#ifdef` opt-in with a member-specific macro; without the macro each header is token-identical to the repo copy.
`pin_asserts.py rebuild/include hdr <header>` was run on every overlaid header: LHHttp2.h / LHHttpHeaderStatus.h / LSocket.h / CAIBrain.h
pinned 0; CScriptedMapBrush.h pinned 14 (its 14 FABLE_STATIC_ASSERTs sit below the 8-line THINGSETS block, now FABLE_STATIC_ASSERT_AT(<repo
line>, ...)); CTCDPhysicalObstruction.h pinned 1 (the sizeof assert below the 5-line SETPARAMS block, FABLE_STATIC_ASSERT_AT(65, ...)).
No landed row or test defines any of the new macros (grep of rebuild/src/compiled and rebuild/tests), so the opt-ins change none of
the landed includers (LHHttp2.h: 15 includers, LHHttpHeaderStatus.h: 3, LSocket.h: 9, CScriptedMapBrush.h: 14, CAIBrain.h: 277,
CTCDPhysicalObstruction.h: 7).

| header | opt-in | adds | rows |
|---|---|---|---|
| engine/LHHttpHeaderStatus.h | FABLE_RETAIL_LHHTTPHEADERSTATUS_RESET | `void Reset();` (0x0083FB60, Ego_r.pdb) in the LOCATION branch | 0x0083FB60 |
| engine/LHHttp2.h | FABLE_RETAIL_LHHTTP2_DTOR | `~LHHttpHeaders() { Reset(); }` in the non-SCOREBOARD branch (Reset's body visible, as the LHHttp TU); `~LHHttp2();` (when SCOREBOARD is off) and `LH_RETURN Close();` declarations | 0x008410D0 |
| engine/LHHttp2.h | FABLE_RETAIL_LHHTTP2_SENDREQUEST | `SendRawData`, `ResetRequest`, `SendRequestAsync` declarations (Ego_r / FableWin spellings) | 0x008405F0 |
| engine/LHHttp2.h | FABLE_RETAIL_LHHTTP2_RESET | `ResetDocumentParts();` (0x0083EB00), `Reset();` (0x0083FF80) declarations | 0x0083FF80, 0x008410D0 |
| engine/CScriptedMapBrush.h | FABLE_RETAIL_CSCRIPTEDMAPBRUSH_THINGSETS | ThingsToDelete / ThingsToAdd typed `std::set<unsigned __int64>` (Ego_r.pdb; 12 bytes each, same offsets; ThingsToMove stays opaque); `#else` keeps the opaque arrays. The includer brings stlport/fable_map.h | 0x0082E8F0, 0x0082E9A0 (+ their tests) |
| engine/CAIBrain.h | FABLE_RETAIL_CAIBRAIN_ISUPDATEZONEACTIVE | `bool IsUpdateZoneActiveThisFrame() const;` (0x0088B810, Ego_r 0x0082F5E0 + FableWin 0x02804440) after the STATEGROUPTEXT block | 0x00830130 (+ test) |
| engine/CTCDPhysicalObstruction.h | FABLE_RETAIL_CTCDPHYSICALOBSTRUCTION_SETPARAMS | `void SetParams(const C3DVector&, const C3DVector&, float);` (0x007FD3F0, Ego_r 0x007F86A0 + FableWin 0x0278A9F0) after the CREATE block | 0x007FD3F0, 0x008CAF10 |
| engine/LSocket.h | FABLE_RETAIL_LSOCKET_DTOR | `~LSocket();` (0x008F9780, Ego_r + FableWin ??1LSocket@LHNetworkLib@@QAE@XZ) declaration | 0x0083FF80, 0x008410D0 |

`engine/LHNetworkTypes.h` is an UNMODIFIED copy: `LHHttpHeaderStatus.h` / `LHHttp2.h` include their siblings by relative name
(`#include "LHHttp2.h"`), which cl resolves from the overlay directory first; without the sibling copies in hdr/engine the compile
spun (missing include under Wine). Copying the overlay into the repo leaves it byte-identical to the repo file.

Sibling overlays read first: NF120 (CTCCreatureModeManager.h, CTCFishing.h, CThingCreatureBase.h) and NF122 (CGameTimeManager.h,
CTCDParticleEmitter.h, CTCRegionFollower.h, NScreenEffect_CScreenEffectPriorityStack.h) touch none of these headers. At hand-back no other NF12x overlay carries CAIBrain.h, CScriptedMapBrush.h or CTCDPhysicalObstruction.h.
Rows that use opt-ins already in the repo (not overlay changes): 0x0088B7D0 (CAIBRAIN_STATEGROUPTEXT), 0x007FD3D0 / 0x008C44B0
(CTCDPHYSICALOBSTRUCTION_CREATE).
