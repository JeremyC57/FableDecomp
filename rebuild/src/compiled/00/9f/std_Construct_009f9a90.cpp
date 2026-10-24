// std::_Construct(CProfileManager::CProfileTreeEntry*, const CProfileManager::CProfileTreeEntry&) -- placement copy of a profile-tree node: the 0x48-byte CProfile (out-of-line copy), id, children vector and parent pointer
// Retail: Fable.exe @ 0x009F9A90, 43 bytes; __fastcall-shaped STLport helper (ecx = element slot, edx = source element).
// Callees: CProfile copy ctor @ 0x009F9450, vector copy ctor @ 0x009F9930.
// Leaf: _Construct
inline void* operator new(unsigned int, void* p) { return p; }   // placement new (STLport <new>)
namespace N009f9a90 {
struct CProfile {
    unsigned char _body[0x48];
    CProfile(const CProfile& other);             // 0x009F9450
};
struct CChildVector {                                 // retail 12-byte STLport-vector body (void*)
    void** _M_start;
    void** _M_finish;
    void** _M_end_of_storage;
    CChildVector(const CChildVector& other);                  // 0x009F9930
};
struct CProfileTreeEntry {                       // CProfileManager::CProfileTreeEntry (retail sizeof 0x5C)
    CProfile           Profile;                  // +0x00
    long               ID;                       // +0x48
    CChildVector       Children;                 // +0x4C vector<CProfileTreeEntry*>
    CProfileTreeEntry* Parent;                   // +0x58
};

// STLport _Construct: placement-copy one element (null-checked placement new, then the implicit copy constructor)
void __fastcall _Construct(CProfileTreeEntry* p, const CProfileTreeEntry& value)
{
    if (p != 0)
        new (p) CProfileTreeEntry(value);
}
}