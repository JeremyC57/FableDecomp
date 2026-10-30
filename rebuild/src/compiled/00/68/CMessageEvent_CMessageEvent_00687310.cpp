// Grade: RELOCATION_MATCH (Fable landing 2026-10-19 after the Codex review hold: test exercises a hand-written stand-in, not the candidate)
// CMessageEvent::CMessageEvent(const CMessageEvent&) -- member-wise copy: CBaseObject copy, the message fields, then the
//   two counted CIntelligentPointer<CThing> members re-registered on the same items
// Retail: Fable.exe (TLC, VC7.1) @ 0x00687310, 189 bytes.
// Callees: CBaseObject::CBaseObject(const CBaseObject&) @ 0x0099A390 (stores vtable 0x0129A7D8 = CBaseObject, clears the
//   object's pointer record, ret 4; the manifest label "CBaseIntelligentPointer" is propagated),
//   CBaseIntelligentPointer::CBaseIntelligentPointer() 0x00A01B10, GetPItem() const 0x00A01B50, SetPItem(CBaseObject*)
//   0x00A01B90. Vtable stores: 0x0125BE8C = CMessageEvent, 0x01238C6C = CIntelligentPointer (rtti-vtables.tsv).
// Layout: PDB (Ego_r) CMessageEvent (0x58) names/types; retail offsets equal the PDB. Local model: no engine/ header
//   exists for CMessageEvent / CBaseObject / CBaseIntelligentPointer.
struct CBaseObjectPointer;
class CThing;
class CFaction;
class C3DVector { public:                                      // the engine vector has a user-written member-wise copy
    float X, Y, Z;
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) {}
};
enum EMessageEventType { EMessageEventType_Force32 = 0x7fffffff };
enum EReactionSpeechType { EReactionSpeechType_Force32 = 0x7fffffff };
#define FABLE_RETAIL_BASE_OBJECT
#include "engine/CBaseObject.h"                       // : CBaseClass, vtable 0x0129A7D8 (~ slot 0, Validate slot 1)
struct CBaseIntelligentPointer {
    virtual ~CBaseIntelligentPointer();
    CBaseObjectPointer* PData;                          // +0x04
    CBaseIntelligentPointer();                          // 0x00A01B10
    CBaseObject* GetPItem() const;                      // 0x00A01B50
    void SetPItem(CBaseObject* item);                   // 0x00A01B90
};
template<class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:
    CIntelligentPointer(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); }
    virtual ~CIntelligentPointer();
};
class CMessageEvent : public CBaseObject { public:
    EMessageEventType Type;                             // +0x08
    C3DVector Pos;                                      // +0x0c
    float SightRadius;                                  // +0x18
    float HearingRadius;                                // +0x1c
    unsigned __int64 OriginatorUID;                     // +0x20
    unsigned __int64 TargetUID;                         // +0x28
    CFaction* PTargetFaction;                           // +0x30
    long TimeStamp;                                     // +0x34
    unsigned long Lifespan;                             // +0x38
    long ExtraData;                                     // +0x3c
    unsigned long MessageID;                            // +0x40
    EReactionSpeechType ReactionSpeechType;             // +0x44
    CIntelligentPointer<CThing> POriginator;            // +0x48
    CIntelligentPointer<CThing> PTarget;                // +0x50
    CMessageEvent(const CMessageEvent& other);
    virtual ~CMessageEvent();
};
CMessageEvent::CMessageEvent(const CMessageEvent& o)
    : CBaseObject(o), Type(o.Type), Pos(o.Pos), SightRadius(o.SightRadius), HearingRadius(o.HearingRadius),
      OriginatorUID(o.OriginatorUID), TargetUID(o.TargetUID), PTargetFaction(o.PTargetFaction), TimeStamp(o.TimeStamp),
      Lifespan(o.Lifespan), ExtraData(o.ExtraData), MessageID(o.MessageID), ReactionSpeechType(o.ReactionSpeechType),
      POriginator(o.POriginator), PTarget(o.PTarget)
{
}