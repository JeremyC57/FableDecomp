// Portable behaviour test for CPersistTraits<CIntelligentPointer<CThing> >::TransferBinaryIn (0x006C8780): an 8-byte UID read
// through the bounds-checked chunk read; a zero UID leaves the pointer alone; a non-zero UID creates the
// THING_DUMMY_FOR_SERIALISATION thing, stores the UID in its TCI_THING_UID_DUMMY_FOR_SERIALISATION (235) component and points
// the intelligent pointer at it.
#include <cstdio>
#include <cstring>
#ifdef _MSC_VER
typedef __int64 s64; typedef unsigned __int64 u64;
#define U64C(x) x##ui64
#else
typedef long long s64; typedef unsigned long long u64;
#define U64C(x) x##ULL
#endif
struct CMemoryDataInputStream {
    long StreamPos; const unsigned char* SourceChunk; long SourceChunkBytesRemaining;
    void Read(void* buffer, long size)
    {
        s64 next = (s64)(unsigned long)(StreamPos + size);
        if (next < 0 || next > 0x7fffffff) return;
        if (size <= SourceChunkBytesRemaining) { std::memcpy(buffer, SourceChunk, size); SourceChunk += size; SourceChunkBytesRemaining -= size; StreamPos += size; }
    }
    template <class T> T ReadType() { T val = 0; Read(&val, sizeof(T)); return val; }
};
struct CTCThingUIDDummyForSerialisation { u64 ThingUID; };
struct CThing {
    bool UsingDummy; CTCThingUIDDummyForSerialisation Dummy;
    bool GetTC(int id, CTCThingUIDDummyForSerialisation** ptc) { if (id == 235 && UsingDummy) { *ptc = &Dummy; return true; } return false; }
};
static CThing g_created; static int g_creates; static int g_lastIndex;
struct CDefinitionManager { long GetDefGlobalIndexFromName(const char* name) const { return std::strcmp(name, "THING_DUMMY_FOR_SERIALISATION") == 0 ? 77 : -1; } };
static CDefinitionManager g_defs;
struct CTCDNone { static CThing* Create(long index, bool) { ++g_creates; g_lastIndex = index; return &g_created; } };
template <class T> struct CIntelligentPointer { T* Item; CIntelligentPointer() : Item(0) {} CIntelligentPointer(T* p) : Item(p) {} };
template <class T> struct CPersistTraits { void TransferBinaryIn(CMemoryDataInputStream& stream, T& value); };
template <>
void CPersistTraits<CIntelligentPointer<CThing> >::TransferBinaryIn(CMemoryDataInputStream& stream, CIntelligentPointer<CThing>& value)
{
    u64 uid = stream.ReadType<u64>();
    if (uid > 0) {
        CThing* thing = CTCDNone::Create(g_defs.GetDefGlobalIndexFromName("THING_DUMMY_FOR_SERIALISATION"), true);
        CTCThingUIDDummyForSerialisation* dummy = 0;
        thing->GetTC(235, &dummy);
        dummy->ThingUID = uid;
        value = CIntelligentPointer<CThing>(thing);
    }
}
int main()
{
    g_created.UsingDummy = true; g_created.Dummy.ThingUID = 0;
    u64 data[2] = { 0, U64C(0xabcdef0000000078) };
    CMemoryDataInputStream in = { 0, (const unsigned char*)data, 16 };
    CPersistTraits<CIntelligentPointer<CThing> > traits; CIntelligentPointer<CThing> out;
    traits.TransferBinaryIn(in, out);
    bool ok = in.StreamPos == 8 && out.Item == 0 && g_creates == 0;
    traits.TransferBinaryIn(in, out);
    ok = ok && in.StreamPos == 16 && out.Item == &g_created && g_creates == 1 && g_lastIndex == 77 && g_created.Dummy.ThingUID == U64C(0xabcdef0000000078);
    if (!ok) { std::printf("006c8780_TEST FAIL\n"); return 1; }
    std::printf("006c8780_TEST PASS\n");
    return 0;
}
