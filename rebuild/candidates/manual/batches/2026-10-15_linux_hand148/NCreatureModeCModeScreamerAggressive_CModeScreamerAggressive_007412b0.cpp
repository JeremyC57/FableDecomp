// NCreatureMode::CModeScreamerAggressive::CModeScreamerAggressive(CThing* thing) -- CModeStandBase(thing), own vtable, the "SCREAMER_AGGRESSIVE" filter, null effect and target pointers
// Leaf: ??0CModeScreamerAggressive
// Retail: Fable.exe (TLC, VC7.1) @ 0x007412B0 (`ret 4`). Vtable 0x012681EC = NCreatureMode::CModeScreamerAggressive (rtti-vtables.tsv); manifest label CModeJackOfBladesFloat is propagated; vtable 0x012681EC = NCreatureMode::CModeScreamerAggressive.
// Callees: CModeStandBase::CModeStandBase(CThing*) @ 0x00861E00 (manifest label "CTCCreatureNavigation" is propagated;
// 0x008621A0 CModeStandBase::OnActivate sits beside it), CCharString @ 0x0099EBF0 / 0x0099E4B0 / operator= 0x0099EFB0 /
// ~ 0x0099EAE0, CRC @ 0x004014A0, CBaseIntelligentPointer::CBaseIntelligentPointer @ 0x00A01B10 / SetPItem @ 0x00A01B90.
// Shape: the filter (vtable 0x01267568) is built from an implicit CCharString conversion of the literal (temporary
// re-addressed with lea, destroyed after the filter); its CRC is ONE `p ? .. : ..` pair of guards that VC folds into a
// single null test; then the remaining init-list members in declaration order.
// Layout (retail): CModeStandBase 0x68, Filter +0x68 (CAnimFilterOfGroup 0xc), members after it as declared below
// (PDB order and sizes; CIntelligentPointer<CThing> is 8 bytes, vptr 0x01238C6C + item).
struct CThing;
struct CBaseObject;

struct CCharStringData {
    const char*   m_pData;                 // +0  (CBasicString<char>::PData)
    unsigned long m_length;                // +4  (StringLength)
};

struct CCharString {
    CCharStringData* m_pData;
    CCharString();                         // 0x0099E4B0
    CCharString(const char* text, long length = -1);   // 0x0099EBF0 (default argument: the literal converts implicitly)
    ~CCharString();                        // 0x0099EAE0
    CCharString& operator=(const CCharString& other);   // 0x0099EFB0
};

unsigned long __fastcall Calc_004014a0(unsigned long seed, const void* data, unsigned long length);   // 0x004014A0: CRC

struct CAnimFilterOfGroup {                // vtable 0x01267568
    virtual void v00();
    CCharString   Group;                   // +4
    unsigned long GroupCRC;                // +8
    CAnimFilterOfGroup(const CCharString& group)
        : Group()
    {
        Group = group;
        GroupCRC = Calc_004014a0(0, group.m_pData == 0 ? "" : group.m_pData->m_pData, group.m_pData == 0 ? 0 : group.m_pData->m_length);   // one shared null test (CSE of the two guards)
    }
};

struct CBaseIntelligentPointer {           // 8 bytes: vptr + the pointed-to item
    virtual void v00();
    CBaseObject* m_pItem;                  // +4
    CBaseIntelligentPointer();             // 0x00A01B10
    void SetPItem(CBaseObject* item);      // 0x00A01B90
};

struct CIntelligentPointer : CBaseIntelligentPointer {   // CIntelligentPointer<CThing>, vtable 0x01238C6C
    virtual void v00();
    CIntelligentPointer() { }
    CIntelligentPointer(CThing* item) { SetPItem((CBaseObject*)item); }
};

struct CModeStandBase {                    // retail sizeof 0x68 (CModeBase 0x1c + the stand-mode members)
    virtual void v00();
    unsigned char _body[0x64];
    CModeStandBase(CThing* thing);         // 0x00861E00
};

namespace NCreatureMode {

struct CModeScreamerAggressive : CModeStandBase {   // vtable 0x012681EC
    CAnimFilterOfGroup  Filter;            // +0x68
    CIntelligentPointer PEffect;           // +0x74
    CIntelligentPointer PTarget;           // +0x7c
    virtual void v00();
    CModeScreamerAggressive(CThing* thing);
};

CModeScreamerAggressive::CModeScreamerAggressive(CThing* thing)
    : CModeStandBase(thing)
    , Filter("SCREAMER_AGGRESSIVE")
{
}

}
