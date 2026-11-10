# Header default-path audit

PASS for the final106-header overlay, including the parked00903f30 headers: all72 pre-existing headers preserve default active tokens with pass18 opt-ins unset;34 headers are new. Every staged header was byte-compared with the exact include tree used by final_overlay_review grades. No source or header was changed by this audit.

Compared preprocessor token streams with legacy options unset and with every pre-existing FABLE_RETAIL macro enabled (144 comparisons). Only include-root paths and numeric suffixes of __LINE__-generated fable_static_assert typedef names were normalized; assertion expressions, fields, access labels, pragmas and function bodies were preserved. All default comparisons match. Blanket-legacy comparisons differ only in the same two harmless forward-declaration additions: CEnginePrimitiveAttachmentManager.h adds CEngineCamera/CArray/C3DBoundingSphere, and CThingMapManager.h adds NThingMap::CThingMap. No data, layout, access or function-body difference accompanies them. Return codes agree throughout. Blanket configurations are a preprocessing audit, not supported C++ translation units.

| Merged header | Default and legacy-path review |
|---|---|
| engine/C3DVector.h | Existing ADD2D and CROSS_X_GROUP branches retained; DIRECT, TO2D, magnitude declaration and CROSS_XY_GROUP are narrow opt-ins. Existing public scope and constructor/arithmetic bodies preserved. |
| engine/CThingCreatureBase.h | New declarations are public only inside COMBATRADIUS/COMBATTYPE. Existing explicit private label precedes data fields; protected/public member regions remain intact. |
| engine/CGameCameraManager.h | GENERALCASE introduces the original private method then restores public access. CAMTOOLS_TYPED selects the real existing-size member/accessor; opaque default member remains unchanged. |
| stlport/fable_containers.h | Native-wchar traits, list construction helper and not1 additions are individually guarded. Existing default list construction, class scopes and templates preserved. |
| engine/CThing.h | Original typed GetTC helper chain is wholly inside GETTC_ORIGINAL_CHAIN; the pre-existing default direct-Find body remains intact. |
| engine/CVectorMap.h | ORIGINAL_FIND guards the key/key comparator availability and positive Find arm; prior lower-bound and default Find branches remain intact. |
| engine/CIntelligentPointer.h | GET_ORIGINAL_CHAIN and COPY_ORIGINAL_CHAIN select their intended accessor/copy bodies. A single private GetPointer helper is available under their OR guard; the default Get/operator->/copy/SetPointer bodies and access scope are unchanged. No duplicate GetPointer declaration. |

Additional original script/action/enum headers are isolated in their respective opt-ins or new files; no existing default data fields or virtual declarations changed in the token comparisons.

The new camera-point hierarchy agrees with saved original PDB records: DefinitionBase has all original fields/access at offsets12..60 and size64; ShapeActivationBase adds no fields; GeneralCase retains private CameraStringLength/CameraCageRadius/CameraHeightOffset at64/68/72, four flags at76..79 and AutoGoBehindTime at80, size84. Original unaligned CoordBase/CoordAxis ranges at22/34 remain full-size opaque composite storage. No data member was omitted.

CStringAndCage retains the full existing CStringAndCageBase/CBase chain. Native sizes164 ->200 ->208 place its private AutoGoBehind at200 and AutoGoBehindTime at204; these are the original PDB suffix fields after container-size adjustment. Original public constructor/Update/GetModeType access is preserved. The visible derived constructor initializes the original five-argument base constructor, then both suffix fields, then inherited DefaultFOV; it does not replace or omit base fields. The four unary-plus float getter spellings preserve the original return types and data access, with signed-zero/extreme-value checks in the row test.

The recorded merged-header regrades independently reproduce all38 selected matches. This audit found no accidental class-scope, data-access, layout or default function-body change.
