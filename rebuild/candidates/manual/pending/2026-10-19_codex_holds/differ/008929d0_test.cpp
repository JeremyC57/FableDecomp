#include <cstdio>
struct Text { const char* p; };
struct Ui { int calls; const Text* text; long tag; bool a,b; };
static Ui ui = {0, 0, 0, false, false};
struct Interface { Ui* u; void display(const Text& t) { ++u->calls; u->text=&t; u->tag=0x2a; u->a=true; u->b=true; } };
int main() { Text t={"hello"}; Interface i={&ui}; i.display(t); if(ui.calls!=1 || ui.text!=&t || ui.tag!=0x2a || !ui.a || !ui.b) { std::printf("008929d0_TEST FAIL\n"); return 1; } std::printf("008929d0_TEST PASS\n"); return 0; }