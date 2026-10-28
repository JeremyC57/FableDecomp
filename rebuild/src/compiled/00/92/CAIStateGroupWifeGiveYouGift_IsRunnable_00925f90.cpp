// CAIStateGroup_WifeGiveYouGift::IsRunnable() -- the spouse has a gift to hand over (its interface-0xD7 component
// names a gift def, remembered in GiftToGiveDef) and the hero is alive within 40 units.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00925F90, length 199 bytes (+ int3 padding).
// Callees: CVectorMap::LowerBound @ 0x0040F020 (inlined out-parameter GetInterface, 0xD7 = bit 23 of +0x38; a miss
// returns false), the state group's player-manager accessor @ 0x008FCF90 (manifest "CPixelShader::CountReferences"
// is propagated), CPlayerManager::GetMainPlayer @ 0x00449970, CPlayer::GetPControlledCreature @ 0x00487DC0 (landed names),
// CThing::GetPos @ 0x004C73D0 (twice). Float 1600.0f @ 0x01291740.
// Shape: the gift def read from the component (+0x74) is stored before its `> 0` test; inline dead-bit test on the
// hero; the squared distance accumulated in retail's order (`s = z*z; s += x*x; s += y*y`, chunk-177 lesson) and
// compared `s < 1600.0f` (`test ah,5 ; jp`). PDB names from the engine header (GiftToGiveDef +0x18); local model.
// ---- shared AI state-group model (same shapes as the landed CAIStateGroup_* sources) ----
class CTCBase;
class C3DVector { public: float X, Y, Z; };

struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};

class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad1[0x45];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    const C3DVector& GetPos() const;       // 0x004C73D0
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
};
class CThingPlayerCreature : public CThing { };

class CAIBrain { public:
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};

class CPlayer { public:
    CThingPlayerCreature* GetPControlledCreature() const;                     // 0x00487DC0
};

class CPlayerManager { public:
public:
    CPlayer& GetMainPlayer();   // 0x00449970
};

class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    unsigned char Terminated;                   // +0x10
    unsigned char Priority;                     // +0x11
    unsigned char Enabled;                      // +0x12
    unsigned char _pad_0x13;
    CPlayerManager* GetPPlayerManager();        // 0x008FCF90
};

struct CTCSpouse {                              // interface 0xD7
    unsigned char _pad[0x74];
    long GiftDefToGive;                         // +0x74
};

struct CAIStateGroup_WifeGiveYouGift : CAIStateGroupBase {
    long FrameToSpeakNext;                      // +0x14
    long GiftToGiveDef;                         // +0x18
    bool DeliveredGiveSpeech;                   // +0x1c
    virtual bool IsRunnable();
};

bool CAIStateGroup_WifeGiveYouGift::IsRunnable()
{
    CTCSpouse* spouse;
    if (PBrain->PCreature->GetInterface(&spouse, 0xD7)) {
        GiftToGiveDef = spouse->GiftDefToGive;
        if (GiftToGiveDef > 0) {
            CThing* hero = GetPPlayerManager()->GetMainPlayer().GetPControlledCreature();
            if (hero != 0 && !(hero->m_flags91 & 1)) {
                const C3DVector& pos = PBrain->PCreature->GetPos();
                const C3DVector& heroPos = hero->GetPos();
                float dx = heroPos.X - pos.X;
                float dy = heroPos.Y - pos.Y;
                float dz = heroPos.Z - pos.Z;
                float s = dz * dz;
                s += dx * dx;
                s += dy * dy;
                if (s < 1600.0f)
                    return true;
            }
        }
    }
    return false;
}