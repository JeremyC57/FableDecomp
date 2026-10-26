// GetDataFromEventString(const CCharString&, long, CCharString&) -- the index-th space-separated field after the event name: skip index+1 words, then cut at the next space
// Leaf: ?GetDataFromEventString@?A0x
// (Leaf is a prefix: the anonymous-namespace hash varies with the file path; it excludes the Call... wrapper.)
// Retail 0x00870C90, 0xD2 bytes (+ int3 padding), __fastcall (event string in ecx, index in edx, result on the stack, `ret 4`).
// PDB (Ego_r): ?GetDataFromEventString@?A0xaf422eae@@YI_NABVCCharString@@JAAV2@@Z (a file-static).
// Callees: CCharString::operator=(const CCharString&) @ 0x0099EFB0, FindCharPos(char) const @ 0x0099E5A0,
// GetSubString(unsigned long, unsigned long) const @ 0x0099EC70 (by-value result), ~CCharString @ 0x0099EAE0 (catalog
// label a propagated _Cons_val).
// Shape: data = event; the first word is cut from the EVENT string (ebx: VC cannot assume data aliases it after the
// out-of-line operator=), then `while (--index != -1)` (`dec edi ; cmp edi,-1`; a `while (index--)` tests before the
// decrement and swaps ebx/edi) cuts one more word from data per pass. The start is `FindCharPos(' ') + 1` tested against
// 0 (`inc eax ; je`; testing pos == -1 first gives `cmp eax,-1` + a later `inc`). Finally data is cut at its next space
// or kept whole (`pos == -1 ? GetLength() : pos`). Each GetSubString temporary is built in the dead `data` argument
// slot and destroyed right after the assignment. The inline GetLength is `p == 0 ? 0 : p->m_length`.
class CCharStringData { public: const char* m_pData; unsigned long m_length; };

class CCharString
{
public:
    CCharStringData* PStringData;                    // +0x00
    ~CCharString();                                  // 0x0099EAE0
    CCharString& operator=(const CCharString& other);                              // 0x0099EFB0
    unsigned long GetLength() const { return PStringData == 0 ? 0 : PStringData->m_length; }
    long FindCharPos(char c) const;                                                 // 0x0099E5A0
    CCharString GetSubString(unsigned long start, unsigned long length) const;      // 0x0099EC70
};

#pragma auto_inline(off)
namespace {
bool __fastcall GetDataFromEventString(const CCharString& eventString, long index, CCharString& data)
{
    data = eventString;
    long start = eventString.FindCharPos(' ') + 1;
    if (start == 0)
        return false;
    data = eventString.GetSubString(start, eventString.GetLength());
    while (--index != -1)
    {
        start = data.FindCharPos(' ') + 1;
        if (start == 0)
            return false;
        data = data.GetSubString(start, data.GetLength());
    }
    long pos = data.FindCharPos(' ');
    data = data.GetSubString(0, pos == -1 ? data.GetLength() : pos);
    return true;
}
}
#pragma auto_inline(on)

bool __fastcall CallGetDataFromEventString(const CCharString& eventString, long index, CCharString& data)
{
    return GetDataFromEventString(eventString, index, data);
}