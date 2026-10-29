// NTCScriptedControl::CActionTalkBase::OnAdd() -- when the talk should make its target listen and the target is another thing under scripted control, open a new conversation and queue a matching talked-to action on the target
// Leaf: ?OnAdd@CActionTalkBase@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00714D30, 205 bytes (+3 int3 padding); vtable 0x012659A8 slot 3.
// FableWin.pdb: ?OnAdd@CActionTalkBase@NTCScriptedControl@@UAEXXZ (slot 3 = OnAdd, as the landed
//          CActionFireProjectileWeapon::OnAdd @ 0x00715110).
// Callees: CScriptThing virtual slot 11 GetThing (+0x2c), CVectorMap::LowerBound @ 0x0040F020 (inlined GetInterface,
//          interface 0x1F = CTCScriptedControl, the sign bit of the bitset dword +0x20), operator new @ 0x00BFEA1A,
//          CBaseObject() @ 0x0099A380 (manifest "CBaseIntelligentPointer" is a byte twin; landed rows name it CBaseObject),
//          CScriptConversation vtable 0x01265520 (rtti-vtables; slot 0 = 0x00718840), CCountedPointer::Reset @ 0x00718940,
//          CActionTalkedTo::CActionTalkedTo(CThing*, CBaseObject*) @ 0x00712DF0, CTCScriptedControl::AddAction @ 0x007137D0.
// Layout: retail CActionTalkBase PTargetThing (CScriptThing) +0x0C, Conversation (CCountedPointer, Ego_r.pdb +0x44) +0x40,
//          MakeTargetListen (PDB +0x72) +0x6E -- the debug CArray Messages is 4 bytes larger; the NTCScriptedControl
//          CScriptConversation is an 8-byte CBaseObject {vptr, +4}; CActionTalkedTo is 0x20 bytes.
// Shape: the inline conversation ctor (base ctor, then its vtable store) under the usual new-null test; the action's
//          new-expression arguments (ParentThing, Conversation.Get()) are read after its allocation; the parent thing is
//          read into a local before the GetThing() compare; the inline Get() accessor (not a raw `.Data` read) keeps the
//          conversation pointer in esi.
class CTCBase;
class CBaseObject {
public:
    void* Info;                                    // +0x04
protected:
    CBaseObject();                                       // 0x0099A380
    virtual ~CBaseObject();                              // 0x0099A430
};
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const ETCInterfaceType& type);          // 0x0040F020
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CThing { public:
    void*         __vftable;
    unsigned char _pad04[0x20 - 0x04];
    unsigned long UsingComponents[9];              // +0x20
    CInterfaceMap Components;                      // +0x44
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (UsingComponents[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
            CInterfaceMap* map = &Components;
            CInterfaceMap::SEntry* it = map->LowerBound(type);
            if (it == map->last || interfaceType < it->first)
                it = map->last;
            *ppOut = (T*)it->second;
            return true;
        }
        return false;
    }
};
class CCPPointerInfo;
class CScriptThing { public:
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005();
    virtual void v006(); virtual void v007(); virtual void v008(); virtual void v009(); virtual void v010();
    virtual CThing* GetThing() const;              // slot 11 (+0x2c)
    CScriptThing*   PImp_Data;                     // +0x04
    CCPPointerInfo* PImp_Info;                     // +0x08
};
template<class T> class CCountedPointer { public:
    T*              Data;                          // +0x00
    CCPPointerInfo* Info;                          // +0x04
    void Reset(T* object);                         // 0x00718940
    T* Get() const { return Data; }
};

namespace NTCScriptedControl {
class CActionBase;
class CScriptConversation : public CBaseObject {   // vtable 0x01265520, 8 bytes
public:
    CScriptConversation() { }
    virtual ~CScriptConversation();                // 0x00718840 (??_G)
};
}

class CTCScriptedControl { public:                        // interface 0x1F
    void*   __vftable;
    CThing* ParentThing;                           // +0x04 (CTCBase)
    void AddAction(NTCScriptedControl::CActionBase* action);   // 0x007137D0
};

namespace NTCScriptedControl {
class CActionBase {
public:
    virtual ~CActionBase();
    virtual void Update();                         // slot 1
    virtual void OnActivate();                     // slot 2
    virtual void OnAdd();                          // slot 3
    CTCScriptedControl* PTCScript;                 // +0x04
    bool                Activated;                 // +0x08
    bool                Terminated;                // +0x09
};

class CActionTalkedTo : public CActionBase {       // 0x20 bytes
public:
    CActionTalkedTo(CThing* talker, CBaseObject* conversation);   // 0x00712DF0
    unsigned char _pad0c[0x20 - 0x0c];
};

class CActionTalkBase : public CActionBase {
public:
    virtual void OnAdd();
    CScriptThing  PTargetThing;                    // +0x0C
    unsigned char _pad18[0x40 - 0x18];             // UpdateCounter, line times, Messages, CurrentMessageIndex
    CCountedPointer<CScriptConversation> Conversation;   // +0x40
    unsigned char _pad48[0x6e - 0x48];             // GameTurnDuration .. PreviousConversationAttitude, AddedConversationMode, FarEnoughAwayForFacingCamera
    bool          MakeTargetListen;                // +0x6E
};

void CActionTalkBase::OnAdd()
{
    if (!MakeTargetListen)
        return;
    CThing* parent = PTCScript->ParentThing;
    if (PTargetThing.GetThing() == parent)
        return;
    CTCScriptedControl* control;
    if (!PTargetThing.GetThing()->GetInterface(&control, 0x1f))
        return;
    Conversation.Reset(new CScriptConversation);
    control->AddAction(new CActionTalkedTo(PTCScript->ParentThing, Conversation.Get()));
}
}