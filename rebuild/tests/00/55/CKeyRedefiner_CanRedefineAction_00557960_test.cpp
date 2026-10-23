#include <cstdio>
unsigned char g_redefineFlag=0;
#define KEY_REDEFINER_FLAG_ADDRESS (&g_redefineFlag)
// CKeyRedefiner::CanRedefineAction(long) -- classify category IDs through retail's dense switch table (0x00557960, 175 bytes).
// Grade: RELOCATION_MATCH -- q.py verified the exact 175-byte body with embedded switch table.
// Owner established by the CKeyRedefiner::ProcessEvent caller at 0x00557f51; method name is descriptive.
// No shared CKeyRedefiner header exists; this helper does not access instance fields.
struct CKeyRedefiner { bool CanRedefineAction(long category); };
#ifndef KEY_REDEFINER_FLAG_ADDRESS
#define KEY_REDEFINER_FLAG_ADDRESS 0x013B864F
#endif
bool CKeyRedefiner::CanRedefineAction(long category)
{
    switch (category) {
    case 1: case 89: case 90: case 91: case 98: case 118: case 119:
        return false;
    case 58:
        if (*(unsigned char*)KEY_REDEFINER_FLAG_ADDRESS == 0) break;
        return false;
    default:
        break;
    }
    return true;
}

int main() {
 CKeyRedefiner object;
 for (int flag=0;flag<3;++flag) {
  g_redefineFlag=(unsigned char)flag;
  for (long action=-1;action<=121;++action) {
   bool expected=!(action==1 || action==89 || action==90 || action==91 || action==98 || action==118 || action==119 || (action==58 && flag!=0));
   if (object.CanRedefineAction(action)!=expected) { std::printf("00557960_TEST FAIL\n"); return 1; }
  }
  if (!object.CanRedefineAction(-2147483647L-1) || !object.CanRedefineAction(2147483647L)) { std::printf("00557960_TEST FAIL\n"); return 1; }
 }
 std::printf("00557960_TEST PASS\n"); return 0;
}