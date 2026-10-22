#include <cstdio>
struct CUserProfileManager { char pad[0xc]; unsigned char b0c; char pad_lo[24-0xd]; unsigned char blo; char pad_hi[216-(24+1)]; unsigned char GuildMasterEnabledNewProfile; char IsGuildMasterEnabled(); };
char CUserProfileManager::IsGuildMasterEnabled(){ if(this->b0c) return this->GuildMasterEnabledNewProfile; return this->blo; }
int main(){ CUserProfileManager o; o.blo=3; o.GuildMasterEnabledNewProfile=7;
 o.b0c=1; if(o.IsGuildMasterEnabled()!=7){ std::printf("005485f0_TEST FAIL\n"); return 1;}
 o.b0c=0; if(o.IsGuildMasterEnabled()!=3){ std::printf("005485f0_TEST FAIL\n"); return 1;}
 std::printf("005485f0_TEST PASS\n"); return 0;}