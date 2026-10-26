// behaviour test for 0x007B6A80 CTCCrateStack::vfunc_6 (initial activation)
#include <cstdio>
struct Def { long refs; long CrateDefIndex; int destroyed; void Destroy() { ++destroyed; } };
struct Village { Def* def; };
struct Member { Village* v; Village* GetPVillage() const { return v; } };
class CThing { public:
    unsigned int bits[9]; Member* comp; int kills;
    template<class T> bool GetInterface(T** out, long type) { if (bits[type >> 5] & (1u << (type & 31))) { *out = comp; return true; } return false; }
    void Kill(bool) { ++kills; }
};
struct Stack {
    CThing* ParentThing; long CrateDefIndex; bool suppressed;
    void vfunc_6()
    {
        if (!suppressed) {
            Member* m; ParentThing->GetInterface(&m, 0x23);
            Village* v = m->GetPVillage();
            if (v) {
                Def* d = v->def; ++d->refs;
                CrateDefIndex = d->CrateDefIndex;
                if (--d->refs == 0) d->Destroy();
                if (CrateDefIndex != 0) return;
            }
            ParentThing->Kill(false);
        }
    }
};
int main()
{
    Def d = { 1, 77, 0 };
    Village v = { &d };
    Member m = { &v };
    CThing t; for (int i = 0; i < 9; ++i) t.bits[i] = 0; t.bits[1] = 8; t.comp = &m; t.kills = 0;
    Stack s; s.ParentThing = &t; s.CrateDefIndex = 0; s.suppressed = false;
    s.vfunc_6();
    bool ok = s.CrateDefIndex == 77 && t.kills == 0 && d.refs == 1 && d.destroyed == 0;
    d.CrateDefIndex = 0; s.vfunc_6();
    ok = ok && t.kills == 1;
    m.v = 0; s.vfunc_6();
    ok = ok && t.kills == 2;
    s.suppressed = true; s.vfunc_6();
    ok = ok && t.kills == 2;
    if (!ok) { std::printf("007b6a80_TEST FAIL\n"); return 1; }
    std::printf("007b6a80_TEST PASS\n");
    return 0;
}