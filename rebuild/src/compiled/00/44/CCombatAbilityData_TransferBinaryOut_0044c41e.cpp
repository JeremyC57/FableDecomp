#pragma optimize("s",on)
struct CBinaryStream {
    void PutInt(int v);
    void PutFloat(float v);
};

struct CCombatAbilityData {
    int   field_0;
    int   int4;
    float SecondsDuration;
    float SecondsToWaitBeforeRepeat;
    int   int10;
};

void __fastcall CCombatAbilityData_TransferBinaryOut(CCombatAbilityData* self, void* edx, CBinaryStream* stream, int extra)
{
    (void)edx; (void)extra;
    stream->PutInt(self->int4);
    stream->PutFloat(*(volatile float*)&self->SecondsDuration);
    stream->PutFloat(*(volatile float*)&self->SecondsToWaitBeforeRepeat);
    stream->PutInt(self->int10);
}