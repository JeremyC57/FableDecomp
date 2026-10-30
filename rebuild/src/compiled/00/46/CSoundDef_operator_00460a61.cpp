// CSoundDef::operator=(const CSoundDef&) -- memberwise copy: CParentDefClassBase part, the five sound/atmos/music/music-set/reverb entry vectors, then the atmos damping pair and the distance-model curve; returns *this
#pragma optimize("s", on)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00460A61, length 105 bytes.
// Pragma: #pragma optimize("s", on) -- the def module is size-optimised (every copy goes through eax).
// Callees: CParentDefClassBase::operator= @ 0x00431ED0; vector<CSoundBankEntry>::operator= @ 0x00460ACA,
//          vector<CAtmosBankEntry>::operator= @ 0x00460C41, vector<CMusicEntry>::operator= @ 0x00460D75,
//          vector<CMusicSetEntry>::operator= @ 0x00460EB5, vector<CReverbEnvironmentEntry>::operator= @ 0x00461017.
// Identity: the catalog label "CParentDefClassBase::operator=" is propagated. Slot 19 (Copy) of CSoundDef's
// vtable 0x012349B4 is the 5-byte `jmp 0x00460A61` right before this body; PDB members +0x40..+0x98 sit
// at retail +0x3c..+0x80 (retail base 0x39 bytes, vectors 12 bytes instead of 16).
// Shape: memberwise assignment in declaration order, each vector through its own out-of-line operator=.
#pragma pack(push, 1)
// Layout: CParentDefClassBase is the retail def chain from the shared headers (FABLE_RETAIL_DEF_CLASS_BASE opt-in:
//   vtable 0x01230CBC, 0x39 bytes packed; ctor 0x00430370, ~CParentDefClassBase 0x00430300, copy 0x0046EE98,
//   operator= 0x00431ED0).
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CParentDefClassBase.h"
#pragma pack(pop)

#define FABLE_RETAIL_VECTOR(Name) \
    struct Name { void* m_first; void* m_last; void* m_end; Name& operator=(const Name& other); }

FABLE_RETAIL_VECTOR(CSoundBankEntryVector);         // operator= 0x00460ACA
FABLE_RETAIL_VECTOR(CAtmosBankEntryVector);         // operator= 0x00460C41
FABLE_RETAIL_VECTOR(CMusicEntryVector);             // operator= 0x00460D75
FABLE_RETAIL_VECTOR(CMusicSetEntryVector);          // operator= 0x00460EB5
FABLE_RETAIL_VECTOR(CReverbEnvironmentEntryVector); // operator= 0x00461017

class CSoundDef : public CParentDefClassBase { public:            // vtable 0x012349B4
    CSoundBankEntryVector         SoundBankEntries;          // +0x3c
    CAtmosBankEntryVector         AtmosBankEntries;          // +0x48
    CMusicEntryVector             MusicEntries;              // +0x54
    CMusicSetEntryVector          MusicSetEntries;           // +0x60
    CReverbEnvironmentEntryVector ReverbEnvironmentEntries;  // +0x6c
    float AtmosListenerDampingMin;                           // +0x78
    float AtmosListenerDampingRange;                         // +0x7c
    float DistanceModelCurve;                                // +0x80
    CSoundDef& operator=(const CSoundDef& other);
};

CSoundDef& CSoundDef::operator=(const CSoundDef& other)
{
    CParentDefClassBase::operator=(other);
    SoundBankEntries = other.SoundBankEntries;
    AtmosBankEntries = other.AtmosBankEntries;
    MusicEntries = other.MusicEntries;
    MusicSetEntries = other.MusicSetEntries;
    ReverbEnvironmentEntries = other.ReverbEnvironmentEntries;
    AtmosListenerDampingMin = other.AtmosListenerDampingMin;
    AtmosListenerDampingRange = other.AtmosListenerDampingRange;
    DistanceModelCurve = other.DistanceModelCurve;
    return *this;
}