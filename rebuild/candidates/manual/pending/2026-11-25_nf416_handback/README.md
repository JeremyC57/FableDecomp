# NF416 hand-back (2026-11-25): NInventory::CItem ??_G copies and the real CChunkedFile destructor

5 rows ACCEPT, landed as `2026-11-24_landing_nf416_b1..b3` (RELOCATION_MATCH PASS); 0x00621A20 HOLD (T3 `nf416 0x00621A20 debt`).
- b1 0x005BF91A, 0x005BF969: FABLE_RETAIL_NINVENTORY_CITEM_TRANSFERIN plus the /Gr destroy opt-ins (incl. the new add-only
  FABLE_RETAIL_STLP_DESTROY_DISPATCH_FORCEINLINE_FASTCALL in stlport/fable_containers.h; retail keeps the unreferenced fastcall
  copies 0x005C1937 / 0x005C1996 / 0x005C149E); 0x005BF969 drops its `#pragma optimize("y", off)`. Every bound copy EQ.
- b2 0x005C1ECC: pragma dropped, implicit ~CItem, __fastcall _Destroy(T*): its ??_GCItem / erase / __copy copies EQ.
- b3 lib_chunked_file.obj as one Option S unit (owner 0x00AA7DB0 ctor; 0x00AA7DF0 = the real ??1CChunkedFile@@UAE@XZ, a 74-byte
  body, both PDBs; 0x00AA9190 its ??_G). 3 self lines applied.
pk_all / pickany_vs_retail: 0 regressions, 10 copies NEQ -> EQ, 2 NOADDR -> EQ. Census /FORCE link: duplicates 16 = 16.
The wrapper's head/payload.json was rebuilt by the coordinator from the repo (recheck) after checking orig/ rows and tests equal it.
