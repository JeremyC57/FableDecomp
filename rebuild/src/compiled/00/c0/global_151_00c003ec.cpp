// #151 import thunk: linker-synthesised from an import-library record, not Fable source (grade toolchain_library).
// Toolchain-library: wsock32.lib(WSOCK32.dll) ___WSAFDIsSet@8 -- 6 bytes verified against Fable.exe 0x00C003EC
//
// Retail 0x00C003EC is `jmp dword ptr [__imp____WSAFDIsSet@8]` (FF 25 60 05 44 01). The IAT slot 0x01440560 is
// WSOCK32.dll!#151 in the retail import directory. The import library is the Platform SDK wsock32.lib (named from
// the WinDDK 3790.1830 lib/wnet/i386 copy); its member `WSOCK32.dll` that defines ___WSAFDIsSet@8 is a short
// import record (CODE, ORDINAL): the linker emits this thunk for it whenever a caller references ___WSAFDIsSet@8
// rather than __imp____WSAFDIsSet@8.
// The build links wsock32.lib for these bytes; nothing here is compiled and there is no body to write.
// Proof: python3 tools/decomp_pipeline/libsig/lib_verify.py 00c003ec -> MATCH;
// oracle row in rebuild/oracles/library_signatures.tsv; docs/pipeline/LIBRARY_SIGNATURES.md.
