# Spline previous-frame sampling

00b20fd0 (177): native cardinal-spline/container layouts shared with landed Update00b21160. Preserves startup-only evaluation, count-change copy then current evaluation, and stable-count current temporary evaluation followed by previous-frame output; third float argument remains unused. Exact-source fixtures test unequal counts both directions, startup-only, stable count, copied control points/tension/endpoint flag, output/callback order and unchanged other state. Three total grades; final row from the retiring Sol worker.

All independently RELOCATION_MATCH + PASS, with verbatim-source host and canonical VC7.1 fixtures.
