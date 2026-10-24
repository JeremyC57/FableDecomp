// ArgPasser::ArgPasser() -- zero the clothing-description argument block: not a suit, no suit parts, zero armour / attractiveness / scariness / good-strength totals and price
// Leaf: ??0ArgPasser
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005B336A, length 52 bytes (unaligned: the CTCInventoryClothing size TU).
// Callers: CTCInventoryClothing::GetItemDescription 0x005B5CF8 (0x005B5D06), GetItemDescriptionNoArmourRating 0x005B62FE
// (0x005B630C), GetItemDescriptionArmourRating 0x005B6767 (0x005B6774).
// Layout: Ego_r.pdb ArgPasser (0x30 bytes): is_suit +0, num_suit_parts +4, max_suit_parts +8, total_item_armor_value_final
// +0xc, total_cur_armor_value_final +0x10, six float totals +0x14..+0x28, total_price +0x2c.
// Shape: size TU (`#pragma optimize("s", on)`: every float member is its own `fldz ; fstp`, the long/bool zeroes share ecx);
// the initialiser list runs in declaration order and VC interleaves the x87 and integer stores; `mov eax,ecx` returns this.
#pragma optimize("s", on)

struct ArgPasser {
    bool  is_suit;                                  // +0x00
    long  num_suit_parts;                           // +0x04
    long  max_suit_parts;                           // +0x08
    long  total_item_armor_value_final;             // +0x0c
    long  total_cur_armor_value_final;              // +0x10
    float total_item_attractiveness_value_final;    // +0x14
    float total_cur_attractiveness_value_final;     // +0x18
    float total_item_scariness_value_final;         // +0x1c
    float total_cur_scariness_value_final;          // +0x20
    float total_item_goodstrength_value_final;      // +0x24
    float total_cur_goodstrength_value_final;       // +0x28
    long  total_price;                              // +0x2c
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