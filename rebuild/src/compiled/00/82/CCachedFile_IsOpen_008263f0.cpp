// Leaf: ?IsOpen@CCachedFile@@QAEEXZ
struct CCachedFile { char pad[0x24]; unsigned char f; unsigned char IsOpen(); };
unsigned char CCachedFile::IsOpen(){ return this->f; }
// Stand-in (owner rule 2026-11-05): the real method on a stand-in class named after the real class (retail ?IsOpen@CCachedFile@@UBE_NXZ); evidence: RTTI vtable slot + PDB name set.
