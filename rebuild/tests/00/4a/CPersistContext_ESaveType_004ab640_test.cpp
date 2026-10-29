// Behaviour test for CPersistContext::Transfer<CWorld::ESaveType> @ 0x004ab640: the five persist modes through a copy of the model.
#include <stdio.h>
#include <string.h>
enum ESaveType { SAVE_TYPE_0, SAVE_TYPE_1, SAVE_TYPE_2 };   // stand-in for CWorld::ESaveType (values only)
typedef ESaveType T;
static const char* g_lastCRC = 0;
static int g_written = 0;
static double g_writtenValue = 0;
static const char* g_writer = 0;
class CDataOutputStream { public:
    void WriteEBOOL(bool v) { g_written++; g_writer = "WriteEBOOL"; g_writtenValue = v ? 1 : 0; }
    void WriteSLONG(int v) { g_written++; g_writer = "WriteSLONG"; g_writtenValue = (double)(unsigned int)v; }
    void WriteFloat(float v) { g_written++; g_writer = "WriteFloat"; g_writtenValue = v; }
};
class CMemoryDataInputStream { public: T Next; };
template <class X> class CPersistTraits { public:
    X DefaultVal;
    void TransferBinaryIn(CMemoryDataInputStream& stream, X& value) { value = stream.Next; }
    void TransferBinaryOut(CDataOutputStream& stream, const X& value) { stream.WriteSLONG((int)value); }
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
static int fail(int line) { printf("004ab640_TEST FAIL\n"); printf("line %d\n", line); return 1; }
int main()
{
    CDataOutputStream out; CMemoryDataInputStream in; in.Next = (ESaveType)2;
    CPersistContext c; c.UseDefaults = false; c.PLoadStream = &in; c.PSaveStream = &out;
    const T def = (ESaveType)1; T v = (ESaveType)2;
    c.Mode = CPersistContext::MODE_RESET; c.Transfer("Key", v, def);
    if (!(v == def) || g_lastCRC == 0 || strcmp(g_lastCRC, "Key") != 0) return fail(__LINE__);
    v = (ESaveType)2; c.Mode = CPersistContext::MODE_SAVE_BINARY; c.Transfer("Key2", v, def);
    if (g_written != 1 || strcmp(g_writer, "WriteSLONG") != 0 || !(v == (ESaveType)2)) return fail(__LINE__);
    v = def; c.Mode = CPersistContext::MODE_LOAD_BINARY; c.Transfer("Key3", v, def);
    if (!(v == (ESaveType)2) || g_written != 1) return fail(__LINE__);
    c.Mode = CPersistContext::MODE_SAVE_TEXT; c.Transfer("Key4", v, def);
    if (g_saveText != 1 || g_textObject != &v || !(v == (ESaveType)2)) return fail(__LINE__);
    c.Mode = CPersistContext::MODE_LOAD_TEXT; c.Transfer("Key5", v, def);
    if (g_loadText != 1 || g_textObject != &v || !(v == (ESaveType)2)) return fail(__LINE__);
    c.UseDefaults = true; c.Transfer("Key6", v, def);
    if (g_loadText != 2 || !(v == def) || strcmp(g_lastCRC, "Key6") != 0) return fail(__LINE__);
    printf("004ab640_TEST PASS\n");
    return 0;
}