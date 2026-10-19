# UI initialization, mesh definition and queen navigation

00547160 (217): actual scrolling-component header with native state/child views. Repeated GetCurrentState calls and selected-child fallback match retail. Fixtures cover empty, positive/negative maxima, ties, callback mutation, height and untouched state. Four total grades.

005500b0 (210): RTTI/callers establish CMesh owner. Native definition pointer copies retain references and release after all fields. Fixture exercises copy elision disabled, shared/last references, numeric conversion, vector fields and unaffected bytes. Four grades including AddRef correction.

0095a6c0 (259): actual queen header, real virtual destruction/movement slots and documented retail circle-vector offsets. Fixtures cover empty/invalid state, repeated movement, blocked route, result persistence and marker index. Three grades.

All independently RELOCATION_MATCH + PASS, with verbatim-source host and canonical VC7.1 fixtures.
