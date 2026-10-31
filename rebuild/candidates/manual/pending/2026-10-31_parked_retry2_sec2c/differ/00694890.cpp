// CCreatureActionBase::UpdateActionAnimation() -- HEAD DRAFT (first blocks of 1152 bytes): return without an anim entry;
// Grade: DIFFER(186v1140) 358 diff lines (skeleton 1v1140/395): head draft (anim-entry guard, TC 0x5A animation, SetAsFinished, slot-69 resource wait); anim start, ftol timing and the cut-scene event walk not drafted (retry wave 2 sec2c)
// fetch the creature's CTCAnimationComplex (TC 0x5A) animation, finish it when the entry is not a combination anim and the
// action is not finished, then (slot 69) wait for the graphic appearance's resources. The anim-start / timing (ftol) /
// cut-scene event walk (0x0069496A..0x00694CF5) is not drafted; retail's `and esp,-8` frame (a double local) is not modelled.
// Leaf: UpdateActionAnimation
// Retail: Fable.exe (TLC, VC7.1) @ 0x00694890, 1152 bytes.
class CTCBase;
struct CVectorMapTC { struct value_type { long first; CTCBase* second; }; value_type* _M_start; value_type* _M_finish;
    value_type* LowerBound(const long& key); };                         // 0x0040F020
class CTCGraphicAppearance { public: bool IsResourcesLoaded(bool); };   // 0x004BE3E0
class CThing { public: unsigned char _pad00[0x20]; unsigned long UsingComponents[9]; CVectorMapTC Components;
    unsigned char _pad4c[0x64 - 0x4c]; CTCGraphicAppearance* PTCGraphicAppearance; };
class CThingCreatureBase : public CThing { public: bool IsAnimCombinationAnim(long anim); };   // 0x00662820
class CAnimateThing { public: void SetAsFinished(); };                  // 0x0070BE80
class CTCAnimationComplex { public: CAnimateThing& GetAnimation(); };   // 0x0070B460
class CBaseObject;
class CBaseIntelligentPointer { public: virtual ~CBaseIntelligentPointer(); void* Info; CBaseObject* GetPItem() const; };
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); } };
struct CAnimEntry { long AnimIndex; };
extern long g_FramesPerSecond;                                          // 0x01375550
class CCreatureActionBase {
public:
    virtual void v00();
    unsigned char _pad04[4];
    CIntelligentPointer<CThingCreatureBase> PCreature;                  // +0x08
    unsigned char _pad10[0x60 - 0x10];
    bool ActionFinished;                                                // +0x60
    unsigned char _pad61[0x74 - 0x61];
    CAnimEntry* PAnimEntry;                                             // +0x74 (counted pointer data)
    virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23(); virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29(); virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35(); virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39(); virtual void v40(); virtual void v41(); virtual void v42(); virtual void v43(); virtual void v44(); virtual void v45(); virtual void v46(); virtual void v47(); virtual void v48(); virtual void v49(); virtual void v50(); virtual void v51(); virtual void v52(); virtual void v53(); virtual void v54(); virtual void v55(); virtual void v56(); virtual void v57(); virtual void v58(); virtual void v59(); virtual void v60(); virtual void v61(); virtual void v62(); virtual void v63(); virtual void v64(); virtual void v65(); virtual void v66(); virtual void v67(); virtual void v68();
    virtual bool WaitForResources();                                    // slot 69 (+0x114)
    void UpdateActionAnimation();
};
void CCreatureActionBase::UpdateActionAnimation()
{
    if (!PAnimEntry || !PAnimEntry->AnimIndex)
        return;
    long fps = g_FramesPerSecond;
    CThingCreatureBase* creature = PCreature.GetPItem();
    CTCAnimationComplex* animTC;
    if (creature->UsingComponents[2] & 0x4000000) {
        long key = 0x5a;
        CVectorMapTC::value_type* it = creature->Components.LowerBound(key);
        if (it == creature->Components._M_finish || key < it->first)
            it = creature->Components._M_finish;
        animTC = (CTCAnimationComplex*)it->second;
    }
    CAnimateThing& animation = animTC->GetAnimation();
    CAnimEntry* entry = PAnimEntry;
    bool combination = PCreature.GetPItem()->IsAnimCombinationAnim(entry->AnimIndex);
    if (!combination) {
        if (ActionFinished)
            return;
        animation.SetAsFinished();
    }
    if (ActionFinished)
        return;
    if (WaitForResources() && !PCreature.GetPItem()->PTCGraphicAppearance->IsResourcesLoaded(true))
        return;
    (void)fps;
}
