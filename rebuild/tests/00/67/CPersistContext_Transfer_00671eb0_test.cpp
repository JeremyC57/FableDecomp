// behaviour test for 0x00671EB0 CPersistContext::Transfer<EThingCreatureProperty>: CRC check of the empty key in every mode, binary save writes the value as a long, binary load goes through a zero-initialised CPersistTraits, text modes do nothing
#include <cstdio>
#include <cstring>
enum EThingCreatureProperty { THING_CREATURE_PROPERTY_0 };
static int gCrc = 0, gWrites = 0, gReads = 0;
static long gWritten = -1;
class CMemoryDataInputStream { public: long Next; };
class CDataOutputStream {
public:
    void WriteSLONG(long value) { ++gWrites; gWritten = value; }
};
template <class T> class CPersistTraits {
public:
    T Value;
    CPersistTraits() : Value((T)0) {}
    void TransferBinaryIn(CMemoryDataInputStream& stream, T& value)
    {
        ++gReads;
        if ((long)Value != 0) return;            // the traits object must arrive zeroed
        value = (T)stream.Next;
    }
};
class CPersistContext {
public:
    enum EMode { MODE_0, MODE_1, MODE_BINARY_LOAD, MODE_BINARY_SAVE, MODE_4 };
    EMode                   Mode;
    CMemoryDataInputStream* PLoadStream;
    CDataOutputStream*      PSaveStream;
    void CheckCRC(const char* name) { if (name[0] == 0) ++gCrc; }
    template <class T> void Transfer(T& value)
    {
        CheckCRC("");
        CPersistTraits<T> traits;
        switch (Mode)
        {
        case MODE_BINARY_LOAD:
            traits.TransferBinaryIn(*PLoadStream, value);
            break;
        case MODE_BINARY_SAVE:
            PSaveStream->WriteSLONG(value);
            break;
        }
    }
};
typedef EThingCreatureProperty T;
int main()
{
    CMemoryDataInputStream in; in.Next = 5;
    CDataOutputStream out;
    CPersistContext c; c.PLoadStream = &in; c.PSaveStream = &out;
    T v = (T)3;
    c.Mode = CPersistContext::MODE_BINARY_SAVE; c.Transfer(v);
    int ok = gCrc == 1 && gWrites == 1 && gWritten == 3 && gReads == 0 && (long)v == 3;
    c.Mode = CPersistContext::MODE_BINARY_LOAD; c.Transfer(v);
    ok = ok && gCrc == 2 && gReads == 1 && (long)v == 5 && gWrites == 1;
    c.Mode = CPersistContext::MODE_0; c.Transfer(v);
    c.Mode = CPersistContext::MODE_4; c.Transfer(v);
    ok = ok && gCrc == 4 && gReads == 1 && gWrites == 1 && (long)v == 5;
    if (!ok) { std::printf("00671eb0_TEST FAIL\n"); return 1; }
    std::printf("00671eb0_TEST PASS\n");
    return 0;
}