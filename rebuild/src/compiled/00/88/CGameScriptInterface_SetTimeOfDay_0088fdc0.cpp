class CThingDrawInformation
{
public:
    void SetBarPercentage(float percentage);
};

class CGameScriptInterface
{
public:
    void SetTimeOfDay(float timeOfDay) const;
};

struct SelfOverlay
{
    unsigned char pad00[0x04];
    void* field04;
};

struct Field04Overlay
{
    unsigned char pad00[0x1C];
    CThingDrawInformation* field1C;
};



void CGameScriptInterface::SetTimeOfDay(float timeOfDay) const
{
    while (timeOfDay >= 24.0f)
    {
        timeOfDay -= 24.0f;
    }

    const SelfOverlay* self = (const SelfOverlay*)this;
    const Field04Overlay* field04 = (const Field04Overlay*)self->field04;
    field04->field1C->SetBarPercentage(
        timeOfDay * (1.0f / 24.0f));
}