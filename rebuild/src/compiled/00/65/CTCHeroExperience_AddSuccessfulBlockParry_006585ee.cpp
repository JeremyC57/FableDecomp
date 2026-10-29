class CTCHeroExperience { public: char pad[0xd8]; int value; };
extern "C" void __cdecl helper_prep(void);
class CWorld { public: static long __fastcall GetFrame(); };   // 0x0049D870 ?GetFrame@CWorld@@SIJXZ
void __fastcall CTCHeroExperience_AddSuccessfulBlockParry(CTCHeroExperience* self)
{
    helper_prep();
    self->value = CWorld::GetFrame();
}