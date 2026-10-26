#pragma optimize("s",on)

class CTCKrakenTentacle {
public:
    void RemoveObstruction();   // 0x007F0810
};
struct Pair;

extern "C" void __fastcall Dtor_first(Pair* tentacle);
extern "C" void __fastcall Dtor_node(Pair* tentacle);

void __fastcall Dest_val(Pair* tentacle)
{
    Dtor_first(tentacle);
    ((CTCKrakenTentacle*)(tentacle))->RemoveObstruction();
    Dtor_node(tentacle);
}