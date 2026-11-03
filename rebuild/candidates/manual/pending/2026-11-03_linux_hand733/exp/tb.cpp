// Behaviour test for CUserProfileManager::LoadGameState (0x0040BCA0): a standalone model of the profile-file read path
// (the "FablePro" header, the version and total-length words, the text length = total - stream position), the
// refresh-rate re-clamp against the config cap, and the save-slot rule (a loaded slot name is kept only when it resolves
// to a non-empty path; every slot starts as L"").
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>

static const long g_refreshRateOptions[4] = { 60, 75, 85, 100 };

struct Stream {                          // a byte stream positioned after the 8-byte signature
    const unsigned char* data; unsigned long size; unsigned long pos;
    bool ReadULONG(unsigned int& v) {
        if (pos + 4 > size) return false;
        std::memcpy(&v, data + pos, 4); pos += 4; return true;
    }
};

static bool LoadText(const std::vector<unsigned char>& file, std::string& text)
{
    if (file.size() >= 8 && std::memcmp(&file[0], "FablePro", 8) == 0) {
        Stream s = { &file[0], (unsigned long)file.size(), 8 };
        unsigned int version, length;
        if (!s.ReadULONG(version) || !s.ReadULONG(length)) return false;
        unsigned long count = length - s.pos;           // ReadString(length - GetPosition())
        text.assign((const char*)&file[s.pos], count);
        return true;
    }
    text.assign((const char*)&file[0], file.size());   // legacy: the whole file is the text
    return true;
}

static void ClampRefreshRate(unsigned long& refreshRate, long maxRefreshRate)
{
    if (refreshRate >= 4) refreshRate = 0;
    if (maxRefreshRate != -1 && g_refreshRateOptions[refreshRate] > maxRefreshRate) {
        unsigned long index = refreshRate;
        while (index >= 0 && g_refreshRateOptions[index] > maxRefreshRate) --index;
        refreshRate = index;
    }
}

static std::wstring PathFor(const std::wstring& name) { return name == L"missing" ? L"" : L"C:\\saves\\" + name; }

int main()
{
    // Header form: "FablePro", version 0, total length = text + 0x10.
    const char* body = "ProfileName \"Hero\";";
    std::vector<unsigned char> file(8 + 8 + std::strlen(body));
    std::memcpy(&file[0], "FablePro", 8);
    unsigned int zero = 0, total = (unsigned int)std::strlen(body) + 0x10;
    std::memcpy(&file[8], &zero, 4); std::memcpy(&file[12], &total, 4);
    std::memcpy(&file[16], body, std::strlen(body));
    std::string text;
    if (!LoadText(file, text) || text != body) { std::printf("0040bca0_TEST FAIL\n"); return 1; }
    // Legacy form: no signature, the whole file is read as text.
    std::vector<unsigned char> legacy((const unsigned char*)body, (const unsigned char*)body + std::strlen(body));
    if (!LoadText(legacy, text) || text != body) { std::printf("0040bca0_TEST FAIL\n"); return 1; }
    // Refresh clamp: out of range resets to 0; a cap walks down to the first option within it.
    unsigned long rate = 7; ClampRefreshRate(rate, -1);
    if (rate != 0) { std::printf("0040bca0_TEST FAIL\n"); return 1; }
    rate = 3; ClampRefreshRate(rate, 80);
    if (rate != 1) { std::printf("0040bca0_TEST FAIL\n"); return 1; }
    // Save slots: 50 entries, a name survives only when its path is non-empty.
    std::vector<std::wstring> slots; slots.resize(50, L"");
    const wchar_t* loaded[3] = { L"slot1", L"missing", L"" };
    for (int i = 0; i < 3; ++i)
        if (std::wstring(loaded[i]) != L"" && PathFor(loaded[i]) != L"") slots[i] = loaded[i];
    if (slots.size() != 50 || slots[0] != L"slot1" || slots[1] != L"" || slots[2] != L"") {
        std::printf("0040bca0_TEST FAIL\n"); return 1;
    }
    std::printf("0040bca0_TEST PASS\n");
    return 0;
}
