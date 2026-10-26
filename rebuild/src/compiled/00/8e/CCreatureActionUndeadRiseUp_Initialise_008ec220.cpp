// CCreatureAction_UndeadRiseUp::Initialise() -- tells the creature (virtual slot 28) to start with flag 1, runs the base Initialise, then suspends the creature's animation (SetSuspendAnimation(true, none))
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008EC220, length 64 bytes.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PCreature at +8, twice), the creature's virtual slot 28 (`call [edx+70h]`, one argument),
// CCombatActionBase::Initialise @ 0x00857520 (non-virtual call), CThingAnimatable::SetSuspendAnimation(bool, long) @ 0x00833A10 (manifest leaf; first
// argument a byte, second pushed as 0).
// Shape: PCreature is addressed once (`lea edi,[esi+8]`) and reused for both GetPItem calls; the second result is a named local because retail
// evaluates it before pushing the two arguments (an inline object expression pushes first).
class CThingCreatureBase { public:                 // only virtual slot 28 and the animatable call are modelled
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15();
    virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27();
    virtual void SetUnderground(long flag); // slot 28
    void SetSuspendAnimation(bool suspend, long reason);   // 0x00833A10
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

class CCombatActionBase { public:                  // retail sizeof 0x114
    virtual void v00();
    virtual void Initialise();              // 0x00857520
    unsigned char _pad04[4];
    CIntelligentPointer<CThingCreatureBase> PCreature;      // +8
    unsigned char _pad10[0x114 - 0x10];
};

class CCreatureAction_UndeadRiseUp : public CCombatActionBase { public:
    virtual void Initialise();
};

void CCreatureAction_UndeadRiseUp::Initialise()
{
    PCreature.GetPItem()->SetUnderground(1);
    CCombatActionBase::Initialise();
    CThingCreatureBase* pCreature = PCreature.GetPItem();   // named: retail fetches it before the argument pushes
    pCreature->SetSuspendAnimation(true, 0);
}