// #6 import thunk: linker-synthesised from an import-library record, not Fable source (grade toolchain_library).
// Toolchain-library: oleaut32.lib(OLEAUT32.dll) _SysFreeString@4 -- 6 bytes verified against Fable.exe 0x00CB5AAA
//
// Retail 0x00CB5AAA is `jmp dword ptr [__imp__SysFreeString@4]` (FF 25 48 02 44 01). The IAT slot 0x01440248 is
// OLEAUT32.dll!#6 in the retail import directory. The import library is the Platform SDK oleaut32.lib (named from
// the WinDDK 3790.1830 lib/wnet/i386 copy); its member `OLEAUT32.dll` that defines _SysFreeString@4 is a short
// import record (CODE, ORDINAL): the linker emits this thunk for it whenever a caller references _SysFreeString@4
// rather than __imp__SysFreeString@4. The address was outside the function manifest until the 2026-10-30 boundary
// pass added it (agent_source boundary).
// The build links oleaut32.lib for these bytes; nothing here is compiled and there is no body to write.
// Proof: python3 tools/decomp_pipeline/libsig/lib_verify.py 00cb5aaa -> MATCH;
// oracle row in rebuild/oracles/library_signatures.tsv; docs/pipeline/LIBRARY_SIGNATURES.md.
