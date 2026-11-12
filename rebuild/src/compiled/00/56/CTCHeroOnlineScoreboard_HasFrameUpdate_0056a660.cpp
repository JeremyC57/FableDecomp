// CTCHeroOnlineScoreboard::HasFrameUpdate() const -- returns true
// Leaf: ?HasFrameUpdate@CTCHeroOnlineScoreboard@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0056A660, thiscall, `ret`.
// Identity: slot 8 of CTCHeroOnlineScoreboard's vtable 0x0124DB34 (defined here: the slot holds this body in 1 vtable(s), all of CTCHeroOnlineScoreboard or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 16 TLC-named slots of that vtable puts HasFrameUpdate(void) at this slot; Ego_r/FableWin name set: ?HasFrameUpdate@CTCHeroOnlineScoreboard@@UBE_NXZ; `ret` matches the signature.
class CTCHeroOnlineScoreboard {
public:
    virtual bool HasFrameUpdate() const;
};
bool CTCHeroOnlineScoreboard::HasFrameUpdate() const
{
    return true;
}
