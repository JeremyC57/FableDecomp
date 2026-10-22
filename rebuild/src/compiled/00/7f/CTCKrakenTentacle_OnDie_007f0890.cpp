#pragma optimize("s",on)

struct Pair;

extern "C" void __fastcall Dtor_first(Pair* tentacle);
extern "C" void __fastcall Dtor_second(Pair* tentacle);
extern "C" void __fastcall Dtor_node(Pair* tentacle);

void __fastcall Dest_val(Pair* tentacle)
{
    Dtor_first(tentacle);
    Dtor_second(tentacle);
    Dtor_node(tentacle);
}