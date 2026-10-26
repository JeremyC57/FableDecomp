// Behaviour test for 008faa80: src/008faa80.cpp copied verbatim, callees stubbed below so calls and arguments are observable.
// NCameraModes::CPhysicsForceModelBase::DoCreatureEvasion()
#include <cstdio>
#include <cmath>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// NCameraModes::CPhysicsForceModelBase::DoCreatureEvasion() -- potential-field push away from the owner creature: the creature point (focal X/Y at the camera height) is sampled 1/distance around the camera position and the scaled gradient is returned
// Leaf: ?DoCreatureEvasion@CPhysicsForceModelBase@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008FAA80, 417 bytes, `ret 4` (C3DVector by hidden pointer).
// Callees: 0x0084F1B0 (the owner's thing; manifest label "_Cons_val<...>" propagated), CTCGraphicAppearance::GetCameraFocalPos
//   0x004C12D0 (thing +0x64), malloc 0x00BFEA0E (list header and node, 0x14 bytes), the list<C3DVector> destructor 0x0048CA60
//   (manifest "SortTreeRecursively"). Constants: offsets table 0x013893AC {0, X, Y, Z}, 0.1f 0x0122EE58, 50.0f 0x0124E7F8,
//   10.0f 0x01237F0C.
// Shape: the creature point pushed into the local list is C3DVector(focal.X, focal.Y, position.Z) (retail reads the node's
//   Z straight from the position local); C3DVector has a user member-wise copy ctor (the x87 sample sums then spill to
//   frame temporaries as retail), and the list<C3DVector> destructor instantiation gets a same-TU noinline body with the
//   retail clear-and-free walk, so VC7.1 knows the list header does not escape and keeps it in esi after push_back.
// Layout: retail Owner +0xA4 (PDB +0xEC: each CPhysicsCollisionPointsObject is 0x18 bytes narrower in retail).
extern "C" void* __cdecl malloc(unsigned int size); // 0x00BFEA0E
extern "C" void __cdecl free(void* p);              // 0x00BFEA14
inline void* __cdecl operator new(unsigned int, void* place) { return place; }
float __cdecl sqrtf_(float);
class C3DVector { public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
    C3DVector operator+(const C3DVector& o) const { return C3DVector(X + o.X, Y + o.Y, Z + o.Z); }
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    C3DVector operator*(float s) const { return C3DVector(X * s, Y * s, Z * s); }
    float MagSquared() const { return X * X + Y * Y + Z * Z; }
};
float Sqrt(float x);
#pragma intrinsic(sqrt)
extern "C" double __cdecl sqrt(double);
inline float Magnitude(const C3DVector& v) { return (float)sqrt(v.Z * v.Z + v.Y * v.Y + v.X * v.X); }

struct SVectorNode { SVectorNode* _M_next; SVectorNode* _M_prev; C3DVector _M_data; };
struct CVectorList {                               // STLport list<C3DVector>: 4-byte header pointer
    SVectorNode* _M_node;
    CVectorList()
    {
        SVectorNode* head = (SVectorNode*)malloc(sizeof(SVectorNode));
        head->_M_next = head;
        head->_M_prev = head;
        _M_node = head;
    }
    ~CVectorList();                                // 0x0048CA60
    void push_back(const C3DVector& value)
    {
        SVectorNode* position = _M_node;
        SVectorNode* node = (SVectorNode*)malloc(sizeof(SVectorNode));
        new (&node->_M_data) C3DVector(value);
        SVectorNode* prev = position->_M_prev;
        node->_M_next = position;
        node->_M_prev = prev;
        prev->_M_next = node;
        position->_M_prev = node;
    }
};

#pragma auto_inline(off)
__declspec(noinline) CVectorList::~CVectorList()      // list<C3DVector>::~list 0x0048CA60 (clear, then free the header)
{
    SVectorNode* cur = _M_node->_M_next;
    while (cur != _M_node) {
        SVectorNode* tmp = cur;
        cur = cur->_M_next;
        free(tmp);
    }
    _M_node->_M_next = _M_node;
    _M_node->_M_prev = _M_node;
    if (_M_node)
        free(_M_node);
}
#pragma auto_inline(on)

extern const C3DVector g_EvasionSampleOffsets[4];  // 0x013893AC: 0, X, Y, Z
struct CTCGraphicAppearance {
    C3DVector GetCameraFocalPos(float t) const;    // 0x004C12D0
};
class CThing { public:
    unsigned char         _pad00[0x64];
    CTCGraphicAppearance* Appearance;              // +0x64
};
namespace NCameraModes {
class CBase { public:
    CThing* GetThing() const;                      // 0x0084F1B0
};
class CPhysicsState { public:
    C3DVector Position;                            // +0x00
};
class CPhysicsForceModelBase { public:
    virtual void v00();
    CPhysicsState* LinkedState;                    // +0x04
    unsigned char  _pad08[0xa4 - 0x08];
    CBase*         Owner;                          // +0xa4 (PDB +0xEC)
    C3DVector DoCreatureEvasion();
};

C3DVector CPhysicsForceModelBase::DoCreatureEvasion()
{
    CThing* thing = Owner->GetThing();
    C3DVector focal = thing->Appearance->GetCameraFocalPos(1.0f);
    C3DVector position = LinkedState->Position;
    CVectorList creatures;
    creatures.push_back(C3DVector(focal.X, focal.Y, position.Z));
    float potential[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
    for (SVectorNode* it = creatures._M_node->_M_next; it != creatures._M_node; it = it->_M_next) {
        for (long i = 0; i < 4; ++i) {
            C3DVector sample = g_EvasionSampleOffsets[i] * 0.1f + position;
            potential[i] += 50.0f / Magnitude(sample - it->_M_data);
        }
    }
    return C3DVector((potential[1] - potential[0]) * 10.0f, (potential[2] - potential[0]) * 10.0f, (potential[3] - potential[0]) * 10.0f);
}
}

// ---- stubs and driver ----
const C3DVector g_EvasionSampleOffsets[4] = { C3DVector(0.0f, 0.0f, 0.0f), C3DVector(1.0f, 0.0f, 0.0f),
                                              C3DVector(0.0f, 1.0f, 0.0f), C3DVector(0.0f, 0.0f, 1.0f) };
static CThing g_thing;
static CTCGraphicAppearance g_appearance;
static C3DVector g_focal(0.0f, 0.0f, 0.0f);
static int g_getThing = 0, g_getFocal = 0;
static const void* g_ownerSeen = 0;
static float g_tSeen = 0.0f;
CThing* NCameraModes::CBase::GetThing() const { ++g_getThing; g_ownerSeen = this; return &g_thing; }
C3DVector CTCGraphicAppearance::GetCameraFocalPos(float t) const
{
    ++g_getFocal; g_tSeen = t;
    if (this != &g_appearance) return C3DVector(999.0f, 999.0f, 999.0f);
    return g_focal;
}
void NCameraModes::CPhysicsForceModelBase::v00() {}
static float Pot(const C3DVector& pos, const C3DVector& off, const C3DVector& c)
{
    double x = pos.X + off.X * 0.1 - c.X, y = pos.Y + off.Y * 0.1 - c.Y, z = pos.Z + off.Z * 0.1 - c.Z;
    return (float)(50.0 / sqrt(x * x + y * y + z * z));
}
static bool Near(float a, float b) { return fabs(a - b) <= 1e-3 * (fabs(b) + 1.0); }
static void Run(float px, float py, float pz, float fx, float fy, float fz)
{
    NCameraModes::CPhysicsState state; state.Position = C3DVector(px, py, pz);
    NCameraModes::CBase owner;
    NCameraModes::CPhysicsForceModelBase model;
    model.LinkedState = &state; model.Owner = &owner;
    g_thing.Appearance = &g_appearance;
    g_focal = C3DVector(fx, fy, fz);
    g_getThing = g_getFocal = 0; g_tSeen = 0.0f; g_ownerSeen = 0;
    C3DVector r = model.DoCreatureEvasion();
    CHECK(g_getThing == 1 && g_ownerSeen == &owner);
    CHECK(g_getFocal == 1 && g_tSeen == 1.0f);
    C3DVector creature(fx, fy, pz);                 // the creature point is taken at the camera height
    C3DVector pos(px, py, pz);
    float p0 = Pot(pos, g_EvasionSampleOffsets[0], creature);
    float p1 = Pot(pos, g_EvasionSampleOffsets[1], creature);
    float p2 = Pot(pos, g_EvasionSampleOffsets[2], creature);
    float p3 = Pot(pos, g_EvasionSampleOffsets[3], creature);
    CHECK(Near(r.X, (p1 - p0) * 10.0f));
    CHECK(Near(r.Y, (p2 - p0) * 10.0f));
    CHECK(Near(r.Z, (p3 - p0) * 10.0f));
}
int main()
{
    Run(10.0f, 20.0f, 5.0f, 13.0f, 24.0f, 50.0f);   // focal Z far above: ignored (Z gradient is the pure offset term)
    Run(-3.0f, 2.0f, 1.0f, -1.0f, -4.0f, -7.0f);
    Run(0.0f, 0.0f, 0.0f, 2.0f, 0.0f, 0.0f);         // creature straight along +X: push in -X, Y/Z gradients from the offsets only
    if (fails == 0) printf("008faa80_TEST PASS\n");
    return fails;
}