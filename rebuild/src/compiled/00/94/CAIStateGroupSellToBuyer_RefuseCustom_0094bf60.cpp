// CAIStateGroup_SellToBuyer::RefuseCustom() -- the seller refuses the buyer's custom: post message 0x10 from the selling creature to the buyer (delay 1000, a zero position, zero values) through the world's message-event manager
// Leaf: ?RefuseCustom@CAIStateGroup_SellToBuyer
// Retail: Fable.exe (TLC, VC7.1) @ 0x0094BF60, 88 bytes (+ int3 padding to 96), plain `ret`.
// Callees: CAIStateGroupBase::GetPWorld @ 0x008FCFC0 (PBrain +0x1c -> +0x24), CBaseIntelligentPointer::GetPItem
//   @ 0x00A01B50 (PBuyer at +0x14), CMessageEventManager::AddMessage @ 0x00687540 (the world's manager at +0x60).
// Shape: the creature is read into a named local first (edi), the zero C3DVector is a named local built before the
//   world call (three `mov [esp+..],0` stores), and the manager is a named local fetched BEFORE the arguments are
//   evaluated (`mov ebx,[eax+60h]`), so GetPItem runs between the pushes; VC evaluates the arguments right to left.
// Layout: Ego_r.pdb CAIStateGroup_SellToBuyer::PBuyer (CIntelligentPointer<CThingCreatureBase>) at +0x14; the base's
//   PBrain at +0x04 and the brain's creature at +0x18 as in the landed AIStateGroup rows. Local model (the engine
//   header flattens the base).
// Label: catalog "make_pair<C3DVector_const_,int>" -> Ego_r.pdb ?RefuseCustom@CAIStateGroup_SellToBuyer@@AAEXXZ;
//   0x008FCFC0 is GetPWorld (not CPlayer::GetCurrentMode).
// Build: the lane defaults (/O2 /Oy /W3), no pragma.
class CThing;

class C3DVector { public:
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
};

class CMessageEvent;
class CMessageEventManager { public:
    CMessageEvent* AddMessage(long type, unsigned long delay, const C3DVector& position, CThing* from, CThing* to,
                              float value1, float value2, long data);   // 0x00687540
};

class CWorld { public:
    unsigned char _pad[0x60];
    CMessageEventManager* MessageEventManager;  // +0x60
};

struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CThing* GetPItem() const;                   // 0x00A01B50
};

class CAIBrain { public:
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};

class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    unsigned char Terminated, Priority, Enabled, _pad13;
    CThing* GetCreature() const { return PBrain->PCreature; }
    CWorld* GetPWorld();                        // 0x008FCFC0
};

class CAIStateGroup_SellToBuyer : public CAIStateGroupBase {
public:
    CBaseIntelligentPointer PBuyer;             // +0x14 CIntelligentPointer<CThingCreatureBase>
private:
    void RefuseCustom();
};

void CAIStateGroup_SellToBuyer::RefuseCustom()
{
    CThing* creature = GetCreature();
    C3DVector position(0.0f, 0.0f, 0.0f);
    CMessageEventManager* messages = GetPWorld()->MessageEventManager;
    messages->AddMessage(0x10, 1000, position, creature, PBuyer.GetPItem(), 0.0f, 0.0f, 0);
}