#include <cstdio>
static char g_nil[1] = { 0 };
char* _afxPchNil = g_nil;
long g_nextMessageSerial = 0;
class CString { public: char* m_pchData; CString() { m_pchData = _afxPchNil; } };
struct SLogMessage {
    bool m_bRead; CString m_text; long m_serial;
    SLogMessage() : m_bRead(false) { m_serial = g_nextMessageSerial++; }
};
static SLogMessage* g_added = 0;
class CLoadLog {
public:
    virtual void Add(SLogMessage* message) { g_added = message; }
    CString& AddMessage() { SLogMessage* message = new SLogMessage; Add(message); return message->m_text; }
};
int main()
{
    g_nextMessageSerial = 41;
    CLoadLog log;
    CString& text = log.AddMessage();
    if (g_added == 0 || &text != &g_added->m_text || g_added->m_bRead || text.m_pchData != _afxPchNil || g_added->m_serial != 41 || g_nextMessageSerial != 42) { std::printf("00c307e0_TEST FAIL\n"); return 1; }
    delete g_added;
    std::printf("00c307e0_TEST PASS\n");
    return 0;
}
