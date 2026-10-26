#include <cstdio>
class C3DVector { public: float x, y, z; };
static void* g_self; static C3DVector* g_vec; static void* g_sub; static bool g_ret;
struct Sub {
  virtual void v0(){}
  virtual void v1(){}
  virtual void v2(){}
  virtual void v3(){}
  virtual void v4(){}
  virtual void v5(){}
  virtual bool slot6(void* a, C3DVector* b){ g_sub=this; g_self=a; g_vec=b; return g_ret; }
};
class CTCCreatureNavigation { public:
  void* f0;
  Sub* ParentThing;
  bool IsNewDestinationGoingToResetNavigation(C3DVector* dest);
};
bool CTCCreatureNavigation::IsNewDestinationGoingToResetNavigation(C3DVector* dest) {
  return this->ParentThing->slot6(this, dest);
}
int main(){
  Sub subObj;
  Sub* subPtr = &subObj;
  CTCCreatureNavigation navObj;
  CTCCreatureNavigation* navPtr = &navObj;
  navObj.f0 = 0;
  navObj.ParentThing = subPtr;
  C3DVector vecObj;
  C3DVector* vecPtr = &vecObj;
  vecObj.x = 1; vecObj.y = 2; vecObj.z = 3;
  g_ret = true;
  bool r = navObj.IsNewDestinationGoingToResetNavigation(vecPtr);
  if (!(r == true && g_sub == (void*)subPtr && g_self == (void*)navPtr && g_vec == vecPtr)) { printf("FAIL1\n"); return 1; }
  g_ret = false;
  r = navObj.IsNewDestinationGoingToResetNavigation(vecPtr);
  if (!(r == false && g_sub == (void*)subPtr && g_self == (void*)navPtr && g_vec == vecPtr)) { printf("FAIL2\n"); return 1; }
  printf("OK_00c2c010\n");
  return 0;
}