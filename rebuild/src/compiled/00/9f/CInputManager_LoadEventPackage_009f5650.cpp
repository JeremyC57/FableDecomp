// CInputManager::LoadEventPackage() -- when the next loaded package (LoadedEventPackages.first[CurrentFilePackageIndex], 48-byte records) carries the current TimeStamp, install its events, refresh Checksum4 from the internal checksum and advance the index
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009F5650, length 50 bytes (+ int3 padding).
// Leaf: ?LoadEventPackage@CInputManager@@IAEXXZ
// Callees: CInputManager::SetEventsFromEventPackage(const CInputEventPackage&) @ 0x009F54D0, CInputManager::CalculateInternalChecksum() const @ 0x009F5170.
// NF75 (round 7, land1117fu q10928): Ego_r.pdb / FableWin.pdb spellings in the row's pre-existing local declarations: the protected
//   ?SetEventsFromEventPackage@CInputManager@@IAEXABVCInputEventPackage@@@Z (const reference; was a public pointer form), the public const
//   ?CalculateInternalChecksum@CInputManager@@QBEKXZ (was non-const) and the row's own protected ?LoadEventPackage@CInputManager@@IAEXXZ
//   (Ego_r 0x00B49310 / FableWin 0x030CF990; was public). Same bytes.
// Layout from Ego_r.pdb with 12-byte vectors: CInputEventPackage {Events +0, TimeStamp +0xc, Checksum +0x10, MousePos +0x20,
// JoystickPos +0x28} = 48 bytes; CInputManager Checksum +0x3c (Checksum4 at +0x48), LoadedEventPackages.first +0x4c,
// CurrentFilePackageIndex +0x58, TimeStamp +0x5c.
// Shape: the record address is `lea eax,[eax+eax*2] ; shl eax,4` plus first (a named pointer kept in eax across the compare),
// `cmp [eax+0Ch],edx` with the package's stamp first, then the two calls and `inc dword ptr [esi+58h]`.
class CInputEventPackage { public:                         // 48 bytes
    unsigned char _events[0x0c];                    // +0x00 Events (std::vector<CInputEvent>)
    long          TimeStamp;                        // +0x0c
    unsigned char _rest[0x20];                      // +0x10 Checksum, MousePos, JoystickPos
};
class CChecksum { public:
    long Checksum1;                                 // +0x00
    long Checksum2;                                 // +0x04
    long Checksum3;                                 // +0x08
    long Checksum4;                                 // +0x0c
};

class CInputManager { public:
    virtual void v00();
    unsigned char       _pad04[0x38];               // +0x04 vectors, counters, devices, flags
    CChecksum           Checksum;                   // +0x3c
    CInputEventPackage* m_pLoadedPackagesFirst;     // +0x4c LoadedEventPackages.first
    CInputEventPackage* m_pLoadedPackagesLast;      // +0x50
    CInputEventPackage* m_pLoadedPackagesEnd;       // +0x54
    long                CurrentFilePackageIndex;    // +0x58
    long                TimeStamp;                  // +0x5c
    unsigned long CalculateInternalChecksum() const;   // 0x009F5170 ?CalculateInternalChecksum@CInputManager@@QBEKXZ
protected:
    void SetEventsFromEventPackage(const CInputEventPackage& package);   // 0x009F54D0 ?SetEventsFromEventPackage@CInputManager@@IAEXABVCInputEventPackage@@@Z
    void LoadEventPackage();                        // ?LoadEventPackage@CInputManager@@IAEXXZ
public:
};

void CInputManager::LoadEventPackage()
{
    CInputEventPackage* package = &m_pLoadedPackagesFirst[CurrentFilePackageIndex];
    if (package->TimeStamp == TimeStamp) {
        SetEventsFromEventPackage(*package);
        Checksum.Checksum4 = CalculateInternalChecksum();
        ++CurrentFilePackageIndex;
    }
}