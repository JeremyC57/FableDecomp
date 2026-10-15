// CTCInventoryExperience::IsTutorialPlayed(const CCharString& name) -- true if already played, else record it and return false
// Retail 0x0057598A, 53 bytes, ret 4. TutorialsPlayed is an STLport vector<CCharString> at retail
// +0x248 (PDB +0x2A8: four 16-byte containers ahead of it are 12 bytes in retail), so a local model.
// Shape: the end pointer is cached first (edi), the SGI `find` front end 0x004B9E20 is a /Gr
// `__fastcall` (first in ecx, last in edx, the value reference on the stack); a hit returns
// `mov al,1`, a miss appends the name with vector<CCharString>::push_back 0x0044BFF0 (thiscall on the
// vector, address kept in esi) and returns `xor al,al` -- an unsigned char result.
#pragma optimize("s", on)

struct CCharString {
    void* m_pData;
};

CCharString* __fastcall find(CCharString* first, CCharString* last, const CCharString& value);   // 0x004B9E20

struct SCharStringVector {
    CCharString* _M_start;
    CCharString* _M_finish;
    CCharString* _M_end_of_storage;
    void push_back(const CCharString& value);      // 0x0044BFF0
};

struct CTCInventoryExperience {
    unsigned char _pad_000[0x248];
    SCharStringVector TutorialsPlayed;             // +0x248
    unsigned char IsTutorialPlayed(const CCharString& name);
};

unsigned char CTCInventoryExperience::IsTutorialPlayed(const CCharString& name)
{
    CCharString* last = TutorialsPlayed._M_finish;
    if (find(TutorialsPlayed._M_start, last, name) != last)
        return 1;
    TutorialsPlayed.push_back(name);
    return 0;
}