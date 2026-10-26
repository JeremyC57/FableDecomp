// CCreatureActionBase::SetMoveToPos(const C3DVector& pos) -- record the next movement position and
// the world frame it belongs to
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006929E0, length 54 bytes (+ int3 padding).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (on PCreature at +0x08),
// a no-argument member @ 0x004C7AB0 on the creature whose result is not used (it reads a global's
// +0x24; the manifest "CViewBase::GetDisplayEngine" label is propagated), and the world-frame
// getter @ 0x0049D870 (returns the global @ 0x013B89BC; its "GFGetPreMainMemoryUsed" label is
// propagated) stored into NextMovementPosWorldFrame at +0x64. The position is then copied into
// NextMovementPos at +0x68 as three dwords, the member address formed in place (`add esi,68h`).
// engine/CCreatureActionBase.h (PDB) agrees: PCreature +0x08, NextMovementPosWorldFrame +0x64,
// NextMovementPos +0x68; a local model is used because the method takes a stack argument.
class C3DVector { public:
    float X;
    float Y;
    float Z;
};

class CThingCreatureBase_Calls { public:
    void* Call_004c7ab0();                     // 0x004C7AB0 (result unused)
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
    T* GetPItem() const { return reinterpret_cast<T*>(CBaseIntelligentPointer::GetPItem()); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem(reinterpret_cast<CBaseObject*>(item)); }
};

long GetWorldFrame_0049d870();                 // 0x0049D870 (returns the global @ 0x013B89BC)

class CCreatureActionBase { public:
    virtual void v00();
    unsigned char _pad04[4];                   // +0x04
    CIntelligentPointer<CThingCreatureBase_Calls> PCreature;   // +0x08
    unsigned char _pad10[0x54];                // +0x10..+0x64
    long      NextMovementPosWorldFrame;       // +0x64
    C3DVector NextMovementPos;                 // +0x68
    void SetMoveToPos(const C3DVector& pos);
};

void CCreatureActionBase::SetMoveToPos(const C3DVector& pos)
{
    PCreature.GetPItem()->Call_004c7ab0();
    NextMovementPosWorldFrame = GetWorldFrame_0049d870();
    NextMovementPos = pos;
}