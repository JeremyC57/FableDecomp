#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "engine/C3DVector.h"

// Test doubles for the world, factory and emitter dependencies of the selected member body.
#define FABLE_ENGINE_CTHING_H
#define FABLE_ENGINE_CTHINGMANAGER_H
#define FABLE_ENGINE_CWORLD_H
#define FABLE_ENGINE_CWORLDMAP_H
#define FABLE_ENGINE_CMAINGAMECOMPONENT_H
#define FABLE_ENGINE_CGAMEDEFINITIONMANAGER_H
#define FABLE_ENGINE_CTCDPARTICLEEMITTER_H
#define FABLE_ENGINE_CPARTICLEEMITTER_H

namespace
{
struct Calls
{
    long MapNumber;
    bool MapLoading;
    bool EmitterPresent;
    int LoadingChecks;
    int MapChecks;
    int DefLookups;
    int Creations;
    int DriverLookups;
    int Constructions;
    int Activations;
    int Kills;
    bool ForcedConstruction;
};
Calls calls;

void Reset(long map_number, bool loading, bool emitter_present)
{
    calls = Calls();
    calls.MapNumber = map_number;
    calls.MapLoading = loading;
    calls.EmitterPresent = emitter_present;
}

void CheckPosition(const C3DVector& pos)
{
    assert(pos.X == 3.0f && pos.Y == -2.0f && pos.Z == 6.0f);
}
}

class CThing;

class CTCDParticleEmitter
{
public:
    static CThing* FABLE_FASTCALL Create(long particle_type_id, const C3DVector& pos, bool force);
private:
    void Construct(long particle_type_id, const C3DVector& pos, bool independent, bool force)
    {
        assert(particle_type_id == 123 && !independent);
        CheckPosition(pos);
        calls.ForcedConstruction = force;
        ++calls.Constructions;
    }

    class EmitterPointer
    {
    public:
        const void* Get() const { return calls.EmitterPresent ? this : 0; }
    };

    EmitterPointer PEmitter;
};

class CWorldMap
{
public:
    bool IsLoadingAnyMap() const
    {
        ++calls.LoadingChecks;
        return calls.MapLoading;
    }

    long GetMapNumberAt(const C3DVector& pos) const
    {
        CheckPosition(pos);
        ++calls.MapChecks;
        return calls.MapNumber;
    }
};

class CWorld
{
public:
    const CWorldMap& PeekWorldMap() const { return Map; }
private:
    CWorldMap Map;
};

class CMainGameComponent
{
public:
    static CMainGameComponent* Get()
    {
        static CMainGameComponent component;
        return &component;
    }
    const CWorld& PeekWorld() const { return World; }
private:
    CWorld World;
};

class CThingManager
{
public:
    const CWorldMap& PeekWorldMap() const { return Map; }
private:
    CWorldMap Map;
};

class CGameDefinitionManager
{
public:
    static CGameDefinitionManager& Get()
    {
        static CGameDefinitionManager manager;
        return manager;
    }

    long GetDefGlobalIndexFromName(const char* name) const
    {
        assert(strcmp(name, "PARTICLE_EMITTER_NORMAL") == 0);
        ++calls.DefLookups;
        return 101;
    }
};

class CThing
{
public:
    static CThing* Create(long def, long player, long map_number, bool level_persistent)
    {
        assert(def == 101 && player == 4 && map_number == calls.MapNumber && level_persistent);
        ++calls.Creations;
        static CThing thing;
        return &thing;
    }

    static const CThingManager* PeekPCurrentThingManager()
    {
        static CThingManager manager;
        return &manager;
    }

    void GetTCDriver(CTCDParticleEmitter** emitter)
    {
        ++calls.DriverLookups;
        *emitter = &Emitter;
    }

    void DoActivationSequence() { ++calls.Activations; }

    void Kill(bool immediately)
    {
        assert(!immediately && calls.Activations == 1);
        ++calls.Kills;
    }
private:
    CTCDParticleEmitter Emitter;
};

// Leaf: ?Create@CTCDParticleEmitter@@SIPAVCThing@@JABVC3DVector@@_N@Z
// Flags:
#define FABLE_RETAIL_CTCDPARTICLEEMITTER_FULL
#define FABLE_RETAIL_CPARTICLEEMITTER
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_WORLD
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLDMAP
#define FABLE_RETAIL_CWORLDMAP_MOVEPLAYERPOS
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_MOREACCESS
#define FABLE_RETAIL_CTHING_STATEACCESS
#define FABLE_RETAIL_CTHING_MANAGERACCESS
#define FABLE_RETAIL_CTHING_GETTCDRIVER
#define FABLE_RETAIL_CTHINGMANAGER_WORLDMAP
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_CCLASSFACTORY
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTC_DRIVER_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/C3DVector.h"
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CThingManager.h"
#include "engine/CWorld.h"
#include "engine/CWorldMap.h"
#include "engine/CMainGameComponent.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CTCDParticleEmitter.h"
#include "engine/CParticleEmitter.h"

CThing* FABLE_FASTCALL CTCDParticleEmitter::Create(long particle_type_id, const C3DVector& pos, bool force)
{
    if (!force && CMainGameComponent::Get()->PeekWorld().PeekWorldMap().IsLoadingAnyMap())
        force = true;
    CThing* out_pthing = 0;
    long map_no = CThing::PeekPCurrentThingManager()->PeekWorldMap().GetMapNumberAt(pos);
    if (map_no > 0) {
        long def_global_index = CGameDefinitionManager::Get().GetDefGlobalIndexFromName("PARTICLE_EMITTER_NORMAL");
        CThing* pthing = CThing::Create(def_global_index, 4, map_no, true);
        CTCDParticleEmitter* ptcd = 0;
        pthing->GetTCDriver(&ptcd);
        ptcd->Construct(particle_type_id, pos, false, force);
        pthing->DoActivationSequence();
        if (ptcd->PEmitter.Get() == 0 && force)
            pthing->Kill(false);
        else
            out_pthing = pthing;
    }
    return out_pthing;
}


namespace
{
void CheckCreated(bool force, bool killed)
{
    assert(calls.MapChecks == 1 && calls.DefLookups == 1);
    assert(calls.Creations == 1 && calls.DriverLookups == 1);
    assert(calls.Constructions == 1 && calls.Activations == 1);
    assert(calls.Kills == (killed ? 1 : 0));
    assert(calls.ForcedConstruction == force);
}
}

int main()
{
    const C3DVector pos(3.0f, -2.0f, 6.0f);

    Reset(0, false, false);
    assert(CTCDParticleEmitter::Create(123, pos, false) == 0);
    assert(calls.LoadingChecks == 1 && calls.MapChecks == 1 && calls.Creations == 0);
    Reset(-2, true, true);
    assert(CTCDParticleEmitter::Create(123, pos, true) == 0);
    assert(calls.LoadingChecks == 0 && calls.MapChecks == 1 && calls.Creations == 0);

    Reset(7, false, false);
    assert(CTCDParticleEmitter::Create(123, pos, false) != 0);
    assert(calls.LoadingChecks == 1);
    CheckCreated(false, false);

    Reset(7, false, false);
    assert(CTCDParticleEmitter::Create(123, pos, true) == 0);
    assert(calls.LoadingChecks == 0);
    CheckCreated(true, true);

    Reset(7, true, true);
    assert(CTCDParticleEmitter::Create(123, pos, false) != 0);
    assert(calls.LoadingChecks == 1);
    CheckCreated(true, false);

    Reset(7, true, false);
    assert(CTCDParticleEmitter::Create(123, pos, false) == 0);
    assert(calls.LoadingChecks == 1);
    CheckCreated(true, true);
    puts("006e0880_TEST PASS");
}