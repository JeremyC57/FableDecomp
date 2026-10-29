// CAIStateGroup_MinionFaceHero::Cleanup() -- drop the hero pointer and stamp the cleanup frame.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00935530, length 74 bytes (+ int3 padding).
// Callees: CBaseIntelligentPointer::CBaseIntelligentPointer() @ 0x00A01B10, SetPItem @ 0x00A01B90 (twice),
// GetPItem @ 0x00A01B50, ~CBaseIntelligentPointer @ 0x00A01C10 (all on the 8-byte frame temporary), then
// CWorld::GetFrame @ 0x0049D870 (__fastcall, no arguments).
// `PHero = 0` goes through a converting CIntelligentPointer temporary (base constructor, vtable 0x01237F14 =
// CIntelligentPointer<CThingPlayerCreature>, SetPItem(0)) and the inline assignment `SetPItem(other.GetPItem())`
// on the member at +0x14; the temporary is destroyed before the frame read; m_frameCleanedUp at +0x1c.
// No engine header for this class; layout local, names descriptive.
class CThing;

class CWorld { public: static long __fastcall GetFrame(); };        // 0x0049D870

class CBaseObject;
struct CBaseIntelligentPointer {
    virtual void v00();
    void* m_pItem;                          // +4
    CBaseIntelligentPointer();              // 0x00A01B10
    CBaseObject* GetPItem() const;               // 0x00A01B50
    void SetPItem(CThing* item);            // 0x00A01B90
    virtual ~CBaseIntelligentPointer();             // 0x00A01C10
};

struct CIntelligentPointer : CBaseIntelligentPointer {    // vtable 0x01237F14
    CIntelligentPointer(CThing* item) : CBaseIntelligentPointer() { SetPItem(item); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other) { SetPItem(((CThing*)other.GetPItem())); return *this; }
};

class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();
    unsigned char _pad[0x10];               // +0x04..+0x13
};

struct CAIStateGroup_MinionFaceHero : CAIStateGroupBase {
    CIntelligentPointer PHero;              // +0x14 CIntelligentPointer<CThingPlayerCreature>
    long m_frameCleanedUp;                  // +0x1c
    virtual void Cleanup();
};

void CAIStateGroup_MinionFaceHero::Cleanup()
{
    PHero = 0;
    m_frameCleanedUp = CWorld::GetFrame();
}