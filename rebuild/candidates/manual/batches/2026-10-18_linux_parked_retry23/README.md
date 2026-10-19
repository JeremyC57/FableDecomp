# Material map lookup

00ac1160 C3DMeshFileMaterialChunk::GetMap (263 bytes) independently verified RELOCATION_MATCH + PASS and landed. Source uses the shared material-chunk header through a method-only view; inherited superchunk helpers preserve the receiver address. The map type is at retail+0x2c, and MMAP literal/helper addresses were checked. Explicit non-inlined wrappers preserve the retail counted-handle lifetime and known helper boundaries.

The self-contained fixture embeds the actual source and checks first/middle/last matches, absent type and empty collection, helper receiver/literal, iteration counts, returned reference count, callback object and destruction order. Host g++ and canonical VC7.1/Wine PASS. Eight agent-recorded grades include comment-only rechecks; coordinator integration is grade9 (shared-owner declaration only), exact.
