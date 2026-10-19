# Weather particle shader constants

00b52160 (235 bytes): actual shared weather-renderer header agrees with native Settings+114 and mask list+66c. PDB CWeatherMask is92 bytes, bounding-box min/max+10/+1c and inverse4x3 matrix+28. Uploads particle span with exact8388608.0f, then three matrix columns and both bounds for each of three masks. Audited native shader setters989B00/989A60 and singleton1436E14; no shader-manager or mask class header exists.

Exact-source fixtures verify all16 uploads and all matrix components, bounding values, increasing indices, native layouts, manager replacement on first callback, live index mutation and a later matrix-column mutation. Main independently reread the entire native body and setter entry points before verification. One new source grade plus UNCERTAIN historical reservation4 (reserved5/10); prior placeholder was not a proven authored failure.
All independently byte-verified (MATCH or RELOCATION_MATCH) + PASS, with verbatim-source host and canonical VC7.1 fixtures.
