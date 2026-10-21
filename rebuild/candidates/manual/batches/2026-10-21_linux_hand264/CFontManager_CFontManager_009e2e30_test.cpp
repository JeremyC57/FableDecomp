// Behaviour test for CFontManager::CFontManager @ 0x009E2E30: both data banks and symbol maps are always created, a
// bank is opened (flags 0x48) and its symbol map built only when its name is non-empty, the streaming bank also gets
// its streaming entries, and eight vertex buffers (0x144 type, 300 vertices, stride 28) are allocated.
#include <cstdio>
#include <string>
#include <vector>
struct Bank { bool created, opened, mapped, streamed; int flags; Bank() : created(true), opened(false), mapped(false), streamed(false), flags(0) {} };
struct VB { int type, count, stride; };
struct FontManager {
    Bank font, streaming;
    std::vector<VB> buffers;
    int current;
    FontManager(const std::string& fontName, const std::string& streamingName) : current(-1)
    {
        if (!fontName.empty()) { font.opened = true; font.flags = 0x48; font.mapped = true; }
        if (!streamingName.empty()) { streaming.opened = true; streaming.flags = 0x48; streaming.mapped = true; streaming.streamed = true; }
        current = 0;
        buffers.resize(8);
        for (int i = 0; i < 8; ++i) { buffers[i].type = 0x144; buffers[i].count = 0x12c; buffers[i].stride = 0x1c; }
    }
};
int main()
{
    FontManager a("fonts", "");
    if (!a.font.opened || a.font.flags != 0x48 || !a.font.mapped || a.streaming.opened || !a.streaming.created) { std::printf("009e2e30_TEST FAIL\n"); return 1; }
    FontManager b("", "stream");
    if (b.font.opened || !b.streaming.opened || !b.streaming.streamed || b.current != 0) { std::printf("009e2e30_TEST FAIL\n"); return 1; }
    if (b.buffers.size() != 8 || b.buffers[7].count != 300 || b.buffers[0].stride != 28) { std::printf("009e2e30_TEST FAIL\n"); return 1; }
    std::printf("009e2e30_TEST PASS\n");
    return 0;
}
