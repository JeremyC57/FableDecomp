// Behaviour test for 006c9a10: src/006c9a10.cpp copied verbatim, callees stubbed below so calls and arguments are observable.
// CPersistContext::Transfer<CIntelligentPointer<CThing>>(const char* name, CIntelligentPointer<CThing>& value, const CIntelligentPointer<CThing>& defaultValue)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CPersistContext::Transfer<CIntelligentPointer<CThing>>(const char* name, CIntelligentPointer<CThing>& value, const CIntelligentPointer<CThing>& defaultValue) -- persist one thing pointer: CRC-check the key, then by mode reset to the default, binary save / load through CPersistTraits, text save, or text load (seeding the default first when asked)
// Leaf: Transfer
// Retail: Fable.exe (TLC, VC7.1) @ 0x006C9A10, 264 bytes + 20-byte jump table, `ret 0Ch`. Called from 0x006C9CB5.
// Callees: CPersistContext::CheckCRC 0x00404500, CBaseIntelligentPointer::CBaseIntelligentPointer() 0x00A01B10,
//   ~CBaseIntelligentPointer 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90,
//   CIntelligentPointer<CThing>::operator= 0x004A9760 (out-of-line copy, catalogued for CThingCreatureBase),
//   CPersistTraits<CIntelligentPointer<CThing>>::TransferBinaryOut 0x006C52D0 / TransferBinaryIn 0x006C8780,
//   TransferObjectSaveText 0x009BAD60, TransferObjectLoadText 0x009BBA40 (wrapper vtable 0x0125E9D0).
// Shape: the closed-enum switch template of the landed enum instantiations (0x00EB65A0): `default: __assume(0)`, case
//   bodies in source order 4,3,2,1,0. The traits object (`CIntelligentPointer<CThing> DefaultVal`, 8 bytes: out-of-line base
//   ctor, then the derived vtable 0x01238C6C) is built once before the jump and destroyed on every exit through the
//   implicit derived destructor (base destructor only). The reset arm is the explicit GetPItem/SetPItem copy and the
//   text-load arm calls the out-of-line operator= 0x004A9760; that arm ends in `return` (not `break`), which keeps the
//   shrink-wrapped ebx saved across the traits destructor (`pop ebx` after it, as retail).
class CBaseObject;
class CDataOutputStream;
class CMemoryDataInputStream;

class CBaseIntelligentPointer {
public:
    CBaseIntelligentPointer();                         // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                // 0x00A01C10
    CBaseObject* GetPItem() const;                     // 0x00A01B50
    void SetPItem(CBaseObject* item);                  // 0x00A01B90
    CBaseObject* PItem;                                // +4
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {   // vtable 0x01238C6C
public:
    CIntelligentPointer& operator=(const CIntelligentPointer& other);   // out-of-line copy 0x004A9760
};
class CThing;

template <class T> struct CPersistTraits {
    T DefaultVal;
    void TransferBinaryOut(CDataOutputStream* stream, const T& value);      // 0x006C52D0 for this T
    void TransferBinaryIn(CMemoryDataInputStream* stream, T& value);        // 0x006C8780 for this T
};

struct CPersistContext_TransferableObjectBase {
    virtual void TransferIn();
};
template <class T> struct CPersistContext_TransferableComponent : CPersistContext_TransferableObjectBase {   // vtable 0x0125E9D0
    T* Value;
    CPersistContext_TransferableComponent(T& value) : Value(&value) {}
    virtual void TransferIn();
};

class CPersistContext { public:
    unsigned char           _pad00[0x14];
    bool                    UseDefaults;          // +0x14
    unsigned char           _pad15[3];
    long                    Mode;                 // +0x18
    unsigned char           _pad1c[8];
    CMemoryDataInputStream* PLoadStream;          // +0x24
    CDataOutputStream*      PSaveStream;          // +0x28
    void CheckCRC(const char* name);              // 0x00404500
    void TransferObjectSaveText(const char* name, CPersistContext_TransferableObjectBase& item);   // 0x009BAD60
    void TransferObjectLoadText(const char* name, CPersistContext_TransferableObjectBase& item);   // 0x009BBA40
    template <class T> void Transfer(const char* name, T& value, const T& defaultValue);
};

template <class T>
void CPersistContext::Transfer(const char* name, T& value, const T& defaultValue)
{
    CheckCRC(name);
    CPersistTraits<T> traits;
    switch (Mode) {
    case 4: {
        CBaseObject* item = defaultValue.GetPItem();
        value.SetPItem(item);
        break;
    }
    case 3:
        traits.TransferBinaryOut(PSaveStream, value);
        break;
    case 2:
        traits.TransferBinaryIn(PLoadStream, value);
        break;
    case 1: {
        CPersistContext_TransferableComponent<T> item(value);
        TransferObjectSaveText(name, item);
        break;
    }
    case 0: {
        if (UseDefaults)
            value = defaultValue;
        CPersistContext_TransferableComponent<T> item(value);
        TransferObjectLoadText(name, item);
        return;
    }
    default:
        __assume(0);
    }
}

template void CPersistContext::Transfer<CIntelligentPointer<CThing> >(const char* name, CIntelligentPointer<CThing>& value, const CIntelligentPointer<CThing>& defaultValue);

// ---- stubs ----
static char g_log[512];
static int g_step = 0;
static void Log(const char* s) { char b[64]; _snprintf(b, sizeof b, "%d:%s;", ++g_step, s); strcat(g_log, b); }
static int g_live = 0;
CBaseIntelligentPointer::CBaseIntelligentPointer() { PItem = 0; ++g_live; }
CBaseIntelligentPointer::~CBaseIntelligentPointer() { --g_live; }
CBaseObject* CBaseIntelligentPointer::GetPItem() const { return PItem; }
void CBaseIntelligentPointer::SetPItem(CBaseObject* item) { PItem = item; }
template <> CIntelligentPointer<CThing>& CIntelligentPointer<CThing>::operator=(const CIntelligentPointer<CThing>& other)
{ Log("Assign"); SetPItem(other.GetPItem()); return *this; }
static const char* g_crc = 0;
void CPersistContext::CheckCRC(const char* name) { Log("CRC"); g_crc = name; }
static const void* g_out = 0; static const void* g_in = 0; static const void* g_stream = 0;
template <> void CPersistTraits<CIntelligentPointer<CThing> >::TransferBinaryOut(CDataOutputStream* stream, const CIntelligentPointer<CThing>& value)
{ Log("BinOut"); g_stream = stream; g_out = &value; }
template <> void CPersistTraits<CIntelligentPointer<CThing> >::TransferBinaryIn(CMemoryDataInputStream* stream, CIntelligentPointer<CThing>& value)
{ Log("BinIn"); g_stream = stream; g_in = &value; }
void CPersistContext_TransferableObjectBase::TransferIn() {}
template <> void CPersistContext_TransferableComponent<CIntelligentPointer<CThing> >::TransferIn() {}
static const char* g_textName = 0; static const void* g_textValue = 0; static CBaseObject* g_itemAtLoad = 0;
void CPersistContext::TransferObjectSaveText(const char* name, CPersistContext_TransferableObjectBase& item)
{ Log("SaveText"); g_textName = name; g_textValue = ((CPersistContext_TransferableComponent<CIntelligentPointer<CThing> >&)item).Value; }
void CPersistContext::TransferObjectLoadText(const char* name, CPersistContext_TransferableObjectBase& item)
{
    Log("LoadText"); g_textName = name;
    CIntelligentPointer<CThing>* v = ((CPersistContext_TransferableComponent<CIntelligentPointer<CThing> >&)item).Value;
    g_textValue = v; g_itemAtLoad = v->PItem;
}
static void Reset() { g_log[0] = 0; g_step = 0; g_textName = 0; g_textValue = 0; g_out = g_in = 0; }

int main()
{
    CPersistContext ctx; memset(&ctx, 0, sizeof ctx);
    ctx.PLoadStream = (CMemoryDataInputStream*)0x111; ctx.PSaveStream = (CDataOutputStream*)0x222;
    CIntelligentPointer<CThing> value, def;
    value.PItem = (CBaseObject*)0x10; def.PItem = (CBaseObject*)0x20;
    int live = g_live;
    // mode 4: reset to the default (inline copy, no out-of-line operator=)
    { Reset(); ctx.Mode = 4; ctx.Transfer("k4", value, def);
      CHECK(strcmp(g_log, "1:CRC;") == 0 && g_crc != 0 && strcmp(g_crc, "k4") == 0 && value.PItem == (CBaseObject*)0x20 && g_live == live); }
    // mode 3: binary save
    { Reset(); value.PItem = (CBaseObject*)0x10; ctx.Mode = 3; ctx.Transfer("k3", value, def);
      CHECK(strcmp(g_log, "1:CRC;2:BinOut;") == 0 && g_out == &value && g_stream == (void*)0x222 && g_live == live); }
    // mode 2: binary load
    { Reset(); ctx.Mode = 2; ctx.Transfer("k2", value, def);
      CHECK(strcmp(g_log, "1:CRC;2:BinIn;") == 0 && g_in == &value && g_stream == (void*)0x111 && value.PItem == (CBaseObject*)0x10); }
    // mode 1: text save
    { Reset(); ctx.Mode = 1; ctx.Transfer("k1", value, def);
      CHECK(strcmp(g_log, "1:CRC;2:SaveText;") == 0 && strcmp(g_textName, "k1") == 0 && g_textValue == &value && g_live == live); }
    // mode 0 without defaults: text load only
    { Reset(); ctx.Mode = 0; ctx.UseDefaults = false; ctx.Transfer("k0", value, def);
      CHECK(strcmp(g_log, "1:CRC;2:LoadText;") == 0 && g_textValue == &value && g_itemAtLoad == (CBaseObject*)0x10); }
    // mode 0 with defaults: seed the default first through operator=
    { Reset(); ctx.UseDefaults = true; ctx.Transfer("k0", value, def);
      CHECK(strcmp(g_log, "1:CRC;2:Assign;3:LoadText;") == 0 && g_itemAtLoad == (CBaseObject*)0x20 && g_live == live); }
    if (fails == 0) printf("006c9a10_TEST PASS\n");
    return fails;
}