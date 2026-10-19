# Viewport and sprite graphic-frame initialization

0054e640 and0041a8d0 (176 bytes each): RTTI identifies CViewport and CSprite, sharing the initialization sequence but using native GraphicIndex offsets170/178. Actual FrontEndManager/GraphicDataBank headers retained, plus actual CSprite header; CViewport has no available header. Native field views document donor offsets and inherited size.

Both fetch counted graphics-bank handles twice, call width/height slots21/22 with frame0 and convert unsigned dimensions to float. Exact-source fixtures test null/shared/final ownership, fastcall deletion callback and cleanup order, high unsigned values, singleton replacement and changed graphic index before the second lookup. Main also runs host with copy elision disabled. Total grades4/1, including viewport identity correction.

All independently RELOCATION_MATCH + PASS, with verbatim-source host and canonical VC7.1 fixtures.
