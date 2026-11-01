# Chunk 687 (NA030, 520-672 band) -- 7 of 10 landed (2026-11-01)

Seven rows landed (`verify.log`; bindings multi-check 0), three of them vendor text compiled from source as retail did: zlib 1.2.1 trees.c
`gen_bitlen` 0x00C04020 (MATCH), DirectShow strmbase wxutil.cpp `WaitDispatchingMessages` 0x00CB09A0, libvorbis 1.0.1 floor1.c `inspect_error`
0x00CA2480; the engine rows CSoundManagerLH ctor 0x00A3EF00 (RTTI; the label said CMainGameComponent), CEaxContext::SetReverbAllParameters 0x00C68D70
(libpk), COptimisedVertex::LimitGroupCount 0x00ACA5F0 (/Gr), and the STLport `std::deque<C2DVector>::_M_insert_aux` 0x00A0F4B0 at its true 1462-byte
extent: the catalog had split one function (single epilogue at 0x00A0FA66) into three rows 0x00A0F4B0 / 0x00A0F6EA / 0x00A0F98B; the two mid-start
rows were removed from the manifest and the catalog (BOUNDS_FIX.tsv: len 1462 + two MIDSTART skips; known functions 57,613 -> 57,611). All nine
non-zlib labels were propagated stand-ins; 27 labels applied (`ghidra_out/labels_chunk687_2026-10-27.tsv`, incl. the libpk sound-system family
landed under CWorldManager / CGameflowAssistanceScript / CMainGameComponent / CInputEvent / CMouseCursor stand-ins, and ClassifyAudioExtension
landed as ClassifyShaderExtension).

Decisions at landing (coordinator, under the owner delegation): the three libpk naming items (one class name for vtable 0x012B1E24,
CPKVoiceParams vs CSoundPlayParams_00c37840, CDebugStream vs CString) are queued c687 for naming wave 20 with the agent's evidence; vendor rows keep
`static` and the vendor text verbatim; the 0x00C73AF0 name parameter is `const unsigned short*` (the landed caller 0x00C62920 declares `const char*`:
queued). Per the owner's 2026-11-01 instruction the freed slot is not refilled (two concurrent agents from here on).

Parked: CAmbientEvent::Trigger 0x00C61D10 (DIFFER 579v581, 31 probes + flag sweep: Play-call pushes interleaved with the height x87 chain),
CAudioSource::ApplySoftware 0x00C733D0 (610v610, 13 probes: register choice), CreateDecoderStream 0x00C73AF0 (671v672, 13 probes + flags:
epilogue block placement). Best sources in `../../pending/2026-10-27_linux_hand687/differ/`. NAME_FIXES.tsv: 36 rows (queued c687). Lessons in LESSONS.md.
