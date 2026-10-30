// NScript::CV_ChickenKickingScript::CChickenSign::Main() -- makes the chicken-kicking sign readable with the text for the current final sector, then each frame re-labels it whenever the sector changes until the sector reaches 7
// Leaf: ?Main@CChickenSign@CV_ChickenKickingScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E64DF0, 391 bytes (400 with padding), `ret`; entity-script vtable slot 1 (Main).
// Callees: CGameScriptInterface (retail vtable 0x01260F0C) slots 7 NewScriptFrame, 72 GetThingWithScriptName (a CScriptThing
//   by hidden pointer, "ChickenSign" 0x012E73BC as an implicit CCharString temporary), 447 SetThingAsUsable, 713
//   SetReadableObjectText; the script base's IsActiveThreadTerminating 0x00F35B30; the CScriptThing temporary's inline
//   destructor (vtable 0x01238C8C store, counted release through the info block, operator delete 0x00BFE9BC,
//   CBaseClass::~CBaseClass 0x0099A2E0) at both sites; ~CCharString 0x0099EAE0.
// Shape: the landed WatchForLeaderKilled CScriptThing/counted-pointer model; the text key is TextKeys[ParentClass->FinalSector]
//   through an STLport operator[] written `*(begin() + n)` (the vector start loads before the index; `_M_start[n]` or
//   `*(_M_start + n)` load the index first); a rotated `while (sector != 7)` with the trailing IsActiveThreadTerminating()
//   whose result is unused.
// Layout: local model -- Ego_r.pdb CChickenSign (44 bytes): Game +4, Me +8, ParentClass +0x14, PMasterData +0x18,
//   TextKeys vector<CWideString> +0x1c; CV_ChickenKickingScript FinalSector retail +0x5c (PDB +0x60, compared with 7).
namespace NScript { class CPredicateBase; }
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
class CWideString { public: void* PStringData; };
class CCPPointerInfo { public:                                 // counted-pointer block (Ego_r.pdb)
    unsigned long RefCount;                             // +0x00
    void (__fastcall* DeleteFunc)(void* data);          // +0x04
    void* Data;                                         // +0x08
};
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
class CScriptThing;
struct CCountedPointerScriptThing {                     // CCountedPointer<CScriptThing>
    CScriptThing*   Data;                               // +0x00
    CCPPointerInfo* Info;                               // +0x04
    ~CCountedPointerScriptThing()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
        Data = 0;
        Info = 0;
    }
};
class CScriptThing : public CBaseClass { public:                      // 12 bytes, vtable 0x01238C8C
    CCountedPointerScriptThing PImp;                    // +0x04
    virtual ~CScriptThing() {}                          // out of line: 0x004AA840
};
template <class T> struct CVector {                     // STLport vector<T> (12 bytes)
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    T* begin() { return _M_start; }
    T& operator[](unsigned int n) { return *(begin() + n); }
};
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
class CEntityScriptBase
{
public:
        virtual void Main();
public:
    bool IsActiveThreadTerminating() const;   // 0x00F35B30
};

namespace NScript {
class CQ_SunnyvaleMasterData;
class CV_ChickenKickingScript { public:
    unsigned char _pad00[0x5c];
    long FinalSector;                                   // +0x5c (Ego_r.pdb +0x60)
    class CChickenSign : public CEntityScriptBase {
    public:
        CGameScriptInterface*    Game;                  // +0x04
        CScriptThing             Me;                    // +0x08
        CV_ChickenKickingScript* ParentClass;           // +0x14
        CQ_SunnyvaleMasterData*  PMasterData;           // +0x18
        CVector<CWideString>     TextKeys;              // +0x1c
    private:
        virtual void Main();
    };
};

void CV_ChickenKickingScript::CChickenSign::Main()
{
    Game->SetThingAsUsable(Me, true);
    Game->SetReadableObjectText(Game->GetThingWithScriptName("ChickenSign"), TextKeys[ParentClass->FinalSector]);
    long sector = ParentClass->FinalSector;
    while (sector != 7) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
        if (sector != ParentClass->FinalSector) {
            if (IsActiveThreadTerminating())
                return;
            Game->SetReadableObjectText(Game->GetThingWithScriptName("ChickenSign"), TextKeys[ParentClass->FinalSector]);
            sector = ParentClass->FinalSector;
        }
    }
    IsActiveThreadTerminating();
}
}