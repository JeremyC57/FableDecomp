#pragma optimize("s",on)
struct CBinaryStream {
    void PutInt(int v);
    void PutFloat(float v);
};

struct CCombatAbilityData {
    int   field_0;
    int   PercentageChance;
    float SecondsDuration;
    float SecondsToWaitBeforeRepeat;
    int   int10;
};

void __fastcall CCombatAbilityData_TransferBinaryOut(CCombatAbilityData* ability, void* edx, CBinaryStream* stream, int extra)
{
    (void)edx; (void)extra;
    stream->PutInt(ability->PercentageChance);
    stream->PutFloat(*(float*)&ability->SecondsDuration);
    stream->PutFloat(*(float*)&ability->SecondsToWaitBeforeRepeat);
    stream->PutInt(ability->int10);
}