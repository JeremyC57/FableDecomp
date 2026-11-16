// CTCSoundPlayer::CTCSoundPlayer(CThing* parent) -- thing-component base, null attached-emitter pointer, two empty strings, zero clip distance and material index
// Leaf: ??0CTCSoundPlayer@@QAE@AAVCThing@@@Z
// Ported 2026-11-16 onto the shared engine/CTCBase.h: the local base model CTCBase (which bound ??0CTCBase@@QAE@PAVCThing@@@Z to 0x00686800) is gone;
//   the base call is ??0CTCBase@@QAE@AAVCThing@@@Z and this constructor takes CThing& as the PDBs spell it (??0CTCSoundPlayer@@QAE@AAVCThing@@@Z).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0066AC80, length 81 bytes.
// Callees: CTCBase::CTCBase(CThing&) @ 0x00686800, CBaseIntelligentPointer::CBaseIntelligentPointer() @ 0x00A01B10,
// CBaseIntelligentPointer::SetPItem @ 0x00A01B90, CCharString::CCharString() @ 0x0099E4B0 (twice).
// Vtables: 0x0125B1C4 = CTCSoundPlayer, 0x0125B1B8 = CIntelligentPointer (rtti-vtables.tsv).
// Layout: Ego_r.pdb CTCSoundPlayer (0x24 bytes): PAttachedEmitter (CIntelligentPointer<CThingSoundEmitter>) +0x0c,
// VoiceName +0x14, ExtraTag +0x18, AnimCriteriaClipDistance (short) +0x1c, LastMaterialIndex +0x20.
// Shape: the derived vtable store comes before the member constructors; the intelligent pointer is built by an
// inline converting constructor from a null pointer (base constructor, own vtable, SetPItem(0)).
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"                     // CTCBase(CThing&) 0x00686800 ??0CTCBase@@QAE@AAVCThing@@@Z
class CThing;
class CBaseObject;
class CThingSoundEmitter;

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

template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {      // CIntelligentPointer<CThingSoundEmitter>, vtable 0x0125B1B8
public:
    CIntelligentPointer(T* item) : CBaseIntelligentPointer() { SetPItem((CBaseObject*)item); }

    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
};

class CCharString { public:
    void* m_pData;
    CCharString();                                          // 0x0099E4B0
};

class CTCSoundPlayer : public CTCBase { public:
    CIntelligentPointer<CThingSoundEmitter> PAttachedEmitter;                   // +0x0c
    CCharString         VoiceName;                          // +0x14
    CCharString         ExtraTag;                           // +0x18
    short               AnimCriteriaClipDistance;           // +0x1c
    long                LastMaterialIndex;                  // +0x20
    CTCSoundPlayer(CThing& parent);
};

CTCSoundPlayer::CTCSoundPlayer(CThing& parent)
    : CTCBase(parent)
    , PAttachedEmitter(0)
    , AnimCriteriaClipDistance(0)
    , LastMaterialIndex(0)
{
}