// CAIStateGroupBehaviourBase::IsRunnable() -- reset target, check behavior predicate, and acquire perceived target.
// Grade: RELOCATION_MATCH -- explicit reset temporary lifetime preserves retail target-address ordering.
#include "engine/CAIStateGroupBehaviourBase.h"
#include "engine/CTCPerceiveThing.h"
class CThing;

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

template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {    // vtable 0x01237F1C
public:
    CIntelligentPointer(T* item=0) : CBaseIntelligentPointer() { SetPItem(item); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); return *this; }

    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};


struct CPerceptionMethods : CTCPerceiveThing { CThing* GetThing(); }; // 007446A0
struct CBehaviourVirtualView {
 virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
 virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
 virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
 virtual void v12(); virtual void v13(); virtual void v14(); virtual bool CanRun();
};
struct CThingRetailLife { unsigned char _beforeDead[0x91]; unsigned char Dead:1; };
// Typed view of the engine header's opaque pointer member; offsets remain unchanged.
struct CBehaviourMethods : CBehaviourVirtualView {
 unsigned char _base[0x10];
 CIntelligentPointer<const CThing> PTargetThing;
 CTCPerceiveThing* PTCPerception;
 inline CIntelligentPointer<const CThing>& Target() { return PTargetThing; }
 bool IsRunnable();
};
bool CBehaviourMethods::IsRunnable() {
 {
 CIntelligentPointer<const CThing> empty(0);
 const CThing* item=empty.GetPItem();
 Target().SetPItem(item);
 }
 if (Target().GetPItem() && !((CThingRetailLife*)Target().GetPItem())->Dead &&
     ((CBehaviourVirtualView*)this)->CanRun()) {
  Target()=((CPerceptionMethods*)PTCPerception)->GetThing();
  return true;
 }
 return false;
}