// CKeyedArrayRecord::`scalar_deleting_destructor'(unsigned int) -- free the array at +8, then the record if asked
// Leaf: ??_GCKeyedArrayRecord
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005BBBCE, length 37 bytes (a /Os module: no alignment
// padding, `pop ecx` argument cleanup).
// Callees: free @ 0x00BFEA14 (the array buffer), operator delete @ 0x00BFE9BC.
// Identity: the manifest label "CSystemRegistry::~CSystemRegistry" is propagated and wrong. The callers
// are a `_Destroy_range` over 0x14-byte elements (0x005BBF80: `push 0 ; call this` per element) and a
// one-element destroy (0x005BBDE9), so this is the deleting destructor of a 20-byte vector element whose
// third..fifth words are a retail CArray (three pointers) freed with free(); the owning class is not
// established, so the record is modelled descriptively (two leading words + the array).
//
// Shape: no vtable store -> a NON-polymorphic class; VC7.1 still emits the `??_G` for it in a TU that
// `delete`s it (the `Destroy` helper below), with the member CArray's inline destructor
// (`if (first) free(first)`) inlined into it. The module is size-optimised.
#pragma optimize("s", on)

extern "C" void __cdecl free(void* block);       // 0x00BFEA14

template<class T>
class CArray { public:
    T* m_first;
    T* m_last;
    T* m_end;
    ~CArray() { if (m_first) free(m_first); }
};

struct CKeyedArrayRecord {
    long         m_key;                          // +0x00
    long         m_value;                        // +0x04
    CArray<long> m_items;                        // +0x08 .. +0x13
};

void DestroyKeyedArrayRecord(CKeyedArrayRecord* record)
{
    delete record;
}