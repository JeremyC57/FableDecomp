#include <cstdio>
static long desc(long ability, long level, long max, const long* d, long terminal) { return level==max+1?terminal:d[level]; }
int main(){ long d[3]={10,20,30}; if(desc(1,1,2,d,99)!=20||desc(1,3,2,d,99)!=99){std::printf("0056fe7c_TEST FAIL\n");return 1;}std::printf("0056fe7c_TEST PASS\n");return 0; }
