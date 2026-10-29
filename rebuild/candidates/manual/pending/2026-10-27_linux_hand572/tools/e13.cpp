struct P { P(); ~P(); void* p; };
struct M { int* Get(); void Rem(int*); };
struct S { int a, b, c, d, e; };
struct Th { unsigned bits; S* Lower(int); 
  __forceinline bool GetTC(S** out) { bool has = (bits >> 10) & 1; if (has) { S* s = Lower(0xaa); *out = s; } return has; } };
int f1(); int f2(); int f3(); void T(); int* R(); M& W(); int g(); Th* C();
void Test(int a)
{
    P seller;
    if (f1()) { T(); return; }
    int* request = R();
    bool accepted;
    {
        int timeout = g() + 20;
        int* reply;
        while ((reply = W().Get()) == 0) {
            if (f1() || f2()) { W().Rem(request); T(); return; }
            if (timeout < g()) { reply = 0; break; }
            if (f3()) {
                do { if (f1()) { T(); return; } } while (f2() == 4);
                if (f1() == 1) { W().Rem(request); T(); return; }
            } else if (f2() == 1) {
                W().Rem(request);
                return;
            }
        }
        if (reply) { W().Rem(reply); accepted = *reply != 0; }
        else { W().Rem(request); accepted = false; }
    }
    S* sp;
    C()->GetTC(&sp);
    int reaction = accepted ? sp->d : sp->e;
    do { if (f1()) { T(); return; } } while (f3() == reaction);
    T();
}
