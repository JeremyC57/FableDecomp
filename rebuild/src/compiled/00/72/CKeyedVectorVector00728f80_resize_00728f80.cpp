// CKeyedVectorVector_00728f80::resize(unsigned int newSize) -- STLport one-argument resize of a vector of {key, vector} records: resize(newSize, value_type()) with a default record (key left unset, empty vector) built and destroyed in the frame
// Leaf: resize
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00728F80, length 46 bytes, `ret 4`; no references (an unreferenced
// instantiation in the CTCCreatureExpression neighbourhood). The two-argument resize 0x00728F20 (manifest
// "vector<unsigned int>::resize", propagated) strides 16 bytes (`sar edi,4`); the record's only destructor call is on its
// +4 member, 0x005BC0D3 (manifest "CActiveFile::OnReadFinished", propagated) = a vector destructor (_Destroy of the range
// through a tag, then free(start)). The element types are not recoverable (descriptive names); the manifest label
// (_Dest_val<allocator<pair<CCharString,CScriptInfo>>>) is propagated and wrong.
// Shape: the landed resize(n) form (0x0056A230): the record's user default constructor leaves the key word alone and
// zeroes the inner vector's three words from one `xor eax,eax`; passed by address with the count, `this` left in ecx; the
// record's inline destructor is the inner vector's out-of-line one.
struct CItemVector_00728f80 {                                // std::vector<...>: start/finish/end
    void* _M_start;
    void* _M_finish;
    void* _M_end_of_storage;
    CItemVector_00728f80() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    ~CItemVector_00728f80();                                 // 0x005BC0D3
};

struct SKeyedItems_00728f80 {                                // 0x10 bytes
    long                 m_key;                              // +0x00 (not initialised by the default ctor)
    CItemVector_00728f80 m_items;                            // +0x04
    SKeyedItems_00728f80() { }
};

struct CKeyedVectorVector_00728f80 {                         // std::vector<SKeyedItems_00728f80>
    SKeyedItems_00728f80* _M_start;
    SKeyedItems_00728f80* _M_finish;
    SKeyedItems_00728f80* _M_end_of_storage;
    void resize(unsigned int newSize, const SKeyedItems_00728f80& value);    // 0x00728F20
    void resize(unsigned int newSize);
};

void CKeyedVectorVector_00728f80::resize(unsigned int newSize)
{
    resize(newSize, SKeyedItems_00728f80());
}