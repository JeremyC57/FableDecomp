// CAFile::GetCopyFilename(const CCharString& filename) -- the name for a copy of a file: split off the extension, then bump a trailing number ("save3.sav" -> "save4.sav") or append 2 when the name has none ("save.sav" -> "save2.sav")
// Leaf: ?GetCopyFilename@CAFile@@
// Flags: /Oa
// Retail: Fable.exe (TLC, VC7.1) @ 0x00997BB0, 496 bytes (incl. int3 padding). FableWin.pdb
//   `?GetCopyFilename@CAFile@@SI?AVCCharString@@ABV2@@Z`: a public static `__fastcall` returning CCharString (hidden
//   result in ecx, filename in edx), plain `ret`. The catalog name is the PDB name.
// Callees: CCharString copy ctor 0x0099EC30, CCharString(const char*, long = -1) 0x0099EBF0, GetSubString(start, end)
//   0x0099EC70, operator= 0x0099EFB0, ~CCharString 0x0099EAE0, GFIsCharStringNumber 0x0099E620, GFCharStringToInt
//   0x0099E7F0, _CIpow 0x00BFEB40, __ftol2 0x00BFEA70, GFIntToCharString 0x0099F830, operator+(CCharString,
//   CCharString) 0x0099F570, operator+= 0x0099F0A0. Data: "" 0x0122D70E, the empty text 0x0129AAF4, 10.0 0x0129A7B0.
// Shape: the extension is found by scanning back from the end for '.'; the trailing number is read one character at a
//   time from the right as digit * 10^(n-1) through the float pow overload (the inline powf's float result is stored and
//   the register copy used), added with the implicit long conversion (fiadd, then __ftol2).
#include <math.h>
extern const char g_emptyLiteral_0129aaf4[];            // 0x0129AAF4: ""

class CCharStringData;
class CCharString
{
public:
    CCharStringData* PStringData;                       // +0x00 {PData +0, StringLength +4}
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
    CCharString& operator=(const CCharString& other);  // 0x0099EFB0
    CCharString& operator+=(const CCharString& other); // 0x0099F0A0
    CCharString GetSubString(unsigned long start, unsigned long end) const;   // 0x0099EC70
    long GetLength() const { return PStringData == 0 ? 0 : ((const long*)PStringData)[1]; }
    const char* GetString() const { return PStringData == 0 ? g_emptyLiteral_0129aaf4 : *(const char* const*)PStringData; }
    char operator[](long index) const { return GetString()[index]; }
};
CCharString __fastcall operator+(const CCharString& left, const CCharString& right);   // 0x0099F570
CCharString __fastcall GFIntToCharString(long value);                                  // 0x0099F830
bool __fastcall GFIsCharStringNumber(const CCharString& text);                         // 0x0099E620
long __fastcall GFCharStringToInt(const CCharString& text);                            // 0x0099E7F0

class CAFile
{
public:
    static CCharString __fastcall GetCopyFilename(const CCharString& filename);
};

CCharString __fastcall CAFile::GetCopyFilename(const CCharString& filename)
{
    CCharString name(filename);
    CCharString extension("");
    long length = filename.GetLength();
    for (long i = 1; i <= length; ++i) {
        if (name[length - i] == '.') {
            name = filename.GetSubString(0, length - i);
            extension = filename.GetSubString(length - i, length);
            break;
        }
    }
    long nameLength = name.GetLength();
    long number = 0;
    for (long digits = 1; digits <= nameLength; ++digits) {
        CCharString character = name.GetSubString(nameLength - digits, nameLength - digits + 1);
        if (!GFIsCharStringNumber(character))
            break;
        number += GFCharStringToInt(character) * pow(10.0f, (float)(digits - 1));
    }
    if (number == 0) {
        return name + GFIntToCharString(2) + extension;
    } else {
        CCharString numberString = GFIntToCharString(number);
        CCharString copyName = name.GetSubString(0, nameLength - numberString.GetLength());
        copyName += GFIntToCharString(number + 1);
        return copyName + extension;
    }
}