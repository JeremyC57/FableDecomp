#include <stdio.h>
struct Spirit{void* thing;};static Spirit* Alloc(void* memory,void* thing){if(!memory)return 0;Spirit* s=(Spirit*)memory;s->thing=thing;return s;}
int main(){Spirit s;int thing;if(Alloc(&s,&thing)!=&s||s.thing!=&thing)return 1;if(Alloc(0,&thing)!=0)return 2;puts("COOP_SPIRIT_CONSTRUCT_PASS");return 0;}