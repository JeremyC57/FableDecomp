// CThingPlayerCreature::ApplyRelativeMovementAcceleration(const C3DVector& acceleration, EGameAction action) -- measure the camera-relative push against the physics velocity, scale it, and add it to the per-action relative acceleration component map
// Leaf: ApplyRelativeMovementAcceleration
// Grade: DIFFER(394v447) -- retail frame is 0x68 with unpacked temporaries, this in edi and ebp pushed only around the camera block; VC here packs to 0x1c, keeps scale in a dead arg slot and holds acceleration in ebp (5 variants tried)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006AD3C0, 447 bytes + int3, `ret 8`.
// Signature from Ego_r.pdb: ?ApplyRelativeMovementAcceleration@CThingPlayerCreature@@QAEXABVC3DVector@@W4EGameAction@@@Z.
// Callees: CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound @ 0x004365B0 (the inlined GetInterface lookup, here the
// const overload), the camera-manager getter @ 0x004C7AB0, its mode getter @ 0x0049D9D0, C3DVector angle getter
// @ 0x00A0C380, C3DVector rotate @ 0x00A14040, C3DVector normalise @ 0x00A14440, the controller getter @ 0x0073A6E0,
// map<EGameAction, pair<C3DVector,long> >::find @ 0x006AE3E0 and ::insert @ 0x006AE9F0.
// Layout: retail CThing (UsingComponents +0x20, Components +0x44, PhysicsTC +0x60, PDB minus 0x10);
// IdleCounter +0x124 and RelativeMovementAccelerationComponents +0x1a8 (PDB +0x1d0, retail creature tail is 0x28
// smaller).
struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    void RotateAboutZ(float angle);                 // 0x00A14040
    void Normalise();                               // 0x00A14440
    float GetAngleXY() const;                       // 0x00A0C380
    C3DVector& operator+=(const C3DVector& o) { X += o.X; Y += o.Y; Z += o.Z; return *this; }
};
inline C3DVector operator*(float s, const C3DVector& v) { return C3DVector(s * v.X, s * v.Y, s * v.Z); }

template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
    pair(const T1& a, const T2& b) : first(a), second(b) {}
    template <class U1, class U2> pair(const pair<U1, U2>& p) : first(p.first), second(p.second) {}
};
template <class T1, class T2> inline pair<T1, T2> make_pair(const T1& a, const T2& b) { return pair<T1, T2>(a, b); }

enum EGameAction { GAME_ACTION_NONE };

struct CTCBase;

struct CMovementController {
    virtual void Slot0();
    virtual void Slot1();
    virtual void Slot2();
    virtual void Slot3();
    virtual void Slot4();
    virtual void Slot5();
    virtual void Slot6();
    virtual float GetRelativeSpeed(const C3DVector& direction, const C3DVector& velocity, bool flag);   // slot 7
};

struct CTCPlayerMovement {
    CMovementController* GetController();           // 0x0073A6E0
};

struct CCameraMode {
    unsigned char _pad0[0x220];
    bool FreeLook;                                  // +0x220
};

struct CCameraHeading {
    virtual void Slot0();
    virtual void Slot1();
    virtual void Slot2();
    virtual void Slot3();
    virtual void Slot4();
    virtual void Slot5();
    virtual void Slot6();
    virtual float GetHeading();                     // slot 7
};

struct CCameraManager {
    virtual void Slot0();
    virtual void Slot1();
    virtual void Slot2();
    virtual void Slot3();
    virtual void Slot4();
    virtual void Slot5();
    virtual C3DVector GetDirection();               // slot 6
    CCameraMode* GetMode();                         // 0x0049D9D0
    unsigned char _pad4[0x30];
    CCameraHeading* Heading;                        // +0x34
};

struct CTCPhysicsBase {
    virtual void Slot0();
    virtual void Slot1();
    virtual void Slot2();
    virtual void Slot3();
    virtual void Slot4();
    virtual void Slot5();
    virtual void Slot6();
    virtual void Slot7();
    virtual void Slot8();
    virtual void Slot9();
    virtual void Slot10();
    virtual void Slot11();
    virtual void Slot12();
    virtual void Slot13();
    virtual void Slot14();
    virtual void Slot15();
    virtual void Slot16();
    virtual void Slot17();
    virtual void Slot18();
    virtual void Slot19();
    virtual void Slot20();
    virtual void Slot21();
    virtual void Slot22();
    virtual void Slot23();
    virtual void Slot24();
    virtual void Slot25();
    virtual void Slot26();
    virtual void Slot27();
    virtual void Slot28();
    virtual void Slot29();
    virtual void Slot30();
    virtual void Slot31();
    virtual void Slot32();
    virtual void Slot33();
    virtual void Slot34();
    virtual void Slot35();
    virtual void Slot36();
    virtual void Slot37();
    virtual void Slot38();
    virtual void Slot39();
    virtual void Slot40();
    virtual void Slot41();
    virtual void Slot42();
    virtual void Slot43();
    virtual void Slot44();
    virtual void Slot45();
    virtual void Slot46();
    virtual void Slot47();
    virtual void Slot48();
    virtual void Slot49();
    virtual void Slot50();
    virtual void Slot51();
    virtual void Slot52();
    virtual void Slot53();
    virtual void Slot54();
    virtual void Slot55();
    virtual void Slot56();
    virtual void Slot57();
    virtual void Slot58();
    virtual void Slot59();
    virtual void Slot60();
    virtual void Slot61();
    virtual void Slot62();
    virtual void Slot63();
    virtual void Slot64();
    virtual void Slot65();
    virtual void Slot66();
    virtual void Slot67();
    virtual void Slot68();
    virtual void Slot69();
    virtual C3DVector GetVelocity();                // slot 70
};

struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    const SEntry* LowerBound(const long* type) const;   // 0x004365B0
};

typedef pair<C3DVector, long> SAccelComponent;
typedef pair<const EGameAction, SAccelComponent> SAccelValue;
struct SAccelNode {
    long Colour;
    SAccelNode* Parent;
    SAccelNode* Left;
    SAccelNode* Right;
    SAccelValue Value;
};
struct CAccelIterator {
    SAccelNode* Node;
    CAccelIterator() {}
    CAccelIterator(SAccelNode* n) : Node(n) {}
    bool operator==(const CAccelIterator& o) const { return Node == o.Node; }
    SAccelValue* operator->() const { return &Node->Value; }
};
struct CAccelInsertResult {
    CAccelIterator It;
    bool Inserted;
};
struct CAccelMap {
    SAccelNode* Header;
    long NodeCount;
    long Compare;
    CAccelIterator end() { return CAccelIterator(Header); }
    CAccelIterator find(const EGameAction& key);                 // 0x006AE3E0
    CAccelInsertResult insert(const SAccelValue& value);        // 0x006AE9F0
};

#pragma auto_inline(off)
CAccelIterator CAccelMap::find(const EGameAction& key)
{
    return CAccelIterator(Header);
}
CAccelInsertResult CAccelMap::insert(const SAccelValue& value)
{
    CAccelInsertResult r;
    r.It = CAccelIterator(Header);
    r.Inserted = value.second.second != 0;
    return r;
}
#pragma auto_inline(on)

struct CThingPlayerCreature {
    virtual void Slot0();
    virtual void Slot1();
    virtual void Slot2();
    virtual void Slot3();
    virtual void Slot4();
    virtual void Slot5();
    virtual void Slot6();
    virtual void Slot7();
    virtual void Slot8();
    virtual void Slot9();
    virtual void Slot10();
    virtual void Slot11();
    virtual void Slot12();
    virtual void Slot13();
    virtual void Slot14();
    virtual void Slot15();
    virtual void Slot16();
    virtual void Slot17();
    virtual void Slot18();
    virtual void Slot19();
    virtual void Slot20();
    virtual void Slot21();
    virtual void Slot22();
    virtual void Slot23();
    virtual void Slot24();
    virtual void Slot25();
    virtual void Slot26();
    virtual void Slot27();
    virtual void Slot28();
    virtual void Slot29();
    virtual void Slot30();
    virtual void Slot31();
    virtual void Slot32();
    virtual void Slot33();
    virtual void Slot34();
    virtual void Slot35();
    virtual void Slot36();
    virtual void Slot37();
    virtual void Slot38();
    virtual void Slot39();
    virtual void Slot40();
    virtual void Slot41();
    virtual void Slot42();
    virtual void Slot43();
    virtual void Slot44();
    virtual void Slot45();
    virtual void Slot46();
    virtual void Slot47();
    virtual void Slot48();
    virtual void Slot49();
    virtual void Slot50();
    virtual void Slot51();
    virtual void Slot52();
    virtual void Slot53();
    virtual void Slot54();
    virtual void Slot55();
    virtual void Slot56();
    virtual void Slot57();
    virtual void Slot58();
    virtual void Slot59();
    virtual void Slot60();
    virtual void Slot61();
    virtual void Slot62();
    virtual void Slot63();
    virtual void Slot64();
    virtual void Slot65();
    virtual void Slot66();
    virtual void Slot67();
    virtual void Slot68();
    virtual void Slot69();
    virtual void Slot70();
    virtual void Slot71();
    virtual void Slot72();
    virtual void Slot73();
    virtual void Slot74();
    virtual void Slot75();
    virtual void Slot76();
    virtual void Slot77();
    virtual void Slot78();
    virtual void Slot79();
    virtual void Slot80();
    virtual void Slot81();
    virtual void Slot82();
    virtual void Slot83();
    virtual void Slot84();
    virtual void Slot85();
    virtual void Slot86();
    virtual float GetMaxSpeed();                    // slot 87
    CCameraManager* GetCameraManager();             // 0x004C7AB0

    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType) const
    {
        if (UsingComponents[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            const CInterfaceMap* map = &Components;
            const CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }

    unsigned char _pad04[0x20 - 0x04];
    unsigned long UsingComponents[9];               // +0x20
    CInterfaceMap Components;                       // +0x44
    unsigned char _pad4c[0x60 - 0x4c];
    CTCPhysicsBase* PhysicsTC;                      // +0x60
    unsigned char _pad64[0x124 - 0x64];
    long IdleCounter;                               // +0x124
    unsigned char _pad128[0x1a8 - 0x128];
    CAccelMap RelativeMovementAccelerationComponents;   // +0x1a8

    void ApplyRelativeMovementAcceleration(const C3DVector& acceleration, EGameAction action);
};

void CThingPlayerCreature::ApplyRelativeMovementAcceleration(const C3DVector& acceleration, EGameAction action)
{
    float scale = 1.0f;
    CTCPlayerMovement* movement;
    GetInterface(&movement, 0x31);
    C3DVector direction = acceleration;
    {
        CCameraManager* cameras = GetCameraManager();
        float heading;
        if (cameras->GetMode()->FreeLook)
            heading = cameras->GetDirection().GetAngleXY();
        else
            heading = cameras->Heading->GetHeading();
        direction.RotateAboutZ(heading);
        direction.Normalise();
        float speed = movement->GetController()->GetRelativeSpeed(direction, PhysicsTC->GetVelocity(), false);
        if (speed > 0.0001f)
            scale = speed / GetMaxSpeed();
    }
    CAccelIterator it = RelativeMovementAccelerationComponents.find(action);
    C3DVector scaled = scale * acceleration;
    if (it == RelativeMovementAccelerationComponents.end()) {
        RelativeMovementAccelerationComponents.insert(make_pair(action, make_pair(scaled, 1)));
    } else {
        it->second.first += scaled;
        it->second.second++;
    }
    IdleCounter = 0;
}
