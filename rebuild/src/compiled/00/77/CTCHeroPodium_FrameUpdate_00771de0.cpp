// CTCHeroPodium::FrameUpdate() -- when the hero steps onto the podium post the podium message event (and reset the crowd), when he leaves remove it
// Leaf: FrameUpdate
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00771DE0, 225 bytes (+ int3 padding).
// Callees: CThing::GetPosition() const @ 0x004C73D0 (x2), CTCBase::GetWorld() @ 0x00686A70 (x2), CWorld::
//          GetCurrentPlayer() @ 0x0049D9D0, CPlayer::GetControlledCreature() @ 0x00487DD0, CMessageEventManager::
//          FindMostRecentMessage<CMessageEventFilter_IsOfType>(const filter&) @ 0x004AB8B0, std::vector<long>::resize(n,
//          const long&) @ 0x004428F0, CMessageEventManager::AddMessage(...) @ 0x00687540, ::RemoveMessage(const
//          CMessageEvent*) @ 0x00493B5B, std::vector<long>::clear() @ 0x00485D1A (out of line;
//          manifest "SetStaticCast" is propagated); the podium radius constant 1.0f @ 0x0126A88C (its own .rdata
//          word, not the shared 1.0f pool entry).
// Shape: the podium thing is a named local spilled to the frame (it is pushed as AddMessage's source at the end);
// `dx*dx + dy*dy < radius` in one expression (the FPU top, dy, squared first) feeds a bool in bl; the message
// manager (world +0x60) is a named local in edi; the type-0x23 filter and the resize value share the frame word.
// Layout: retail CTCHeroPodium NumberOfCrowdOccupants +0x0C, CrowdOccupation +0x10 (as the landed
// OnInitialActivate 0x00771BD0); CWorld PMessageEventManager +0x60.
class C3DVector { public:
    float X, Y, Z;
};

extern const float g_podiumRadiusSquared_0126a88c;   // 0x0126A88C (1.0f)

class CThing { public:
    const C3DVector& GetPosition() const;            // 0x004C73D0
};
class CPlayer { public:
    CThing* GetControlledCreature();                 // 0x00487DD0
};
class CMessageEvent;
class CMessageEventFilter_IsOfType { public:
    long Type;
    CMessageEventFilter_IsOfType(long type) : Type(type) {}
};
class CMessageEventManager { public:
    CMessageEvent* FindMostRecentMessage(const CMessageEventFilter_IsOfType& filter);   // 0x004AB8B0
    CMessageEvent* AddMessage(long type, unsigned long delay, const C3DVector& position, CThing* from, CThing* to,
                              float value1, float value2, long data);   // 0x00687540
    void RemoveMessage(const CMessageEvent* message);   // 0x00493B5B
};
class CWorld { public:
    unsigned char         _pad00[0x60];
    CMessageEventManager* PMessageEventManager;      // +0x60
    CPlayer* GetCurrentPlayer();                     // 0x0049D9D0
};

struct CLongVector {                                 // retail std::vector<long> (12 bytes)
    long* _M_start;
    long* _M_finish;
    long* _M_end_of_storage;
    void resize(unsigned long n, const long& value); // 0x004428F0
    void clear();                                    // 0x00485D1A
};

class CTCBase { public:
    virtual ~CTCBase();
    CThing* ParentThing;                             // +0x04
    bool    ToBeRemoved;                             // +0x08
    bool    IsInGlobalUpdateTCs;                     // +0x09
protected:
    CWorld& GetWorld() const;   // 0x00686A70
};

class CTCHeroPodium : public CTCBase { public:
    long        NumberOfCrowdOccupants;              // +0x0C
    CLongVector CrowdOccupation;                     // +0x10
    virtual void FrameUpdate();
};

void CTCHeroPodium::FrameUpdate()
{
    CThing* podium = ParentThing;
    const C3DVector& podiumPos = podium->GetPosition();
    CThing* hero = GetWorld().GetCurrentPlayer()->GetControlledCreature();
    if (hero) {
        const C3DVector& heroPos = hero->GetPosition();
        float dx = podiumPos.X - heroPos.X;
        float dy = podiumPos.Y - heroPos.Y;
        bool onPodium = dx * dx + dy * dy < g_podiumRadiusSquared_0126a88c ? true : false;
        CMessageEventManager* messages = GetWorld().PMessageEventManager;
        CMessageEvent* message = messages->FindMostRecentMessage(CMessageEventFilter_IsOfType(0x23));
        if (onPodium) {
            if (message == 0) {
                CrowdOccupation.resize(8, 0);
                NumberOfCrowdOccupants = 0;
                messages->AddMessage(0x23, 0x249F0, podiumPos, podium, 0, 0.0f, 0.0f, 0);
            }
        } else if (message) {
            messages->RemoveMessage(message);
            CrowdOccupation.clear();
        }
    }
}