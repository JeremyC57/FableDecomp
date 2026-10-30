// Behaviour test for 0x00C172F0 Process_00c172f0 (libpk bank-file load): no file -> false; otherwise the bank data is read
// into a stack log (errors clear its status), the wide name is converted for the log, and the log status is returned;
// the file wrapper releases the file on every exit.
#include <cstdio>
#include <cstring>

static int g_released = 0;
static bool g_readFails = false, g_serialiseFails = false;
static char g_lastName[0x100];

struct File { int id; };
struct AutoFile {
    File* m_pFile;
    AutoFile(File* f) : m_pFile(f) { }
    ~AutoFile() { if (m_pFile) ++g_released; }
    File* GetChecked() { return m_pFile; }
};
struct LoadLog {
    bool m_ok;
    LoadLog() : m_ok(true) { }
    void AddError() { m_ok = false; }
    bool IsOk() const { return m_ok; }
    void SetSourceName(const char* name) { strcpy(g_lastName, name); }
};
struct BankData { int words[12]; };
static void ReadBankData(File*, BankData*, unsigned int, LoadLog* log) { if (g_readFails) log->AddError(); }
static bool SerialiseBankMetaData(BankData*, void*) { return !g_serialiseFails; }
static void LoadBank(LoadLog* log, File* file, void* request)
{
    BankData data;
    ReadBankData(file, &data, 0x2000, log);
    if (log->IsOk()) {
        if (!SerialiseBankMetaData(&data, request))
            log->AddError();
    }
}
static void WideToMultiByte(const unsigned short* src, char* dst, int size)
{
    int i = 0;
    for (; src[i] && i < size - 1; ++i) dst[i] = (char)src[i];
    dst[i] = 0;
}
struct Processor {
    File* m_file;
    File* Open(const void*) { return m_file; }
};
static bool Process(Processor* processor, const unsigned short* name, void*, void* request)
{
    AutoFile file(processor->Open(name));
    if (!file.m_pFile)
        return false;
    LoadLog log;
    LoadBank(&log, file.GetChecked(), request);
    char buffer[0x100];
    WideToMultiByte(name, buffer, 0x100);
    log.SetSourceName(buffer);
    if (!log.IsOk())
        return false;
    return true;
}

int main()
{
    static const unsigned short name[] = { 'b', 'a', 'n', 'k', 0 };
    File f; f.id = 1;
    Processor none; none.m_file = 0;
    Processor some; some.m_file = &f;
    if (Process(&none, name, 0, 0) || g_released != 0) { std::printf("00c172f0_TEST FAIL\n"); return 1; }
    if (!Process(&some, name, 0, 0) || g_released != 1 || strcmp(g_lastName, "bank") != 0) { std::printf("00c172f0_TEST FAIL\n"); return 1; }
    g_serialiseFails = true;
    if (Process(&some, name, 0, 0) || g_released != 2) { std::printf("00c172f0_TEST FAIL\n"); return 1; }
    g_serialiseFails = false; g_readFails = true;
    if (Process(&some, name, 0, 0) || g_released != 3) { std::printf("00c172f0_TEST FAIL\n"); return 1; }
    std::printf("00c172f0_TEST PASS\n");
    return 0;
}