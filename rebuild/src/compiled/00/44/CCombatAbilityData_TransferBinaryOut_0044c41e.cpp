// Leaf: ?TransferBinaryOut@CCombatAbilityData@@UBEXAAVCDataOutputStream@@ABV1@@Z
#pragma optimize("s",on)
class CDataOutputStream { public:
public:
    void WriteFloat(float);   // 0x00993EE0
    void WriteSLONG(long);   // 0x00993EB0
};

class CCombatAbilityData { public:
        int   PercentageChance;
    float SecondsDuration;
    float SecondsToWaitBeforeRepeat;
    int   int10;
    virtual void TransferBinaryOut(class CDataOutputStream&, const class CCombatAbilityData&) const;
};

void CCombatAbilityData::TransferBinaryOut(class CDataOutputStream& stream, const class CCombatAbilityData& extra) const
{
    CCombatAbilityData* ability = const_cast<CCombatAbilityData*>(this);
(void)extra;
    stream.WriteSLONG(ability->PercentageChance);
    stream.WriteFloat(*(float*)&ability->SecondsDuration);
    stream.WriteFloat(*(float*)&ability->SecondsToWaitBeforeRepeat);
    stream.WriteSLONG(ability->int10);
}
// Flatten route (flatten_pilot v4, NF324): CCombatAbilityData_TransferBinaryOut -> CCombatAbilityData::TransferBinaryOut, the RTTI-placed PDB member; class model: the row's local class model.
