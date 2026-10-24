// CCharString::ReplaceAll(const CCharString& find, const CCharString& replacement) -- replace every occurrence of find in the string with replacement (the string's own buffer is cut in place while the result is built)
// Leaf: ?ReplaceAll@CCharString@@
//
// Retail 0x00527200, 224 bytes, `ret 8` (thiscall: the string in ecx); 8-byte frame (the result string and the spilled
//   `this`).
// Callees: 0x0099E4B0 CCharString::CCharString(), 0x00BFEB3A strstr (twice), 0x0099EBF0 CCharString::CCharString(const
//   char*, int length = -1) (the piece temporaries, homed in the dead `find` argument slot), 0x0099F0A0
//   CCharString::operator+=(const CCharString&), 0x0099EAE0 ~CCharString, 0x0099EFB0 CCharString::operator=.
//   strlen is the /Oi inline scan.
// Callers: none by direct call.
// Identity: unproven name. The body is compiled in the 0x0051xxxx-0x0052xxxx thing-manager TU (its "" literal is
//   0x0129AAF4, not the CCharString TU's 0x0129A8E0), so it is an inline/header helper rather than the CCharString
//   library's own `?Replace@CCharString@@QAEXABV1@0@Z` (FableWin.pdb name set; its body is likely 0x0099C1E0, unchecked).
//   The catalog "CCharString_ReplaceAll" is kept as a CCharString member of the local model.
// Shape: GetString() is `PStringData == 0 ? "" : PStringData->m_pData` (the literal loaded first, then `je`); the first
//   strstr is ahead of the loop and the next one is at its bottom on the advanced hit pointer (`hit += strlen(key)` right
//   after the NUL store, then `start = hit`): a `while ((hit = strstr(start, key)) != 0)` loop instead keeps the length
//   live in ebx and rebuilds start with a lea (221 v 224).
extern "C" char* __cdecl strstr(const char* str, const char* sub);   // 0x00BFEB3A
extern "C" unsigned int __cdecl strlen(const char* str);               // /Oi intrinsic (inline scan)

struct CCharStringData {
    char*         m_pData;                      // +0x0
    unsigned long m_length;                     // +0x4
};
class CCharString {
public:
    CCharStringData* PStringData;               // +0x0 (null = empty)
    CCharString();                              // 0x0099E4B0
    CCharString(const char* text, int length = -1);   // 0x0099EBF0
    ~CCharString();                             // 0x0099EAE0
    CCharString& operator=(const CCharString& other);   // 0x0099EFB0
    CCharString& operator+=(const CCharString& other);  // 0x0099F0A0
    const char* GetString() const { return PStringData == 0 ? "" : PStringData->m_pData; }
    void ReplaceAll(const CCharString& find, const CCharString& replacement);
};
void CCharString::ReplaceAll(const CCharString& find, const CCharString& replacement)
{
    CCharString result;
    char* start = (char*)GetString();
    const char* key = find.GetString();
    char* hit = strstr(start, key);
    while (hit) {
        *hit = 0;
        hit += strlen(key);
        result += CCharString(start);
        result += replacement;
        start = hit;
        hit = strstr(hit, key);
    }
    result += CCharString(start);
    *this = result;
}