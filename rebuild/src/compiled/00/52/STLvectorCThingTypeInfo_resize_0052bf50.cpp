// std::vector<CThingTypeInfo>::resize(size_type n) -- STLport one-argument resize: resize(n, CThingTypeInfo()) with a default-constructed temporary
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0052BF50, length 48 bytes.
// Leaf: ?resize@?$vector@VCThingTypeInfo@@V?$allocator@VCThingTypeInfo@@@std@@@std@@QAEXI@Z
// Callees: CCharString::CCharString() @ 0x0099E4B0, vector<CThingTypeInfo>::resize(size_type, const CThingTypeInfo&)
// @ 0x0052BEA0 (thiscall, `ret 8`; the element stride is 12: the 0x2AAAAAAB/`sar 1` divide), CCharString::~CCharString()
// @ 0x0099EAE0.
// Label correction: the manifest's "CPersistContext_TransferableComponent<CIntelligentPointer<CThing>>::TransferIn" is
// propagated -- this takes one argument (`ret 4`) and has no persist context. The two-argument resize 0x0052BEA0 is
// also called from CThingManager::InitThingTypes (0x00522A3D: `resize(16, CThingTypeInfo())` on the vector at +0x3c,
// the PDB's vector<CThingTypeInfo> ThingTypeInfo). Ego_r.pdb CThingTypeInfo (12 bytes): long DefType, unsigned long
// ThingTypeFlags, CCharString DefClassType (+8); FableWin.pdb has ?resize@?$vector@VCThingTypeInfo@@...@QAEXI@Z and
// ??0CThingTypeInfo@@QAE@XZ, a default constructor that only constructs the string.
// Shape: the 12-byte temporary in the frame (`sub esp,0xc`), its string constructed at +8, the call, the string
// destroyed (the implicit ~CThingTypeInfo).
class CCharString { public:
    void* PStringData;
    CCharString();                                  // 0x0099E4B0
    ~CCharString();                                 // 0x0099EAE0
};

class CThingTypeInfo {
public:
    long          DefType;                          // +0x00
    unsigned long ThingTypeFlags;                   // +0x04
    CCharString   DefClassType;                     // +0x08
    CThingTypeInfo() {}
};

namespace std {
template <class T> class allocator { };

template <class T, class Alloc = allocator<T> >
class vector {
public:
    typedef unsigned int size_type;
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    void resize(size_type new_size, const T& x);    // 0x0052BEA0 for CThingTypeInfo
    void resize(size_type new_size) { resize(new_size, T()); }
};

template void vector<CThingTypeInfo>::resize(size_type);
}