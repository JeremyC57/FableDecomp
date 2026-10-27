// NScript::CV_TourGuideScript::OnPersist(CPersistContext& context) -- saves/loads the script members TourGuideKilled, WaypointCounter, TourFinished, QuestActivated by name (default false / 0)
// Leaf: ?OnPersist@CV_TourGuideScript@NScript@@UAEXAAVCPersistContext@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00ee5720, 131 bytes, thiscall, `ret 4`.
// Identity (proven): slot 4 (OnPersist) of the script vtable 0x012F46B8 (no vtable store found; the four literals are Ego_r.pdb CV_TourGuideScript members); the four literals name Ego_r.pdb CV_TourGuideScript members (TourGuideKilled, WaypointCounter, TourFinished, QuestActivated); the PDB name sets hold ?OnPersist@CV_TourGuideScript@NScript@@UAEXAAVCPersistContext@@@Z.
// Callees: CPersistContext::Transfer<bool>(const char*, bool&, const bool&) 0x004045C0, CPersistContext::Transfer<long>(const char*, long&, const long&) 0x00410BE0 (FableWin.pdb template names, bound).
// Shape (family L141 = the landed entity OnPersist 0x00E641B0 with four members): each default is a temporary in a dead
//   stack slot; the member offsets are retail's (the script base is retail 0x40 bytes).
class CPersistContext {
public:
    template <class T> void Transfer(const char* name, T& value, const T& defaultValue);
};
class CScriptBase {                                     // retail 0x40 bytes
public:
    virtual ~CScriptBase();
protected:
    virtual void RegisterMain();
    virtual void Main();
    virtual void Init();
public:
    virtual void OnPersist(CPersistContext& context);   // slot 4
    unsigned char _body[0x3c];                          // +0x04 .. +0x40
};
namespace NScript {
class CV_TourGuideScript : public CScriptBase {
public:
    virtual void OnPersist(CPersistContext& context);
    void* m_pInterface;                                 // +0x40
    unsigned char _pad44[0x120 - 0x44];
    bool           TourGuideKilled;                         // +0x120
    unsigned char _pad121[0x123 - 0x121];
    bool           TourFinished;                            // +0x123
    bool           QuestActivated;                          // +0x124
    unsigned char _pad125[0x164 - 0x125];
    long           WaypointCounter;                         // +0x164
};
void CV_TourGuideScript::OnPersist(CPersistContext& context)
{
    context.Transfer("TourGuideKilled", TourGuideKilled, false);
    context.Transfer("WaypointCounter", WaypointCounter, 0L);
    context.Transfer("TourFinished", TourFinished, false);
    context.Transfer("QuestActivated", QuestActivated, false);
}
}