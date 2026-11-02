// CTCNoise::~CTCNoise() -- implicit destructor: Creator, then the embedded Definition (CTCNoiseDef: the inline def-chain destructors down to ~CDefPointeeBase's vtable reset and ~CResource), then ~CTCBase
// Leaf: ??1CTCNoise@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00749A70, 33 bytes, thiscall; `call ~CResource` 0x009FC550 on +0x0C, tail `jmp ~CTCBase`
//   0x00686830.
// Identity (proven, two sources): the only caller is the scalar deleting destructor 0x00749A50, slot 0 of vtable 0x01268A74 (RTTI
//   .?AVCTCNoise@@ : CTCBase : CBaseClassNonCopyable : CBaseClass; the vtable is stored by the constructor at 0x00749825); FableWin.pdb
//   ??1CTCNoise@@UAE@XZ (0x02598B40, right after ??_G 0x02598B00; Ego_r inlines it). It stores no CTCNoise vtable, so it is the
//   compiler-generated destructor. The vtable it does store, 0x01230BA0, is CDefPointeeBase's (rtti-vtables; engine/CDefPointeeBase.h)
//   at +0x0C: Ego_r.pdb's CTCNoise members are Definition (CTCNoiseDef, by value) +0x0C, LifeTimer +0x40, DistanceCanBeHeardFrom +0x44,
//   Creator (CIntelligentPointer<const CThing>) +0x48, sizeof 0x50 (no STL container, so the offsets are retail's).
//   The catalog label "CThingNoiseDef::~CThingNoiseDef" is propagated and wrong (CThingNoiseDef is not a retail class).
// Callees: ??1CBaseIntelligentPointer@@UAE@XZ 0x00A01C10, ??_7CDefPointeeBase@@6B@ 0x01230BA0, ??1CResource@@UAE@XZ 0x009FC550,
//   ??1CTCBase@@UAE@XZ 0x00686830 (tail).
// Shape: members in reverse order. CTCNoiseDef's implicit dtor and the inline ~CSubDefClassBase / ~CDefClassBase / ~CDefPointeeBase
//   expand in place; their vtable stores are dead behind the last one (0x01230BA0), which retail keeps, then ~CResource is a call
//   (the def is a member, not the most-derived base, so it cannot be the tail). Retail's own constructor
//   (??0CTCNoise@@QAE@AAVCThing@@@Z; body not reconstructed here) emits the vtable, hence ??_G and this out-of-line ??1.
// Layout: engine/CTCBase.h and the def chain engine/CSubDefClassBase.h / CDefClassBase.h / CDefPointeeBase.h / CResource.h, all
//   retail opt-ins; CTCNoise / CTCNoiseDef / CIntelligentPointer have no shared header (local models).
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"                                 // CTCBase : CBaseClassNonCopyable (vtable 0x0125BE0C)
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CSubDefClassBase.h"                        // : CDefClassBase : CDefPointeeBase (vtable 0x01230BA0) : CResource

class CThing;
class CBaseObject;
class CBaseObjectPointer;

class CBaseIntelligentPointer {                             // vtable 0x0129C95C (vptr + PData, 8 bytes; 4-aligned members here, no pack needed)
public:
    CBaseIntelligentPointer();                              // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                     // 0x00A01C10 ??1CBaseIntelligentPointer@@UAE@XZ
    CBaseObject* GetPItem() const;                          // 0x00A01B50
private:
    CBaseObjectPointer* PData;                              // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
};

enum ENoiseType;                                            // MS forward enum (Ego_r CTCNoiseDef::Type)

class CTCNoiseDef : public CSubDefClassBase {               // RTTI CTCNoiseDef : CSubDefClassBase; Ego_r 0x34 bytes
public:
    CTCNoiseDef();                                          // ??0CTCNoiseDef@@QAE@XZ
    virtual void Transfer(CPersistContext& context);        // slot 18
    virtual void Copy(const CDefClassBase* other);          // slot 19
    virtual unsigned long GetSizeofClass();                 // slot 20
    ENoiseType Type;                                        // +0x28
    float      LifeTime;                                    // +0x2C
    float      DistanceCanBeHeardFrom;                      // +0x30
};
FABLE_STATIC_ASSERT(sizeof(CTCNoiseDef) == 0x34);

enum ETCInterfaceType;

class CTCNoise : public CTCBase {                           // vtable 0x01268A74
public:
    CTCNoise(CThing& thing);                                // ??0CTCNoise@@QAE@AAVCThing@@@Z
    virtual ETCInterfaceType GetInterfaceType() const;
    virtual CCharString GetClassTypeName() const;
    CTCNoiseDef                       Definition;           // +0x0C
    long                              LifeTimer;            // +0x40
    float                             DistanceCanBeHeardFrom;   // +0x44
    CIntelligentPointer<const CThing> Creator;              // +0x48
};
FABLE_STATIC_ASSERT(sizeof(CTCNoise) == 0x50);

CTCNoise::CTCNoise(CThing& thing)
    : CTCBase(thing), LifeTimer(0), DistanceCanBeHeardFrom(0)     // retail 0x00749847 / 0x0074984A store 0 to both
{
}