// CModeSummonerSummon::OnActivate  @ 008d68e0
// __fastcall void OnActivate(CModeSummonerSummon* this)

struct CSomeState {
    char pad0[0x10];
    unsigned char flag;   // +0x10
    void Notify(int a, int b);   // thiscall member @ 0x833a10
};

// base-class OnActivate (direct thiscall member call @ 0x8621a0)
void __fastcall BaseMode_OnActivate(void* mode);

struct CModeSummonerSummon {
    void* vtbl;          // +0x00
    CSomeState* state;   // +0x04
    void OnActivate();
};

void CModeSummonerSummon::OnActivate()
{
    BaseMode_OnActivate(this);
    CSomeState* s = this->state;
    if (s->flag == 1)
        s->Notify(1, 0);
}