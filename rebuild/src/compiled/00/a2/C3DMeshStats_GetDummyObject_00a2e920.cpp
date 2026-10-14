// C3DMeshStats::GetDummyObject(unsigned long key, unsigned long index) -- the index-th dummy object stored under `key`, or null
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A2E920, oracle length 59 bytes.
// Callees: CPreAllocVectorMap<unsigned long,C3DDummyObject>::LowerBound(const ulong&) @ 0x00A31790 (manifest).
// Same shape as GetHelperPoint @ 0x00A2E8C0, on the DummyObjects map at +0x48 whose entries are 56 bytes
// (pair<ulong, C3DDummyObject{CMatrix3x4 Transform; long BoneIndex}>): `imul ecx,ecx,38h` is that stride.
//
// Retail bytes:  push esi ; lea esi,[ecx+48h] ; lea eax,[esp+8] ; push eax ; mov ecx,esi ; call LowerBound ; mov ecx,[esi+8] ; mov edx,[esi+4] ;
//                imul ecx,ecx,38h ; add ecx,edx ; cmp eax,ecx ; pop esi ; je N ; mov edx,[eax] ; cmp edx,[esp+4] ; jne N ;
//                mov ecx,[esp+8] ; imul ecx,ecx,38h ; lea eax,[ecx+eax+4] ; ret 8 ; N: xor eax,eax ; ret 8
// Retail layout: CPreAllocVectorMap {Compare +0, Elements +4, Size +8, Dirty +0xC} (PDB struct_layouts_egor.tsv).
// The lower bound is checked against end() = Elements + Size and against the exact key (several dummy
// objects can share a key, hence the index into the run), then the value of entry [bound + index] is returned.
struct CMatrix3x4 { float m[12]; };
struct C3DDummyObject { CMatrix3x4 Transform; long BoneIndex; };

struct SDummyObjectEntry {                 // pair<unsigned long, C3DDummyObject>, 56 bytes
    unsigned long  first;
    C3DDummyObject second;
};

struct CDummyObjectMap {                   // CPreAllocVectorMap<unsigned long, C3DDummyObject>
    bool               Compare;            // +0  (CKeyPairCompareLess)
    SDummyObjectEntry* Elements;           // +4
    unsigned long      Size;               // +8
    bool               Dirty;              // +0xC
    SDummyObjectEntry* LowerBound(const unsigned long& key);   // 0x00A31790
    SDummyObjectEntry* End() { return Elements + Size; }
};

struct C3DMeshStats {
    virtual void v00();
    unsigned char   _pad_0x04[0x48 - 0x04];
    CDummyObjectMap DummyObjects;          // +0x48
    C3DDummyObject* GetDummyObject(unsigned long key, unsigned long index);
};

C3DDummyObject* C3DMeshStats::GetDummyObject(unsigned long key, unsigned long index)
{
    SDummyObjectEntry* it = DummyObjects.LowerBound(key);
    if (it != DummyObjects.End() && it->first == key)
        return &it[index].second;
    return 0;
}