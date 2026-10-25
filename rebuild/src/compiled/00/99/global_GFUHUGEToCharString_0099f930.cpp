// GFUHUGEToCharString(unsigned __int64) -- formats an unsigned 64-bit value as decimal text ("%I64u") and returns it by value
// Leaf: ?GFUHUGEToCharString@@YI?AVCCharString@@_K@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0099F930, 128-byte slot (125 bytes of code).
// Identity: Ego_r.pdb lists ?GFIntToCharString@@YI?AVCCharString@@J@Z, ?GFUIntToCharString@@YI?AVCCharString@@K@Z,
//   ?GFUHUGEToCharString@@YI?AVCCharString@@_K@Z 0x30 apart in that order; retail has GFIntToCharString 0x0099F830 and
//   GFUIntToCharString 0x0099F8B0 (FINDINGS chunk 409) followed by this 64-bit body 0x80 later. A free __fastcall function,
//   not a CCharString static (the forge name "CCharString::FromUInt64" is descriptive). FINDINGS chunk 409.
// Callees: CCharString::Sprintf(const char*, ...) @ 0x0099F1F0 (varargs member: __cdecl with `this` pushed first),
//          CCharString::UnassignString @ 0x0099E9B0 (twice: the copy's inlined assignment and the local's destructor).
// Globals: g_CCharStringInstanceCount @ 0x013BD800 (+1 per constructor, -1 per destructor). Literal "%I64u" @ 0x0129AB04.
// Shape: a `__fastcall` function returning the class by value: the hidden result pointer arrives in ecx (esi) and the
// 64-bit argument stays on the stack (`ret 8`). The named local is default-constructed (null block, counter++),
// formatted, then copied out through the inline copy constructor (null block, counter++, the self-test and the
// inlined operator= body: unassign the old block, share the source block and bump its NoRefs at +0xd), and
// destroyed through the inline destructor (UnassignString, counter--). Same inline shapes as the landed
// copy constructor @ 0x0099EC30 and destructor @ 0x0099EAE0.
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
    void __cdecl Sprintf(const char* format, ...);   // 0x0099F1F0
};

CCharString __fastcall GFUHUGEToCharString(unsigned __int64 value)
{
    CCharString result;
    result.Sprintf("%I64u", value);
    return result;
}