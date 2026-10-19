#include <cstdio>
struct Def { long refs; void AddRef() { ++refs; } };
struct Ptr { void* p; };
struct Model { Def* def; Ptr item; long id; unsigned char active,loop,finished,pad; long start,end,speed; };
static Model copy_model(const Model& o) { Model n=o; if(n.def) n.def->AddRef(); return n; }
int main() { Def d={1}; Model a={&d,{(void*)0x1234},7,1,0,1,0,10,20,2}; Model b=copy_model(a); if(b.def!=&d || d.refs!=2 || b.item.p!=(void*)0x1234 || b.id!=7 || b.speed!=2) { std::printf("0084bd40_TEST FAIL\n"); return 1; } std::printf("0084bd40_TEST PASS\n"); return 0; }