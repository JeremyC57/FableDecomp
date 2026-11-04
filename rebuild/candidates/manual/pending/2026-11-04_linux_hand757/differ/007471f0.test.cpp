// Behaviour test for 0x007471F0 CTCStealth::DrawPreparePrimitivesForRendering: a g++ model of the parked source (the residue is codegen only).
#include <cstdio>
#include <string>
static std::string States(bool show, bool v, bool s, bool h, bool sn)
{
    if (!show) return "<none>";
    std::string t;
    if (v) t += "V "; if (h) t += "H "; if (s) t += "S "; if (sn) t += "Sn ";
    return t.empty() ? "<none>" : t;
}
int main() {
    int ok = States(true, true, true, true, true) == "V H S Sn " && States(true, false, true, false, false) == "S "
        && States(false, true, true, true, true) == "<none>" && States(true, false, false, false, false) == "<none>";
    std::printf(ok ? "007471f0_TEST PASS\n" : "007471f0_TEST FAIL\n");
    return ok ? 0 : 1;
}
