# SDK / library needs of the byte-exact Fable.exe rebuild (audit 2026-11-08, w22/sdkaudit)

Scope: the owner's question "check if we need any windows sdk or direct x libraries, or any other libraries".
"Need" is split three ways: **(a)** foreign code/data the linker put into retail, **(b)** headers whose inline
code or layout lands in retail bytes, **(c)** import / link libraries a full link needs. Everything below was
measured in this session on the Linux lane (cl 13.10.3077 RTM under Wine, `FABLE_EXE` = retail). Scripts:
`$S/w22/sdkaudit/scripts/`; downloads `$S/sdk/dl`, unpacked trees `$S/sdk/x`, `$S/sdk/src`; nothing was written
to the repo.

## 0. Headline findings

1. **The DirectShow side is the DirectX 9.0c SDK Summer 2004, not Feb 2005 Extras / Server 2003 SP1.**
   * `strmiids.lib` of DX SDK Summer 2004 (one object, `@comp.id` **Utc13_C build 9178**) is linked whole: its 848 GUID
     sections sit contiguously at **0x012A83A4..0x012AB8A4**, 848/848, in object order. That object is the Rich header's
     lone `0x1C/9178` entry. Dec 2004 / Feb 2005 Extras / DDK 3790.1830 `strmiids.lib` (925 GUIDs, 4035) do not fit.
   * BaseClasses: retail `CRendererInputPin::QueryId` 0x00CA51C0 is `CoTaskMemAlloc(8)` + `call [KERNEL32!lstrcpyW]`
     (IAT 0x0143FEDC) = Summer 2004 `renbase.cpp`; Feb 2005 Extras replaced it with `StringCchCopyW` (inline, no import).
   * **Probe: Summer 2004 `Samples\C++\DirectShow\BaseClasses` amfilter/combase/ctlutil/mtype/renbase/wxlist/wxutil
     built with cl 3077 `/O2 /W3 /MD /GS /EHsc /Gz /GF /Gy /DNDEBUG /D_MBCS /D_LIB /D_WIN32_DCOM /DWINVER=0x400 /DWIN32
     /DUNICODE` reproduce every code section of those 7 TUs** (objscan: 0 NO MATCH). Through a scratch copy of
     `libsig_scan.py --vendor` + `lib_verify.py`: **588 oracle rows, 588/588 MATCH with relocations resolved**
     (348 landed rows, 7 unlanded rows, 233 sections not in the manifest). `/Gz` (+8 sections) and `/GS` (+4) are
     required; `/Ob1` (the SDK .vcproj setting) loses 55; `WINVER=0x400` is required for `CompatibleTimeSetEvent`.
2. **D3D9 / D3DX / DInput import libs = DirectX SDK April 2005** (`d3d9.lib`, `d3dx9.lib` -> `d3dx9_25.dll`,
   `dinput8.lib`, all Implib710 **4035**): import hints 1/1, 11/11, 1/1; the three DLLs are exactly the Rich header's
   `0x5D/4035 x6`. `dinput8.lib`'s four data-format objects (`c_dfDIMouse` 0x012ADA08, `c_dfDIMouse2` 0x012ADC10,
   `c_dfDIKeyboard` 0x012ADE18, `c_dfDIJoystick` 0x012AE020; C 4035) are the Rich `0x5F/4035 x4`, and `dxguid.lib`'s
   `dxguid.obj` (550 GUIDs, C++ 4035) is the `0x60/4035 x1`. **So the five "QFE-4035 compiled objects" are DirectX SDK
   library data, not code** (the docs' "may be WinDDK GUID objects" guess is half right: GUID/data objects, but from
   the DX SDK).
3. **System import libraries are VS .NET 2003 / Feb-2003-PSDK era (2179), not the WinDDK 3790.1830 ones the lane uses.**
   Hints: KERNEL32 120/120 only with VS 2003 `Vc7/lib/kernel32.lib` (DDK 0/120); ADVAPI32 9/9, OLE32 7/7 only with the
   Feb 2003 PSDK (DDK 0/9, 2/7). USER32 / IMM32 / SHELL32 match neither (52-54/75, 13/17, 1/2): two of them are the
   Rich header's `Implib710/2067` and `Implib700/9210` libraries, still unidentified. `.text` thunk bytes do not depend
   on this; the `.idata` hint/name table does.
4. **The lane's C/C++ runtime headers are the WinDDK's, not VS 2003's.** `$FABLE_VC71/include` -> WinDDK
   `inc/crt`: an old Dinkumware (no `_CPPLIB_VER`; VS 2003 has 313) - `<vector>`/`<xstring>`/`<xtree>` differ by
   1.7-3.9k lines, all 116 common headers differ. Retail imports MSVCP71 `basic_string` members, which only the VS 2003
   `<xstring>` (`_CRTIMP2` extern templates) produces. **Probe: 50 random landed rows (30 game, 20 libpk) re-graded
   with VS 2003 `Vc7/include` instead: 50/50 identical grades (14 MATCH, 36 RELOCATION_MATCH, all tests PASS).**
5. **DXUT ImeUi.cpp is not in any public DirectX SDK of the period.** Checked (listing every file): Summer 2004,
   Dec 2004, Feb 2005 Extras, Apr 2005, Jun 2005, Aug 2005, Oct 2005, Dec 2005, Apr 2006: no `ImeUi.cpp`
   (only `DXUTgui.cpp`'s `CDXUTIMEEditBox`, a sibling of the same IME code). The earliest public copy is later
   (DXUT `Optional\ImeUi.cpp`, e.g. DXSDK_Jun10; a 2008 gamedev thread places it in the Nov 2007 SDK) and has lost
   `WA_GetCandidateList`. Retail's revision predates every public release: no exact source is obtainable.
6. **CImaAdpcmCodec (libpk 0x00C5F000..0x00C5FA50) is Xbox XDK sample code (`imaadpcm.cpp`)**, not in any DX SDK
   above; the XDK is NDA-only. Stays reconstruction (12 rows landed; 0x00C5F6A0 `Audio_ExpandADPCM_Mono` unlanded,
   probably `CImaAdpcmCodec::DecodeM16`).
7. Swapping real SDK headers into landed rows is byte-neutral where tested: `d3dx9math.h` (Apr 2005) in
   0x00A565C0 GFDXGetHermiteSplinePosition (RELOCATION_MATCH) and 0x00988990 SetD3DMatrix (MATCH), `dinput.h`
   (`DIRECTINPUT_VERSION 0x0800`) in 0x00A60050 CInputManagerDX ctor (RELOCATION_MATCH).

## 1. SDK / library table

"Lane now": what `$S` has today (before this audit); "scratch" = fetched in this session under `$S/sdk`.
Licence column: none of these may be committed; fetch into scratch / `FABLE_*` dirs at build time.

| # | Name, exact version | Need | Retail evidence | Official source (verified) | Size | Lane now | Licence / redistribution |
|---|---|---|---|---|---|---|---|
| 1 | **VS .NET 2003 RTM `Vc7/lib`** (cl/link 13.10.3077): msvcrt.lib, msvcprt.lib, kernel32.lib (2179) | (a) CRT objects, (c) MSVCR71/MSVCP71/KERNEL32 import libs | 28 CRT sections + 440 toolchain_library rows; hints MSVCR71 123/123, MSVCP71 3/3, KERNEL32 120/120; Rich 0x5D/3077 x5, 0x0F/3077 x12, 0x5F/3077 CRT members | archive.org `visual-studio-2003-unpacked` (already used for cl.exe; `fetch_vs2003_libs.sh`) | 15 MB (21 files) | **yes** `$FABLE_VS2003_LIB` | Microsoft product files; never committed |
| 2 | **VS .NET 2003 RTM `Vc7/include`** (CRT + Dinkumware `_CPPLIB_VER 313`, ivec/fvec/dvec, comdef/comutil) | (b) | MSVCP71 `basic_string` imports (Dinkumware extern templates); libpk Dinkumware instantiations; libpk SIMD rows use Intel `ivec.h`/`dvec.h` class names; 50/50 neutral re-grade | same archive.org item, `.../Vc7/include/` (155 files, 153 fetched in one pass + 2 retried, all 200) | 3.7 MB | **no** (lane uses WinDDK `inc/crt`, older Dinkumware, no ivec/fvec/dvec/comdef) | as #1 |
| 3 | **DirectX 9.0c SDK Summer 2004** (`dxsdk_sum2004.exe`, Aug 2004): `Samples\C++\DirectShow\BaseClasses`, `Include` (strmif.h, control.h, uuids.h, Amvideo.h, DShow.h, evcode.h, ks*.h ...), `Lib\strmiids.lib` | (a) strmbase code + strmiids GUID data, (b) DirectShow headers, (c) strmiids.lib | strmbase block 0x00CA4700..0x00CB2100: 588/588 vendor rows MATCH; strmiids 848/848 GUIDs at 0x012A83A4 (Rich 0x1C/9178 x1); QueryId 0x00CA51C0 lstrcpyW form | archive.org `dxsdk_sum2004` (Microsoft's installer; metadata sha1 `73d875b97591f48707c38ec0dbc63982ff45c661`, verified on download) | 239,008,008 B; Include 9.2 MB, Lib 24 MB, BaseClasses 67 files | **no** (scratch: `$S/sdk/x/sum2004`, `$S/sdk/src/bc_sum2004`) | DirectX SDK EULA: sample code may be compiled into products; headers/libs not redistributable -> fetch at build time |
| 4 | **DirectX SDK April 2005** (`dxsdk_apr2005.exe`): `Include` (d3d9*.h, d3dx9*.h/.inl, dinput.h, dsound.h, dxerr9.h), `Lib\x86` d3d9.lib, d3dx9.lib, dinput8.lib, dxguid.lib | (a) dxguid.obj + 4 dinput8 data objects, (b) D3D9/D3DX/DInput/DSound headers, (c) d3d9/d3dx9_25/dinput8 import libs | hints 1/1, 11/11, 1/1; Rich 0x5D/4035 x6, 0x5F/4035 x4, 0x60/4035 x1; dxguid 550 GUIDs ~0x012AB8A4..0x012ADAD4 (duplicates of strmiids folded); c_dfDI* at 0x012ADA08..0x012AE020; 3 row swaps byte-neutral | archive.org `dxsdk_apr2005` (sha1 `ec4dd48826a399c6ce041745f14b90356d8c5cd1`, verified) | 159,694,584 B; Include 2.9 MB, Lib\x86 5.5 MB | **no** (lane has WinDDK `inc/wnet` d3d9.h/dinput.h/dsound.h: 1-9 line diffs from Apr 2005; no d3dx9*.h; no d3dx9.lib) | DirectX SDK EULA; fetch at build time |
| 5 | **Platform SDK Feb 2003** (Windows Server 2003 RTM PSDK, 5.2.3790.0; import libs Implib710 **2179**) | (c) ADVAPI32, OLE32, GDI32, OLEAUT32, VERSION, WINMM, WSOCK32 import libs; (b) imm.h, msctf.h, ctfutb.h (ImeUi), period windows.h | hints ADVAPI32 9/9, OLE32 7/7, GDI32 5/5, OLEAUT32 2/2, VERSION 3/3, WINMM 22/22, WSOCK32 ordinals equal; Rich 0x5D/2179 x18 = 9 DLLs | archive.org `psdk-full.-1` "Platform SDK Feb 2003" (Microsoft's web-install cabs PSDK-FULL.1-13.cab; all 13 sha1 match the item metadata) | 13 cabs, 324 MB | **no** (scratch: `$S/sdk/x/psdk2003/l20`, `l21`) | PSDK EULA; fetch at build time |
| 6 | **VS .NET 2003 bundled `Vc7/PlatformSDK`** (2179 build snapshot) | (c) probably USER32/IMM32/SHELL32 import libs and the 5 `uuid.lib` objects (Rich `0x5F/2179 x5`) | USER32 52/75, IMM32 13/17, SHELL32 1/2 match neither the Feb 2003 PSDK nor the DDK; Feb 2003 `uuid.lib` is build **2190**, retail's uuid objects are 2179 | **not found**: the archive.org VS 2003 items checked (`visual-studio-2003-unpacked`, Professional Disc 1 and 2 ISOs, `vs_setup.msi`) have no `Vc7/PlatformSDK` | ? | no | Microsoft product files |
| 7 | **Unidentified import libraries**: one `Implib710/2067` (VS 2003 pre-RTM linker) and one `Implib700/9210` (VC 7.0 pre-RTM linker) | (c) two of USER32/IMM32/SHELL32 | Rich `0x5D/2067 x2`, `0x19/9210 x2` | not identified | - | no | - |
| 8 | **Windows Server 2003 SP1 Platform SDK** (3790.1830) / **WinDDK 3790.1830** | (b) fallback headers (windows.h, imm.h, ks.h, ksmedia.h, mmreg.h, uuids.h, d3d9.h) | the strmbase probe (588/588) and both D3DX swaps used DDK `inc/wnet` for windows.h and were byte-exact; DDK import libs do NOT match retail hints (#3 finding) | Microsoft Download Center id=15656 `5.2.3790.1830.15.PlatformSDK_Svr2003SP1_rtm.img` (page 200; 414,361,600 B); DDK via the existing lane mirror | 414 MB / 708 MB | **yes** (DDK `inc/wnet`, `lib/wnet/i386`) | PSDK EULA |
| 9 | **DXUT ImeUi.cpp**, 2004/2005 revision with `WA_GetCandidateList` | (a) 0x00CB2100..0x00CB5000 (29 rows, 28 landed) | PDB `ImeUi_*` names; IME strings 0x012BFA88..0x012BFBC8 | **none public** (absent from 9 DX SDKs 2004-2006 checked); nearest: DXSDK_Jun10 (Microsoft Download id=6812, 200) or GitHub microsoft/DXUT (MIT), both later revisions | - | no | stays reconstruction |
| 10 | **Xbox XDK `imaadpcm.cpp` (CImaAdpcmCodec)** | (a) 0x00C5F000..0x00C5FB?? inside libpk | class name + FINDINGS_LOG chunk 677; absent from every DX SDK listed above | none (XDK under NDA) | - | no | stays reconstruction |
| 11 | **Boost 1.30.0** (scoped_ptr, checked_delete, shared_ptr `sp_counted_base`) | (b) header code in game TUs | `rebuild/include/boost/scoped_ptr.h` already models Boost 1.30; PDB `boost::detail::sp_counted_base`, `checked_delete` (`GOTCHAS` 1038/1194) | boost.org history page (200), SourceForge `boost/files/boost/1.30.0` (200) | ~10 MB | no (hand model) | Boost licence (permissive); could even be vendored as headers, but keep it fetched |
| 12 | **STLport 4.6** | (b) STL instantiations in game TUs | LIBRARY_MAP "spread" row; GOTCHAS 2026-10-26 | SourceForge `stlport/files/STLport/` (200) | ~2 MB | no (hand models in `rebuild/include/stlport`) | STLport licence (permissive) |
| 13 | **OpenAL 1.1 / EAX 2-4 headers** | (b) typedefs, function-pointer tables, EAX property GUIDs/structs only | no OpenAL/EAX import; `/OpenAL32.dll`, `/wrap_oal.dll` loaded at run time (LIBRARY_LINKING section 7); 51 libpk rows redeclare EAX structs | openal.org downloads (OpenAL11CoreSDK.zip, page 200 per LIBRARY_LINKING); EAX SDK no longer hosted | small | no | GUID values are public constants: a repo header is fine |
| 14 | zlib 1.2.1, LZO 1.08, libogg 1.1.1, libvorbis 1.0.1 | (a) | done (vendor_libs.tsv) | as recorded | - | yes `$FABLE_THIRDPARTY` | as recorded |
| 15 | `comsupp.lib`, `dxerr9.lib`, `d3dx9` statics, `strmbase.lib` (prebuilt), `ksguid.lib`, `ksuser.lib`, `dmoguids.lib` | **not needed** | `d3dx9.lib` (Apr 2005) has no static code (323 import records + 3 descriptors); DxErr9 strings absent; the DDK's prebuilt `strmbase.lib` (4035) matches only 130 trivial sections and its objects would be Rich `0x60/4035`, which retail has only once (dxguid); ksguid/ksuser hits are KS GUIDs that strmiids/dxguid already carry; the comsupp `.xdata$x` hit is a generic EH table shape | - | - | - | - |

### How the Rich header entries resolve

| Rich entry | Count | Resolved to | Contributes |
|---|---:|---|---|
| 0x60 Utc1310_CPP 3077 | 1369 | Fable + libpk + LHNetworkLib + strmbase (7 TUs, Summer 2004 source) + ImeUi + 4 CRT members | .text code |
| 0x5F Utc1310_C 3077 | 51 | zlib 9 + LZO 5 + ogg 2 + vorbis 21 + CRT 5 + libpk C TUs | .text code |
| 0x0F Masm710 3077 | 12 | 11 msvcrt.lib asm members + LZO lzo1x_f1.asm | .text code |
| 0x5D Implib710 3077 | 5 | MSVCR71 + MSVCP71 descriptors (+ `__NULL_IMPORT_DESCRIPTOR`) from VS 2003 msvcrt/msvcprt.lib | .idata only |
| 0x5D Implib710 2179 | 18 | 9 DLLs: KERNEL32 (VS 2003 Vc7/lib), ADVAPI32, OLE32, GDI32, OLEAUT32, VERSION, WINMM, WSOCK32 (Feb 2003 PSDK hints) + one of USER32/IMM32/SHELL32 | .idata only |
| 0x5D Implib710 4035 | 6 | d3d9, d3dx9_25, DINPUT8 (DirectX SDK April 2005) | .idata only |
| 0x5D Implib710 2067 / 0x19 Implib700 9210 | 2 / 2 | the other two of USER32/IMM32/SHELL32 (libraries unidentified) | .idata only |
| 0x5F Utc1310_C 4035 | 4 | dinput8.lib `dilib1/2/3/5.obj` = c_dfDIMouse/Keyboard/Joystick/Mouse2 (Apr 2005) | .rdata data |
| 0x60 Utc1310_CPP 4035 | 1 | dxguid.lib `dxguid.obj` (Apr 2005) | .rdata GUIDs |
| 0x1C Utc13_C 9178 | 1 | strmiids.lib `strmiids.obj` (DX SDK Summer 2004) | .rdata GUIDs |
| 0x5F Utc1310_C 2179 | 5 | uuid.lib members (cguid_i, unknwn_i, objidl_i, TSF msctf GUID object(s), ctfutb) of a 2179 uuid.lib = presumably VS 2003's Vc7/PlatformSDK (Feb 2003 PSDK's is 2190) | .rdata GUIDs at 0x012BFBD4..0x012C1100 |
| 0x69 (2067) | 8 | msvcrt.lib `sdknames\*.obj` alias objects (only a `.drectve`, no bytes) | nothing |
| 0x01 Import0 | 422 | short import records | thunks + IAT |
| 0x5A / 0x5E | 1 / 1 | link 7.10.3077, cvtres 7.10.3052 | - |

So **no foreign compiled code** comes from a prebuilt library apart from the VS 2003 CRT members; every other
non-Lionhead code byte was compiled from source by cl 3077 (strmbase, ImeUi, zlib, LZO, Xiph). The 4035/9178/2179
compiled objects are GUID / data-format data only. Retail links with /OPT:NOREF semantics (whole GUID objects kept,
149 identical type_info copies), so a full relink must pass the same libraries in the same order to reproduce
`.rdata`.

## 2. Library code not (correctly) in LIBRARY_MAP.tsv

| Range | What | Evidence | Rows |
|---|---|---|---|
| **0x00CA4700..0x00CA4780** | strmbase starts 0x80 earlier: `TimeDiff` (renbase.cpp static, 51 bytes, **not in the manifest**) right after libvorbis `vorbis_lsp_to_curve` 0x00CA4570 (398 B) | vendor lib_verify MATCH 0x00CA4700 | 0 manifest rows; 1 boundary row to add; map start should be 0x00CA4700 |
| 0x00CA4700..0x00CB2100 | DirectShow BaseClasses, **pin: DX9.0c SDK Summer 2004**, 7 TUs, flags above | 588/588 lib_verify MATCH (`strmbase_vendor_signatures.tsv`, `strmbase_lib_verify.log`) | manifest 410 in range: 348 landed + 7 unlanded proven; 55 not assigned (ret stubs, ??_G, adjustor thunks with several identical copies; 9 of them are the unlanded 0x00CA53C0, 0x00CA6170-90, 0x00CAA470, 0x00CAA720-40, 0x00CAF430, all reproduced by the build but ambiguous); 201 in-block sections are not manifest rows |
| 0x00CB0C10 | **landed as `WINMM.DLL::timeSetEvent` (thunk) but it is `CompatibleTimeSetEvent` (wxutil.cpp)**: under `/Gz` + `WINVER=0x400` the wrapper compiles to exactly `FF 25 [__imp__timeSetEvent@20]` (6 bytes); retail's real winmm thunk run is at 0x00F3FE0A | wxutil.obj section `?CompatibleTimeSetEvent@@YGIIIP6GXIIKKK@ZKI@Z` = `jmp dword ptr [__imp__timeSetEvent@20]`; neighbour `TimeKillSynchronousFlagAvailable` 0x00CB0C20 MATCH | 1 landed row to re-grade/rename (NAME_FIXES) |
| 0x00A3BD60..0x00A3BDB0 | six `[thunk]:CBaseVideoRenderer::QueryInterface/AddRef/Release adjustor{N}` copies near the game's CTextureRenderer | vendor scan (medium/low: 3 identical copies, assigned by order) | outside manifest; header-generated in the game TU, not library |
| 0x01216990.. (.text end) | 26 `__ehhandler$` stubs of strmbase functions | vendor scan high | outside manifest |
| 0x00A3AEF0 / 0x00A3AF10 / 0x00A3AF30 | out-of-line `guiddef.h` `IsEqualGUID` / `operator==` / `operator!=` (header code in a /Gr game TU) | landed rows say so | 3 landed (hand-written header bodies) |
| 0x00C5F000..0x00C5FA50+ | Xbox XDK `CImaAdpcmCodec` inside libpk | class name, FINDINGS_LOG chunk 677 | 12 landed, 0x00C5F6A0 unlanded |
| 0x00C0B000..0x00C0E2xx | MFC-6-style CString copy inside libpk | LIBRARY_LINKING section 12 | ~49 rows (already described there) |
| .rdata 0x012A83A4..0x012AB8A4 | strmiids.lib (Summer 2004) GUIDs | 848/848 contiguous | data |
| .rdata ~0x012AB8A4..0x012ADAD4 | dxguid.lib dxguid.obj (Apr 2005) | 550 GUIDs present, layout minus folded duplicates | data |
| .rdata 0x012ADA08..0x012AE020+ | dinput8.lib c_dfDIMouse / c_dfDIMouse2 / c_dfDIKeyboard / c_dfDIJoystick | relocation-free 516-byte object-format arrays found | data |
| .rdata 0x012BFBD4..~0x012C1104 | uuid.lib cguid_i, unknwn_i, objidl_i (89/89), msctf/ctfutb GUIDs | guidmap | data |

## 3. Landed rows that redeclare SDK types / inlines locally

No landed row includes a single SDK header (`windows.h`, `d3d9.h`, `dinput.h`, `strmif.h` ... zero hits); every
SDK type is a local redeclaration. `scripts/sdkredecl.py` -> `sdk_redeclarations.tsv`: **1,320 source files** with
at least one SDK redeclaration (of 54,818 .cpp under `rebuild/src/compiled`), by area:

| Area | Files | Where |
|---|---:|---|
| windef/winnt typedefs, `__declspec(dllimport)` API decls, RECT/POINT/CRITICAL_SECTION | 909 | game 227, libpk 224, strmbase 291, xiph 143, ImeUi 22 |
| COM GUID / IUnknown / IsEqualGUID / IID_ consts | 331 | strmbase 242, libpk 60, game 28 |
| d3d9 / d3d9types (interfaces, D3DFORMAT, D3DCAPS9, ...) | 301 | game |
| DirectShow (AM_MEDIA_TYPE, IPin, IBaseFilter, CBase*, CMediaType) | 248 | strmbase 241, game 7 |
| OpenAL / EAX | 51 | libpk |
| dsound / mmreg / ks (WAVEFORMATEX, IDirectSound*, KSDATAFORMAT) | 20 | libpk |
| winsock | 20 | game (LHNetworkLib) |
| dinput | 19 | game |
| imm / msctf | 16 | ImeUi 14, game 2 |
| d3dx9 (D3DXVECTOR3, D3DXMATRIX, D3DXIMAGE_INFO) | 4 | game |

Byte-neutrality of the real header, tested with `FABLE_HDR_OVERLAY=$S/sdk/x/apr2005/Include:<DDK inc/wnet>`:

| Row | Swap | Result |
|---|---|---|
| 0x00A565C0 GFDXGetHermiteSplinePosition | local `D3DXVECTOR3` + `D3DXVec3Hermite` decl -> `<windows.h>` + `<d3dx9math.h>` | RELOCATION_MATCH (unchanged) |
| 0x00988990 CShaderRenderManager::SetD3DMatrix | local `D3DXMATRIX` 16-float ctor -> `<d3dx9math.h>` (.inl ctor, `D3DMATRIX` base with union) | MATCH (unchanged) |
| 0x00A60050 CInputManagerDX ctor | local `_GUID`, `IID_IDirectInput8W`, `DirectInput8Create` -> `DIRECTINPUT_VERSION 0x0800` + `<dinput.h>` | RELOCATION_MATCH (unchanged) |
| 7 strmbase TUs | whole real SDK source + headers | 588/588 MATCH (section 0) |
| 50 random landed rows | `$FABLE_VC71/include` = VS 2003 Vc7/include instead of DDK inc/crt | 50/50 identical grades + PASS |

Conclusion: swapping in the period SDK headers is byte-neutral where tested; the rows only need the headers to be
present in the lane. Swaps are worth doing for the 241 strmbase rows (they become `Vendor-proof` / vendor stubs
outright) and for the D3D9/DInput/D3DX rows (shared headers instead of 300+ local interface redeclarations, where a
wrong vtable slot order is a silent bug). The `__thiscall` -> `__fastcall` rewrite in `verify_and_land.vc71()` only
touches row text, not headers, so real SDK headers are unaffected by it.

## 4. Recommended setup

### Directory layout (scratch on Linux, `D:\Tools\sdk` on Windows; never in the repo)

```
$FABLE_SDK/                                   # new root, e.g. $S/sdk
  vs2003/include/          FABLE_VS2003_INCLUDE   # Vc7/include, 155 files (archive.org visual-studio-2003-unpacked)
  vs2003/lib/              FABLE_VS2003_LIB       # existing (fetch_vs2003_libs.sh)
  dxsdk_sum2004/Include    FABLE_DXSDK_DSHOW      # DShow headers + Lib/strmiids.lib + Samples/C++/DirectShow/BaseClasses
  dxsdk_apr2005/Include    FABLE_DXSDK            # D3D9/D3DX/DInput/DSound headers + Lib/x86 (d3d9, d3dx9, dinput8, dxguid)
  psdk_feb2003/{include,lib}  FABLE_PSDK          # Feb 2003 PSDK x86 import libs + imm.h/msctf.h/ctfutb.h (+ windows.h)
```

Unpack recipe (all `7z`; every DX SDK is WEXTRACT exe -> CAB -> `dxsdk.exe`, which 7z opens as a CAB):
```
curl -LO https://archive.org/download/dxsdk_sum2004/dxsdk_sum2004.exe   # sha1 73d875b97591f48707c38ec0dbc63982ff45c661
7z x dxsdk_sum2004.exe && 7z x dxsdk.exe 'Include/*' 'Lib/*' 'Samples/C++/DirectShow/BaseClasses/*' -x'!Lib/x64*'
curl -LO https://archive.org/download/dxsdk_apr2005/dxsdk_apr2005.exe   # sha1 ec4dd48826a399c6ce041745f14b90356d8c5cd1
7z x dxsdk_apr2005.exe && 7z x dxsdk.exe 'Include/*' 'Lib/x86/*'
for i in $(seq 1 13); do curl -LO "https://archive.org/download/psdk-full.-1/Feb%202003/PSDK-FULL.$i.cab"; done   # sha1 per item metadata
7z x PSDK-FULL.1.cab            # opens the 13-volume set -> setup/*.cab, setup/*.msi
7z x setup/CoreSDK-common.2.0.cab '*_Lib.*'; 7z x setup/CoreSDK-common.2.1.cab '*_Lib.*' '*_h.*'   # x86 libs/headers, file keys -> rename by the msi File table (msiextract) or by prefix
```
(Files inside the PSDK cabs carry MSI file keys like `User32_Lib.95CE7DF8_...`; `msiextract CoreSDK-x86.msi` with the
cabs beside it, or a small rename script, restores real names.)

### env.sh additions (Linux lane)
```
export FABLE_SDK=$S/sdk
export FABLE_VS2003_INCLUDE=$FABLE_SDK/vs2003/include
export FABLE_DXSDK=$FABLE_SDK/dxsdk_apr2005            # Include, Lib/x86
export FABLE_DXSDK_DSHOW=$FABLE_SDK/dxsdk_sum2004      # Include, Lib, Samples/C++/DirectShow/BaseClasses
export FABLE_PSDK=$FABLE_SDK/psdk_feb2003
export FABLE_THIRDPARTY=$S/thirdparty:$FABLE_SDK/thirdparty   # second entry holds the strmbase vendor tree
# header overlay for SDK-using rows (after VC include, before rebuild/include):
export FABLE_HDR_OVERLAY=$FABLE_DXSDK/Include:$FABLE_DXSDK_DSHOW/Include:$S/wine/winddk/3790.1830/inc/wnet
```
and, after a full landed-catalog re-grade confirms the 50-row sample, point `$S/wine/vc71rtm/include` at
`$FABLE_VS2003_INCLUDE` instead of the DDK `inc/crt` symlink (keep the std shims only if still needed: VS 2003's
headers already put the C library into `namespace std`). On Windows, `D:\Tools\vc71\include` already is that set;
`rebuild/vc71_sdk_env.ps1` and `build_bootstrap.ps1` currently use **Windows Kits 10 (10.0.22621)** headers and
`um\x86` import libraries: wrong era for inline code and for `.idata` hints - replace with the layout above.

### vendor_libs.tsv: add the strmbase row (proven in this audit)
```
tag      strmbase-dxsdk-sum2004
version  DirectX 9.0c SDK Summer 2004 (Samples\C++\DirectShow\BaseClasses)
tree     dxsdk-sum2004            # BaseClasses/ + Include/ (DX Summer 2004) + wnet/ (windows.h set)
url      https://archive.org/download/dxsdk_sum2004/dxsdk_sum2004.exe   (sha1 73d875b9...; sha256 to be recorded at first fetch)
flags    /O2 /W3 /MD /GS /EHsc /Gz /GF /Gy /DNDEBUG /D_MBCS /D_LIB /D_WIN32_DCOM /DWINVER=0x400 /DWIN32 /DUNICODE
includes BaseClasses Include wnet
sources  BaseClasses/amfilter.cpp combase.cpp ctlutil.cpp mtype.cpp renbase.cpp wxlist.cpp wxutil.cpp
licence  DirectX SDK EULA (sample code)
tree_sha256 (BaseClasses dir) 145d177cec5fb6adb6270f749804fa5b2fca891d66cf4c1e1b1b16568ad5a27c
```
`fetch_vendor_libs.py` needs an "SDK exe" fetch kind (7z twice + member filter) and `include_dirs` needs an absolute /
env-var include form (the probe copied DX `Include` and DDK `inc/wnet` into the tree). The probe's scratch copy of
the tools: `$S/sdk/libsig` (vendor_libs.tsv row appended), tree `$S/sdk/thirdparty/dxsdk-sum2004`, objects
`$S/sdk/vobj`.

Do NOT add rows for ImeUi (no period source), CImaAdpcmCodec (XDK), or the DDK's prebuilt strmbase.lib.

## 5. Next steps

1. Land the strmbase vendor row: 7 unlanded rows close as `// Vendor-library:` stubs (0x00CA61A0, 0x00CA61B0, 0x00CA61C0,
   0x00CA7D60, 0x00CAA750, 0x00CAA760, 0x00CAFD60); 348 landed rows get `// Vendor-proof:`; extend the scan's
   neighbourhood rule to assign the 9 ambiguous identical-shape unlanded rows; add 0x00CA4700 `TimeDiff` and the other
   in-block sections as boundary rows; re-grade 0x00CB0C10 as `CompatibleTimeSetEvent` (NAME_FIXES), not a WINMM thunk.
2. Fix LIBRARY_MAP.tsv: strmbase start 0x00CA4700, version "DX9.0c SDK Summer 2004", flags; add the .rdata data rows.
3. Run the full landed-catalog `--recheck-landed` with VS 2003 `Vc7/include` (sample was 50/50), then switch the lane.
4. Add `fetch_sdks.sh` (DX Summer 2004, DX Apr 2005, PSDK Feb 2003, VS 2003 include) with SHA-1 checks.
5. For a full byte-exact `.idata`: find the USER32/IMM32/SHELL32 import libraries (VS 2003's own `Vc7/PlatformSDK`
   is the lead: locate an archive of a VS .NET 2003 disc that has it; then the 2067 and 9210 libs).
6. Optional: move the D3D9/DInput/DShow rows to the real headers through shared `rebuild/include` wrappers
   (byte-neutral per the three tests), which removes 300+ local interface redeclarations.

## Files in this directory

* `strmbase_vendor_signatures.tsv` - 588-row vendor oracle for the Summer 2004 BaseClasses (scratch libsig copy).
* `strmbase_lib_verify.log` - 588/588 MATCH.
* `sdk_redeclarations.tsv` - 1,320 files x SDK areas.
* `crt_sample_addrs.txt` + `$S/sdk/crtprobe/{A,B}.log` - the 50-row header swap probe.
* `swap/*.cpp` - the three header-swap sources.
* `datafind_ddk.txt`, `guidrun_ddk.txt` - GUID/data object scans.
* `scripts/` - compid.py (Rich/comp.id per lib), hints.py (import hint comparison), guidlayout.py / guidsegs.py /
  guidmap.py (GUID object placement), sdkredecl.py, bcbuild.sh, scansum.py, crtprobe.sh, libdump.py.
