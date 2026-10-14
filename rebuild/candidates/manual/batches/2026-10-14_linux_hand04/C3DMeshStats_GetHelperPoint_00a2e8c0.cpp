// C3DMeshStats::GetHelperPoint(unsigned long key, unsigned long index) -- the index-th helper point stored under `key`, or null
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A2E8C0, oracle length 57 bytes.
// Callees: CPreAllocVectorMap<unsigned long,C3DHelperPoint>::LowerBound(const ulong&) @ 0x00A316B0 (manifest).
// The manifest label "GetDummyObject" is propagated from the byte-equivalent body @ 0x00A2E920 (same code on the
// DummyObjects map at +0x48 with 56-byte entries); this one walks HelperPoints at +0x38 with 20-byte entries
// (pair<ulong, C3DHelperPoint{C3DVector Position; long BoneIndex}>), so it is the helper-point getter.
//
// Retail bytes:  push esi ; lea esi,[ecx+38h] ; lea eax,[esp+8] ; push eax ; mov ecx,esi ; call LowerBound ; mov ecx,[esi+8] ; mov edx,[esi+4] ;
//                lea ecx,[ecx+ecx*4] ; lea ecx,[edx+ecx*4] ; cmp eax,ecx ; pop esi ; je N ; mov edx,[eax] ; cmp edx,[esp+4] ; jne N ;
//                mov ecx,[esp+8] ; lea ecx,[ecx+ecx*4] ; lea eax,[eax+ecx*4+4] ; ret 8 ; N: xor eax,eax ; ret 8
// Retail layout: CPreAllocVectorMap {Compare +0, Elements +4, Size +8, Dirty +0xC} (PDB struct_layouts_egor.tsv).
// The lower bound is checked against end() = Elements + Size and against the exact key (several helper
// points can share a key, hence the index into the run), then the value of entry [bound + index] is returned.
struct C3DVector { float X, Y, Z; };
struct C3DHelperPoint { C3DVector Position; long BoneIndex; };

struct SHelperPointEntry {                 // pair<unsigned long, C3DHelperPoint>, 20 bytes
    unsigned long  first;
    C3DHelperPoint second;
};

struct CHelperPointMap {                   // CPreAllocVectorMap<unsigned long, C3DHelperPoint>
    bool               Compare;            // +0  (CKeyPairCompareLess)
    SHelperPointEntry* Elements;           // +4
    unsigned long      Size;               // +8
    bool               Dirty;              // +0xC
    SHelperPointEntry* LowerBound(const unsigned long& key);   // 0x00A316B0
    SHelperPointEntry* End() { return Elements + Size; }
};

struct C3DMeshStats {
    virtual void v00();
    unsigned char   _pad_0x04[0x38 - 0x04];
    CHelperPointMap HelperPoints;          // +0x38
    C3DHelperPoint* GetHelperPoint(unsigned long key, unsigned long index);
};

C3DHelperPoint* C3DMeshStats::GetHelperPoint(unsigned long key, unsigned long index)
{
    SHelperPointEntry* it = HelperPoints.LowerBound(key);
    if (it != HelperPoints.End() && it->first == key)
        return &it[index].second;
    return 0;
}
