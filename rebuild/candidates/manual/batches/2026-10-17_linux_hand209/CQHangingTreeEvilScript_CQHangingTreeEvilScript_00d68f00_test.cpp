#include <cstdio>
struct S{long z[8];S(){for(int i=0;i<8;++i)z[i]=0;}};int main(){S s;if(s.z[0]||s.z[7]){std::printf("00d68f00_TEST FAIL\n");return 1;}std::printf("00d68f00_TEST PASS\n");return 0;}
