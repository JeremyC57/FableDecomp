// C3DPrimitive2::Sizeof() const -- memory footprint: the object, its device-independent data, vertex and index buffers (pointer + pointee each), the partitioned source index buffer and the partition tree
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A90860, length 157 bytes.
// Callees: 0x00A90450 N3DPrimitive::CDeviceIndependent::Sizeof, 0x00AC20D0 CVertexBufferWin32::Sizeof,
// 0x00AC1F30 CIndexBuffer::Sizeof (landed), 0x00AC2930 C3DMeshPartitionElement::Sizeof (landed).
// Layout: retail C3DPrimitive2 (0xa0 bytes; PDB less 4 bytes per CArray): Data +0x2c, VertexBuffer +0x30,
// IndexBuffer +0x34, PartitionedMeshSourceIndexBuffer +0x80 (CArray<unsigned short>), PartitionTree +0x8c
// (CCountedPointer: data + count block). ClothPrimitives and the mirrored index list are not counted.
// Shape: the engine's GetSizeof helpers inlined and accumulated statement by statement from sizeof(*this)
// (`lea edi,[eax+0A0h]`): a scoped pointer is `p ? p->Sizeof() + 4 : 4`, a counted pointer `+ 8`, a CArray
// `12 + count * 4` (`lea edx,[edx*4+0Ch]`) plus every element's own size (2 for an unsigned short, the
// pointer walk against the end held in ecx); the partition tree's size is added in the return (a single
// sum expression evaluates every term before adding and spills them).
// Leaf: ?Sizeof@C3DPrimitive2
class CVertexBufferWin32 { public: unsigned long Sizeof() const; };        // 0x00AC20D0
class CIndexBuffer       { public: unsigned long Sizeof() const; };        // 0x00AC1F30
class C3DMeshPartitionElement { public: unsigned long Sizeof() const; };   // 0x00AC2930
namespace N3DPrimitive { class CDeviceIndependent { public: unsigned long Sizeof() const; }; }   // 0x00A90450

template<class T> class CArray { public:                    // retail CArray<T>: begin/end/capacity
    T* m_begin; T* m_end; T* m_capacity;
    unsigned long Size() const { return (unsigned long)(m_end - m_begin); }
};
template<class T> struct scoped_ptr { T* m_p; };
template<class T> class CCountedPointer { public: T* Data; void* Info; };

inline unsigned long GetSizeof(const unsigned short&) { return sizeof(unsigned short); }
template<class T> inline unsigned long GetSizeof(const scoped_ptr<T>& p)
{
    return p.m_p ? p.m_p->Sizeof() + sizeof(T*) : sizeof(T*);
}
template<class T> inline unsigned long GetSizeof(const CCountedPointer<T>& p)
{
    return p.Data ? p.Data->Sizeof() + sizeof(CCountedPointer<T>) : sizeof(CCountedPointer<T>);
}
template<class T> inline unsigned long GetSizeof(const CArray<T>& a)
{
    unsigned long total = sizeof(CArray<T>) + a.Size() * 4;
    for (const T* p = a.m_begin; p != a.m_end; ++p)
        total += GetSizeof(*p);
    return total;
}

class C3DPrimitive2 { public:
    virtual void v00();
    unsigned char                                 _pad04[0x28];
    scoped_ptr<N3DPrimitive::CDeviceIndependent>  Data;                               // +0x2c
    scoped_ptr<CVertexBufferWin32>                VertexBuffer;                       // +0x30
    scoped_ptr<CIndexBuffer>                      IndexBuffer;                        // +0x34
    unsigned char                                 _pad38[0x48];
    CArray<unsigned short>                        PartitionedMeshSourceIndexBuffer;   // +0x80
    CCountedPointer<C3DMeshPartitionElement>      PartitionTree;                      // +0x8c
    unsigned char                                 _pad94[0x0c];                       // sizeof == 0xa0
    unsigned long Sizeof() const;
};

unsigned long C3DPrimitive2::Sizeof() const
{
    unsigned long size = sizeof(C3DPrimitive2) + GetSizeof(Data);
    size += GetSizeof(VertexBuffer);
    size += GetSizeof(IndexBuffer);
    size += GetSizeof(PartitionedMeshSourceIndexBuffer);
    return size + GetSizeof(PartitionTree);
}