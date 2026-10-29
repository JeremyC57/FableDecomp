// CAIStateGroup_GoToPub::PayMoney() -- find the barman through the bar's tavern component, ask him for payment and wait up to 20 seconds for his reply; on reply play PAY_AT_BAR and send the paid message, else withdraw the request and send the gave-up message; the bar point is released on every path past the barman lookup
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0094A840, length 637 bytes (+ int3 padding).
// Callees: CBaseIntelligentPointer ctor @ 0x00A01B10 / GetPItem @ 0x00A01B50 / SetPItem @ 0x00A01B90 / dtor @ 0x00A01C10,
// CVectorMap::LowerBound @ 0x0040F020, FreeBarPoint @ 0x00949320, CAIStateGroupBase::GetPWorld @ 0x008FCFC0 / IsStopping
// @ 0x008FD010 / AIAction_PerformActionAndWait @ 0x0097DB70, CMessageEventManager::AddMessage @ 0x00687540 / RemoveMessage
// @ 0x00493B5B, CWorld::GetWorldFrame @ 0x0049D870, GetPaymentRequestReplyMessage @ 0x00947E20, CCharString ctor
// @ 0x0099EBF0 / dtor @ 0x0099EAE0, CCreatureAction_PlayAnimation ctor @ 0x008424F0, ~CCreatureActionBase @ 0x00693EF0.
// Shape: every message call evaluates its non-trivial arguments (barman, creature, a named zero position) into registers
// before the world call, so they are named locals ahead of a named world / manager local; the barman copy is an inline
// by-value GetBarman (temporary CIntelligentPointer, vtable 0x0123E7E0).
// Layout: Ego_r.pdb CAIStateGroup_GoToPub PBar +0x1c, PBarman +0x2c, BarIndex +0x9c; CTCTavernBar (0xb9) Barman +0x18
// (retail STLport vector 12 bytes); CWorld +0x60 message event manager.
class C3DVector { public:
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
};
class CCharString { public:
    void* m_pData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};
class CSoundPair { public:
    long m_first;
    long m_second;
    CSoundPair() : m_first(0), m_second(0) { }
};
class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const ETCInterfaceType& type);              // 0x0040F020
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];                    // +0x20
    CInterfaceMap m_interfaces;                        // +0x44
    template<class T> __forceinline void GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(type);
            *ppOut = (T*)((it == map->last || interfaceType < it->first) ? map->last : it)->second;
        }
    }
};
class CThingCreatureBase : public CThing { public: };
class CThingAICreature;
class CThingObject;
class CThingBuilding;
class CBaseObject;
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;
    CBaseIntelligentPointer();                         // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                        // 0x00A01C10
    CBaseObject* GetPItem() const;                          // 0x00A01B50
    void SetPItem(const CThing* item);                 // 0x00A01B90
};
template<class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:
    virtual void v00();
    __forceinline CIntelligentPointer() { }
    __forceinline CIntelligentPointer(const CIntelligentPointer& other) { SetPItem(((CThing*)other.GetPItem())); }
    __forceinline CIntelligentPointer& operator=(const CIntelligentPointer& other) { CThing* item = ((CThing*)other.GetPItem()); SetPItem(item); return *this; }
};
class CTCTavernBar { public:                                  // interface 0xb9
    unsigned char _pad0[0x18];
    CIntelligentPointer<CThingAICreature> Barman;      // +0x18
    CIntelligentPointer<CThingAICreature> GetBarman() const { return Barman; }
};
class CCreatureActionBase { public:
    virtual void v00();
    unsigned char _body[0xa4];
    virtual ~CCreatureActionBase();                            // 0x00693EF0
};
class CCreatureAction_PlayAnimation : public CCreatureActionBase { public:
    unsigned char _anim[0x0c];
    CCreatureAction_PlayAnimation(CThingCreatureBase* creature, const CCharString& animation,
        const CSoundPair& sound, bool a, bool b, bool c, long d, long e);   // 0x008424F0
};
class CMessageEvent;
class CMessageEventManager { public:
    CMessageEvent* AddMessage(long type, unsigned long distance, const C3DVector& pos, CThing* source, CThing* target,
                              float a, float b, long index);   // 0x00687540
    void RemoveMessage(const CMessageEvent* message);   // 0x00493B5B
};
class CWorld { public:
    unsigned char _pad0[0x60];
    CMessageEventManager* MessageEventManager;         // +0x60
    static long __fastcall GetFrame();            // 0x0049D870
};
extern long g_framesPerSecond;                         // 0x01375550
class CAIBrain { public:
    unsigned char _pad[0x18];
    CThingCreatureBase* PCreature;                     // +0x18
};
enum EAIActionReturn { };
class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                                  // +0x04
    unsigned char _pad08[0x0c];
    CThingCreatureBase* GetCreature() const { return PBrain->PCreature; }
    CWorld* GetPWorld();                               // 0x008FCFC0
    bool IsStopping();                                 protected: // 0x008FD010
    EAIActionReturn AIAction_PerformActionAndWait(const CCreatureActionBase& action, bool wait); public:   // 0x0097DB70
};
struct CAIStateGroup_GoToPub : CAIStateGroupBase {
    CIntelligentPointer<CThingBuilding> PPub;          // +0x14
    CIntelligentPointer<CThingObject> PBar;            // +0x1c
    CIntelligentPointer<CThingObject> PTable;          // +0x24
    CIntelligentPointer<CThingAICreature> PBarman;     // +0x2c
    unsigned char _pad34[0x9c - 0x34];
    long BarIndex;                                     // +0x9c
    void FreeBarPoint();                               // 0x00949320
    bool GetPaymentRequestReplyMessage();              // 0x00947E20
    bool PayMoney();
};

bool CAIStateGroup_GoToPub::PayMoney()
{
    if (((CThing*)PBarman.GetPItem()) == 0) {
        CTCTavernBar* bar;
        ((CThing*)PBar.GetPItem())->GetInterface(&bar, 0xb9);
        PBarman = bar->GetBarman();
        if (((CThing*)PBarman.GetPItem()) == 0) {
            FreeBarPoint();
            return false;
        }
    }
    CMessageEvent* request;
    {
        CThing* barman = ((CThing*)PBarman.GetPItem());
        CThingCreatureBase* creature = GetCreature();
        C3DVector pos(0.0f, 0.0f, 0.0f);
        CMessageEventManager* messages = GetPWorld()->MessageEventManager;
        request = messages->AddMessage(0x30, 1000, pos, creature, barman, 0.0f, 0.0f, BarIndex);
    }
    long fps = g_framesPerSecond;
    long timeout = CWorld::GetFrame() + fps * 20;
    while (!GetPaymentRequestReplyMessage()) {
        if (CWorld::GetFrame() > timeout || IsStopping()) {
            {
                CWorld* world = GetPWorld();
                world->MessageEventManager->RemoveMessage(request);
            }
            CThing* barman = ((CThing*)PBarman.GetPItem());
            CThingCreatureBase* creature = GetCreature();
            C3DVector pos(0.0f, 0.0f, 0.0f);
            CWorld* world = GetPWorld();
            world->MessageEventManager->AddMessage(0x31, 1000, pos, creature, barman, 0.0f, 0.0f, BarIndex);
            FreeBarPoint();
            return false;
        }
    }
    AIAction_PerformActionAndWait(CCreatureAction_PlayAnimation(GetCreature(), "PAY_AT_BAR", CSoundPair(), false, true,
                                                                 false, 0, 0), false);
    {
        CThingCreatureBase* creature = GetCreature();
        C3DVector pos(0.0f, 0.0f, 0.0f);
        CMessageEventManager* messages = GetPWorld()->MessageEventManager;
        messages->AddMessage(0x30, 1000, pos, creature, ((CThing*)PBarman.GetPItem()), 0.0f, 0.0f, 0);
    }
    FreeBarPoint();
    return true;
}