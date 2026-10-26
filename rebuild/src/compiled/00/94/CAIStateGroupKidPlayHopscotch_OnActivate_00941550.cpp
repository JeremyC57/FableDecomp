// CAIStateGroup_KidPlayHopscotch::OnActivate() -- count the game start and broadcast a "hopscotch started"
// message (type 0x18) from the hopscotch marker's position, naming the kid and the marker.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00941550, length 83 bytes (+ int3 padding).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (the marker pointer at +0x15 -- the object really is
// unaligned: a bool at +0x14 precedes it, hence `lea edi,[esi+15h]`; called twice), CAIStateGroupBase::GetPWorld
// @ 0x008FCFC0, CThing::GetPosition @ 0x004C73D0 (C3DVector*), CMessageEventManager::AddMessage(type, ulong
// distance, C3DVector* pos, CThing* source, CThing* extra, float, float, long) @ 0x00687540 on the world's
// message manager at +0x60 (result dropped).
// Shape: the counter at +0x60 is incremented first, then the creature, the marker and the world's message
// manager are NAMED locals (ebx, ebp, esi) read before the argument pushes (a bare `GetPWorld()->mgr->Add(...)`
// makes the world call after the pushes); the arguments go right to left (0, 0.0f, 0.0f, GetPItem(), creature,
// marker->GetPosition(), 9999999, 0x18). No engine header for this class; the layout is local (pack(1) for the
// unaligned pointer), names descriptive.
class C3DVector;

class CThing { public:
    C3DVector* GetPosition();               // 0x004C73D0
};

class CMessageEvent;

class CMessageEventManager { public:
    CMessageEvent* AddMessage(long type, unsigned long distance, C3DVector* pos, CThing* source, CThing* extra,
                              float a, float b, long c);     // 0x00687540
};

class CWorld { public:
    unsigned char _pad[0x60];
    CMessageEventManager* m_pMessageManager;    // +0x60
};

#pragma pack(push, 1)
struct CBaseIntelligentPointer {
    void* __vftable;
    CThing* m_pItem;
    CThing* GetPItem();                     // 0x00A01B50
};

class CAIBrain { public:
    unsigned char _pad[0x18];
    CThing* Creature;                       // +0x18
};

class CAIStateGroupBase { public:
    void* __vftable;
    CAIBrain* PBrain;                       // +0x04
    unsigned char _pad[0xc];                // +0x08..+0x13
    CWorld* GetPWorld();                    // 0x008FCFC0
};

struct CAIStateGroup_KidPlayHopscotch : CAIStateGroupBase {
    bool m_initiated;                       // +0x14
    CBaseIntelligentPointer m_pMarker;      // +0x15 CIntelligentPointer<CThing> (unaligned)
    unsigned char _pad1d[0x43];             // +0x1d
    long m_gamesStarted;                    // +0x60
    void OnActivate();
};
#pragma pack(pop)

void CAIStateGroup_KidPlayHopscotch::OnActivate()
{
    ++m_gamesStarted;
    CThing* creature = PBrain->Creature;
    CThing* marker = m_pMarker.GetPItem();
    CMessageEventManager* messages = GetPWorld()->m_pMessageManager;
    messages->AddMessage(0x18, 9999999, marker->GetPosition(), creature, m_pMarker.GetPItem(), 0.0f, 0.0f, 0);
}