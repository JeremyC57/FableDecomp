#include <cstdio>
// Behaviour model of CTCGuard::ReportCrime (0x007B2C70).
struct Opinion { int known; int informed; bool Knows(long c) { return c == known; } void Inform(long c) { informed = (int)c; } };
struct Village { int reported; bool flag; Opinion* opinion; };
static void ReportCrime(Village& v, long crime, bool flag)
{
    v.reported = (int)crime;
    if (flag) v.flag = true;
    Opinion* o = v.opinion;
    if (o) { if (!o->Knows(crime)) o->Inform(crime); }
}
int main()
{
    Opinion o; o.known = 3; o.informed = 0;
    Village v; v.reported = 0; v.flag = false; v.opinion = &o;
    ReportCrime(v, 7, false);
    if (v.reported != 7 || v.flag || o.informed != 7) { std::printf("007b2c70_TEST FAIL\n"); return 1; }
    o.informed = 0; ReportCrime(v, 3, true);
    if (!v.flag || o.informed != 0) { std::printf("007b2c70_TEST FAIL\n"); return 1; }
    v.opinion = 0; ReportCrime(v, 4, false);
    if (v.reported != 4) { std::printf("007b2c70_TEST FAIL\n"); return 1; }
    std::printf("007b2c70_TEST PASS\n");
    return 0;
}
