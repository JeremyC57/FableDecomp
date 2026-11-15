#0x006BDC60 probes

| Attempt | Strategy | Grade | First residue |
|---|---|---|---|
| sol2_01_original_update | Full shared classes, genuine conditional GetDef temporary lifetime, copied music state | DIFFER500/492; strict0.9693 | Reset accessor stores offset before flag; early regionMusic declaration occupies edi across GetMapNumberAt and spills the vtable. |
| sol2_02_scoped_region_result | Original donor-proven Reset statement order; regionMusic declared after map-number query | RELOCATION_MATCH492/492; helper113/113 | Every instruction and frame slot equal.22 relocations independently bound; no helper debt. |

Standalone C++03 test retains the complete Update and actual GetDef helper bodies.2304 combinations exercise force request, pending playback, missing/dead/live hero, no cell/failed lookup/zero/positive/negative cell music, no map/no region/null region def, region music, paused state and preserved override/danger state. Checks both coordinate conversions, fresh position queries, pending/update call order, ignored outcomes, force-reset state and balanced handles. Exact006bdc60_TEST PASS under realVC71/Od/W3/GR- without/EHsc and g++64C++03.
