// behaviour test for 0x006DC6A0 CPersistTraits<CAttackHistoryCombo>::TransferBinaryIn (body copied from the row source; stubs)
#include <stdio.h>
#include <string.h>
#include <vector>
#include <utility>
enum EAttackHistoryType { AH_A = 0, AH_B = 1, AH_C = 2 };
enum EDamageAttribute { DA_X = 0, DA_Y = 1, DA_Z = 2 };
class CMemoryDataInputStream {
public:
    const unsigned char* Data; unsigned long Pos;
    unsigned long ReadULONG() { unsigned int v; memcpy(&v, Data + Pos, 4); Pos += 4; return v; }
    float ReadFloat() { float v; memcpy(&v, Data + Pos, 4); Pos += 4; return v; }
    template <class T> T ReadType() { int v; memcpy(&v, Data + Pos, 4); Pos += 4; return (T)v; }
};
class CAttackHistoryCombo {
public:
    std::vector<std::pair<EAttackHistoryType, EDamageAttribute> > AttackCombo;
    float Multiplier;
};
template <class T> class CPersistTraits;
template <> class CPersistTraits<CAttackHistoryCombo> {
public:
    void TransferBinaryIn(CMemoryDataInputStream& stream, CAttackHistoryCombo& value);
};
void CPersistTraits<CAttackHistoryCombo>::TransferBinaryIn(CMemoryDataInputStream& stream, CAttackHistoryCombo& value)
{
    for (unsigned long count = stream.ReadULONG(); count != 0; --count) {
        EAttackHistoryType type = stream.ReadType<EAttackHistoryType>();
        EDamageAttribute attribute = stream.ReadType<EDamageAttribute>();
        value.AttackCombo.push_back(std::make_pair(type, attribute));
    }
    value.Multiplier = stream.ReadFloat();
}
static void Put(unsigned char* b, unsigned long& p, unsigned int v) { memcpy(b + p, &v, 4); p += 4; }
int main()
{
    unsigned char buf[64]; unsigned long n = 0; float m = 2.5f;
    Put(buf, n, 2); Put(buf, n, 2); Put(buf, n, 1); Put(buf, n, 0); Put(buf, n, 2);
    memcpy(buf + n, &m, 4); n += 4;
    CMemoryDataInputStream stream; stream.Data = buf; stream.Pos = 0;
    CAttackHistoryCombo combo; combo.Multiplier = 0.0f;
    combo.AttackCombo.push_back(std::make_pair(AH_A, DA_X));
    CPersistTraits<CAttackHistoryCombo>().TransferBinaryIn(stream, combo);
    bool ok = combo.AttackCombo.size() == 3 && combo.AttackCombo[1].first == AH_C && combo.AttackCombo[1].second == DA_Y
        && combo.AttackCombo[2].first == AH_A && combo.AttackCombo[2].second == DA_Z && combo.Multiplier == 2.5f && stream.Pos == n;
    unsigned char empty[8]; unsigned long e = 0; float z = -1.0f; Put(empty, e, 0); memcpy(empty + e, &z, 4); e += 4;
    CMemoryDataInputStream s2; s2.Data = empty; s2.Pos = 0; CAttackHistoryCombo c2; c2.Multiplier = 0.0f;
    CPersistTraits<CAttackHistoryCombo>().TransferBinaryIn(s2, c2);
    ok = ok && c2.AttackCombo.empty() && c2.Multiplier == -1.0f && s2.Pos == 8;
    printf(ok ? "006dc6a0_TEST PASS\n" : "006dc6a0_TEST FAIL\n");
    return ok ? 0 : 1;
}