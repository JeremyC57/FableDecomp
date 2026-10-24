// CParticleProgram::WriteGroups(CDataOutputStream& stream, const CArray<C3DGroup2>& groups) -- private static save helper: the group count, then per group its bone index, its vertex-blend count and the raw blend array
// Leaf: ?WriteGroups@CParticleProgram@@CI
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AE0680, 133 bytes, static __fastcall (stream in ecx, groups in edx), plain `ret`.
// Identity: Ego_r.pdb `?WriteGroups@CParticleProgram@@CIXAAVCDataOutputStream@@ABV?$CArray@VC3DGroup2@@@@@Z`; its only
//   callers are the two tail calls of CParticleProgram::WriteToFile 0x00AE2230 (landed; SimGroups +0xFC, NonSimGroups +0x108).
// Callees: CDataOutputStream::WriteULong 0x00993EA0 (x3), C3DGroup2::PeekVertexBlends() const 0x00AC40A0 (the CArray data
//   accessor `size() > 0 ? first : (T*)&array`; catalogued under the propagated "C3DMeshFileGroupChunk::GetList"), the
//   stream's virtual Write(const void*, long) (slot 4, +0x10).
// Layout: retail CArray<T> is an STLport vector of three pointers (12 bytes); C3DGroup2 is {long BoneIndex,
//   CArray<C3DVertexBlend2> VertexBlends} = 16 bytes; C3DVertexBlend2 8 bytes.
// Shape: the count is read once (`for (i = 0; i < count; ++i)` becomes a down-counter in a frame word next to the spilled
//   groups reference); the blend count is the inline C3DGroup2::GetVertexBlendCount(); Write's byte count is pushed
//   first (right-to-left), and the stream's vtable is fetched ahead of the out-of-line accessor call.
struct C3DVertexBlend2 { long Vertex; float Weight; };   // 8 bytes

template <class T> class CArray {                      // retail CArray<T>: STLport vector, three pointers
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    const T& operator[](unsigned int i) const { return _M_start[i]; }
};

class C3DGroup2 {                                      // 16 bytes in retail
public:
    long                     BoneIndex;                // +0x0
    CArray<C3DVertexBlend2>  VertexBlends;             // +0x4
    long GetBoneIndex() const { return BoneIndex; }
    long GetVertexBlendCount() const { return VertexBlends.size(); }
    const C3DVertexBlend2* PeekVertexBlends() const;   // 0x00AC40A0
};

class CDataOutputStream {
public:
    virtual void v00();
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void Write(const void* data, long length); // slot 4 (+0x10)
    void WriteULong(unsigned long value);              // 0x00993EA0
};

class CParticleProgram {
private:
    static void __fastcall WriteGroups(CDataOutputStream& stream, const CArray<C3DGroup2>& groups);
};

void __fastcall CParticleProgram::WriteGroups(CDataOutputStream& stream, const CArray<C3DGroup2>& groups)
{
    unsigned long count = groups.size();
    stream.WriteULong(count);
    for (unsigned long i = 0; i < count; ++i) {
        const C3DGroup2& group = groups[i];
        stream.WriteULong(group.GetBoneIndex());
        long blends = group.GetVertexBlendCount();
        stream.WriteULong(blends);
        stream.Write(group.PeekVertexBlends(), blends * sizeof(C3DVertexBlend2));
    }
}