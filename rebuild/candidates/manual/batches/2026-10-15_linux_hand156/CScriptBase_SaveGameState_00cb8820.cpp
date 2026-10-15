// CScriptBase::SaveGameState(CCharString& out) -- save every active entity script, then append the script's own data and the persisted entity scripts to the game-state text between START_/END_ markers
// Leaf: SaveGameState
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CB8820, length 135 bytes (+9 int3 padding).
// PDB: ?SaveGameState@CScriptBase@@QAEXAAVCCharString@@@Z; EntityScripts is a CVectorMap<unsigned __int64,
// CCountedPointer<CActiveEntityScriptBase> > (16-byte pairs; retail element vector begin +0x08, end +0x0C).
// Callees: CScriptBase::SaveEntityScript @ 0x00CB83D0, CCharString::operator+=(const char*) @ 0x0099F100 (x4:
// "START_SCRIPT_DATA\r\n" 0x012C1720, "END_SCRIPT_DATA\r\n\r\n" 0x012C170C, "START_ENTITY_SCRIPTS\r\n" 0x012C16F4,
// "END_ENTITY_SCRIPTS\r\n" 0x012C16DC), CPersistContext::CPersistContext(CCharString&) @ 0x009BAE10, the script's
// own virtual slot 4 (+0x10, persist its data into the context), CRuntimeScriptedEntityPersist::
// SaveToGameStateScript(CCharString&) @ 0x00CB7A00 on the member at +0x30.
// Shape: the context is the whole 0x2C-byte frame (no destructor call); `out` is cached in esi after the loop.
struct CCharString {
    void* PStringData;
    CCharString& operator+=(const char* text);       // 0x0099F100
};

struct CPersistContext {
    unsigned char _body[0x2c];
    CPersistContext(CCharString& output);            // 0x009BAE10
};

class CActiveEntityScriptBase;

template <class T> struct CCountedPointer {
    T*    Data;
    void* Info;
};

namespace NScript {
struct CRuntimeScriptedEntityPersist {
    unsigned char _body[0x14];
    void SaveToGameStateScript(CCharString& out);    // 0x00CB7A00
};
}

struct CEntityScriptEntry {                          // pair<unsigned __int64, CCountedPointer<CActiveEntityScriptBase> >
    unsigned __int64                          first;
    CCountedPointer<CActiveEntityScriptBase>  second;
};

class CScriptBase {
public:
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0c();
    virtual void PersistScriptData(CPersistContext& context);   // slot 4 (+0x10)
    void SaveGameState(CCharString& out);
    void SaveEntityScript(unsigned __int64 entityID, CActiveEntityScriptBase& script);   // 0x00CB83D0
private:
    void*               Threads;                     // +0x04
    CEntityScriptEntry* EntityScripts_start;         // +0x08
    CEntityScriptEntry* EntityScripts_finish;        // +0x0C
    unsigned char       _pad10[0x30 - 0x10];
    NScript::CRuntimeScriptedEntityPersist RuntimeScriptedEntityPersist;   // +0x30 (retail)
};

void CScriptBase::SaveGameState(CCharString& out)
{
    for (CEntityScriptEntry* it = EntityScripts_start; it != EntityScripts_finish; ++it)
        SaveEntityScript(it->first, *it->second.Data);
    out += "START_SCRIPT_DATA\r\n";
    CPersistContext context(out);
    PersistScriptData(context);
    out += "END_SCRIPT_DATA\r\n\r\n";
    out += "START_ENTITY_SCRIPTS\r\n";
    RuntimeScriptedEntityPersist.SaveToGameStateScript(out);
    out += "END_ENTITY_SCRIPTS\r\n";
}
