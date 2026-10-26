// behaviour test for 00472a8d: CPersistContext::TransferVectorOfSubComponents<CDialogueLayerDef> --
// saving writes size() then transfers each element; binary loading (mode 2) reads the count,
// resizes and transfers each element; the CRC key is checked first in both modes.
#include <cstdio>
#include <cstring>
class CPersistContext;
static int g_crcChecks = 0;
static int g_transfers = 0;
class CMemoryDataInputStream { public:
    const unsigned char* m_pData;
    long m_remaining;
    long ReadLong()
    {
        long value = 0;
        if (m_remaining >= (long)sizeof(int)) {
            int v = 0;
            std::memcpy(&v, m_pData, sizeof(int));
            value = v;
            m_pData += sizeof(int);
            m_remaining -= sizeof(int);
        }
        return value;
    }
};
class CDataOutputStream { public:
    long Written[4];
    int Count;
    void WriteLong(long value) { Written[Count++] = value; }
};
struct CDialogueLayerDef {
    int Transferred;
    void Transfer(CPersistContext&) { ++Transferred; ++g_transfers; }
};
struct CDialogueLayerVector {
    CDialogueLayerDef Items[8];
    long Count;
    long size() const { return Count; }
    CDialogueLayerDef& operator[](long index) { return Items[index]; }
    void resize(long count) { long i = 0; for (i = Count; i < count; ++i) Items[i].Transferred = 0; Count = count; }
};
class CPersistContext { public:
    long Mode;
    CMemoryDataInputStream* PLoadStream;
    CDataOutputStream* PSaveStream;
    void CheckCRC(const char*) { ++g_crcChecks; }
    void TransferVectorOfSubComponents(CDialogueLayerVector& value)
    {
        CheckCRC("");
        switch (Mode) {
        case 2: {
            long count = PLoadStream->ReadLong();
            value.resize(count);
            long i = 0;
            for (i = 0; i < count; ++i)
                value[i].Transfer(*this);
            break;
        }
        default: {
            long count = value.size();
            PSaveStream->WriteLong(count);
            long i = 0;
            for (i = 0; i < count; ++i)
                value[i].Transfer(*this);
            break;
        }
        }
    }
};
int main()
{
    int ok = 1;
    CDialogueLayerVector v;
    v.Count = 0;
    v.resize(3);
    CDataOutputStream out;
    out.Count = 0;
    CPersistContext save;
    save.Mode = 1;
    save.PSaveStream = &out;
    save.PLoadStream = 0;
    save.TransferVectorOfSubComponents(v);
    if (out.Count != 1 || out.Written[0] != 3 || g_transfers != 3 || g_crcChecks != 1) ok = 0;
    int stored = 5;
    unsigned char bytes[4];
    std::memcpy(bytes, &stored, 4);
    CMemoryDataInputStream in;
    in.m_pData = bytes;
    in.m_remaining = 4;
    CPersistContext load;
    load.Mode = 2;
    load.PLoadStream = &in;
    load.PSaveStream = 0;
    load.TransferVectorOfSubComponents(v);
    if (v.Count != 5 || g_transfers != 8 || g_crcChecks != 2 || v.Items[4].Transferred != 1 || v.Items[0].Transferred != 2) ok = 0;
    if (!ok) { std::printf("00472a8d_TEST FAIL\n"); return 1; }
    std::printf("00472a8d_TEST PASS\n");
    return 0;
}