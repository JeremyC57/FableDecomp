// Behaviour test for 0x0088B8F0 CAIBrain::SetOutput: the output is assigned (once, through CBrainOutput::operator=) and
// OutputValid is set; nothing else in the brain changes.
#include <stdio.h>
#include <string.h>
static int g_assign = 0;
class CBrainOutput {
public:
    unsigned char body[0x48];
    CBrainOutput& operator=(const CBrainOutput& o) { memcpy(body, o.body, sizeof(body)); ++g_assign; return *this; }
};
class CAIBrain {
public:
    unsigned char before[0x44];
    CBrainOutput  Output;
    bool          OutputValid;
    void SetOutput(const CBrainOutput& output) { Output = output; OutputValid = true; }
};
int main()
{
    static CAIBrain b; memset(&b, 0, sizeof(b));
    CBrainOutput o; for (int i = 0; i < 0x48; ++i) o.body[i] = (unsigned char)(i + 1);
    b.SetOutput(o);
    int ok = g_assign == 1 && b.OutputValid && memcmp(b.Output.body, o.body, 0x48) == 0 && b.before[0] == 0 && b.before[0x43] == 0;
    ok = ok && (char*)&b.Output - (char*)&b == 0x44 && (char*)&b.OutputValid - (char*)&b == 0x8C;
    printf(ok ? "0088b8f0_TEST PASS\n" : "0088b8f0_TEST FAIL\n");
    return ok ? 0 : 1;
}
