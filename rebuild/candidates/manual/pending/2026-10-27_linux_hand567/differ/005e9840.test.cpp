// Behaviour test for CPersistContext::TransferVector<unsigned long> (0x005E9840): the mode switch dispatch.
#include <cstdio>

struct ULongVector { unsigned long Data[4]; int Count; void clear() { Count = 0; } };
static int g_calls[5];
static const char* g_name;
struct CDataOutputStream { };
struct CMemoryDataInputStream { };
void GFSerialiseVectorBinaryOut(CDataOutputStream&, const ULongVector&) { ++g_calls[3]; }
void GFSerialiseVectorBinaryIn(CMemoryDataInputStream&, ULongVector&) { ++g_calls[2]; }
struct CPersistContext_TransferableVectorGenericType {
    CPersistContext_TransferableVectorGenericType() { }
    virtual ~CPersistContext_TransferableVectorGenericType() { }
    virtual int Size() const = 0;
};
struct CPersistContext_TransferableVector : public CPersistContext_TransferableVectorGenericType {
    ULongVector& TransVector;
    CPersistContext_TransferableVector(ULongVector& v) : TransVector(v) { }
    int Size() const { return TransVector.Count; }
};
struct CPersistContext {
    enum EMode { MODE_LOAD_TEXT, MODE_SAVE_TEXT, MODE_LOAD_BINARY, MODE_SAVE_BINARY, MODE_RESET };
    EMode Mode;
    CDataOutputStream* PSaveStream;
    CMemoryDataInputStream* PLoadStream;
    int Checked;
    void CheckCRC(const char* name) { ++Checked; g_name = name; }
    void TransferVectorSaveText(const char*, CPersistContext_TransferableVectorGenericType& item) { if (item.Size() == 3) ++g_calls[1]; }
    void TransferVectorLoadText(const char*, CPersistContext_TransferableVectorGenericType& item) { if (item.Size() == 3) ++g_calls[0]; }
    void TransferVector(const char* name, ULongVector& value)
    {
        CheckCRC(name);
        switch (Mode) {
        case MODE_RESET: value.clear(); ++g_calls[4]; break;
        case MODE_SAVE_BINARY: GFSerialiseVectorBinaryOut(*PSaveStream, value); break;
        case MODE_LOAD_BINARY: GFSerialiseVectorBinaryIn(*PLoadStream, value); break;
        case MODE_SAVE_TEXT: { CPersistContext_TransferableVector item(value); TransferVectorSaveText(name, item); break; }
        case MODE_LOAD_TEXT: { CPersistContext_TransferableVector item(value); TransferVectorLoadText(name, item); break; }
        }
    }
};
int main()
{
    CDataOutputStream out; CMemoryDataInputStream in;
    CPersistContext pc; pc.PSaveStream = &out; pc.PLoadStream = &in; pc.Checked = 0;
    ULongVector v; v.Count = 3;
    for (int m = 0; m < 4; ++m) {
        pc.Mode = (CPersistContext::EMode)m;
        pc.TransferVector("Key", v);
        if (g_calls[m] != 1) { std::printf("005e9840_TEST FAIL mode %d\n", m); return 1; }
    }
    pc.Mode = CPersistContext::MODE_RESET;
    pc.TransferVector("Key", v);
    if (g_calls[4] != 1 || v.Count != 0 || pc.Checked != 5 || g_name[0] != 'K') { std::printf("005e9840_TEST FAIL reset\n"); return 1; }
    std::printf("005e9840_TEST PASS\n");
    return 0;
}
