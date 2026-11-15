# Independent main review

1331 target bytes; two called helpers222 bytes;51 relocations;4096 target cases plus12 stock-helper and3300 bound cases. Fresh main RTM13.10.3077 build and exact masked comparison passed, as did complete VC71 /Od /W3 /GR- and g++64 C++03 fixture runs. Source, fixture, original names/locals, full extents and call binding evidence reviewed. All statically called emitted helper bodies and their relocations checked. Combined49-row/149-header regression preserves every emitted C++ code section and relocation name. No helpers counted as additional reserved rows.
