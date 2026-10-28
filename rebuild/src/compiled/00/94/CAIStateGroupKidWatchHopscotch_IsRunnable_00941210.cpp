// CAIStateGroup_KidWatchHopscotch::IsRunnable() -- update the fatigue flag from the activation rate (tired above 3 per minute, rested below 2.5), then, if not fatigued, find the nearest hopscotch board and cache its centre and radius
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00941210, length 417 bytes (+ int3 padding).
// Callees: CWorld::GetWorldFrame @ 0x0049D870 (static, twice), CThingCreatureBase::PeekPos @ 0x006A4D00 (the creature's
// byte twin of CThing::PeekPos), CAIStateGroupBase::PeekThingSearchTools @ 0x008FD000,
// CThingSearchTools::GetPBestThing<CIsHopscotchBoard, CThingCompare_Nearest, CThingObject> @ 0x00941450 (this TU),
// CBaseIntelligentPointer ctor @ 0x00A01B10 / SetPItem @ 0x00A01B90 / GetPItem @ 0x00A01B50 / dtor @ 0x00A01C10,
// CVectorMap::LowerBound const @ 0x004365B0 (inlined lookup 2 = CTCPhysicsBase), CTCPhysicsBase virtual slot 93 (+0x174,
// GetPhysicsMeshInfo), CThing::PeekPos @ 0x004C73D0.
// Globals: 2.5f @ 0x01292718, 3.0f @ 0x0122DED4; CIntelligentPointer<CThing> vtable 0x01238C6C.
// Shape: the rate expression is written in each arm (VC hoists the fild/fimul and spills it per arm); the empty filter has
// a user ctor (no zero byte store); the creature position is a named C3DVector local; the board out pointer lives in an
// inner `{}` block so the physics lookup key reuses its frame word; the intelligent-pointer ctor takes `U* const&`, so
// the board is read after the base ctor; the physics lookup is the out-pointer GetInterface (miss: never-written word).
// Layout: Ego_r.pdb CAIStateGroup_KidWatchHopscotch (PHopscotchBoard +0x14, HopscotchCentre +0x1c, HopscotchRadius +0x28,
// CreationFrame +0x2c, FramesPerMin +0x34, Fatigued +0x38, ActivationCount +0x3c, ActivationFrequency +0x40);
// CThingPhysicsMeshInfo from Ego_r.pdb (ToCentre +0x08, BoundingCircleRadius +0x18). The engine header flattens the base.
class C3DVector { public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector operator+(const C3DVector& v) const { return C3DVector(X + v.X, Y + v.Y, Z + v.Z); }
};
class CTCBase;

struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    const SEntry* LowerBound(const long* type) const;   // 0x004365B0
};

class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad1[0x45];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    const C3DVector& GetPos() const;    // 0x004C73D0
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            const CInterfaceMap* map = &m_interfaces;
            const CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *ppOut = (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
            return true;
        }
        return false;
    }
};
class CThingPhysical : public CThing { public:
    const C3DVector& PeekPos() const;       // 0x006A4D00
};
class CThingCreatureBase : public CThingPhysical { public:
};
class CThingObject : public CThing { public: };

struct CBaseIntelligentPointer {                // 8 bytes: vptr + the pointee link
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CBaseIntelligentPointer();                  // 0x00A01B10
    ~CBaseIntelligentPointer();                 // 0x00A01C10
    void SetPItem(const CThing* item);          // 0x00A01B90
    CThing* GetPItem() const;                   // 0x00A01B50
};
struct CIntelligentPointer : CBaseIntelligentPointer {   // CIntelligentPointer<CThing>, vtable 0x01238C6C
    virtual void v00();
    template<class U> CIntelligentPointer(U* const& thing) : CBaseIntelligentPointer() { SetPItem(thing); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other) { CThing* item = other.GetPItem(); SetPItem(item); return *this; }
};

class CThingPhysicsMeshInfo { public:
    unsigned short BankIndex;                   // +0x00
    float          RenderSizeX;                 // +0x04
    C3DVector      ToCentre;                    // +0x08
    float          BoundingRadius;              // +0x14
    float          BoundingCircleRadius;        // +0x18
};
class CTCPhysicsBase { public:                         // interface 2
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005();
    virtual void v006(); virtual void v007(); virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011();
    virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016(); virtual void v017();
    virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023();
    virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029();
    virtual void v030(); virtual void v031(); virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035();
    virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039(); virtual void v040(); virtual void v041();
    virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047();
    virtual void v048(); virtual void v049(); virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053();
    virtual void v054(); virtual void v055(); virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059();
    virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063(); virtual void v064(); virtual void v065();
    virtual void v066(); virtual void v067(); virtual void v068(); virtual void v069(); virtual void v070(); virtual void v071();
    virtual void v072(); virtual void v073(); virtual void v074(); virtual void v075(); virtual void v076(); virtual void v077();
    virtual void v078(); virtual void v079(); virtual void v080(); virtual void v081(); virtual void v082(); virtual void v083();
    virtual void v084(); virtual void v085(); virtual void v086(); virtual void v087(); virtual void v088(); virtual void v089();
    virtual void v090(); virtual void v091(); virtual void v092();
    virtual const CThingPhysicsMeshInfo* GetPhysicsMeshInfo();   // slot 93 (+0x174)
};

class CIsHopscotchBoard { public: CIsHopscotchBoard() { } };
class CThingCompare_Nearest { public:
    C3DVector Pos;
    CThingCompare_Nearest(const C3DVector& pos) : Pos(pos) { }
};
class CThingSearchTools { public:
    template<class TFilter, class TCompare, class TThing>
    bool GetPBestThing(const TFilter& filter, const TCompare& compare, TThing** ppBest) const;   // 0x00941450 <..>
};
class CWorld { public:
    static long __fastcall GetWorldFrame();     // 0x0049D870
};

class CAIBrain { public:
    virtual void v00();
    unsigned char _pad[0x14];
    CThingCreatureBase* PCreature;              // +0x18
};

class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    unsigned char Terminated, Priority, Enabled, _pad13;
    const CThingSearchTools& PeekThingSearchTools() const;   // 0x008FD000
};

struct CAIStateGroup_KidWatchHopscotch : CAIStateGroupBase {
    CIntelligentPointer PHopscotchBoard;        // +0x14
    C3DVector HopscotchCentre;                  // +0x1c
    float HopscotchRadius;                      // +0x28
    long CreationFrame;                         // +0x2c
    long StartFrame;                            // +0x30
    long FramesPerMin;                          // +0x34
    bool Fatigued;                              // +0x38
    long ActivationCount;                       // +0x3c
    float ActivationFrequency;                  // +0x40
    bool IsRunnable();
};

bool CAIStateGroup_KidWatchHopscotch::IsRunnable()
{
    if (CWorld::GetWorldFrame() > CreationFrame) {
        if (Fatigued) {
            ActivationFrequency = (float)ActivationCount * FramesPerMin / (float)(CWorld::GetWorldFrame() - CreationFrame);
            if (ActivationFrequency < 2.5f)
                Fatigued = false;
        } else {
            ActivationFrequency = (float)ActivationCount * FramesPerMin / (float)(CWorld::GetWorldFrame() - CreationFrame);
            if (ActivationFrequency > 3.0f)
                Fatigued = true;
        }
    }
    if (!Fatigued) {
        {
            CThingObject* board;
            C3DVector pos = PBrain->PCreature->PeekPos();
            if (!PeekThingSearchTools().GetPBestThing(CIsHopscotchBoard(), CThingCompare_Nearest(pos), &board))
                return false;
            PHopscotchBoard = board;
        }
        CTCPhysicsBase* physics;
        PHopscotchBoard.GetPItem()->GetInterface(&physics, 2);
        const CThingPhysicsMeshInfo* mesh = physics->GetPhysicsMeshInfo();
        HopscotchCentre = mesh->ToCentre + PHopscotchBoard.GetPItem()->GetPos();
        HopscotchRadius = mesh->BoundingCircleRadius;
        return true;
    }
    return false;
}