// CCombatAction_StrikeResponseJackOfBladesIntoRush::FinishAction() -- bump Jack's rush counter (interface 0xAD; wraps to 1
//   past the battle def's MaxRushCount), then, when CauseRush is set and a valid rush position exists, take the 2D distance
//   and the normalised direction to it, run the base strike-response FinishAction and make Jack cast an assassin rush along
//   that direction with a "RUSH_OUTOF" combat animation as its final action; otherwise just the base FinishAction.
// Leaf: FinishAction
// Retail: Fable.exe (TLC, VC7.1) @ 0x008C2550, 607 bytes (+ int3 padding), virtual, no EH frame.
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50 (PCreature +8), CVectorMap::LowerBound 0x0040F020 (same-TU stand-in),
//   CDefClassBase::GetSubDef<CJackOfBladesBattleDef> 0x008C33E0 (same-TU stand-in), GetValidRushPosition 0x008C1600 (same-TU
//   stand-in), CThing::GetPos 0x004C73D0, CCombatAction_GenericStrikeResponseBase::FinishAction 0x008663D0, operator new
//   0x00BFEA1A, CCharString 0x0099EBF0 / ~CCharString 0x0099EAE0, CCreatureAction_PlayCombatAnimation 0x008B7FF0,
//   CCountedPointer<T>(T*) 0x00695880 (same-TU stand-in; ICF-shared, catalog label names CFileInstaller::CFileInfo),
//   CCreatureAction_CastAssassinRush 0x008CE1B0 / ~ 0x0072D990, CThingCreatureBase::SetCurrentAction 0x006644F0, operator
//   delete 0x00BFE9BC, def release through vtable slot 1. Fast inverse-sqrt table 0x013CE558, 3.0f 0x0122DED4, 0.5f 0x0122F59C.
// Layout: local model -- the PDB-generated engine/CCombatAction_StrikeResponseJackOfBladesIntoRush.h is opaque padding of the
//   PDB size 0x164 while retail CauseRush sits at +0x148 (PDB +0x160; retail CCombatActionBase is 0x114, PCreature +8,
//   GenericStrikeResponseBase 0x148), so the class is modelled locally like the landed sibling 0x008C2810. CJackOfBladesBattleDef
//   MaxRushCount +0x40, CTCRushCounter RushCount +0x10.
// Shape: the same-TU stand-ins for every out-of-line callee that takes a local's address (LowerBound key, GetSubDef out,
//   rushPosition, finalAction) let VC7.1 pack the frame as retail does: the delta reuses the dead rushPosition words (dz
//   spilled to rushPosition.Z), counter / flag / finalAction.Data share +20h, finalAction.Info stays in esi across the rush
//   calls. GetSubDef goes through an inline CThing wrapper so the creature is fetched before &def is pushed; the incremented
//   count is a named local (without it esi/edi swap); Normalise(delta) before delta.Get2DLength() with `const float&` scaling
//   gives retail's x87 order (cached dx2/dy2, sqrt last). The CDefPointer null test is dropped by the GetSubDef stand-in.
extern unsigned long g_InvSqrtMantissaTable[128];   // 0x013CE558
__forceinline float FastInvSqrt(float lengthSquared)
{
    unsigned long bits = *(unsigned long*)&lengthSquared;
    unsigned long estimate = (((0x17c - ((bits >> 23) & 0xff)) >> 1) << 23) |
                             (g_InvSqrtMantissaTable[(bits >> 17) & 0x7f] << 15);
    float r = *(float*)&estimate;
    return (3.0f - r * r * lengthSquared) * r * 0.5f;
}
extern "C" double __cdecl sqrt(double);
#pragma intrinsic(sqrt)
class C3DVector { public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    __forceinline float Get2DLength() const { return (float)sqrt(X * X + Y * Y); }
};
__forceinline C3DVector operator*(const C3DVector& v, const float& s) { return C3DVector(v.X * s, v.Y * s, v.Z * s); }
__forceinline C3DVector Normalise(const C3DVector& v) { return v * FastInvSqrt(v.X * v.X + v.Y * v.Y + v.Z * v.Z); }
__forceinline C3DVector operator-(const C3DVector& a, const C3DVector& b) { return C3DVector(a.X - b.X, a.Y - b.Y, a.Z - b.Z); }

class CCharString { public:
    void* m_pData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};
void __cdecl operator delete(void* block);            // 0x00BFE9BC

class CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);              // 0x0040F020
};
#pragma auto_inline(off)
__declspec(noinline) CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
{
    SEntry* it = first;
    long count = last - first;
    while (count > 0) {
        long half = count / 2;
        if (it[half].type < *type) {
            it += half + 1;
            count -= half + 1;
        } else
            count = half;
    }
    return it;
}
#pragma auto_inline(on)

class CJackOfBladesBattleDef { public:
    virtual void v00();
    virtual void Destroy();                            // slot 1
    long RefCount;                                     // +0x04
    unsigned char _pad08[0x40 - 0x08];
    long MaxRushCount;                                 // +0x40
};
struct CDefPointer_CJackOfBladesBattleDef {          // CDefPointer<const CJackOfBladesBattleDef>
    CJackOfBladesBattleDef* Data;
    CDefPointer_CJackOfBladesBattleDef() : Data(0) { }
    ~CDefPointer_CJackOfBladesBattleDef()
    {
        if (Data && --Data->RefCount == 0)
            Data->Destroy();
    }
    CJackOfBladesBattleDef* operator->() const { return Data; }
};
class CDefClassBase { public:
    bool GetSubDef(CDefPointer_CJackOfBladesBattleDef* out) const;   // 0x008C33E0
};
void* FindSubDef_CJackOfBladesBattleDef(const CDefClassBase* def);
#pragma auto_inline(off)
__declspec(noinline) bool CDefClassBase::GetSubDef(CDefPointer_CJackOfBladesBattleDef* out) const
{
    void* found = FindSubDef_CJackOfBladesBattleDef(this);
    if (!found)
        return false;
    out->Data = (CJackOfBladesBattleDef*)found;
    return true;
}
#pragma auto_inline(on)

struct CTCRushCounter {                                // interface 0xad
    unsigned char _pad[0x10];
    long RushCount;                                    // +0x10
};

class CCreatureActionBase;
class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];                    // +0x20
    CInterfaceMap m_interfaces;                        // +0x44
    unsigned char _pad4c[0x70 - 0x4c];
    CDefClassBase* PDef;                               // +0x70
    const C3DVector& GetPos() const;              // 0x004C73D0
    __forceinline bool GetSubDef(CDefPointer_CJackOfBladesBattleDef* out) const { return PDef->GetSubDef(out); }
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *ppOut = (T*)(it == map->last || interfaceType < it->type ? map->last : it)->component;
            return true;
        }
        return false;
    }
};
class CThingCreatureBase : public CThing { public:
    bool SetCurrentAction(const CCreatureActionBase& action);   // 0x006644F0
};

struct CBaseIntelligentPointer {
    virtual void v00();
    CThingCreatureBase* m_pItem;
    CThingCreatureBase* GetPItem() const;              // 0x00A01B50
};

struct CCountedItem { };
class CCPPointerInfo { public:
    long m_count;
    void (CCountedItem::*m_pfnDestroy)();
    CCountedItem* m_pItem;
};
struct CCountedPointer_CCreatureActionBase {           // CCountedPointer<CCreatureActionBase>
    CCreatureActionBase* Data;
    CCPPointerInfo*      Info;
    CCountedPointer_CCreatureActionBase(CCreatureActionBase* item);   // 0x00695880
    CCountedPointer_CCreatureActionBase(const CCountedPointer_CCreatureActionBase& other) : Data(other.Data), Info(other.Info)
    {
        if (Info)
            ++Info->m_count;
    }
    ~CCountedPointer_CCreatureActionBase()
    {
        if (Info) {
            if (--Info->m_count == 0) {
                (Info->m_pItem->*Info->m_pfnDestroy)();
                operator delete(Info);
            }
        }
    }
};

class CCreatureActionBase { public:
    virtual void v00();
    unsigned char _pad04[4];
    CBaseIntelligentPointer PCreature;                 // +0x08
    unsigned char _pad10[0xa8 - 0x10];
    CThingCreatureBase* GetCreature() const { return PCreature.GetPItem(); }
};
class CCombatActionBase : public CCreatureActionBase { public:       // retail sizeof 0x114
    unsigned char _combat[0x114 - 0xa8];
};
class CCreatureAction_PlayCombatAnimation : public CCombatActionBase { public:
    unsigned char _pad114[0x118 - 0x114];
    CCreatureAction_PlayCombatAnimation(CThingCreatureBase* creature, const CCharString& animation, bool loop);   // 0x008B7FF0
};
class CCreatureAction_CastAssassinRush : public CCombatActionBase { public:   // retail sizeof 0x138
    unsigned char _pad114[0x138 - 0x114];
    CCreatureAction_CastAssassinRush(CThingCreatureBase* owner, const C3DVector& dir, float distance, bool useCollision, long level,
                                     CCountedPointer_CCreatureActionBase finalAction);   // 0x008CE1B0
    ~CCreatureAction_CastAssassinRush();               // 0x0072D990
};

class CCombatAction_GenericStrikeResponseBase : public CCombatActionBase { public:   // retail sizeof 0x148
    unsigned char _pad114[0x148 - 0x114];
    virtual void FinishAction();                       // 0x008663D0
};
class CCombatAction_StrikeResponseJackOfBladesIntoRush : public CCombatAction_GenericStrikeResponseBase { public:
    bool CauseRush;                                    // +0x148 (PDB +0x160)
    bool GetValidRushPosition(C3DVector& position);    // 0x008C1600
    virtual void FinishAction();
};

C3DVector FindRushPosition(CCombatAction_StrikeResponseJackOfBladesIntoRush* action);
bool IsRushPositionValid(CCombatAction_StrikeResponseJackOfBladesIntoRush* action);
CCPPointerInfo* NewPointerInfo(CCreatureActionBase* item);
#pragma auto_inline(off)
__declspec(noinline) bool CCombatAction_StrikeResponseJackOfBladesIntoRush::GetValidRushPosition(C3DVector& position)
{
    position = FindRushPosition(this);
    return IsRushPositionValid(this);
}
__declspec(noinline) CCountedPointer_CCreatureActionBase::CCountedPointer_CCreatureActionBase(CCreatureActionBase* item)
    : Data(item), Info(NewPointerInfo(item))
{
}
#pragma auto_inline(on)

void CCombatAction_StrikeResponseJackOfBladesIntoRush::FinishAction()
{
    CTCRushCounter* counter;
    PCreature.GetPItem()->GetInterface(&counter, 0xad);
    CDefPointer_CJackOfBladesBattleDef def;
    PCreature.GetPItem()->GetSubDef(&def);
    long count = counter->RushCount + 1;
    if (count > def->MaxRushCount)
        counter->RushCount = 1;
    else
        counter->RushCount = count;
    C3DVector rushPosition;
    if (CauseRush && GetValidRushPosition(rushPosition)) {
        C3DVector delta = rushPosition - PCreature.GetPItem()->GetPos();
        C3DVector direction = Normalise(delta);
        float distance = delta.Get2DLength();
        CThingCreatureBase* creature = PCreature.GetPItem();
        CCombatAction_GenericStrikeResponseBase::FinishAction();
        CCountedPointer_CCreatureActionBase finalAction(new CCreatureAction_PlayCombatAnimation(creature, "RUSH_OUTOF", true));
        GetCreature()->SetCurrentAction(CCreatureAction_CastAssassinRush(creature, direction, distance, false, 0, finalAction));
    } else
        CCombatAction_GenericStrikeResponseBase::FinishAction();
}