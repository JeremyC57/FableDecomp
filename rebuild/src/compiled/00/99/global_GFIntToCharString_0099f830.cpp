// GFIntToCharString(long value) -- format a signed 32-bit value as decimal text ("%d") and return it by value
// Retail: Fable.exe (TLC, VC7.1) @ 0x0099F830, 116 bytes. FableWin.pdb ?GFIntToCharString@@YI?AVCCharString@@J@Z: a
// free `__fastcall` returning CCharString by value (hidden result pointer in ecx, value in edx), plain `ret`.
// Callees: CCharString::Sprintf(const char*, ...) @ 0x0099F1F0 (varargs member: `__cdecl` with `this` pushed first;
// landed as "Format_vsnprintf"), CCharString::UnassignString() @ 0x0099E9B0 (the copy's inlined assignment and the
// local's destructor).
// Globals: g_CCharStringInstanceCount @ 0x013BD800. Literal "%d" @ 0x01265DE8.
// Shape: the landed GFUHUGEToCharString (0x0099F930, catalog "CCharString::FromUInt64") with a 32-bit argument: named
// local default-constructed, formatted, copied out through the inline copy constructor (no NRVO) and destroyed.
// Leaf: ?GFIntToCharString@@
class CCharStringData;
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

CCharString __fastcall GFIntToCharString(long value)
{
    CCharString result;
    result.Sprintf("%d", value);
    return result;
}