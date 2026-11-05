// Behaviour test for 0x009a8170: CBankFileManager::OpenIniFile -- bank lines, retail-file and header-directory keywords
// Host model: the ini text is parsed line by line with the same keyword and quoted-string rules; SplitPath keeps the directory.
// The bank name is the text before '=' as read (not trimmed), as retail keys Banks with the ReadUntilCharacter result.
#include <cstdio>
#include <cctype>
#include <map>
#include <string>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { g_fail = 1; std::printf("check failed line %d\n", __LINE__); } } while (0)
static int StricmpModel(const char* a, const char* b)     // _stricmp model (portable)
{
    for (;; ++a, ++b) { int ca = tolower((unsigned char)*a), cb = tolower((unsigned char)*b); if (ca != cb || !ca) return ca - cb; }
}
struct LineStream {                                  // CMemoryDataInputStream over one line
    std::string Text; size_t Pos;
    explicit LineStream(const std::string& t) : Text(t), Pos(0) { }
    std::string ReadUntilCharacter(char c, bool& found) { size_t e = Text.find(c, Pos); found = e != std::string::npos; if (!found) e = Text.size(); std::string r = Text.substr(Pos, e - Pos); Pos = e; return r; }
    void Read(char* c) { if (Pos < Text.size()) *c = Text[Pos++]; }
    void SetPosition(size_t p) { Pos = p; }
    std::string ReadUntilWhiteSpace() { size_t s = Pos; while (Pos < Text.size() && !isspace((unsigned char)Text[Pos])) ++Pos; return Text.substr(s, Pos - s); }
};
static std::string Trim(const std::string& s) { size_t b = s.find_first_not_of(" \t\r\n"), e = s.find_last_not_of(" \t\r\n"); return b == std::string::npos ? "" : s.substr(b, e - b + 1); }
struct CBankInfo { std::string DevBankFilePath, RetailBankFilePath, DevHeaderFilePath, RetailHeaderFilePath; void* RegisteredBank; };
struct CBankFileManager {
    std::map<std::string, CBankInfo> Banks; std::string BasePath;
    bool ReadQuotedString(std::string& text, LineStream& s)      // 0x009A77D0 model: the next "..." on the line
    {
        size_t a = s.Text.find('"', s.Pos); if (a == std::string::npos) return false;
        size_t b = s.Text.find('"', a + 1); if (b == std::string::npos) return false;
        text = s.Text.substr(a + 1, b - a - 1); s.Pos = b + 1; return true;
    }
    void OpenIniFile(const std::string& pathname, const std::string& contents)
    {
        Banks.clear();
        std::string retail_file, dev_header_directory, retail_header_directory;
        size_t at = 0;
        while (at < contents.size()) {
            size_t nl = contents.find('\n', at); if (nl == std::string::npos) nl = contents.size();
            std::string line = Trim(contents.substr(at, nl - at)); at = nl + 1;
            LineStream line_stream(line);
            bool found;
            std::string bank_name = line_stream.ReadUntilCharacter('=', found);
            if (found) {
                char equals; line_stream.Read(&equals);
                std::string dev_path;
                if (ReadQuotedString(dev_path, line_stream)) {
                    std::string header_name, dev_header, retail_header;
                    if (ReadQuotedString(header_name, line_stream) && header_name.size() > 0) {
                        dev_header = dev_header_directory + header_name;
                        retail_header = retail_header_directory + header_name;
                    }
                    CBankInfo info = { dev_path, retail_file, dev_header, retail_header, 0 };
                    Banks[bank_name] = info;
                }
            } else {
                line_stream.SetPosition(0);
                std::string keyword = line_stream.ReadUntilWhiteSpace();
                std::string value;
                if (!StricmpModel(keyword.c_str(), "BeginRetailFile")) { ReadQuotedString(value, line_stream); retail_file = value; }
                else if (!StricmpModel(keyword.c_str(), "EndRetailFile")) retail_file = "";
                else if (!StricmpModel(keyword.c_str(), "DevHeaderDirectory")) { ReadQuotedString(value, line_stream); dev_header_directory = value; }
                else if (!StricmpModel(keyword.c_str(), "RetailHeaderDirectory")) { ReadQuotedString(value, line_stream); retail_header_directory = value; }
            }
        }
        BasePath = pathname.substr(0, pathname.find_last_of("\\/") + 1);
    }
};
int main()
{
    CBankFileManager m;
    CBankInfo stale = { "x", "x", "x", "x", 0 }; m.Banks["STALE"] = stale;
    const char* ini =
        "  DevHeaderDirectory \"Data\\\\Dev\\\\\"  \n"
        "RetailHeaderDirectory \"Data\\\\Retail\\\\\"\n"
        "begINRetailFile \"Data\\\\textures.big\"\n"
        "GBANK_GUI=\"Data\\\\gui.bnk\" \"gui.h\"\n"
        "GBANK_MAIN= \"Data\\\\main.bnk\"\n"
        "EndRetailFile\n"
        "GBANK_FONT=\"Data\\\\font.bnk\" \"\"\n"
        "BROKEN=no quotes\n"
        "UnknownKeyword \"ignored\"\n";
    m.OpenIniFile("C:\\Fable\\Data\\Banks.ini", ini);
    CHECK(m.Banks.size() == 3 && !m.Banks.count("STALE") && !m.Banks.count("BROKEN"));
    const CBankInfo& gui = m.Banks["GBANK_GUI"];
    CHECK(gui.DevBankFilePath == "Data\\\\gui.bnk" && gui.RetailBankFilePath == "Data\\\\textures.big");
    CHECK(gui.DevHeaderFilePath == "Data\\\\Dev\\\\gui.h" && gui.RetailHeaderFilePath == "Data\\\\Retail\\\\gui.h");
    const CBankInfo& main = m.Banks["GBANK_MAIN"];
    CHECK(main.RetailBankFilePath == "Data\\\\textures.big" && main.DevHeaderFilePath.empty() && main.RetailHeaderFilePath.empty());
    const CBankInfo& font = m.Banks["GBANK_FONT"];
    CHECK(font.RetailBankFilePath.empty() && font.DevHeaderFilePath.empty());
    CHECK(m.BasePath == "C:\\Fable\\Data\\");
    if (g_fail) { std::printf("009a8170_TEST FAIL\n"); return 1; }
    std::printf("009a8170_TEST PASS\n");
    return 0;
}