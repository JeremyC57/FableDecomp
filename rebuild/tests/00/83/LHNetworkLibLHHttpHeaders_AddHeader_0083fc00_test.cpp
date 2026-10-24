// Behaviour test for LHHttpHeaders::AddHeader (0x0083FC00): "name" + ':' (only when the name does not already end in
// one) + ' ' + value is appended to the header list; the header string member is untouched.
#include <cstdio>
#include <string>
#include <vector>
struct LHHttpHeaders {
    std::vector<std::string> HdrDocumentHeader;
    std::string HeaderString;
    void AddHeader(const std::string& name, const std::string& value)
    {
        std::string header(name);
        if (name[name.size() - 1] != ':')
            header += ":";
        header += " ";
        header += value;
        HdrDocumentHeader.push_back(header);
    }
};
int main()
{
    LHHttpHeaders h;
    h.HeaderString = "kept";
    h.AddHeader(std::string("Host"), std::string("www.example.net"));
    h.AddHeader(std::string("Content-Length:"), std::string("42"));
    h.AddHeader(std::string("X"), std::string(""));
    for (unsigned int i = 0; i < 20; ++i)
        h.AddHeader(std::string("N"), std::string("v"));
    if (h.HdrDocumentHeader.size() != 23u || h.HdrDocumentHeader[0] != "Host: www.example.net"
        || h.HdrDocumentHeader[1] != "Content-Length: 42" || h.HdrDocumentHeader[2] != "X: "
        || h.HdrDocumentHeader[22] != "N: v" || h.HeaderString != "kept") {
        std::printf("0083fc00_TEST FAIL\n");
        return 1;
    }
    std::printf("0083fc00_TEST PASS\n");
    return 0;
}