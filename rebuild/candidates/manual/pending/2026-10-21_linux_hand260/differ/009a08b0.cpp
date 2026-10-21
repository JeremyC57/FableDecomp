// Grade: DIFFER(458v491) -- retail keeps 0 in ebp and the block in esi across the strip loop, cross-jumps the two Format pushes and inlines the block release twice; ours folds the zero constant to push imm and allocates differently
// CCharString::FormatDouble(long, double) -- format a double with the given precision and strip trailing zeros, keeping one after the point
// Leaf: FormatDouble
// Layout: CCharString model of the landed 0x009A0880 / 0x0099F930 (4-byte handle on a packed 17-byte shared block).
#include <float.h>

#pragma pack(push, 1)
struct CBasicStringChar {
    char*         PData;
    unsigned long StringLength;
    unsigned long DataLength : 31;
    bool          UseFastExtend : 1;
    CBasicStringChar(const char* text);          // 0x009A0590
    void Truncate(unsigned long length);         // 0x009A0080
    void Extend(char c, long count);             // 0x009A0460
    ~CBasicStringChar()
    {
        if (PData != 0) {
            delete[] PData;
            PData = 0;
        }
        StringLength = 0;
        DataLength = 0;
    }
};
struct CCharStringData {
    CBasicStringChar Data;
    long             NoRefs;
    CCharStringData(const char* text) : Data(text), NoRefs(1) { }
};
#pragma pack(pop)

extern const double g_ZeroDouble;                 // 0x0122ED70
extern long g_CCharStringInstanceCount;          // 0x013BD800

class CCharString {
public:
    CCharStringData* PStringData;
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
                ++data->NoRefs;
            }
        }
    }
    ~CCharString() { ReleaseData(); --g_CCharStringInstanceCount; }
    void ReleaseData()
    {
        if (PStringData != 0) {
            if (--PStringData->NoRefs <= 0)
                delete PStringData;
            PStringData = 0;
        }
    }
    long GetLength() const { if (PStringData == 0) return 0; return PStringData->Data.StringLength; }
    const char* GetData() const { if (PStringData == 0) return ""; return PStringData->Data.PData; }
    void Truncate(unsigned long length)
    {
        if (length < (unsigned long)GetLength()) {
            if (length == 0) {
                ReleaseData();
            } else {
                if (PStringData->NoRefs > 1) {
                    CCharStringData* copy = new CCharStringData(PStringData->Data.PData);
                    UnassignString();
                    PStringData = copy;
                }
                PStringData->Data.Truncate(length);
            }
        }
    }
    void UnassignString();                       // 0x0099E9B0
    void MakeOwnCopyOfData();                    // 0x0099EAF0
    void Format(const char* format, ...);        // 0x0099F1F0
    static CCharString __fastcall FormatDouble(long precision, double value);
};

CCharString __fastcall CCharString::FormatDouble(long precision, double value)
{
    CCharString text;
    if (_isnan(value))
        text.Format("%.*f", precision, g_ZeroDouble);
    else
        text.Format("%.*f", precision, value);
    long i = 0;
    while (true) {
        if (i >= text.GetLength())
            goto done;
        if (text.GetData()[i] == '.')
            break;
        i++;
    }
    while (text.PStringData != 0 && (long)text.PStringData->Data.StringLength > 0) {
        char last = text.PStringData->Data.PData[text.PStringData->Data.StringLength - 1];
        if (last == '.') {
            text.MakeOwnCopyOfData();
            text.PStringData->Data.Extend('0', 1);
            break;
        }
        if (last != '0')
            break;
        text.Truncate(text.PStringData->Data.StringLength - 1);
    }
done:
    return text;
}
