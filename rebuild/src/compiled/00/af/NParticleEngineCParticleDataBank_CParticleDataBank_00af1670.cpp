// NParticleEngine::CParticleDataBank::CParticleDataBank() -- bank-file and resource-bank bases, empty emitter array, the graphic/mesh symbol maps; resource list reset with a 64 KB budget
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AF1670, length 103 bytes (padded to 112).
// Leaf: ??0CParticleDataBank
// Callees: CBankFile::CBankFile() @ 0x009D5F80, CResourceBank::CResourceBank() @ 0x009FC5F0, CSymbolMap::CSymbolMap()
// @ 0x009E5200 (x2; manifest label "CParticleUpdateParameters" is propagated), CResourceList::SetMaxSize(unsigned long)
// @ 0x009FC520 (stores +0x28 and tail-calls the trim at 0x009FC210).
// Layout: retail CBankFile is 0x164 bytes (PDB 0x174), so the CResourceBank base sits at +0x164 (secondary vtable
// 0x0129F380, 0x7c bytes as in the PDB: ResourceList +4, LoadingQueue +0x40); ParticleEmitters (12-byte CArray)
// +0x1e0, GraphicSymbols +0x1ec, MeshSymbols +0x210 (CSymbolMap 0x24 bytes in retail). Primary vtable 0x0129F38C.
// Shape: two bases (both ctors out of line; the secondary vtable is stored before the primary), the CArray modelled as
// STLport _Vector_base with the _STLP_alloc_proxy end member (puts the `lea ecx` before the third zero store), its inline
// zeroing ctor, then the body: ResourceList's +0x34 word cleared and SetMaxSize(0x10000).
struct CBankFile {
    virtual void v00();
    unsigned char _bankFile[0x160];         // +0x004
    CBankFile();                            // 0x009D5F80
};
struct CResourceList {
    unsigned char _list[0x28];              // +0x00
    unsigned long m_maxSize;                // +0x28
    unsigned char _list2[0x08];             // +0x2c
    unsigned long m_usedSize;               // +0x34
    unsigned char _list3[0x04];             // +0x38
    void SetMaxSize(unsigned long size);    // 0x009FC520
};
struct CResourceBank {
    virtual void v00();
    CResourceList ResourceList;             // +0x04
    CResourceList LoadingQueue;             // +0x40
    CResourceBank();                        // 0x009FC5F0
};
struct CCharString { void* m_pData; };
struct CSymbolMap {
    unsigned char _map[0x24];
    CSymbolMap();                           // 0x009E5200
};
namespace NParticleEngine {
struct CParticleEmitterInfo;
template <class T> struct _STLP_alloc_proxy {   // STLport end-of-storage member with its own inline ctor
    T* _M_data;
    _STLP_alloc_proxy(T* p) : _M_data(p) {}
};
template <class T> struct CArray {          // retail 12 bytes: STLport _Vector_base layout
    T* _M_start;
    T* _M_finish;
    _STLP_alloc_proxy<T> _M_end_of_storage;
    CArray() : _M_start(0), _M_finish(0), _M_end_of_storage(0) {}
};
struct CParticleDataBank : CBankFile, CResourceBank {   // vtables 0x0129F38C / 0x0129F380
    CArray<CParticleEmitterInfo> ParticleEmitters;   // +0x1e0
    CSymbolMap GraphicSymbols;              // +0x1ec
    CSymbolMap MeshSymbols;                 // +0x210
    CParticleDataBank();
    virtual void v01();
};
CParticleDataBank::CParticleDataBank()
{
    ResourceList.m_usedSize = 0;
    ResourceList.SetMaxSize(0x10000);
}
}