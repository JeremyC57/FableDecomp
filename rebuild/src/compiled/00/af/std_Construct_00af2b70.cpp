// std::_Construct(std::pair<CCharString,NParticleEngine::CRegisteredPSCInfo>*, const std::pair<CCharString,NParticleEngine::CRegisteredPSCInfo>&) -- placement copy of a registered particle-component entry: key string, alloc function, type, GUI-name string
// Retail: Fable.exe @ 0x00AF2B70, 43 bytes; __fastcall-shaped STLport helper (ecx = element slot, edx = source element).
// Callees: CCharString copy ctor @ 0x0099EC30 (x2).
// Leaf: _Construct
inline void* operator new(unsigned int, void* p) { return p; }   // placement new (STLport <new>)
namespace N00af2b70 {
struct CCharString {
    void* m_pData;                               // ref-counted string body
    CCharString(const CCharString& other);       // 0x0099EC30
};
struct CPSCBase;
typedef CPSCBase* (__cdecl *PSCAllocFunc)();
enum EPSCType { PSC_TYPE_NONE };
struct CRegisteredPSCInfo {                      // NParticleEngine::CRegisteredPSCInfo (PDB, sizeof 0xC)
    PSCAllocFunc AllocFunc;                      // +0x00
    EPSCType     ComponentType;                  // +0x04
    CCharString  GUIName;                        // +0x08
};
struct CNamedPSCInfo {                           // std::pair<CCharString, CRegisteredPSCInfo>
    CCharString        first;                    // +0x00
    CRegisteredPSCInfo second;                   // +0x04
};

// STLport _Construct: placement-copy one element (null-checked placement new, then the implicit copy constructor)
void __fastcall _Construct(CNamedPSCInfo* p, const CNamedPSCInfo& value)
{
    if (p != 0)
        new (p) CNamedPSCInfo(value);
}
}