// CScriptGameResourceObjectScriptedThing::CScriptGameResourceObjectScriptedThing(CGameScriptInterface& scriptInterface, CThing& thing, long scriptID, EScriptAIPriority priority) -- constructor for a scripted thing: lock the thing's scripted control to this handle at the given priority and drop a creature out of mode 7
// Leaf: ??0CScriptGameResourceObjectScriptedThing@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009039D0, length 234 bytes (`ret 10h`).
// PDB: ??0CScriptGameResourceObjectScriptedThing@@QAE@AAVCGameScriptInterface@@AAVCThing@@JW4EScriptAIPriority@@@Z.
// Callees: CBaseObject::CBaseObject @ 0x0099A380, CBaseIntelligentPointer::CBaseIntelligentPointer @ 0x00A01B10,
// CBaseIntelligentPointer::SetPItem @ 0x00A01B90, CBaseIntelligentPointer::GetPItem @ 0x00A01B50,
// CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound @ 0x0040F020 (two inlined GetInterface lookups),
// CTCScriptedControl::SetLocked(CScriptGameResourceObjectScriptedThing&, EScriptAIPriority) @ 0x00712C20,
// CTCCreatureModeManager::GetCurrentMode @ 0x0073A6E0 (manifest "CPixelShader::CountReferences" is propagated),
// the mode's virtual GetModeType (slot 1), CTCCreatureModeManager::RemoveMode(unsigned long) @ 0x0073ABA0.
// Vtables: 0x0128D86C (CScriptGameResourceObjectScriptedThing), 0x01238C6C (CIntelligentPointer<CThing>).
// Shape: the landed default constructor's chain (0x00903710: handle base inline, PImp zeroed through the
// shared zero register, derived vtable, ScriptInterface) with PThing built from the thing (inline
// CIntelligentPointer(CThing*) = base call, vtable, SetPItem), then ScriptID, NullResource = false and
// ActionPriority = 0 from the same zero register. The body locks the scripted control (interface 0x1f,
// an unchecked inline GetTC whose end/hit choice is ONE ternary, so the component lands straight in ecx for
// SetLocked; the miss reads the never-written local from the key word in the dead `scriptInterface` slot)
// and, when the thing has
// a creature mode manager (interface 0x31 = bit 17 of the bitset word at +0x24), removes mode 7 if it is current.
// Layout: engine/CScriptGameResourceObjectScriptedThing.h (PDB, sizeof 0x28) agrees on every offset but is a
// flat record without bases or constructors, so the class chain is modelled locally (as in 0x00903710).
class CCPPointerInfo;
class CGameScriptInterface;
class CTCBase;

class CBaseObject { public:
    virtual void v00();
    void* IntelligentPointer;                         // +0x04
    CBaseObject();                                    // 0x0099A380
};

struct CInterfaceMap {                                // CVectorMap<ETCInterfaceType, CTCBase*>
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);             // 0x0040F020
};

class CThing : public CBaseObject { public:
    unsigned char  _pad08[0x18];
    unsigned long  m_presentBits[9];                  // +0x20 std::bitset of interface types
    CInterfaceMap  m_interfaces;                      // +0x44
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
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
        T* tc;
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            tc = (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
        }
        return tc;
    }
};

class CScriptGameResourceObjectScriptedThingBase;
struct CCountedPointer_ScriptedThingBase {            // CCountedPointer<CScriptGameResourceObjectScriptedThingBase>
    CScriptGameResourceObjectScriptedThingBase* Data;
    CCPPointerInfo*                             Info;
    CCountedPointer_ScriptedThingBase() : Data(0), Info(0) { }
};

class CScriptGameResourceObjectScriptedThingBase : public CBaseObject { public:
    CCountedPointer_ScriptedThingBase PImp;           // +0x08 PImp_Data / +0x0c PImp_Info
    virtual void v00();
    CScriptGameResourceObjectScriptedThingBase() { }
};

struct CBaseIntelligentPointer {
    virtual void v00();
    void* m_pItem;
    CBaseIntelligentPointer();                        // 0x00A01B10
    void SetPItem(CBaseObject* item);                 // 0x00A01B90
    CBaseObject* GetPItem() const;                    // 0x00A01B50
};
struct CIntelligentPointer : CBaseIntelligentPointer {   // CIntelligentPointer<CThing>, vtable 0x01238C6C
    virtual void v00();
    CIntelligentPointer(CThing* item) : CBaseIntelligentPointer() { SetPItem(item); }
    CThing* GetPItem() const { return (CThing*)CBaseIntelligentPointer::GetPItem(); }
};

enum EScriptAIPriority { SCRIPT_AI_PRIORITY_0 = 0 };

class CScriptGameResourceObjectScriptedThing;
class CTCScriptedControl { public:                           // interface 0x1f component
    void SetLocked(CScriptGameResourceObjectScriptedThing& owner, EScriptAIPriority priority);   // 0x00712C20
};

struct CCreatureMode {
    virtual void v00();
    virtual unsigned long GetModeType();              // slot 1
};
class CTCCreatureModeManager { public:                       // interface 0x31 component
    CCreatureMode* GetCurrentMode();                  // 0x0073A6E0
    void RemoveMode(unsigned long mode);              // 0x0073ABA0
};

class CScriptGameResourceObjectScriptedThing : public CScriptGameResourceObjectScriptedThingBase { public:   // vtable 0x0128D86C
    CGameScriptInterface* ScriptInterface;            // +0x10
    CIntelligentPointer   PThing;                     // +0x14
    long                  ScriptID;                   // +0x1c
    bool                  NullResource;               // +0x20
    long                  ActionPriority;             // +0x24
    virtual void v00();
    CScriptGameResourceObjectScriptedThing(CGameScriptInterface& scriptInterface, CThing& thing, long scriptID, EScriptAIPriority priority);
};

CScriptGameResourceObjectScriptedThing::CScriptGameResourceObjectScriptedThing(CGameScriptInterface& scriptInterface, CThing& thing, long scriptID, EScriptAIPriority priority)
    : ScriptInterface(&scriptInterface), PThing(&thing), ScriptID(scriptID), NullResource(false), ActionPriority(0)
{
    PThing.GetPItem()->GetTC<CTCScriptedControl>(0x1f)->SetLocked(*this, priority);
    CTCCreatureModeManager* modes;
    if (PThing.GetPItem()->GetInterface(&modes, 0x31)) {
        if (modes->GetCurrentMode()->GetModeType() == 7)
            modes->RemoveMode(7);
    }
}