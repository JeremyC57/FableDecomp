// Behaviour test for CAnimComponentCombatMisc::TransferBinaryOut @ 0x005db370: WriteBool(MeleeFlourish) then WriteBool(MeleeKnockdown).
#include <cstdio>
struct CDataOutputStream {
    int count; bool values[4];
    CDataOutputStream() : count(0) { }
    void WriteBool(bool value) { values[count++] = value; }
};
struct CAnimComponentCombatMisc {
    void* __vftable; long m_refCount; bool MeleeFlourish; bool MeleeKnockdown;
    void TransferBinaryOut(CDataOutputStream& stream) const { stream.WriteBool(MeleeFlourish); stream.WriteBool(MeleeKnockdown); }
};
int main()
{
    CAnimComponentCombatMisc c; c.MeleeFlourish = true; c.MeleeKnockdown = false;
    CDataOutputStream s; c.TransferBinaryOut(s);
    bool ok = s.count == 2 && s.values[0] == true && s.values[1] == false;
    c.MeleeFlourish = false; c.MeleeKnockdown = true;
    CDataOutputStream s2; c.TransferBinaryOut(s2);
    if (!(s2.count == 2 && s2.values[0] == false && s2.values[1] == true)) ok = false;
    if (!ok) { std::printf("005db370_TEST FAIL\n"); return 1; }
    std::printf("005db370_TEST PASS\n");
    return 0;
}