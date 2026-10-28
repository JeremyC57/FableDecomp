// CGameTimeManager::IsTimeBetween(float time, float start, float end) const -- whether a time of day lies in [start, end), the window wrapping past midnight when start is not before end
// Grade: DIFFER(60v75) -- VC7.1 shares one `time < end` compare between the two windows (jumping into it from the wrap-around path); retail keeps a separate copy per window (jp -> false in the first, jnp -> true in the second). Tried: returned `&&`/`||` (78 bytes, full-register 0/1), if over `&&`/`||`, strict-less guard returns, `>=` guard returns. Next: a bool result local (`bool r = false; ... r = true; return r;`) or an inline helper per window.
// Leaf: ?IsTimeBetween@CGameTimeManager@@ABE_NMMM@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x006BB5B0, 75 bytes, thiscall (this unused), `ret 0Ch`.
// Identity (probable): Ego_r.pdb ?IsTimeBetween@CGameTimeManager@@ABE_NMMM@Z is the private const three-float overload
//   (the public ones take two longs); this body takes three floats (`ret 0Ch`) and is called from the CGameTimeManager
//   code at 0x006BBA30. The catalog label already names it.
// Shape: x87 compares (`test ah,5 ; jp` is `!(start < end)`, `test ah,1` the `time >= start` tests); the two return
//   blocks (`mov al,1` / `xor al,al`: one guard return per compare; a returned `&&`/`||` widens to eax and
//   an if over `&&`/`||` merges the two `time < end` tests) are shared by both windows.
class CGameTimeManager {
    bool IsTimeBetween(float time, float start, float end) const;
public:
    long m_dummy;
};

bool CGameTimeManager::IsTimeBetween(float time, float start, float end) const
{
    if (start < end) {
        if (!(time >= start))
            return false;
        if (time < end)
            return true;
        return false;
    }
    if (time >= start)
        return true;
    if (time < end)
        return true;
    return false;
}
