// NBalverineCombat::CanLungeThisFrame(const CThingCreatureBase& creature) -- true when enough frames have passed since the last lunge: the creature's CBalverineBattleDef SecondsBetweenLunges (default 5 s) times the frame rate is below the frames elapsed since SetLungedThisFrame
// Leaf: ?CanLungeThisFrame@NBalverineCombat@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00905550, 140 bytes, __fastcall (creature in ecx), returns bool.
// Identity: Ego_r.pdb `?CanLungeThisFrame@NBalverineCombat@@YI_NABVCThingCreatureBase@@@Z` (catalog `_global`).
// Callees: CDefClassBase::GetSubDef<CBalverineBattleDef> 0x008CD0D0 (on the creature's def class, +0x70),
//   GFGetCurrentFrame 0x0049D870 (manifest "GFGetPreMainMemoryUsed" is propagated); the def is released through
//   its vtable slot 1. Globals: g_framesPerSecond 0x01375550, g_lastLungeFrame 0x013BB660 (written by
//   SetLungedThisFrame 0x009054F0, the function right above this one).
// Layout: retail CBalverineBattleDef SecondsBetweenLunges +0x3c (PDB +0x40: the retail CDefClassBase is 4 bytes
//   shorter); the creature's def class at +0x70 (local model: the generated CThingCreatureBase header is all pads).
// Shape: the def pointer is a zeroed CDefPointer local whose destructor runs once per return (two inlined
//   releases); GetSubDef gets a same-TU auto_inline(off) stand-in body that writes only the out slot, so VC7.1
//   keeps the def in esi and reuses the pointer's frame word for the fild temporary of the frame delta. The
//   frame rate is read into a long local BEFORE the frame call (it is stored to its frame slot there).
struct CDefBase {
    virtual void v00();
    virtual void Destroy();                     // slot 1
    long m_count;                               // +0x04
    void Release()
    {
        if (--m_count == 0)
            Destroy();
    }
};

struct CBalverineBattleDef : CDefBase {
    unsigned char _pad08[0x3c - 0x08];
    float SecondsBetweenLunges;                 // +0x3c
};

template<class T> struct CDefPointer {
    T* m_p;
    CDefPointer() : m_p(0) { }
    ~CDefPointer()
    {
        if (m_p)
            m_p->Release();
    }
    T* operator->() const { return m_p; }
};

struct CDefClassBase {
    template<class T> bool GetSubDef(CDefPointer<T>& out);     // 0x008CD0D0 (this instantiation)
};
void* FindSubDef_CDefClassBase_GetSubDef(CDefClassBase* def);   // stand-in callee of the stand-in body below

#pragma auto_inline(off)
template<class T> __declspec(noinline) bool CDefClassBase::GetSubDef(CDefPointer<T>& out)
{
    void* def = FindSubDef_CDefClassBase_GetSubDef(this);
    if (!def)
        return false;
    out.m_p = (T*)def;
    return true;
}
#pragma auto_inline(on)

struct CThingCreatureBase {
    unsigned char _pad00[0x70];
    CDefClassBase* m_pDef;                      // +0x70
};

long __fastcall GFGetCurrentFrame();            // 0x0049D870
extern long g_framesPerSecond;                  // 0x01375550
extern long g_lastLungeFrame;                   // 0x013BB660

namespace NBalverineCombat {
bool __fastcall CanLungeThisFrame(const CThingCreatureBase& creature);
}

bool __fastcall NBalverineCombat::CanLungeThisFrame(const CThingCreatureBase& creature)
{
    float seconds = 5.0f;
    CDefPointer<CBalverineBattleDef> def;
    if (creature.m_pDef->GetSubDef(def))
        seconds = def->SecondsBetweenLunges;
    long fps = g_framesPerSecond;
    if ((float)(GFGetCurrentFrame() - g_lastLungeFrame) > seconds * fps)
        return true;
    return false;
}