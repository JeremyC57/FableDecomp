// Behaviour test for retail 0x00687310: the candidate source below is embedded verbatim (from src/00687310.cpp),
// followed by dependency stubs/spies and a main that asserts on the observable effects.
// ===== candidate source =====
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
struct CThing;
struct CFaction;
struct C3DVector {                                      // the engine vector has a user-written member-wise copy
    float X, Y, Z;
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) {}
};
enum EMessageEventType { EMessageEventType_Force32 = 0x7fffffff };
enum EReactionSpeechType { EReactionSpeechType_Force32 = 0x7fffffff };
struct CBaseObject {
    virtual ~CBaseObject();
    CBaseObjectPointer* IntelligentPointer;             // +0x04
    CBaseObject(const CBaseObject& other);              // 0x0099A390
};
struct CBaseIntelligentPointer {
    virtual ~CBaseIntelligentPointer();
    CBaseObjectPointer* PData;                          // +0x04
    CBaseIntelligentPointer();                          // 0x00A01B10
    CBaseObject* GetPItem() const;                      // 0x00A01B50
    void SetPItem(CBaseObject* item);                   // 0x00A01B90
};
template<class T> struct CIntelligentPointer : CBaseIntelligentPointer {
    CIntelligentPointer(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); }
    virtual ~CIntelligentPointer();
};
struct CMessageEvent : CBaseObject {
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

// ===== harness =====
#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include <new>
static int g_base_copy, g_ip_ctor, g_get, g_set, g_msg_dtor, g_ip_dtor;
CBaseObject::CBaseObject(const CBaseObject&) : IntelligentPointer(0) { ++g_base_copy; }
CBaseObject::~CBaseObject() {}
CBaseIntelligentPointer::CBaseIntelligentPointer() : PData(0) { ++g_ip_ctor; }
CBaseIntelligentPointer::~CBaseIntelligentPointer() {}
CBaseObject* CBaseIntelligentPointer::GetPItem() const { ++g_get; return (CBaseObject*)PData; }   /* test registry: PData holds the item */
void CBaseIntelligentPointer::SetPItem(CBaseObject* item) { ++g_set; PData = (CBaseObjectPointer*)item; }
template<class T> CIntelligentPointer<T>::~CIntelligentPointer() { ++g_ip_dtor; }
CMessageEvent::~CMessageEvent() { ++g_msg_dtor; }
static unsigned char src_buf[sizeof(CMessageEvent)], dst_buf[sizeof(CMessageEvent)];
int main() {
    memset(src_buf, 0, sizeof src_buf); memset(dst_buf, 0xcd, sizeof dst_buf);
    CMessageEvent& s = *(CMessageEvent*)src_buf;
    s.IntelligentPointer = (CBaseObjectPointer*)0x77; s.Type = (EMessageEventType)4; s.Pos.X = 1.0f; s.Pos.Z = 3.0f; s.HearingRadius = 9.0f;
    s.OriginatorUID = 0x1122334455667788ULL; s.TargetUID = 0x99; s.TimeStamp = 1234; s.MessageID = 55; s.ReactionSpeechType = (EReactionSpeechType)2;
    s.POriginator.PData = (CBaseObjectPointer*)0x500; s.PTarget.PData = (CBaseObjectPointer*)0x600;
    CMessageEvent* d = new (dst_buf) CMessageEvent(s);
    int ok = sizeof(CMessageEvent) == 0x58 && offsetof(CMessageEvent, POriginator) == 0x48 && offsetof(CMessageEvent, PTarget) == 0x50;
    ok = ok && g_base_copy == 1 && g_ip_ctor == 2 && g_get == 2 && g_set == 2 && d->IntelligentPointer == 0;
    ok = ok && d->Type == 4 && d->Pos.X == 1.0f && d->Pos.Z == 3.0f && d->HearingRadius == 9.0f && d->OriginatorUID == 0x1122334455667788ULL;
    ok = ok && d->TargetUID == 0x99 && d->TimeStamp == 1234 && d->MessageID == 55 && d->ReactionSpeechType == 2;
    ok = ok && d->POriginator.PData == (CBaseObjectPointer*)0x500 && d->PTarget.PData == (CBaseObjectPointer*)0x600;
    /* the copy installed the real vtables: destroy through base pointers */
    CBaseIntelligentPointer* p = &d->PTarget; p->~CBaseIntelligentPointer();
    CBaseObject* b = d; b->~CBaseObject();
    ok = ok && g_ip_dtor == 3 && g_msg_dtor == 1;          /* PTarget once explicitly, then both members inside ~CMessageEvent */
    printf(ok ? "00687310_TEST PASS\n" : "00687310_TEST FAIL\n"); return ok ? 0 : 1;
}