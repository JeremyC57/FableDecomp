// Behaviour test for CEngineSurfaceCompositionManager::AddCustomMorphTexture (0x00B2C4E0): reopen the custom texture bank
// read-write (create flag when it is empty), add an entry for the name when it has none (then look it up again), clamp the
// power-of-two texture size into [MinRenderTargetSize, MaxCompositeTextureSize], pick the alpha / opaque compressed format,
// write the texture to that entry, reopen the bank read-only and return the entry index with the top bit set.
#include <stdio.h>
#include <string>
#include <map>
#include <vector>
struct Bank {
    unsigned long Size; bool open, rw; unsigned long lastCreate; std::map<std::string, unsigned long> entries; std::vector<std::string> log;
    void Close() { open = false; log.push_back("close"); }
    void OpenRW(unsigned long create) { open = rw = true; lastCreate = create; log.push_back("openrw"); }
    void OpenRO() { open = true; rw = false; log.push_back("openro"); }
    unsigned long Find(const std::string& n) { std::map<std::string, unsigned long>::iterator i = entries.find(n); return i == entries.end() ? 0 : i->second; }
    void Add(const std::string& n) { entries[n] = (unsigned long)entries.size() + 1; ++Size; log.push_back("add"); }
};
static unsigned long HighestSetBit(unsigned long v) { unsigned long b = 0; while (v >>= 1) ++b; return b; }
static unsigned long NextPowerOfTwo(unsigned long v) { if (v == 0) return 0; return 1ul << HighestSetBit(v * 2 - 1); }
struct Written { unsigned long index, w, h; bool alpha; };
static Written written;
static unsigned long AddCustomMorphTexture(Bank& bank, unsigned long texW, unsigned long texH, bool alpha, const std::string& name,
                                           unsigned long minSize, unsigned long maxSize)
{
    unsigned long create = 0;
    if (bank.Size == 0) create = 1;
    bank.Close(); bank.OpenRW(create);
    unsigned long index = bank.Find(name);
    if (index == 0) { bank.Add(name); index = bank.Find(name); }
    unsigned long width = NextPowerOfTwo(texW), height = NextPowerOfTwo(texH);
    if (width < minSize) width = minSize; else if (width > maxSize) width = maxSize;
    if (height < minSize) height = minSize; else if (height > maxSize) height = maxSize;
    written.index = index; written.w = width; written.h = height; written.alpha = alpha;
    bank.Close(); bank.OpenRO();
    return index | 0x80000000;
}
int main()
{
    Bank b = Bank();
    unsigned long r = AddCustomMorphTexture(b, 100, 3, true, "hero", 16, 256);
    if (r != 0x80000001 || b.lastCreate != 1 || written.w != 128 || written.h != 16 || !written.alpha || b.rw) { printf("00b2c4e0_TEST FAIL\n"); return 1; }
    r = AddCustomMorphTexture(b, 1000, 64, false, "hero", 16, 256);
    if (r != 0x80000001 || b.lastCreate != 0 || b.entries.size() != 1 || written.w != 256 || written.h != 64) { printf("00b2c4e0_TEST FAIL\n"); return 1; }
    r = AddCustomMorphTexture(b, 0, 16, false, "cape", 16, 256);
    if (r != 0x80000002 || written.w != 16 || written.h != 16) { printf("00b2c4e0_TEST FAIL\n"); return 1; }
    printf("00b2c4e0_TEST PASS\n"); return 0;
}