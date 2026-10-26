// CGlobalDef::~CGlobalDef() -- the four intelligent pointers at +0xec..+0xd4, the two 12-byte members at +0x74 and +0x68 (0x004E1AAF), the intelligent pointer at +0x4c, then the def base (0x004DA8B7, a tail jump)
// Leaf: ??1CGlobalDef@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004E37BF, length 79 bytes.
// Callees: CBaseIntelligentPointer::~CBaseIntelligentPointer @ 0x00A01C10 five times, the member destructor @ 0x004E1AAF twice (labelled "CActiveFile::OnReadFinished",
// propagated), the base destructor @ 0x004DA8B7 (labelled "~CTCTargetingBase", propagated). Shape: members in reverse declaration order, the size-optimised module.
#pragma optimize("s", on)
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
struct CDefMember_004e1aaf {                       // 12 bytes
    unsigned char _body[12];
    ~CDefMember_004e1aaf();                        // 0x004E1AAF
};
struct CDefBase_004da8b7 {                         // 0x4c bytes (no vtable store in this destructor: the class is not polymorphic here)
    unsigned char _body[0x4c];
    ~CDefBase_004da8b7();                          // 0x004DA8B7
};

class CGlobalDef : public CDefBase_004da8b7 { public:
    CBaseIntelligentPointer m_pointer4c;           // +0x4c
    unsigned char           _pad54[0x14];
    CDefMember_004e1aaf     m_member68;            // +0x68
    CDefMember_004e1aaf     m_member74;            // +0x74
    unsigned char           _pad80[0x54];
    CBaseIntelligentPointer m_pointerd4;           // +0xd4
    CBaseIntelligentPointer m_pointerdc;           // +0xdc
    CBaseIntelligentPointer m_pointere4;           // +0xe4
    CBaseIntelligentPointer m_pointerec;           // +0xec
    ~CGlobalDef();
};

CGlobalDef::~CGlobalDef()
{
}