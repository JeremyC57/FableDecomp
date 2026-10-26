// CSoundBankLH::IsSoundEventValid(const CCharString& name) -- whether the loaded bank holds a sample of that name: needs an engine and a loaded bank, then looks the name up in the engine's sample library
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A0BA40, length 114 bytes (`ret 4`, + int3 padding). Vtable 0x0129C9DC
// (CSoundBankLH) slot 9; the name is the one the landed CTCSoundPlayer callers (0x0066B280, 0x0066B080, 0x0066B120,
// 0x0066ACE0, 0x0066A920) give slot 9 (CASoundBank's own slot 9, 0x00A0BD20, returns false).
// Callees: the query record's name constructor @ 0x00C08720 (bank, name chars) and destructor @ 0x00C08780, the engine's
// slot 8 (+0x20, the sample library), the library's slot 1 (+0x04, the lookup). "" @ 0x0129AAF4 (the inline GetPString).
// Layout: as the landed constructor 0x00A0BAC0 -- the sound system at +0x0c (its engine at +0x40), the bank handle at +0x10.
// Shape: the sound system in a NAMED local -- it stays in eax across the bank-handle test and the engine is re-read through
// it (`mov ecx,[eax+40h]`; the plain member form re-reads +0x0c as well, keeps `this` in esi and the library in edi);
// GetPString inlined (`test ; jne ; mov eax,"" ; jmp ; mov eax,[eax]`) before the lookup's `push 0 ; push 0`; the result
// is `unsigned char` (`setne al` with no `xor eax,eax`; the bool form adds it -- callers read al).
// Leaf: IsSoundEventValid
class CCharStringData { public:                      // the shared string block
    char* m_pData;                            // +0x00
};
class CCharString {
public:
    CCharStringData* PStringData;             // +0x00
    const char* GetPString() const { return PStringData == 0 ? "" : PStringData->m_pData; }   // "" = 0x0129AAF4
};
class CSampleQueryLH {                        // vtable 0x012B1A54 (no RTTI; the landed CVtable_012b1a54), 0x14 bytes
public:
    virtual void v00();
    long        m_group;                      // +0x04
    long        m_sample;                     // +0x08
    const void* m_pName;                      // +0x0c
    void*       m_pBank;                      // +0x10
    CSampleQueryLH(void* bank, long group, long sample);   // 0x00C08750
    CSampleQueryLH(void* bank, const void* name);          // 0x00C08720 (group and sample -1)
    ~CSampleQueryLH();                                     // 0x00C08780 (stores the vtable back)
};
class CSampleLH {                             // a sample record of the engine's library (descriptive slot names)
public:
    virtual long  GetValue();                                    // slot 0
    virtual long  GetRange(long channel, long* first, long* last); // slot 1 (+0x04), < 0 = failure
    virtual long  GetRate();                                     // slot 2 (+0x08)
    virtual long  GetFormat();                                   // slot 3 (+0x0c)
    virtual float GetLength();                                   // slot 4 (+0x10)
};
class CSampleLibraryLH {
public:
    virtual void v00();
    virtual CSampleLH* Find(const CSampleQueryLH& query, long a, long b);   // slot 1 (+0x04)
};
class CSoundEngineLH {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07();
    virtual CSampleLibraryLH* GetSampleLibrary();                           // slot 8 (+0x20)
};
struct CSoundSystemLH {                       // the sound system the bank was created by
    unsigned char   _pad[0x40];
    CSoundEngineLH* m_pEngine;                // +0x40
};
class CSoundBankLH {                          // vtable 0x0129C9DC (CSoundBankLH, rtti-vtables.tsv)
public:
    virtual void v00();
    long            m_4;                      // +0x04
    long            m_8;                      // +0x08
    CSoundSystemLH* m_pSystem;                // +0x0c
    void*           m_pBank;                  // +0x10 the engine's handle of the loaded bank
    virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08();
    virtual unsigned char IsSoundEventValid(const CCharString& name);   // slot 9 (+0x24)
};

unsigned char CSoundBankLH::IsSoundEventValid(const CCharString& name)
{
    CSoundSystemLH* system = m_pSystem;
    if (system->m_pEngine && m_pBank) {
        CSampleLH* found = system->m_pEngine->GetSampleLibrary()->Find(CSampleQueryLH(m_pBank, name.GetPString()), 0, 0);
        return found != 0;
    }
    return false;
}