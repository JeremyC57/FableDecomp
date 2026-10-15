// C3DMeshVolume::C3DMeshVolume(const C3DMeshFileVolumeChunk&, CMovableResourceMemoryPool*) -- copies the chunk's volume name and builds a new containment volume from the chunk's planes
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A2E9A0, length 126 bytes.
// Leaf: ??0C3DMeshVolume
// Callees: 0x0099E4B0 CCharString(), 0x0099EFB0 CCharString::operator=, 0x00BFEA1A operator new(0x10),
// 0x00A752D0 CContainmentVolume() (propagated label C3DGroup2), 0x00A31640 CCountedPointer<T>::Reset(T*),
// 0x00A755C0 CContainmentVolume::Initialise(const CPlane*, unsigned long, CMovableResourceMemoryPool*).
// Layout: C3DMeshVolume (PDB, 12 bytes; landed default ctor 0x00A0A3D0): Name +0, ConvexVolume +4
// (CCountedPointer<CContainmentVolume>: object +4, count block +8). The volume chunk keeps its name at +0x2c
// and its planes (retail three-pointer CArray of 16-byte CPlane) at +0x30.
// Shape: member constructors (the string's out-of-line default ctor, the counted pointer's inline zeroing),
// then body assignments; the counted pointer is addressed once (`lea esi,[edi+4]`) and reused for Reset and
// the object fetch. The plane data goes through the CArray GetData() idiom (`Size() > 0 ? first : this`, `ja`).
struct CCharString {
    char* m_pString;
    CCharString();                                       // 0x0099E4B0
    CCharString& operator=(const CCharString& other);    // 0x0099EFB0
};

struct CPlane { float A, B, C, D; };                     // 16 bytes
struct CMovableResourceMemoryPool;
struct CContainmentVolume {
    CContainmentVolume();                                // 0x00A752D0
    bool Initialise(const CPlane* planes, unsigned long count, CMovableResourceMemoryPool* pool);   // 0x00A755C0
    unsigned char _body[0x10];
};

template<class T> struct CCountedPointer {
    T*    m_pObject;
    void* m_pCount;
    CCountedPointer() : m_pObject(0), m_pCount(0) { }
    void Reset(T* object);                               // 0x00A31640
    T* operator->() const { return m_pObject; }
};

template<class T> struct CArray {                        // retail CArray<T>: begin/end/capacity
    T* m_begin; T* m_end; T* m_capacity;
    unsigned long Size() const { return (unsigned long)(m_end - m_begin); }
    const T* GetData() const { return Size() > 0 ? m_begin : (const T*)this; }
};

struct C3DMeshFileVolumeChunk {
    unsigned char  _pad00[0x2c];
    CCharString    Name;                                 // +0x2c
    CArray<CPlane> Planes;                               // +0x30
};

struct C3DMeshVolume {
    CCharString                         Name;            // +0x00
    CCountedPointer<CContainmentVolume> ConvexVolume;    // +0x04
    C3DMeshVolume(const C3DMeshFileVolumeChunk& chunk, CMovableResourceMemoryPool* pool);
};

C3DMeshVolume::C3DMeshVolume(const C3DMeshFileVolumeChunk& chunk, CMovableResourceMemoryPool* pool)
{
    Name = chunk.Name;
    ConvexVolume.Reset(new CContainmentVolume);
    ConvexVolume->Initialise(chunk.Planes.GetData(), chunk.Planes.Size(), pool);
}
