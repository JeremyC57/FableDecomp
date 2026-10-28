// C3DMeshStats::HasDummyObject(unsigned long key) -- is any dummy object stored under `key`?
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A2EA60, oracle length 63 bytes.
// Callees: CPreAllocVectorMap<unsigned long,C3DDummyObject>::LowerBound(const ulong&) @ 0x00A31790 (manifest).
// Result type: `bool` const (Ego_r.pdb ?HasDummyObject@C3DMeshStats@@QBE_NK@Z). The bare `setne al` epilogue comes from the
// `if (found) return true; return false;` form; `return a != b;` as a bool materialises `xor edx,edx ; setne dl ; mov al,dl`.
// Same shape as HasHelperPoint @ 0x00A2EA20, on the DummyObjects map at +0x48 whose entries are 56 bytes
// (`imul ecx,ecx,38h` is that stride).
//
// Retail bytes:  push esi ; push edi ; lea esi,[ecx+48h] ; lea eax,[esp+0Ch] ; push eax ; mov ecx,esi ; call LowerBound ; mov ecx,[esi+8] ;
//                mov edx,[esi+4] ; imul ecx,ecx,38h ; add ecx,edx ; cmp eax,ecx ; je E ; mov edx,[esp+0Ch] ; cmp edx,[eax] ; jae K ; E: mov eax,ecx ;
//                K: mov ecx,[esi+8] ; mov edi,[esi+4] ; imul ecx,ecx,38h ; add ecx,edi ; cmp eax,ecx ; pop edi ; setne al ; pop esi ; ret 4
// Retail layout: CPreAllocVectorMap {Compare +0, Elements +4, Size +8, Dirty +0xC}. The map's Find() is
// inlined (lower bound, then end() = Elements + Size when the key sorts before the element) and the result
// is compared with a recomputed end().
class CMatrix3x4 { public: float m[12]; };
class C3DDummyObject { public: CMatrix3x4 Transform; long BoneIndex; };

struct SDummyObjectEntry {                 // pair<unsigned long, C3DDummyObject>, 56 bytes
    unsigned long  first;
    C3DDummyObject second;
};

struct CDummyObjectMap {                   // CPreAllocVectorMap<unsigned long, C3DDummyObject>
    bool               Compare;            // +0  (CKeyPairCompareLess)
    SDummyObjectEntry* Elements;           // +4
    unsigned long      Size;               // +8
    bool               Dirty;              // +0xC
    SDummyObjectEntry* LowerBound(const unsigned long& key) const;   // 0x00A31790
    SDummyObjectEntry* End() const { return Elements + Size; }
    SDummyObjectEntry* Find(const unsigned long& key) const
    {
        SDummyObjectEntry* it = LowerBound(key);
        if (it == End() || key < it->first)
            it = End();
        return it;
    }
};

class C3DMeshStats { public:
    virtual void v00();
    unsigned char   _pad_0x04[0x48 - 0x04];
    CDummyObjectMap DummyObjects;          // +0x48
    bool HasDummyObject(unsigned long key) const;   // ?HasDummyObject@C3DMeshStats@@QBE_NK@Z
};

bool C3DMeshStats::HasDummyObject(unsigned long key) const
{
    if (DummyObjects.Find(key) != DummyObjects.End())
        return true;
    return false;
}