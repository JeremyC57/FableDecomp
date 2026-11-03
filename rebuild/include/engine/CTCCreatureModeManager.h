#ifndef FABLE_ENGINE_CTCCREATUREMODEMANAGER_H
#define FABLE_ENGINE_CTCCREATUREMODEMANAGER_H
#include "engine/CTCBase.h"
#include "engine/CCountedPointer.h"
#include "stlport/fable_containers.h"
// Raw TPI0x27a12 and nested0x28568; native0073a6e0 reads front mode of PModes+0c.
class CTCCreatureModeManager : public CTCBase {
public:
    static ETCInterfaceType __fastcall StatGetInterfaceType() { return TCI_ENTITY_MODE_MANAGER; }
    class CModeBase {
    public:
        virtual ~CModeBase();
        virtual unsigned long GetType() const=0; // Native slot1, used by006680f5.
    protected:
        CThing& ParentThing;
        unsigned char WeaponBasedFilter[12]; // Original CAnimFilterOfGroup.
        long FramesBeforeRemovingSelf;bool Active;
    };
    CModeBase& GetCurrentMode() const;
    bool IsModeActive(unsigned long) const;
private:
    std::list<CCountedPointer<CModeBase> > PModes;
    long TimeLeftForCurrentMode,TimeForNextIdleMode;
    unsigned char AvailableIdleModes[12]; // std::vector<NCreatureMode::EMode>.
};
FABLE_STATIC_ASSERT(sizeof(CTCCreatureModeManager)==0x24);

#endif
