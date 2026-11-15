# Attempts — 008E3A20

| Source strategy | Builds | Result |
|---|---|---|
|1. Empty user destructor using the real shared class and original implicit filter cleanup|main_01, main_01b, main_01c|Two include prerequisites repaired (not source attempts); compiled116v115. Extra derived-vptr store and displaced loop alignment.|
|2. Original compiler-generated destructor, materialised by the real landed constructor|main_02, main_02b, main_02c|Header-only compile did not instantiate the leaf; adding the real constructor produced115v115. Corrected two inherited virtual declarations in a new opt-in; bytes remain exact. Constructor203v203 and generated scalar cleanup30v30 also exact.|

Two distinct strategies, six compiler invocations. No claim of six distinct attempts. Final selected build main_02c_complete_vtable. Twelve portable fixtures passed VC7.1 /Od /W3 /GR- and g++ C++03. No new packing or inline-control devices. Existing shared intelligent-pointer packing is reused unchanged.
