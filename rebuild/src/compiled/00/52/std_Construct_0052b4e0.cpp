// std::_Construct(NThingManager::CEntityRuntimePersistence::CEntityScriptInfo*, const NThingManager::CEntityRuntimePersistence::CEntityScriptInfo&) -- placement copy of an entity script record: script string, save-data byte array, uncompressed size, valid flag
// Retail: Fable.exe @ 0x0052B4E0, 43 bytes; __fastcall-shaped STLport helper (ecx = element slot, edx = source element).
// Callees: CCharString copy ctor @ 0x0099EC30, CArray<unsigned char> copy ctor @ 0x005290E0.
// Leaf: _Construct
inline void* operator new(unsigned int, void* p) { return p; }   // placement new (STLport <new>)
namespace N0052b4e0 {
struct CCharString {
    void* m_pData;                               // ref-counted string body
    CCharString(const CCharString& other);       // 0x0099EC30
};
struct CArrayByte {                                 // retail 12-byte STLport-vector body (unsigned char)
    unsigned char* _M_start;
    unsigned char* _M_finish;
    unsigned char* _M_end_of_storage;
    CArrayByte(const CArrayByte& other);                  // 0x005290E0
};
struct CEntityScriptInfo {                       // retail sizeof 0x18 (PDB 0x1C with a 16-byte CArray)
    CCharString Script;                          // +0x00
    CArrayByte  SaveData;                        // +0x04 CArray<unsigned char>
    long        UncompressedSize;                // +0x10
    bool        Valid;                           // +0x14
};

// STLport _Construct: placement-copy one element (null-checked placement new, then the implicit copy constructor)
void __fastcall _Construct(CEntityScriptInfo* p, const CEntityScriptInfo& value)
{
    if (p != 0)
        new (p) CEntityScriptInfo(value);
}
}