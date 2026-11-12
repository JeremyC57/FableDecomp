#ifndef FABLE_ENGINE_CTCIDLESCHEDULER_H
#define FABLE_ENGINE_CTCIDLESCHEDULER_H
// New header (chunk F1, row 0x00801920 CTCIdleScheduler::RegisterIdleStateGroup): the CTCIdleScheduler class moved out of
// engine/campfire_types.h unchanged (which now includes this file), so a row can use it without that bundle's prerequisites.
// Ego_r.pdb CTCIdleScheduler: IdleStates +0x0C, IdleStatesWeight +0x18 (retail 12-byte STLport maps), Preferred +0x24, FrameLastChecked +0x28.
#include "engine/CTCBase.h"
#include "engine/ETCInterfaceType.h"
#include "stlport/fable_map.h"
enum EIdleStateGroup;
class CAIStateGroupBase;
class CTCIdleScheduler : public CTCBase {
public: static ETCInterfaceType __fastcall StatGetInterfaceType() { return TCI_IDLE_SCHEDULER; }
public: virtual ETCInterfaceType GetInterfaceType() const;
public: static CTCBase* __fastcall Alloc(CThing&);
public: static CCharString __fastcall StatGetClassTypeName();
public: virtual CCharString GetClassTypeName() const;
public: CTCIdleScheduler(CThing&);
public: void RegisterIdleStateGroup(CAIStateGroupBase*,EIdleStateGroup);
public: void ChooseNewIdle();
public: bool IsPreferredIdle(CAIStateGroupBase*);
public: void Clear();
protected: std::map<enum EIdleStateGroup,CAIStateGroupBase *,std::less<enum EIdleStateGroup>,std::allocator<std::pair<enum EIdleStateGroup const ,CAIStateGroupBase *> > > IdleStates;
protected: std::map<enum EIdleStateGroup,long,std::less<enum EIdleStateGroup>,std::allocator<std::pair<enum EIdleStateGroup const ,long> > > IdleStatesWeight;
protected: EIdleStateGroup Preferred;
protected: long FrameLastChecked;
public: virtual ~CTCIdleScheduler();
public: CTCIdleScheduler& operator=(const CTCIdleScheduler&);
};
#endif
