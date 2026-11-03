# Chunk 725 (agent725 + agent725b) -- 24 / 24 MATCH or RELOCATION_MATCH

Rows: 12 "CChunkedFileChunk" byte-twin labels and 12 tiny "CScriptThing" labels. Both labels were wrong for most rows.

- The 11-byte `mov [ecx],vt; jmp base_dtor` "~CChunkedFileChunk" twins are destructors of 10 different classes; each is named by
  the vtable it stores first (RTTI where present: CXMVPlayerBuffers, CChunkedFileChunk; otherwise CVtable_<va>, libpk / GameCODA,
  CCodaObjectBase root per D3). Model: `class Derived : public Base { virtual ~Derived(); }` with Base's dtor declared out of line
  (gen.py). ~CVtable_012b4f9c is `{ Close(); }` (virtual call inside a dtor binds statically).
- The 8/11-byte "CScriptThing::operator==" twins are member forwarders through a pointer member's vtable:
  CDisplayEngine -> CIEngine (Engine3D +0x2C), CGameCameraManager -> NCameraModes::CBase (CurrentMode), CTCCreatureNavigation ->
  CNavigatorBase (PNavigator), CThingSoundEmitter -> CASoundEmitter (+0x98). Names from FableWin.pdb (two sources: FableWin + Ego_r
  member list / TU order).
- Header overlays in hdr/engine (hdrcheck2 default=same): CIEngine (FABLE_RETAIL_CIENGINE, full slot list 0..16),
  CDisplayEngine, CTCCreatureNavigation, CGameCameraManager (CCountedPointer<NCameraModes::CBase> CurrentMode under the macro),
  new NCameraModes_CBase.h.
- 00aa5c80 C3DMeshFileXLoader::C3DMeshFileXLoader (2144 B): RELOCATION_MATCH on attempt 3 (agent725b). Callees paired with the
  object relocations one-to-one (callchk.py); names from FableWin.pdb (+ Ego_r for the Register* members). Levers:
  1. X chunk classes get NO user destructor. An inline `virtual ~X(){}` re-stores the derived vtable in each temporary's
     teardown (2203 v 2144 B); the implicit dtor calls the base dtor directly, as retail does.
  2. Overloaded virtuals: VC7.1 lays them out in reverse declaration order. Retail calls non-const GetAsSuperChunk() at slot 6
     ([edx+18h]), so CChunkedFileChunk declares the non-const before the const (0x00AA8460 InitNewInstance agrees).
  3. `CCountedPointer<CChunkedFileChunk> p; p = Register...();` (default-construct then assign) for the kept result.
  New overlay headers: C3DMeshFileXLoader.h, C3DMeshFileLoader.h (22 CCountedPointer members, Register*SubChunkType
  out of line), CChunkedFileLoader.h, C3DMeshFileXAnimatedPrimitiveBlockChunk.h ("XAPB", 0x60), CVertexCompressionParams.h,
  CVertexGroupBlock.h (opaque), C3DIndexedBoneWeight.h; FABLE_RETAIL_<CLASS> branches in CChunkedFileChunk,
  CChunkedFileSuperChunk, C3DMeshFileTrianglesChunk and the eight XBDS/XBDB/XUVT/XMIF/XVBD/XTST/XTID/XAPR chunk headers
  (default branch = generated header).
  Name fix: 0x00A99FF0 is ?DeleteData@?$CCountedPointer@VCChunkedFileChunk@@@@SIXPAX@Z (FableWin), bound as ?Destroy_00a99ff0@@YIXPAX@Z.
- hdr/ is the complete merged overlay for 724 + 725 (identical to agent724b/hdr). All 24 rows regraded against it (LOG "rg").
- Attempt counts in STATUS/LOG are 1 per row (several rows were regraded during header work; all first-shape matches).
