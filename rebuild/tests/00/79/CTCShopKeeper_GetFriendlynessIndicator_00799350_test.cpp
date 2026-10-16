#include <cstdio>
#include <cstring>
// Behaviour model of CTCShopKeeper::GetFriendlynessIndicator (0x00799350): attitude text key by multiplier difference.
static const char* Indicator(bool hasShop, float base, float attitude)
{
    if (hasShop) {
        float d = base - attitude;
        if (d < 0.2f) return "TEXT_AI_SHOP_KEEPER_SHOW_WARES_LIKES";
        if (d < 0.55f) return "TEXT_AI_SHOP_KEEPER_SHOW_WARES_NEUTRAL";
        return "TEXT_AI_SHOP_KEEPER_SHOW_WARES_DISLIKES";
    }
    return "";
}
int main()
{
    if (std::strcmp(Indicator(true, 1.0f, 0.9f), "TEXT_AI_SHOP_KEEPER_SHOW_WARES_LIKES") ||
        std::strcmp(Indicator(true, 1.0f, 0.6f), "TEXT_AI_SHOP_KEEPER_SHOW_WARES_NEUTRAL") ||
        std::strcmp(Indicator(true, 1.5f, 0.5f), "TEXT_AI_SHOP_KEEPER_SHOW_WARES_DISLIKES") ||
        std::strcmp(Indicator(false, 0, 0), "")) { std::printf("00799350_TEST FAIL\n"); return 1; }
    std::printf("00799350_TEST PASS\n");
    return 0;
}