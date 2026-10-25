// Behaviour test for Explode (0x00560920): each piece before a separator is appended to the output, the tail after
// the last separator is dropped, existing entries are kept, and the return value is the vector's total size --
// through a copy of the source loop over the standard string/vector.
#include <cstdio>
#include <string>
#include <vector>

static int Explode(std::string& text, const std::string& separator, std::vector<std::string>& out)
{
    std::string::size_type pos = 0;
    std::string::size_type found;
    while ((found = text.find(separator, pos)) != std::string::npos) {
        std::string piece = text.substr(pos, found - pos);
        out.push_back(piece);
        pos = found + separator.size();
    }
    return (int)out.size();
}

int main()
{
    std::vector<std::string> lines;
    std::string text("SID:OK\r\nalpha\r\n\r\nbeta\r\ntail");
    if (Explode(text, std::string("\r\n"), lines) != 4) { std::printf("00560920_TEST FAIL\n"); return 1; }
    if (lines[0] != "SID:OK" || lines[1] != "alpha" || lines[2] != "" || lines[3] != "beta") {
        std::printf("00560920_TEST FAIL\n");
        return 1;
    }
    std::string none("no separator");
    if (Explode(none, std::string("\r\n"), lines) != 4) { std::printf("00560920_TEST FAIL\n"); return 1; }
    std::string commas("a,b,");
    if (Explode(commas, std::string(","), lines) != 6 || lines[4] != "a" || lines[5] != "b") {
        std::printf("00560920_TEST FAIL\n");
        return 1;
    }
    std::printf("00560920_TEST PASS\n");
    return 0;
}