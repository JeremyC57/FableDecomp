// CMainGameComponent::InitParticleEngine() -- start the particle engine: initialise it, give it the game's environment-interaction object (a new counted object holding the world), and open the "PARTICLE_MAIN" bank of its emitter database
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004174F1, length 119 bytes. Frame-pointer size TU: `#pragma optimize("y", off)` + `("s", on)`.
// Callees: NParticleEngine::Initialise @ 0x00AEBE20 (`__fastcall`), operator new @ 0x00BFEA1A (0x24 bytes; the inline constructor stores
//   vtable 0x0122ED8C and the world at +4), CCountedPointer(T*) @ 0x0041956C, its copy constructor @ 0x00418FBC (the by-value argument
//   built in its slot), NParticleEngine::SetEnvironmentInteraction(CCountedPointer<CAEnvironmentInteraction>) @ 0x00AEAB20 (`__fastcall`,
//   callee-cleaned 8-byte argument), CCharString(const char*, long) @ 0x0099EBF0 ("PARTICLE_MAIN" @ 0x0122DAEC), the emitter-database
//   getter @ 0x00AEAAE0 (returns the global 0x013D2E14; manifest label GFGetPreMainMemoryUsed is a byte-twin),
//   CParticleEmitterDatabase::OpenBank(const CCharString&) @ 0x00AE8980, ~CCharString @ 0x0099EAE0, ~CCountedPointer @ 0x004191C1.
// Layout: CMainGameComponent world pointer at +0x24 (retail).
// Shape: the bank name is a NAMED CCharString local (`lea eax,[ebp-4] ; push eax` after the constructor; a bare temporary pushes the
//   constructor's eax), destroyed before the counted pointer.
#pragma optimize("y", off)
#pragma optimize("s", on)
void* __cdecl operator new(unsigned int size);     // 0x00BFEA1A
struct CCharString {
    void* m_pStringData;
    CCharString(const char* text, long length);    // 0x0099EBF0
    ~CCharString();                                // 0x0099EAE0
};
struct CWorld;
namespace NParticleEngine {
struct CAEnvironmentInteraction {
    virtual void v00();
};
}
struct CGameEnvironmentInteraction : NParticleEngine::CAEnvironmentInteraction {
    CWorld*       m_pWorld;                        // +0x04
    unsigned char _pad08[0x1c];
    CGameEnvironmentInteraction(CWorld* world) : m_pWorld(world) { }
    virtual void v00();
};
struct CCPPointerInfo;
template <class T> struct CCountedPointer {
    T*              m_pObject;
    CCPPointerInfo* m_pInfo;
    CCountedPointer(T* object);                    // 0x0041956C
    CCountedPointer(const CCountedPointer& other); // 0x00418FBC
    ~CCountedPointer();                            // 0x004191C1
};
struct CParticleEmitterDatabase {
    void OpenBank(const CCharString& name);        // 0x00AE8980
};
namespace NParticleEngine {
void __fastcall Initialise();                                                         // 0x00AEBE20
void __fastcall SetEnvironmentInteraction(CCountedPointer<CAEnvironmentInteraction> interaction);   // 0x00AEAB20
CParticleEmitterDatabase* __fastcall GetEmitterDatabase();                            // 0x00AEAAE0
}
struct CMainGameComponent {
    unsigned char _pad00[0x24];
    CWorld*       m_pWorld;                        // +0x24
    void InitParticleEngine();
};
void CMainGameComponent::InitParticleEngine()
{
    NParticleEngine::Initialise();
    CCountedPointer<NParticleEngine::CAEnvironmentInteraction> interaction(new CGameEnvironmentInteraction(m_pWorld));
    NParticleEngine::SetEnvironmentInteraction(interaction);
    CCharString bankName("PARTICLE_MAIN", -1);
    NParticleEngine::GetEmitterDatabase()->OpenBank(bankName);
}