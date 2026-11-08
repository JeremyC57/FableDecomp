// CBaseInputPin::Release`adjustor{140}': DirectShow base classes (strmbase) vendor library code, not Fable source (grade toolchain_library).
// Vendor-library: strmbase-dxsdk-sum2004(BaseClasses/renbase.cpp) ?Release@CBaseInputPin@@WIM@AGKXZ -- 13 bytes verified against Fable.exe 0x00CA61C0
//
// Retail 0x00CA61C0 (13 bytes) is the .text section (COMDAT, /Gy) defining ?Release@CBaseInputPin@@WIM@AGKXZ in BaseClasses/renbase.cpp of the DirectX 9.0c SDK
// Summer 2004 (dxsdk_sum2004.exe, Samples\C++\DirectShow\BaseClasses, unmodified), compiled by cl 13.10.3077 with the
// recorded flags /O2 /W3 /MD /GS /EHsc /Gz /GF /Gy /DNDEBUG /D_MBCS /D_LIB /D_WIN32_DCOM /DWINVER=0x400 /DWIN32 /DUNICODE
// (tools/decomp_pipeline/libsig/vendor_libs.tsv). What it is: the `this -= 140` adjustor thunk for IMemInputPin::Release (renbase.cpp TU); assigned like 0x00CA61A0.
// Owner direction (2026-11-08): third-party code is isolated, identified and linked from its unmodified source, not
// rewritten by hand. The build compiles the vendor TU; nothing here is compiled and there is no body to write.
// Proof: python3 tools/decomp_pipeline/libsig/lib_verify.py 00ca61c0 -> MATCH; source tree:
// tools/decomp_pipeline/libsig/fetch_vendor_libs.py strmbase-dxsdk-sum2004; oracle row in
// rebuild/oracles/vendor_signatures.tsv; docs/pipeline/LIBRARY_SIGNATURES.md "Vendor libraries".
