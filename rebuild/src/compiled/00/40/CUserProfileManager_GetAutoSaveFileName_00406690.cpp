class CWideString { public: int d; CWideString(const unsigned short* lit); };
class CUserProfileManager { public: CWideString GetAutoSaveFileName(); };
CWideString CUserProfileManager::GetAutoSaveFileName()
{ return CWideString((const unsigned short*)0x122de9c); }