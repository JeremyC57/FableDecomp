// CFileInstaller::GetPercentComplete() -- under the installer's critical section (+4): 1.0 when the total byte count (+0x70) is zero, else (bytes done (+0x6c) + the current file's progress (+0x10 of the entry at +0x54, when there is one)) / total
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009C32C0, length 103 bytes (+ int3 padding).
// Callees: CCriticalSectionLock::CCriticalSectionLock @ 0x00A62920 (the section, false), ~CCriticalSectionLock @ 0x00A629A0 (8-byte stack local).
// Constant: 0x0122DED8 = 1.0f.
// Shape: the quotient reads the MEMBERS (total, then done) so it must be computed before the lock destructor (which could
// change them): the total load is CSE'd and homed in the frame for `fidiv`, done is a long local (`fild`), and the result is a
// named float local stored across the destructor. Dividing two plain locals lets VC7.1 sink the division after the destructor.
struct CRITICAL_SECTION_Fable { unsigned char _body[0x18]; };

struct CCriticalSectionLock_Retail {              // 8 bytes on the stack
    CRITICAL_SECTION_Fable* CriticalSection;       // +0x00
    bool                    Locked;                // +0x04
    CCriticalSectionLock_Retail(CRITICAL_SECTION_Fable* section, bool tryOnly);   // 0x00A62920
    ~CCriticalSectionLock_Retail();                                               // 0x00A629A0
};

struct CActiveFile {                       // the file being installed
    unsigned char _pad[0x10];
    long          m_bytesProcessed;                // +0x10
};

class CFileInstaller { public:
    unsigned char          _pad[4];
    CRITICAL_SECTION_Fable CriticalSection;              // +0x04
    unsigned char          _pad1c[0x38];
    CActiveFile*   PCurrentWriteFile;        // +0x54
    unsigned char          _pad58[0x14];
    long                   NoTotalBytesWritten;            // +0x6c
    long                   TotalSizeOfAllFiles;           // +0x70
    float GetPercentComplete();
};

float CFileInstaller::GetPercentComplete()
{
    CCriticalSectionLock_Retail lock(&CriticalSection, false);
    if (TotalSizeOfAllFiles == 0)
        return 1.0f;
    long done = NoTotalBytesWritten;
    if (PCurrentWriteFile)
        done += PCurrentWriteFile->m_bytesProcessed;
    float fraction = (float)done / TotalSizeOfAllFiles;
    return fraction;
}