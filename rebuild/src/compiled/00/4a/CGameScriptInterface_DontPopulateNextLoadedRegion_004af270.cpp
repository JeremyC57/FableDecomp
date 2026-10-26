struct RegionMgr { char pad[0xda]; unsigned char dontPopulateNext; };
class CGameScriptInterface { public: char pad[0x7c]; RegionMgr* mgr; };
void __fastcall DontPopulateNextLoadedRegion(CGameScriptInterface* self)
{
    self->mgr->dontPopulateNext = 1;
}