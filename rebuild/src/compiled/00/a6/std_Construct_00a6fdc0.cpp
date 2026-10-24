// std::_Construct(std::pair<CCharString,C3DMeshVolume>*, const std::pair<CCharString,C3DMeshVolume>&) -- placement copy of a named mesh volume: key string, then the volume's name string and counted convex-volume pointer
// Retail: Fable.exe @ 0x00A6FDC0, 51 bytes; __fastcall-shaped STLport helper (ecx = element slot, edx = source element).
// Callees: CCharString copy ctor @ 0x0099EC30 (x2).
// Leaf: _Construct
inline void* operator new(unsigned int, void* p) { return p; }   // placement new (STLport <new>)
namespace N00a6fdc0 {
struct CCharString {
    void* m_pData;                               // ref-counted string body
    CCharString(const CCharString& other);       // 0x0099EC30
};
struct CCPPointerInfo { long RefCount; };
template <class T> struct CCountedPointer {      // shared pointer: object + shared count block
    T*              Data;
    CCPPointerInfo* Info;
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info) { if (Info) ++Info->RefCount; }
};
struct CContainmentVolume;
struct C3DMeshVolume {                           // PDB sizeof 0xC
    CCharString                         Name;          // +0x00
    CCountedPointer<CContainmentVolume> ConvexVolume;  // +0x04
};
struct CNamedMeshVolume {                        // std::pair<CCharString, C3DMeshVolume>
    CCharString   first;                         // +0x00
    C3DMeshVolume second;                        // +0x04
};

// STLport _Construct: placement-copy one element (null-checked placement new, then the implicit copy constructor)
void __fastcall _Construct(CNamedMeshVolume* p, const CNamedMeshVolume& value)
{
    if (p != 0)
        new (p) CNamedMeshVolume(value);
}
}