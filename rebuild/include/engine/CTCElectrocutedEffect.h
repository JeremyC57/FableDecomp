#ifndef FABLE_ENGINE_CTCELECTROCUTEDEFFECT_H
#define FABLE_ENGINE_CTCELECTROCUTEDEFFECT_H
#ifdef FABLE_RETAIL_ELECTROCUTED_ORIGINAL_PASS23
// Original complete Ego_r and FableWin types; native members0c..28, RTTI0126f044.
#include "engine/CTCBase.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CDefPointer.h"
#include "stlport/fable_containers.h"
class CSpecialAbilitiesElectrocutedEffectDef;
class CTCElectrocutedEffect:public CTCBase {
public:
 static ETCInterfaceType FABLE_FASTCALL StatGetInterfaceType(){return TCI_ELECTROCUTED_EFFECT;}
 virtual ETCInterfaceType GetInterfaceType()const;
 static CTCBase* FABLE_FASTCALL Alloc(CThing&);
 static CCharString FABLE_FASTCALL StatGetClassTypeName();
 virtual CCharString GetClassTypeName()const;
 CTCElectrocutedEffect(const CTCElectrocutedEffect&);
 CTCElectrocutedEffect(CThing&);
 virtual ~CTCElectrocutedEffect();
 virtual bool HasFrameUpdate()const;
 virtual void FrameUpdate();
 virtual void OnKill();
 virtual void OnRemove();
 virtual void OnInitialActivate();
 void Reset();
private:
 void FinalCleanup();
 CDefPointer<const CSpecialAbilitiesElectrocutedEffectDef> PDef;
 long CurTime,FadeInTime,FadeOutTimeStart,FadeOutTime;
 CIntelligentPointer<CThing> PEffect;
 std::vector<CIntelligentPointer<CThing> > TintList;
public:
 CTCElectrocutedEffect& operator=(const CTCElectrocutedEffect&);
};
FABLE_STATIC_ASSERT(sizeof(CTCElectrocutedEffect)==0x34);

#else
// New shared header (w22 agent2042, row 0x007F2DC0 CTCLightningNode::FrameUpdate): the electrocuted-effect component. Ego_r.pdb / FableWin.pdb
// ?StatGetInterfaceType@CTCElectrocutedEffect@@SI?AW4ETCInterfaceType@@XZ (TCI_ELECTROCUTED_EFFECT = 138: FrameUpdate's inline GetTC reads
// bitset word +0x30 bit 10 and looks up key 0x8A, 0x007F348D..0x007F34BD) and ?Reset@CTCElectrocutedEffect@@QAEXXZ (out of line at 0x007CE7A0,
// the bound spelling). Members not modelled (declarations only). Needs FABLE_RETAIL_CTCBASE.
#include "engine/CTCBase.h"
#include "engine/ETCInterfaceType.h"
class CTCElectrocutedEffect : public CTCBase {
public:
    static ETCInterfaceType __fastcall StatGetInterfaceType() { return TCI_ELECTROCUTED_EFFECT; }
    void Reset();                                                // 0x007CE7A0
};
#endif // FABLE_RETAIL_ELECTROCUTED_ORIGINAL_PASS23
#endif
