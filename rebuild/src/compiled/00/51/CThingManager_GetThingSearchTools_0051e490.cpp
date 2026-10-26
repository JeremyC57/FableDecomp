// CThingManager::GetThingSearchTools() -- the world's thing search tools
// Leaf: ?GetThingSearchTools@CThingManager@@QAEAAVCThingSearchTools@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0051E490, 7 bytes: mov eax,[ecx+24h] (World) ; mov eax,[eax+20h] (PThingSearchTools) ; ret.
// Identity (sweep3; was _global::GetMember_dword0x24_deref0x20_0051e490, a propagated "CPixelShader::CountReferences" stand-in): Ego_r.pdb / FableWin.pdb
//   `?GetThingSearchTools@CThingManager@@QAEAAVCThingSearchTools@@XZ`; Ego_r and FableWin list PeekThingSearchTools then GetThingSearchTools (Ego_r 0x00528280 / 0x00528290),
//   retail 0x0051E480 / 0x0051E490 are the byte-identical pair in that order, and the CTCBase run agrees: retail
//   CTCBase::GetThingSearchTools 0x00686D80 jumps to 0x0051E490 and CTCBase::PeekThingSearchTools 0x00686D90 to 0x0051E480
//   with ecx = the thing manager (0x013B8A1C). Fields: Ego_r CThingManager World +0x30 (PC +0x24), CWorld
//   PThingSearchTools (scoped_ptr<CThingSearchTools>) +0x20.
class CThingSearchTools;

class CWorld {                                 // only the field read here
public:
    unsigned char      _pad00[0x20];
    CThingSearchTools* PThingSearchTools;      // +0x20 (scoped_ptr)
};

class CThingManager {
public:
    unsigned char _pad00[0x24];
    CWorld*       World;                       // +0x24
    CThingSearchTools& GetThingSearchTools();
};

CThingSearchTools& CThingManager::GetThingSearchTools()
{
    return *World->PThingSearchTools;
}
