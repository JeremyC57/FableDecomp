# Original SDK proof, not a descriptive guess

The original GameCODA2.10 PC SDK CODAApi.lib member Release/SalOutputKS.obj defines `?SetConnectionState@CSalKsPin@@QAE_NW4KSSTATE@@@Z`. Its22bytes match the native assigned head exactly after its two relocations are masked. The original debug object contains the complete class records (SDK_TYPES.json): CSalKsPin is8bytes and publicly derives from CSalFile, also8bytes; its fields and access are already in shared CSalOutputKS.h. The actual game caller CSalOutputKS::CreateBuffer00C5E650 passes the pin object and the KSSTATE argument to this function. No original PDB name-set entry is available for this vendor library; original vendor COFF symbols/types plus byte-identical native code provide stronger direct identity proof. The old CKsObject label is not used.

The supplied107-byte extent contains three functions: SetConnectionState00C5E0C0..00C5E0D6 (22bytes), padding, a separate16-byte constructor00C5E0E0..00C5E0F0, then a59-byte event creation function00C5E0F0..00C5E12B. Only the assigned first head is reconstructed; BOUNDS_FIX.tsv reports the boundaries.

# Exact callees, GUID and SDK types

The original target has two relocations: offset10 names `__GUID_1d58c920_ac9b_11cf_a5d6_28db04c10000`, whose16bytes equal native012B90D8; offset15 calls `?SetProperty@CSalKsPin@@QAE_NABU_GUID@@KPAXK@Z`, native00C5E050. That original SDK SetProperty body independently matches all109 native bytes after its sole DeviceIoControl import relocation is masked. The prior bound name used CKsObject and GUID rather than the SDK's CSalKsPin and _GUID; NAME_FIXES.tsv records both that correction and the wrongly named GUID constant. The emitted target symbols exactly match the original SDK object's relocations.

Original debug types prove KSSTATE_STOP0/ACQUIRE1/PAUSE2/RUN3 and the complete KSPROPERTY_CONNECTION enum (including PRIORITY1 before DATAFORMAT2). The source uses KSPROPERTY_CONNECTION_STATE0 and sizeof(state)=4. The shared KernelStreaming.h retains the SDK KSSTATE; a new guarded section adds the original property enum and guarded UUID annotation. A new declaration-only option in CSalOutputKS.h adds the original public SetProperty member. No receiver layout changes. Both old header paths remain byte-for-byte unchanged.

The original UUID operand requires the compiler's __uuidof on MSVC. It is guarded by _MSC_VER; the other branch supplies the identical public Windows KS GUID as ordinary aggregate C++. This is SDK metadata, not embedded retail code. Both branches execute the same SetProperty call and return its bool. No helper implementations are carried.

# Tests and provenance

Eight standalone fixtures retain the full target body, cover all four states and true/false callee results, check the exact GUID, zero property id,4-byte value, original receiver and unmodified caller argument. VC7.1 /Od /W3 /GR- without /EHsc and64-bit g++ C++03 pass. The fixture uses32-bit Win32 DWORD on both hosts.

SDK_MATCH_PROOF.json stores the source-object hashes and nonbinary comparison results. Private SDK objects and native byte cuts are excluded from the handback. PDB name sets, RTTI, catalog/manifest, established/queued bindings and existing shared headers were searched before the original SDK objects settled the names.
