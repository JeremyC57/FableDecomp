#include <cstdio>
struct CUserProfileManager { char pad[0xc]; unsigned char b0c; char pad_lo[25-0xd]; unsigned char blo; char pad_hi[217-(25+1)]; unsigned char HUDTooltipEnabledNewProfile; char IsHUDTooltipEnabled(); };
char CUserProfileManager::IsHUDTooltipEnabled(){ if(this->b0c) return this->HUDTooltipEnabledNewProfile; return this->blo; }
int main(){ CUserProfileManager o; o.blo=3; o.HUDTooltipEnabledNewProfile=7;
 o.b0c=1; if(o.IsHUDTooltipEnabled()!=7){ std::printf("00548610_TEST FAIL\n"); return 1;}
 o.b0c=0; if(o.IsHUDTooltipEnabled()!=3){ std::printf("00548610_TEST FAIL\n"); return 1;}
 std::printf("00548610_TEST PASS\n"); return 0;}