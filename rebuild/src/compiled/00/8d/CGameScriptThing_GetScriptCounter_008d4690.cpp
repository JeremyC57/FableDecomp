int __fastcall CGameScriptThing_GetScriptCounter(const unsigned char* thing)
{
    return *reinterpret_cast<const int*>(thing + 20);
}