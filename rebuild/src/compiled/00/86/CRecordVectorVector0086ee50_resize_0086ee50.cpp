// CRecordVectorVector_0086ee50::resize(unsigned int newSize) -- STLport one-argument resize of a vector of vectors: resize(newSize, value_type()) with an empty inner vector built and destroyed in the frame
// Leaf: resize
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0086EE50, length 44 bytes, `ret 4`; no references (an unreferenced
// instantiation). The two-argument resize it forwards to, 0x0086EDE0 (manifest "vector<CTCInventoryMap::CUIMiniMapMarker>
// ::resize", propagated), strides 12 bytes (`imul 2AAAAAABh ; sar edx,1`) = the inner vector {start, finish, end}; the
// inner vector's destructor 0x006755A0 (manifest "CScriptInfo_DestroyArray", propagated) walks 0x18-byte records freeing
// the block at +8 of each, then frees its storage. The element types are not recoverable (descriptive names); the manifest
// label (_Dest_val<allocator<pair<CCharString,CScriptInfo>>>) is propagated and wrong.
// Shape: the landed resize(n) form (0x0056A230): the default inner vector is three zero words stored from one `xor eax,eax`
// (count loaded into edx first), passed by address with the count, `this` left in ecx, destroyed after the call.
struct SRecord18_0086ee50 {                                  // 0x18 bytes, owns a malloc'd block at +8
    long  m_value00;
    long  m_value04;
    void* m_pBlock;                                          // +0x08
    long  m_value0c;
    long  m_value10;
    long  m_value14;
};

struct CRecordVector_0086ee50 {                              // std::vector<SRecord18_0086ee50>: start/finish/end
    SRecord18_0086ee50* _M_start;
    SRecord18_0086ee50* _M_finish;
    SRecord18_0086ee50* _M_end_of_storage;
    CRecordVector_0086ee50() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    ~CRecordVector_0086ee50();                               // 0x006755A0
};

struct CRecordVectorVector_0086ee50 {                        // std::vector<CRecordVector_0086ee50>
    CRecordVector_0086ee50* _M_start;
    CRecordVector_0086ee50* _M_finish;
    CRecordVector_0086ee50* _M_end_of_storage;
    void resize(unsigned int newSize, const CRecordVector_0086ee50& value);   // 0x0086EDE0
    void resize(unsigned int newSize);
};

void CRecordVectorVector_0086ee50::resize(unsigned int newSize)
{
    resize(newSize, CRecordVector_0086ee50());
}