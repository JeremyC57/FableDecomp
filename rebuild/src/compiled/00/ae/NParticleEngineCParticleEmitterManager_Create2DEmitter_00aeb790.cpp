// NParticleEngine::CParticleEmitterManager::Create2DEmitter(long handle, const C2DVector& position, EEngine2DLayer layer, bool start,
// const CRightHandedSet& orientation) -- fetch the emitter template; if it is a 2D emitter, clone it into the active list (a
// counted-pointer local), put it at the screen position on the layer and orient it; null otherwise.
// Leaf: ?Create2DEmitter@CParticleEmitterManager@NParticleEngine@@QAEPAVCParticleEmitter@2@JABVC2DVector@@W4EEngine2DLayer@@_NABVCRightHandedSet@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AEB790, 171 bytes, `ret 14h`.
// Identity: Ego_r.pdb / FableWin.pdb, the spelling above.
// Callees: CParticleEmitterDatabase::GetEmitterTemplate 0x00AE89E0 on NParticleEngine::PEmitterDatabase 0x013D2E14,
//   CParticleEmitter::DatabaseClone 0x00AF0650 / Set2DPosition 0x00AED090 / SetOrientation 0x00AEDE60,
//   CCountedPointer<CParticleEmitter>(CParticleEmitter*) 0x00AE9450 (called COMDAT), CParticleEmitterList::Insert 0x00AEB060,
//   C3DOrientationInterpolator ctor 0x005508D0, ~CBaseClass 0x0099A2E0, operator delete 0x00BFE9BC (the inline counted release).
// Layout: the manager starts with its CParticleEmitterList (Insert takes `this`); CParticleEmitter Emitter2D = bit 11 of +0x40.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_CPARTICLEEMITTER
#define FABLE_RETAIL_CPARTICLEEMITTER_CREATE2D
#define FABLE_RETAIL_CPARTICLEEMITTER_KILL
#define FABLE_RETAIL_CPARTICLEEMITTER_GETPRIORITY
#define FABLE_RETAIL_C3DORIENTATIONINTERPOLATOR_IMPLICIT_DTOR
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_LIST_SIZE_DISTANCE
#define FABLE_RETAIL_CPARTICLEEMITTERLIST_INSERT
#define FABLE_RETAIL_CPARTICLEEMITTERMANAGER_CREATE2D
#define FABLE_RETAIL_CCOUNTEDPOINTER_VISIBLE_CALLED_CTOR
#define FABLE_RETAIL_CPARTICLEEMITTERDATABASE_GETEMITTERTEMPLATE
#include "engine/C2DVector.h"
#include "engine/C3DVector.h"
#include "engine/CRightHandedSet.h"
#include "engine/C3DOrientationInterpolator.h"
#include "engine/CCountedPointer.h"
#include "engine/CParticleEmitter.h"
#include "engine/CParticleEmitterManager.h"
#include "engine/CParticleEmitterDatabase.h"
namespace NParticleEngine {
extern CCountedPointer<CParticleEmitterDatabase> PEmitterDatabase;      // 0x013D2E14 (the name the landed Initialise 0x00AEBE20 binds)
}

// The real body of CParticleEmitterList::Insert (landed 0x00AEB060, NParticleEngineCParticleEmitterList_Insert_00aeb060.cpp), ported
// onto engine/CParticleEmitterList.h and the shared STLport list: out of line in this TU as in retail (0x00AEB060 precedes the row in
// the same object), so VC7.1 sees that it keeps neither the counted pointer's address nor its block.
#pragma auto_inline(off)
bool NParticleEngine::CParticleEmitterList::Insert(CCountedPointer<CParticleEmitter>& emitter)
{
    if (Active.size() == 0) {
        Active.push_back(emitter);
        return true;
    }
    for (std::list<CCountedPointer<CParticleEmitter> >::iterator it = Active.begin(); it != Active.end(); ) {
        if ((*it)->IsDead())
            it = Active.erase(it);
        else {
            if ((*it)->GetPriority() < emitter->GetPriority()) {
                if (it == Active.begin()) {
                    Active.push_front(emitter);
                    return true;
                }
                --it;
                Active.insert(it, emitter);
                return true;
            }
            ++it;
        }
    }
    if ((long)Active.size() < MaxActiveEmitters) {
        Active.push_back(emitter);
        return true;
    }
    return false;
}
#pragma auto_inline(on)

NParticleEngine::CParticleEmitter* NParticleEngine::CParticleEmitterManager::Create2DEmitter(long handle, const C2DVector& position,
    EEngine2DLayer layer, bool start, const CRightHandedSet& orientation)
{
    CParticleEmitter* emitter_template = PEmitterDatabase->GetEmitterTemplate(handle, start);
    CParticleEmitter* result = 0;
    if (emitter_template && emitter_template->Is2DEmitter()) {
        CCountedPointer<CParticleEmitter> emitter(emitter_template->DatabaseClone());
        if (ActiveEmitters.Insert(emitter)) {
            result = emitter.Get();
            result->Set2DPosition(position, layer);
            result->SetOrientation(C3DOrientationInterpolator(orientation, orientation));
        }
    }
    return result;
}