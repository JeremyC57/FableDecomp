// CV_FisticuffsClubScript::ResetRegion_00e82010() -- resets the club script's region state: region 6, Village cleared to an empty script thing, the four per-region completion flags cleared
// Leaf: ResetRegion_00e82010
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E82010, 192 bytes (plain `ret`). Manifest label "CChunkedFile::~CChunkedFile" is
//          propagated and wrong: the body writes the CV_FisticuffsClubScript layout (Ego_r.pdb minus 0x14, as the landed
//          SetFightCount 0x00E82800): RelevantRegion +0x48, CompletedKnothole/Bowerstone/Bandit/OakvalePunchClub
//          +0x95..+0x98, Village (CScriptThing, vtable 0x01238C8C) +0x9c (PImp_Data +0xa0, PImp_Info +0xa4).
// Callees: CBase::CBase @ 0x0099A2D0 / CBase::~CBase @ 0x0099A2E0 (the temporary's base), the count block's
//          destroy hook (`call [block+4]`, object [block+8] in ecx), operator delete @ 0x00BFE9BC.
// Shape: `Village = CScriptThing();` -- the temporary is built in the frame (base ctor, own vtable, zero words); the
//          inline counted operator= reads the source words first, so with a null source it folds to "release the old
//          block and zero both words, only when the old block is non-null"; the temporary's inline destructor
//          re-stores its vtable, releases its (null) block, zeroes the words and calls the base destructor.
struct CBase {
    virtual ~CBase();                              // 0x0099A2E0
    CBase();                                       // 0x0099A2D0
};

struct SScriptThingInfo {                          // the counted block behind CScriptThing::PImp_Info
    long m_references;                             // +0
    void (__fastcall* m_pfnDestroy)(void*);        // +4
    void* m_pObject;                               // +8
};

struct CScriptThing : CBase {                      // vtable 0x01238C8C, 12 bytes
    void* PImp_Data;                               // +0x04
    SScriptThingInfo* PImp_Info;                   // +0x08
    CScriptThing() : PImp_Data(0), PImp_Info(0) { }
    ~CScriptThing()
    {
        if (PImp_Info) {
            --PImp_Info->m_references;
            if (PImp_Info->m_references == 0) {
                PImp_Info->m_pfnDestroy(PImp_Info->m_pObject);
                delete PImp_Info;
            }
        }
        PImp_Data = 0;
        PImp_Info = 0;
    }
    CScriptThing& operator=(const CScriptThing& other)
    {
        SScriptThingInfo* info = other.PImp_Info;
        void* data = other.PImp_Data;
        if (PImp_Info != info) {
            if (PImp_Info) {
                --PImp_Info->m_references;
                if (PImp_Info->m_references == 0) {
                    PImp_Info->m_pfnDestroy(PImp_Info->m_pObject);
                    delete PImp_Info;
                }
            }
            PImp_Data = data;
            PImp_Info = info;
            if (info)
                ++info->m_references;
        }
        return *this;
    }
};

struct CV_FisticuffsClubScript {
    unsigned char _base[0x48];                     // CScriptBase + interface/data pair
    long RelevantRegion;                           // +0x48
    unsigned char _pad_0x4c[0x95 - 0x4c];
    bool CompletedKnotholePunchClub;               // +0x95
    bool CompletedBowerstonePunchClub;             // +0x96
    bool CompletedBanditPunchClub;                 // +0x97
    bool CompletedOakvalePunchClub;                // +0x98
    unsigned char _pad_0x99[0x3];
    CScriptThing Village;                          // +0x9c
    void ResetRegion_00e82010();
};

void CV_FisticuffsClubScript::ResetRegion_00e82010()
{
    RelevantRegion = 6;
    Village = CScriptThing();
    CompletedKnotholePunchClub = false;
    CompletedBowerstonePunchClub = false;
    CompletedBanditPunchClub = false;
    CompletedOakvalePunchClub = false;
}
