// CCoopSpiritDef::StatGetTypeName() -- return the def type name "CCoopSpiritDef" as a CCharString
// Retail: Fable.exe @ 0x0066FA10, 19 bytes, plain `ret`: the hidden result pointer arrives in ecx and nothing is on
// the stack, which is a __fastcall static returning a class by value (VC passes the hidden pointer as the first
// register argument). The body is CCharString::CCharString(const char*, int = -1) (0x0099EBF0) on the result slot
// with the literal at 0x012358E8.
class CCharString {
public:
    CCharString(const char* text, int length);   // 0x0099EBF0
private:
    char* m_pData;
};
class CCoopSpiritDef {
public:
    static CCharString __fastcall StatGetTypeName();
};
CCharString __fastcall CCoopSpiritDef::StatGetTypeName()
{
    return CCharString("CCoopSpiritDef", -1);
}
