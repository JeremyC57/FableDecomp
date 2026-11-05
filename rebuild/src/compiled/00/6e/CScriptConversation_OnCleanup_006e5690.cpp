// CScriptConversation::OnCleanup(bool stopSpeech) -- for every person still alive in the conversation, clear its scripted look-at target (targeting component, interface 8) and, when asked, stop its current speech (talk component, interface 0xf)
// Leaf: OnCleanup
// Plan phase 2 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). By-value members re-padded to the header layout (re-padded by-value maps +8).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006E5690, length 242 bytes (`ret 4`).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (four per person), CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound
// @ 0x0040F020 (two inlined GetTC lookups), CTCTargetingBase::SetPScriptOverrideInterestingThing(CThing*) @ 0x007F5890 (with 0),
// CTCTalk::StopCurrentSpeech @ 0x006E2E60.
// Layout: as the landed CScriptConversation::IsValid 0x006E5600: PeopleInConversation (retail 12-byte vector of 8-byte
// CIntelligentPointer<CThing>) first/last at +8/+0xc (engine/CScriptConversation.h agrees on that offset).
// Shape: the IsValid loop (size() re-derived each pass, unsigned index); the alive test is the inline IS_THING_ALIVE on one
// element reference; each component block re-indexes the vector (`*(begin() + i)`, the scaled index kept in edi), fetches the
// item once and tests the raw bitset (`test ch,1` / `test ch,ch ; jns`): the targeting block is the out-parameter GetInterface
// (component left in eax: `mov eax,[eax+4] ; push 0 ; mov ecx,eax`), the talk block HasTC plus the ternary entry-pick GetTC
// (`mov ecx,[eax+4]`).
class CTCBase;

#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CBaseObject;
class CBaseObjectPointer;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"


inline bool IS_THING_ALIVE(const CIntelligentPointer<CThing>& p)
{
    if (p.Get() != 0) {
        if (!p.Get()->IsDead())
            return true;
    }
    return false;
}

struct CRetailPointerVector {                  // retail vector<CIntelligentPointer<CThing> >: first, last, end of storage
    CIntelligentPointer<CThing>* m_pFirst;
    CIntelligentPointer<CThing>* m_pLast;
    CIntelligentPointer<CThing>* m_pEnd;
    CIntelligentPointer<CThing>* begin() const { return m_pFirst; }
    unsigned long size() const { return m_pLast - m_pFirst; }
    CIntelligentPointer<CThing>& operator[](unsigned long index) const { return *(begin() + index); }
};

class CTCTargetingBase { public:
    void SetPScriptOverrideInterestingThing(CThing* thing);   // 0x007F5890
};
class CTCTalk { public:
    void StopCurrentSpeech();                  // 0x006E2E60
};

struct CScriptConversationRetail {
    void*                Manager;              // +0x00 CScriptConversationManager*
    long                 ConversationIndex;    // +0x04
    CRetailPointerVector PeopleInConversation; // +0x08
    void OnCleanup(bool stopSpeech);
};

void CScriptConversationRetail::OnCleanup(bool stopSpeech)
{
    for (unsigned long i = 0; i < PeopleInConversation.size(); ++i) {
        if (IS_THING_ALIVE(PeopleInConversation[i])) {
            {
                CTCTargetingBase* targeting;
                if (PeopleInConversation[i].Get()->GetTC(TCI_TARGETING, &targeting))
                    targeting->SetPScriptOverrideInterestingThing(0);
            }
            if (stopSpeech) {
                CThing* thing = PeopleInConversation[i].Get();
                if (thing->HasTC(TCI_TALK))
                    ((CTCTalk*)&thing->GetTC(TCI_TALK))->StopCurrentSpeech();
            }
        }
    }
}