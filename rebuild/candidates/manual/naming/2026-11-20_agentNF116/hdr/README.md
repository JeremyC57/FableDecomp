# NF116 header overlay (round 10)

Three repo headers, each copied unchanged from rebuild/include/engine/ (bases in ../orig/engine/) plus one `#ifdef` opt-in. The
default path (macro undefined) is token-identical to the repo header in all three. Other agents' overlays (NF114:
NUISystem_CUIScreenUnitConverter.h; NF115: CThingCreatureBase.h, CThingSearchTools.h) touch none of these headers.

| header | opt-in | what it adds | rows |
|---|---|---|---|
| engine/LHHttp2.h | `FABLE_RETAIL_LHHTTP2_OPEN` | `LH_RETURN Open();` in LHHttp2's public section (Ego_r.pdb / FableWin.pdb `?Open@LHHttp2@LHNetworkLib@@QAE?AW4LH_RETURN@2@XZ`, 0x0083F9B0). Declaration only, no layout change. | 0x0083FB20 |
| engine/LSocket.h | `FABLE_RETAIL_LSOCKET_LADDRESS_NAMEPORT_CTOR` | `LAddress(const std::string& serverName, unsigned short serverPort);` in LHNetworkLib::LAddress (Ego_r.pdb `??0LAddress@LHNetworkLib@@QAE@ABV?$basic_string@...@G@Z`, 0x00841EE0). Declaration only. | 0x00841EE0 |
| engine/LHHttpHeaderStatus.h | `FABLE_RETAIL_LHHTTPHEADERSTATUS_LOCATION` | A retail branch: `#ifdef` the typed LHNetworkLib::LHHttpHeaderStatus (Ego_r member names and types, the generated header's offsets; ParseLocationData / GetLocationHost / GetLocationURI declared; needs FABLE_RETAIL_LHHTTP2), `#else` the generated record unchanged, `#endif`. | 0x0083F5C0, 0x0083F600 |

Static asserts: only LHHttpHeaderStatus.h has them. The retail branch adds 24 lines above them, so
`pin_asserts.py /home/user/FableDecomp/rebuild/include <hdr> engine/LHHttpHeaderStatus.h` pinned 8 (each `FABLE_STATIC_ASSERT` became
`FABLE_STATIC_ASSERT_AT(<repo line>, ...)`, lines 25..32 of the repo header). LHHttp2.h and LSocket.h have none (LHHttp2.h's size check
is a typedef, untouched).

Header users (landed includers, grep of rebuild/src/compiled + rebuild/include): engine/LHHttp2.h 14 (11 rows + CTCHeroOnlineScoreboard.h,
LHStringFunctions_bodies.h, LHHttpHeaders_bodies.h); engine/LSocket.h 8 rows; engine/LHHttpHeaderStatus.h 1 row (0x00569D30). None of
them defines any of the three new macros (grep), so the opt-ins change none of them.

Grading note: a quoted sibling include (`#include "LHNetworkTypes.h"` in LHHttp2.h / LSocket.h) does not resolve from an overlay
directory, so the rows were graded with `FABLE_HDR_OVERLAY=$S/w22/agentNF116/gov`: symlinks to the three hdr/ files plus a symlink to
the repo's engine/LHNetworkTypes.h. Once the headers are in rebuild/include/engine/ the siblings resolve normally. gov/ is not part of
the hand-back.
