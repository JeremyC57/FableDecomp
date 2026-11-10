// Behaviour test for 0x00477cd2 CPersistContext::TransferVectorOfSubComponents<CEngineLocalDetailObjectDef> (self-contained C++03 model of the shared body:
// count + each element's Transfer on save, inline chunk read of the count + resize + each element's Transfer on binary load).
#include <stdio.h>
#include <string.h>
#include <vector>

#ifdef _MSC_VER
typedef __int64 TestInt64;
#else
typedef long long TestInt64;
#endif

class CPersistContext;
static int g_fail = 0;
static void Check(bool ok, const char* what) { if (!ok) { printf("FAIL: %s\n", what); g_fail = 1; } }

static const CPersistContext* g_transferContext[16];
static int g_transferIndex[16];
static int g_transferCount = 0;

class CEngineLocalDetailObjectDef {                                   // element model: the real def is 0x88 bytes, Transfer is vtable slot 18
public:
    int Index;
    unsigned char Body[0x88 - 8];
    CEngineLocalDetailObjectDef() : Index(-1) { memset(Body, 0, sizeof(Body)); }
    virtual ~CEngineLocalDetailObjectDef() { }
    virtual void Transfer(CPersistContext& context)
    {
        if (g_transferCount < 16) {
            g_transferContext[g_transferCount] = &context;
            g_transferIndex[g_transferCount] = Index;
        }
        ++g_transferCount;
    }
};

class CDataOutputStream {                     // WriteSLONG 0x00993EB0 model: records the written longs
public:
    long Written[8];
    int WriteCount;
    CDataOutputStream() : WriteCount(0) { }
    void WriteSLONG(long value) { if (WriteCount < 8) Written[WriteCount] = value; ++WriteCount; }
};

class CMemoryDataInputStream {                // CDataInputStream fields +4..+0x14 and the inline bounds-checked Read
public:
    unsigned long StreamPos;
    long StreamSize;
    const unsigned char* CurrentSourcePtr;
    unsigned long SourceChunkStreamPos;
    long SourceChunkBytesRemaining;
    int OverflowValue;
    int OverflowCalls;
    CMemoryDataInputStream() : StreamPos(0), StreamSize(0), CurrentSourcePtr(0), SourceChunkStreamPos(0),
        SourceChunkBytesRemaining(0), OverflowValue(0), OverflowCalls(0) { }
    void ReadWithSrcChunkOverflow(unsigned char* buffer, long size)   // 0x00993CA0 model: the next chunk supplies the bytes
    {
        ++OverflowCalls;
        memcpy(buffer, &OverflowValue, size);
        StreamPos += size;
    }
    void Read(void* buffer, long size)
    {
        if (size <= 0) return;
        TestInt64 newPosition = (TestInt64)(StreamPos + size);
        if (newPosition < 0 || newPosition > 0x7fffffff) return;
        if (size <= SourceChunkBytesRemaining) {
            memcpy(buffer, CurrentSourcePtr, size);
            CurrentSourcePtr += size;
            SourceChunkBytesRemaining -= size;
            StreamPos += size;
        } else ReadWithSrcChunkOverflow((unsigned char*)buffer, size);
    }
    long ReadSLONG() { int value; Read(&value, sizeof(value)); return value; }   // the stream long is 4 bytes (int on LP64 hosts)
};

class CPersistContext {
public:
    enum EMode { MODE_LOAD_TEXT = 0, MODE_SAVE_TEXT = 1, MODE_LOAD_BINARY = 2, MODE_SAVE_BINARY = 3, MODE_CLEAR = 4 };
    EMode Mode;
    CMemoryDataInputStream* PLoadStream;
    CDataOutputStream* PSaveStream;
    int CRCChecks;
    const char* LastCRCName;
    CPersistContext() : Mode(MODE_SAVE_BINARY), PLoadStream(0), PSaveStream(0), CRCChecks(0), LastCRCName(0) { }
    void CheckCRC(const char* name) { ++CRCChecks; LastCRCName = name; }
    void TransferVectorOfSubComponents(std::vector<CEngineLocalDetailObjectDef>& value)
    {
        CheckCRC("");
        switch (Mode) {
        case MODE_LOAD_BINARY: {
            long count = PLoadStream->ReadSLONG();
            value.resize(count);
            if (count > 0)
                for (long i = 0; i < count; ++i)
                    value[i].Transfer(*this);
            break;
        }
        default: {
            long count = value.size();
            PSaveStream->WriteSLONG(count);
            for (long i = 0; i < count; ++i)
                value[i].Transfer(*this);
            break;
        }
        }
    }
};

int main()
{
    // save: the count, then every element in order with this context
    {
        CPersistContext context; CDataOutputStream out; context.PSaveStream = &out;
        std::vector<CEngineLocalDetailObjectDef> v(3);
        for (int i = 0; i < 3; ++i) v[i].Index = 10 + i;
        g_transferCount = 0;
        context.TransferVectorOfSubComponents(v);
        Check(context.CRCChecks == 1 && context.LastCRCName != 0 && context.LastCRCName[0] == 0, "save checks the empty key");
        Check(out.WriteCount == 1 && out.Written[0] == 3, "save writes the element count");
        Check(g_transferCount == 3, "save transfers every element");
        Check(g_transferIndex[0] == 10 && g_transferIndex[1] == 11 && g_transferIndex[2] == 12, "save transfers in order");
        Check(g_transferContext[0] == &context && g_transferContext[2] == &context, "save passes this context");
    }
    // any mode but binary load writes (text / reset modes take the save arm)
    {
        CPersistContext context; CDataOutputStream out; context.PSaveStream = &out; context.Mode = CPersistContext::MODE_CLEAR;
        std::vector<CEngineLocalDetailObjectDef> v(2);
        g_transferCount = 0;
        context.TransferVectorOfSubComponents(v);
        Check(out.WriteCount == 1 && out.Written[0] == 2 && g_transferCount == 2, "mode 4 writes like a save");
    }
    // binary load from the current chunk: count, resize, transfer each new element
    {
        unsigned char chunk[8]; int two = 2; memcpy(chunk, &two, 4); memset(chunk + 4, 0xAB, 4);
        CMemoryDataInputStream in; in.CurrentSourcePtr = chunk; in.SourceChunkBytesRemaining = 8; in.StreamPos = 100;
        CPersistContext context; context.Mode = CPersistContext::MODE_LOAD_BINARY; context.PLoadStream = &in;
        std::vector<CEngineLocalDetailObjectDef> v(5);
        g_transferCount = 0;
        context.TransferVectorOfSubComponents(v);
        Check(v.size() == 2, "load resizes to the read count");
        Check(g_transferCount == 2 && g_transferContext[1] == &context, "load transfers every element");
        Check(in.StreamPos == 104 && in.SourceChunkBytesRemaining == 4 && in.CurrentSourcePtr == chunk + 4, "load advances the stream by 4");
        Check(in.OverflowCalls == 0, "a full chunk needs no overflow read");
    }
    // binary load of an empty vector: no element transfers
    {
        unsigned char chunk[4]; int zero = 0; memcpy(chunk, &zero, 4);
        CMemoryDataInputStream in; in.CurrentSourcePtr = chunk; in.SourceChunkBytesRemaining = 4;
        CPersistContext context; context.Mode = CPersistContext::MODE_LOAD_BINARY; context.PLoadStream = &in;
        std::vector<CEngineLocalDetailObjectDef> v(4);
        g_transferCount = 0;
        context.TransferVectorOfSubComponents(v);
        Check(v.empty() && g_transferCount == 0, "a zero count empties the vector and transfers nothing");
    }
    // binary load across a chunk boundary: ReadWithSrcChunkOverflow supplies the count
    {
        unsigned char chunk[2] = { 0, 0 };
        CMemoryDataInputStream in; in.CurrentSourcePtr = chunk; in.SourceChunkBytesRemaining = 2; in.OverflowValue = 1;
        CPersistContext context; context.Mode = CPersistContext::MODE_LOAD_BINARY; context.PLoadStream = &in;
        std::vector<CEngineLocalDetailObjectDef> v;
        g_transferCount = 0;
        context.TransferVectorOfSubComponents(v);
        Check(in.OverflowCalls == 1 && v.size() == 1 && g_transferCount == 1, "a short chunk reads the count through the overflow path");
    }
    if (g_fail) return 1;
    printf("00477cd2_TEST PASS\n");
    return 0;
}