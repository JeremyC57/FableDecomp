// Behaviour test for LoadBankFile (0x00C174A0): a file the processor cannot open logs "Could not open "; otherwise the
// name's last four characters pick the loader (.lug ProcessEntry, .lut the lookup-table loader, .met the meta loader,
// case-insensitive) with the open file, and any other extension logs the unrecognised-format error; the file wrapper
// releases the file either way.
#include <cstdio>
#include <cstring>
#include <cctype>
#include <string>

static std::string g_log; static int g_released = 0; static char g_called = 0;
struct File { void Release() { ++g_released; } };
struct PKCAutoFile {
    File* m_pFile;
    PKCAutoFile(File* f) : m_pFile(f) { }
    ~PKCAutoFile() { if (m_pFile) m_pFile->Release(); }
    File* GetChecked() { return m_pFile; }
};
struct CLoadLog { std::string& AddError() { g_log += "|"; return g_log; } };
struct Processor { File* file; File* Open(const unsigned short*) { return file; } };
struct Host; struct Settings;
static File* g_got = 0;
static void ProcessEntry(const Settings*, File* f, Host*, CLoadLog*) { g_called = 'g'; g_got = f; }
static void LoadLookupTableBank(CLoadLog*, File* f, Host*, const Settings*) { g_called = 't'; g_got = f; }
static void DeleteTrackNodeCommand(CLoadLog*, File* f, Host*, const Settings*) { g_called = 'm'; g_got = f; }
static std::string Narrow(const unsigned short* w) { std::string s; while (*w) s += (char)*w++; return s; }
static std::string Right(const std::string& s, int n) { return (int)s.size() <= n ? s : s.substr(s.size() - n); }
static int CompareNoCase(const std::string& a, const char* b)
{ std::string x = a; for (size_t i = 0; i < x.size(); ++i) x[i] = (char)std::tolower(x[i]); return x == b ? 0 : 1; }

static void LoadBankFile(Processor* processor, CLoadLog* log, const unsigned short* name, Host* host, const Settings* settings)
{
    PKCAutoFile file(processor->Open(name));
    if (!file.m_pFile) {
        log->AddError() += "Could not open ";
        return;
    }
    std::string fileName = Narrow(name);
    std::string extension = Right(fileName, 4);
    if (CompareNoCase(extension, ".lug") == 0)
        ProcessEntry(settings, file.GetChecked(), host, log);
    else if (CompareNoCase(extension, ".lut") == 0)
        LoadLookupTableBank(log, file.GetChecked(), host, settings);
    else if (CompareNoCase(extension, ".met") == 0)
        DeleteTrackNodeCommand(log, file.GetChecked(), host, settings);
    else
        log->AddError() += "This bank is not a recognised format (.lug, .lut, .met)";
}
static void W(unsigned short* d, const char* s) { while ((*d++ = (unsigned char)*s++)) { } }

int main()
{
    File f; Processor p; p.file = &f; CLoadLog log; unsigned short n[32];
    bool ok = true;
    W(n, "a.LUG"); LoadBankFile(&p, &log, n, 0, 0); ok = ok && g_called == 'g' && g_got == &f && g_released == 1;
    W(n, "b.lut"); LoadBankFile(&p, &log, n, 0, 0); ok = ok && g_called == 't' && g_released == 2;
    W(n, "c.Met"); LoadBankFile(&p, &log, n, 0, 0); ok = ok && g_called == 'm' && g_released == 3 && g_log.empty();
    g_called = 0;
    W(n, "d.wav"); LoadBankFile(&p, &log, n, 0, 0); ok = ok && g_called == 0 && g_released == 4 && g_log == "|This bank is not a recognised format (.lug, .lut, .met)";
    p.file = 0; g_log = "";
    W(n, "e.lug"); LoadBankFile(&p, &log, n, 0, 0); ok = ok && g_called == 0 && g_released == 4 && g_log == "|Could not open ";
    if (!ok) { std::printf("00c174a0_TEST FAIL\n"); return 1; }
    std::printf("00c174a0_TEST PASS\n");
    return 0;
}