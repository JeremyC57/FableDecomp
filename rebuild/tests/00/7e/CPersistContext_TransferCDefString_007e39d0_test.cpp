// Behaviour test for CPersistContext::Transfer<CDefString> @ 0x007E39D0: the five persist modes through a copy of the model
// (reset to the default, binary save writes TablePos as an SLONG, binary load goes through CDefString::TransferBinaryIn,
// text save/load hand a wrapper of the value to the text transfer, text load applies the default first when asked).
#include <stdio.h>
#include <string.h>
static const char* g_lastCRC = 0;
static int g_written = 0; static long g_writtenValue = 0;
class CDataOutputStream { public: void WriteSLONG(long v) { g_written++; g_writtenValue = v; } };
class CDataInputStream { public: long Next; };
class CMemoryDataInputStream : public CDataInputStream { };
class CDefString { public:
    long TablePos;
    void TransferBinaryIn(CDataInputStream& stream) { TablePos = stream.Next; }
    void TransferBinaryOut(CDataOutputStream& stream) const { stream.WriteSLONG(TablePos); }
};
template <class T> class CPersistTraits;
template <> class CPersistTraits<CDefString> { public:
    void TransferBinaryIn(CMemoryDataInputStream& stream, CDefString& value) { value.TransferBinaryIn(stream); }
    void TransferBinaryOut(CDataOutputStream& stream, const CDefString& value) { value.TransferBinaryOut(stream); }
};
class CPersistContext;
class CPersistContext_TransferableComponentGenericType { public: virtual void TransferOut(CPersistContext&) = 0; };
template <class X> class CPersistContext_TransferableComponent : public CPersistContext_TransferableComponentGenericType { public:
    X* Object; CPersistContext_TransferableComponent(X& object) : Object(&object) { } virtual void TransferOut(CPersistContext&) { } };
static void* g_textObject = 0; static int g_saveText = 0, g_loadText = 0;
class CPersistContext { public:
    enum EMode { MODE_LOAD_TEXT, MODE_SAVE_TEXT, MODE_LOAD_BINARY, MODE_SAVE_BINARY, MODE_RESET };
    bool UseDefaults; EMode Mode; CMemoryDataInputStream* PLoadStream; CDataOutputStream* PSaveStream;
    void CheckCRC(const char* name) { g_lastCRC = name; }
    void TransferObjectSaveText(const char*, CPersistContext_TransferableComponentGenericType& item)
    { g_saveText++; g_textObject = static_cast<CPersistContext_TransferableComponent<CDefString>&>(item).Object; }
    void TransferObjectLoadText(const char*, CPersistContext_TransferableComponentGenericType& item)
    { g_loadText++; g_textObject = static_cast<CPersistContext_TransferableComponent<CDefString>&>(item).Object; }
    template <class T> void Transfer(const char* name, T& value, const T& defaultValue)
    {
        CheckCRC(name);
        switch (Mode) {
        case MODE_RESET: value = defaultValue; break;
        case MODE_SAVE_BINARY: { CPersistTraits<T> traits; traits.TransferBinaryOut(*PSaveStream, value); break; }
        case MODE_LOAD_BINARY: { CPersistTraits<T> traits; traits.TransferBinaryIn(*PLoadStream, value); break; }
        case MODE_SAVE_TEXT: { CPersistContext_TransferableComponent<T> item(value); TransferObjectSaveText(name, item); break; }
        case MODE_LOAD_TEXT: {
            if (UseDefaults) value = defaultValue;
            CPersistContext_TransferableComponent<T> item(value);
            TransferObjectLoadText(name, item);
            break;
        }
        default: break;
        }
    }
};
static int fail(int line) { printf("007e39d0_TEST FAIL\n"); printf("line %d\n", line); return 1; }
int main()
{
    CDataOutputStream out; CMemoryDataInputStream in; in.Next = 42;
    CPersistContext c; c.UseDefaults = false; c.PLoadStream = &in; c.PSaveStream = &out;
    CDefString def; def.TablePos = 7; CDefString v; v.TablePos = 99;
    c.Mode = CPersistContext::MODE_RESET; c.Transfer("Key", v, def);
    if (v.TablePos != 7 || strcmp(g_lastCRC, "Key") != 0) return fail(__LINE__);
    v.TablePos = 99; c.Mode = CPersistContext::MODE_SAVE_BINARY; c.Transfer("Key2", v, def);
    if (g_written != 1 || g_writtenValue != 99 || v.TablePos != 99) return fail(__LINE__);
    c.Mode = CPersistContext::MODE_LOAD_BINARY; c.Transfer("Key3", v, def);
    if (v.TablePos != 42 || g_written != 1) return fail(__LINE__);
    c.Mode = CPersistContext::MODE_SAVE_TEXT; c.Transfer("Key4", v, def);
    if (g_saveText != 1 || g_textObject != &v || v.TablePos != 42) return fail(__LINE__);
    c.Mode = CPersistContext::MODE_LOAD_TEXT; c.Transfer("Key5", v, def);
    if (g_loadText != 1 || v.TablePos != 42) return fail(__LINE__);
    c.UseDefaults = true; c.Transfer("Key6", v, def);
    if (g_loadText != 2 || v.TablePos != 7 || strcmp(g_lastCRC, "Key6") != 0) return fail(__LINE__);
    printf("007e39d0_TEST PASS\n");
    return 0;
}