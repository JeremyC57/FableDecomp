extern int g_attTable[];

class CCharString { public: void* Data; const CCharString& operator=(const CCharString& other); };   // 0x0099EFB0
struct StrDest;

void __stdcall CGameScriptInterface_GetOpinionAttitudeAsString(int attitude, StrDest* dest)
{
    *(CCharString*)dest = *(const CCharString*)&g_attTable[attitude];
}