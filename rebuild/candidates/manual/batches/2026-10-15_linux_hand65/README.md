# Hand chunk 65 (CPatchTesselationEdgeStrip .. CPersistContext): 35 of 40 byte-exact

All 35 sources in `src/` grade MATCH / RELOCATION_MATCH (`grades.txt`); all 35 tests print `<addr>_TEST PASS` under
g++ (`tall.sh`) and the lane's VC7.1 cl.exe (`tall.py`). `payload.json` + `oracle.tsv` written by `finish.py`.
Only 2 of the 15 "CPatchTesselationEdgeStrip" rows and 19 of the 24 "CPersistContext" rows really belong to those
classes: the rest are propagated labels (see Label corrections). Each source names the class the evidence gives.

## What matched (35)

- **CPatchTesselationEdgeStrip** (vtable 0x012A8318; PDB layout StartPos +4, Size +6, four `CEdgeStipVertexArray<T>`
  at +8/+0x10/+0x18/+0x20, Tesselated/Vertical +0x28/+0x29):
  - 00bfc3b0 `Reset(long startPos, long size, bool vertical, CMovableResource*)`: four `Resize(size + 1, res)` in source
    order Verts, WaterVerts, FanBaseVerts, WaterFanBaseVerts, then the word/byte stores. The `size` argument is a
    `long` (loaded as a dword for `lea edi,[ebp+1]`) stored to a `unsigned short` member; matched first try.
  - 00bfc4c0 `ResizeVertexCount(long, long, long, long, CMovableResource*)`: the four arrays in layout order.
- **CPerItemData** 005c14c4 copy constructor: out-of-line `vector<long>` copy (0x00514160) then six scalars. The lane
  rotates eax/ecx/edx for the copies; `#pragma optimize("s", on)` gives retail's eax-only copies. The float
  BestFishWeight copies as a dword through a memberwise init list (no FPU) in this form.
- **CPersistContext** (local model of the PDB 0x2c layout; `fable_persist_context.h` has no members):
  - 009bae10 / 009bae30 / 009bae50 constructors (text-save / binary-load / binary-save) and 009baa50 `Init(CDataOutputStream*, bool)`:
    the constructors inline the out-of-line Init; Init stores in the order (stream, Mode, other stream, +0, +0x15). The
    `bool` argument is unread in release.
  - 004aef50 `IsSaving()` (label "IsBinary"): `return Mode == 1 || Mode == 3;` as a `long` (full-register 0/1).
  - 009bad60 `TransferObjectSaveText` / 009bada0 `TransferSubComponentSaveText`: `*PStringToSaveTo += name; += " "; component->v0(this, PStringToSaveTo); += ";\r\n"` (".", `v0(this)` for the sub-component).
  - Two-argument Transfer forwarders that build a default and call the three-argument template on the same `this`:
    004ab810 `Transfer(name, double&)` with `0.0` (label said `<long>`; `fld [0.0 pool]`), 007e3aa0 `(long&)` with `-1L`
    (default stored after the pushes), 00800d30 `(C2DVector&)` with `C2DVector(0.0f, 0.0f)` (immediate stores = floats),
    00757160 `TransferMap(name, map&)` with a two-`long` struct (one zero register = integers), 00485b44 `(C3DVector&)`
    with `C3DVector(0,0,0)` in a frame-pointer size TU (`optimize("y",off)` + `("s",on)`: `fldz ; fstp` is the /Os zero).
    All are a literal temporary passed straight to the `const T&` parameter.
  - 00411010 / 00411060 `Transfer(name, CCharString& / CWideString&)`: retail default-constructs a temporary, copy-constructs
    the named default from it and destroys the temporary BEFORE the call. `T d = T();` and `T d = static_cast<const T&>(T());`
    are both elided by VC7.1; the shape is an inline helper returning a NAMED local (`static T DefaultValue() { T v; return v; }`
    -- no NRVO in VC7.1).
- **CInputProcessGameBase** (labels "Transfer<CEngineGraphic>") 00687cb0 / 00687d50: three-argument player-interface
  queries with `MyPlayer->GetPlayerNumber()` as the last argument (evaluated first); engine header + `_Methods` struct.
- **CCombatManager** (labels "IsBinary") 006e92c0 / 006e9270: one-argument twins of the landed 0x006E9310
  (`find(creature.GetUID())`, `it != end() && <field>`, explicit true/false): AttackerInfo.Attacked at value +4, and
  DefenderInfo.Attackers.size() at value +0x38 (retail AttackerInfo is 0x30 bytes with a 12-byte vector).
- **vector<CShopItemDef>::resize(n)** 004ed04d (label "Transfer<CAnimationSet>"): `resize(n, CShopItemDef())`; the
  0x48-byte temporary's inline destructor chain keeps only the CDefPointeeBase vtable store + the root destructor call
  (the derived vtable stores are dead and dropped because nothing runs between them). Frame-pointer size TU.
- **Destructors** (labels "~CPatchTesselationEdgeStrip"):
  - NShape::CShape / CClosedShapeBase / CSubdivisionTriangle `??_G` (00764fc0 / 00765080 / 00765370): a plain inline virtual
    destructor leaves `call ??1 ; delete` in the `??_G`; `__forceinline` on every destructor (and the vector destructor)
    inlines the chain. The derived classes' own vtable stores are dead (base store follows immediately) and vanish, so all
    three bodies store only 0x0126A208. A `new T(id)` factory with an inline constructor emits the vtable and the `??_G`.
  - 004ecafd CObjectAugmentationsDef::~ (vtable 0x01243974 via its `??_G` 0x004ECAE1): `__declspec(novtable)` over the
    landed CDefPointeeBase model, /Os (`pop ecx`) with `__forceinline` vector destructors; two out-of-line map destructors.
  - 0066ccf0 CRegionMinimap::~ (member of CMiniMapDisplay +0x54): CSurface's inline `~CSurface() { Uninitialise(); }` gives
    the vtable store + tail jump (0x009F2E20 "CSurface::Uninitialise" is right), CTexture's `{ Uninitialise(); }` the call.
  - 00aa7480 C3DMeshFileXAnimatedPrimitiveBlockChunk::~ (vtable 0x0129E404): novtable derived over the out-of-line
    CChunkedFileSuperChunk destructor (tail jump), two CArray frees, the vertex-group block destructor.
  - 00a5eb70 CVertexShaderInput::~ and its byte-identical N3DPrimitive::CVertexShaderInputStatic / Animated copies
    (00a98640 / 00a986d0, the classes' stack locals in 0x00A91100/0x00A91140): novtable, the landed CRetailArray pattern
    (`DestroyRange(first, last, Tag())` + `if (first) free(first)`, tag homed at `[esp+7]`), out-of-line CBaseClass::~.
  - 00a2d710 CMeshBankStateBlock::~ (vtable 0x0129CE7C): the inline STLport `set<CCharString>` destructor = `clear()`
    (count test, `_M_erase(root)`, leftmost/root/rightmost/count reset with the header re-read) + `if (header) free(header)`;
    the empty CBankStateBlock destructor's vtable store is the last instruction (novtable derived).
  - 00ac39c0 non-virtual `??_G` of an unnamed binary tree node (children +0x3c/+0x40, array +0x30): inline destructor
    `delete m_pChildA; delete m_pChildB;` (recursive `push 1 ; call ??_G`), emitted by a `delete node` helper; leaf
    `??_GSBinaryTreeNode_00ac39c0@@QAEPAXI@Z`.
  - 00b231e0 the counted-pointer deleter of CSplineRandomOffsetData (stored at +4 of the count block by Reset 0x00B23240):
    `__fastcall`, written as the expanded delete `if (p) { p->~T(); operator delete(p); }` with `__forceinline` CArray
    destructors (a plain `delete p` of a non-polymorphic class calls an out-of-line `??_G`).
  - 009c5110 CFileInstaller::FreeBuffers (not a destructor; called by InitBuffers and the installer destructor): stack
    CCriticalSectionLock (engine header + a derived struct for the out-of-line ctor/dtor), `DeallocateMemory`, two
    `if (p) { GlobalFree(p); p = 0; }` with `__declspec(dllimport)` GlobalFree (import loaded once into edi).

## Parked for Fable 5.1 (5)

- 009ba9d0 CPersistContext::Init(6 args) DIFFER(58v58): retail copies each pointer argument through ONE register (`mov edx,[esp+10h] ; mov [ecx],edx ; mov edx,[esp+14h] ; mov [ecx+4],edx ...`) and zeroes eax before the +0x10 store, the lane interleaves eax/edx loads ahead of the stores | tried: plain body, `optimize("s"/"t"/"y")`, `g` off, assignments through a reference-taking inline helper, volatile members | next: lane gap: copy-register scheduling (RTM vs QFE)
- 009badd0 CPersistContext::CPersistContext(6 args) DIFFER(60v60): the same single-register (edx) copy sequence as 009ba9d0 inlined into the constructor; the lane alternates ecx/edx | tried: Init inlined into the constructor, `optimize("s")` (stops the inlining, 36 bytes) | next: lane gap: copy-register scheduling (same as 009ba9d0)
- 009bb0d0 CPersistContext::CleanOutLine DIFFER(50v50): only the SIB operand order: retail `cmp byte ptr [eax+ecx],0Ah` / `mov byte ptr [eax+ecx],20h` (index i as base), the lane `[ecx+eax]` (text as base) | tried: ternary text accessor (gave a jmp diamond), default-first accessor (fixed everything else), `i[text]`, `optimize("s")` | next: try `operator[]` on CCharString or an `unsigned char*` text; else lane gap: address-mode operand order
- 004310a7 CPersistContext::TransferWideString DIFFER(91v91): the two 4-byte string frame slots are swapped: retail default local `[ebp-4]`, read temporary `[ebp-8]`; the lane `[ebp-8]` / `[ebp-4]` | tried: body local after CheckCRC, named local in the case block, default as an argument temporary to an inlined helper, explicit `operator=` call, const-ref bound temporary | next: lane gap: frame-slot order of a local vs a later temporary
- 00431143 CPersistContext::TransferString DIFFER(91v91): same frame-slot swap as 004310a7 (CCharString twin) | tried: the same five forms | next: lane gap: frame-slot order (as 004310a7)

## Skipped

None (no row spans two functions).

## Label corrections

- 0x004AEF50 "CPersistContext::IsBinary" is `IsSaving()`: Mode 1 = text save (PStringToSaveTo), Mode 3 = binary save
  (PSaveStream), per the constructors 0x009BAE10 / 0x009BAE50; Mode 2 = binary load, Mode 0 = text load (PParser).
- 0x009BA9D0 / 0x009BAA50 "CPersistContext::CPersistContext" do not return `this`: they are the void `Init` overloads the
  real constructors 0x009BADD0 / 0x009BAE50 inline (as are 0x009BAA10 / 0x009BAA30 for 0x009BAE10 / 0x009BAE30).
- 0x006E9270 / 0x006E92C0 "CPersistContext::IsBinary" are CCombatManager queries (twins of the landed 0x006E9310).
- 0x00687CB0 / 0x00687D50 "CPersistContext::Transfer<CEngineGraphic>" are CInputProcessGameBase::IsAssignableSpecialMoveButtonInQueue
  and IsGameActionEventInQueue (2-argument forwarders to 0x004460C0 / 0x00446B10).
- 0x004AB810 "Transfer<long>" is Transfer<double> (default 0.0 double); 0x00757160 "Transfer<C2DVector>" is
  TransferMap<CObjectAugmentationParticleSet, EObjectAugmentationType>; 0x004ED04D "Transfer<CAnimationSet>" is
  `vector<CShopItemDef>::resize(n)` (0x004ED07D, labelled "CArray<pair<ulong,CSoundPair>>::push_back", is resize(n, value)).
- Callee labels propagated: 0x004AB710 / 0x007E39D0 / 0x00800B90 "Transfer<signed char>" are Transfer<double> / <long> /
  <C2DVector>; 0x004859AD "Transfer<CFloatRange>" is Transfer<C3DVector>; 0x004109A0 / 0x00410AC0 are Transfer<CCharString> /
  <CWideString>; 0x0099A2E0 ("CBase") is CBaseClass::~CBaseClass (vtable 0x01231710); 0x00A5E900 is the CStreamInfo range
  destroy; 0x004DD32B / 0x004EB98F ("CActiveFile::OnReadFinished") are the CObjectAugmentationsDef map destructors;
  0x009F9F70 / 0x009F2E20 are CTexture / CSurface::Uninitialise (correct) used as their inline destructors; 0x00A96DA0 is
  CVertexGroupBlock::~; 0x00AA8490 is CChunkedFileSuperChunk::~; 0x00A2CFE0 is CLODStateBlock::~; 0x004ABA70 is an STLport
  `_M_erase`; 0x009C76D0 is CArray<CFileInstaller::CBufferChunk>::DeallocateMemory.
- The "~CPatchTesselationEdgeStrip" / "SetMovableResourceHandler" rows are: 0x004ECAFD CObjectAugmentationsDef::~,
  0x0066CCF0 CRegionMinimap::~, 0x00764FC0 / 0x00765080 / 0x00765370 NShape::CShape / CClosedShapeBase / CSubdivisionTriangle
  `??_G`, 0x009C5110 CFileInstaller::FreeBuffers (name by behaviour), 0x00A2D710 CMeshBankStateBlock::~, 0x00A5EB70
  CVertexShaderInput::~, 0x00A98640 / 0x00A986D0 N3DPrimitive::CVertexShaderInputStatic / Animated::~, 0x00AA7480
  C3DMeshFileXAnimatedPrimitiveBlockChunk::~, 0x00AC39C0 a no-RTTI binary tree node `??_G`, 0x00B231E0 the
  CSplineRandomOffsetData counted-pointer deleter.
