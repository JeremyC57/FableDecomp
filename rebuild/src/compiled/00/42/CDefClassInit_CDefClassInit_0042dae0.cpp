// CDefClassInit::CDefClassInit(const CCharString&, CDefClassBase* (*)()) -- a definition-class registration record (name + allocator)
// Leaf: ??0CDefClassInit@@QAE@ABVCCharString@@P6APAVCDefClassBase@@XZ@Z
// Retail: Fable.exe @ 0x0042DAE0, 26 bytes, `ret 8`, returns this. Catalog label CEnginePrimitiveOcclusionManager::GetOccludedFrames
// is wrong: Init_RegisterGameComponents (0x004EE23F) builds CCharString("CVillageDef") etc., calls this on a local with the
// def's Alloc function (e.g. 0x004E213B), and hands the result to CDefinitionManager::AddDefClass 0x009B0AC0
// (?AddDefClass@CDefinitionManager@@QAEXABVCDefClassInit@@@Z). Ego_r.pdb CDefClassInit (8 bytes): Name (CCharString) +0,
// AllocFunc +4. CCharString copy constructor 0x0099EC30.
class CCharStringData;
class CCharString {
public:
    CCharStringData* PStringData;
    CCharString(const CCharString& other);               // 0x0099EC30
    ~CCharString();
};
class CDefClassBase;
typedef CDefClassBase* (*DefClassAllocFunc)();
class CDefClassInit {
public:
    CCharString       Name;                              // +0x00
    DefClassAllocFunc AllocFunc;                         // +0x04
    CDefClassInit(const CCharString& name, DefClassAllocFunc allocFunc);
};
CDefClassInit::CDefClassInit(const CCharString& name, DefClassAllocFunc allocFunc)
    : Name(name), AllocFunc(allocFunc)
{
}