// NCreatureMode::CModeStrafe::Alloc(CThing&) -- mode factory: new CModeStrafe(thing, false)
// Leaf: ?Alloc@CModeStrafe@NCreatureMode@@SIPAVCModeBase@CTCCreatureModeManager@@AAVCThing@@@Z
// Retail: Fable.exe @ 0x007431C0, 33 bytes, static __fastcall (ecx = thing).
// Identity (proven): Ego_r.pdb `?Alloc@CModeStrafe@NCreatureMode@@SIPAVCModeBase@CTCCreatureModeManager@@AAVCThing@@@Z`;
// it allocates 0x70 bytes (Ego_r sizeof(CModeStrafe) = 0x70) and runs the landed CModeStrafe::CModeStrafe(CThing&, bool)
// 0x00741FE0 with false; its only reference is the mode-factory table word at 0x0073B9AC, next to the other
// NCreatureMode::CMode*::Alloc factories (landed 0x00742550 CModeAttracted::Alloc of the same shape).
// Callees: operator new 0x00BFEA1A, CModeStrafe::CModeStrafe 0x00741FE0.
class CThing;
class CTCCreatureModeManager { public:
    class CModeBase { public:
        virtual ~CModeBase();
    };
};
namespace NCreatureMode {
class CModeStrafe : public CTCCreatureModeManager::CModeBase { public:
    unsigned char _body[0x70 - 4];                      // sizeof(CModeStrafe) = 0x70 (Ego_r)
    CModeStrafe(CThing& thing, bool unknown);           // 0x00741FE0
    static CTCCreatureModeManager::CModeBase* __fastcall Alloc(CThing& thing);
};
CTCCreatureModeManager::CModeBase* __fastcall CModeStrafe::Alloc(CThing& thing)
{
    return new CModeStrafe(thing, false);
}
}