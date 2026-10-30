// Behaviour test for CPersistContext::Transfer<unsigned char> (retail 00405a60): the CRC check runs first, then the mode picks
// text load (default seeded only when UseDefaults), text save, binary load, binary save (WriteSLONG) or reset.
#include <cstdio>
typedef unsigned char T;
static int g_crc, g_save, g_load, g_binIn, g_binOut; static T g_written;
struct Stream { };
class CPersistContext { public:
    enum EMode { MODE_LOAD_TEXT, MODE_SAVE_TEXT, MODE_LOAD_BINARY, MODE_SAVE_BINARY, MODE_RESET };
    bool UseDefaults; EMode Mode; Stream In, Out;
    void CheckCRC(const char*) { ++g_crc; }
    void TransferObjectSaveText(const char*, T* obj) { ++g_save; g_written = *obj; }
    void TransferObjectLoadText(const char*, T* obj) { ++g_load; *obj += 1; }
    void Transfer(const char* name, T& value, const T& defaultValue)
    {
        CheckCRC(name);
        switch (Mode) {
        case MODE_RESET: value = defaultValue; break;
        case MODE_SAVE_BINARY: ++g_binOut; g_written = value; break;
        case MODE_LOAD_BINARY: ++g_binIn; value = (T)42; break;
        case MODE_SAVE_TEXT: TransferObjectSaveText(name, &value); break;
        case MODE_LOAD_TEXT: if (UseDefaults) value = defaultValue; TransferObjectLoadText(name, &value); break;
        }
    }
};
int main()
{
    CPersistContext c; const T def = (T)7; T v = (T)200; bool ok = true;
    c.UseDefaults = false;
    c.Mode = CPersistContext::MODE_RESET; c.Transfer("k", v, def); ok = ok && v == def;
    v = (T)200; c.Mode = CPersistContext::MODE_SAVE_BINARY; c.Transfer("k", v, def); ok = ok && g_binOut == 1 && g_written == (T)200;
    c.Mode = CPersistContext::MODE_LOAD_BINARY; c.Transfer("k", v, def); ok = ok && g_binIn == 1 && v == (T)42;
    c.Mode = CPersistContext::MODE_SAVE_TEXT; c.Transfer("k", v, def); ok = ok && g_save == 1 && g_written == (T)42;
    c.Mode = CPersistContext::MODE_LOAD_TEXT; c.Transfer("k", v, def); ok = ok && g_load == 1 && v == (T)43;
    c.UseDefaults = true; c.Transfer("k", v, def); ok = ok && g_load == 2 && v == (T)(def + 1);
    ok = ok && g_crc == 6;
    if (!ok) { std::printf("00405a60_TEST FAIL\n"); return 1; }
    std::printf("00405a60_TEST PASS\n");
    return 0;
}