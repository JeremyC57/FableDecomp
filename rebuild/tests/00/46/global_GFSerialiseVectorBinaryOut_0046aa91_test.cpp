// Behaviour test for 0x0046AA91 GFSerialiseVectorBinaryOut<CSoundBankEntry>: count, then one traits write per entry
#include <cstdio>
struct Writer { int log[32]; int n; void WriteLong(int v) { log[n++] = v; } };
class CSoundBankEntry { public:
    unsigned int ID; int Filename; int SymbolFilename; int LipSyncBankHandle; bool DVD;
    CSoundBankEntry() : ID(0), Filename(-1), SymbolFilename(-1), LipSyncBankHandle(-1) {}
};
struct Traits {                                    // the traits writer @ 0x0046AAF0: ID, three indices, DVD flag
    CSoundBankEntry DefaultVal;
    void TransferBinaryOut(Writer& w, const CSoundBankEntry* e) const
    { w.WriteLong((int)e->ID); w.WriteLong(e->Filename); w.WriteLong(e->SymbolFilename); w.WriteLong(e->LipSyncBankHandle); w.WriteLong(e->DVD ? 1 : 0); }
};
struct EntryVector { CSoundBankEntry* _M_start; CSoundBankEntry* _M_finish; CSoundBankEntry* _M_end_of_storage; };
static void SerialiseOut(Writer& stream, EntryVector& value)
{
    Traits traits;
    int count = (int)(value._M_finish - value._M_start);
    stream.WriteLong(count);
    for (int i = 0; i < count; ++i)
        traits.TransferBinaryOut(stream, value._M_start + i);
}
int main()
{
    CSoundBankEntry e[2];
    e[0].ID = 5; e[0].Filename = 6; e[0].DVD = true;
    e[1].ID = 9; e[1].LipSyncBankHandle = 4; e[1].DVD = false;
    EntryVector v = { e, e + 2, e + 2 };
    Writer w; w.n = 0;
    SerialiseOut(w, v);
    const int want[11] = { 2, 5, 6, -1, -1, 1, 9, -1, -1, 4, 0 };
    if (w.n != 11) { std::printf("0046aa91_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 11; ++i) if (w.log[i] != want[i]) { std::printf("0046aa91_TEST FAIL\n"); return 1; }
    std::printf("0046aa91_TEST PASS\n");
    return 0;
}