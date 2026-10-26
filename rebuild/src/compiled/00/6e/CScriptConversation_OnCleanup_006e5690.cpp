// CScriptConversation::OnCleanup(bool stopSpeech) -- for every person still alive in the conversation, clear its scripted look-at target (targeting component, interface 8) and, when asked, stop its current speech (talk component, interface 0xf)
// Leaf: OnCleanup
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

struct CInterfaceMap {                         // CVectorMap<ETCInterfaceType, CTCBase*>
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);      // 0x0040F020
};

class CThing { public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];            // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                // +0x44
    unsigned char _pad4c[0x45];
    unsigned char m_flags91;                   // +0x91 bit 0: dead
    __forceinline unsigned long HasTC(long interfaceType) const
    {
        return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31));
    }
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (HasTC(interfaceType)) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
    template<class T> __forceinline T* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
    }
};

class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer {                       // vtable 0x0129C95C (Ego_r: vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
private:
    CBaseObjectPointer* PData;                           // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};

inline bool IS_THING_ALIVE(const CIntelligentPointer<CThing>& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
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
                if (PeopleInConversation[i].GetPItem()->GetInterface(&targeting, 8))
                    targeting->SetPScriptOverrideInterestingThing(0);
            }
            if (stopSpeech) {
                CThing* thing = PeopleInConversation[i].GetPItem();
                if (thing->HasTC(0xf))
                    thing->GetTC<CTCTalk>(0xf)->StopCurrentSpeech();
            }
        }
    }
}