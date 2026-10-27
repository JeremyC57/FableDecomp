// CVillageTaskCarryCrate::RemoveThingFromTask(const CThing& thing, bool& removed) -- if the thing is the task's PCrateStack, clear it and report true; otherwise clear the assigned creature and report false
// Leaf: ?RemoveThingFromTask@CVillageTaskCarryCrate@@UAEXABVCThing@@AA_N@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00861030, 158 bytes, thiscall, `ret 8`.
// Identity (proven): vtable 0x0125A950 (RTTI .?AVCVillageTaskCarryCrate@@) slot 6; Ego_r.pdb ?RemoveThingFromTask@CVillageTaskCarryCrate@@UAEXABVCThing@@AA_N@Z; the members it clears are Ego_r.pdb PCrateStack (CIntelligentPointer<CThing>, vtable 0x01238C6C) and CreatureAssignedToTask (CIntelligentPointer<const CThing>, vtable 0x01237F1C).
// Callees: CBaseIntelligentPointer::CBaseIntelligentPointer 0x00A01B10, GetPItem() const 0x00A01B50, SetPItem 0x00A01B90,
//   ~CBaseIntelligentPointer 0x00A01C10 (each `member = 0` builds a CIntelligentPointer temporary: base ctor, vptr, SetPItem(0),
//   then the member's SetPItem(temp.GetPItem()) and the temporary's base destructor).
// Header debt: engine/CVillageTaskCarryCrate.h is a flat byte layout without its CVillageTaskBase base or methods; modelled locally.
class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer {                       // Ego_r: vptr + PData, 8 bytes
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
    CIntelligentPointer(T* item) : CBaseIntelligentPointer() { SetPItem(item); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); return *this; }
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};
class CThing { };
class CThingObject : public CThing { };
class CVillageTaskBase {                               // retail: vptr + one dword
public:
    virtual ~CVillageTaskBase();
    unsigned char _pad04[4];                           // +0x04
};
class CVillageTaskCarryCrate : public CVillageTaskBase {
public:
    CIntelligentPointer<const CThing> CreatureAssignedToTask;   // +0x08
    CIntelligentPointer<CThing>       PCrateStack;            // +0x10
    bool                              Pickup;                   // +0x18
    long                              Priority;                 // +0x1C
    virtual void RemoveThingFromTask(const CThing& thing, bool& removed);
};
void CVillageTaskCarryCrate::RemoveThingFromTask(const CThing& thing, bool& removed)
{
    if (PCrateStack.GetPItem() == &thing) {
        PCrateStack = 0;
        removed = true;
    } else {
        CreatureAssignedToTask = 0;
        removed = false;
    }
}