# CSalPlatform::CreateSwVoices and its generated cleanup states

The reserved rows01215240 (11bytes) and0121524b (21bytes) belong to parent00c3f800/356. The second span contains11-byte cleanup plus10-byte compiler handler01215256. Native parent prologue references that handler; FuncInfo0136f470 references two-state unwind map0136f460 with cleanup pointers01215240 and0121524b. The old InitializeActorArrays/CTCHeroStats label was false. Neither cleanup was written by hand.

## Identity, naming and version differences

Original CODAApi.lib and CODAApiDbg.lib SalPlatform objects both name private CSalPlatform::CreateSwVoices. SDK types name the receiver's m_Sw2DVoices/m_Sw3DVoices and m_Sw2DVoiceMem/m_Sw3DVoiceMem; release body allocates the corresponding actual CSalSw2DVoice/CSalSw3DVoice arrays, then calls CCodaObjectList::CreateStatic. Native receiver offsets2c/40/174/178 and call/constructor/destructor graph agree. Original caller00c3fca4 tests AL, and native parent produces AL=0/1. SDK2.10 declares int and returns an error code; that later-version signature is NOT blindly copied. The retail leaf is ?CreateSwVoices@CSalPlatform@@AAE_NXZ, confirmed in the new COFF symbol table. The private method's name/access/owner are original; bool is independently native-proven.

All external game/library names emitted by the selected parent occur in original named SDK object tables. The existing canonical reloc binding agrees for g_SalCaps, CreateStatic, CodaMalloc and CodaFree. Wrong earlier class/global aliases are listed in NAME_FIXES.tsv; they are not retained to satisfy a label checker. Each new object relocation is resolved to native target, including the nonzero g_SalCaps member addends. No external symbol is mapped to two native addresses.

## Real shared layouts; no local stand-ins

New opt-in FABLE_RETAIL_CSALPLATFORM_CREATE_SW_PASS23 selects the proven native layouts in four shared headers, retaining each default path. SDK types identify every named field, including m_asHistory, m_FrameBuffer, m_fGains, m_gainArray, m_gainArrayNew, m_iReverbLevel and both environment gains. SDK channel counts differ from retail; native accesses determine actual extents:

- CSalSwBaseVoice history begins124h. Native channel indexing at00c5b785 advances20h bytes; next fields at1e4/1e8/1ec imply six histories of16shorts, not two histories of48. Base size1f0h. The rejected2x48 interpretation was not authored or graded.
- CSalSw2DVoice framebuffer1f0/32; gains210/24; update flag228; gain arrays22a/12 and236/12; reverb244; environment flag248; environment gains24a/24c; native total250h. Native00c5c39a/3af/3b6 and00c5c803 onward independently access these fields.
- CSalSw3DVoice frame1f0; Azimuth210,Elevation214,MacroFXGain218; render220; filters224 and230; gains23c/23e; flag240; total248h under existing natural alignment. Constructor00c555e0 corroborates frame, render and both filters.
- CSalPlatform retains inherited list offsets2c/40 and allocation pointers174/178. The opt-in restores protected inherited list access for its genuine derived member. No new packing scope, invented helper class or placement-allocation trick was used.

Production asserts validate both native array strides592 and584. The test's existing host ABI opt-in accommodates the64-bit C++03 host; production remains native32-bit. Eight general default-token profiles plus two explicit CSal profiles with/without existing PASS22 opt-in were unchanged; old/new patch pairs reconstruct all99 merged headers.

## Compiler and relocation verification

VC7.1RTM13.10.3077 /O2 /Oy /W3 /EHsc emits356-byte parent exactly with relocation masks. Its entire32-byte generated cleanup/handler section,44-byte unwind map/FuncInfo and existing5-byte CCodaObjectBase::operator delete[] also match. All33 code/data bindings are audited individually. The constructor iterator at00f3fb96 is statically linked CRT, not an import thunk: original VC7.1 msvcrt.lib ehvecctr.obj supplies its exact mangled symbol and its98-byte code section, including adjacent cleanup, matches after relocation masking. Scalar operator-new00bfea1a and CxxFrameHandler00bfeb34 are independently decoded named PE import thunks. Existing helper bytes are not counted as newly matched scope.

## Behavior coverage

Each self-contained test pastes the actual parent source and uses include/ only. RealVC7.1 /Od /W3, without /EHsc, passes75 combinations:2D/3D counts-1/0/1/2/4 with no allocation failure, first allocation failure or second allocation failure. Assertions verify array construction counts, list receivers/first entries/strides/counts/order, retained pointers, unchanged hardware counts, and destructor/allocation balance. On first allocation failure both software voice counts are cleared; on second failure only3D counts are cleared and successful2D allocation/counts remain. That distinction fixed the first source attempt.

GCC64 C++03 with RTTI disabled passes the same75 plus six throwing-constructor cases (first three indices of each array), checking partial-array destruction and retained successful2D state. Native noEH tests do not claim exception-execution coverage: actual production compiler-generated EH code and metadata are independently byte-verified.
