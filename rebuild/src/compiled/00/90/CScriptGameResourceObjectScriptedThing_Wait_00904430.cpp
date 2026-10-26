// CScriptGameResourceObjectScriptedThing::Wait(float seconds) -- queue a wait action on the live scripted thing's scripted control: an open-ended wait for 0 seconds, else a wait of the matching frame count
// Leaf: Wait@CScriptGameResourceObjectScriptedThing
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00904430, length 270 bytes (`ret 4`), vtable 0x0128D86C slot 29.
// PDB: ?Wait@CScriptGameResourceObjectScriptedThing@@UAEXM@Z.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (x3, PThing at +0x14), CVectorMap<ETCInterfaceType, CTCBase*>::
// LowerBound @ 0x0040F020 (inlined GetInterface, interface 0x1f), operator new @ 0x00BFEA1A (0x14 bytes), CBase::CBase
// @ 0x0099A2F0 (the inline NTCScriptedControl::CActionWait ctor, vtable 0x0128D760), GFSecondsToFrames(float) @ 0x00415FC1
// (`__stdcall`), CTCScriptedControl::AddAction @ 0x007137D0. Float pool 0x0122DEDC = 0.0f.
// Layout: engine/CScriptGameResourceObjectScriptedThing.h (PThing +0x14); CActionWait (PDB): CActionBase {PTCScript +4,
// Activated +8, Terminated +9}, NoFramesToWait +0xc, UpdateCounter +0x10.
// Shape: the ClearCommands 0x00904310 guard and unchecked GetInterface (control in ebp, the miss arm reads the dead `seconds`
// slot); `seconds == 0.0f` (`fld [0.0f] ; fld seconds ; fucompp ; test ah,44h ; jp`) picks one of two complete
// `AddAction(new CActionWait(frames))` statements; the frame count is evaluated before the inline ctor chain (a ctor
// argument), the new-expression's null result is shared by both arms; the ctors are `__forceinline`.
#include "engine/CScriptGameResourceObjectScriptedThing.h"

void* __cdecl operator new(unsigned int size);     // 0x00BFEA1A
long __stdcall GFSecondsToFrames(float seconds);   // 0x00415FC1

class CTCBase;

struct CInterfaceMap {                             // CVectorMap<ETCInterfaceType, CTCBase*>
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);          // 0x0040F020
};

class CThing_Retail { public:
    unsigned char  _pad00[0x20];
    unsigned long  m_presentBits[9];               // +0x20 std::bitset of interface types
    CInterfaceMap  m_interfaces;                   // +0x44
    unsigned char  _pad4c[0x45];
    unsigned char  m_dead : 1;                     // +0x91 bit 0
    unsigned char  m_91_1 : 7;
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

struct CBase {                                     // CBaseClassNonCopyable root
    virtual ~CBase();
    CBase();                                       // 0x0099A2F0
};
class CTCScriptedControl;
namespace NTCScriptedControl {
struct CActionBase : CBase {                       // vtable 0x01265504
    CTCScriptedControl* PTCScript;                 // +0x04
    bool                Activated;                 // +0x08
    bool                Terminated;                // +0x09
    __forceinline CActionBase() : PTCScript(0), Activated(false), Terminated(false) { }
};
struct CActionWait : CActionBase {                 // vtable 0x0128D760, 0x14 bytes
    long NoFramesToWait;                           // +0x0c
    long UpdateCounter;                            // +0x10
    virtual ~CActionWait();
    __forceinline CActionWait(long frames) : NoFramesToWait(frames), UpdateCounter(0) { }
};
}

class CTCScriptedControl { public:                        // interface 0x1f component
    void AddAction(NTCScriptedControl::CActionBase* action);   // 0x007137D0
};

struct CScriptGameResourceObjectScriptedThing_Methods : CScriptGameResourceObjectScriptedThing {
    void Wait(float seconds);
};

void CScriptGameResourceObjectScriptedThing_Methods::Wait(float seconds)
{
    CIntelligentPointer<CThing_Retail>* thing = (CIntelligentPointer<CThing_Retail>*)PThing;
    if (thing->GetPItem()) {
        if (!thing->GetPItem()->m_dead) {
            CTCScriptedControl* control;
            thing->GetPItem()->GetInterface(&control, 0x1f);
            if (seconds == 0.0f)
                control->AddAction(new NTCScriptedControl::CActionWait(0));
            else
                control->AddAction(new NTCScriptedControl::CActionWait(GFSecondsToFrames(seconds)));
        }
    }
}