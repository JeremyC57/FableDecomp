// behaviour test for 0x00C62E60 PKRASlaveObject::PassToSlave: forwards (data, 0) to the slave's slot 1; a null slave is reported
#include <cstdio>
#include <cstring>
static char g_report[128];
static int g_reports = 0;
static void Report(const char* file, long line, const char* text)
{
    std::sprintf(g_report, "%s(%ld) : %s\n", file, line, text);
    ++g_reports;
}
struct PKRASlaveObject {
    void* m_lastData; long m_lastFlags; int m_calls;
    void OnComplete(void* data, long flags) { m_lastData = data; m_lastFlags = flags; ++m_calls; }
    void PassToSlave(void* data, PKRASlaveObject* slave)
    {
        if (!slave) { Report(".\\PKRASlaveObject.cpp", 0x1c, "Null slave object"); return; }   // retail then breaks
        slave->OnComplete(data, 0);
    }
};
int main()
{
    PKRASlaveObject a = { 0, 0, 0 }, b = { 0, 7, 0 };
    int payload = 3;
    a.PassToSlave(&payload, &b);
    if (b.m_calls != 1 || b.m_lastData != &payload || b.m_lastFlags != 0 || g_reports != 0) { std::printf("00c62e60_TEST FAIL\n"); return 1; }
    a.PassToSlave(&payload, 0);
    if (g_reports != 1 || std::strcmp(g_report, ".\\PKRASlaveObject.cpp(28) : Null slave object\n") != 0) { std::printf("00c62e60_TEST FAIL\n"); return 1; }
    std::printf("00c62e60_TEST PASS\n");
    return 0;
}