// NScript::CPersonalScript_EmptyScript::CVillager::Init() -- looks up the "ScrumpyCrate" thing into the villager entity script's Crate handle
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EF49A0, 191 bytes (padded to 192), plain `ret`. Slot 3 of vtable 0x012F6514.
//   PDB: ?Init@CVillager@CPersonalScript_EmptyScript@NScript@@EAEXXZ is the only CVillager::Init in Ego_r.pdb; its layout
//   (Crate CScriptThing at +0x1c) matches the stores. The owning quest is not proven (several scripts nest a CVillager).
// Callees: CGameScriptInterface slot 72 GetThingWithScriptName(const CCharString&) (+0x120, through the entity script's
//   interface pointer at +4), CCharString::CCharString(const char*, long) 0x0099EBF0, ~CCharString 0x0099EAE0,
//   ~CBaseClass 0x0099A2E0, operator delete 0x00BFE9BC. String "ScrumpyCrate" 0x012F5F24.
// Shape: as CV_MayorsInvitationScript::Init 0x00EB6BD0: implicit conversion temporary for the name, inline counted
//   operator= into the member, then the returned temporary's inline destructor (vtable re-store, release, zeroes,
//   base destructor; the zeroes become immediates once the callee-saved registers are popped).
// Layout (Ego_r.pdb CVillager, entity scripts match the PDB): interface +0x04, ParentClass +0x14, PMasterData +0x18,
//   Crate +0x1c (PImp Data +0x20, Info +0x24).
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
void operator delete(void* p);                     // 0x00BFE9BC (MSVCR71)
class CCPPointerInfo { public:                            // CCountedPointer's shared block
    long RefCount;
    void (__fastcall* DeleteFunc)(void* data);
    void* Data;
};
template <class T> class CCountedPointer { public:
    T*              Data;
    CCPPointerInfo* Info;
    ~CCountedPointer()
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
class CBaseClass { public:                                // polymorphic root of the script handles
    CBaseClass();                                  // 0x0099A2D0
    virtual ~CBaseClass();                         // 0x0099A2E0
};
class CScriptThing : public CBaseClass { public:                 // 12 bytes, vtable 0x01238C8C
    CCountedPointer<CScriptThing> PImp;            // +0x04 Data, +0x08 Info
    ~CScriptThing() { }
};
class CGameScriptInterface { public:
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007(); virtual void v008(); virtual void v009();
    virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019();
    virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023(); virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029();
    virtual void v030(); virtual void v031(); virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039();
    virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047(); virtual void v048(); virtual void v049();
    virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055(); virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059();
    virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063(); virtual void v064(); virtual void v065(); virtual void v066(); virtual void v067(); virtual void v068(); virtual void v069();
    virtual void v070(); virtual void v071();
    virtual CScriptThing GetThingWithScriptName(const CCharString& name);   // slot 72 (+0x120)
};
namespace NScript {
class CPersonalScript_EmptyScript { public:
    struct CVillager {
        virtual void v00();
        CGameScriptInterface* Game;                // +0x04
        unsigned char _base08[0x0c];               // +0x08 (entity script base)
        void* ParentClass;                         // +0x14
        void* PMasterData;                         // +0x18
        CScriptThing Crate;                        // +0x1c
        virtual void Init();
    };
};
void CPersonalScript_EmptyScript::CVillager::Init()
{
    Crate = Game->GetThingWithScriptName("ScrumpyCrate");
}
}