#pragma optimize("g",on)

typedef unsigned short wchar16;

class CCharString { public:
    void* p;
    CCharString(const char* s, long len);   // 0x597c80 (this, char*, len)
    ~CCharString();                          // 0x597b70 (this)
};

class CWideString { public:
    void* p;
    CWideString(const wchar16* s);                              // 0x594740 (this, wchar_t*)
    ~CWideString();                                             // 0x5945a0 (this)
};

// 0x595080 : CWideString __fastcall operator+(wchar_t const* pre, CWideString const& suf)
//   retptr in ecx, pre in edx, suf pushed
CWideString __fastcall WConcat(const wchar16* pre, const CWideString& suf);

// 0x406e30
extern "C" long __fastcall CreateSaveGame(CCharString* a, CWideString* b, long c, long d, wchar16* e, long f);

class CUserProfileManager { public:
    char pad[8];
    CWideString field8;
    CWideString GetAutoSavePathName();
};

// Literal (NF413, 2026-11-25): HEAD cast three retail addresses held in named constants (LIT_C / LIT_A / LIT_B); each is now the literal spelled from
//   retail's bytes, at its use site: 0x0122E05C narrow "U:\\" (55 3A 5C 00), 0x0122DE9C wide L"AutoSave", 0x0122D70C wide L"".
CWideString CUserProfileManager::GetAutoSavePathName()
{
    wchar16 buf[0x104];
    bool ok;
    {
        CCharString cs("U:\\", -1);
        ok = (CreateSaveGame(&cs, &this->field8, 3, 0, buf, 0x104) == 0);
    }
    if (ok) {
        CWideString suf((const wchar16*)L"AutoSave");
        return WConcat(buf, suf);
    }
    return CWideString((const wchar16*)L"");
}