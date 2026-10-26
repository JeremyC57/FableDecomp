#include <cstdio>
class CCharString;
class CAIStateGroupBase { public: int dummy; };
const CCharString* __fastcall CAIStateGroupBase_GetDebugText(const CAIStateGroupBase* stateGroup)
{
  (void)stateGroup;
  return 0;
}
int main()
{
  CAIStateGroupBase obj; obj.dummy = 1;
  const CCharString* r = CAIStateGroupBase_GetDebugText(&obj);
  if (r == 0) { std::printf("CAIStateGroupBase_008fcfe0_TEST PASS\n"); return 0; }
  std::printf("FAIL\n"); return 1;
}