// GFByteSizeToCharString(unsigned long size, long decimals) -- formats a byte count as "<n> bytes" below 4096, else as "<x> KB" / "<x> MB" with `decimals` decimal places
// Leaf: ?GFByteSizeToCharString@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x0099FA90, 203 bytes (+ int3 padding), `ret 4`. Catalog name "FormatFileSizeString";
// the Ego_r PDB name is `?GFByteSizeToCharString@@YI?AVCCharString@@KJ@Z` (= `CCharString __fastcall (unsigned long, long)`).
// Callees: GFIntToCharString(long) @ 0x0099F830 ("%d"), GFFloatToCharString(double, long) @ 0x009A08B0,
// operator+(const CCharString&, const char*) @ 0x0099F600 (all `__fastcall`, hidden result pointer in ecx),
// CCharString::UnassignString @ 0x0099E9B0. Globals: g_CCharStringInstanceCount @ 0x013BD800.
// Literals " bytes" @ 0x0129AB0C, " KB" @ 0x0129AB14, " MB" @ 0x0129AB18; floats 2^32 @ 0x0122DCB4 (the unsigned
// conversion fix-up), 1/1024 @ 0x01230008, 1/1048576 @ 0x0122ED60 (the power-of-two divisions become multiplies).
// Shape: hidden result in ecx (esi), size in edx, decimals on the stack. Each `return a + "suffix"` pushes the literal
// first (right-to-left), then builds the left temporary in the dead `decimals` argument slot ([esp0+4], decimals is
// read into edx first) and lets operator+ build straight into the result. The KB/MB arms cross-jump after their
// literal push and multiply. The default-constructed CCharString at [esp+4] (null block, counter++, destroyed after
// the temporary) is an unused local of the large-size block; the
// small-size return is written last, which puts its block at the function end like retail.
class CCharStringData;
extern long g_CCharStringInstanceCount;         // 0x013BD800

class CCharString {
public:
    CCharStringData* PStringData;               // +0x00
    CCharString() : PStringData(0) { ++g_CCharStringInstanceCount; }
    CCharString(const CCharString& other);      // 0x0099EC30
    ~CCharString() { UnassignString(); --g_CCharStringInstanceCount; }
    protected: void UnassignString(); public:                      // 0x0099E9B0
};

CCharString __fastcall operator+(const CCharString& left, const char* right);   // 0x0099F600
CCharString __fastcall GFIntToCharString(long value);                          // 0x0099F830
CCharString __fastcall GFFloatToCharString(double value, long decimals);       // 0x009A08B0

CCharString __fastcall GFByteSizeToCharString(unsigned long size, long decimals)
{
    if (size >= 4096) {
        CCharString text;
        if (size >= 1048576)
            return GFFloatToCharString(size / 1048576.0f, decimals) + " MB";
        return GFFloatToCharString(size / 1024.0f, decimals) + " KB";
    }
    return GFIntToCharString(size) + " bytes";
}