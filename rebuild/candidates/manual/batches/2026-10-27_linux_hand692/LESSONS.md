# Chunk 692 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- Declaring a dimension local AFTER its first use (`if ((unsigned __int64)bpp * rect.Dimensions.X > 0xffffffff) return; long width = rect.Dimensions.X;`) makes VC7.1 keep the CSE'd load in a callee-saved register and removes most register swaps in the texture stream rows (0x009FB110 residue 251 -> 26, 0x00A68AE0 -> 42).
- Under /GS a struct local that contains a char ARRAY member is placed with the buffers (just under the cookie); retail's 20-byte load descriptor sat with the scalars, so its six bytes are named members, not `unsigned char[6]` (0x00C08D40 residue 138 -> 66). Frame order otherwise ignored declaration order and block scopes.
- libpk LTCG mod/ref: after a call to the out-of-line PKCAutoDelete::GetChecked(&holder), retail still reuses the register copy of the held pointer on early-return paths (a non-LTCG build reloads it): lane gap, park (0x00C08D40).
- A plain `inline` CDataInputStream::Read is outlined by VC7.1 inside a large body (634v850); `__forceinline` (the established inline-only pattern, no Ego_r copy) restores retail's inlined Read (0x009FB3F0).
- zlib 1.2.1 deflate.c rows match verbatim when the `local` helper (flush_pending) is a same-TU `static` in the `extern "C"` block: VC7.1 inlines two of its three sites exactly as retail (0x00C011A0).
- A ??_G stored in an RTTI-less vtable whose ctor is landed proves an implicit dtor's class; define the PDB ctor to emit it (0x00EE8C30).
- q.py: pass `--flags /EHsc /GS` as separate arguments; one quoted string is silently ignored (the build had no EH frame).

Review at landing:
- All four parked rows needed more than 4 grades (owner's 12-probe rule for this chunk): 0x009FB110 (27), 0x009FB3F0 (23), 0x00A68AE0 (18), 0x00C08D40 (15).
- Local base models next to existing engine headers (flattened, not usable as bases): CVertexShader / CShaderBase / CPixelShader, CIndexBuffer, CMovableResource, CSimpleMemoryPool, CVertexBufferWin32, CEngineComponent, CEngineSkyRenderer, CTexture, CPixelFormat, CDataOutputStream, CDataInputStream; engine/CLevelScriptBase.h declares an out-of-line dtor (retail's is implicit), so 0x00EE8C30 uses a local copy.
- Ego_r's CEngineClothRenderer layout uses a 16-byte CArray; retail's is 12 -- check struct_layouts before reusing.
- Already-landed sources with wrong spellings: 0x00EE8100 (ctor outside NScript, wrong parameter types), 0x00EE8C10 (CLandscapeBackgroundPatch vector deleting dtor), 0x00A681F0 (GetByteLength), 0x00C09920 (CAutoDeletePointer non-template), and the 0x009E3830 ctor bindings -- see NAME_FIXES.tsv.
- 0x00C011A0: zlib's deflate_stored is `local` (static) in deflate.c; the src uses extern "C" `_deflate_stored` to be gradable -- decide the landed linkage.
- 0x00EA4980/0x00EA4BB0: CEntityScriptBase::IsActiveThreadTerminating vs IsDone for 0x00F35B30 is still mixed in the label DB; the header spelling was used.
