// CTCPerceiveThing::IsPlayerIgnoreFOVLineOfSight() -- the perceived thing exists, is a player (type byte 2 at +0x10) and is in line of sight ignoring the field of view
// Retail: Fable.exe (TLC, VC7.1) @ 0x007448B0, 51 bytes (catalogued as IsPlayerExtendedLineOfSight: the landed twins 0x00744830 /
//   0x00744870 test LineOfSight +0x2d / ExtendedLineOfSight +0x2e, this one tests IgnoreFOVLineOfSight +0x2f).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (twice, on PerceivedThing +0x0c).
// Layout: engine/CTCPerceiveThing.h (PerceivedThing +0x0c, IgnoreFOVLineOfSight +0x2f); CThing +0x10 = the thing type byte.
// Shape: nested ifs with a long 0/1 result; the pointer object's address kept in esi across both GetPItem calls.
// Leaf: IsPlayerIgnoreFOVLineOfSight
#include "engine/CTCPerceiveThing.h"

class CThing { public:
    unsigned char _pad00[0x10];
    unsigned char Type;                     // +0x10 (2 = player)
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

struct CTCPerceiveThing_Methods : CTCPerceiveThing {
    long IsPlayerIgnoreFOVLineOfSight() const;
};

long CTCPerceiveThing_Methods::IsPlayerIgnoreFOVLineOfSight() const
{
    const CIntelligentPointer<CThing>* perceived = reinterpret_cast<const CIntelligentPointer<CThing>*>(PerceivedThing);
    if (perceived->GetPItem()) {
        if (perceived->GetPItem()->Type == 2) {
            if (IgnoreFOVLineOfSight)
                return 1;
        }
    }
    return 0;
}