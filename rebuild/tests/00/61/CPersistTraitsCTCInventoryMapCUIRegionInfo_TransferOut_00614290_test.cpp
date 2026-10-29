// Behaviour test (0x00614290): CPersistTraits<CTCInventoryMap::CUIRegionInfo>::TransferOut writes the ten region counts as one comma-separated tuple
#include <cstdio>
#include <cstring>
#include <cmath>
// Text-builder model: a fixed-buffer CCharString with operator+= and the scalar traits' TransferOut formatting.
class CCharString { public:
    char Text[256];
    CCharString() { Text[0] = 0; }
    CCharString(const char* s) { std::strcpy(Text, s); }
    const CCharString& operator+=(const char* s) { std::strcat(Text, s); return *this; }
    bool Equals(const char* s) const { return std::strcmp(Text, s) == 0; }
};
class CPersistContext { public: int Unused; };
static void TransferOutLong(CPersistContext*, CCharString* out, const int* value) { char b[32]; std::sprintf(b, "%d", *value); *out += b; }
static void TransferOutFloat(CPersistContext*, CCharString* out, const float* value) { char b[32]; std::sprintf(b, "%g", (double)*value); *out += b; }
static void TransferOutString(CPersistContext*, CCharString* out, const CCharString* value) { *out += "\""; *out += value->Text; *out += "\""; }
struct CUIRegionInfo { int Region, Shops, BarberShops, TatooShops, TitleShops, ShopsForSale, HousesForSale, ShopsOwned, HousesOwned, Taverns; };
static void TransferOut(CPersistContext* context, CCharString* out, const CUIRegionInfo* value)
{
    *out += "(";
    TransferOutLong(context, out, &value->Region); *out += ",";
    TransferOutLong(context, out, &value->Shops); *out += ",";
    TransferOutLong(context, out, &value->BarberShops); *out += ",";
    TransferOutLong(context, out, &value->TatooShops); *out += ",";
    TransferOutLong(context, out, &value->TitleShops); *out += ",";
    TransferOutLong(context, out, &value->ShopsForSale); *out += ",";
    TransferOutLong(context, out, &value->HousesForSale); *out += ",";
    TransferOutLong(context, out, &value->ShopsOwned); *out += ",";
    TransferOutLong(context, out, &value->HousesOwned); *out += ",";
    TransferOutLong(context, out, &value->Taverns);
    *out += ")";
}
int main()
{
    CPersistContext ctx; ctx.Unused = 0;
    CUIRegionInfo r; r.Region = 1; r.Shops = 2; r.BarberShops = 3; r.TatooShops = 4; r.TitleShops = 5; r.ShopsForSale = 6;
    r.HousesForSale = 7; r.ShopsOwned = 8; r.HousesOwned = 9; r.Taverns = 10;
    CCharString out; TransferOut(&ctx, &out, &r);
    if (!out.Equals("(1,2,3,4,5,6,7,8,9,10)")) { std::printf("00614290_TEST FAIL\n"); return 1; }
    std::printf("00614290_TEST PASS\n");
    return 0;
}