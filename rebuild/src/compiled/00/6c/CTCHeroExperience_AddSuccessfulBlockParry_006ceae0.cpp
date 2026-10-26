class CTCHeroExperience { public: char pad[0xc]; int FishingLevel; };
extern void First_Helper();
class CWorld {
public:
    static long __fastcall GetFrame();   // 0x0049D870
};   // 0x0049D870
void __fastcall CTCHeroExperience_AddSuccessfulBlockParry(CTCHeroExperience* self)
{
    First_Helper();
    self->FishingLevel = CWorld::GetFrame();
}