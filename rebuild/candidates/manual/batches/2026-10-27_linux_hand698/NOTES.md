# Hand chunk 698 (CTCSpecialAbilities .. CCompressorZlib): 10 of 10 byte-exact

All ten NA041 rows were named from their own vtable / ??_G / caller / donor-order / PDB evidence before authoring.
10 RELOCATION_MATCH on the first probe each (no flags except /EHsc on the strmbase row, no pragmas, no retail_asm, no
volatile); every test PASS under g++ and VC7.1; calls.py: every call target is retail's. Nine of the ten catalog labels
were wrong (all nine "~CPatchTesselationEdgeStrip" rows are ten different classes' destructors, see below); the tenth
(~CCompressorZlib) was right.

## What matched (10)
- **CTCSpecialAbilities::~CTCSpecialAbilities 0x0072DA60** -- implicit (no 0x0126683C store; DestroyOne emission helper):
  AbilityAvailability (STLport byte vector<bool>, ~_Vector_base null-tested free), then UndeadFollowers (~_List_base = the
  out-of-line clear() 0x0072D4D0 + the sentinel's null-tested free), then ~CTCBase. Real STLport list/vector templates.
- **CTCTavernGameCardBlackjack::~CTCTavernGameCardBlackjack 0x007DCA90** -- implicit: Bets (vector<long>) and DealersCards
  (vector<CCard*>) expand inline, PlayersCards' ~vector<vector<CCard*> > keeps its element loop and stays out of line
  (VC7.1 decides; it emits exactly retail's 0x007DC890 symbol), then ~CTCTavernGameCardBase.
- **CCRCSymbolMap::~CCRCSymbolMap 0x004D1D50** -- implicit: NamesUsed set<unsigned long> (STLport ~_Rb_tree clear + header
  free, _M_erase 0x004CF810 private) then SLONGMap (CVectorMap -> CArray -> vector<pair<unsigned long,long> >), then
  ~CBaseClassNonCopyable. Models from the landed ctor 0x00A38500 / CVectorMap::DeallocateMemory 0x00A39900.
- **C3DAnimationStats::~C3DAnimationStats 0x00A4EB50** -- user `virtual ~C3DAnimationStats() {}` (the vtable store),
  TimingEvents multimap<CCharString,float> (_M_erase 0x00A54240 under its bound PDB spelling), then ~CBaseObject (MAE).
- **CEngineWorldMap::~CEngineWorldMap 0x00B42E70** -- user `{ CloseStaticMapFile(); DeleteMap(); }`, then ChangedAreas,
  StaticMapFile (implicit ~CStaticMapBankFile = a call of ~CBankFileAsync), StaticMapBankName, VisibleMapList (STLport
  `~list() {}` inline, ~_List_base out of line = retail 0x00B43BB0, cross-graded RELOCATION_MATCH), MapList, ~CEngineComponent.
- **CDynamicOutputPin::~CDynamicOutputPin 0x00CA9690** -- DX9 BaseClasses amfilter.cpp body verbatim over the landed
  strmbase model (CDynamicOutputPin_BlockOutputPin_00ca9820.cpp) plus wxutil.h's `~CCritSec() { DeleteCriticalSection(...) }`;
  CBaseOutputPin's implicit dtor; tail ~CBasePin. /EHsc like the other strmbase rows.
- **CNavigationAreaInit::~CNavigationAreaInit 0x0051D590** -- implicit: NavRegionSeeds, DetailedAreas, then the base
  ~CNavigationAreaUpdate (declared, out of line at 0x0051D530).
- **CCombatDebugInfo::~CCombatDebugInfo 0x006F2FD0** -- implicit: AttackerPos, PlacementVectors (vector<C3DVector>), then
  CurrentSequenceName's ~CCharString as the tail jump.
- **C3DMeshFileXUVTangentChunk::~C3DMeshFileXUVTangentChunk 0x00AA7320** -- implicit: VTangents, UTangents
  (CArray<C3DVector>), then ~CChunkedFileChunk.
- **CCompressorZlib::~CCompressorZlib 0x00A3ACC0** -- user `if (ZlibInitialised) { if (Compressing) FinishCompressStream();
  else FinishDecompressStream(); }` with both members and SetupStream(z_stream_s&, ...) defined earlier in the TU (auto-inlined;
  the same file regrades RELOCATION_MATCH at 0x00A3A930 and 0x00A3ABB0), then the implicit ~CACompressor (OwnWorkspace free,
  no 0x0129B424 store) and ~CBaseClass. zlib callees under the Z_PREFIX names z_deflateEnd / z_inflateEnd.

## Names and evidence (all proven from two sources; none descriptive)
| row | symbol | evidence |
|---|---|---|
| 0x0072DA60 | ??1CTCSpecialAbilities@@UAE@XZ | ??_G 0x0072DA40 slot 0 of vtable 0x0126683C (RTTI); Ego_r 0x007448F0 + FableWin 0x0254AD70; landed clear() 0x0072D4D0 names it |
| 0x007DCA90 | ??1CTCTavernGameCardBlackjack@@UAE@XZ | ??_G 0x007DCA70 slot 0 of vtable 0x0126F8F4 (RTTI : CTCTavernGameCardBase); Ego_r 0x007D9050 + FableWin 0x02756BD0; Ego_r members DealersCards / PlayersCards / Bets |
| 0x004D1D50 | ??1CCRCSymbolMap@@UAE@XZ | ??_G 0x00A399E0 slot 0 of vtable 0x0129CF84 (RTTI); atexit dtor 0x01228DD0 of global 0x013B8A54; Ego_r 0x004F0700 + FableWin 0x01B829A0 (both far from the class's TU: implicit) |
| 0x00A4EB50 | ??1C3DAnimationStats@@UAE@XZ | vtable 0x0129D514 stored first (RTTI); ??_G 0x00A555A0 + C3DAnimation2 dtor tail 0x00A4EF86; Ego_r + FableWin; binding exists |
| 0x00B42E70 | ??1CEngineWorldMap@@UAE@XZ | vtable 0x012A1F50 stored first (RTTI); ??_G 0x00B44440; Ego_r 0x009E8650 + FableWin 0x02D5F5F0 |
| 0x00CA9690 | ??1CDynamicOutputPin@@UAE@XZ | the four CDynamicOutputPin vtables its ctors store (landed strmbase rows); slot-3 dtor entry 0x00CAB750; amfilter.cpp body (strmbase has no RTTI / PDB names: source names) |
| 0x0051D590 | ??1CNavigationAreaInit@@QAE@XZ | Ego_r donor order ??1CNavigationAreaUpdate -> ??1CNavigationAreaInit = retail 0x0051D530 -> 0x0051D590 (tail call); Ego_r layout (base + WorldArea + MapIndex + two vectors); FableWin 0x01C8E290 |
| 0x006F2FD0 | ??1CCombatDebugInfo@@QAE@XZ | FableWin ??1 0x01D8EAF0 and copy ctor 0x0241C5C0; retail copy ctor 0x006F3000 next to it, same members; landed GetCombatDebugInfo 0x006EAC50 builds the record; Ego_r layout |
| 0x00AA7320 | ??1C3DMeshFileXUVTangentChunk@@UAE@XZ | own ??_G 0x00AA7370 (vtable 0x0129E3C0, RTTI) expands the same body; position in the class's run; Ego_r UTangents/VTangents; FableWin 0x0340EA40; the only chunk class with two trivial arrays at +0x10/+0x1C |
| 0x00A3ACC0 | ??1CCompressorZlib@@UAE@XZ | vtable 0x01245974 stored first (RTTI); ??_G 0x0052AE40 + 12 owners; Ego_r 0x00B95B10 + FableWin 0x031FA2D0 |

Three rows have no callers at all (0x0051D590, 0x006F2FD0, 0x00AA7320): no call, jump, absolute reference or vtable slot
reaches them (xref.py); each is a compiler-generated member the linker kept (0x006F2FD0's copy-ctor neighbour 0x006F3000
is unreferenced too; 0x00AA7320's ??_G inlines its own copy). They were named by donor order / FableWin symbols / layouts.

Callee names (checked by address with calls.py; every target is retail's): free 0x00BFEA14, ~CTCBase 0x00686830,
_List_base<CUndeadFollower>::clear 0x0072D4D0 (landed leaf), ~vector<vector<CCard*> > 0x007DC890, ~CTCTavernGameCardBase
0x007D5E80, set<unsigned long> _M_erase 0x004CF810 and multimap<CCharString,float> _M_erase 0x00A54240 (bound PDB
spellings), ~CBaseClassNonCopyable 0x0099A300 (MAE), ~CBaseObject 0x0099A430 (MAE), CloseStaticMapFile 0x00B40000,
DeleteMap() 0x00B42C70 (AAE), ~CBankFileAsync 0x009D6490, ~CWideString 0x0099B510, ~_List_base<CEngineMap*> 0x00B43BB0,
~CEngineComponent 0x00B59730, CloseHandle / DeleteCriticalSection IAT 0x0143FE30 / 0x0143FDE4, ~CBasePin 0x00CA7C80,
~CNavigationAreaUpdate 0x0051D530, ~CCharString 0x0099EAE0, ~CChunkedFileChunk 0x00AA75C0 (UAE), z_deflateEnd 0x00C00AE0,
z_inflateEnd 0x00C038D0, ~CBaseClass 0x0099A2E0. New spellings that differ from a landed one are all in NAME_FIXES.tsv; none
collides with another address's spelling.

## Parked for Fable 5.1 (0)

## Skipped
None. Extents: all ten oracle extents are right; each ends with its tail `jmp` followed by int3 padding.

## Label corrections
- 0x0072DA60 / 0x007DCA90 / 0x004D1D50 / 0x00A4EB50 / 0x00B42E70 / 0x00CA9690 / 0x0051D590 / 0x006F2FD0 / 0x00AA7320
  "CPatchTesselationEdgeStrip::~CPatchTesselationEdgeStrip" -> ~CTCSpecialAbilities, ~CTCTavernGameCardBlackjack,
  ~CCRCSymbolMap, ~C3DAnimationStats, ~CEngineWorldMap, ~CDynamicOutputPin, ~CNavigationAreaInit, ~CCombatDebugInfo,
  ~C3DMeshFileXUVTangentChunk.
- Landed neighbours with wrong names (NAME_FIXES.tsv): 0x007DC890 CRetailVector_SBufferEntry12 dtor (is
  ~vector<vector<CCard*> >), 0x00B40000 CloseStaticMap (is CloseStaticMapFile), 0x00B42C70 DeleteAllMaps (is private
  DeleteMap()), 0x00B43BB0 SortTreeRecursively_00436900 (is ~_List_base<CEngineMap*>), 0x0051D530 CVectorQuad_0051d530 (is
  ~CNavigationAreaUpdate), 0x006F3000 CRecord_006f3000 copy ctor (is CCombatDebugInfo's), 0x00AA7370 CChunkedFileChunkBuffers
  ??_G (is C3DMeshFileXUVTangentChunk's) with a QAE ~CChunkedFileChunk binding (retail UAE), 0x00A3ABB0
  CCompressorZlib_Retail::FinishDecompressStream, the zlib bindings _inflateEnd / _deflateEnd / _EndDeflateStream (retail
  _z_inflateEnd / _z_deflateEnd), 0x0072DA40 / 0x00A399E0 ??_G rows landed as free __fastcall stand-ins, 0x007D5E80 manifest
  "Catch@0047e8cc" (is ~CTCTavernGameCardBase).

## Header debt
- engine/CTCBase.h, engine/CEngineComponent.h, engine/CCompressorZlib.h, engine/CEngineWorldMap.h flatten the vptr into a data
  member (and CCompressorZlib.h the CACompressor base into pads): local polymorphic models were used (0x0072DA60, 0x00B42E70,
  0x00A3ACC0). engine/CEngineWorldMap.h also keeps the Ego_r members after ChangedAreas (IntermidiateStaticMapFile ..
  CommonHeaderPosLookupTable) that this build does not have: retail's destructor stops at ChangedAreas +0x1B4.
- engine/CTCTavernGameCardBlackjack.h does not exist; the Ego_r base size (0x338) is Dinkumware: retail's base is 0x318.
- engine/CBaseClassNonCopyable.h, engine/CBaseClass.h, engine/CBaseObject.h (FABLE_RETAIL_BASE_OBJECT) were used as-is.
- CNavigationAreaUpdate's destructor is declared (out of line at 0x0051D530) in 0x0051D590's model; retail's is probably
  compiler-generated too (FableWin puts it in CWorldMap's TU), which needs the full CNavigationLayerUpdate element model and
  its out-of-line __fastcall range destroy 0x0051CD80 to reproduce; the declaration gives the same call.

## vland (read-only) result
WINS 10, no RELOC_IDENTITY / RELOC_RENAMED. Warnings: SHARED_HEADER_AVAILABLE CEngineComponent on 0x00B42E70 (engine header
flattens the vptr; local polymorphic model, header debt above), SHARED_HEADER_AVAILABLE CBaseObject on 0x00CA9690 (the known
strmbase false positive: DirectShow's CBaseObject is not the engine's, GOTCHAS chunk 594), RELOC_NAME on 0x00B42C70 /
0x00B43BB0 (catalog labels DeleteAllMaps / StdList_Clear_00b43bb0: NAME_FIXES.tsv carries the proven spellings).
