// CAIStateGroup_UsesSeat::OnReplace() -- release the seat's action point when one was taken, and when sitting get up (movement 8, or 9 with the alternate get-up name), removing the movement mode if the get-up fails
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009387F0, length 368 bytes (+ int3 padding).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PSeat +0x14), the inlined GetTC 0x2d (CTCUserActionPointBase,
//          bit 13 of +0x24) and 0x31 (CTCCreatureModeManager, bit 17 of +0x24), the action-point release @ 0x007FEF40,
//          CCharString copy ctor @ 0x0099EC30 (from the globals @ 0x013BB6E0 / 0x013BB6E8) / dtor @ 0x0099EAE0,
//          CBrainOutput ctor @ 0x00835620 (implicit destructor: intelligent pointer +0x40, then the counted pointer +0x20),
//          GetUpFromSitting @ 0x0085FF10 (__fastcall), CTCCreatureModeManager::RemoveMode @ 0x0073ABA0, operator delete
//          @ 0x00BFE9BC, ~CBaseIntelligentPointer @ 0x00A01C10.
// Shape: the seat release is guarded by the flag and a live seat (out-param GetTC, miss reads the key slot); the get-up
//          name is copied from one of two global strings; an unused CBrainOutput local (0x48 bytes) is constructed and
//          destroyed around the get-up call.
// Layout: no engine header for this class; modelled locally on the PDB CAIStateGroupBase (PBrain +0x04) with PSeat +0x14,
//          the seat flag +0x1c, sitting +0x1d, the action point index +0x20 and the alternate flag +0x30 (descriptive).
class CCharStringData;
class CCharString { public:
    CCharStringData* PStringData;
    CCharString(const CCharString& other);      // 0x0099EC30
    ~CCharString();                             // 0x0099EAE0
};
extern CCharString g_GetUpName;                 // 0x013BB6E0 (descriptive)
extern CCharString g_GetUpNameAlternate;        // 0x013BB6E8 (descriptive)
class CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad1[0x91 - 0x4c];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    template<class T> __forceinline void GetInterfaceV(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
        }
    }
};
struct CBaseIntelligentPointer {
    void* __vftable;
    CThing* m_pItem;                            // +0x04
    CThing* GetPItem() const;                   // 0x00A01B50
    ~CBaseIntelligentPointer();                 // 0x00A01C10
};
__forceinline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}
struct CTCUserActionPointBase {                 // interface 0x2d
    void FreeActionPoint(long index);           // 0x007FEF40
};
class CTCCreatureModeManager { public:                 // interface 0x31
    void RemoveMode(unsigned long mode);        // 0x0073ABA0
};
struct CCountBlock { long Count; void (__fastcall* Deleter)(void*); void* Object; };
void DeleteCountBlock(void* p);                // 0x00BFE9BC (operator delete)
struct CCountedPointer {                        // CCountedPointer<T>: object + count block, released inline
    void* Object;
    CCountBlock* Block;
    ~CCountedPointer()
    {
        if (Block) {
            --Block->Count;
            if (Block->Count == 0) {
                Block->Deleter(Block->Object);
                DeleteCountBlock(Block);
            }
        }
        Object = 0;
        Block = 0;
    }
};
class CBrainOutput { public:
    unsigned char _pad[0x20];
    CCountedPointer Counted;                    // +0x20
    unsigned char _pad28[0x40 - 0x28];
    CBaseIntelligentPointer PTarget;            // +0x40 (retail offset; the frame places it 0x40 into the 0x48-byte local)
    CBrainOutput();                             // 0x00835620
};
class CAIBrain { public:
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};
bool __fastcall GetUpFromSitting(CThing* creature, long movement, const CCharString* anim, CAIBrain* brain);   // 0x0085FF10

class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    unsigned char _pad[0x0c];
    CThing* GetCreature() const { return PBrain->PCreature; }
};
struct CAIStateGroup_UsesSeat : CAIStateGroupBase {
    CBaseIntelligentPointer PSeat;              // +0x14
    bool GotSeat;                               // +0x1c (descriptive)
    bool Sitting;                               // +0x1d (descriptive)
    unsigned char _pad1e[2];
    long ActionPointIndex;                      // +0x20 (descriptive)
    unsigned char _pad24[0x30 - 0x24];
    bool AlternateGetUp;                        // +0x30 (descriptive)
    virtual void OnReplace();
};

void CAIStateGroup_UsesSeat::OnReplace()
{
    if (GotSeat && IS_THING_ALIVE(PSeat)) {
        CTCUserActionPointBase* points;
        PSeat.GetPItem()->GetInterfaceV(&points, 0x2d);
        points->FreeActionPoint(ActionPointIndex);
        GotSeat = false;
    }
    ActionPointIndex = -1;
    if (Sitting) {
        long movement = (AlternateGetUp != 0) + 8;
        CCharString anim(AlternateGetUp ? g_GetUpNameAlternate : g_GetUpName);
        CBrainOutput output;
        if (!GetUpFromSitting(GetCreature(), movement, &anim, PBrain)) {
            CTCCreatureModeManager* modes;
            GetCreature()->GetInterfaceV(&modes, 0x31);
            modes->RemoveMode(movement);
        }
        Sitting = false;
    }
}