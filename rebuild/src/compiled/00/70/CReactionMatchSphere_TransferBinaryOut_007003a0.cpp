// CReactionMatch_Sphere::TransferBinaryOut(CDataOutputStream& stream) const -- write the reaction type, the centre opinion set (count + raw floats) and the squared radius
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007003A0, length 87 bytes (virtual).
// Callees: CDataOutputStream::WriteLong @ 0x00993EB0 (x2), the stream's virtual WriteData(const void*, unsigned long)
// (slot 4, +0x10), WriteFloat @ 0x00993EE0.
// Layout: PDB names (CReactionMatch::ReactionType +0x04, Centre, SqRadius) but the retail offsets: retail CArray<T>
// is three pointers (0x0c) where the PDB's is 0x10, so COpinionSet Centre is +0x08..+0x13 and SqRadius sits at
// +0x14 (the constructor @ 0x0067C2F0 stores it there) -- engine/CReactionMatch_Sphere.h puts it at +0x18, so
// the class is modelled locally.
// Shape: the opinion set's inline writer (count as a long, then `WriteData(GetList(), Size() * 4)`) is reached
// through `lea esi,[ebx+8]`; GetList() is the retail CArray accessor returning the array object itself when empty
// (`test eax,eax ; ja ; mov ecx,esi`), as in the landed C3DMeshFileXTriangleStripChunk::WriteToFile.
// Leaf: TransferBinaryOut
struct CDataOutputStream {
    virtual void v0(); virtual void v1(); virtual void v2(); virtual void v3();
    virtual void WriteData(const void* data, unsigned long size);   // slot 4 (+0x10)
    void WriteLong(long value);                         // 0x00993EB0
    void WriteFloat(float value);                       // 0x00993EE0
};

template<class T> struct CArray {                       // retail CArray<T>: begin/end/capacity
    T* m_begin; T* m_end; T* m_capacity;
    unsigned long Size() const { return (unsigned long)(m_end - m_begin); }
    const T* GetList() const { return Size() > 0 ? m_begin : (const T*)this; }
};

struct COpinionSet {
    CArray<float> OpinionValues;                        // +0x00
    void TransferBinaryOut(CDataOutputStream& stream) const
    {
        stream.WriteLong(OpinionValues.Size());
        stream.WriteData(OpinionValues.GetList(), OpinionValues.Size() * sizeof(float));
    }
};

struct CReactionMatch_Sphere {
    void*       __vftable;                              // +0x00
    long        ReactionType;                           // +0x04 CReactionMatch::ReactionType (EOpinionReactionType)
    COpinionSet Centre;                                 // +0x08
    float       SqRadius;                               // +0x14
    void TransferBinaryOut(CDataOutputStream& stream) const;
};

void CReactionMatch_Sphere::TransferBinaryOut(CDataOutputStream& stream) const
{
    stream.WriteLong(ReactionType);
    Centre.TransferBinaryOut(stream);
    stream.WriteFloat(SqRadius);
}