// CHeroStatsDef::~CHeroStatsDef() -- release eight owned blocks around two embedded cleanup records, then tail-call the definition base destructor; retail @ 0x0045619E, 166 bytes.
// Retail member-call receivers at +0x70/@0x004533E3 and +0xFC/@0x00453406, eight pointer loads, and the base tail-call at 0x00430300 establish this cleanup sequence. The quarantined PDB layout is not used because its members do not agree with these retail offsets.
#pragma optimize("s", on)
extern "C" void __cdecl free(void* memory); // 0x00BFEA14
struct CDefinitionBaseModel {
    unsigned char data[0x70];
    ~CDefinitionBaseModel(); // 0x00430300
};
struct DefinitionCleanup70 {
    void* first;
    void* second;
    unsigned char pad08[0x34];
    void* pAC;
    unsigned char pad40[8];
    void* pB8;
    unsigned char pad4c[8];
    void* pC4;
    unsigned char pad58[0x28];
    void* pF0;
    unsigned char pad84[8];
    void Cleanup(); // 0x004533E3
};
struct DefinitionCleanupFC {
    void* first;
    void* second;
    unsigned char pad08[4];
    void Cleanup(); // 0x00453406
};
class CHeroStatsDef : public CDefinitionBaseModel { public:
    DefinitionCleanup70 cleanup70; // +0x70
    DefinitionCleanupFC cleanupFC; // +0xfc
    void* p108;
    unsigned char pad10c[8];
    void* p114;
    unsigned char pad118[8];
    void* p120;
    unsigned char pad124[8];
    void* p12c;
    ~CHeroStatsDef();
};
CHeroStatsDef::~CHeroStatsDef() {
    if (p12c) free(p12c);
    if (p120) free(p120);
    if (p114) free(p114);
    if (p108) free(p108);
    cleanupFC.Cleanup();
    if (cleanup70.pF0) free(cleanup70.pF0);
    if (cleanup70.pC4) free(cleanup70.pC4);
    if (cleanup70.pB8) free(cleanup70.pB8);
    if (cleanup70.pAC) free(cleanup70.pAC);
    cleanup70.Cleanup();
}