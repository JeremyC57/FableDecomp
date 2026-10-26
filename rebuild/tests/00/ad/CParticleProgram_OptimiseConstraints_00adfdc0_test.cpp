// Behaviour test for CParticleProgram::OptimiseConstraints (0x00ADFDC0): records are (type, count, size, data);
// consecutive records of one type other than 0 and 1 merge (counts and sizes summed, data concatenated), type 0/1
// records and type changes start a new record, and the merged stream replaces the program's constraint stream.
#include <cstdio>
#include <cstring>
#include <vector>

class CDataOutputStream {
public:
    std::vector<unsigned char> bytes;
    virtual ~CDataOutputStream() {}
    virtual void Write(const void* data, long length)
    {
        const unsigned char* p = (const unsigned char*)data;
        bytes.insert(bytes.end(), p, p + length);
    }
    void WriteULong(unsigned long value)
    {
        unsigned int v = (unsigned int)value;
        Write(&v, 4);
    }
};
class CMemoryDataOutputStream : public CDataOutputStream {
public:
    void Reset() { bytes.clear(); }
    long GetSize() { return (long)bytes.size(); }
    void* PeekData() { return bytes.empty() ? (void*)this : (void*)&bytes[0]; }
};
class CDataInputStream { public:
    std::vector<unsigned char> source;
    unsigned long StreamPos;
    long StreamSize;
    void Read(void* dst, long size)
    {
        if ((long)StreamPos + size > StreamSize)
            return;
        std::memcpy(dst, &source[StreamPos], size);
        StreamPos += size;
    }
    unsigned long ReadULong() { unsigned int value = 0; Read(&value, 4); return value; }
    void ReadToStream(CDataOutputStream& out, long size)
    {
        if (size > 0)
            out.Write(&source[StreamPos], size);
        StreamPos += size;
    }
};
class CMemoryDataInputStream : public CDataInputStream { public:
    CMemoryDataInputStream(const CMemoryDataOutputStream& s)
    {
        source = s.bytes;
        StreamPos = 0;
        StreamSize = (long)source.size();
    }
};
class CParticleProgram {
public:
    CMemoryDataOutputStream Constraints;
    void OptimiseConstraints();
};

void CParticleProgram::OptimiseConstraints()
{
    CMemoryDataOutputStream& constraints = Constraints;
    CMemoryDataInputStream in(constraints);
    CMemoryDataOutputStream merged;
    long lastType = -1;
    long totalCount = 0;
    long totalSize = 0;
    CMemoryDataOutputStream data;
    while (in.StreamPos != (unsigned long)in.StreamSize) {
        long type = in.ReadULong();
        long count = in.ReadULong();
        long size = in.ReadULong();
        if (type == lastType && type != 0 && type != 1) {
            totalCount += count;
            totalSize += size;
        } else {
            if (totalCount != 0) {
                merged.WriteULong(lastType);
                merged.WriteULong(totalCount);
                merged.WriteULong(totalSize);
                merged.Write(data.PeekData(), data.GetSize());
            }
            lastType = type;
            totalCount = count;
            totalSize = size;
            data.Reset();
        }
        in.ReadToStream(data, size);
    }
    if (totalCount != 0) {
        merged.WriteULong(lastType);
        merged.WriteULong(totalCount);
        merged.WriteULong(totalSize);
        merged.Write(data.PeekData(), data.GetSize());
    }
    constraints.Reset();
    constraints.Write(merged.PeekData(), merged.GetSize());
}

static void Record(CMemoryDataOutputStream& s, unsigned long type, unsigned long count, const char* payload)
{
    unsigned long size = (unsigned long)std::strlen(payload);
    s.WriteULong(type);
    s.WriteULong(count);
    s.WriteULong(size);
    s.Write(payload, (long)size);
}

int main()
{
    CParticleProgram program;
    Record(program.Constraints, 2, 1, "ab");
    Record(program.Constraints, 2, 2, "cde");
    Record(program.Constraints, 0, 1, "f");
    Record(program.Constraints, 0, 1, "g");
    Record(program.Constraints, 3, 4, "hi");
    Record(program.Constraints, 2, 1, "j");
    Record(program.Constraints, 2, 1, "k");
    program.OptimiseConstraints();

    CMemoryDataOutputStream expected;
    Record(expected, 2, 3, "abcde");
    Record(expected, 0, 1, "f");
    Record(expected, 0, 1, "g");
    Record(expected, 3, 4, "hi");
    Record(expected, 2, 2, "jk");
    if (program.Constraints.bytes != expected.bytes) {
        std::printf("00adfdc0_TEST FAIL\n");
        return 1;
    }

    // An empty stream stays empty.
    CParticleProgram empty;
    empty.OptimiseConstraints();
    if (!empty.Constraints.bytes.empty()) {
        std::printf("00adfdc0_TEST FAIL\n");
        return 1;
    }
    std::printf("00adfdc0_TEST PASS\n");
    return 0;
}