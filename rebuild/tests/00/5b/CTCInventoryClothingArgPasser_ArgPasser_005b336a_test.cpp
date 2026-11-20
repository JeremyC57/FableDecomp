#include <cstdio>
#include <cstring>
#include <new>
// copy of the ArgPasser model (the pragma is a codegen-only detail)
struct ArgPasser {
    bool  is_suit;
    long  num_suit_parts;
    long  max_suit_parts;
    long  total_item_armor_value_final;
    long  total_cur_armor_value_final;
    float total_item_attractiveness_value_final;
    float total_cur_attractiveness_value_final;
    float total_item_scariness_value_final;
    float total_cur_scariness_value_final;
    float total_item_goodstrength_value_final;
    float total_cur_goodstrength_value_final;
    long  total_price;
    ArgPasser();
};

ArgPasser::ArgPasser()
    : is_suit(false)
    , num_suit_parts(0)
    , max_suit_parts(0)
    , total_item_armor_value_final(0)
    , total_cur_armor_value_final(0)
    , total_item_attractiveness_value_final(0.0f)
    , total_cur_attractiveness_value_final(0.0f)
    , total_item_scariness_value_final(0.0f)
    , total_cur_scariness_value_final(0.0f)
    , total_item_goodstrength_value_final(0.0f)
    , total_cur_goodstrength_value_final(0.0f)
    , total_price(0)
{
}

int main()
{
    unsigned char storage[sizeof(ArgPasser)];
    std::memset(storage, 0xcd, sizeof(storage));
    ArgPasser* a = new (storage) ArgPasser;
    if (a->is_suit || a->num_suit_parts || a->max_suit_parts || a->total_item_armor_value_final ||
        a->total_cur_armor_value_final || a->total_item_attractiveness_value_final != 0.0f ||
        a->total_cur_attractiveness_value_final != 0.0f || a->total_item_scariness_value_final != 0.0f ||
        a->total_cur_scariness_value_final != 0.0f || a->total_item_goodstrength_value_final != 0.0f ||
        a->total_cur_goodstrength_value_final != 0.0f || a->total_price != 0) {
        std::printf("005b336a_TEST FAIL\n");
        return 1;
    }
    std::printf("005b336a_TEST PASS\n");
    return 0;
}