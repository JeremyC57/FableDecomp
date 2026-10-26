#pragma optimize("s",on)
class CDataOutputStream { public:
public:
    void WriteFloat(float);   // 0x00993EE0
    void WriteSLONG(long);   // 0x00993EB0
};

class CCombatAbilityData { public:
    int   field_0;
    int   PercentageChance;
    float SecondsDuration;
    float SecondsToWaitBeforeRepeat;
    int   int10;
};

void __fastcall CCombatAbilityData_TransferBinaryOut(CCombatAbilityData* ability, void* edx, CDataOutputStream* stream, int extra)
{
    (void)edx; (void)extra;
    stream->WriteSLONG(ability->PercentageChance);
    stream->WriteFloat(*(float*)&ability->SecondsDuration);
    stream->WriteFloat(*(float*)&ability->SecondsToWaitBeforeRepeat);
    stream->WriteSLONG(ability->int10);
}