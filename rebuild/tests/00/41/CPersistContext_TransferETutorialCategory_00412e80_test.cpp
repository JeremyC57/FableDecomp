// Behaviour test for CPersistContext::Transfer<ETutorialCategory> (0x00412E80): the key is CRC-checked first, then each
// persist mode dispatches to exactly one arm -- 0 text load / 1 text save through the {vtable, vector*} wrapper, 2 binary
// load from PLoadStream, 3 binary save to PSaveStream, 4 clears the vector.
#include <stdio.h>
#include <string.h>

// ---- reconstruction under test (verbatim copy of src/00412e80.cpp) ----
// Leaf: Transfer
// Grade: RELOCATION_MATCH 180/180 on the corrected span 0x00412E80+180 (code + embedded jump table; bounds audit) -- catalog row 224 B also held two unlisted 9-byte stubs at 0x00412F40/0x00412F50
// CPersistContext::Transfer<ETutorialCategory>(const char* name, std::vector<ETutorialCategory>& value) -- persist one vector member ("GivenTutorialCategories"): CRC-check the key, then by mode text load / text save / binary load / binary save / reset to empty
// Retail: Fable.exe (TLC, VC7.1) @ 0x00412E80 (the switch table follows the code), `ret 8`. Called from CUserProfileManager::SaveGameState (0x00409FD0) with the key "GivenTutorialCategories".
// Callees: CPersistContext::CheckCRC 0x00404500, CPersistContext::TransferVectorLoadText 0x009BB110 and TransferVectorSaveText
//   0x009BAC80 (both take the CPersistContext_TransferableVector<T> wrapper, vtable 0x0122E044), GFSerialiseVectorBinaryIn<T>
//   0x00412D20 and GFSerialiseVectorBinaryOut<T> 0x0040FD40 (both `__fastcall` stream, vector), and for the reset mode
//   `value.clear()`, emitted out of line in this module (0x00411980).
// Shape: the persist mode (CPersistContext +0x18: 0 text load, 1 text save, 2 binary load, 3 binary save, 4 reset) is a
//   closed enum, so the switch ends in `default: __assume(0)` and VC7.1 emits the bare `jmp [eax*4+table]` with no range
//   check; VC7.1 lays the case bodies out in source order, and retail has them 4,3,2,1,0, so the source lists them in that
//   order. The 8-byte frame is the {vtable, vector*} wrapper shared by the two text arms; the key stays in edi from the
//   CheckCRC push. CPersistContext layout from Ego_r.pdb (0x2C bytes): UseDefaults +0x14, Mode +0x18, PLoadStream +0x24,
//   PSaveStream +0x28. The element type comes from the PDB member that owns the key where one exists.
class CDataOutputStream;
class CMemoryDataInputStream;
enum ETutorialCategory { TUTORIAL_CATEGORY_NONE };

template <class T> struct vector {
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    void clear();                 // 0x00411980 for this T (out of line)
};

void __fastcall GFSerialiseVectorBinaryIn(CMemoryDataInputStream* stream, vector<ETutorialCategory>& value);   // 0x00412D20
void __fastcall GFSerialiseVectorBinaryOut(CDataOutputStream* stream, vector<ETutorialCategory>& value);       // 0x0040FD40

class CPersistContext_TransferableVectorGenericType { public:
    virtual long GetCount();
};
template <class T> struct CPersistContext_TransferableVector : CPersistContext_TransferableVectorGenericType {   // vtable 0x0122E044
    vector<T>* TransVector;
    CPersistContext_TransferableVector(vector<T>& v) : TransVector(&v) {}
    virtual long GetCount();
};

class CPersistContext { public:
    unsigned char           _pad00[0x14];
    bool                    UseDefaults;          // +0x14
    unsigned char           _pad15[3];
    long                    Mode;                 // +0x18
    unsigned char           _pad1c[8];
    CMemoryDataInputStream* PLoadStream;           // +0x24
    CDataOutputStream*      PSaveStream;           // +0x28
    protected: void CheckCRC(const char* name); public:              // 0x00404500
    void TransferVectorSaveText(const char* name, CPersistContext_TransferableVectorGenericType& item);   // 0x009BAC80
    void TransferVectorLoadText(const char* name, CPersistContext_TransferableVectorGenericType& item);   // 0x009BB110
    template <class T> void Transfer(const char* name, vector<T>& value);
};

template <class T>
void CPersistContext::Transfer(const char* name, vector<T>& value)
{
    CheckCRC(name);
    switch (Mode) {
    case 4:
        value.clear();
        break;
    case 3:
        GFSerialiseVectorBinaryOut(PSaveStream, value);
        break;
    case 2:
        GFSerialiseVectorBinaryIn(PLoadStream, value);
        break;
    case 1: {
        CPersistContext_TransferableVector<T> item(value);
        TransferVectorSaveText(name, item);
        break;
    }
    case 0: {
        CPersistContext_TransferableVector<T> item(value);
        TransferVectorLoadText(name, item);
        break;
    }
    default:
        __assume(0);
    }
}

template void CPersistContext::Transfer<ETutorialCategory>(const char* name, vector<ETutorialCategory>& value);

// ---- harness ----
static const char* g_crc;
static const char* g_name;
static int g_which;
static void* g_vec;
static void* g_stream;
static long g_count;

template <class T> void vector<T>::clear() { _M_finish = _M_start; }
long CPersistContext_TransferableVectorGenericType::GetCount() { return -1; }
template <class T> long CPersistContext_TransferableVector<T>::GetCount() { return (long)(TransVector->_M_finish - TransVector->_M_start); }
void CPersistContext::CheckCRC(const char* name) { g_crc = name; }
void CPersistContext::TransferVectorSaveText(const char* name, CPersistContext_TransferableVectorGenericType& item)
{
    g_which = 1; g_name = name; g_count = item.GetCount();
    g_vec = ((CPersistContext_TransferableVector<ETutorialCategory>&)item).TransVector;
}
void CPersistContext::TransferVectorLoadText(const char* name, CPersistContext_TransferableVectorGenericType& item)
{
    g_which = 10; g_name = name; g_count = item.GetCount();
    g_vec = ((CPersistContext_TransferableVector<ETutorialCategory>&)item).TransVector;
}
void __fastcall GFSerialiseVectorBinaryIn(CMemoryDataInputStream* stream, vector<ETutorialCategory>& value) { g_which = 2; g_stream = stream; g_vec = &value; }
void __fastcall GFSerialiseVectorBinaryOut(CDataOutputStream* stream, vector<ETutorialCategory>& value) { g_which = 3; g_stream = stream; g_vec = &value; }

static int g_fail;
static void check(int cond, const char* what) { if (!cond) { printf("FAIL %s\n", what); g_fail = 1; } }

int main()
{
    ETutorialCategory store[3];
    vector<ETutorialCategory> v;
    CPersistContext ctx;
    int mode;
    const char* key = "GivenTutorialCategories";
    memset(&ctx, 0, sizeof(ctx));
    ctx.PLoadStream = (CMemoryDataInputStream*)0x1111;
    ctx.PSaveStream = (CDataOutputStream*)0x2222;
    for (mode = 0; mode <= 4; ++mode) {
        v._M_start = store; v._M_finish = store + 3; v._M_end_of_storage = store + 3;
        g_crc = 0; g_name = 0; g_which = -1; g_vec = 0; g_stream = 0; g_count = 0;
        ctx.Mode = mode;
        ctx.Transfer(key, v);
        check(g_crc == key, "CheckCRC gets the key");
        if (mode == 0) { check(g_which == 10 && g_name == key && g_vec == &v && g_count == 3, "mode 0 text load"); }
        if (mode == 1) { check(g_which == 1 && g_name == key && g_vec == &v && g_count == 3, "mode 1 text save"); }
        if (mode == 2) { check(g_which == 2 && g_stream == (void*)0x1111 && g_vec == &v, "mode 2 binary load"); }
        if (mode == 3) { check(g_which == 3 && g_stream == (void*)0x2222 && g_vec == &v, "mode 3 binary save"); }
        if (mode == 4) { check(g_which == -1 && v._M_finish == v._M_start && v._M_end_of_storage == store + 3, "mode 4 clear"); }
        if (mode != 4) check(v._M_finish == store + 3, "non-reset modes keep the vector");
    }
    if (g_fail) return 1;
    printf("00412e80_TEST PASS\n");
    return 0;
}