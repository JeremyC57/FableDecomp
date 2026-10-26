class CTCHeroExperience { public: char pad[0xc]; int FishingLevel; };
extern void First_Helper();
extern int Second_Helper();
void __fastcall CTCHeroExperience_AddSuccessfulBlockParry(CTCHeroExperience* self)
{
    First_Helper();
    self->FishingLevel = Second_Helper();
}