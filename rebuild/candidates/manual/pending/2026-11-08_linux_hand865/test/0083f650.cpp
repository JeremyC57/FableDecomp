// Behaviour test for LHNetworkLib::LHHttpHeaders::GetHeader(const std::string& name) 0x0083F650: the value of the first
// header line that starts (case-insensitively) with name -- the text after the first ':' with leading spaces skipped, the
// whole line when it has no ':' past column 0, or "" when no line matches. Same logic over the standard containers.
#include <cstdio>
#include <string>
#include <vector>
#include <cctype>

static std::string toLowerCase(const std::string& text)
{
    std::string result;
    int length = (int)text.size();
    for (int i = 0; i < length; ++i)
        result += (char)std::tolower((unsigned char)text[i]);
    return result;
}

static bool StartsWith(const std::string& text, const std::string& prefix)
{
    return toLowerCase(text).substr(0, prefix.size()) == toLowerCase(prefix);
}

struct LHHttpHeaders {
    std::vector<std::string> HdrDocumentHeader;
    std::string HeaderString;
    std::string GetHeader(const std::string& name)
    {
        for (std::vector<std::string>::iterator it = HdrDocumentHeader.begin(); it != HdrDocumentHeader.end(); ++it) {
            std::string line = *it;
            if (StartsWith(line, name)) {
                int colon = (int)line.find(":");
                if (colon > 0)
                    return line.substr(line.find_first_not_of(" ", colon + 1));
                return line;
            }
        }
        return "";
    }
};

int main()
{
    LHHttpHeaders h;
    h.HdrDocumentHeader.push_back("HTTP/1.1 200 OK");
    h.HdrDocumentHeader.push_back("Content-Length:   1234");
    h.HdrDocumentHeader.push_back("Location:http://example.org/x");
    h.HdrDocumentHeader.push_back(":odd");
    h.HdrDocumentHeader.push_back("Transfer-Encoding: chunked");
    bool ok = true;
    ok = ok && h.GetHeader("content-length") == "1234";          // case-insensitive prefix, spaces skipped
    ok = ok && h.GetHeader("Location") == "http://example.org/x"; // no spaces after the colon
    ok = ok && h.GetHeader("HTTP/1.1") == "HTTP/1.1 200 OK";      // no ':' -> the whole line
    ok = ok && h.GetHeader(":odd") == ":odd";                     // ':' in column 0 -> the whole line
    ok = ok && h.GetHeader("TRANSFER-encoding") == "chunked";
    ok = ok && h.GetHeader("Server") == "";                       // no match -> ""
    LHHttpHeaders empty;
    ok = ok && empty.GetHeader("x") == "";
    if (!ok) { std::printf("0083f650_TEST FAIL\n"); return 1; }
    std::printf("0083f650_TEST PASS\n");
    return 0;
}
