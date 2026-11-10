# Vendor naming and audit

CCodaContext and its member/list/descriptor names come from GameCODA SDK2.10 CodeView/CODA.h. Native offsets are12 bytes below the SDK debug donor; SDK release itself retains8 more base bytes than this native fork (see VENDOR_COMPILER_EVIDENCE.txt). All native accesses agree with shared header: platform4; list free counts28,3c,50,64 (stride14); maxima6c,70,74,78. Complete context is a8 bytes; unaccessed tail storage remains present.

GetResourceCounts is explicitly descriptive, not a PDB-proven method spelling. SDK2.10 does not contain this exact query declaration. The vendor exception in the row package permits this label; do not claim direct PDB evidence or guessed constness as proof. Native ret4 and EAX output pointer support the selected pointer argument/result model. API thunk00c333f0 loads global CODA at0143bb58 and jumps to00c3d030, corroborating the owner rather than neighbouring legacy CAudioSystem placeholder names.

There are zero external relocations in the selected leaf: list GetNumFree bodies inline to field loads. All35 native instructions and94 bytes match frozen034 exactly. Explicit two-arm output stores restore the native entry output-pointer load. No switch/literal/vtable references require binding.
