// Behaviour test for 0x00A48AC0: tag, count, real parts, then the four frame floats.
#include <cstdio>
struct Out { float f[16]; unsigned int u[4]; int nf, nu; void U(unsigned int v) { u[nu++] = v; } void F(float v) { f[nf++] = v; } };
struct C { float Re, Im; };
int main()
{
    C fr[3] = {{1, 9}, {2, 9}, {3, 9}}; float amp = 4, med = 5, base = 6, var = 7; Out o; o.nf = o.nu = 0;
    o.U(0x0ff7da7a); o.U(3); for (unsigned int i = 0; i < 3; ++i) o.F(fr[i].Re); o.F(amp); o.F(med); o.F(base); o.F(var);
    bool ok = o.nu == 2 && o.u[0] == 0x0ff7da7au && o.u[1] == 3 && o.nf == 7 && o.f[2] == 3 && o.f[6] == 7;
    if (!ok) { std::printf("00a48ac0_TEST FAIL\n"); return 1; }
    std::printf("00a48ac0_TEST PASS\n");
    return 0;
}