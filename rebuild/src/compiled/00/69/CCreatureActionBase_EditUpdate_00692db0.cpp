// CCreatureActionBase::EditUpdate(float) -- per-frame update: remember the last running time; a finished action clears the creature's current action; otherwise accumulate the frame time step, run the movement update once the start frame is reached, finish (virtual slot 4) when the total time has elapsed, else recompute the frames running from the elapsed seconds, clamped to the total
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00692DB0, length 149 bytes (+ int3 padding). `ret 4`: the float argument is unused.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PCreature at +8), CThingCreatureBase::ClearCurrentAction @ 0x006633A0
// (releases the counted pointer at +0x100 of the creature; the manifest "_Dest_val<...CFaction>" label is propagated), this action's
// virtual slots 62 (+0xf8, the frame time step: the inlined UpdateActionRunningTime, landed 0x00692480), 6 (+0x18, the start
// delay in frames) and 4 (+0x10, FinishAction), CCreatureActionBase::UpdateActionMovement @ 0x00692AA0 (direct), the frame rate
// global @ 0x01375550 (`fild`), __ftol2 @ 0x00BFEA70 (the `(long)` cast).
// Shape: `fild g ; fmul [RunningTimeSeconds]` is `RunningTimeSeconds * g` (the right operand is loaded first, GOTCHAS); TotalTime
// is read once into edi and kept across the __ftol2 helper; the recomputed FramesRunning is stored, then clamped to TotalTime
// through the register copy (`cmp eax,edi ; jle`).
// Layout: engine/CCreatureActionBase.h (PDB) agrees on every member used (TotalTime +0x10, AnimDelayTime +0x18, FramesRunning
// +0x1c, RunningTimeSeconds +0x40, LastRunningTimeSeconds +0x44, StartFrame +0x48, Finished +0x61); a local model carries the virtual slots.
class CThing { public:
    void ClearCurrentAction();              // 0x006633A0
};
class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer {                       // vtable 0x0129C95C (Ego_r: vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
private:
    CBaseObjectPointer* PData;                           // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};
extern long g_framesPerSecond;              // 0x01375550

class CCreatureActionBase { public:                // retail sizeof 0xa8
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void FinishAction();            // slot 4 (+0x10)
    virtual void v05();
    virtual long GetStartDelayFrames();     // slot 6 (+0x18)
    virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35();
    virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39(); virtual void v40(); virtual void v41();
    virtual void v42(); virtual void v43(); virtual void v44(); virtual void v45(); virtual void v46(); virtual void v47();
    virtual void v48(); virtual void v49(); virtual void v50(); virtual void v51(); virtual void v52(); virtual void v53();
    virtual void v54(); virtual void v55(); virtual void v56(); virtual void v57(); virtual void v58(); virtual void v59();
    virtual void v60(); virtual void v61();
    virtual float GetFrameTimeStep();       // slot 62 (+0xf8)
    unsigned char _pad04[4];                // +0x04
    CIntelligentPointer<CThing> PCreature;      // +0x08 CIntelligentPointer<CThingCreatureBase>
    long  TotalTime;                        // +0x10
    long  ActionTime;                       // +0x14
    long  AnimDelayTime;                    // +0x18
    long  FramesRunning;                    // +0x1c
    unsigned char _pad20[0x40 - 0x20];      // +0x20
    float RunningTimeSeconds;               // +0x40
    float LastRunningTimeSeconds;           // +0x44
    long  StartFrame;                       // +0x48
    unsigned char _pad4c[0x61 - 0x4c];      // +0x4c
    bool  Finished;                         // +0x61
    unsigned char _pad62[0xa8 - 0x62];      // +0x62
    void UpdateActionRunningTime() { RunningTimeSeconds += GetFrameTimeStep(); }   // 0x00692480, inlined here
    void UpdateActionMovement();            // 0x00692AA0
    virtual void EditUpdate(float timeStep);
};

void CCreatureActionBase::EditUpdate(float timeStep)
{
    LastRunningTimeSeconds = RunningTimeSeconds;
    if (Finished) {
        PCreature.GetPItem()->ClearCurrentAction();
        return;
    }
    UpdateActionRunningTime();
    if (FramesRunning >= GetStartDelayFrames() + StartFrame) {
        if (!Finished)
            UpdateActionMovement();
    }
    if (TotalTime >= 0 && FramesRunning >= TotalTime) {
        if (!Finished) {
            FinishAction();
            return;
        }
    }
    FramesRunning = (long)(RunningTimeSeconds * g_framesPerSecond);
    if (TotalTime >= 0 && FramesRunning > TotalTime)
        FramesRunning = TotalTime;
}