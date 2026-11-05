#include <cstdio>
char CUserProfileManager_OnPostDeviceReset(){ return 1; }
int main(){ if(CUserProfileManager_OnPostDeviceReset()!=1){ std::printf("00406800_TEST FAIL\n"); return 1;}
 std::printf("00406800_TEST PASS\n"); return 0;}