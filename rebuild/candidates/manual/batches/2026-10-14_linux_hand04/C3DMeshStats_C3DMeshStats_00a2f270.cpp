// C3DMeshStats::C3DMeshStats(bool animated) -- vtable, empty name, zero bounding sphere, empty containers, AnimatedFlag from the argument
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A2F270, oracle length 81 bytes.
// Callees: CCharString::CCharString @ 0x0099E4B0 (landed). Vtable 0x0129CF04 = C3DMeshStats (rtti-vtables.tsv).
//
// Retail bytes:  push esi ; mov esi,ecx ; lea ecx,[esi+0Ch] ; mov [esi],offset vftable ; call CCharString::CCharString ; xor eax,eax ;
//                mov [esi+10h..1Ch],eax (x4) ; mov [esi+3Ch],eax ; mov [esi+40h],eax ; mov [esi+44h],al ; mov [esi+4Ch],eax ; mov [esi+50h],eax ;
//                mov [esi+54h],al ; mov [esi+58h..60h],eax (x3) ; mov [esi+65h],al ; mov [esi+68h..70h],eax (x3) ; mov al,[esp+8] ;
//                mov [esi+7Ah],al ; mov eax,esi ; pop esi ; ret 4
// Same retail layout as the default constructor @ 0x00A2F210 (see that file for the offset table); this
// overload leaves MemoryPool, StatsMemoryHandle, PackedNames and PackedNamesSize uninitialised and only
// sets AnimatedFlag (+0x7A) from its bool argument. Every container has an inline constructor that
// nulls its pointers and clears its flag.
struct C3DVector { float X, Y, Z; };
struct C3DBoundingSphere {
    C3DVector Center;
    float     Radius;
    C3DBoundingSphere() { Center.X = 0.0f; Center.Y = 0.0f; Center.Z = 0.0f; Radius = 0.0f; }
};
struct C3DBoundingBox { C3DVector Min, Max; };

struct CCharString {
    char* m_pString;
    CCharString();                         // 0x0099E4B0
};

struct CKeyPairCompareLess { };

template <class T> struct CPreAllocVectorMap {   // retail: 16 bytes
    CKeyPairCompareLess Compare;           // +0
    T*                  Elements;          // +4
    unsigned long       Size;              // +8
    bool                Dirty;             // +0xC
    CPreAllocVectorMap() : Elements(0), Size(0), Dirty(false) { }
};

template <class T> struct CVectorMap {     // retail: 16 bytes
    T* m_pFirst;                           // +0
    T* m_pLast;                            // +4
    T* m_pEnd;                             // +8
    CKeyPairCompareLess Compare;           // +0xC
    bool Dirty;                            // +0xD
    CVectorMap() : m_pFirst(0), m_pLast(0), m_pEnd(0), Dirty(false) { }
};

template <class T> struct CArray {         // retail: 12 bytes
    T* m_pFirst;
    T* m_pLast;
    T* m_pEnd;
    CArray() : m_pFirst(0), m_pLast(0), m_pEnd(0) { }
};

struct CMovableResourceMemoryPool;
struct C3DHelperPointPair;                 // pair<ulong, C3DHelperPoint>
struct C3DDummyObjectPair;                 // pair<ulong, C3DDummyObject>
struct C3DMeshVolumePair;                  // pair<ulong, C3DMeshVolume>
struct C3DGeneratorObject;

struct C3DMeshStats {
    virtual void v00();                    // vtable 0x0129CF04
    CMovableResourceMemoryPool*          MemoryPool;         // +0x04 (not initialised here)
    long                                 StatsMemoryHandle;  // +0x08 (not initialised here)
    CCharString                          Name;               // +0x0C
    C3DBoundingSphere                    BoundingSphere;     // +0x10
    C3DBoundingBox                       BoundingBox;        // +0x20
    CPreAllocVectorMap<C3DHelperPointPair> HelperPoints;     // +0x38
    CPreAllocVectorMap<C3DDummyObjectPair> DummyObjects;     // +0x48
    CVectorMap<C3DMeshVolumePair>        MeshVolumes;        // +0x58
    CArray<C3DGeneratorObject>           MeshGenerators;     // +0x68
    char*                                PackedNames;        // +0x74 (not initialised here)
    unsigned short                       PackedNamesSize;    // +0x78 (not initialised here)
    bool                                 AnimatedFlag;       // +0x7A
    C3DMeshStats(bool animated);
};

C3DMeshStats::C3DMeshStats(bool animated)
    : Name(), BoundingSphere(), HelperPoints(), DummyObjects(), MeshVolumes(), MeshGenerators(), AnimatedFlag(animated)
{
}
