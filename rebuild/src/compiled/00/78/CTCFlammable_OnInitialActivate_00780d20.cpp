// CTCFlammable::OnInitialActivate() -- fetch the flammable def from the parent's def class (dropping the pointer when there is none), reset the burning state from it, build the fire points and ask for per-frame updates
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00780D20, length 94 bytes (vtable 0x0126B1A4 slot 6, the CTCBase
// "OnInitialActivate" slot).
// Callees: CDefClassBase::GetSubDef<CFlammableDef>(CDefPointer<..>*) @ 0x007815A0 (on the def class at +0x70 of the parent),
// CTCFlammable::InitialiseFirePoints() @ 0x007808B0 (labelled "CTCAvoidRegion_Initialize"), CThing::SetForceFrameUpdate(bool)
// @ 0x004C7C60, the def's virtual slot 1 (+0x04, Destroy at count zero).
// Layout: PDB CTCFlammable (56 bytes): PDef +0xc, FirePoints +0x10 (vector: PDB 16 bytes, retail 12), then retail
// Burning +0x1c, FireDamageAfterFrame +0x20, FireResistance +0x24, FireStrength +0x28, DamageCaused +0x2c,
// DamagePerPoint +0x30, HasFirePoint +0x31 (PDB +0x20.. +0x35, 4 higher); the def's FireResistance at +0x28.
// Shape: the def pointer's address held in edi for the call and the release; the zero register (ebx) is set right after
// the call and serves the release's null test, both zero stores of `PDef = 0` (release + unconditional store), the byte
// and dword clears; the def re-read after the reset; the stores follow member order.
#include "engine/CTCBase.h"

struct CDefPointee {
    virtual void v000();
    virtual void Destroy();                      // slot 1 (+0x04)
    long RefCount;                               // +0x04
    void Release()
    {
        if (--RefCount == 0)
            Destroy();
    }
};

struct CFlammableDef : CDefPointee {
    unsigned char _pad[0x20];
    long FireResistance;                         // +0x28
};

struct CFlammableDefPointer {                    // CDefPointer<CFlammableDef const>
    CFlammableDef* Object;
    CFlammableDef* operator->() const { return Object; }
    CFlammableDefPointer& operator=(int)
    {
        if (Object) {
            Object->Release();
            Object = 0;
        }
        Object = 0;
        return *this;
    }
};

struct CDefClassBase {
    bool GetSubDef(CFlammableDefPointer* out);   // 0x007815A0 (GetSubDef<CFlammableDef>)
};

struct CThing {
    unsigned char  _pad[0x70];
    CDefClassBase* m_pDefClass;                  // +0x70
    void SetForceFrameUpdate(bool update);       // 0x004C7C60
};

struct CTCFlammable : CTCBase {
    CFlammableDefPointer PDef;                   // +0x0c
    unsigned char FirePoints[0xc];               // +0x10 vector<NFire::CFirePoint>
    bool          Burning;                       // +0x1c
    long          FireDamageAfterFrame;          // +0x20
    long          FireResistance;                // +0x24
    long          FireStrength;                  // +0x28
    float         DamageCaused;                  // +0x2c
    bool          DamagePerPoint;                // +0x30
    bool          HasFirePoint;                  // +0x31
    void InitialiseFirePoints();                 // 0x007808B0
    void OnInitialActivate();
};

void CTCFlammable::OnInitialActivate()
{
    if (!ParentThing->m_pDefClass->GetSubDef(&PDef))
        PDef = 0;
    Burning = false;
    FireDamageAfterFrame = 0;
    FireResistance = PDef->FireResistance;
    FireStrength = 1;
    DamageCaused = 0.0f;
    DamagePerPoint = false;
    InitialiseFirePoints();
    ParentThing->SetForceFrameUpdate(true);
}