// CThingSoundEmitter::CThingSoundEmitter() -- CThing(type 6) base, zeroed sound object / attach flags, the attached-thing intelligent pointer; kill-on-level-unload cleared when the engine's slot-9 query says false
// Leaf: ??0CThingSoundEmitter
// Retail: Fable.exe 0x008367B0, 89 bytes. Vtables: CThingSoundEmitter 0x01272C24, CIntelligentPointer<const CThing>
// 0x01237F1C.
// Callees: CThing::CThing(unsigned long) 0x004C9030, CBaseIntelligentPointer::CBaseIntelligentPointer
// 0x00A01B10, slot 9 (+0x24) of the engine singleton 0x013B86A0, CThing::SetToKillOnLevelUnload(bool)
// 0x004C9C40.
// Retail layout (Ego_r.pdb is packed here and lacks the +0x98 sound object; its members sit 0x0c higher):
// sound object +0x98, AttachedToThing +0x9c, PAttachedThing +0x9d (unaligned: `#pragma pack(1)`),
// ContinuePlayingAfterAttachedThingDeath +0xa5, Pos +0xa6. Local model.
// Shape: init-list members in declaration order around the intelligent pointer, whose inline derived
// constructor stores its own vtable after the out-of-line base constructor (`mov [edi],1237F1Ch`); the
// member address is kept in edi; the engine query is a body `if`.
class CThing { public:
    virtual void v000();
    unsigned char _pad04[0x94];
    CThing(unsigned long thingType);                     // 0x004C9030
    void SetToKillOnLevelUnload(bool kill);              // 0x004C9C40
};

class CEngine { public:
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003();
    virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007();
    virtual void v008();
    virtual bool IsLevelPersistent();                    // slot 9 (+0x24)
};
extern CEngine* g_pEngine;                               // 0x013B86A0

#pragma pack(push, 1)
struct CBaseIntelligentPointer {
    virtual void v000();
    void* m_pItem;                                       // +0x04
    CBaseIntelligentPointer();                           // 0x00A01B10
};

template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:
    CIntelligentPointer() {}
    virtual void v000();
};

struct CSoundPlayer;
class C3DVector { public: float x, y, z; };

class CThingSoundEmitter : public CThing { public:
    CSoundPlayer*               m_pSoundPlayer;          // +0x98
    bool                        AttachedToThing;         // +0x9c
    CIntelligentPointer<const CThing> PAttachedThing;          // +0x9d
    bool                        ContinuePlayingAfterAttachedThingDeath;   // +0xa5
    C3DVector                   Pos;                     // +0xa6
    CThingSoundEmitter();
};
#pragma pack(pop)

CThingSoundEmitter::CThingSoundEmitter()
    : CThing(6), m_pSoundPlayer(0), AttachedToThing(false), ContinuePlayingAfterAttachedThingDeath(false)
{
    if (!g_pEngine->IsLevelPersistent())
        SetToKillOnLevelUnload(false);
}