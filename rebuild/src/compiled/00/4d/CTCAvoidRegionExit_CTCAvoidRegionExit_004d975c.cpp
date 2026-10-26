// CTCAvoidRegionExit::CTCAvoidRegionExit(CThing* thing) -- CTCBase(thing), own vtable, the EntranceMarker and ExitMarker intelligent pointers constructed, NextFrameCheck = 0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D975C, length 61 bytes; a size-optimised TU
// (`push dword ptr [esp+10h]` pass-through, `and dword ptr [esi+1Ch],0`, the CIntelligentPointer vtable
// held in ebx for both member stores): `#pragma optimize("s", on)`.
// Callees: CTCBase::CTCBase(CThing*) @ 0x00686800, CBaseIntelligentPointer::CBaseIntelligentPointer()
// @ 0x00A01B10 (x2). Vtables: 0x0123EFAC = CTCAvoidRegionExit, 0x01238C6C = CIntelligentPointer (rtti-vtables.tsv).
// Layout = Ego_r.pdb CTCAvoidRegionExit (32 bytes): EntranceMarker +0xc, ExitMarker +0x14
// (CIntelligentPointer<CThing>), NextFrameCheck +0x1c. Shape: the vtable store precedes the member
// constructors (inline CIntelligentPointer() over the out-of-line base, vtable stored through the
// member's address register); the scalar initialiser is stored last.
#pragma optimize("s", on)
class CThing;

struct CTCBase_Calls {                                      // engine/CTCBase.h layout (0xc bytes)
    virtual void v00();
    CThing* ParentThing;                                    // +0x04
    bool    ToBeRemoved;                                    // +0x08
    bool    IsInGlobalUpdateTCs;                            // +0x09
    CTCBase_Calls(CThing* thing);                           // 0x00686800
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

template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {      // vtable 0x01238C6C
public:
    CIntelligentPointer() : CBaseIntelligentPointer() { }

    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};

class CTCAvoidRegionExit : public CTCBase_Calls { public:                 // vtable 0x0123EFAC
    CIntelligentPointer<CThing> EntranceMarker;                     // +0x0c
    CIntelligentPointer<CThing> ExitMarker;                         // +0x14
    unsigned long       NextFrameCheck;                     // +0x1c
    virtual void v00();
    CTCAvoidRegionExit(CThing* thing);
};

CTCAvoidRegionExit::CTCAvoidRegionExit(CThing* thing)
    : CTCBase_Calls(thing)
    , NextFrameCheck(0)
{
}