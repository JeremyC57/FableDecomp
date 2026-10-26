// C3DMeshStats::C3DMeshStats() -- vtable, no memory pool, handle -1, empty name, zero bounding sphere, empty containers and packed-name fields
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A2F210, oracle length 96 bytes.
// Callees: CCharString::CCharString @ 0x0099E4B0 (landed). Vtable 0x0129CF04 = C3DMeshStats (rtti-vtables.tsv).
//
// Retail bytes:  push ebx ; push esi ; mov esi,ecx ; xor ebx,ebx ; lea ecx,[esi+0Ch] ; mov [esi],offset vftable ; mov [esi+4],ebx ;
//                mov dword ptr [esi+8],-1 ; call CCharString::CCharString ; mov [esi+10h..1Ch],ebx (x4) ; mov [esi+3Ch],ebx ;
//                mov [esi+40h],ebx ; mov [esi+44h],bl ; mov [esi+4Ch],ebx ; mov [esi+50h],ebx ; mov [esi+54h],bl ; mov [esi+58h..60h],ebx (x3) ;
//                mov [esi+65h],bl ; mov [esi+68h..74h],ebx (x4) ; mov [esi+78h],bx ; mov [esi+7Ah],bl ; mov eax,esi ; pop esi ; pop ebx ; ret
// Retail layout (the PDB layout in struct_layouts_egor.tsv is the debug build's with 16-byte CArray and
// 20-byte CVectorMap; retail containers are 4 bytes smaller, so everything after +0x38 sits 4..8 bytes
// lower -- RelocateData @ 0x00A2E820 and GetDummyObject @ 0x00A2E920 confirm these offsets):
//   +0x04 MemoryPool (null)           +0x08 StatsMemoryHandle (-1)        +0x0C Name (CCharString)
//   +0x10 BoundingSphere {Center, Radius} zeroed (four immediate-0 float stores from the cleared register)
//   +0x20 BoundingBox (not initialised)
//   +0x38 HelperPoints  CPreAllocVectorMap<ulong,C3DHelperPoint>  {Compare +0, Elements +4, Size +8, Dirty +0xC}
//   +0x48 DummyObjects  CPreAllocVectorMap<ulong,C3DDummyObject>  (same shape)
//   +0x58 MeshVolumes   CVectorMap<ulong,C3DMeshVolume>           {vector first/last/end, Compare +0xC, Dirty +0xD}
//   +0x68 MeshGenerators CArray<C3DGeneratorObject>               {first, last, end}
//   +0x74 PackedNames (char*)  +0x78 PackedNamesSize (ushort)  +0x7A AnimatedFlag (bool); sizeof = 0x7C.
// Every container has an inline constructor that nulls its pointers and clears its flag, which is why
// the stores trail the vtable store and the CCharString call in declaration order.
class C3DVector { public: float X, Y, Z; };
class C3DBoundingSphere { public:
    C3DVector Center;
    float     Radius;
    C3DBoundingSphere() { Center.X = 0.0f; Center.Y = 0.0f; Center.Z = 0.0f; Radius = 0.0f; }
};
class C3DBoundingBox { public: C3DVector Min, Max; };

class CCharString { public:
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

template <class T> class CVectorMap { public:     // retail: 16 bytes
    T* m_pFirst;                           // +0
    T* m_pLast;                            // +4
    T* m_pEnd;                             // +8
    CKeyPairCompareLess Compare;           // +0xC
    bool Dirty;                            // +0xD
    CVectorMap() : m_pFirst(0), m_pLast(0), m_pEnd(0), Dirty(false) { }
};

template <class T> class CArray { public:         // retail: 12 bytes
    T* m_pFirst;
    T* m_pLast;
    T* m_pEnd;
    CArray() : m_pFirst(0), m_pLast(0), m_pEnd(0) { }
};

class CMovableResourceMemoryPool;
struct C3DHelperPointPair;                 // pair<ulong, C3DHelperPoint>
struct C3DDummyObjectPair;                 // pair<ulong, C3DDummyObject>
struct C3DMeshVolumePair;                  // pair<ulong, C3DMeshVolume>
class C3DGeneratorObject;

class C3DMeshStats { public:
    virtual void v00();                    // vtable 0x0129CF04
    CMovableResourceMemoryPool*          MemoryPool;         // +0x04
    long                                 StatsMemoryHandle;  // +0x08
    CCharString                          Name;               // +0x0C
    C3DBoundingSphere                    BoundingSphere;     // +0x10
    C3DBoundingBox                       BoundingBox;        // +0x20
    CPreAllocVectorMap<C3DHelperPointPair> HelperPoints;     // +0x38
    CPreAllocVectorMap<C3DDummyObjectPair> DummyObjects;     // +0x48
    CVectorMap<C3DMeshVolumePair>        MeshVolumes;        // +0x58
    CArray<C3DGeneratorObject>           MeshGenerators;     // +0x68
    char*                                PackedNames;        // +0x74
    unsigned short                       PackedNamesSize;    // +0x78
    bool                                 AnimatedFlag;       // +0x7A
    C3DMeshStats();
};

C3DMeshStats::C3DMeshStats()
    : MemoryPool(0), StatsMemoryHandle(-1), Name(), BoundingSphere(), HelperPoints(), DummyObjects(),
      MeshVolumes(), MeshGenerators(), PackedNames(0), PackedNamesSize(0), AnimatedFlag(false)
{
}