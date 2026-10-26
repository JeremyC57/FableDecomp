struct CLevelObj { char pad[0x41]; unsigned char flags; };
class CThing { public: char pad[0x70]; CLevelObj* level; };

bool __fastcall CThing_IsLevelLess(const CThing* thing)
{
    const CLevelObj* lvl = thing->level;
    bool r = false;
    if (lvl != 0 && (lvl->flags & 0x8) != 0)
        r = true;
    return r;
}