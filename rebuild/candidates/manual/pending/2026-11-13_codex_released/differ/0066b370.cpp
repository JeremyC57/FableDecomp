// CTCSoundPlayer::PlayAllSoundEvents() -- play the sound events of the parent's current animation events
// Grade: DIFFER(1805v1808) -- 439 diff lines, all register colouring / CSE: this lands in edi (retail esi; the Components pointer swaps to edi), the theme and material CDefPointer releases come out load/dec/store (retail in-place dec [ecx+4]), sound_def's Object is not CSE'd into a register before the GetSubDef branch; control flow, frame 0x74, calls and the 4x-unrolled best-alpha loop agree
// Next idea: a CDefPointer release spelling (header opt-in) that VC7.1 folds into dec [ecx+4] at these two sites (if(!--count) tried, identical), then re-check whether this moves back to esi once those registers free up
// Leaf: ?PlayAllSoundEvents@CTCSoundPlayer@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0066B370, 1808 bytes, thiscall, no args. Identity: Ego_r.pdb ?PlayAllSoundEvents@CTCSoundPlayer@@QAEXXZ
//   (0x005749A0) and FableWin.pdb (0x01E7E870); the sole retail caller is CTCSoundPlayer::FrameUpdate's tail jump; the body reads the
//   CTCSoundPlayer members (AnimCriteriaClipDistance, LastMaterialIndex, VoiceName, ExtraTag, PAttachedEmitter) at their PDB offsets.
// Callees (exact PDB spellings; objdump -r paired with retail): GFWorld3DDistanceSmallerThan 0x006614F0, CTCAnimationComplex::GetAnimation
//   0x0070B460 (Ego_r 0x0057A430), CWorldMap::GetStrongestEngineThemeAt(const C3DVector&, long&) const 0x0063ADF0 (Ego_r 0x006E84E0, AAJ: the
//   landed PAJ spellings are wrong, NAME_FIXES.tsv), CAnimateThing::GetAllCombinationAnims 0x0070D100 (Ego_r 0x005764A0; landed label
//   "CGraphicBankManager::ProcessLoading" is wrong), NAnimationEvents::GetSoundEventManager 0x006F90C0 (Ego_r 0x0071E740),
//   CManager::GetAnimEventsBetweenTimes 0x006F99B0 (Ego_r 0x0071F0C0), CTCSoundPlayer::InternalPlaySoundEvent 0x0066B120,
//   CSymbolMap::FindFirstSymbolWithVal, CCharString helpers. Globals: g_bSoundEnabled 0x0137E974, g_InvalidSoundHandle 0x0129D25C (both
//   established landed spellings), g_bPlayAnimationSoundEvents 0x01375758 (DESCRIPTIVE: a .data bool = 1 read only here; no PDB name found).
//   CThingSurfaceView / CSoundEventHandlerView are descriptive vtable-slot views (no header models those slots yet: header debt in NOTES.md).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_MOREACCESS
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CMAINGAMECOMPONENT_WORLD
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CSYMBOLMAP
#define FABLE_RETAIL_CSYMBOLMAP_FINDBYVALUE
#define FABLE_RETAIL_CCHARSTRING_CONCAT_STRING
#define FABLE_RETAIL_CTCSOUNDPLAYER_PLAYALLSOUNDEVENTS
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/C3DVector.h"
#include "engine/CDefPointer.h"
#include "engine/CDefClassBase.h"
#include "engine/CDefString.h"
#include "engine/CDefinitionManager.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CTCBase.h"
#include "engine/CThing.h"
#include "engine/CThingBaseDef.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CThingSoundEmitter.h"
#include "engine/CWorld.h"
#include "engine/CMainGameComponent.h"
#include "engine/CSymbolMap.h"
#include "engine/CTCSoundPlayer.h"
class CASoundBank;
class CWorldMap {
public:
    long GetStrongestEngineThemeAt(const C3DVector& pos, long& strength) const;   // 0x0063ADF0
};
extern bool g_bSoundEnabled;                       // 0x0137E974
extern bool g_bPlayAnimationSoundEvents;           // 0x01375758 (descriptive: .data bool = 1, read only here)
extern const unsigned long g_InvalidSoundHandle;   // 0x0129D25C
bool __fastcall GFWorld3DDistanceSmallerThan(const C3DVector& a, const C3DVector& b, float distance);   // 0x006614F0
class CEntitySoundDef : public CDefClassBase {
public:
    unsigned char _pad[0x48 - sizeof(CDefClassBase)];
    CCharString SoundIdentifier;                   // +0x48
};
class CEngineThemeDef : public CDefClassBase {
public:
    unsigned char _pad[0x8c - sizeof(CDefClassBase)];
    long MaterialDef;                              // +0x8c
};
class CMaterialDef : public CDefClassBase {
};
class CAnimSet;
class CTransitionBlendAlpha {
public:
    float Alpha;
    long  Data;
};
class CAnimSet {
public:
    long UpdateMode;                               // +0x00
    std::vector<short> Anims;                      // +0x04
    std::vector<CTransitionBlendAlpha> InternalAlphas;   // +0x10
    float CurrentPercentageComplete;               // +0x1c
    float LastPercentageComplete;                  // +0x20
};
class CAnimateThing : public CAnimSet {
public:
    void GetAllCombinationAnims(std::vector<CAnimSet*>& sets);   // 0x0070D100
};
class CTCAnimationComplex : public CTCBase {
public:
    static ETCInterfaceType __fastcall StatGetInterfaceType() { return TCI_ANIMATION; }
    CAnimateThing& GetAnimation();                 // 0x0070B460
};
namespace NAnimationEvents {
class CEvent {
public:
    unsigned long ID;
    float Time;
};
template <class T, long N> class CSmallVector {
public:
    T* Data;
    long Size;
    CSmallVector() : Data(0), Size(0) { }
    ~CSmallVector() { delete [] Data; Data = 0; Size = 0; }
    T* begin() { return Data; }
    T* end() { return Data + Size; }
};
class CManager {
public:
    char _pad[8];
    CSymbolMap Symbols;                            // +0x08
    bool GetAnimEventsBetweenTimes(unsigned long anim, CSmallVector<CEvent, 32>& events, float from, float to) const;   // 0x006F99B0
};
CManager& __fastcall GetSoundEventManager();       // 0x006F90C0
}
class CThingSurfaceView {                          // descriptive view of two CThing virtuals (slots 38 / 40, base bodies return 0)
public:
    virtual void v00();
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void v09();
    virtual void v10();
    virtual void v11();
    virtual void v12();
    virtual void v13();
    virtual void v14();
    virtual void v15();
    virtual void v16();
    virtual void v17();
    virtual void v18();
    virtual void v19();
    virtual void v20();
    virtual void v21();
    virtual void v22();
    virtual void v23();
    virtual void v24();
    virtual void v25();
    virtual void v26();
    virtual void v27();
    virtual void v28();
    virtual void v29();
    virtual void v30();
    virtual void v31();
    virtual void v32();
    virtual void v33();
    virtual void v34();
    virtual void v35();
    virtual void v36();
    virtual void v37();
    virtual CThingSurfaceView* GetSurfaceThing();     // slot 38 (+0x98)
    virtual void s39();
    virtual long GetSurfaceMaterial();             // slot 40 (+0xa0)
};
class CSoundEventHandlerView {                     // descriptive view of the native-only CThingSoundEmitter +0x98 object
public:
    virtual void h00();
    virtual void h01();
    virtual void h02();
    virtual void h03();
    virtual void h04();
    virtual void h05();
    virtual void h06();
    virtual void h07();
    virtual void h08();
    virtual void h09();
    virtual void h10();
    virtual void h11();
    virtual void HandleEvent(const CCharString& name);   // slot 12 (+0x30)
};

void CTCSoundPlayer::PlayAllSoundEvents()
{
    if (!g_bSoundEnabled || !g_bPlayAnimationSoundEvents)
        return;
    if (!GFWorld3DDistanceSmallerThan(CMainGameComponent::Get()->PeekWorld().GetGameLODCentre(), ParentThing->GetPos(), AnimCriteriaClipDistance))
        return;
    CTCAnimationComplex* anim_tc;
    if (!ParentThing->GetTC(&anim_tc))
        return;
    CCharString sound_identifier;
    CDefPointer<const CEntitySoundDef> sound_def;
    if (ParentThing->PeekBaseDef().GetSubDef(&sound_def))
        sound_identifier = sound_def->SoundIdentifier;
    if (sound_identifier != "") {
        CASoundBank& bank = ParentThing->GetSampleBank();
        CCharString material_name;
        long material = 0;
        CThingSurfaceView* surface = reinterpret_cast<CThingSurfaceView*>(ParentThing)->GetSurfaceThing();
        if (surface) {
            material = surface->GetSurfaceMaterial();
            LastMaterialIndex = material;
        } else if (ParentThing->GetPhysicsTC()->IsOnGroundSurface()) {
            CDefPointer<const CEngineThemeDef> theme;
            long strength;
            if (GetDefinitionManager().GetDef(PeekWorldMap().GetStrongestEngineThemeAt(ParentThing->GetPos(), strength), &theme)) {
                material = theme->MaterialDef;
                LastMaterialIndex = material;
            }
        } else
            material = LastMaterialIndex;
        if (material > 0) {
            CDefPointer<const CMaterialDef> material_def;
            ParentThing->PeekDefinitionManager().GetDef(material, &material_def);
            material_name = material_def->GetInstantiationName().GetString();
        }
        CAnimateThing& animator = anim_tc->GetAnimation();
        NAnimationEvents::CManager& manager = NAnimationEvents::GetSoundEventManager();
        std::vector<CAnimSet*> anim_sets;
        animator.GetAllCombinationAnims(anim_sets);
        anim_sets.push_back(&animator);
        for (std::vector<CAnimSet*>::iterator it = anim_sets.begin(); it != anim_sets.end(); ++it) {
            float best_alpha = 0.0f;
            long best_anim = 0;
            const short* anims = (*it)->Anims.begin();
            long count = (*it)->Anims.size();
            for (long i = 0; i < count; ++i) {
                float alpha = (*it)->InternalAlphas[i].Alpha;
                if (alpha > best_alpha) {
                    best_anim = anims[i];
                    best_alpha = alpha;
                }
            }
            if (best_anim != 0 && (*it)->CurrentPercentageComplete > (*it)->LastPercentageComplete) {
                NAnimationEvents::CSmallVector<NAnimationEvents::CEvent, 32> events;
                if (manager.GetAnimEventsBetweenTimes(best_anim, events, (*it)->LastPercentageComplete, (*it)->CurrentPercentageComplete)) {
                    for (NAnimationEvents::CEvent* event = events.begin(); event != events.end(); ++event) {
                        const CCharString& event_name = manager.Symbols.FindFirstSymbolWithVal(event->ID & 0x3fffffff)->EntryName;
                        if (event->ID & 0x80000000) {
                            if (PAttachedEmitter.GetPItem()) {
                                CSoundEventHandlerView* handler = *reinterpret_cast<CSoundEventHandlerView**>(reinterpret_cast<char*>(PAttachedEmitter.GetPItem()) + 0x98);
                                if (handler)
                                    handler->HandleEvent(event_name);
                            }
                        } else {
                            CCharString criteria;
                            criteria += sound_identifier;
                            criteria += ";";
                            criteria += event_name;
                            CCharString voice(VoiceName);
                            CCharString extra(ExtraTag);
                            bool played = false;
                            if (voice != "") {
                                if (extra != "") {
                                    CCharString name = criteria + ";" + voice + ";" + extra;
                                    if (InternalPlaySoundEvent(bank, name, false, false, false, -1) != g_InvalidSoundHandle)
                                        played = true;
                                } else {
                                    CCharString name = criteria + ";" + voice;
                                    if (InternalPlaySoundEvent(bank, name, false, false, false, -1) != g_InvalidSoundHandle)
                                        played = true;
                                }
                            }
                            if (!played) {
                                CCharString name = criteria + ";" + material_name + ";" + extra;
                                if (InternalPlaySoundEvent(bank, name, false, false, false, -1) == g_InvalidSoundHandle) {
                                    CCharString name2 = criteria + ";" + material_name;
                                    if (InternalPlaySoundEvent(bank, name2, false, false, false, -1) == g_InvalidSoundHandle)
                                        if (InternalPlaySoundEvent(bank, criteria, false, false, false, -1) == g_InvalidSoundHandle)
                                            InternalPlaySoundEvent(bank, event_name, false, false, false, -1);
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
