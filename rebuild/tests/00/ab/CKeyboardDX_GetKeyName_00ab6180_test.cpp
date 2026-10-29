// Behaviour test for 00ab6180: body copied from src/00ab6180.cpp (CKeyboardDX::GetKeyName), the DirectInput device and CWideString stubbed.
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
struct SDIPropHeader { unsigned long dwSize; unsigned long dwHeaderSize; unsigned long dwObj; unsigned long dwHow; };
struct SDIPropString { SDIPropHeader diph; unsigned short wsz[260]; };
static int g_getProperty = 0; static const void* g_guid = 0; static long g_result = 0; static SDIPropHeader g_seenHeader;
struct IDirectInputDevice8_Retail {
    virtual long __stdcall QueryInterface(void* iid, void** out) { return 0; }
    virtual unsigned long __stdcall AddRef() { return 0; }
    virtual unsigned long __stdcall Release() { return 0; }
    virtual long __stdcall GetCapabilities(void* caps) { return 0; }
    virtual long __stdcall EnumObjects(void* callback, void* ref, unsigned long flags) { return 0; }
    virtual long __stdcall GetProperty(const void* guid, void* header)
    {
        ++g_getProperty; g_guid = guid; g_seenHeader = *(SDIPropHeader*)header;
        unsigned short* wsz = ((SDIPropString*)header)->wsz; wsz[0] = 'A'; wsz[1] = 'B'; wsz[2] = 0;
        return g_result;
    }
    virtual long __stdcall SetProperty(const void* guid, const void* header) { return 0; }
};
static int g_assigns = 0;
class CWideString { public:
    void* PStringData;
    const CWideString& operator=(const unsigned short* text) { ++g_assigns; PStringData = (void*)text; return *this; }
};
class CKeyboardDX {
public:
    unsigned char               _pad[0x3418];
    IDirectInputDevice8_Retail* DKeyboard;
    unsigned char               KeyStates[0x100];
    unsigned short              KeyMap[0x7a];
    bool GetKeyName(long key, CWideString& name);
};
bool CKeyboardDX::GetKeyName(long key, CWideString& name)
{
    SDIPropString prop;
    prop.diph.dwSize = sizeof(SDIPropString);
    prop.diph.dwHeaderSize = sizeof(SDIPropHeader);
    prop.diph.dwObj = KeyMap[key];
    prop.diph.dwHow = 1;
    if (DKeyboard->GetProperty((const void*)20, &prop.diph) >= 0) {
        name = prop.wsz;
        return true;
    }
    return false;
}
int main()
{
    static CKeyboardDX kb; memset(&kb, 0, sizeof(kb)); IDirectInputDevice8_Retail dev; kb.DKeyboard = &dev; kb.KeyMap[5] = 0x1234;
    CWideString name; name.PStringData = 0;
    CHECK(sizeof(SDIPropString) == 0x218 && sizeof(SDIPropHeader) == 16);
    // 1. the property query succeeds: the name is the returned wide string
    g_result = 0;
    CHECK(kb.GetKeyName(5, name) == true);
    CHECK(g_getProperty == 1 && g_guid == (const void*)20 && g_seenHeader.dwSize == 0x218 && g_seenHeader.dwHeaderSize == 16 && g_seenHeader.dwObj == 0x1234 && g_seenHeader.dwHow == 1);
    CHECK(g_assigns == 1 && name.PStringData != 0 && ((unsigned short*)name.PStringData)[0] == 'A' && ((unsigned short*)name.PStringData)[1] == 'B');
    // 2. the query fails (negative HRESULT): false, name untouched
    g_result = -1; name.PStringData = 0;
    CHECK(kb.GetKeyName(5, name) == false && g_getProperty == 2 && g_assigns == 1 && name.PStringData == 0);
    if (fails == 0) printf("00ab6180_TEST PASS\n"); else printf("00ab6180_TEST FAIL\n");
    return fails;
}