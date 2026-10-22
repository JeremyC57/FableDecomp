#include <cstdio>

struct CBinaryStream {
    int intCalls; int floatCalls; int lastInt; float lastFloat;
    void PutInt(int v){ intCalls++; lastInt=v; }
    void PutFloat(float v){ floatCalls++; lastFloat=v; }
};
struct CCombatAbilityData { int field_0; int PercentageChance; float SecondsDuration; float SecondsToWaitBeforeRepeat; int int10; };

static void TransferBinaryOut(CCombatAbilityData* ability, CBinaryStream* stream)
{
    stream->PutInt(ability->PercentageChance);
    stream->PutFloat(*(volatile float*)&ability->SecondsDuration);
    stream->PutFloat(*(volatile float*)&ability->SecondsToWaitBeforeRepeat);
    stream->PutInt(ability->int10);
}

int main(){
    CCombatAbilityData d; d.field_0=1; d.PercentageChance=42; d.SecondsDuration=1.5f; d.SecondsToWaitBeforeRepeat=2.5f; d.int10=99;
    CBinaryStream s; s.intCalls=0; s.floatCalls=0; s.lastInt=0; s.lastFloat=0.0f;
    TransferBinaryOut(&d,&s);
    if(s.intCalls==2 && s.floatCalls==2 && s.lastInt==99 && s.lastFloat==2.5f){
        std::printf("CCombatAbilityData_0044c41e_TEST PASS\n"); return 0;
    }
    std::printf("FAIL i=%d f=%d li=%d lf=%f\n", s.intCalls, s.floatCalls, s.lastInt, s.lastFloat);
    return 1;
}