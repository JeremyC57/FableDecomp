// NCameraModes::CScriptModuleConversationBase::IsThingsOk() -- true when both the speaker and the spoken-to thing exist and are different things
// Leaf: IsThingsOk
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008527F0, length 47 bytes.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 on PSpeaker (+0x4c) then PSpokenTo (+0x54).
// Shape: both pointers fetched into locals first (speaker kept in edi across the second call); the
// `bool` result of an `&&` chain is materialised in the full register (`mov eax,1` / `xor eax,eax`,
// two exits) -- the `if (...) return true; return false;` form would give `mov al,1`/`xor al,al`.
// Layout: PDB NCameraModes::CScriptModuleConversationBase (0x60 bytes): PSpeaker +0x4c, PSpokenTo +0x54,
// RandomDirection +0x5c, over the 0x4c-byte CScriptModuleBase. No engine header exists.
class CBaseObject;
struct CBase;
class CGameCameraManager;
class C3DVector { public: float X, Y, Z; };
class CRightHandedSet { public: C3DVector Up; C3DVector Forward; };

class CThing_Retail { public:
    unsigned char  _pad00[0x91];
    unsigned char  m_dead : 1;                       // +0x91 bit 0
    unsigned char  m_91_1 : 7;
};

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

struct CScriptModuleBase {                           // NCameraModes::CScriptModuleBase, 0x4c bytes (PDB)
    virtual void v00();
    CIntelligentPointer<CThing_Retail> PCamPoint;       // +0x04
    CIntelligentPointer<CThing_Retail> PThingOrigin;    // +0x0c
    C3DVector           CoordOrigin;                 // +0x14
    CRightHandedSet     CoordAxis;                   // +0x20
    CBase*              POwner;                      // +0x38
    CGameCameraManager* PCameraManager;              // +0x3c
    bool                SelfTerminate;               // +0x40
    bool                ShortenOnCollision;          // +0x41
    long                CutOutOverride;              // +0x44
    float               FOVOverride;                 // +0x48
};
namespace NCameraModes {
struct CScriptModuleConversationBase : CScriptModuleBase {
    CIntelligentPointer<CThing_Retail> PSpeaker;        // +0x4c
    CIntelligentPointer<CThing_Retail> PSpokenTo;       // +0x54
    float RandomDirection;                           // +0x5c
    bool IsThingsOk() const;
};

bool CScriptModuleConversationBase::IsThingsOk() const
{
    const CThing_Retail* speaker = PSpeaker.GetPItem();
    const CThing_Retail* spokenTo = PSpokenTo.GetPItem();
    return speaker && spokenTo && speaker != spokenTo;
}
}