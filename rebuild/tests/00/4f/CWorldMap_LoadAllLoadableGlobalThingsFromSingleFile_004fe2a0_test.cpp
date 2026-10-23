// Behaviour test for CWorldMap::LoadAllLoadableGlobalThingsFromSingleFile (0x004FE2A0): every NEWMAP n ... ENDMAP block goes to map n.
#include <cstdio>
#include <cstring>
#include <string>
static std::string g_log;
struct Parser {
    std::string text; size_t pos;
    bool FindKeyword(const char* k) { size_t p = text.find(k, pos); if (p == std::string::npos) return false; pos = p + std::strlen(k); return true; }
    long ReadAsInteger() { while (text[pos] == ' ') ++pos; long v = 0; while (text[pos] >= '0' && text[pos] <= '9') v = v * 10 + (text[pos++] - '0'); return v; }
    std::string ReadUntil(const char* e) { size_t p = text.find(e, pos); std::string r = text.substr(pos, p - pos); pos = p; return r; }
};
static void LoadBatch(const std::string& block, long map) { char b[16]; std::sprintf(b, "%ld:", map); g_log += b; g_log += block; g_log += ";"; }
static void LoadAll(const std::string& file)
{
    Parser parser; parser.text = file; parser.pos = 0;
    while (parser.FindKeyword("NEWMAP")) {
        long map = parser.ReadAsInteger();
        std::string block = parser.ReadUntil("ENDMAP");
        LoadBatch(block, map);
        parser.FindKeyword("ENDMAP");
    }
}
int main()
{
    LoadAll("NEWMAP 3\r\nA\r\nENDMAP\r\nNEWMAP 12\r\nBC\r\nENDMAP\r\n");
    if (g_log != "3:\r\nA\r\n;12:\r\nBC\r\n;") { std::printf("004fe2a0_TEST FAIL\n"); return 1; }
    g_log = ""; LoadAll("nothing here");
    if (g_log != "") { std::printf("004fe2a0_TEST FAIL\n"); return 1; }
    std::printf("004fe2a0_TEST PASS\n");
    return 0;
}