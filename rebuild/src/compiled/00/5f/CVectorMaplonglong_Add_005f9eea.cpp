// CVectorMap<long,long>::Add(const long&, const long&) -- append a {key, value} pair to the map's vector and mark the map dirty
// Leaf: Add
// Retail: Fable.exe (TLC, VC7.1) @ 0x005F9EEA, 42 bytes, `__fastcall`, `ret 8`; ebp frame of a size TU with frame pointers kept
//   (`#pragma optimize("y", off)` + `("s", on)`: `push ebp ; mov ebp,esp ; push ecx ; push ecx` ... `leave`).
// Callee: the 8-byte pair vector's push_back(const pair&) @ 0x005947B6.
// Identity: the propagated label `CPersistTraits<long>::TransferOut` is wrong -- the body is the CVectorMap<K,V>::Add shape
//   of the landed 0x006FCF40 / 0x00AF2910 family: PDB CVectorMap = vector (12 bytes) + empty Compare (+0xC) + Dirty (+0xD).
//   Key and value are both 4-byte dwords; the exact enum/long types of this instantiation are not recoverable from the body
//   (no callers by call/jmp; it sits among the NUISystem::CManager EUsedKeys map helpers).
// Shape: the pair temporary is built in the 8-byte frame (key at [ebp-8], value at [ebp-4]) by an inline pair constructor,
//   its address is pushed before `this` moves to esi; the Dirty store follows the call.
#pragma optimize("y", off)
#pragma optimize("s", on)

struct SLongPair_005f9eea {
    long first;
    long second;
    SLongPair_005f9eea(const long& key, const long& value) : first(key), second(value) { }
};

struct CLongPairVector_005f9eea {
    SLongPair_005f9eea* m_pFirst;
    SLongPair_005f9eea* m_pLast;
    SLongPair_005f9eea* m_pEnd;
    void push_back(const SLongPair_005f9eea& pair);   // 0x005947B6
};

struct CVectorMap_005f9eea {
    CLongPairVector_005f9eea m_vector;               // +0x00
    unsigned char      Compare;                // +0x0C (empty CKeyPairCompareLess)
    bool               Dirty;                  // +0x0D
    void Add(const long& key, const long& value);
};

void CVectorMap_005f9eea::Add(const long& key, const long& value)
{
    m_vector.push_back(SLongPair_005f9eea(key, value));
    Dirty = true;
}