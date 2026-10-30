// Behaviour test for 0x0083F420 LHNetworkLib::LHHttpHeaderStatus::ParseLocationData (see src/0083f420.cpp): nothing
// happens when hsURI or hsLocation is empty; otherwise the port defaults to 80, an "http://" prefix is skipped, the host
// is substr(start, colon ? colon : slash) (Lionhead's length argument), the port is atoi of substr(colon + 1, slash) when
// a colon was found, and the URI is everything after the first slash.
#include <cstdio>
#include <cstdlib>
#include <string>
struct Status { std::string hsLocation, hsHost, hsURI; unsigned short hsPort; };
static void Parse(Status& s)
{
    if (s.hsURI == "") return;
    if (s.hsLocation == "") return;
    s.hsPort = 80;
    int start = 0;
    if (s.hsLocation.compare(0, 7, "http://") == 0) start = 7;
    int colon = (int)s.hsLocation.find(":", start);
    int slash = (int)s.hsLocation.find("/", start);
    s.hsHost = s.hsLocation.substr(start, colon ? colon : slash);
    if (colon > 0) s.hsPort = (unsigned short)std::atoi(s.hsLocation.substr(colon + 1, slash).c_str());
    s.hsURI = s.hsLocation.substr(slash + 1);
}
int main()
{
    Status a; a.hsLocation = "http://h:81/x"; a.hsURI = "old"; a.hsPort = 0;
    Parse(a);
    bool ok = a.hsPort == 81 && a.hsURI == "x" && a.hsHost.compare(0, 1, "h") == 0;
    Status b; b.hsLocation = "srv/page"; b.hsURI = "old"; b.hsPort = 0;
    Parse(b);
    ok = ok && b.hsPort == 80 && b.hsURI == "page" && b.hsHost == "srv/page"; // no colon: find gives npos, a non-zero length, so the host keeps the rest
    Status c; c.hsLocation = "x"; c.hsURI = ""; c.hsPort = 7;
    Parse(c);
    ok = ok && c.hsPort == 7 && c.hsHost == "";
    std::printf(ok ? "0083f420_TEST PASS\n" : "0083f420_TEST FAIL\n");
    return ok ? 0 : 1;
}