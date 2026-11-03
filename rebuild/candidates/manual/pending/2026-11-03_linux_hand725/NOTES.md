# Chunk 725 (agent725) -- 23 / 24 RELOCATION_MATCH

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
- Not attempted: 00aa5c80 C3DMeshFileXLoader::C3DMeshFileXLoader (2144 B). Shape from 5c80.ann: base ctor 0x00A9BBE0, vtable
  0x0129E450, then 14 x { <XChunk> tmp("XBDS"...); RegisterRootSubChunkType(tmp) -> CCountedPointer temp released }, a few stored
  results with virtual +0x18 calls; aligned (and esp,-8) frame, no EH frame. Needs a C3DMeshFileXLoader model; budget went to 724.
- Attempt counts in STATUS/LOG are 1 per row (several rows were regraded during header work; all first-shape matches).
