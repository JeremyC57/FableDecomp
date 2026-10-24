// Behaviour test for 0x0057F78F CPersistTraits<CHeroMarriageInfo>::TransferOut: appends
// "(<wifeUID>,<homeUID>,<state>,TRUE|FALSE)" to the output text, the UIDs in unsigned decimal.
#include <cstdio>
#include <cstring>

struct CHeroMarriageInfo { unsigned long long WifeUID; unsigned long long MaritalHomeUID; int State; bool GayMarriage; };
struct Out {
    char text[256];
    Out& operator+=(const char* s) { std::strcat(text, s); return *this; }
};
static void AppendU64(Out& out, unsigned long long v)
{
    char buf[32]; int n = 0; char rev[32];
    do { rev[n++] = (char)('0' + (int)(v % 10)); v /= 10; } while (v);
    for (int i = 0; i < n; ++i) buf[i] = rev[n - 1 - i];
    buf[n] = 0;
    out += buf;
}
static void TransferOutLong(Out& out, int v) { char buf[16]; std::sprintf(buf, "%d", v); out += buf; }
static void TransferOut(Out& out, const CHeroMarriageInfo* value)
{
    out += "(";
    AppendU64(out, value->WifeUID);
    out += ",";
    AppendU64(out, value->MaritalHomeUID);
    out += ",";
    TransferOutLong(out, value->State);
    out += ",";
    if (value->GayMarriage) out += "TRUE"; else out += "FALSE";
    out += ")";
}

int main()
{
    CHeroMarriageInfo a = { 18446744073709551615ull, 12345ull, 2, true };
    CHeroMarriageInfo b = { 0ull, 4294967296ull, 0, false };
    Out o; o.text[0] = 0;
    TransferOut(o, &a);
    bool ok = std::strcmp(o.text, "(18446744073709551615,12345,2,TRUE)") == 0;
    o.text[0] = 0;
    TransferOut(o, &b);
    ok = ok && std::strcmp(o.text, "(0,4294967296,0,FALSE)") == 0;
    std::printf(ok ? "0057f78f_TEST PASS\n" : "0057f78f_TEST FAIL\n");
    return ok ? 0 : 1;
}