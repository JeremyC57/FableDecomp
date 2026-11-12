# Chunk Q14 (parked near misses, x87-order, Opus 5.5)

1 of 40 rows matched: 0x00876CA0 NCameraModes::CTools::PotentialFieldBufferObjects, landed in landing cy. The other 39 are in
differ/ (38 DIFFER, 1 SRC_FAIL), each with its residue on line 2. 26 were only regraded because they are local padded models that
need porting to the shared headers first; chunk Q15 therefore ports first. The closest are 0x00B80CE0 GetDeltaTransform
(682v682, PreMultiply sum order) and 0x00987040 (370v374).
