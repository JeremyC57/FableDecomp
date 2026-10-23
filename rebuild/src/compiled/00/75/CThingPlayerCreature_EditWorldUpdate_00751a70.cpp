struct Sub; extern unsigned char __fastcall ewu_probe_0085f200(Sub*);
struct S { char pad[4]; Sub* sub; };
bool __fastcall EditWorldUpdate(S* self){ return ewu_probe_0085f200(self->sub) ? true : false; }