// Behaviour test for 0x0074C5F0 CCreatureGeneratorDef::Transfer: runs the reconstructed transfer against a recording context
// in load mode (2), save mode (3) and an unknown mode, checking the CRC keys and which stream call each member takes.
#include <cstdio>
#include <cstring>
struct CMemoryDataInputStream { long values[8]; int pos; };
struct CDataOutputStream { long written[8]; int count; void WriteLong(long v) { written[count++] = v; } };
struct CDefString { long TablePos; void TransferBinaryIn(CMemoryDataInputStream* s) { TablePos = s->values[s->pos++]; } };
struct CLongVector { long* _First; long* _Last; long* _End; };
static int g_vecIn = 0, g_vecOut = 0;
void GFSerialiseVectorBinaryOutLong(CDataOutputStream* s, CLongVector*) { ++g_vecOut; s->WriteLong(-7); }
void GFSerialiseVectorBinaryInLong(CMemoryDataInputStream* s, CLongVector*) { ++g_vecIn; s->pos++; }
template<class T> struct CPersistTraits;
template<> struct CPersistTraits<long> {
    CPersistTraits() {}
    void TransferBinaryIn(CMemoryDataInputStream* s, long* v) { *v = s->values[s->pos++]; }
    static void TransferBinaryOut(CDataOutputStream* s, const long& v) { s->WriteLong(v); }
};
template<> struct CPersistTraits<CDefString> {
    CPersistTraits() {}
    void TransferBinaryIn(CMemoryDataInputStream* s, CDefString* v) { v->TransferBinaryIn(s); }
    static void TransferBinaryOut(CDataOutputStream* s, const CDefString& v) { s->WriteLong(v.TablePos); }
};
struct CPersistContext {
    long m_mode; CMemoryDataInputStream* m_pLoadStream; CDataOutputStream* m_pSaveStream; int m_crcs;
    void CheckCRC(const char* name) { if (std::strcmp(name, "") == 0) ++m_crcs; }
    template<class T> void Transfer(const char* name, T& value)
    {
        CheckCRC(name);
        switch (m_mode) {
        case 2: CPersistTraits<T>().TransferBinaryIn(m_pLoadStream, &value); break;
        case 3: CPersistTraits<T>::TransferBinaryOut(m_pSaveStream, value); break;
        }
    }
    void Transfer(const char* name, CLongVector& value)
    {
        CheckCRC(name);
        switch (m_mode) {
        case 2: GFSerialiseVectorBinaryInLong(m_pLoadStream, &value); break;
        default: GFSerialiseVectorBinaryOutLong(m_pSaveStream, &value); break;
        }
    }
};
struct CCreatureGeneratorDef {
    virtual void Transfer(CPersistContext& context);
    CLongVector RandomEffects; CDefString RandomSoundCriteria; long SecondsBetweenEffects; long SecondsBetweenSoundEffects;
};
void CCreatureGeneratorDef::Transfer(CPersistContext& context)
{
    context.Transfer("", RandomEffects);
    context.Transfer("", RandomSoundCriteria);
    context.Transfer("", SecondsBetweenEffects);
    context.Transfer("", SecondsBetweenSoundEffects);
}
int main()
{
    CMemoryDataInputStream in = { { 0, 11, 22, 33 }, 0 }; CDataOutputStream out; out.count = 0;
    CCreatureGeneratorDef d; d.RandomSoundCriteria.TablePos = 0; d.SecondsBetweenEffects = 0; d.SecondsBetweenSoundEffects = 0;
    CPersistContext c; c.m_mode = 2; c.m_pLoadStream = &in; c.m_pSaveStream = &out; c.m_crcs = 0;
    d.Transfer(c);
    bool ok = g_vecIn == 1 && d.RandomSoundCriteria.TablePos == 11 && d.SecondsBetweenEffects == 22 && d.SecondsBetweenSoundEffects == 33 && c.m_crcs == 4 && out.count == 0;
    c.m_mode = 3; d.Transfer(c);
    ok = ok && g_vecOut == 1 && out.count == 4 && out.written[0] == -7 && out.written[1] == 11 && out.written[2] == 22 && out.written[3] == 33 && c.m_crcs == 8;
    c.m_mode = 5; d.Transfer(c);                         // unknown mode: only the vector takes the save path
    ok = ok && g_vecOut == 2 && out.count == 5 && c.m_crcs == 12;
    if (!ok) { std::printf("0074c5f0_TEST FAIL\n"); return 1; }
    std::printf("0074c5f0_TEST PASS\n");
    return 0;
}
