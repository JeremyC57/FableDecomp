// Behaviour test for CConsole::RunCommandLine (0x009EB430): the first parsed item must be an identifier naming a
// registered input (matched lower-cased), which then executes the whole line; otherwise the user is told why.
#include <cstdio>
#include <cctype>
#include <string>
#include <map>

class CConsole;
struct CInput { std::string Last; int Runs; void Execute(const std::string& line, CConsole*) { Last = line; ++Runs; } };
class CConsole { public:
    std::string Front; std::map<std::string, CInput*> Inputs; std::string Message;
    static std::string ToLower(std::string s) { for (size_t i = 0; i < s.size(); ++i) s[i] = (char)std::tolower((unsigned char)s[i]); return s; }
    void RunCommandLine()
    {
        size_t i = 0; while (i < Front.size() && Front[i] == ' ') ++i;
        if (i == Front.size()) return;                                  // nothing parsed
        if (!std::isalpha((unsigned char)Front[i])) { Message = "command line was not a string"; return; }
        size_t j = i; while (j < Front.size() && (std::isalnum((unsigned char)Front[j]) || Front[j] == '_')) ++j;
        std::string name = Front.substr(i, j - i);
        std::map<std::string, CInput*>::iterator it = Inputs.find(ToLower(name));
        CInput* input = it != Inputs.end() ? it->second : 0;
        if (!input) Message = "unknown input - " + name;
        else input->Execute(Front, this);
    }
};

int main()
{
    CInput god = { "", 0 };
    CConsole c; c.Inputs["god"] = &god;
    c.Front = "God 1"; c.RunCommandLine();
    bool ok = god.Runs == 1 && god.Last == "God 1" && c.Message.empty();
    c.Front = "fly"; c.RunCommandLine();
    ok = ok && c.Message == "unknown input - fly" && god.Runs == 1;
    c.Front = "42"; c.RunCommandLine();
    ok = ok && c.Message == "command line was not a string";
    if (!ok) { std::printf("009eb430_TEST FAIL\n"); return 1; }
    std::printf("009eb430_TEST PASS\n");
    return 0;
}