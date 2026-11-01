# 2026-10-27_linux_hand673 — chunk 673 (NA018, never-attempted pool)

Landed 2026-11-01 via `land_chunk.sh 673`: 10/10 (1 MATCH, 9 RELOCATION_MATCH), bindings multi-check 0. Eight destructors (3 of 8 labels wrong;
five quest scripts named RTTI-less via ctor -> Alloc -> registration string) and the two gap-scan rows proven as code, not library bytes:
CPosPassThru::QueryInterface (DirectShow strmbase compiled from DX9 SDK source) and the `_$E1` static initializer of
CConversationAttitude::GeneralAttitudeToAnimName. NAME_FIXES.tsv (28 rows) queued as c673.
Decisions (coordinator under owner delegation 2026-11-01): empty-bodied PDB ctors in the ParticleMeshGroup and script TUs exist only to emit vtables (chunks 664-672 practice);
the `_$E1`/`_$E2` RELOC_CONFLICT is the compiler's per-TU naming, expected. Follow-ups: catalogue CPosPassThru AddRef/Release/thunks 0x00CAFC50-0x00CAFC90;
at the c673 respell check whether 0x00B25D80's RemoveDeviceResetCallback global is the display manager. Header debt (NOTES.md): CLevelScriptBase out-of-line dtor retail lacks; CEngine/CIEngine/CSimpleMemoryPool headers flattened.
