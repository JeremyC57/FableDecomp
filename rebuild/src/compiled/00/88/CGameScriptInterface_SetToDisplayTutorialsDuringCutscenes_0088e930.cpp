struct GS2 { char pad[0xcc]; char fcc; };
class CUserProfileManager : public GS2 { public: static CUserProfileManager& __fastcall Get(); };   // 0x0040D2A0 ?Get@CUserProfileManager@@SIAAV1@XZ
void __fastcall fn_0088e930(void* self, int _edx, char arg){ CUserProfileManager::Get().fcc = arg; }