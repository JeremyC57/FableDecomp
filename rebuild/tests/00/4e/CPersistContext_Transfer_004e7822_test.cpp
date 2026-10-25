// behaviour test for 0x004e7822 CPersistContext::Transfer<CTextureMorphs>: CRC check of the empty key in every mode, one default
// CTextureMorphs built and destroyed per call, binary load -> value.TransferBinaryIn(load stream), binary save ->
// value.TransferBinaryOut(save stream), text modes touch nothing else
#include <cstdio>
static int gCrc = 0, gCtor = 0, gDtor = 0, gIn = 0, gOut = 0;
class CMemoryDataInputStream { public: int Next; };
class CDataOutputStream { public: int Last; };
class CTextureMorphs {
public:
    int Value;
    CTextureMorphs() : Value(0) { ++gCtor; }
    ~CTextureMorphs() { ++gDtor; }
    void TransferBinaryIn(CMemoryDataInputStream& stream) { ++gIn; Value = stream.Next; }
    void TransferBinaryOut(CDataOutputStream& stream) const { ++gOut; stream.Last = Value; }
};
template <class T> class CPersistTraits {
public:
    T DefaultVal;
    void TransferBinaryIn(CMemoryDataInputStream& stream, T& value) { value.TransferBinaryIn(stream); }
    void TransferBinaryOut(CDataOutputStream& stream, const T& value) { value.TransferBinaryOut(stream); }
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
            traits.TransferBinaryOut(*PSaveStream, value);
            break;
        }
    }
};
int main()
{
    CMemoryDataInputStream in; in.Next = 5;
    CDataOutputStream out; out.Last = -1;
    CPersistContext c; c.PLoadStream = &in; c.PSaveStream = &out;
    int fails = 0;
    {
        CTextureMorphs v; v.Value = 3;
        c.Mode = CPersistContext::MODE_BINARY_SAVE; c.Transfer(v);
        if (gCrc != 1 || gOut != 1 || out.Last != 3 || gIn != 0 || gCtor != 2 || gDtor != 1) fails |= 1;
        c.Mode = CPersistContext::MODE_BINARY_LOAD; c.Transfer(v);
        if (gCrc != 2 || gIn != 1 || v.Value != 5 || gOut != 1 || gCtor != 3 || gDtor != 2) fails |= 2;
        c.Mode = CPersistContext::MODE_0; c.Transfer(v);
        c.Mode = CPersistContext::MODE_4; c.Transfer(v);
        if (gCrc != 4 || gIn != 1 || gOut != 1 || v.Value != 5 || gCtor != 5 || gDtor != 4) fails |= 4;
    }
    if (fails) { std::printf("004e7822_TEST FAIL %d\n", fails); return 1; }
    std::printf("004e7822_TEST PASS\n");
    return 0;
}