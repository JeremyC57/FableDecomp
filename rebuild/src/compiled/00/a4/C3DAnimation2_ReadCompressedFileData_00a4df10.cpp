// C3DAnimation2::ReadCompressedFileData(C3DAnimFileObjectChunk* chunk, CSimpleMemoryPool* pool) -- size the sequence arrays from the XSEQ sub-chunk count, then read each XSEQ chunk's compressed sequence through a memory stream
// Leaf: ReadCompressedFileData
// Shape: the walk pointer is default-constructed and then assigned from FindFirst (retail zeroes it before the call and
//   adopts with `cmp ecx,0 ; je`); the stream's data pointer is a named local (computed before the size); the step
//   `sequence = FindNext(prev copy)` is the last body statement, so the stream dies after it.
// Layout: retail C3DAnimation2 is 4 smaller than Ego_r.pdb from +0x40 (landed 0x00A4E150): SequenceCount +0x58, Sequences +0x5c,
//   SequenceDataBufferSize +0x60, SequenceDataBuffer +0x64; C3DAnimationSequence 8 bytes, C3DAnimationSequenceData 0x2c;
//   C3DAnimFileXSequenceChunk CompressedData (STLport vector) +0x10; retail CMemoryDataInputStream 0x1c (GOTCHAS chunk 244).
struct SCountBlock { long references; void (__fastcall* destroy)(void*); void* object; };
template<class T> class CCountedPointer { public:
    T* Object; SCountBlock* Count;
    CCountedPointer() : Object(0), Count(0) {}
    CCountedPointer(const CCountedPointer& o) : Object(o.Object), Count(o.Count) { if (Count) ++Count->references; }
    template<class U> CCountedPointer(const CCountedPointer<U>& o) : Object((T*)o.Object), Count(o.Count) { if (Count) ++Count->references; }
    ~CCountedPointer() { if (Count) { --Count->references; if (!Count->references) { Count->destroy(Count->object); delete Count; } } Object = 0; Count = 0; }
    struct CastTag {};
    template<class U> CCountedPointer(const CCountedPointer<U>& o, CastTag) : Object(0), Count(0) { if (o.Count) { Object = (T*)o.Object; Count = o.Count; ++Count->references; } }
    template<class U> CCountedPointer& operator=(const CCountedPointer<U>& o)
    {
        SCountBlock* c = o.Count; T* p = (T*)o.Object;
        if (Count != c) {
            if (Count) { --Count->references; if (!Count->references) { Count->destroy(Count->object); delete Count; } }
            Object = p; Count = c; if (c) ++c->references;
        }
        return *this;
    }
    T* operator->() const { return Object; }
};

template <class T> class CArray { public:                 // retail STLport vector layout
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    unsigned long size() const { return _M_finish - _M_start; }
    const void* GetData() const { return size() ? (const void*)_M_start : (const void*)this; }
};

class CDataInputStream { public: virtual ~CDataInputStream(); };
class CMemoryDataInputStream : public CDataInputStream { public:
    CMemoryDataInputStream(const void* data, long size);   // 0x00994930
    ~CMemoryDataInputStream();                       // 0x00993B10
    long position, sourceLength, chunkBase, chunkPosition, chunkLength;
    void* StreamSource;
};

class CChunkedFileChunk;
class CChunkedFileSuperChunk { public:
    unsigned long CountSubChunksOfType(const char* type) const;                                  // 0x00AA7EA0
    CCountedPointer<CChunkedFileChunk> FindFirstSubChunkOfType(const char* type) const;         // 0x00AA7F40
    CCountedPointer<CChunkedFileChunk> FindNextSubChunkOfType(const CCountedPointer<const CChunkedFileChunk>& current) const;   // 0x00AA7FF0
};
class C3DAnimFileObjectChunk : public CChunkedFileSuperChunk { public:};
class C3DAnimFileXSequenceChunk { public:
    unsigned char _00[0x10];
    CArray<unsigned char> CompressedData;           // +0x10
};

class CSimpleMemoryPool { public: template<class T> T* New(unsigned long count); };   // C3DAnimationSequence 0x00A533F0, Data 0x00A53440
class C3DAnimationSequenceData { public: unsigned char _00[0x2c]; };
class C3DAnimationSequence { public:
    void* _00; void* _04;
    void ReadCompressedFromFile(CDataInputStream* in, unsigned long size, C3DAnimationSequenceData* data, CSimpleMemoryPool* pool);   // 0x00A4CDD0
};

class C3DAnimation2 {
public:
    unsigned char _00[0x58];
    unsigned long             SequenceCount;            // +0x58
    C3DAnimationSequence*     Sequences;                // +0x5c
    unsigned long             SequenceDataBufferSize;   // +0x60
    C3DAnimationSequenceData* SequenceDataBuffer;       // +0x64
    void ReadCompressedFileData(C3DAnimFileObjectChunk* chunk, CSimpleMemoryPool* pool);
};

void C3DAnimation2::ReadCompressedFileData(C3DAnimFileObjectChunk* chunk, CSimpleMemoryPool* pool)
{
    SequenceCount = chunk->CountSubChunksOfType("XSEQ");
    Sequences = pool->New<C3DAnimationSequence>(SequenceCount);
    SequenceDataBufferSize = SequenceCount;
    SequenceDataBuffer = pool->New<C3DAnimationSequenceData>(SequenceDataBufferSize);
    unsigned long index = 0;
    CCountedPointer<C3DAnimFileXSequenceChunk> sequence;
    sequence = chunk->FindFirstSubChunkOfType("XSEQ");
    while (sequence.Object) {
        const void* data = sequence->CompressedData.GetData();
        CMemoryDataInputStream stream(data, sequence->CompressedData.size());
        Sequences[index].ReadCompressedFromFile(&stream, sequence->CompressedData.size(), &SequenceDataBuffer[index], pool);
        ++index;
        sequence = chunk->FindNextSubChunkOfType(CCountedPointer<const CChunkedFileChunk>(sequence));
    }
}