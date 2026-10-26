// Behaviour test for retail 0x0084bd40: the candidate source below is embedded verbatim (from src/0084bd40.cpp),
// followed by dependency stubs/spies and a main that asserts on the observable effects.
// ===== candidate source =====
// CCreatureAction_PerformExpression::CCreatureAction_PerformExpression(const CCreatureAction_PerformExpression&) -- member-wise
//   copy: base action copy, the counted expression-def pointer, the intelligent target pointer and the expression scalars
// Retail: Fable.exe (TLC, VC7.1) @ 0x0084BD40, 0xB4 bytes. Callees: CCreatureActionBase::CCreatureActionBase(const
//   CCreatureActionBase&) 0x0062EBC0 (manifest), CBaseIntelligentPointer ctor/GetPItem/SetPItem 0x00A01B10/0x00A01B50/
//   0x00A01B90. Vtable stores: 0x012743DC = CCreatureAction_PerformExpression, 0x01238C6C = CIntelligentPointer.
// Layout: PDB (Ego_r) names/types; the TLC CCreatureActionBase is 0xa8 bytes (PDB 0xb4), so each retail offset is the
//   PDB offset - 0x0c. CDefPointer's pointee keeps its count at +0x04. Local model: no engine/ header exists for these.
class CBaseObject;
struct CBaseObjectPointer;
class CThing;
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
class CDefPointeeBase { public: virtual void VFunc00(); long RefCount; };
template<class T> class CDefPointer { public:
    T* Object;
    CDefPointer(const CDefPointer& o) : Object(o.Object) { if (Object) ++((CDefPointeeBase*)Object)->RefCount; }
};
class CExpressionDef;
class CCreatureActionBase { public:
    virtual ~CCreatureActionBase();
    unsigned char _body[0xa8 - 4];
    CCreatureActionBase(const CCreatureActionBase& other);   // 0x0062EBC0
};
class CCreatureAction_PerformExpression : public CCreatureActionBase { public:
    CDefPointer<const CExpressionDef> PExpressionDef;   // +0xa8
    CIntelligentPointer<CThing> PPerformToThing;        // +0xac
    long ExpressionItemDefIndex;                        // +0xb4
    bool EmoteIconAdded;                                // +0xb8
    bool ReplacedAction;                                // +0xb9
    bool Extended;                                      // +0xba
    bool ButtonHeld;                                    // +0xbb
    unsigned long SoundEffectID;                        // +0xbc
    long ExtraData1;                                    // +0xc0
    long ExtraData2;                                    // +0xc4
    virtual ~CCreatureAction_PerformExpression();
    CCreatureAction_PerformExpression(const CCreatureAction_PerformExpression& other);
};
CCreatureAction_PerformExpression::CCreatureAction_PerformExpression(const CCreatureAction_PerformExpression& o)
    : CCreatureActionBase(o), PExpressionDef(o.PExpressionDef), PPerformToThing(o.PPerformToThing),
      ExpressionItemDefIndex(o.ExpressionItemDefIndex), EmoteIconAdded(o.EmoteIconAdded), ReplacedAction(o.ReplacedAction),
      Extended(o.Extended), ButtonHeld(o.ButtonHeld), SoundEffectID(o.SoundEffectID), ExtraData1(o.ExtraData1),
      ExtraData2(o.ExtraData2)
{
}

// ===== harness =====
#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include <new>
static int g_base_copy, g_ip_ctor, g_get, g_set, g_ip_dtor, g_act_dtor;
CBaseIntelligentPointer::CBaseIntelligentPointer() : PData(0) { ++g_ip_ctor; }
CBaseIntelligentPointer::~CBaseIntelligentPointer() {}
CBaseObject* CBaseIntelligentPointer::GetPItem() const { ++g_get; return (CBaseObject*)PData; }   /* test registry: PData holds the item */
void CBaseIntelligentPointer::SetPItem(CBaseObject* item) { ++g_set; PData = (CBaseObjectPointer*)item; }
template<class T> CIntelligentPointer<T>::~CIntelligentPointer() { ++g_ip_dtor; }
void CDefPointeeBase::VFunc00() {}
CCreatureActionBase::CCreatureActionBase(const CCreatureActionBase& o) { ++g_base_copy; memcpy(_body, o._body, sizeof _body); }
CCreatureActionBase::~CCreatureActionBase() {}
CCreatureAction_PerformExpression::~CCreatureAction_PerformExpression() { ++g_act_dtor; }
class CExpressionDef : public CDefPointeeBase { public:};
static unsigned char src_buf[sizeof(CCreatureAction_PerformExpression)], dst_buf[sizeof(CCreatureAction_PerformExpression)];
int main() {
    CExpressionDef def; def.RefCount = 2;
    memset(src_buf, 0, sizeof src_buf); memset(dst_buf, 0xcd, sizeof dst_buf);
    CCreatureAction_PerformExpression& s = *(CCreatureAction_PerformExpression*)src_buf;
    s._body[0x10] = 0x42; s.PExpressionDef.Object = &def; s.PPerformToThing.PData = (CBaseObjectPointer*)0x700;
    s.ExpressionItemDefIndex = 17; s.EmoteIconAdded = true; s.ButtonHeld = true; s.SoundEffectID = 0xabc; s.ExtraData2 = -3;
    CCreatureAction_PerformExpression* d = new (dst_buf) CCreatureAction_PerformExpression(s);
    int ok = sizeof(CCreatureAction_PerformExpression) == 0xc8 && offsetof(CCreatureAction_PerformExpression, PPerformToThing) == 0xac
          && offsetof(CCreatureAction_PerformExpression, ExtraData2) == 0xc4;
    ok = ok && g_base_copy == 1 && d->_body[0x10] == 0x42 && d->PExpressionDef.Object == &def && def.RefCount == 3;
    ok = ok && g_ip_ctor == 1 && g_get == 1 && g_set == 1 && d->PPerformToThing.PData == (CBaseObjectPointer*)0x700;
    ok = ok && d->ExpressionItemDefIndex == 17 && d->EmoteIconAdded && !d->ReplacedAction && !d->Extended && d->ButtonHeld;
    ok = ok && d->SoundEffectID == 0xabc && d->ExtraData1 == 0 && d->ExtraData2 == -3;
    CCreatureActionBase* b = d; b->~CCreatureActionBase();      /* real vtables installed */
    ok = ok && g_act_dtor == 1 && g_ip_dtor == 1;
    s.PExpressionDef.Object = 0; new (dst_buf) CCreatureAction_PerformExpression(s);   /* null def: no count */
    ok = ok && def.RefCount == 3 && ((CCreatureAction_PerformExpression*)dst_buf)->PExpressionDef.Object == 0;
    printf(ok ? "0084bd40_TEST PASS\n" : "0084bd40_TEST FAIL\n"); return ok ? 0 : 1;
}