// _global::CopyBackward_PairCharStringMeshVolume_00a70ab0(std::pair<CCharString,C3DMeshVolume>*, std::pair<CCharString,C3DMeshVolume>*, std::pair<CCharString,C3DMeshVolume>*, const random_access_iterator_tag&, int*) -- SGI __copy_backward over 16-byte (CCharString, C3DMeshVolume) pairs
// Leaf: CopyBackward_PairCharStringMeshVolume_00a70ab0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A70AB0, length 152 bytes.
// SGI-STL `__copy_backward(first, last, result, random_access_iterator_tag, Distance*)`, /Gr TU (__fastcall: first in
// ecx, last in edx, result + the tag reference + the distance pointer on the stack, `ret 0Ch`); count = (last - first)
// >> 4, kept in a frame word; the result pointer lives in its argument slot (rewritten each step and returned from it).
// Element: std::pair<CCharString,C3DMeshVolume> (the manifest label), 16 bytes = the CCharString key + Ego_r.pdb C3DMeshVolume (CCharString Name +0,
// CCountedPointer<CContainmentVolume> ConvexVolume +4). The implicit operator= copies the key, calls the out-of-line
// CCharString::operator= @ 0x0099EFB0 for the name, then runs the inlined CCountedPointer::operator= (source fields
// read first; when the count blocks differ release the old block -- destroy function at +4 with the object, then
// operator delete @ 0x00BFE9BC -- adopt both words and add a reference), modelled as the landed 0x0084F130.
class CCharString { public:
    void* m_pData;                           // +0x00
    const CCharString& operator=(const CCharString& other);     // 0x0099EFB0
};

struct SCountBlock {
    long m_references;                       // +0
    void (__fastcall* m_pfnDestroy)(void*);  // +4
    void* m_pObject;                         // +8
};

struct CCountedPointer {                     // CCountedPointer<CContainmentVolume>, 8 bytes
    void*        m_pObject;                  // +0
    SCountBlock* m_pCount;                   // +4
    CCountedPointer& operator=(const CCountedPointer& other)
    {
        SCountBlock* count = other.m_pCount;
        void* object = other.m_pObject;
        if (m_pCount != count) {
            if (m_pCount) {
                --m_pCount->m_references;
                if (m_pCount->m_references == 0) {
                    m_pCount->m_pfnDestroy(m_pCount->m_pObject);
                    delete m_pCount;
                }
            }
            m_pObject = object;
            m_pCount = count;
            if (count)
                ++count->m_references;
        }
        return *this;
    }
};

class C3DMeshVolume { public:                       // Ego_r.pdb, 12 bytes
    CCharString     Name;                    // +0x00
    CCountedPointer ConvexVolume;            // +0x04 CCountedPointer<CContainmentVolume>
};

struct PairCharStringMeshVolume {                // std::pair<CCharString,C3DMeshVolume>, 16 bytes
    CCharString first;                 // +0x00
    C3DMeshVolume second;              // +0x04
};

struct random_access_iterator_tag { random_access_iterator_tag() { } };

PairCharStringMeshVolume* __fastcall CopyBackward_PairCharStringMeshVolume_00a70ab0(PairCharStringMeshVolume* first, PairCharStringMeshVolume* last, PairCharStringMeshVolume* result, const random_access_iterator_tag& tag, int* distance)
{
    for (int n = last - first; n > 0; --n)
        *--result = *--last;
    return result;
}