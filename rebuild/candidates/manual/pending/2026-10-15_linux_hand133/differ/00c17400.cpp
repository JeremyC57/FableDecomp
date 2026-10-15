// _global::DeleteTrackNodeCommand(CScriptContext* context, CScriptObject* object, CTrackNodeArgs* args, CTrackNodeExtra* extra) -- look up the selected track node's waypoint data; report "no selection" through the object when there is none, else run the delete on it and free the data
// Grade: DIFFER(138v155) -- lane gap: retail loads the second stack argument into esi right before calling 0x00C16CE0 (`push esi ; mov esi,[esp+24h]`) and never uses it again -- the callee reads esi as an input (`mov eax,[esi]` with no setup), an LTCG custom register argument plain C++ cannot express; the null-path/early-return layout and the edi-held waypoint also differ in this first draft. Tried: one draft (parked at once as a lane gap).
// Leaf: DeleteTrackNodeCommand
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C17400, length 155 bytes (+ int3 padding). Flags: /EHsc (the waypoint holder is EH state 0).
// Callees: LookupSelectedWaypoint @ 0x00C17940 (`__fastcall`, context in ecx, object in edx), CScriptObject::GetReporter @ 0x00C30840 (thiscall),
// Report @ 0x00C0DEC0 (`__fastcall`, the reporter in ecx and the message @ 0x012B2D44 in edx), CWaypointHolder::Get @ 0x00C17740 (thiscall on the
// frame holder), DeleteTrackNode @ 0x00C16CE0 (`__fastcall` args, extra, waypoint, object -- it ALSO takes `extra` in esi: an LTCG register
// argument), WaypointData::~WaypointData @ 0x00C2EF30, operator delete @ 0x00BFE9BC.
// Label correction: the manifest's "CEditControlCentre::DeleteSelectedTrackNode" (1 parameter) is a propagated name: the body is a `__fastcall`
// with two register and two stack arguments (`ret 8`) in the 0x00C0..0x00C4 script module; all names are descriptive.
struct CWaypointData {
    ~CWaypointData();                        // 0x00C2EF30
};

struct CWaypointHolder {
    CWaypointData* m_pData;
    CWaypointHolder(CWaypointData* data) : m_pData(data) {}
    ~CWaypointHolder() { delete m_pData; }
    CWaypointData* Get();                    // 0x00C17740
};

struct CReporter;
void __fastcall Report(CReporter* reporter, const char* message);   // 0x00C0DEC0
extern const char g_NoTrackNodeSelected[];   // 0x012B2D44

struct CScriptObject {
    CReporter* GetReporter();                // 0x00C30840
};
struct CScriptContext;
struct CTrackNodeArgs;
struct CTrackNodeExtra;

CWaypointData* __fastcall LookupSelectedWaypoint(CScriptContext* context, CScriptObject* object);   // 0x00C17940
void __fastcall DeleteTrackNode(CTrackNodeArgs* args, CWaypointData* waypoint, CScriptObject* object);   // 0x00C16CE0

void __fastcall DeleteTrackNodeCommand(CScriptContext* context, CScriptObject* object, CTrackNodeArgs* args, CTrackNodeExtra* extra)
{
    CWaypointHolder holder(LookupSelectedWaypoint(context, object));
    if (holder.m_pData == 0) {
        Report(object->GetReporter(), g_NoTrackNodeSelected);
        return;
    }
    DeleteTrackNode(args, holder.Get(), object);
}
