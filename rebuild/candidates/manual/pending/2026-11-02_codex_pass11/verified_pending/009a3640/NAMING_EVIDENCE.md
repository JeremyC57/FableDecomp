#009a3640 naming and fidelity evidence

The original identity is the anonymous-namespace free function `InitializeStreamingFontTextureCache(CStreamingFontDataBank*)`. Original Ego_r PUBLIC is `?InitializeStreamingFontTextureCache@?A0xfe46a155@@YIXPAVCStreamingFontDataBank@@@Z`, donor00aee990; FableWin has the same spelling at02fb40e0. Its native caller009a3cba is in CStreamingFontBank's constructor. The argument is in ecx and there is no stack pop at return, matching the PDB fastcall void(pointer) prototype. The complete assigned1490-byte extent is009a3640..009a3c12 exclusive, ending with ret before alignment. objdump -t emits exactly the declared Leaf apart from the compiler's translation-unit anonymous-namespace hash; only that hash is normalized, not any name/type/access text.

## Native globals and original module records

Original release/lib_streaming_font_bank.obj DATA32 records give every variable's real name and type. Original PUBLIC records independently give exact namespace/type manglings. The native contiguous blocks preserve the original donor ordering, widths and distinct use roles; five file-header offsets0,4,8,12,16 directly identify the five metadata receivers. Array allocation multipliers8,4,2 independently confirm the element types/sizes.

|Native base|Original name|Original type / donor segment3 offset|
|---|---|---|
|013bda24|textureCacheMem|unsigned char* /e58d4|
|013bda28|textureCacheSize|unsigned long /e58d8|
|013bda2c|glyphTextureCacheGlyphDataSize|unsigned long /e58dc|
|013bda30|glyphTextureCacheGlyphDataNumIndices|unsigned long /e58e0|
|013bda34|glyphTextureCacheGlyphDataIndices|unsigned long* /e58e4|
|013bda38|glyphTextureCacheGlyphData|void* /e58e8|
|013bda3c|glyphTextureCacheGlyphDataCache|CStreamingGlyphData* /e58ec|
|013bda40|glyphTextureCacheGlyphAdjustment|CStreamingGlyphAdjustment* /e58f0|
|013bda44|textureCacheNumGlyphs|unsigned long /e58f4|
|013bda48|textureCacheNumGlyphAdjustments|unsigned long /e58f8|
|013bda4c|textureCacheMemOffset|unsigned long /e58fc|
|0138e068|numTextureCacheGlyphDatasPerChunk|unsigned long /23f58|
|0138e06c|glyphTextureCacheGlyphDataCacheChunk|unsigned long /23f5c|
|0138e070|GFontTextureBytesPerPixel|long /23f60|
|013c5a80|GFontTextureCache|CTexture /ed92c|
|013c5a88|GFontPixelLookupTable|CArray<unsigned char> /ed934|

All above are in the original anonymous namespace. The lookup vector's+4 relocation addends mean its finish member, not a separately invented global.

`CMemoryAllocator::PMemoryAllocator` at013caa90 is the original protected static classV pointer, PUBLIC `?PMemoryAllocator@CMemoryAllocator@@1PAV1@A`, donor segment3+1301b0. The native ensure/create/enter/allocate/leave sequence and protected TPI member declarations identify the singleton and the real static Allocate wrapper.

`GD3DFormats` at0129ba40 is the original const array of classV CD3DFormatInfo, PUBLIC `?GD3DFormats@@3QBVCD3DFormatInfo@@B`, donor segment2+9edb8. Original TPI gives its36-byte stride and public D3DFormat,PFType,BitDepth,AlphaBits,RBits,GBits,BBits,StencilBits,Linear fields. Native index*36 and relocation addends0xc/0x10/0x14/0x18 select exactly those channel fields. It is not one of the many old ad-hoc table types in reloc_bindings.

## Callees

Every emitted target is listed in RELOCATIONS.tsv with the original exact spelling. Key corrected identities are:

-009f2870: protected static fastcall CMemoryAllocator::Create(), not a free allocator creator.
-009f2000/009f2010: protected static fastcall EnterSynchronisation/LeaveSynchronisation, not public methods or free lock functions.
-009f2a40: protected thiscall InternalAllocate(long,unsigned long,bool).
-009e3830: public CPixelFormat::Initialise(_D3DFORMAT), reached through the original inline constructor; see the independently reviewed009c0880 proof.
-00485f3a: public STLport vector<unsigned char>::_M_fill_insert(unsigned char*,unsigned int,const unsigned char&), established by real STLport4.6 source plus native three-stack-argument call and uchar element stride.
-009fa280: public CTexture::Initialise(const C2DExtentsI&,long,const CPixelFormat&,unsigned long,ESurfacePool,bool,bool). Both object arguments are references, and the pool is the real enum.
-009a4ec0: free fastcall GFGetSystemManager() returning classV CSystemManager*.
-009be6c0: public CDisplayManager::GetBestAlphaTextureFormat(unsigned long,CPixelFormat&), not a pointer parameter. The caller passes32 and a writable default-constructed CPixelFormat; original TPI/PUBLIC confirm its reference.
-00a5f010: public CStreamingFontDataBank::StreamGlyph(void*,unsigned long,unsigned long,bool), returning CCountedPointer<CBankFileAsyncData> by value through a hidden result pointer. Original donor00bfe570 and its neighboring ReadData/InitStreamingEntry order match native; five header reads use the same receiver and wait flag.
-00a5ef30: public CStreamingFontDataBank::ReadData(void*,unsigned long,unsigned long), original donor00bfe490. Three native calls receive the table pointer, file offset and byte length.
-00bfeb22 is array new, emitted as??_U. Explicit array declarations preserve that binding. Counted-pointer control blocks call scalar delete00bfe9bc, emitted as??3; these are deliberately distinct.

Original TPI gives CCountedPointer<T> public Data+0 and Info+4 and CCPPointerInfo's count/deleter/data fields. The supplied genuine engine header reproduces the five temporary destructors, including the reload after count decrement, fastcall deleter, and scalar control-block delete. There is no artificial pointer wrapper.

## Recovered locals and header layout

FableWin module debug/lib_streaming_font_bank.obj supplies original local names and types: data_bank, success, pixel_format, table_ptr, c_val, pixel_val, glyphDataOffset, glyphIndicesOffset, glyphIndicesSize, glyphAdjustmentOffset and glyphAdjustmentSize. Separate nested pixel_format/table_ptr/c_val/pixel_val locals exist in the two fallback scopes. Their offsets and types are retained in PDB_NAMING_PROOF.json.

The decisive additional recovery is CPixelFormat::GetAPosShift's original `const CD3DFormatInfo& format` local. Restoring that actual alias prevented the compiler from merging two channel sums and reproduced the native alpha-loop register allocation. CPixelFormat::GetAMask has only `this` in the original debug local records. Fixed-base `table_ptr[c_val]` reproduces the four-way unrolled32-bit loop; incrementing table_ptr changed the generated address updates.

CSystemManager uses its corrected native0x180 storage layout: real CInitBaseClass base, original members/access, PDisplayManager+0x60, and named opaque storage for unused WinClass, SystemColours and ApplicationName composites. Its sizeof is checked under VC. CStreamingFontDataBank and CMemoryAllocator are explicitly documented declaration-only API projections: the production function neither constructs nor sizes them nor accesses their fields. No sizeof/layout claim is made for those projections. The original generated CStreamingFontDataBank default header remains untouched behind its opt-in branch.

CStreamingGlyphData is the original classV8-byte record with OffsetX/OffsetY/Width/Height and MemOffset23/Advance8/BigChar1 bitfields. CStreamingGlyphAdjustment is classV2-byte Scale/Baseline. All new accessors and enum values use original PDB spelling. The existing display/signature/geometry header branches are preserved.

## Historical resize signature correction and exact result

The prior selected by-value resize model differed in six temporary-slot bytes. Its value signature came from the later Anniversary PDBs and STLport4.6, not a proven retail ABI. Authentic STLport4.5.2 `_vector.h:480` supplies the original const-reference overload, with the same one-argument forwarding overload at486. This row passes each caller-created byte directly to the reference-reading fill callee, and a separate retail out-of-line resize chain independently confirms that historical signature exists in Fable.exe.

Recompiling with that real historical overload and all current header cleanups gives exact1490-byte parity at attempt27. The earlier rejection of exact attempt12 is explicitly superseded, not erased. Full proof and its limits are in RESIZE_SIGNATURE_PROOF.md. This row's resize is inlined, so its calls independently establish the fill reference contract, while historical source plus native lifetime/codegen select the resize source form. All112 external operands are separately audited in RELOCATIONS.tsv; no symbol is changed merely to satisfy masking.

The earlier0x178 claim was the donor layout and is superseded. Native constructor009a7440 initializes an additional eight-byte composite at84..8b; the later original members shift by8. Native final callback fields are independently read/stored at174/178/17c, and009a5010 returns0x180. The extra composite has opaque storage until its identity is proven. Original PDisplayManager+60 is unchanged. Evidence is evidence/009bca20/SYSTEM_TAIL_NATIVE.txt; the fixture was regenerated and independent byte/relocation/dual-test review rerun after correction.
