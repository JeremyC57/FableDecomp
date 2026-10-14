// Flags: /EHsc
// CEntryTableOwner_00c48db0::~CEntryTableOwner_00c48db0() -- restore the vtable and `delete[]` the 0x40-byte entry array at +8 (count-prefixed, each entry destroyed through the iterator)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C48DB0, length 44 bytes.
// Callees: `eh vector destructor iterator` @ 0x00BFEED8 (array, element size 0x40, the count stored before the array, the entry destructor @ 0x00C48740), operator
// delete[] @ 0x00BFEB1C (the block starting at the count word). Vtable 0x012B8024 is not in rtti-vtables.tsv; the chunk label "CEngineComponent::~CEngineComponent"
// is propagated. Shape: the plain `??1` of a polymorphic class (the vtable store, no deleting tail): a virtual function plus a NON-virtual destructor;
// the `eh vector destructor iterator` form of `delete[]` needs `/EHsc` (the lane otherwise unrolls the loop inline).
struct SEntry_00c48740 {                           // 0x40 bytes with a destructor @ 0x00C48740
    unsigned char _body[0x40];
    ~SEntry_00c48740();
};

struct CEntryTableOwner_00c48db0 {                 // vtable 0x012B8024
    virtual void v00();
    unsigned char    _pad04[4];
    SEntry_00c48740* m_pEntries;                   // +0x08
    ~CEntryTableOwner_00c48db0();
};

CEntryTableOwner_00c48db0::~CEntryTableOwner_00c48db0()
{
    if (m_pEntries != 0)
        delete[] m_pEntries;
}
