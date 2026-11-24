#pragma optimize("s",on)
// Leaf: ?CTCKraken_EndAttackBeam@@YIXPAVCTCKraken@@@Z

class CTCKraken { public:
    char pad[0x64];
    int  beamState;
};

extern "C" void __fastcall Kraken_PreEnd(CTCKraken* self);
extern "C" void __fastcall Kraken_Finish(CTCKraken* self);

void __fastcall CTCKraken_EndAttackBeam(CTCKraken* self)
{
    self->beamState = 3;
    Kraken_PreEnd(self);
    Kraken_Finish(self);
}