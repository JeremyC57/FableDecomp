// Behaviour test for 0x00479187 CPersistContext::Transfer<NSpeechGainManager::EDialogueLayer>(NSpeechGainManager::EDialogueLayer&): the unnamed binary-only enum transfer.
// Self-contained model of the row (same body, mocked callees): CheckCRC("") always, Mode 2 reads through the traits' binary-in,
// Mode 3 writes the value with WriteSLONG, any other mode transfers nothing.
#include <cstdio>
#include <cstring>

namespace NSpeechGainManager { enum EDialogueLayer { EDIALOGUELAYER_A = 0, EDIALOGUELAYER_B = 7 }; }
using namespace NSpeechGainManager;

class CMemoryDataInputStream { public: long Next; };
class CDataOutputStream {
public:
    int Writes; long Last;
    void WriteSLONG(long value) { ++Writes; Last = value; }
};
static int g_reads = 0;
template <class T> class CPersistTraits {
public:
    T DefaultVal;
    CPersistTraits() : DefaultVal() { }
    void TransferBinaryIn(CMemoryDataInputStream& stream, T& value);
    void TransferBinaryOut(CDataOutputStream& stream, T value) { stream.WriteSLONG((long)value); }
};
template <> void CPersistTraits<EDialogueLayer>::TransferBinaryIn(CMemoryDataInputStream& stream, EDialogueLayer& value)
{
    ++g_reads; value = (EDialogueLayer)stream.Next;
}
class CPersistContext {
public:
    enum EMode { MODE_LOAD_TEXT = 0, MODE_SAVE_TEXT = 1, MODE_LOAD_BINARY = 2, MODE_SAVE_BINARY = 3, MODE_CLEAR = 4 };
    long Mode;
    CMemoryDataInputStream* PLoadStream;
    CDataOutputStream* PSaveStream;
    int Crcs; bool CrcNameEmpty;
    template <class T> void Transfer(T& value);
protected:
    void CheckCRC(const char* name) { ++Crcs; CrcNameEmpty = std::strcmp(name, "") == 0; }
};
template <> void CPersistContext::Transfer<EDialogueLayer>(EDialogueLayer& value)
{
    CheckCRC("");
    CPersistTraits<EDialogueLayer> traits;
    switch (Mode) {
    case MODE_LOAD_BINARY:
        traits.TransferBinaryIn(*PLoadStream, value);
        break;
    case MODE_SAVE_BINARY:
        traits.TransferBinaryOut(*PSaveStream, value);
        break;
    }
}

int main()
{
    CMemoryDataInputStream in; in.Next = 7;
    CDataOutputStream out; out.Writes = 0; out.Last = -1;
    CPersistContext context; context.PLoadStream = &in; context.PSaveStream = &out; context.Crcs = 0; context.CrcNameEmpty = false;
    EDialogueLayer value = EDIALOGUELAYER_A;
    context.Mode = CPersistContext::MODE_LOAD_BINARY;
    context.Transfer(value);
    if (value != EDIALOGUELAYER_B || g_reads != 1 || out.Writes != 0 || context.Crcs != 1 || !context.CrcNameEmpty) { std::printf("FAIL load\n"); return 1; }
    context.Mode = CPersistContext::MODE_SAVE_BINARY;
    context.Transfer(value);
    if (out.Writes != 1 || out.Last != 7 || g_reads != 1 || context.Crcs != 2) { std::printf("FAIL save\n"); return 1; }
    context.Mode = CPersistContext::MODE_LOAD_TEXT;
    context.Transfer(value);
    if (out.Writes != 1 || g_reads != 1 || context.Crcs != 3 || value != EDIALOGUELAYER_B) { std::printf("FAIL other\n"); return 1; }
    std::printf("UPDATE_SHADOW_SCENE_479187_TEST PASS\n");
    return 0;
}
