// CPersistContext_TransferableVector<SPersistRecord1C>::Resize(long count) -- resizes the transferred vector of 28-byte {float, CCharString, 5 floats} records, new records default-constructed
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006193B0, length 103 bytes (+ int3 padding), `ret 4`. Vtable 0x01257164 slot 1
// (Ego_r.pdb ?Resize@?$CPersistContext_TransferableVector@...@@EAEXJ@Z family; catalog label "CTCKraken::StartHeadEffects"
// is propagated from the byte twin 0x00619220).
// Callees: the vector's two-argument resize(n, const T&) @ 0x00619050, CCharString::CCharString(const char*, long) @ 0x0099EBF0
// on "" @ 0x0122D70E, ~CCharString @ 0x0099EAE0.
// Shape: the two-argument resize called directly with a default element temporary, so the TransVector load (+0x04) comes after
// the argument pushes (an inlined one-argument resize(n) reads it first);
// the default element's float zeros are immediate stores; only its string member is destroyed afterwards.
// Leaf: Resize
struct CCharString {
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
};

struct CPersistContext;
struct CStringParser;
struct CPersistContext_TransferableVector {
    virtual long GetCount();
    virtual void Resize(long count);
    virtual void TransferOut(CPersistContext* context, CCharString* out, long index);
    virtual void TransferIn(CPersistContext* context, CStringParser* parser, long index);
    CRetailVectorSPersistRecord1C* TransVector;       // +0x04
};

void CPersistContext_TransferableVector::Resize(long count)
{
    TransVector->resize(count, SPersistRecord1C());
}