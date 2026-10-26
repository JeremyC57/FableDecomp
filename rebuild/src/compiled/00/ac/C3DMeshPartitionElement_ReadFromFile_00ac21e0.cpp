// C3DMeshPartitionElement::ReadFromFile(CDataInputStream&) -- free the old sub-element tree, read the box, sphere and triangle range, then (if flagged) allocate both children and read them recursively
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AC21E0, 1472 bytes, ret 4.
// Callees: ReadWithSrcChunkOverflow 0x00993CA0, the scalar deleting destructor 0x00AC37B0 (grandchildren),
//   operator delete 0x00BFE9BC, operator new 0x00BFEA1A, itself (the second recursion becomes the loop back to the top).
// Shape: the box floats fill two C3DVector locals component by component and go through an inline Set (this copied
//   to a scratch register); the radius is read into a named local BEFORE the centre copy, then both are plain stores.
//   The new'd children run the inline constructor (sphere zeroed by its member ctor first, then the body stores).
// Layout: PDB (Ego_r) C3DMeshPartitionElement, 0x38 bytes, as in the landed 0x00AC2100 / 0x00AC27A0. No engine header.
extern "C" void* __cdecl memcpy(void* dest, const void* src, unsigned int bytes);
#pragma intrinsic(memcpy)

class CDataInputStream { public:
    void* __vftable;
    unsigned long m_position;              // +4
    long StreamSize;                       // +8
    const unsigned char* m_pData;          // +0xC
    long SourceChunkStreamPos;             // +0x10
    long m_remaining;                      // +0x14
    void ReadWithSrcChunkOverflow(unsigned char* buffer, long size);   // 0x00993CA0
    static __forceinline bool IsValidPosition(__int64 position) { return position >= 0 && position <= 0x7fffffff; }
    __forceinline void Read(void* buffer, long size)
    {
        __int64 newPosition = (__int64)(m_position + size);
        if (!IsValidPosition(newPosition))
            return;
        if (m_remaining >= size) {
            memcpy(buffer, m_pData, size);
            m_pData += size;
            m_remaining -= size;
            m_position += size;
        }
        else
            ReadWithSrcChunkOverflow((unsigned char*)buffer, size);
    }
    __forceinline bool ReadBool() { bool value; Read(&value, sizeof(value)); return value; }
    __forceinline long ReadLong() { long value; Read(&value, sizeof(value)); return value; }
    __forceinline float ReadFloat() { float value; Read(&value, sizeof(value)); return value; }
};

void* __cdecl operator new(unsigned int size);      // 0x00BFEA1A
void __cdecl operator delete(void* block);          // 0x00BFE9BC

class C3DVector { public:
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
};
class C3DBoundingBox { public:
    C3DVector Min, Max;
    void Set(const C3DVector& mn, const C3DVector& mx) { Min = mn; Max = mx; }
};
class C3DBoundingSphere { public:
    C3DVector Center;
    float Radius;
    C3DBoundingSphere() : Center(0.0f, 0.0f, 0.0f), Radius(0.0f) {}
};

class C3DMeshPartitionElement { public:
    C3DBoundingBox           BoundingBox;      // +0x00
    long                     StartTriangle;    // +0x18
    long                     TriangleCount;    // +0x1C
    C3DMeshPartitionElement* SubElements[2];   // +0x20
    C3DBoundingSphere        BoundingSphere;   // +0x28

    C3DMeshPartitionElement() { SubElements[0] = 0; SubElements[1] = 0; StartTriangle = 0; TriangleCount = 0; }
    ~C3DMeshPartitionElement()
    {
        delete SubElements[0];
        delete SubElements[1];
        StartTriangle = 0;
        TriangleCount = 0;
    }
    void ReadFromFile(CDataInputStream& in);
};

void C3DMeshPartitionElement::ReadFromFile(CDataInputStream& in)
{
    delete SubElements[0];
    delete SubElements[1];
    SubElements[0] = 0;
    SubElements[1] = 0;
    C3DVector boxMin;
    boxMin.X = in.ReadFloat();
    boxMin.Y = in.ReadFloat();
    boxMin.Z = in.ReadFloat();
    C3DVector boxMax;
    boxMax.X = in.ReadFloat();
    boxMax.Y = in.ReadFloat();
    boxMax.Z = in.ReadFloat();
    BoundingBox.Set(boxMin, boxMax);
    C3DVector centre;
    centre.X = in.ReadFloat();
    centre.Y = in.ReadFloat();
    centre.Z = in.ReadFloat();
    float radius = in.ReadFloat();
    BoundingSphere.Center = centre;
    BoundingSphere.Radius = radius;
    StartTriangle = in.ReadLong();
    TriangleCount = in.ReadLong();
    if (in.ReadBool()) {
        SubElements[0] = new C3DMeshPartitionElement;
        SubElements[1] = new C3DMeshPartitionElement;
        SubElements[0]->ReadFromFile(in);
        SubElements[1]->ReadFromFile(in);
    }
}