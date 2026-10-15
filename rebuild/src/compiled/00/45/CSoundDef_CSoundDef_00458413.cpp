// CSoundDef::CSoundDef() -- base CParentDefClassBase() (0x00430370), this class's vtable, then the five retail vectors +0x3c..+0x74 built empty
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00458413, length 65 bytes.
// Callees: 0x00430370 (the shared def root constructor, CParentDefClassBase). Vtable 0x012349B4 = CSoundDef
// (rtti-vtables.tsv). Layout from the PDB (Ego_r: five std::vectors from +0x40, three floats; 156 bytes)
// shifted to the retail build: the def root ends at +0x3c and a retail vector is 12 bytes (first / last /
// end), so the vectors sit at +0x3c/+0x48/+0x54/+0x60/+0x6c and the floats at +0x78..+0x80.
// Shape: `xor eax,eax` scheduled before the vtable store, the vtable store before the fifteen
// `mov [esi+N],eax`: each vector is a member object whose inline constructor zeroes its three pointers
// (plain init-list scalars would sink the vtable store below them). The floats are left uninitialised.
struct CParentDefClassBase {
    virtual void v00();
    unsigned char _base[0x38];                  // +0x04..+0x3b: the def root's own members
    CParentDefClassBase();                      // 0x00430370
};

template <class T>
struct CRetailVector {                          // VC6-era Dinkumware vector: first / last / end
    T* first;
    T* last;
    T* end;
    CRetailVector() : first(0), last(0), end(0) { }
};

struct CSoundBankEntry;
struct CAtmosBankEntry;
struct CMusicEntry;
struct CMusicSetEntry;
struct CReverbEnvironmentEntry;

struct CSoundDef : CParentDefClassBase {
    CRetailVector<CSoundBankEntry>         SoundBankEntries;            // +0x3c
    CRetailVector<CAtmosBankEntry>         AtmosBankEntries;            // +0x48
    CRetailVector<CMusicEntry>             MusicEntries;                // +0x54
    CRetailVector<CMusicSetEntry>          MusicSetEntries;             // +0x60
    CRetailVector<CReverbEnvironmentEntry> ReverbEnvironmentEntries;    // +0x6c
    float AtmosListenerDampingMin;              // +0x78
    float AtmosListenerDampingRange;            // +0x7c
    float DistanceModelCurve;                   // +0x80
    virtual void v00();
    CSoundDef();
};

CSoundDef::CSoundDef()
    : CParentDefClassBase()
{
}