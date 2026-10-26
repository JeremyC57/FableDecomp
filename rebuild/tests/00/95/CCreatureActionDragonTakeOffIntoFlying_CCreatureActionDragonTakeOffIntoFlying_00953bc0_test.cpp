// Behaviour test for retail 0x00953bc0: the candidate source below is embedded verbatim (from src/00953bc0.cpp),
// followed by dependency stubs/spies and a main that asserts on the observable effects.
// ===== candidate source =====
// CCreatureAction_DragonTakeOffIntoFlying::CCreatureAction_DragonTakeOffIntoFlying(const CCreatureAction_DragonTakeOffIntoFlying&)
//   -- member-wise copy: combat-action base, the three points, the movement-point vector, the flag and the counted end action
// Retail: Fable.exe (TLC, VC7.1) @ 0x00953BC0, 0xC1 bytes. Callees: CCombatActionBase::CCombatActionBase(const CCombatActionBase&)
//   0x0062F3B0 (manifest), the STLport vector<C3DVector> copy ctor 0x006F0810 (label "Vector_CopyFrom"); vtable
//   0x01293A7C = CCreatureAction_DragonTakeOffIntoFlying.
// Layout: PDB (Ego_r) names/types; the TLC CCombatActionBase is 0x114 bytes (PDB 0x12c), so retail offset = PDB - 0x18:
//   Dest +0x114, StartPos +0x120, PreviousPoint +0x12c, MovementPoints +0x138 (12-byte STLport vector), UsePreviousPoint
//   +0x144, PEndAction +0x148 (CCountedPointer: Data, Info; the shared CCPPointerInfo keeps RefCount at +0).
//   Local model: no engine/ header exists for these classes.
class CCreatureActionBase;
class CCombatActionBase { public:
    virtual ~CCombatActionBase();
    unsigned char _body[0x110];
    CCombatActionBase(const CCombatActionBase& other);  // 0x0062F3B0
};
class C3DVector { public:                                      // the engine vector has a user-written member-wise copy
    float X, Y, Z;
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) {}
};
template<class T> struct CRetailVector {                // STLport vector: begin/end/capacity
    T* m_first; T* m_last; T* m_end;
    CRetailVector(const CRetailVector& other);          // C3DVector: 0x006F0810
};
class CCPPointerInfo { public: unsigned long RefCount; void (__cdecl* DeleteFunc)(void*); void* Data; };
template<class T> class CCountedPointer { public:
    T* Data;
    CCPPointerInfo* Info;
    CCountedPointer(const CCountedPointer& o) : Data(o.Data), Info(o.Info) { if (Info) ++Info->RefCount; }
};
class CCreatureAction_DragonTakeOffIntoFlying : public CCombatActionBase { public:
    C3DVector Dest;                                     // +0x114
    C3DVector StartPos;                                 // +0x120
    C3DVector PreviousPoint;                            // +0x12c
    CRetailVector<C3DVector> MovementPoints;            // +0x138
    bool UsePreviousPoint;                              // +0x144
    CCountedPointer<CCreatureActionBase> PEndAction;    // +0x148
    virtual ~CCreatureAction_DragonTakeOffIntoFlying();
    CCreatureAction_DragonTakeOffIntoFlying(const CCreatureAction_DragonTakeOffIntoFlying& other);
};
CCreatureAction_DragonTakeOffIntoFlying::CCreatureAction_DragonTakeOffIntoFlying(const CCreatureAction_DragonTakeOffIntoFlying& o)
    : CCombatActionBase(o), Dest(o.Dest), StartPos(o.StartPos), PreviousPoint(o.PreviousPoint),
      MovementPoints(o.MovementPoints), UsePreviousPoint(o.UsePreviousPoint), PEndAction(o.PEndAction)
{
}

// ===== harness =====
#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include <new>
static int g_base, g_vec, g_dtor;
CCombatActionBase::CCombatActionBase(const CCombatActionBase& o) { ++g_base; memcpy(_body, o._body, sizeof _body); }
CCombatActionBase::~CCombatActionBase() {}
template<class T> CRetailVector<T>::CRetailVector(const CRetailVector<T>& o) : m_first(o.m_first), m_last(o.m_last), m_end(o.m_end) { ++g_vec; }
template struct CRetailVector<C3DVector>;
CCreatureAction_DragonTakeOffIntoFlying::~CCreatureAction_DragonTakeOffIntoFlying() { ++g_dtor; }
typedef CCreatureAction_DragonTakeOffIntoFlying D;
static unsigned char s_buf[sizeof(D)], d_buf[sizeof(D)];
int main() {
    memset(s_buf, 0, sizeof s_buf); memset(d_buf, 0xcd, sizeof d_buf);
    D& s = *(D*)s_buf; CCPPointerInfo info = { 4, 0, 0 };
    s._body[0x20] = 0x5a; s.Dest.X = 1.0f; s.Dest.Z = 3.0f; s.StartPos.Y = 5.0f; s.PreviousPoint.Z = 9.0f;
    s.MovementPoints.m_last = (C3DVector*)0x40; s.UsePreviousPoint = true;
    s.PEndAction.Data = (CCreatureActionBase*)0x1234; s.PEndAction.Info = &info;
    D* d = new (d_buf) D(s);
    int ok = sizeof(D) == 0x150 && offsetof(D, Dest) == 0x114 && offsetof(D, MovementPoints) == 0x138 && offsetof(D, PEndAction) == 0x148;
    ok = ok && g_base == 1 && g_vec == 1 && d->_body[0x20] == 0x5a && d->Dest.X == 1.0f && d->Dest.Z == 3.0f && d->StartPos.Y == 5.0f;
    ok = ok && d->PreviousPoint.Z == 9.0f && d->MovementPoints.m_last == (C3DVector*)0x40 && d->UsePreviousPoint;
    ok = ok && d->PEndAction.Data == (CCreatureActionBase*)0x1234 && d->PEndAction.Info == &info && info.RefCount == 5;
    CCombatActionBase* b = d; b->~CCombatActionBase(); ok = ok && g_dtor == 1;   /* derived vtable installed */
    s.PEndAction.Info = 0; new (d_buf) D(s); ok = ok && info.RefCount == 5 && ((D*)d_buf)->PEndAction.Info == 0;
    printf(ok ? "00953bc0_TEST PASS\n" : "00953bc0_TEST FAIL\n"); return ok ? 0 : 1;
}