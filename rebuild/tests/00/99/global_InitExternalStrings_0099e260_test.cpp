// Behaviour test for 0x0099E260 InitExternalStrings: the language id is 0x409 unless the registry holds LangID, a LangID
// without a sub-language gets SUBLANG_DEFAULT (0x400), and each string the strings module cannot supply falls back to
// its built-in English text. The Win32 calls are replaced by scripted stubs.
#include <cstdio>
#include <cstring>

struct Script { bool dllPresent; bool keyOpens; bool valuePresent; unsigned int langValue; bool stringsPresent; };
static Script g_script;
static int g_exitCode = -1, g_closeCount = 0, g_messageBoxes = 0;

static void* LoadLibraryStub(const char*) { return g_script.dllPresent ? (void*)0x10000000 : 0; }
static long RegOpenStub() { return g_script.keyOpens ? 0 : 2; }
static long RegQueryStub(unsigned int* type, unsigned int* data, unsigned int* size)
{
    if (!g_script.valuePresent || *size < 4) return 2;
    *type = 4; *data = g_script.langValue; *size = 4; return 0;
}
static int RetrieveStringStub(void*, unsigned int id, char* buffer, int size)
{
    if (!g_script.stringsPresent) return 0;
    if (size < 16) return 0;
    std::sprintf(buffer, "localised %u", id);
    return 1;
}

static void* g_hStringsDll;
static unsigned short g_LanguageID;
static char g_ExceptionTitle[0x100], g_ExceptionText[0x100], g_EulaFileName[0x20], g_InvalidStringsDllText[0x400];

static void InitExternalStrings()
{
    g_hStringsDll = LoadLibraryStub("strings.dll");
    if (g_hStringsDll == 0) {
        char message[260] = "C:\\Games\\Fable";
        std::strcat(message, "\\strings.dll is missing.");
        ++g_messageBoxes;
        g_exitCode = 1;
        return;   // ExitProcess(1)
    }
    unsigned int type, langID, size;
    g_LanguageID = 0x409;
    if (RegOpenStub() == 0) {
        type = 4;
        size = sizeof(langID);
        if (RegQueryStub(&type, &langID, &size) == 0)
            g_LanguageID = (unsigned short)(((langID & 0xFC00) == 0) ? (langID | 0x400) : langID);
        ++g_closeCount;
    }
    if (!RetrieveStringStub(g_hStringsDll, 0xDB, g_ExceptionTitle, sizeof(g_ExceptionTitle))) std::strcpy(g_ExceptionTitle, "Exception!");
    if (!RetrieveStringStub(g_hStringsDll, 0xDC, g_ExceptionText, sizeof(g_ExceptionText))) std::strcpy(g_ExceptionText, "Gathering Exception Data...");
    if (!RetrieveStringStub(g_hStringsDll, 0xE8, g_EulaFileName, sizeof(g_EulaFileName))) std::strcpy(g_EulaFileName, "eula.rtf");
    if (!RetrieveStringStub(g_hStringsDll, 0xEC, g_InvalidStringsDllText, sizeof(g_InvalidStringsDllText))) std::strcpy(g_InvalidStringsDllText, "Invalid / missing strings.dll");
}

static int g_fail = 0;
static void Check(bool ok, const char* what) { if (!ok) { std::printf("FAIL: %s\n", what); g_fail = 1; } }

int main()
{
    Script s1 = { true, false, false, 0, false };
    g_script = s1; InitExternalStrings();
    Check(g_LanguageID == 0x409, "no key -> 0x409");
    Check(std::strcmp(g_ExceptionTitle, "Exception!") == 0, "title fallback");
    Check(std::strcmp(g_ExceptionText, "Gathering Exception Data...") == 0, "text fallback");
    Check(std::strcmp(g_EulaFileName, "eula.rtf") == 0, "eula fallback");
    Check(std::strcmp(g_InvalidStringsDllText, "Invalid / missing strings.dll") == 0, "error fallback");
    Check(g_closeCount == 0, "no close without an open key");

    Script s2 = { true, true, true, 0x07, true };     // LANG_GERMAN, no sub-language
    g_script = s2; InitExternalStrings();
    Check(g_LanguageID == 0x407, "German gets SUBLANG_DEFAULT");
    Check(std::strcmp(g_EulaFileName, "localised 232") == 0, "localised eula name");
    Check(g_closeCount == 1, "key closed");

    Script s3 = { true, true, true, 0x80a, true };    // es-MX keeps its sub-language
    g_script = s3; InitExternalStrings();
    Check(g_LanguageID == 0x80a, "sub-language kept");

    Script s4 = { true, true, false, 0, true };       // key without the value
    g_script = s4; InitExternalStrings();
    Check(g_LanguageID == 0x409 && g_closeCount == 3, "missing value -> 0x409, key still closed");

    Script s5 = { false, false, false, 0, false };
    g_script = s5; InitExternalStrings();
    Check(g_exitCode == 1 && g_messageBoxes == 1, "missing dll -> message box + exit 1");

    if (!g_fail) std::printf("0099e260_TEST PASS\n");
    return g_fail;
}