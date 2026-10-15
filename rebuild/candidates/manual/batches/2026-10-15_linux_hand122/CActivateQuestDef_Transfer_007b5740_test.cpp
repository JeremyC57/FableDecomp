// Behaviour test for CActivateQuestDef::Transfer (0x007B5740): save then load round trip.
#include <cstdio>

struct CMemoryDataInputStream { long words[4]; int pos; long Read() { return words[pos++]; } };
struct CDataOutputStream {
    long words[4]; int pos;
    void WriteLong(long v) { words[pos++] = v; }
    void WriteBool(bool v) { words[pos++] = v ? 1 : 0; }
};

struct CDefString { long TablePos; void TransferBinaryIn(CMemoryDataInputStream* s) { TablePos = s->Read(); } };

template<class T> struct CPersistTraits;
template<> struct CPersistTraits<bool> {
    void TransferBinaryIn(CMemoryDataInputStream* s, bool* v) { *v = s->Read() != 0; }
    static void TransferBinaryOut(CDataOutputStream* s, const bool& v) { s->WriteBool(v); }
};
template<> struct CPersistTraits<CDefString> {
    void TransferBinaryIn(CMemoryDataInputStream* s, CDefString* v) { v->TransferBinaryIn(s); }
    static void TransferBinaryOut(CDataOutputStream* s, const CDefString& v) { s->WriteLong(v.TablePos); }
};

struct CPersistContext {
    long m_mode; CMemoryDataInputStream* m_pLoadStream; CDataOutputStream* m_pSaveStream; int crcs;
    void CheckCRC(const char*) { ++crcs; }
    template<class T> void Transfer(const char* name, T& value)
    {
        CheckCRC(name);
        switch (m_mode) {
        case 2: CPersistTraits<T>().TransferBinaryIn(m_pLoadStream, &value); break;
        case 3: CPersistTraits<T>::TransferBinaryOut(m_pSaveStream, value); break;
        }
    }
};

struct CActivateQuestDef {
    CDefString ScriptName; bool LoadResources;
    void Transfer(CPersistContext& context)
    {
        context.Transfer("", ScriptName);
        context.Transfer("", LoadResources);
    }
};

int main()
{
    CDataOutputStream out; out.pos = 0;
    CPersistContext save; save.m_mode = 3; save.m_pSaveStream = &out; save.m_pLoadStream = 0; save.crcs = 0;
    CActivateQuestDef a; a.ScriptName.TablePos = 1234; a.LoadResources = true;
    a.Transfer(save);
    CMemoryDataInputStream in; in.pos = 0; in.words[0] = out.words[0]; in.words[1] = out.words[1];
    CPersistContext load; load.m_mode = 2; load.m_pLoadStream = &in; load.m_pSaveStream = 0; load.crcs = 0;
    CActivateQuestDef b; b.ScriptName.TablePos = 0; b.LoadResources = false;
    b.Transfer(load);
    CPersistContext other; other.m_mode = 1; other.crcs = 0; b.Transfer(other);
    if (out.pos != 2 || b.ScriptName.TablePos != 1234 || !b.LoadResources || save.crcs != 2 || other.crcs != 2) {
        std::printf("007b5740_TEST FAIL\n");
        return 1;
    }
    std::printf("007b5740_TEST PASS\n");
    return 0;
}
