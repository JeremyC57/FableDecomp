// Grade: DIFFER(693v701) -- frame slot order (retail homes the lookup keys at +0x18/+0x1c, ours at +0x14/+0x20) and the fallback operator= sequence (retail evaluates the fallback string id after the temp's address); found on the way: the get-up name is copy-constructed from a by-value table string (derived CDefString return stops VC eliding the copy), the const bed lookup is an out-parameter GetInterface on a const thing with the pointer zeroed first; 4 grades spent
// CAIStateGroup_MinionSleep::OnReplace() -- reset the creature's sleep component and stamp the frame; when the creature is in sleep mode get it up from its bed (the bed def's get-up animation, else its fallback), release its bed slot and, if still asleep, drop the sleep mode and reset the bed; then the go-to-bed base OnReplace and clear the in-bed flag
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0092CDA0, length 704 bytes (+ int3 padding), plain `ret`, frame 0x14.
// Callees: CVectorMap::LowerBound @ 0x0040F020 / its const twin @ 0x004365B0 (0xD4 sleep component, 0x31 mode manager, 0x35 bed),
// CAIStateGroupBase::GetPThingWorld @ 0x008FCFA0, GFGetCurrentFrame @ 0x0049D870, mode manager GetCurrentMode @ 0x0073A6E0 (mode
// virtual slot 1) / RemoveMode @ 0x0073ABA0, CBaseIntelligentPointer::GetPItem @ 0x00A01B50, CDefStringTable::GetString @ 0x009D49B0
// (the global table @ 0x013CA828), CCharString copy ctor @ 0x0099EC30 / operator= @ 0x0099EFB0 / dtor @ 0x0099EAE0,
// CBasicString<char>::operator== @ 0x004115A0, GetUpFromSleeping @ 0x0085F6F0 (__fastcall), the bed's slot release @ 0x00746A70 and
// reset @ 0x00746810, CAIStateGroup_GoToBed::OnReplace @ 0x0092C740.
// Layout: local model (PBrain +4; GoToBed PBed +0x14 per Ego_r.pdb; in-bed flag +0x34, bed slot +0x38; bed def pointer +0x14 of
// the bed component, def string ids +0x30 / +0x34).
// Shape: the empty-name test is the inline CCharString::operator== whose null arm is strcmp(text, "") (1-byte repe cmpsb).
#include <string.h>
long __fastcall GFGetCurrentFrame();             // 0x0049D870
struct CBasicStringChar {
    unsigned char _body[0xd];
    bool operator==(const char* text) const;    // 0x004115A0
};
struct CCharStringData {
    CBasicStringChar Data;
    long NoRefs;
};
class CCharString {
public:
    CCharStringData* PStringData;
    CCharString(const CCharString& other);      // 0x0099EC30
    ~CCharString();                             // 0x0099EAE0
    CCharString& operator=(const CCharString& other);   // 0x0099EFB0
    bool operator==(const char* text) const
    {
        if (PStringData == 0) {
            if (strcmp(text, "") == 0)
                return true;
            return false;
        }
        return PStringData->Data == text;
    }
};
struct CDefString : CCharString {               // the table's by-value string (a CCharString)
    CDefString(const CDefString& other) : CCharString(other) { }
};
struct CDefStringTable {
    CDefString GetString(long id) const;        // 0x009D49B0
};
extern CDefStringTable g_DefStringTable;        // 0x013CA828
struct CDefVirtual {
    virtual ~CDefVirtual();
    virtual void Destroy();                     // slot 1
};
struct CRefCounted { long m_count; };
struct CBedDef : CDefVirtual, CRefCounted {
    unsigned char _pad08[0x28];
    long FallbackGetUpAnim;                     // +0x30
    long GetUpAnim;                             // +0x34
};
struct CDefPointer {
    CBedDef* m_p;
    CDefPointer(CBedDef* p) : m_p(p) { if (m_p) ++m_p->m_count; }
    ~CDefPointer() { if (--m_p->m_count == 0) m_p->Destroy(); }
    CBedDef* operator->() const { return m_p; }
};
struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);                   // 0x0040F020
    const SEntry* LowerBound(const long* type) const;       // 0x004365B0
};
struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad4c[0x45];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1u << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *ppOut = (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
            return true;
        }
        return false;
    }
    template<class T> __forceinline bool GetInterface(const T** ppOut, long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1u << (interfaceType & 31))) {
            long type = interfaceType;
            const CInterfaceMap* map = &m_interfaces;
            const CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *ppOut = (const T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
            return true;
        }
        return false;
    }
};
struct CThingCreatureBase : CThing { };
struct CAIBrain {
    unsigned char _pad[0x18];
    CThingCreatureBase* Creature;               // +0x18
};
bool __fastcall GetUpFromSleeping(CThingCreatureBase* creature, CThing* bed, const CCharString& anim, CAIBrain* brain);   // 0x0085F6F0
struct CTCSleep {                               // interface 0xD4
    unsigned char _pad[0xc];
    bool Asleep;                                // +0x0c
    bool Dreaming;                              // +0x0d
    long FrameStateChanged;                     // +0x10
};
struct CTCBed {                                 // interface 0x35
    unsigned char _pad[0x14];
    CBedDef* PDef;                              // +0x14
    void ReleaseSlot(long slot);                // 0x00746A70
    void Reset();                               // 0x00746810
};
struct CCreatureMode {
    virtual void v00();
    virtual long GetType() const;               // slot 1
};
struct CTCCreatureModeManager {                 // interface 0x31
    CCreatureMode* GetCurrentMode() const;      // 0x0073A6E0
    void RemoveMode(unsigned long mode);        // 0x0073ABA0
};
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;
    CThing* GetPItem() const;                   // 0x00A01B50
};
__forceinline bool IsThingAlive(const CBaseIntelligentPointer& thing) { return thing.GetPItem() != 0 && !(thing.GetPItem()->m_flags91 & 1); }
struct CWorld {
    static long GetWorldFrame() { return GFGetCurrentFrame(); }
};
struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    unsigned char _pad08[0x0c];
    CThingCreatureBase* GetCreature() const { return PBrain->Creature; }
    CWorld* GetPThingWorld();                   // 0x008FCFA0
};
struct CAIStateGroup_GoToBed : CAIStateGroupBase {
    CBaseIntelligentPointer PBed;               // +0x14
    unsigned char _pad1c[0x18];
    bool InBed;                                 // +0x34
    long BedSlot;                               // +0x38
    void OnReplace();                           // 0x0092C740
};
struct CAIStateGroup_MinionSleep : CAIStateGroup_GoToBed {
    void OnReplace();
};

void CAIStateGroup_MinionSleep::OnReplace()
{
    CTCSleep* sleep;
    if (GetCreature()->GetInterface(&sleep, 0xd4)) {
        sleep->Asleep = false;
        sleep->Dreaming = false;
        sleep->FrameStateChanged = GetPThingWorld()->GetWorldFrame();
    }
    CTCCreatureModeManager* modes;
    GetCreature()->GetInterface(&modes, 0x31);
    bool asleep = modes->GetCurrentMode()->GetType() == 0xd;
    if (asleep) {
        if (IsThingAlive(PBed)) {
            const CTCBed* bedTC = 0;
            ((const CThing*)PBed.GetPItem())->GetInterface(&bedTC, 0x35);
            CDefPointer def(bedTC->PDef);
            CCharString anim = g_DefStringTable.GetString(def->GetUpAnim);
            if (anim == "")
                anim = g_DefStringTable.GetString(def->FallbackGetUpAnim);
            CThing* bedThing = PBed.GetPItem();
            if (GetUpFromSleeping(GetCreature(), bedThing, anim, PBrain)) {
                CTCBed* bed;
                if (IsThingAlive(PBed) && PBed.GetPItem()->GetInterface(&bed, 0x35) && BedSlot)
                    bed->ReleaseSlot(BedSlot);
                asleep = false;
            }
        }
        if (asleep) {
            modes->RemoveMode(0xd);
            CTCBed* bed;
            if (IsThingAlive(PBed) && PBed.GetPItem()->GetInterface(&bed, 0x35))
                bed->Reset();
        }
    }
    CAIStateGroup_GoToBed::OnReplace();
    InBed = false;
}
