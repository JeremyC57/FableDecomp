// Behaviour test for LHHttpHeaderStatus::Reset (0x0083FB60): code -1, header lines cleared (header string kept),
// location / host / URI emptied, port 0, document size untouched.
#include <cstdio>
#include <string>
#include <vector>
struct LHHttpHeaders {
    std::vector<std::string> HdrDocumentHeader;
    std::string HeaderString;
};
struct LHHttpHeaderStatus {
    int hsServerCode;
    LHHttpHeaders hsDocumentHeader;
    std::string hsLocation;
    long hsDocumentSize;
    std::string hsHost;
    std::string hsURI;
    unsigned short hsPort;
    void Reset()
    {
        hsServerCode = -1;
        hsDocumentHeader.HdrDocumentHeader.clear();
        hsLocation = "";
        hsHost = "";
        hsURI = "";
        hsPort = 0;
    }
};
int main()
{
    LHHttpHeaderStatus s;
    s.hsServerCode = 200;
    s.hsDocumentHeader.HdrDocumentHeader.push_back("Content-Length: 5");
    s.hsDocumentHeader.HeaderString = "kept";
    s.hsLocation = "http://x/";
    s.hsDocumentSize = 5;
    s.hsHost = "x";
    s.hsURI = "/";
    s.hsPort = 80;
    s.Reset();
    if (s.hsServerCode != -1 || !s.hsDocumentHeader.HdrDocumentHeader.empty() || s.hsDocumentHeader.HeaderString != "kept"
        || !s.hsLocation.empty() || !s.hsHost.empty() || !s.hsURI.empty() || s.hsPort != 0 || s.hsDocumentSize != 5) {
        std::printf("0083fb60_TEST FAIL\n");
        return 1;
    }
    std::printf("0083fb60_TEST PASS\n");
    return 0;
}