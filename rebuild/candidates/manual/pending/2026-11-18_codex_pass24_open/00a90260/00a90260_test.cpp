#include <cassert>
#include <cstdio>
#include <string.h>
#include <vector>

template<class T> class CArray {
    std::vector<T> Values;
public:
    explicit CArray(unsigned long size):Values(size) {}
    T& operator[](unsigned long index) { return Values.at(index); }
};
namespace boost {
template<class T> class scoped_ptr {
    T* Pointer;
    scoped_ptr(const scoped_ptr&);
    scoped_ptr& operator=(const scoped_ptr&);
public:
    scoped_ptr():Pointer(0) {}
    ~scoped_ptr() { delete Pointer; }
    void reset(T* pointer) { delete Pointer; Pointer=pointer; }
    T* operator->() const { return Pointer; }
    T& operator*() const { return *Pointer; }
    void swap(scoped_ptr& other) { T* old=Pointer; Pointer=other.Pointer; other.Pointer=old; }
};
}
namespace N3DPrimitive {
struct CStaticBlock {
    unsigned long StartIndex, PrimitiveCount;
    bool DegenerateTriangles;
};
}
struct BufferEvents {
    unsigned Init, SourceLocks, SourceUnlocks, DestLocks, DestUnlocks, SourceDeletes, DestDeletes;
    unsigned long Flags, NewSize;
    BufferEvents():Init(0),SourceLocks(0),SourceUnlocks(0),DestLocks(0),DestUnlocks(0),
        SourceDeletes(0),DestDeletes(0),Flags(0),NewSize(0) {}
};
static BufferEvents Events;
class CIndexBuffer {
    bool Locked;
public:
    bool Source;
    std::vector<unsigned short> Indices;
    CIndexBuffer():Locked(false),Source(false) {}
    ~CIndexBuffer() { assert(!Locked); ++(Source ? Events.SourceDeletes : Events.DestDeletes); }
    unsigned long GetSize() const { return Indices.size()*sizeof(unsigned short); }
    void* Lock(unsigned long flags, unsigned long offset, unsigned long size)
    {
        assert(!Locked && flags==0 && offset==0 && size==GetSize() && size>0);
        assert(Source || Events.SourceLocks==1);
        Locked=true;
        ++(Source ? Events.SourceLocks : Events.DestLocks);
        return &Indices[0];
    }
    void Unlock()
    {
        assert(Locked);
        assert(!Source || Events.DestUnlocks==1);
        Locked=false;
        ++(Source ? Events.SourceUnlocks : Events.DestUnlocks);
    }
};
struct C3DPrimitive2 {
    boost::scoped_ptr<CIndexBuffer> IndexBuffer;
    unsigned long StaticBlockCount, InitFlags;
    N3DPrimitive::CStaticBlock* StaticBlocks;
    void InitIndexBuffer(CIndexBuffer& buffer,unsigned long size,unsigned long flags)
    {
        assert(!buffer.Source && size && !(size % sizeof(unsigned short)));
        ++Events.Init;
        Events.Flags=flags;
        Events.NewSize=size;
        buffer.Indices.resize(size/sizeof(unsigned short));
    }
    bool CreateDegenerateTriangleIndexBuffer();
};

bool C3DPrimitive2::CreateDegenerateTriangleIndexBuffer()
{
    unsigned long total_indices = IndexBuffer->GetSize() / sizeof(unsigned short);
    CArray<unsigned long> index_counts(StaticBlockCount);
    {
        for (unsigned long c_block = 0; c_block < StaticBlockCount; ++c_block) {
            N3DPrimitive::CStaticBlock& block_info = StaticBlocks[c_block];
            unsigned long count;
            if (c_block + 1 < StaticBlockCount) {
                N3DPrimitive::CStaticBlock& next_block_info = StaticBlocks[c_block + 1];
                count = next_block_info.StartIndex - block_info.StartIndex;
            } else
                count = total_indices - block_info.StartIndex;
            index_counts[c_block] = count;
        }
    }
    unsigned long degenerate_indices = 0;
    {
        for (unsigned long c_block = 0; c_block < StaticBlockCount; ++c_block) {
            N3DPrimitive::CStaticBlock& block_info = StaticBlocks[c_block];
            if (block_info.DegenerateTriangles)
                degenerate_indices += index_counts[c_block];
        }
    }
    boost::scoped_ptr<CIndexBuffer> new_index_buffer;
    if (degenerate_indices > 0) {
        new_index_buffer.reset(new CIndexBuffer);
        unsigned long new_byte_size = degenerate_indices * sizeof(unsigned short);
        InitIndexBuffer(*new_index_buffer, new_byte_size, InitFlags);
        const unsigned short* src_indices = (const unsigned short*)IndexBuffer->Lock(0, 0, total_indices * sizeof(unsigned short));
        unsigned short* dest_indices = (unsigned short*)new_index_buffer->Lock(0, 0, new_byte_size);
        unsigned long src_offset = 0;
        unsigned long dest_offset = 0;
        for (unsigned long c_block = 0; c_block < StaticBlockCount; ++c_block) {
            N3DPrimitive::CStaticBlock& block_info = StaticBlocks[c_block];
            unsigned long count = index_counts[c_block];
            if (block_info.DegenerateTriangles) {
                block_info.StartIndex = dest_offset;
                memcpy(dest_indices + dest_offset, src_indices + src_offset, count * sizeof(unsigned short));
                dest_offset += count;
            }
            src_offset += count;
        }
        new_index_buffer->Unlock();
        IndexBuffer->Unlock();
    }
    {
        for (unsigned long c_block = 0; c_block < StaticBlockCount; ++c_block) {
            N3DPrimitive::CStaticBlock& block_info = StaticBlocks[c_block];
            if (!block_info.DegenerateTriangles) {
                block_info.StartIndex = 0;
                block_info.PrimitiveCount = 0;
            }
        }
    }
    IndexBuffer.swap(new_index_buffer);
    return true;
}

int main()
{
    for (unsigned count=0; count<=4; ++count) {
        for (unsigned mask=0; mask<(1U<<count); ++mask) {
            Events=BufferEvents();
            {
                C3DPrimitive2 primitive;
                N3DPrimitive::CStaticBlock blocks[4];
                unsigned long starts[4];
                std::vector<unsigned short> expected;
                CIndexBuffer* original=new CIndexBuffer;
                original->Source=true;
                primitive.StaticBlocks=blocks;
                primitive.StaticBlockCount=count;
                primitive.InitFlags=0x63;
                primitive.IndexBuffer.reset(original);
                for (unsigned b=0; b<count; ++b) {
                    N3DPrimitive::CStaticBlock& block=blocks[b];
                    block.StartIndex=original->Indices.size();
                    block.PrimitiveCount=b+6;
                    block.DegenerateTriangles=(mask & (1U<<b))!=0;
                    starts[b]=block.DegenerateTriangles ? expected.size() : 0;
                    for (unsigned i=0; i<=b; ++i) {
                        unsigned short value=static_cast<unsigned short>(b*100+i);
                        original->Indices.push_back(value);
                        if (block.DegenerateTriangles)
                            expected.push_back(value);
                    }
                }
                assert(primitive.CreateDegenerateTriangleIndexBuffer());
                const unsigned calls=mask ? 1 : 0;
                assert(Events.Init==calls && Events.SourceLocks==calls && Events.SourceUnlocks==calls);
                assert(Events.DestLocks==calls && Events.DestUnlocks==calls);
                assert(Events.SourceDeletes==1 && Events.DestDeletes==0);
                assert(Events.NewSize==expected.size()*sizeof(unsigned short));
                if (mask) {
                    assert(Events.Flags==primitive.InitFlags);
                    assert(primitive.IndexBuffer->Indices==expected);
                } else {
                    assert(!primitive.IndexBuffer.operator->());
                }
                for (unsigned b=0; b<count; ++b) {
                    assert(blocks[b].StartIndex==starts[b]);
                    assert(blocks[b].PrimitiveCount==((mask & (1U<<b)) ? b+6 : 0));
                }
            }
            assert(Events.DestDeletes==(mask ? 1U : 0U));
        }
    }
    std::puts("00a90260_TEST PASS");
    return 0;
}
