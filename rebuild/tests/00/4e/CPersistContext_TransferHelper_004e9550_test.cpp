// Behaviour test for 004e9550: src/004e9550.cpp copied verbatim (shared headers expanded inline), callees stubbed below so calls
// and arguments are observable. CPersistContext::TransferHelper(CObjectAugmentationParticleSet* value)
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CPersistContext::TransferHelper(CObjectAugmentationParticleSet* value) -- persist one particle set: CRC-check the empty key, then run CPersistTraits<CObjectAugmentationParticleSet> in the context's direction (mode 2 load, mode 3 save)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004E9550, length 77 bytes (`ret 4`). Callees: CPersistContext::CheckCRC
// @ 0x00404500 with "" @ 0x0122D70E, the traits constructor @ 0x004E9627 (manifest "CDateAndTime"), TransferBinaryIn
// @ 0x004E9642 / TransferBinaryOut @ 0x004E959D, the traits destructor @ 0x004E25F1. The manifest calls it
// CObjectAugmentationParticleSet::TransferHelper, but `this` is the persist context (mode at +0x18, input stream +0x24,
// output stream +0x28, as in the landed CPersistContext::TransferBinary_00467062); the 0x2C-byte frame is the traits
// object (it holds a default CObjectAugmentationParticleSet, which is 0x2C bytes in retail). Frame-pointer size TU:
// #pragma optimize("y", off) + ("s", on).
// Shape: the switch arms RETURN (the traits destructor still runs at each exit): VC7.1 then forms the traits `this`
// once above the first `je` and the default arm falls straight into the destructor call; with `break` every arm re-forms
// `lea ecx,[ebp-2Ch]` (+3 bytes).
// Leaf: TransferHelper
#pragma optimize("y", off)
#pragma optimize("s", on)
class CDataOutputStream;
class CMemoryDataInputStream;
class CObjectAugmentationParticleSet;

class CPersistTraits_ParticleSet {                 // CPersistTraits<CObjectAugmentationParticleSet>, 0x2C bytes
public:
    unsigned char _body[0x2C];
    CPersistTraits_ParticleSet();                  // 0x004E9627
    ~CPersistTraits_ParticleSet();                 // 0x004E25F1
    void TransferBinaryOut(CDataOutputStream* stream, CObjectAugmentationParticleSet* value);     // 0x004E959D
    void TransferBinaryIn(CMemoryDataInputStream* stream, CObjectAugmentationParticleSet* value); // 0x004E9642
};

class CPersistContext {
public:
    unsigned char           _pad00[0x18];
    long                    Mode;                  // +0x18 (2 = load)
    unsigned char           _pad1c[0x08];
    CMemoryDataInputStream* InputStream;           // +0x24
    CDataOutputStream*      OutputStream;          // +0x28
    void CheckCRC(const char* name);               // 0x00404500
    void TransferHelper(CObjectAugmentationParticleSet* value);
};

void CPersistContext::TransferHelper(CObjectAugmentationParticleSet* value)
{
    CheckCRC("");
    CPersistTraits_ParticleSet traits;
    switch (Mode) {
    case 2:
        traits.TransferBinaryIn(InputStream, value);
        return;
    case 3:
        traits.TransferBinaryOut(OutputStream, value);
        return;
    }
}

// ---- stubs and driver ----
static int g_ctor = 0, g_dtor = 0, g_in = 0, g_out = 0, g_crc = 0; static const char* g_crcName = 0;
static void* g_stream = 0; static void* g_value = 0; static void* g_traitsThis[4]; static int g_nThis = 0;
CPersistTraits_ParticleSet::CPersistTraits_ParticleSet() { ++g_ctor; g_traitsThis[g_nThis++ & 3] = this; }
CPersistTraits_ParticleSet::~CPersistTraits_ParticleSet() { ++g_dtor; CHECK(this == g_traitsThis[(g_nThis - 1) & 3]); }
void CPersistTraits_ParticleSet::TransferBinaryOut(CDataOutputStream* stream, CObjectAugmentationParticleSet* value)
{ ++g_out; g_stream = stream; g_value = value; CHECK(this == g_traitsThis[(g_nThis - 1) & 3]); }
void CPersistTraits_ParticleSet::TransferBinaryIn(CMemoryDataInputStream* stream, CObjectAugmentationParticleSet* value)
{ ++g_in; g_stream = stream; g_value = value; CHECK(this == g_traitsThis[(g_nThis - 1) & 3]); }
void CPersistContext::CheckCRC(const char* name) { ++g_crc; g_crcName = name; CHECK(g_ctor == g_dtor); }
int main()
{
    CPersistContext c;
    c.InputStream = (CMemoryDataInputStream*)0x1111; c.OutputStream = (CDataOutputStream*)0x2222;
    CObjectAugmentationParticleSet* v = (CObjectAugmentationParticleSet*)0x3333;
    c.Mode = 2; c.TransferHelper(v);
    CHECK(g_crc == 1 && g_crcName && g_crcName[0] == 0 && g_in == 1 && g_out == 0 && g_stream == (void*)0x1111 && g_value == v);
    CHECK(g_ctor == 1 && g_dtor == 1);
    c.Mode = 3; c.TransferHelper(v);
    CHECK(g_in == 1 && g_out == 1 && g_stream == (void*)0x2222 && g_ctor == 2 && g_dtor == 2);
    c.Mode = 1; c.TransferHelper(v);
    CHECK(g_in == 1 && g_out == 1 && g_crc == 3 && g_ctor == 3 && g_dtor == 3);
    c.Mode = 4; c.TransferHelper(v);
    CHECK(g_in == 1 && g_out == 1 && g_ctor == 4 && g_dtor == 4);
    if (fails == 0) printf("004e9550_TEST PASS\n");
    return fails;
}
