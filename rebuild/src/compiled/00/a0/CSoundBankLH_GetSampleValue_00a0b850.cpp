// CSoundBankLH::GetSampleValue(long sample, long* value) -- look a sample of the loaded bank up in the sound engine's sample library and report its slot-0 value when it is positive; false when there is no engine, no such sample or a non-positive value
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A0B850, length 119 bytes (`ret 8`, + int3 padding). No callers or vtable
// slot in the image (an out-of-line member of the CSoundBankLH TU, between slots 1 and 6); method and slot names descriptive.
// Callees: the query record's constructor @ 0x00C08750 (bank, group, sample; landed as CVtable_012b1a54) and destructor
// @ 0x00C08780, the engine's slot 8 (+0x20, the sample library), the library's slot 1 (+0x04, the lookup), the sample's slot 0.
// Layout: as the landed constructor 0x00A0BAC0 -- the sound system at +0x0c (its engine at +0x40), the bank handle at +0x10.
// Shape: the sound system read into a NAMED local (`CSoundSystemLH* system = m_pSystem;`) and the engine tested and used
// through it: that is what makes VC take eax for every vtable load and hoist the sample and the bank handle into ecx/edx
// before the lookup's `push 0 ; push 0` (an `engine` local, or plain `m_pSystem->m_pEngine`, gives `mov edx,[ecx]` and
// interleaved eax loads, 119 v 119). The lookup's trailing zeros are pushed before the temporary query record is built
// (VC pushes the outer call's later arguments first); the record is destroyed right after the call. `unsigned char`
// result (`xor al,al` / `mov al,1`); the "no sample" and "value <= 0" exits share one tail, the no-engine exit has its own.
// Leaf: GetSampleValue
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
    unsigned char GetSampleValue(long sample, long* value);
};

unsigned char CSoundBankLH::GetSampleValue(long sample, long* value)
{
    CSoundSystemLH* system = m_pSystem;
    if (system->m_pEngine) {
        CSampleLH* found = system->m_pEngine->GetSampleLibrary()->Find(CSampleQueryLH(m_pBank, 0, sample), 0, 0);
        if (!found)
            return false;
        long v = found->GetValue();
        if (v <= 0)
            return false;
        *value = v;
        return true;
    }
    return false;
}