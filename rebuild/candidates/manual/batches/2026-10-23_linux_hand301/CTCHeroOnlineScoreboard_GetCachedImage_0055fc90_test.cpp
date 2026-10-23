// Behaviour test for CTCHeroOnlineScoreboard::GetCachedImage (0055fc90): cache hit returns the cached texture; a miss loads (when the logo file exists) and caches, also caching a null result
#include <cstdio>

struct Texture { int width, height; };
struct CacheEntry { int id; Texture* texture; };
struct Cache {
    CacheEntry entries[8];
    int count;
    Cache() : count(0) {}
    CacheEntry* find(int id) { for (int i = 0; i < count; ++i) if (entries[i].id == id) return &entries[i]; return 0; }
};
static Texture g_textures[8];
static int g_created = 0;

static Texture* GetCachedImage(Cache& cache, int id, bool fileExists)
{
    Texture* texture = 0;
    CacheEntry* it = cache.find(id);
    if (!it) {
        if (fileExists) {
            texture = &g_textures[g_created++];
            texture->width = 256;
            texture->height = 256;
        }
        cache.entries[cache.count].id = id;
        cache.entries[cache.count].texture = texture;
        ++cache.count;
    } else {
        texture = it->texture;
    }
    return texture;
}

int main()
{
    Cache cache;
    Texture* a = GetCachedImage(cache, 5, true);
    Texture* b = GetCachedImage(cache, 5, false);
    Texture* c = GetCachedImage(cache, 9, false);
    Texture* d = GetCachedImage(cache, 9, true);
    if (!a || a != b || a->width != 256 || c != 0 || d != 0 || g_created != 1 || cache.count != 2) {
        std::printf("0055fc90_TEST FAIL\n");
        return 1;
    }
    std::printf("0055fc90_TEST PASS\n");
    return 0;
}
