// std::vector<std::pair<unsigned long, C3DMeshVolume> >::resize(unsigned int newSize) -- STLport one-argument resize: resize(newSize, value_type()) with the default (key 0, empty name, null volume) entry built and destroyed in the frame
// Leaf: resize
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A35580, 107 bytes (`ret 4`).
// Identity: the manifest label "CThingCreatureBase::GetAnimsWithNameStartingWith" is propagated. The only caller
//   (0x00A2F629, in C3DMeshStats::LoadBinary) passes the vector at C3DMeshStats+0x58 = MeshVolumes (Ego_r.pdb
//   CVectorMap<unsigned long, C3DMeshVolume>), and the callee 0x00A35430 (manifest "vector<unsigned int>::resize",
//   propagated) is resize(n, const T&) over 16-byte elements (`sar 4`): pair<unsigned long, C3DMeshVolume>.
// Callees: CCharString::CCharString() @ 0x0099E4B0, resize(unsigned int, const value_type&) @ 0x00A35430, the volume
//   count block's destroy function (+4, object +8), operator delete @ 0x00BFE9BC, CCharString::~CCharString @ 0x0099EAE0.
// Layout: pair.first +0x00; C3DMeshVolume (PDB 12 bytes) at +0x04: Name (CCharString) +0x00, ConvexVolume
//   (CCountedPointer<CContainmentVolume>: object, count block) +0x04/+0x08.
// Shape: the landed 0x0056A230 resize(n) form; one zero register (edi) for the key, the pointer words and the clears;
//   this TU's counted-pointer dtor clears both words after the release.
void __cdecl operator delete(void* block);              // 0x00BFE9BC

class CCharString { public:
    void* m_pData;
    CCharString();                              // 0x0099E4B0
    ~CCharString();                             // 0x0099EAE0
};
struct SCountBlock {
    long m_references;                          // +0
    void (__fastcall* m_pfnDestroy)(void*);     // +4 per-type deleter, object in ecx
    void* m_pObject;                            // +8
};
template <class T> class CCountedPointer { public:
    T* Data;
    SCountBlock* m_pCount;
    CCountedPointer() : Data(0), m_pCount(0) { }
    ~CCountedPointer()
    {
        if (m_pCount && --m_pCount->m_references == 0) {
            m_pCount->m_pfnDestroy(m_pCount->m_pObject);
            operator delete(m_pCount);
        }
        Data = 0;
        m_pCount = 0;
    }
};
class CContainmentVolume;
class C3DMeshVolume { public:                          // 12 bytes
    CCharString Name;                           // +0x00
    CCountedPointer<CContainmentVolume> ConvexVolume;   // +0x04
};
struct SMeshVolumePair {                        // std::pair<unsigned long, C3DMeshVolume>
    unsigned long first;
    C3DMeshVolume second;
    SMeshVolumePair() : first(0), second() { }
};

struct CMeshVolumeVector {                      // std::vector<pair<unsigned long, C3DMeshVolume> >: first/last/end
    SMeshVolumePair* first;
    SMeshVolumePair* last;
    SMeshVolumePair* end_of_storage;
    void resize(unsigned int newSize, const SMeshVolumePair& value);    // 0x00A35430
    void resize(unsigned int newSize);
};

void CMeshVolumeVector::resize(unsigned int newSize)
{
    resize(newSize, SMeshVolumePair());
}