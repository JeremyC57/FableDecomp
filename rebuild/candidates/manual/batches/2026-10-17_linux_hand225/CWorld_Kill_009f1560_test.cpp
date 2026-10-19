#include <cstdio>
struct Tree { unsigned long count; Tree():count(0){} void clear(){count=0;} };
struct Owner { Tree a,b,c; ~Owner(){c.clear();b.clear();a.clear();} };
int main(){Owner x; x.a.count=2;x.b.count=1;x.~Owner();if(x.a.count||x.b.count||x.c.count){std::printf("009f1560_TEST FAIL\n");return 1;}std::printf("009f1560_TEST PASS\n");return 0;}
