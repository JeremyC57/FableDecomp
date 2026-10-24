// GFUIntToCharString(unsigned long value) -- format an unsigned 32-bit value as decimal text ("%u") and return it by value
// Retail: Fable.exe (TLC, VC7.1) @ 0x0099F8B0, 116 bytes. FableWin.pdb ?GFUIntToCharString@@YI?AVCCharString@@K@Z: a
// free `__fastcall` returning CCharString by value (hidden result pointer in ecx, value in edx), plain `ret`.
// Callees: CCharString::Sprintf(const char*, ...) @ 0x0099F1F0 (varargs member: `__cdecl` with `this` pushed first;
// landed as "Format_vsnprintf"), CCharString::UnassignString() @ 0x0099E9B0 (the copy's inlined assignment and the
// local's destructor).
// Globals: g_CCharStringInstanceCount @ 0x013BD800. Literal "%u" @ 0x0129AB00.
// Shape: the landed GFUHUGEToCharString (0x0099F930, catalog "CCharString::FromUInt64") with an unsigned 32-bit argument; sibling of GFIntToCharString 0x0099F830: named
// local default-constructed, formatted, copied out through the inline copy constructor (no NRVO) and destroyed.
// Leaf: ?GFUIntToCharString@@
struct CCharStringData;
extern long g_CCharStringInstanceCount;        // 0x013BD800

class CCharString {
public:
    CCharStringData* PStringData;               // +0x00
    CCharString() : PStringData(0) { ++g_CCharStringInstanceCount; }
    CCharString(const CCharString& other) : PStringData(0)
    {
        ++g_CCharStringInstanceCount;
        if (&other != this) {
            if (PStringData != 0)
                UnassignString();
            CCharStringData* data = other.PStringData;
            if (data != 0) {
                PStringData = data;
                ++*(long*)((char*)data + 0xd);   // CCharStringData::NoRefs
            }
        }
    }
    ~CCharString() { UnassignString(); --g_CCharStringInstanceCount; }
    void UnassignString();                      // 0x0099E9B0
    void Sprintf(const char* format, ...);      // 0x0099F1F0
};

CCharString __fastcall GFUIntToCharString(unsigned long value)
{
    CCharString result;
    result.Sprintf("%u", value);
    return result;
}