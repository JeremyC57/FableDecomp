#include <cstdio>
struct Entry { bool present; int state; };
static bool save(Entry* e, int n) { for (int i=0;i<n;++i) if (e[i].present && (e[i].state==1 || e[i].state==2)) return true; return false; }
int main(){ Entry a[2]={{true,0},{true,2}}; Entry b[1]={{true,3}}; if(!save(a,2)||save(b,1)){std::printf("0058376c_TEST FAIL\n");return 1;} std::printf("0058376c_TEST PASS\n"); return 0; }