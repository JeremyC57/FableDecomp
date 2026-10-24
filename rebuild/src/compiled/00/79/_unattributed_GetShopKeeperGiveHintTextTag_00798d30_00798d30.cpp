// _unattributed::GetShopKeeperGiveHintTextTag_00798d30() -- returns the text tag "TEXT_AI_SHOP_KEEPER_GIVE_HINT" as a CCharString
// Leaf: ?GetShopKeeperGiveHintTextTag_00798d30@_unattributed@@
// Renamed (fistp pass, 2026-10-24) from NTCScriptedControl::CActionDoCreatureAction::GetActionName @ 00/79/CActionDoCreatureAction_GetActionName_00798d30.cpp: not in any vtable; sole caller 0x0093009E lies in an unlabelled function; the literal is the only fact, so the name is descriptive
// (was: _unattributed::GetShopKeeperGiveHintTextTag_00798d30  @ 00798d30)
// __fastcall CCharString GetShopKeeperGiveHintTextTag_00798d30(_unattributed* this)
// Body: return CCharString((const char*)0x0126C12C, -1);
// The this-pointer is unused; a CCharString is constructed in the hidden
// return buffer from a literal string address and length -1.

class CCharString {
public:
    // Ctor at 0x0099EBF3, __fastcall(this, const char* s, int len)
    CCharString(const char* s, int len);
private:
    char* m_p;
};

class _unattributed {
public:
    CCharString GetShopKeeperGiveHintTextTag_00798d30();
};

CCharString _unattributed::GetShopKeeperGiveHintTextTag_00798d30()
{
    return CCharString((const char*)0x0126C12C, -1);
}
