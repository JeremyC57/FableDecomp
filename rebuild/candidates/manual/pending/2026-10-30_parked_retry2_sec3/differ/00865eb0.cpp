// CCombatAction_GenericStrikeResponseKnockdownRecoverBase::Initialise() -- plays the recover animation named by the preceding animation entry's data, else RESPONSE_GENERIC_HIT_KNOCKDOWN_RECOVER, then runs the base Initialise
// Grade: DIFFER(144v144) -- scratch-register rotation only; tried (this sweep): shared engine/CCountedPointer.h (IsNull / operator->; kept), inline CDefString::GetString() (long and by-value CDefString table forms)
// Retail: Fable.exe (TLC, VC7.1) @ 0x00865EB0, 144 bytes. Initialise is vtable slot 12.
// Callees: CAnimationEntry lookup @ 0x00867FE0 (thiscall, one out pointer, `ret 4`: finds the entry's type-8 data
//          record in its own sorted map, else in the global default map @ 0x013B8C38; manifest label
//          "CTCBase_GetInterfaceAbility" is propagated), CDefStringTable::GetString(long) @ 0x009D49B0 on the global
//          string table @ 0x013CA828 (CCharString returned through the hidden pointer), CCharString::operator!=(const char*)
//          @ 0x0099E960, CCombatActionBase::SetCombatAnimation(const CCharString&) @ 0x008574D0,
//          CCharString::~CCharString @ 0x0099EAE0, CCharString::CCharString(const char*, long) @ 0x0099EBF0,
//          CCombatActionBase::Initialise @ 0x00857520 (direct base call).
// Shape: a `found` flag in bl (`xor bl,bl` before the pointer test) carries the outcome of the first block past
// the name's destructor; the fallback literal is a named CCharString in its own block sharing the name's
// frame word. PPreceedingPAnimEntry is the CCountedPointer<CAnimationEntry> at +0x7c (PDB offset, unchanged in
// retail: the 12 bytes the retail CCreatureActionBase lacks come after it).
#include "engine/CCountedPointer.h"
struct CCharString {
    void* m_pData;
    CCharString(const char* text, long length);         // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
    bool operator!=(const char* text) const;            // 0x0099E960
};

class CDefString;
struct CDefStringTable {
    CCharString GetString(CDefString name) const;       // 0x009D49B0
};
extern CDefStringTable g_DefStringTable;                // 0x013CA828
class CDefString {
public:
    long TablePos;
    CCharString GetString() const { return g_DefStringTable.GetString(*this); }   // Ego_r ?GetString@CDefString@@QBE?AVCCharString@@XZ (inline)
};

struct CAnimationNameData {                 // the type-8 data record of an animation entry
    unsigned char _pad0[8];
    CDefString Name;                        // +0x08 index into the def string table
};

struct CAnimationEntry {
    void GetNameData(CAnimationNameData** ppOut) const;  // 0x00867FE0
};


struct CCombatActionBase {                  // retail sizeof 0x114
    virtual void v00();
    virtual void Initialise();              // slot 12: 0x00857520
    unsigned char _pad04[0x7c - 4];
    CCountedPointer<CAnimationEntry> PPreceedingPAnimEntry;   // +0x7c
    unsigned char _pad84[0x114 - 0x84];
    void SetCombatAnimation(const CCharString& name);   // 0x008574D0
};

struct CCombatAction_GenericStrikeResponseKnockdownRecoverBase : CCombatActionBase {
    bool CanBeHit;                          // +0x114
    bool Flip;                              // +0x115
    virtual void Initialise();
};

void CCombatAction_GenericStrikeResponseKnockdownRecoverBase::Initialise()
{
    bool found = false;
    if (!PPreceedingPAnimEntry.IsNull()) {
        CAnimationNameData* data;
        PPreceedingPAnimEntry->GetNameData(&data);
        CCharString name = data->Name.GetString();
        if (name != "") {
            SetCombatAnimation(name);
            found = true;
        }
    }
    if (!found) {
        CCharString name("RESPONSE_GENERIC_HIT_KNOCKDOWN_RECOVER", -1);
        SetCombatAnimation(name);
    }
    CCombatActionBase::Initialise();
}
