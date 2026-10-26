// CCreatureAction_CallSpouseHome::PerformAction() -- looks up the "STANDARD_MESSAGE_EVENT_CALL_SPOUSE_HOME" message event def through the creature's definition manager and, when found, posts it from the creature's position with the creature as the first thing; releases the def
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00849140, length 158 bytes (+ int3 padding).
// Callees: CCharString::CCharString(const char*, long) @ 0x0099EBF0, CCreatureActionBase::GetPDefinitionManager @ 0x00692910 (`PCreature.GetPItem()`
// then the creature's manager getter @ 0x004C7990; the manifest label is STL noise), CDefinitionManager::GetDef<T>(const CCharString&,
// CDefPointer<T>&) @ 0x007923C0 (bool), CCharString::~CCharString @ 0x0099EAE0, CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PCreature
// at +8, three times), CCreatureActionBase::GetPWorld @ 0x00692940 (landed naming of 0x00842F70), CThing::GetPosition @ 0x006A4D00,
// CMessageEventManager::AddMessage(CMessageEventDef*, const C3DVector*, CThing*, CThing*, long) @ 0x00687730, then the def's release
// (`dec [p+4] ; jne ; call [vt+4]`) written by hand on a pointer local (GOTCHAS wave 25).
// Shape: the name is a named local built first (`push -1 ; push offset` before the frame zero store); the def pointer is copied into
// ebp right after the lookup result is moved to bl; inside the `if` the creature is fetched twice into locals (the first unused), the
// manager into a third (edi, `this` being dead by then), then the five arguments right to left (0, 0, the creature, its position, the def).
// Layout: retail CCreatureActionBase is 0xa8 bytes with PCreature at +8; no engine header for this class, so `this` is modelled locally.
class C3DVector;
class CMessageEvent;

class CCharString { public:                        // one pointer (PStringData)
    void* m_pData;
    CCharString(const char* text, long length);   // 0x0099EBF0
    ~CCharString();                         // 0x0099EAE0
};

class CMessageEventDef { public:                   // CDefBase: vptr, then the reference count at +4
    virtual void v00();
    virtual void Destroy();                 // slot 1 (+4)
    long m_count;                           // +4
};

struct CDefPointer {                        // CDefPointer<CMessageEventDef>: one word, zero-initialised; released by hand below
    CMessageEventDef* m_p;
    CDefPointer() : m_p(0) { }
};

class CDefinitionManager { public:
    bool GetDef(const CCharString& name, CDefPointer& def);   // 0x007923C0
};

class CThing { public:
    const C3DVector& GetPosition_006a4d00() const;   // 0x006A4D00
};

class CMessageEventManager { public:
    CMessageEvent* AddMessage(CMessageEventDef* def, const C3DVector* position, CThing* thing1, CThing* thing2, long priority);   // 0x00687730
};

class CWorld { public:
    unsigned char _pad[0x60];
    CMessageEventManager* m_pMessageEventManager;   // +0x60
};

struct CBaseIntelligentPointer {            // CIntelligentPointer<CThingCreatureBase>: vptr + item
    virtual void v00();
    CThing* m_pItem;                        // +4
    CThing* GetPItem() const;               // 0x00A01B50
};

class CCreatureActionBase { public:                // retail sizeof 0xa8
    virtual void v00();
    unsigned char _pad04[4];                // +0x04
    CBaseIntelligentPointer PCreature;      // +0x08 CIntelligentPointer<CThingCreatureBase>
    unsigned char _pad10[0xa8 - 0x10];      // +0x10
    CDefinitionManager* GetPDefinitionManager() const;   // 0x00692910
    CWorld* GetPWorld() const;              // 0x00692940
};

class CCreatureAction_CallSpouseHome : public CCreatureActionBase { public:
    virtual void PerformAction();
};

void CCreatureAction_CallSpouseHome::PerformAction()
{
    CDefPointer def;
    CMessageEventDef* p;
    bool found;
    {
        CCharString name("STANDARD_MESSAGE_EVENT_CALL_SPOUSE_HOME", -1);
        found = GetPDefinitionManager()->GetDef(name, def);
    }
    p = def.m_p;
    if (found) {
        CThing* pCreatureThing = PCreature.GetPItem();   // fetched but unused: retail keeps the dead call
        CThing* pCreature = PCreature.GetPItem();
        CMessageEventManager* pManager = GetPWorld()->m_pMessageEventManager;   // named: resolved before the arguments
        pManager->AddMessage(p, &pCreature->GetPosition_006a4d00(), PCreature.GetPItem(), 0, 0);
    }
    if (p) {
        if (--p->m_count == 0)
            p->Destroy();
    }
}