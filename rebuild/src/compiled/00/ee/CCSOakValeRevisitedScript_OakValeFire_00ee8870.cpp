// CCS_OakValeRevisitedScript::OakValeFire() -- Oakvale-revisited cutscene thread: wait until the "fire" flag is set,
// gather the Q_REVISITED_FIREPOINT things, size Fires to match and create an OAKVALE_BURNING_PATCH effect at every fire
// point; then wait until the flag is cleared again and remove every fire effect
// Leaf: OakValeFire
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EE8870, 862 bytes (866 with padding).
// Callees: CCharString ctor 0x0099EBF0 / dtor 0x0099EAE0, map<CCharString,bool>::operator[] 0x008ADF10, CScriptThing
//   default ctor 0x006E7B40 / dtor 0x004AA840 / slot 6 GetPos, vector<CScriptThing>::resize(n, const T&) 0x00D34AC0
//   (the one-argument resize is inline), CScriptBase::IsActiveThreadTerminating 0x00CB7940; CGameScriptInterface slots
//   7 NewScriptFrame, 75 GetAllThingsWithScriptName, 100 CreateEffect, 108 RemoveThing. The by-value CreateEffect result is
//   assigned with CScriptThing's implicit operator= (CCountedPointer::operator= expanded inline).
//   Literals "fire" 0x012C1978, "Q_REVISITED_FIREPOINT" 0x012F5458, "FirePoints" 0x012F544C, "OAKVALE_BURNING_PATCH" 0x012F5434.
// Shape: retail expands CScriptThing::operator= here although its out-of-line copy exists; the lane only inlines it
//   with __forceinline (CCountedPointer::operator= reads the source block then the object first, the 0x008AB980 form
//   reversed). Element access is *(begin() + i): the start pointer is read before the CreateEffect call, the index added after.
// Layout: Ego_r.pdb CCS_OakValeRevisitedScript - 0x14 (retail CScriptBase is 0x40): FirePoint +0x48, Fires +0x54,
//   OakValeFlag +0x60 (map<CCharString,bool>), v1..v7 handles +0x78.. (GSI +0x40, Owner +0x44).
class C3DVector { public: float X, Y, Z; };
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};
void operator delete(void* p);                         // 0x00BFE9BC (MSVCR71)
class CCPPointerInfo { public:                                // CCountedPointer's shared block
    long RefCount;                                     // +0
    void (__fastcall* DeleteFunc)(void* data);         // +4
    void* Data;                                        // +8
};
template <class T> class CCountedPointer { public:
    T*              Data;
    CCPPointerInfo* Info;
    CCountedPointer& operator=(const CCountedPointer& other)
    {
        CCPPointerInfo* info = other.Info;
        T* data = other.Data;
        if (Info != info) {
            if (Info) {
                --Info->RefCount;
                if (Info->RefCount == 0) {
                    Info->DeleteFunc(Info->Data);
                    delete Info;
                }
            }
            Data = data;
            Info = info;
            if (info)
                ++info->RefCount;
        }
        return *this;
    }
};
struct CBase {
    virtual ~CBase();                                  // 0x0099A2E0
};
namespace N_006e7b40 { struct CScriptThing : CBase {                          // vtable 0x01238C8C, 12 bytes
    virtual void t001(); virtual void t002(); virtual void t003(); virtual void t004(); virtual void t005();
    virtual const C3DVector& GetPos() const;           // slot 6 (+0x18)
    CCountedPointer<void> PImp;                        // +0x04 Data, +0x08 Info
    CScriptThing();                                    // 0x006E7B40
    CScriptThing(const CScriptThing& other);           // 0x004ABE90
    ~CScriptThing();                                   // 0x004AA840
    __forceinline CScriptThing& operator=(const CScriptThing& other) { PImp = other.PImp; return *this; }   // inline here (out-of-line copy 0x008AB980)
}; } using N_006e7b40::CScriptThing;
namespace std {
template <class T> class allocator;
template <class T, class A = allocator<T> > class vector {
public:                     // STLport vector (retail 12 bytes)
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    unsigned int size() const { return _M_finish - _M_start; }
    T* begin() { return _M_start; }
    T& operator[](unsigned int i) { return *(begin() + i); }
    void resize(unsigned int n, const T& value);       // 0x00D34AC0
    void resize(unsigned int n) { resize(n, T()); }
};
}
using std::vector;
struct CBoolMap {                                      // map<CCharString, bool>
    char _M_t[0xc];
    bool& operator[](const CCharString& key);          // 0x008ADF10
};
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
// CScriptBase: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40).
#include "engine/CScriptBase.h"
struct CCS_OakValeRevisitedScript : CScriptBase {
    CGameScriptInterface* GSI;                         // +0x40
    void*                 Owner;                       // +0x44
    vector<CScriptThing>  FirePoint;                   // +0x48
    vector<CScriptThing>  Fires;                       // +0x54
    CBoolMap              OakValeFlag;                 // +0x60
    void OakValeFire();
};

void CCS_OakValeRevisitedScript::OakValeFire()
{
    while (OakValeFlag["fire"] != true) {
        GSI->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    GSI->GetAllThingsWithScriptName("Q_REVISITED_FIREPOINT", FirePoint);
    Fires.resize(FirePoint.size());
    for (unsigned int i = 0; i < FirePoint.size(); ++i) {
        if (IsActiveThreadTerminating())
            return;
        Fires[i] = GSI->CreateEffect("OAKVALE_BURNING_PATCH", FirePoint[i].GetPos(), "FirePoints", 0.0f, false, false);
    }
    if (IsActiveThreadTerminating())
        return;
    while (OakValeFlag["fire"] != false) {
        GSI->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    Fires.resize(FirePoint.size());
    for (unsigned int j = 0; j < Fires.size(); ++j) {
        if (IsActiveThreadTerminating())
            return;
        GSI->RemoveThing(Fires[j], false, true);
    }
    if (IsActiveThreadTerminating())
        return;
}