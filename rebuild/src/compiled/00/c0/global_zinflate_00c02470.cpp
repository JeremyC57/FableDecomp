// _z_inflate: zlib 1.2.1 vendor library code, not Fable source (grade toolchain_library).
// Vendor-library: zlib-1.2.1(inflate.c) _z_inflate -- 5204 bytes verified against Fable.exe 0x00C02470
//
// Retail 0x00C02470..0x00C038C3 is the whole 5204-byte .text section (COMDAT, /Gy) of `inflate()` in inflate.c of the
// unmodified zlib 1.2.1 release (https://zlib.net/fossils/zlib-1.2.1.tar.gz, sha256 94ded520...; zlib License),
// compiled by cl 13.10.3077 with the recorded flags /O2 /Oy /W3 /MD /GS /DZ_PREFIX (Z_PREFIX spells it z_inflate). The
// 79 relocations all agree with retail: the two jump tables $L2785/$L2786 (same-section labels), z_crc32 0x00C05FD0 and
// z_adler32 0x00C03CB0 (their own vendor bytes sit there), fixedtables' lenfix/distfix and the error strings in .rdata.
// Retail version strings: "inflate 1.2.1 Copyright 1995-2003 Mark Adler", ZLIB_VERSION "1.2.1" at 0x0129CFE0.
// Owner direction (2026-11-08): third-party code is isolated, identified and linked from its unmodified source, not
// rewritten by hand. The build compiles the vendor TU (tools/decomp_pipeline/libsig/vendor_libs.tsv); nothing here is
// compiled and there is no body to write.
// Callers: CCompressorZlib::DecompressStream 0x00A3A840 (Z_SYNC_FLUSH), CCompressorZlib::InternalDecompress 0x00A3AB00
// (Z_FINISH); both declare extern "C" int __cdecl z_inflate(z_stream_s*, int).
// Proof: python3 tools/decomp_pipeline/libsig/lib_verify.py 00c02470 -> MATCH (vendor zlib-1.2.1(inflate.c), 79/79
// relocations proven); source tree: tools/decomp_pipeline/libsig/fetch_vendor_libs.py zlib-1.2.1;
// oracle row in rebuild/oracles/vendor_signatures.tsv; docs/pipeline/LIBRARY_SIGNATURES.md "Vendor libraries".
