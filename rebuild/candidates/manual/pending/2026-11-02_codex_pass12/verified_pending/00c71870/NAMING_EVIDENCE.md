# Original vendor identity recovered

The GameCODA 2.10 Windows SDK CODAApi.lib member SalOpenALReverb.obj defines the exact original symbol `?Update@CSalOpenALReverb@@UAEXPBVCCodaEnvironment@@@Z`. Its section is 1115 bytes and has 120 relocations. An independent comparison with retail 00c71870 found zero differing bytes outside those relocation operands. This is a native retail comparison against an original vendor object, not a claim that game donor PDBs contain this vendor code.

The second identity source is original public CODA.h: `_CENVIRONMENT` has precisely the two vectors and 23 scalar values read by this function, in their exact original order and types. CODAApiDbg.lib's own COFF types independently prove CCodaEnvironment::GetParams, GetCommit, the CSalOpenALReverb hierarchy, member access and SDK spellings. Retail vtable 012bedf4 slot 5 points to the target; the constructor at 00c71710 and factory allocation 0x220 corroborate its complete retail receiver layout. The label suggesting an anonymous function/destructor is wrong.

The source is newly authored C++ using those original SDK fields and plain inline accessors/EAX wrapper bodies. The extracted SDK object is used only as private evidence, never as the rebuilt function. The authored source independently compiles under cl13.10.3077 to RELOCATION_MATCH on its first strategy, and objdump confirms the exact virtual/public leaf above.

# Version-specific complete headers

CCodaEnvironment has the release CCodaObject list base (vptr and two pointers, 12 bytes; the debug-only test word is absent), CENVIRONMENT at +0x0c, m_ffCommit at +0x78 and m_pReverb at +0x7c. All 0x80 bytes are retained. Both inline getters are independently present in the release SDK object with those exact offsets.

The native mix hierarchy differs from the later SDK: CSalReverb derives from the four-byte object root, with m_bActive +4; native CSalBaseReverb holds the complete aligned mix store through +0x21b and its pointer at +0x21c. Its native constructor proves those offsets; the factory proves sizeof 0x220. The native eight-slot vtable is represented using the SDK's real method names; later SDK-only query/list-base slots are absent. No extra partial class is substituted.

CSalOpenAL replaces the earlier descriptive CAudioContext identity because the original SDK Update object proves its global `g_OpenAL`, ALGetProcAddress and inline EAX3SetGlobal/EAXSetGlobal/EAXSet call chain. Retail has one extra pointer before the SDK resolver/EAX slots: m_pGetProcAddress +0xe8 and m_pEAXSet +0xf0. The full established native 0x1b0 object is preserved with opaque unmodelled API storage. The global and every member-address relocation bind one base address (0143bdb0) with the correct addend.

`SAL_EAX30_ListenerProperties` is the exact original GameCODA symbol, proven by the SDK relocation and the native GUID value A8FA6882-B476-11D3-BDB9-00C0F02DDF87. ALGetProcAddress is public, non-const, takes unsigned-char* and returns void*, exactly as the SDK symbol and type record show. It binds 00c4d740. The original EAXSet literal is checked independently. Wrong earlier descriptive spellings are in NAME_FIXES.tsv.

# Behavioral verification

The standalone fixture embeds the actual source and complete headers. Private access is opened only in the fixture for setup; Win32 longs become 32-bit ints on GNU64. It checks 270 dirty-mask/cache combinations, including every single relevant flag, mixed/random masks, ignored bits, the retail shared 0x80000 flag for both frequency references, lazy callback resolution and repeated resolution when the callback clears itself. It verifies ordered selectors, byte widths, exact original member pointers, input preservation and ignored return codes. GNU64 C++03 and VC7.1 /GR- both pass.
