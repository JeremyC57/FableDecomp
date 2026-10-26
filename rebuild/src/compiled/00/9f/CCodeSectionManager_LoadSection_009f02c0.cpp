// CCodeSectionManager::LoadSection(unsigned long) -- for a real handle, mark the section descriptor loaded if it is not already
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009F02C0, length 41 bytes.
// Callees: CCodeSectionManager::GetSectionDesc(unsigned long) @ 0x009F0240 (returns a reference into
// the SectionDesc map, or a static empty descriptor when the handle is unknown; called twice because
// it is out of line). CSectionDesc (PDB, 16 bytes): ShortName +0, LongName +4, Loaded +8, Size +0xc.
// The handle test is against (unsigned long)-1; both calls pass the handle through from its slot.
struct CSectionDesc {
    void* ShortName;                                        // +0x0 CCharString
    void* LongName;                                         // +0x4 CCharString
    bool  Loaded;                                           // +0x8
    long  Size;                                             // +0xc
};

class CCodeSectionManager { public:
    virtual void v00();
    unsigned char _maps[0x24];                              // ShortNameToHandle +4, LongNameToHandle +0x10, SectionDesc +0x1c (12-byte maps)
    bool SectionLoadingEnabled;                             // +0x28
    CSectionDesc& GetSectionDesc(unsigned long handle);     // 0x009F0240
    void LoadSection(unsigned long handle);
};

void CCodeSectionManager::LoadSection(unsigned long handle)
{
    if (handle != (unsigned long)-1) {
        if (!GetSectionDesc(handle).Loaded)
            GetSectionDesc(handle).Loaded = true;
    }
}