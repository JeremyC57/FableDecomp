#ifndef FABLE_ENGINE_CPERSONALSCRIPT_ALEXTESTSCRIPT_H
#define FABLE_ENGINE_CPERSONALSCRIPT_ALEXTESTSCRIPT_H
// Original Ego_r TPI101c2/101f2; original private SpeakCounter1c, three nested classes.
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CLevelScriptBase.h"
#include "engine/CEntityScriptBase.h"
namespace NScript {
class CalextestData;
class CPersonalScript_alextestScript : public CLevelScriptBase<CalextestData> {
public:
    virtual void RegisterMain();
    virtual void Init();
    virtual void Main();
    class CStopDrawing : public CEntityScriptBase {
    public:
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CPersonalScript_alextestScript&,CScriptDataBase*,const CScriptThing&);
        virtual CScriptBase* GetParentScript() const;
        CalextestData* GetMasterData();
    protected:
        CPersonalScript_alextestScript& ParentClass;
        CalextestData* PMasterData;
    private:
        virtual void Init();
        virtual void Main();
    };
    class CSignpost : public CEntityScriptBase {
    public:
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CPersonalScript_alextestScript&,CScriptDataBase*,const CScriptThing&);
        virtual CScriptBase* GetParentScript() const;
        CalextestData* GetMasterData();
    protected:
        CPersonalScript_alextestScript& ParentClass;
        CalextestData* PMasterData;
    private:
        virtual void Init();
        virtual void Main();
    };
    class CBurningPerson : public CEntityScriptBase {
    public:
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CPersonalScript_alextestScript&,CScriptDataBase*,const CScriptThing&);
        virtual CScriptBase* GetParentScript() const;
        CalextestData* GetMasterData();
    protected:
        CPersonalScript_alextestScript& ParentClass;
        CalextestData* PMasterData;
    private:
        long SpeakCounter;
        virtual void Init();
        virtual void OnPersist(CPersistContext&);
        virtual void OnPredicateFail();
        virtual void Main();
    };
};
}
FABLE_STATIC_ASSERT(sizeof(NScript::CPersonalScript_alextestScript)==0x48);
FABLE_STATIC_ASSERT(sizeof(NScript::CPersonalScript_alextestScript::CBurningPerson)==0x20);
#endif
