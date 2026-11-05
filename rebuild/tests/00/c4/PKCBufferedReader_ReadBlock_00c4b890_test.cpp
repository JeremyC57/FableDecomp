// Behaviour test for 0x00c4b890: PKCBufferedReader::ReadBlock -- loop the block copier until size bytes are read, refuse reads past the end
#include <cstdio>
#include <cstring>
#include <string>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { g_fail = 1; std::printf("check failed line %d\n", __LINE__); } } while (0)
static std::string g_log;
struct Reader {
    unsigned int blockSize, numBlocks, size, firstBlock, position; const char* data;
    unsigned int CopyOutOfBuffers(void* buffer, unsigned int n) {   // at most one block per call
        unsigned int pos = firstBlock * blockSize + position; unsigned int take = n < blockSize ? n : blockSize;
        std::memcpy(buffer, data + pos, take); position += take; return take; }
    bool ReadBlock(void* buffer, unsigned int n) {
        if (n == 0) return true;
        unsigned int capacity = numBlocks * blockSize;
        if (n > capacity) g_log += "ratio";
        unsigned int pos = firstBlock * blockSize + position;
        if (pos + n > size) { g_log += "past-end"; return false; }
        do { unsigned int c = CopyOutOfBuffers(buffer, n); n -= c; buffer = (char*)buffer + c; } while (n != 0);
        return true; } };
int main()
{
    const char* text = "abcdefghijklmnopqrstuvwxyz";
    Reader r; r.blockSize = 4; r.numBlocks = 2; r.size = 26; r.firstBlock = 0; r.position = 0; r.data = text;
    char buf[32]; std::memset(buf, 0, sizeof buf);
    CHECK(r.ReadBlock(buf, 0)); CHECK(g_log.empty());
    CHECK(r.ReadBlock(buf, 10)); CHECK(std::memcmp(buf, "abcdefghij", 10) == 0); CHECK(g_log == "ratio");
    CHECK(!r.ReadBlock(buf, 20)); CHECK(g_log == "ratioratiopast-end");
    if (g_fail) { std::printf("00c4b890_TEST FAIL\n"); return 1; }
    std::printf("00c4b890_TEST PASS\n");
    return 0;
}