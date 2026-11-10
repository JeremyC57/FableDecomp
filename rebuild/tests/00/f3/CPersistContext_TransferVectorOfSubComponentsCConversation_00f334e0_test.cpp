// Behaviour test for 0x00F334E0 CPersistContext::TransferVectorOfSubComponents<CConversation>(const char*, vector&) (self-contained
// C++03 model: CheckCRC(name); clear mode transfers every element; binary save writes the count then transfers every element;
// binary load reads the count, resizes and transfers every element; text load hands a vector wrapper to the shared text reader).
#include <stdio.h>
#include <string.h>
#include <vector>

static int g_fail = 0;
static void Check(bool ok, const char* what) { if (!ok) { printf("FAIL: %s\n", what); g_fail = 1; } }

class CPersistContext;
static int g_transfers = 0;
static int g_lastIndex = -1;

class CConversation {                         // element model: the real class is 0x5C bytes, Transfer is vtable slot 18
public:
    int Index;
    unsigned char Body[0x5C - 8];
    CConversation() : Index(-1) { memset(Body, 0, sizeof(Body)); }
    virtual ~CConversation() { }
    virtual void Transfer(CPersistContext&) { ++g_transfers; g_lastIndex = Index; }
};

class CPersistContext {
public:
    enum EMode { MODE_LOAD_TEXT = 0, MODE_SAVE_TEXT = 1, MODE_LOAD_BINARY = 2, MODE_SAVE_BINARY = 3, MODE_CLEAR = 4 };
    EMode Mode;
    const char* CRCName;
    std::vector<int> Stream;                  // the binary stream as 4-byte longs
    unsigned int ReadPos;
    const char* TextName;
    std::vector<CConversation>* TextVector;
    CPersistContext(EMode mode) : Mode(mode), CRCName(0), ReadPos(0), TextName(0), TextVector(0) { }
    void CheckCRC(const char* name) { CRCName = name; }
    void TransferVectorOfSubComponentsLoadText(const char* name, std::vector<CConversation>& v) { TextName = name; TextVector = &v; }
    void TransferVectorOfSubComponents(const char* name, std::vector<CConversation>& value)
    {
        CheckCRC(name);
        switch (Mode) {
        case MODE_CLEAR: { long count = (long)value.size(); for (long i = 0; i < count; ++i) value[i].Transfer(*this); break; }
        case MODE_SAVE_BINARY: { long count = (long)value.size(); Stream.push_back((int)count); for (long i = 0; i < count; ++i) value[i].Transfer(*this); break; }
        case MODE_LOAD_BINARY: { long count = Stream[ReadPos++]; value.resize(count); for (long i = 0; i < count; ++i) value[i].Transfer(*this); break; }
        case MODE_LOAD_TEXT: TransferVectorOfSubComponentsLoadText(name, value); break;
        default: break;                       // text save never reaches this body (jump-table entry 1 is unused in retail)
        }
    }
};

int main()
{
    static const char key[] = "BookReactions";
    {
        CPersistContext c(CPersistContext::MODE_SAVE_BINARY);
        std::vector<CConversation> v(3); v[2].Index = 42; g_transfers = 0;
        c.TransferVectorOfSubComponents(key, v);
        Check(c.CRCName == key, "checks the CRC of the name");
        Check(c.Stream.size() == 1 && c.Stream[0] == 3 && g_transfers == 3 && g_lastIndex == 42, "binary save writes the count and transfers in order");
    }
    {
        CPersistContext c(CPersistContext::MODE_LOAD_BINARY); c.Stream.push_back(2);
        std::vector<CConversation> v(5); g_transfers = 0;
        c.TransferVectorOfSubComponents(key, v);
        Check(v.size() == 2 && g_transfers == 2 && c.ReadPos == 1, "binary load resizes to the count and transfers each");
    }
    {
        CPersistContext c(CPersistContext::MODE_CLEAR);
        std::vector<CConversation> v(4); g_transfers = 0;
        c.TransferVectorOfSubComponents(key, v);
        Check(v.size() == 4 && g_transfers == 4 && c.Stream.empty(), "clear transfers every element and writes nothing");
    }
    {
        CPersistContext c(CPersistContext::MODE_LOAD_TEXT);
        std::vector<CConversation> v; g_transfers = 0;
        c.TransferVectorOfSubComponents(key, v);
        Check(c.TextName == key && c.TextVector == &v && g_transfers == 0, "text load hands the wrapper to the shared reader");
    }
    if (g_fail) return 1;
    printf("00f334e0_TEST PASS\n");
    return 0;
}