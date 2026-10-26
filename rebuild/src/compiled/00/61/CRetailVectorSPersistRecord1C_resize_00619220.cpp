// CRetailVector<SPersistRecord1C>::resize(unsigned int newSize) -- grows/shrinks a vector of 28-byte {float, CCharString, 5 floats} records, new records default-constructed
// Leaf: ?resize@CRetailVectorSPersistRecord1C@@QAEXI@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00619220, length 103 bytes (+ int3 padding), `ret 4`.
// Identity: the catalog label "CTCKraken::StartHeadEffects" is propagated; the body is STLport's one-argument
// `resize(n) { resize(n, T()); }` (as the landed 0x0056A230): `this` is the vector, the two-argument resize @ 0x00619050
// (manifest "vector<CVectorMap<...>>::resize", a propagated instantiation) takes the count and the default element by address.
// Its twin 0x006193B0 is the same call inlined into CPersistContext_TransferableVector<T>::Resize (vtable 0x01257164 slot 1).
// Callees: CCharString::CCharString(const char*, long) @ 0x0099EBF0 on "" @ 0x0122D70E, ~CCharString @ 0x0099EAE0.
// Shape: the default element is built in the 0x1C-byte frame block: the float zeros are immediate stores (long members would
// share an `xor` register); word 0 zeroed (scheduled before the string ctor), the
// string from "", the five trailing words zeroed after the argument pushes; only the string member is destroyed afterwards.
// The element type's real name is not resolved (named by layout).
class CCharString { public:
    void* m_pData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                   // 0x0099EAE0
};

struct SPersistRecord1C {                             // 28 bytes
    float       m_time;                               // +0x00
    CCharString m_name;                               // +0x04
    float       m_value0;                             // +0x08
    float       m_value1;                             // +0x0C
    float       m_value2;                             // +0x10
    float       m_value3;                             // +0x14
    float       m_value4;                             // +0x18
    SPersistRecord1C() : m_time(0.0f), m_name(""), m_value0(0.0f), m_value1(0.0f), m_value2(0.0f), m_value3(0.0f), m_value4(0.0f) { }
};

struct CRetailVectorSPersistRecord1C {                // std::vector<SPersistRecord1C>: first/last/end
    SPersistRecord1C* first;
    SPersistRecord1C* last;
    SPersistRecord1C* end_of_storage;
    void resize(unsigned int newSize, const SPersistRecord1C& value);   // 0x00619050
    void resize(unsigned int newSize);
};

void CRetailVectorSPersistRecord1C::resize(unsigned int newSize)
{
    resize(newSize, SPersistRecord1C());
}