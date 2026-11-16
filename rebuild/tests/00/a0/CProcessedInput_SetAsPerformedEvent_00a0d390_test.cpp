#include <cstdio>
struct S { char pad[4]; int f4; char pad2[0xa0]; unsigned char b_a8; };
static void SetAsPerformedEvent(S* input){ input->b_a8=0; input->f4=2; }   // NF37: CProcessedInput::SetAsPerformedEvent (0x00A0D390): NoGameEvents +0xA8, Type +0x04
int main(){ S o; o.f4=9;o.b_a8=9; SetAsPerformedEvent(&o); if(o.f4!=2||o.b_a8!=0){ std::printf("00a0d390_TEST FAIL\n"); return 1;}
 std::printf("00a0d390_TEST PASS\n"); return 0;}