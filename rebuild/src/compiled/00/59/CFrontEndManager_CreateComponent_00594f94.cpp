// NUISystem::CFrontEndManager::CreateComponent @ 0x00594F94
//
// A one-argument __thiscall wrapper around the base manager factory: it calls
// NUISystem::CManager::GetInstance 0x0041E5F2 (not its own getter 0x00595582), then
// CManager::CreateComponent(const CCharString&, true) 0x0041DB1D.  Size optimization preserves the retail push order and
// stack cleanup without an assembly shim.

#pragma optimize("s", on)

class CCharString;

namespace NUISystem
{
class CComponent;

class CManager
{
public:
    static CManager* __fastcall GetInstance();      // 0x0041E5F2
    CComponent* CreateComponent(const CCharString& definition, bool frontEnd);   // 0x0041DB1D
};

class CFrontEndManager : public CManager
{
public:
    CComponent* CreateComponent(const CCharString& definition);
};

CComponent* CFrontEndManager::CreateComponent(const CCharString& definition)
{
    return CManager::GetInstance()->CreateComponent(definition, true);
}
}

#pragma optimize("s", off)
