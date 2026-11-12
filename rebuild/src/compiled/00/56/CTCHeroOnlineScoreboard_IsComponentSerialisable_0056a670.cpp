// CTCHeroOnlineScoreboard::IsComponentSerialisable() const -- returns true
// Leaf: ?IsComponentSerialisable@CTCHeroOnlineScoreboard@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0056A670, thiscall, `ret`.
// Identity: slot 23 of CTCHeroOnlineScoreboard's vtable 0x0124DB34 (defined here: the slot holds this body in 1 vtable(s), all of CTCHeroOnlineScoreboard or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 16 TLC-named slots of that vtable puts IsComponentSerialisable(void) at this slot; Ego_r/FableWin name set: ?IsComponentSerialisable@CTCHeroOnlineScoreboard@@UBE_NXZ; `ret` matches the signature.
class CTCHeroOnlineScoreboard {
public:
    virtual bool IsComponentSerialisable() const;
};
bool CTCHeroOnlineScoreboard::IsComponentSerialisable() const
{
    return true;
}
