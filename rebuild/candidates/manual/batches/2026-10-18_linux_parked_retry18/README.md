# Material serialization and skipped frame-start recovery

00abf6b0 C3DMeshMaterial::SaveBinary (250 bytes) and 00b8dbe0 CEngineOcclusionManager::FrameStart (225 bytes), independently RELOCATION_MATCH + PASS. The latter was previously skipped/unattempted and was authored through ten counted grades. The serializer uses a method-only view of the shared stream header (author grades 1–2; integration grade 3). Filename prefix ownership follows constructor 00abef60: vptr at +0 and reference count at +4.

Actual-source fixtures check serialized byte order, optional filenames, null string data, negative material index and boolean patterns; and frame entry/count swaps, both vector swaps before callbacks, resize receiver/order, empty queues and frame wraparound. FrameStart uses the shared manager header for scalar fields and local 12-byte retail vector views because donor containers occupy 16 bytes. The second queue contains handle pairs, not strings. The unused resize fill value is intentionally unread for resize(0). Test/source RetailWord aliases were unified without changing the target unsigned-long type or target behavior.

Both host fixtures and canonical VC7.1/Wine tests pass. Host-only storage adapters accommodate 64-bit widths; target tests include the actual shared headers. The final landing evidence is in landing_evidence.txt.
