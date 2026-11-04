// NScript::CV_FisticuffsClubScript::Init() -- resets the club script's region state: region 6, Village cleared to an empty script thing, the four per-region completion flags cleared
// Leaf: ?Init@CV_FisticuffsClubScript@NScript@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E82010, 192 bytes (plain `ret`). Manifest label "CChunkedFile::~CChunkedFile" is
//          propagated and wrong: the body writes the CV_FisticuffsClubScript layout (Ego_r.pdb minus 0x14, as the landed
//          SetFightCount 0x00E82800): RelevantRegion +0x48, CompletedKnothole/Bowerstone/Bandit/OakvalePunchClub
//          +0x95..+0x98, Village (CScriptThing, vtable 0x01238C8C) +0x9c (PImp_Data +0xa0, PImp_Info +0xa4).
// Callees: CBaseClass::CBaseClass @ 0x0099A2D0 / CBaseClass::~CBaseClass @ 0x0099A2E0 (the temporary's base), the count block's
//          destroy hook (`call [block+4]`, object [block+8] in ecx), operator delete @ 0x00BFE9BC.
// Shape: `Village = CScriptThing();` -- the temporary is built in the frame (base ctor, own vtable, zero words); the
//          inline counted operator= reads the source words first, so with a null source it folds to "release the old
//          block and zero both words, only when the old block is non-null"; the temporary's inline destructor
//          re-stores its vtable, releases its (null) block, zeroes the words and calls the base destructor.
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (wrapped, struct->class). Same bytes.
// Naming wave 35: identity outer script vtable 012eaf14 slot 3 (slot 1 RegisterMain pushes "Main" 0x012C2F94); enclosing 00e82220 (slot 2) registers 4 entity scripts whose PDB Alloc outer is CV_FisticuffsClubScript; PDB ?Init@CV_FisticuffsClubScript@NScript@@; refs=1; was NScript::CV_FisticuffsClubScript::ResetRegion_00e82010 (wave3/labels_alloc.tsv); PDB ?Init@CV_FisticuffsClubScript@NScript@@UAEXXZ in Ego_r.pdb / FableWin.pdb.
//   Was `?ResetRegion_00e82010@CV_FisticuffsClubScript@NScript@@QAEXXZ`; this source now defines the PDB member (vptr placeholder dropped (own)); same bytes.
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0

struct SScriptThingInfo {                          // the counted block behind CScriptThing::PImp_Info
    long m_references;                             // +0
    void (__fastcall* m_pfnDestroy)(void*);        // +4
    void* m_pObject;                               // +8
};

class CScriptThing : public CBaseClass { public:                      // vtable 0x01238C8C, 12 bytes
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

namespace NScript {
class CV_FisticuffsClubScript {
public:
    unsigned char _base[0x44];   // (-4: now the vptr of the virtual member)
    long RelevantRegion;                           // +0x48
    unsigned char _pad_0x4c[0x95 - 0x4c];
    bool CompletedKnotholePunchClub;               // +0x95
    bool CompletedBowerstonePunchClub;             // +0x96
    bool CompletedBanditPunchClub;                 // +0x97
    bool CompletedOakvalePunchClub;                // +0x98
    unsigned char _pad_0x99[0x3];
    CScriptThing Village;                          // +0x9c
public:
    virtual void Init();                          // PDB ?Init@CV_FisticuffsClubScript@NScript@@UAEXXZ
};
} // namespace NScript
using NScript::CV_FisticuffsClubScript;

void NScript::CV_FisticuffsClubScript::Init()
{
    RelevantRegion = 6;
    Village = CScriptThing();
    CompletedKnotholePunchClub = false;
    CompletedBowerstonePunchClub = false;
    CompletedBanditPunchClub = false;
    CompletedOakvalePunchClub = false;
}