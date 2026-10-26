// NScript::CV_ChapelOfEvilScript::CCOE_AcolyteEvil::Alloc(CV_ChapelOfEvilScript& parent, CScriptDataBase* data, const CScriptThing& me) -- entity-script factory of the "COE_AcolyteEvil" binding: new CCOE_AcolyteEvil(parent, data, me) (CEntityScriptBase(*parent.Game, me), ParentClass = &parent, PMasterData = data) handed to a fresh CCountedPointer<CEntityScriptBase>
// Leaf: ?Alloc@CCOE_AcolyteEvil@CV_ChapelOfEvilScript@NScript@@SI?AV?$CCountedPointer@VCEntityScriptBase@@@@AAV23@PAVCScriptDataBase@@ABVCScriptThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00E6A500, 153 bytes (+ int3 padding), static `__fastcall` (hidden result
//   pointer ecx, parent edx, data and me on the stack), `ret 8`, returns the result pointer.
// Identity: Ego_r.pdb ?Alloc@CCOE_AcolyteEvil@CV_ChapelOfEvilScript@NScript@@SI?AV?$CCountedPointer@VCEntityScriptBase@@@@AAV23@PAVCScriptDataBase@@ABVCScriptThing@@@Z is exactly the symbol this source emits.
//   CV_ChapelOfEvilScript::Main 0x00E693F0 (slot 2 of the script vtable 0x012E8080, which the "V_ChapelOfEvil" script's Alloc stores at its CScriptInfoManager::RegisterScripts registration) stores this address as the factory of the CEntityScriptBinding<CV_ChapelOfEvilScript> record named "COE_AcolyteEvil" (site 0x00E6943F).
//   Was landed as _global::Create_CVtable_012e8468 (named by the entity vtable address); same shape as the family-pass F014 Alloc rows.
// Callees: scalar operator new 0x00BFEA1A (0x20 = sizeof(CCOE_AcolyteEvil), then 0x0C for the count block),
//   CScriptThing::CScriptThing(const CScriptThing&) 0x004ABE90 (Me), CTimer::CTimer() 0x00CD4450 (AcolytePrayerTimer +0x1C), and the count block's deleter
//   0x00CDEE00 (stored, not called: `if (p) p->~CEntityScriptBase()` deleting, `__fastcall`).
// Data: vtables CEntityScriptBase 0x012C3224 (stored before the Me copy) and CCOE_AcolyteEvil 0x012E8468.
// Layout: Ego_r.pdb CV_ChapelOfEvilScript::CCOE_AcolyteEvil (32 bytes = the retail new size): ParentClass +0x14, PMasterData +0x18, AcolytePrayerTimer +0x1C.
//   CEntityScriptBase: vptr, Game +0x04, Me +0x08 (Ego_r 20 bytes). The parent's Game is CLevelScriptBase's +0x40.
// Shape: the PDB ctor (parent, data, me) inlined: base and ParentClass in the init list, PMasterData assigned in the body;
//   STLport-free counted pointer ctor: Data first, the null test on the argument, the 12-byte {1, deleter, Data} block
//   read back from Data, three exits (block, failed new, null object).
class CGameScriptInterfaceBase;
class CScriptDataBase;

void __fastcall Destroy_00cdee00(void* object);        // 0x00CDEE00 (catalog name; the CEntityScriptBase deleter)

class CCPPointerInfo {                                  // 12 bytes
public:
    unsigned long RefCount;
    void (__fastcall* DeleteFunc)(void*);
    void* Data;
    CCPPointerInfo(void* data, void (__fastcall* deleteFunc)(void*)) : RefCount(1), DeleteFunc(deleteFunc), Data(data) { }
};

template <class T> class CCountedPointer {
public:
    T* Data;
    CCPPointerInfo* Info;
    CCountedPointer(T* data) : Data(data)
    {
        if (data)
            Info = new CCPPointerInfo(Data, Destroy_00cdee00);
        else
            Info = 0;
    }
};

class CScriptThing {                                    // 12 bytes (Ego_r): vptr, PImp
public:
    virtual ~CScriptThing();
    CCountedPointer<CScriptThing> PImp;                 // +0x04
    CScriptThing();                                     // 0x006E7B40 (base ctor 0x0099A2D0, vtable 0x01238C8C, PImp nulled)
    CScriptThing(const CScriptThing& other);            // 0x004ABE90
};
class CTimer {                                          // 4 bytes (Ego_r)
public:
    long TimerIndex;
    CTimer();                                           // 0x00CD4450
};

class CEntityScriptBase {                               // 20 bytes (Ego_r)
public:
    CEntityScriptBase(CGameScriptInterfaceBase& game, const CScriptThing& me) : Game(&game), Me(me) { }
    virtual ~CEntityScriptBase();
    CGameScriptInterfaceBase* Game;                     // +0x04
    CScriptThing Me;                                    // +0x08
};

class CScriptBase {                                     // retail 0x40 bytes
public:
    virtual ~CScriptBase();
    unsigned char _body[0x3c];
};
template <class TData> class CLevelScriptBase : public CScriptBase {
public:
    CGameScriptInterfaceBase* Game;                     // +0x40
    TData* MasterScriptData;                            // +0x44
};

namespace NScript {
class CQ_SunnyvaleMasterData;
class CV_ChapelOfEvilScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    class CCOE_AcolyteEvil : public CEntityScriptBase {
    public:
        CV_ChapelOfEvilScript* ParentClass;             // +0x14
        CQ_SunnyvaleMasterData* PMasterData;            // +0x18
        CTimer AcolytePrayerTimer;                      // +0x1C (Ego_r)
        CCOE_AcolyteEvil(CV_ChapelOfEvilScript& parent, CScriptDataBase* data, const CScriptThing& me)
            : CEntityScriptBase(*parent.Game, me), ParentClass(&parent)
        {
            PMasterData = (CQ_SunnyvaleMasterData*)data;
        }
        virtual ~CCOE_AcolyteEvil();
        static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CV_ChapelOfEvilScript& parent, CScriptDataBase* data, const CScriptThing& me);
    };
};

CCountedPointer<CEntityScriptBase> __fastcall CV_ChapelOfEvilScript::CCOE_AcolyteEvil::Alloc(CV_ChapelOfEvilScript& parent, CScriptDataBase* data, const CScriptThing& me)
{
    return CCountedPointer<CEntityScriptBase>(new CCOE_AcolyteEvil(parent, data, me));
}
}
