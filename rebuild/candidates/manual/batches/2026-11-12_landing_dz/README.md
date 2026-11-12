# 2026-11-12 landing dz -- chunk Q29: 2 small parked rows matched

Source: `pending/2026-11-12_linux_handQ29/`. Landed (RELOCATION_MATCH, test PASS under VC7.1, LANDVERIFY_STRICT_HEADERS=1):
0x007CC710 CTCMultiArrow::ReleaseShot, 0x009D6350 CBankFileAsyncData::Abort.
Headers (the three the rows use; additions only, hdrcheck2 default=same, macros new): CTCMultiArrow.h RELEASESHOT,
CBankFileAsyncData.h ABORT, CMovableResourceMemoryPool.h FREERESOURCE.
Checks: reserved_check 0; bindings_multi_check at baseline; funclet_includes_check 1317 checked, 0 broken.
Queued (land1011dz): 0x0098EC90 is CThreadedFile::RemoveObject (landed ~CBankFileAsyncData 0x009D61F0 calls it "HasReadFinished");
0x00A62ED0 is CMovableResourceMemoryPool::FreeResource (landed 0x009D61F0 / 0x009D54A0 call it "Free"); 0x00EB3BA0's manifest label.
Under the no-switching rule the agent stayed on its third row (44 grades, one byte off) and never opened five rows; chunk Q32
continues.
