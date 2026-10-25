// CPersistContext::Transfer(const char* name, bool& value) -- two-argument bool overload: transfers the value with a default of false (forwards to the three-argument Transfer<bool>)
// Leaf: ??$Transfer@_N@CPersistContext@@QAEXPBDAA_N@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004107C0, 30 bytes, thiscall `ret 8`.
// The default is built in the `push ecx` slot (`mov byte [esp+3], 0`) and passed by address as the const bool& default.
// Callee: CPersistContext::Transfer<bool>(const char*, bool&, const bool&) 0x004045C0 (FSE manifest: the bool Transfer API).
// Label: landed as CEngineSceneGrid::GetCellBoundingBox (that PDB member takes three arguments and would `ret 0xC`); the
//   Ego_r/FableWin name sets hold ??$Transfer@_N@CPersistContext@@QAEXPBDAA_N@Z and its ..AB_N@Z three-argument partner.
class CPersistContext {
public:
    template <class T> void Transfer(const char* name, T& value);
    template <class T> void Transfer(const char* name, T& value, const T& defaultValue);
protected:
    template <class T> static T GetDefaultVal() { return T(); }
};

template <class T> void CPersistContext::Transfer(const char* name, T& value)
{
    Transfer(name, value, GetDefaultVal<T>());
}

template void CPersistContext::Transfer<bool>(const char* name, bool& value);
