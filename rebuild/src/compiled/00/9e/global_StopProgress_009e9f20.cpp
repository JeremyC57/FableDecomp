// NProgressDisplay::StopProgress(bool, bool) -- when a progress display exists, forward both flags to its stop member (vtable slot 2)
// Leaf: ?StopProgress@NProgressDisplay@@YIX_N0@Z
struct Prog {
    virtual void m0();
    virtual void m1();
    virtual void stop(bool a, bool b);
};
Prog* g_prog;

namespace NProgressDisplay {   // NF308: the PDB scope (Ego_r / FableWin ?StopProgress@NProgressDisplay@@YIX_N0@Z)
void __fastcall StopProgress(bool a, bool b)
{
    Prog* p = g_prog;
    if (p)
        p->stop(a, b);
}
}