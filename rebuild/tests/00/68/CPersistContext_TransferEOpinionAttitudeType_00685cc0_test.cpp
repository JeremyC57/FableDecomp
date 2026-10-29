// Behaviour test for CPersistContext::Transfer<EOpinionAttitudeType> @ 0x00685cc0: the five persist modes through a copy of the model.
#include <stdio.h>
#include <string.h>
enum EOpinionAttitudeType { OPINION_ATTITUDE_A = 3, OPINION_ATTITUDE_B = 7, OPINION_ATTITUDE_C = 11 };   // values for the test only
typedef EOpinionAttitudeType T;
static const char* g_lastCRC = 0;
static int g_written = 0;
static double g_writtenValue = 0;
static const char* g_writer = 0;
class CDataOutputStream { public:
    void WriteEBOOL(bool v) { g_written++; g_writer = "WriteEBOOL"; g_writtenValue = v ? 1 : 0; }
    void WriteSLONG(int v) { g_written++; g_writer = "WriteSLONG"; g_writtenValue = (double)(unsigned int)v; }
    void WriteFloat(float v) { g_written++; g_writer = "WriteFloat"; g_writtenValue = v; }
};
class CMemoryDataInputStream { public: int Next; };
template <class X> class CPersistTraits { public:
    X DefaultVal;
    void TransferBinaryIn(CMemoryDataInputStream& stream, X& value) { value = (X)stream.Next; }
    void TransferBinaryOut(CDataOutputStream& stream, X value) { stream.WriteSLONG((int)value); }
};
class CPersistContext;
class CPersistContext_TransferableComponentGenericType { public:
    virtual void TransferOut(CPersistContext&) = 0;
};
template <class X> class CPersistContext_TransferableComponent : public CPersistContext_TransferableComponentGenericType { public:
    X* Object;
    CPersistContext_TransferableComponent(X& object) : Object(&object) {}
    virtual void TransferOut(CPersistContext&) {}
};
static void* g_textObject = 0;
static int g_saveText = 0, g_loadText = 0;
class CPersistContext { public:
    enum EMode { MODE_LOAD_TEXT, MODE_SAVE_TEXT, MODE_LOAD_BINARY, MODE_SAVE_BINARY, MODE_RESET };
    bool UseDefaults;
    EMode Mode;
    CMemoryDataInputStream* PLoadStream;
    CDataOutputStream* PSaveStream;
    void CheckCRC(const char* name) { g_lastCRC = name; }
    void TransferObjectSaveText(const char*, CPersistContext_TransferableComponentGenericType& item)
    { g_saveText++; g_textObject = static_cast<CPersistContext_TransferableComponent<T>&>(item).Object; }
    void TransferObjectLoadText(const char*, CPersistContext_TransferableComponentGenericType& item)
    { g_loadText++; g_textObject = static_cast<CPersistContext_TransferableComponent<T>&>(item).Object; }
    template <class X> void Transfer(const char* name, X& value, const X& defaultValue)
    {
        CheckCRC(name);
        switch (Mode) {
        case MODE_RESET: value = defaultValue; break;
        case MODE_SAVE_BINARY: { CPersistTraits<X> traits; traits.TransferBinaryOut(*PSaveStream, value); break; }
        case MODE_LOAD_BINARY: { CPersistTraits<X> traits; traits.TransferBinaryIn(*PLoadStream, value); break; }
        case MODE_SAVE_TEXT: { CPersistContext_TransferableComponent<X> item(value); TransferObjectSaveText(name, item); break; }
        case MODE_LOAD_TEXT: {
            if (UseDefaults) value = defaultValue;
            CPersistContext_TransferableComponent<X> item(value);
            TransferObjectLoadText(name, item);
            break;
        }
        default: break;
        }
    }
};
static int fail(int line) { printf("00685cc0_TEST FAIL\n"); printf("line %d\n", line); return 1; }
int main()
{
    CDataOutputStream out; CMemoryDataInputStream in; in.Next = OPINION_ATTITUDE_C;
    CPersistContext c; c.UseDefaults = false; c.PLoadStream = &in; c.PSaveStream = &out;
    const T def = OPINION_ATTITUDE_B; T v = OPINION_ATTITUDE_A;
    c.Mode = CPersistContext::MODE_RESET; c.Transfer("Key", v, def);
    if (!(v == def) || g_lastCRC == 0 || strcmp(g_lastCRC, "Key") != 0) return fail(__LINE__);
    v = OPINION_ATTITUDE_C; c.Mode = CPersistContext::MODE_SAVE_BINARY; c.Transfer("Key2", v, def);
    if (g_written != 1 || strcmp(g_writer, "WriteSLONG") != 0 || !(v == OPINION_ATTITUDE_C) || g_writtenValue != 11.0) return fail(__LINE__);
    v = def; c.Mode = CPersistContext::MODE_LOAD_BINARY; c.Transfer("Key3", v, def);
    if (!(v == OPINION_ATTITUDE_C) || g_written != 1) return fail(__LINE__);
    c.Mode = CPersistContext::MODE_SAVE_TEXT; c.Transfer("Key4", v, def);
    if (g_saveText != 1 || g_textObject != &v || !(v == OPINION_ATTITUDE_C)) return fail(__LINE__);
    c.Mode = CPersistContext::MODE_LOAD_TEXT; c.Transfer("Key5", v, def);
    if (g_loadText != 1 || g_textObject != &v || !(v == OPINION_ATTITUDE_C)) return fail(__LINE__);
    c.UseDefaults = true; c.Transfer("Key6", v, def);
    if (g_loadText != 2 || !(v == def) || strcmp(g_lastCRC, "Key6") != 0) return fail(__LINE__);
    printf("00685cc0_TEST PASS\n");
    return 0;
}