// CCombatAction_GenericStrikeResponseBase::IsOverrideableByActionOfSamePriority() const -- false only while the attacker is alive and has flag 0x40000 set; true otherwise
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00865E70, length 58 bytes (+ int3 padding).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 three times on PAttacker (+0x114, the pointer the
// class constructor @ 0x00867950 builds); the item is re-fetched for each use (a call, nothing is CSE'd).
// Reads: the thing's dead bit (+0x91 & 1, the IS_THING_DEAD test of the landed 0x00836BE0) and its flags at
// +0x30 (bit 0x40000). Shape: nested ifs returning false, shared `mov al,1` true tail; the pointer address
// is formed once into esi. No engine header exists for this class.
class CThing { public:
    unsigned char _pad[0x30];
    unsigned long Flags;                        // +0x30
    unsigned char _pad34[0x91 - 0x34];          // +0x34
    unsigned char DeadFlags;                    // +0x91 (bit 0: dead)
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

class CCombatAction_GenericStrikeResponseBase { public:
    virtual void v00();
    unsigned char _body[0x114 - 4];
    CIntelligentPointer<CThing> PAttacker;          // +0x114
    bool IsOverrideableByActionOfSamePriority() const;
};

bool CCombatAction_GenericStrikeResponseBase::IsOverrideableByActionOfSamePriority() const
{
    if (PAttacker.GetPItem() != 0) {
        if (!(PAttacker.GetPItem()->DeadFlags & 1)) {
            if (PAttacker.GetPItem()->Flags & 0x40000)
                return false;
        }
    }
    return true;
}