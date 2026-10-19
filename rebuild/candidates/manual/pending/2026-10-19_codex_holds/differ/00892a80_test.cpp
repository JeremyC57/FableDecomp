#include <cstdio>
struct Text { const unsigned short* p; };
struct Ui { int calls; const Text* text; long tag; bool a,b; };
static Ui ui={0,0,0,false,false};
struct Interface { Ui* u; void display(const Text& t) { ++u->calls; u->text=&t; u->tag=0x2a; u->a=true; u->b=true; } };
int main(){ static const unsigned short s[]={1,0}; Text t={s}; Interface i={&ui}; i.display(t); if(ui.calls!=1||ui.text!=&t||ui.tag!=0x2a||!ui.a||!ui.b){std::printf("00892a80_TEST FAIL\n");return 1;} std::printf("00892a80_TEST PASS\n");return 0;}