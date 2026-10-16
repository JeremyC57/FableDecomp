// behaviour test for 0x006A1D60 CTCCarrying::UpdateCarriedObjects
#include <cstdio>
struct Carryable { int updates; bool last; void UpdateCarriedPosition(bool f) { ++updates; last = f; } };
struct CThing {
    unsigned int bits[9]; Carryable* comp;
    template<class T> bool GetInterface(T** out, long type) { if (bits[type >> 5] & (1u << (type & 31))) { *out = comp; return true; } return false; }
};
struct Ptr { CThing* p; CThing* GetPItem() const { return p; } };
struct Entry { long first; Ptr second; };
struct Carrying {
    Entry* firstE; Entry* lastE;
    unsigned int size() const { return (unsigned int)(lastE - firstE); }
    void UpdateCarriedObjects()
    {
        Carryable* carryable = 0;
        for (unsigned int i = 0; i < size(); ++i) {
            CThing* t = (firstE + i)->second.GetPItem();
            if (t) { t->GetInterface(&carryable, 0x47); carryable->UpdateCarriedPosition(false); }
        }
    }
};
int main()
{
    Carryable ca = { 0, true }, cb = { 0, true };
    CThing a, b; for (int i = 0; i < 9; ++i) a.bits[i] = b.bits[i] = 0;
    a.bits[2] = 0x80; a.comp = &ca; b.bits[2] = 0x80; b.comp = &cb;
    Entry e[3]; e[0].second.p = &a; e[1].second.p = 0; e[2].second.p = &b;
    Carrying c; c.firstE = e; c.lastE = e + 3;
    c.UpdateCarriedObjects();
    bool ok = ca.updates == 1 && cb.updates == 1 && !ca.last && !cb.last;
    if (!ok) { std::printf("006a1d60_TEST FAIL\n"); return 1; }
    std::printf("006a1d60_TEST PASS\n");
    return 0;
}
