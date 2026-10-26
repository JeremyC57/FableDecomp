// Behaviour test for 0x006BC290 CRegion::TransferGameState(CPersistContext&).
#include <cstdio>

static int g_crc = 0;

class CMemoryDataInputStream { public: const bool* src; int pos; };
class CDataOutputStream { public: bool out[8]; int n; void WriteBool(bool v) { out[n++] = v; } };

template <class T> struct CPersistTraits;
template <> struct CPersistTraits<bool> {
    void TransferBinaryIn(CMemoryDataInputStream* s, bool* v) { *v = s->src[s->pos++]; }
    static void TransferBinaryOut(CDataOutputStream* s, const bool& v) { s->WriteBool(v); }
};

class CPersistContext { public:
    long m_mode;
    CMemoryDataInputStream* m_pLoadStream;
    CDataOutputStream* m_pSaveStream;
    void CheckCRC(const char* name) { if (name[0] == 0) ++g_crc; }
    template <class T> void Transfer(const char* name, T& value)
    {
        CheckCRC(name);
        switch (m_mode) {
        case 2: CPersistTraits<T>().TransferBinaryIn(m_pLoadStream, &value); break;
        case 3: CPersistTraits<T>::TransferBinaryOut(m_pSaveStream, value); break;
        }
    }
};

class CRegion { public:
    bool AppearOnWorldMap;
    bool CreatureGenerationEnabled;
    bool SoundThemesEnabled;
    void TransferGameState(CPersistContext& context)
    {
        context.Transfer("", AppearOnWorldMap);
        context.Transfer("", CreatureGenerationEnabled);
        context.Transfer("", SoundThemesEnabled);
    }
};

int main()
{
    CRegion r; r.AppearOnWorldMap = true; r.CreatureGenerationEnabled = false; r.SoundThemesEnabled = true;
    CDataOutputStream out; out.n = 0;
    CPersistContext save; save.m_mode = 3; save.m_pLoadStream = 0; save.m_pSaveStream = &out;
    r.TransferGameState(save);
    int ok = g_crc == 3 && out.n == 3 && out.out[0] == true && out.out[1] == false && out.out[2] == true;
    bool data[3] = { false, true, false };
    CMemoryDataInputStream in; in.src = data; in.pos = 0;
    CPersistContext load; load.m_mode = 2; load.m_pLoadStream = &in; load.m_pSaveStream = 0;
    r.TransferGameState(load);
    if (g_crc != 6 || in.pos != 3 || r.AppearOnWorldMap != false || r.CreatureGenerationEnabled != true || r.SoundThemesEnabled != false) ok = 0;
    CPersistContext other; other.m_mode = 1; other.m_pLoadStream = 0; other.m_pSaveStream = 0;   // neither: only the CRCs
    r.TransferGameState(other);
    if (g_crc != 9 || r.CreatureGenerationEnabled != true) ok = 0;
    if (!ok) { std::printf("006bc290_TEST FAIL\n"); return 1; }
    std::printf("006bc290_TEST PASS\n");
    return 0;
}