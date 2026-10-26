// SW2 sweep (advisor/008d99b0/probe/z3.cpp), on RST2's best.cpp (target as C3DVector(0,0,rise) + StartPos; the copied def; release duplicated by hand on both exits).
// NCreatureMode::CModeJackOfBladesDeath::FrameUpdate() -- Jack of Blades' death rise: freeze the body's physics, lift it from the start position towards start + rise height over the death duration, and when the time is up remove the death mode (0x35)
// Leaf: FrameUpdate
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008D99B0, length 326 bytes (padded to 336), `ret`.
// Callees: CDefClassBase::GetSubDef<T>(T**) @ 0x008C33E0 (on the creature's def +0x70; manifest instantiation name
//   "CAICreatureWillPowerIndicatorDef" is a byte twin), the physics object's (+0x60) virtual slots 38 (+0x98, enable) and 30 (+0x78,
//   SetPosition), __ftol2 @ 0x00BFEA70, CVectorMap::LowerBound @ 0x0040F020 (inlined GetInterface, CTCCreatureModeManager 0x31),
//   CTCCreatureModeManager::RemoveMode @ 0x0073ABA0, the sub-def's virtual slot 1 (destroy on the last release).
// Globals: game frames per second (long) @ 0x01375550.
// Layout: PDB CModeJackOfBladesDeath (0x84) = retail: CModeStandBase 0x68, StartPos +0x74, Timer +0x80; the death sub-def's
//   duration (seconds) +0x60 and rise height +0x64.
// Shape: as the landed CAIStateGroup_JackOfBladesStage1::HasHitsCausedRush 0x00972340: the sub-def pointer is a zero-initialised
//   local filled by GetSubDef(&def) and released by hand (no null test) on both exits; the position is the lerp
//   (target - start) * t + start with t = Timer / (long)(fps * duration); the mode manager pointer is zero-initialised and the
//   GetInterface result is not tested.
class CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};

class C3DVector { public:
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    C3DVector operator+(const C3DVector& o) const { return C3DVector(X + o.X, Y + o.Y, Z + o.Z); }
    C3DVector operator*(float s) const { return C3DVector(X * s, Y * s, Z * s); }
};

struct CJackOfBladesDeathDef {
    virtual void v00();
    virtual void Destroy();                     // slot 1
    long          m_refCount;                   // +0x04
    unsigned char _pad08[0x58];
    float         DeathDuration;                // +0x60 seconds
    float         RiseHeight;                   // +0x64
};

class CDefClassBase { public:
    bool GetSubDef(CJackOfBladesDeathDef** out);   // 0x008C33E0
};
void* FindDef_CDefClassBase_GetSubDef_0();             // stand-in callee
#pragma auto_inline(off)
__declspec(noinline) bool CDefClassBase::GetSubDef(CJackOfBladesDeathDef** out)
{
    void* def = FindDef_CDefClassBase_GetSubDef_0();
    if (!def)
        return false;
    *(void**)out = def;
    return true;
}
#pragma auto_inline(on)

struct CPhysicsObject {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void SetPosition(const C3DVector& position);   // slot 30 (+0x78)
    virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35(); virtual void v36();
    virtual void v37();
    virtual void SetEnabled(bool enabled);      // slot 38 (+0x98)
};

class CTCCreatureModeManager { public:                 // interface type 0x31
    void RemoveMode(unsigned long mode);        // 0x0073ABA0
};

class CThing { public:
    unsigned char   _pad00[0x20];
    unsigned long   m_presentBits[9];           // +0x20 std::bitset of interface types
    CInterfaceMap   m_interfaces;               // +0x44
    unsigned char   _pad4c[0x14];
    CPhysicsObject* PPhysics;                   // +0x60
    unsigned char   _pad64[0x0c];
    CDefClassBase*  PDef;                       // +0x70
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *ppOut = (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
            return true;
        }
        return false;
    }
};

extern long g_GameFramesPerSecond;              // 0x01375550

struct CModeStandBase {                         // retail sizeof 0x68
    virtual void v00();
    CThing*       ParentThing;                  // +0x04
    unsigned char _body[0x60];
};

namespace NCreatureMode {

struct CModeJackOfBladesDeath : CModeStandBase {
    unsigned char Filter[0x0c];                 // +0x68 CAnimFilterOfGroup
    C3DVector     StartPos;                     // +0x74
    long          Timer;                        // +0x80
    void FrameUpdate();
};

void CModeJackOfBladesDeath::FrameUpdate()
{
    CJackOfBladesDeathDef* subDef = 0;
    ParentThing->PDef->GetSubDef(&subDef);
    CJackOfBladesDeathDef* def = subDef;
    ParentThing->PPhysics->SetEnabled(false);
    float t = (float)Timer / (long)(g_GameFramesPerSecond * def->DeathDuration);
    C3DVector target = C3DVector(0.0f, 0.0f, def->RiseHeight) + StartPos;
    C3DVector position = (target - StartPos) * t + StartPos;
    ParentThing->PPhysics->SetPosition(position);
    if (++Timer > g_GameFramesPerSecond * def->DeathDuration) {
        CTCCreatureModeManager* modes = 0;
        ParentThing->GetInterface(&modes, 0x31);
        modes->RemoveMode(0x35);
        if (--def->m_refCount == 0)
            def->Destroy();
        return;
    }
    if (--def->m_refCount == 0)
        def->Destroy();
}

}