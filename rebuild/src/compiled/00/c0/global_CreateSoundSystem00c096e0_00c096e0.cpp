// CreateSoundSystem_00c096e0(CSoundSystemSetup* setup) -- libpk sound-system factory (`__stdcall`, no `this`): resolve the optional context object from the setup's +0x24 / +0x2C words (0x00C1C1A0) and, when there is one, build the 0x84-byte CSoundSystem (vtable 0x012B1E24) from the setup and it; 0 otherwise
// Leaf: ?CreateSoundSystem_00c096e0@@YGPAUCSoundSystem@@PAUCSoundSystemSetup@@@Z
// Flags: /EHsc
// Retail: Fable.exe @ 0x00C096E0, 120 bytes, `ret 4`, EH frame (handler 0x01210C33; state 0 guards the new'd block).
// Naming (naming wave 21, DECISIONS_W20_RESOLVED / chunk 687 NAME_FIXES): was landed as CWorldManager::InitializeSteeringAfterSave over a
//   CSteeringGroup stand-in (propagated; `this` is never used). The landed CSoundManagerLH ctor 0x00A3EF00 pushes its 0x30-byte
//   CSoundSystemSetup and binds exactly this `__stdcall` spelling; the object it returns stores vtable 0x012B1E24 (the CSoundSystem ctor
//   0x00C094A0 / dtor 0x00C09380) and is driven through slot 4 Initialise / slot 11 SetBufferSize there. Descriptive (libpk, no PDB).
// Callees: the __fastcall lookup 0x00C1C1A0 taking the setup's +0x24 and +0x2C words (manifest label "CWorldMap::SaveToFile" is wrong;
//   the callee ignores the second register; its landed spelling LookupSteeringWorldEntry / CSteeringWorldEntry is the earlier landing's,
//   kept here), operator new 0x00BFEA1A (0x84 bytes), CSoundSystem::CSoundSystem(CSoundSystemSetup*, CSteeringWorldEntry*) 0x00C094A0.
// Shape: a missing entry or a failed allocation returns 0; `return new CSoundSystem(...)` keeps the allocation in the EH frame slot for
//   the constructor's unwind.
struct CSteeringWorldEntry;

struct CSoundSystemSetup {                      // 0x30 bytes (the landed 0x00A3EF00 model); only +0x24 / +0x2C are read here
    unsigned char _pad[0x24];
    void*         Long24;                       // +0x24
    unsigned char _pad28[0x4];
    void*         Long2C;                       // +0x2C
};

CSteeringWorldEntry* __fastcall LookupSteeringWorldEntry(void* worldKey, void* entryKey);   // 0x00C1C1A0

struct CSoundSystem {                           // vtable 0x012B1E24, 0x84 bytes
    CSoundSystem(CSoundSystemSetup* setup, CSteeringWorldEntry* entry);   // 0x00C094A0
    unsigned char _body[0x84];
};

CSoundSystem* __stdcall CreateSoundSystem_00c096e0(CSoundSystemSetup* setup);

CSoundSystem* __stdcall CreateSoundSystem_00c096e0(CSoundSystemSetup* setup)
{
    CSteeringWorldEntry* entry = LookupSteeringWorldEntry(setup->Long24, setup->Long2C);
    if (entry)
        return new CSoundSystem(setup, entry);
    return 0;
}
