// CRendererPosPassThru::`vector deleting destructor'`adjustor{8}': DirectShow base classes (strmbase) vendor library code, not Fable source (grade toolchain_library).
// Vendor-library: strmbase-dxsdk-sum2004(BaseClasses/ctlutil.cpp) ??_ECRendererPosPassThru@@W7AEPAXI@Z -- 8 bytes verified against Fable.exe 0x00CAFD60
//
// Retail 0x00CAFD60 (8 bytes) is the .text section (COMDAT, /Gy) defining ??_ECRendererPosPassThru@@W7AEPAXI@Z in BaseClasses/ctlutil.cpp of the DirectX 9.0c SDK
// Summer 2004 (dxsdk_sum2004.exe, Samples\C++\DirectShow\BaseClasses, unmodified), compiled by cl 13.10.3077 with the
// recorded flags /O2 /W3 /MD /GS /EHsc /Gz /GF /Gy /DNDEBUG /D_MBCS /D_LIB /D_WIN32_DCOM /DWINVER=0x400 /DWIN32 /DUNICODE
// (tools/decomp_pipeline/libsig/vendor_libs.tsv). What it is: ctlutil.cpp: the `this -= 8` adjustor thunk of CRendererPosPassThru's vector deleting destructor (`??_E...@W7AEPAXI@Z`; the manifest called it `scalar deleting destructor'). ctlutil.cpp has two sections with these bytes and retail two copies, assigned in object section order.
// Owner direction (2026-11-08): third-party code is isolated, identified and linked from its unmodified source, not
// rewritten by hand. The build compiles the vendor TU; nothing here is compiled and there is no body to write.
// Proof: python3 tools/decomp_pipeline/libsig/lib_verify.py 00cafd60 -> MATCH; source tree:
// tools/decomp_pipeline/libsig/fetch_vendor_libs.py strmbase-dxsdk-sum2004; oracle row in
// rebuild/oracles/vendor_signatures.tsv; docs/pipeline/LIBRARY_SIGNATURES.md "Vendor libraries".
