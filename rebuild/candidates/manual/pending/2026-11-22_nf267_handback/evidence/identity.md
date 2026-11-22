# NF267: the CEngineLandscapeMemoryManager Delete trios, second-angle proof (RULINGS D2)

Working claim (NF265, D2): the queue trio 0x00BDA090 / 0x00BDA0D0 / 0x00BDA110 is the PUBLIC DeleteIndexBuffer / DeleteVertexBuffer /
DeleteTexture; the destroy-now trio 0x00BD9C00 / 0x00BD9C20 / 0x00BD9CC0 is the PRIVATE Delete<X>BlockWhileBusy.

## Angle 1: access (three PDBs) and who calls each trio (retail)
- Ego_d.pdb type record of CEngineLandscapeMemoryManager (LF_FIELDLIST 0x68318, $S/dl_debug/types.txt): DeleteIndexBuffer (0x68304),
  DeleteVertexBuffer (0x68305), DeleteTexture (0x68306) `attrs = public`; DeleteTextureBlockWhileBusy (0x68306),
  DeleteVertexBufferBlockWhileBusy (0x68305), DeleteIndexBufferBlockWhileBusy (0x68304) `attrs = private`. Same procedure types pairwise,
  so the parameter (CIndexBuffer* / CVertexBufferWin32* / CTexture*) and void return cannot tell the twins apart; only access and body can.
  No other public member takes one of those three pointer types and returns void (AllocateTexture etc. have other types). Ego_r.pdb and
  FableWin.pdb name sets: the same six names, public `QAE` / private `AAE` (ghidra_out/egor_pdb_names.tsv, fablewin_pdb_names.tsv, grepped this session: Ego_r 009c2370 / 009c23a0 / 009c2450 private,
  009c25e0 / 009c2640 / 009c26a0 public; FableWin 02cb73d0 / 02cb7410 / 02cb7390 private, 02cb7350 / 02cb7450 / 02cb7490 public).
- Friend records: the Ego_d type dump has no LF_FRIENDCLS record anywhere (0 hits in 499,444 records), so the dump cannot show friends
  and the "no friend" argument is NOT used.
- Retail callers (`$S/w22/nftools/callers.py`, evidence/callers_trios.tsv): the queue trio has 22 E8 call sites: 16 inside named members of other
  classes (~CLandscapeBackgroundPatch 0x00BE6090, CLandscapeBackgroundPatch::ClearTextures 0x00BE5C30, ~CEngineWaterBackgroundSubPatch
  0x00BFD480, ~CLandscapeLayerMesh 0x00BFDE20, CEngineScreenEffectOutlineGlow 0x00BEAAA0 / 0x00BEB4D0, 0x00BEAA00, 0x00BFDD40, and six
  in unlabelled code near 0x00BE3807 / 0x00BFD205 / 0x00BFD530 / 0x00BFD8C4 / 0x00BFDAD0, all outside the manager's own address
  range 0x00BD9AF0 .. 0x00BDA5xx); none inside the manager. With access only (friends cannot be ruled out from the dump), this is
  consistent with public, not a proof on its own; the conclusion rests on the four legs together: outside callers, the bodies (queue v
  destroy-now), the sizes, and three PDBs agreeing on public Delete<X> / private Delete<X>BlockWhileBusy.
- The destroy-now trio has no E8/E9 caller and no 4-byte absolute reference anywhere in Fable.exe (callers.py + absrefs.py; absrefs
  control: 0x01436EA8 lists its readers). Its bodies are inlined into the manager's own ProcessDisposalList 0x00BDA150 and
  FreeDisposalList 0x00BD9CE0 (both landed RELOCATION_MATCH with `p->~T(); FreeVirtual(p);` helper bodies). Only members use it: private
  is consistent.

## Angle 2: bodies v the names, and sizes
- Queue bodies: retail 0x00BDA090 / 0x00BDA0D0 / 0x00BDA110 each test the pointer, read CurrentDisposalListIndex (+0x34), select the
  list sentinel at +0x28 / +0x1C / +0x10 (Index / Vertex / Texture DisposalList, Ego_d member offsets in Ego_r order) and link a
  malloc'd node before it (push_back). That settles IB / VB / Tex for the public trio by body.
- Destroy-now bodies: 0x00BD9C00 / 0x00BD9C20 call the object's vtable slot 0 with flag 0 then FreeVirtual 0x00BD9B90; 0x00BD9CC0 calls
  CTexture::Uninitialise 0x009F9F70 then FreeVirtual. "BlockWhileBusy" (destroy now, waiting on the device) v "Delete" (queue for a
  later frame) matches.
- Sizes: retail destroy-now copies are 0x20 apart, the queue trio 0x40; Ego_r has the BlockWhileBusy trio at 0x30 spacing
  (009c2370 / 009c23a0 / 009c2450) and the public trio at 0x60 (009c25e0 / 009c2640 / 009c26a0): the small bodies are the
  BlockWhileBusy names in both builds.
- Order: retail FreeVirtual 0x00BD9B90 / 0x00BD9C00 / 0x00BD9C20 / AllocVirtualAligned 0x00BD9C40 / FreeVirtualAligned 0x00BD9CA0 /
  0x00BD9CC0 is Ego_r's run FreeVirtual / DeleteVertexBufferBlockWhileBusy / DeleteIndexBufferBlockWhileBusy / AllocVirtualAligned /
  FreeVirtualAligned / DeleteTextureBlockWhileBusy (009c2300 .. 009c2450), in order (NF265).

## Limits
- VB v IB for 0x00BD9C00 v 0x00BD9C20 rests on Ego_r order only (byte-identical bodies). It changes no landed row's bytes or binding
  (no caller, inlined everywhere); it affects only the NAME_FIXES lines and comments.
- 0x00BD9CE0's old Identity line used the Ego_r neighbour DeleteTexture 009C26A0 as the retail neighbour 0x00BD9CC0: in Ego_r the public
  trio sits before FreeDisposalList, in retail the BlockWhileBusy texture copy does. The line is corrected in rows/00bd9ce0.cpp.
