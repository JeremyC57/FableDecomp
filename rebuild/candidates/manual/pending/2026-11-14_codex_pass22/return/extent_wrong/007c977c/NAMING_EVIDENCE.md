# 0x007C977C -- extent correction

Assigned007c977c begins push esi/mov esi,ecx, then je007c9894. The branch consumes flags from cmp al,bl at007c977a. Whole entry007c9770 loads fullState from[esp+8], allocates10h stack bytes, saves ebx, zeroes ebx, compares the flag, then reaches the assigned start. Final ret8 occupies007c9899..007c989b; int3 padding begins007c989c. Thus the whole function is300bytes and the assigned288-byte slice omits12bytes of prologue.

This is already-landed CTCCreatureGeneratorCreator::OnSerialise(CPersistContext&,bool), native vtable0126eb3c slot1. names/catalog_names.tsv line22056 and names/landed_sources.tsv line22056 identify the300-byte whole function and canonical source00/7c/CTCCreatureGeneratorCreator_OnSerialise_007c9770.cpp. names/labels/labels_boundaries_2026-10-22.tsv line71 already records this exact split and original slot identity; labels_manifest_sync_2026-11-03.tsv line13576 resynchronizes the head's name from vfunc_1. The supplied landed neighbour was read in full; its full-state serialization, remapped generator UID and frame counter explain the body. Its legacy local models are not promoted into new source.

No fragment or duplicate function was authored, no new byte match is claimed, and no original source was changed. This is an EXTENT_WRONG/alias record for the100-row manifest.
