#ifndef FABLE_ENGINE_CGAMEDEFINITIONMANAGER_H
#define FABLE_ENGINE_CGAMEDEFINITIONMANAGER_H
#include "engine/CDefinitionManager.h"
// Original class V and public CDefinitionManager base: both PDB name sets,
// native RTTI. Native ctor0044c6c2 constructs that base and zeroes d0..dc.
// Both donor TPI sets retain only a forward record for this class; therefore
// the extra native storage is deliberately unnamed until its fields are proven.
#ifdef FABLE_RETAIL_CGAMEDEFINITIONMANAGER_HEROSTATSDEF
class CHeroStatsDef;
#endif
#ifdef FABLE_RETAIL_CGAMEDEFINITIONMANAGER_GLOBALDEF
class CGlobalDef;
#endif
class CGameDefinitionManager : public CDefinitionManager {
public:
    static CGameDefinitionManager& __fastcall Get();
    CGameDefinitionManager();
    virtual ~CGameDefinitionManager();
    virtual void InitAndCompile();
#ifndef FABLE_RETAIL_CGAMEDEFINITIONMANAGER
    template<class T> bool GetDef(long,CDefPointer<T>&) const;
#endif // retail: no CGameDefinitionManager::GetDef in Ego_r.pdb / FableWin.pdb; callers reach CDefinitionManager::GetDef(long,CDefPointer<const T>*) (0x008EDFF0)
#ifdef FABLE_RETAIL_CGAMEDEFINITIONMANAGER_HEROSTATSDEF
    // Nested opt-in (chunk 794, row 0x007039A0 CThingObject::OnHit): the hero-stats definition the native ctor zeroes in the d0..dc block,
    // read at +0xDC by CTCHeroStats::ApplyPenaltyForDeath 0x0057DE14 (landed) and by OnHit (`mov edx,[eax+0DCh]` off PeekDefinitionManager).
    // No PDB layout records this class (forward record only), so the member name is descriptive. No layout change.
#ifdef FABLE_RETAIL_CGAMEDEFINITIONMANAGER_GLOBALDEF
    // Nested opt-in (chunk 1023, row 0x006E3EC0 NSpeechGainManager::Initialise): the global definition read at +0xD0 (`mov ecx,
    // [eax+0D0h]` off the world's definition manager, then CGlobalDef::DialogueLayers +0x10C); Ego_r / FableWin
    // ?PeekGlobalDef@CGameDefinitionManager@@QBEABVCGlobalDef@@XZ (inline: no call in retail). Member name descriptive (forward
    // record only; the accessor pair GetPGlobalDef / PeekGlobalDef names it). No layout change.
public:
    const CGlobalDef& PeekGlobalDef()const{return *PGlobalDef;}
    const CGlobalDef* PGlobalDef;                                        // +0xd0 (descriptive name)
private:
    unsigned char _pad_0xd4[8];
#else
private:
    unsigned char _pad_0xd0[12];
#endif
public:
#ifdef FABLE_RETAIL_CGAMEDEFINITIONMANAGER_PEEKHEROSTATSDEF
    // Retail opt-in (Codex pass 17): FableWin01d87170 returns the object at nativeDC.
    const CHeroStatsDef& PeekHeroStatsDef()const{return *PHeroStatsDef;}
#endif
    const CHeroStatsDef* PHeroStatsDef;                                  // +0xdc (descriptive name)
#elif defined(FABLE_RETAIL_CGAMEDEFINITIONMANAGER_GLOBALDEF)
public:
    const CGlobalDef& PeekGlobalDef()const{return *PGlobalDef;}          // see the nested opt-in above
    const CGlobalDef* PGlobalDef;                                        // +0xd0 (descriptive name)
private:
    unsigned char _pad_0xd4[12];
#else
private:
    unsigned char _pad_0xd0[16];
#endif
};
FABLE_STATIC_ASSERT(sizeof(CGameDefinitionManager)==0xe0);
#endif
