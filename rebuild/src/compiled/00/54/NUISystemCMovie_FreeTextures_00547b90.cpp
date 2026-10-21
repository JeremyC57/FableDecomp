// NUISystem::CMovie::FreeTextures() -- hand the movie's frame texture back to the engine's resource pools and forget it
// Retail: Fable.exe @ 0x00547B90, 35 bytes; Ego_r.pdb ?FreeTextures@CMovie@NUISystem@@IAEXXZ. Touches only class
// statics: the texture at 0x013B8AC0 and the engine at 0x013B8AB8 (stored by the CMovie(CIEngine&, long) ctor). When the
// texture is set it calls CIEngine slot 70 (+0x118) ?FreeTextureFromEngineResourcePools@CIEngine@@UAEXPAVCTexture@@@Z
// (vtable 0x012A0D24) and clears the static. Caller 0x00547EED (movie stop path) passes ecx = the movie.
// Layout: static member names are behavioural (Ego_r.pdb gives no data symbols for them); CIEngine is modelled with
// placeholder slots so FreeTextureFromEngineResourcePools lands on slot 70 (engine/CIEngine.h is data-only).
class CTexture;
class CIEngine {                                         // vtable 0x012A0D24
public:
    virtual ~CIEngine();                                 // slot 0
    virtual void _slot01();
    virtual void _slot02();
    virtual void _slot03();
    virtual void _slot04();
    virtual void _slot05();
    virtual void _slot06();
    virtual void _slot07();
    virtual void _slot08();
    virtual void _slot09();
    virtual void _slot10();
    virtual void _slot11();
    virtual void _slot12();
    virtual void _slot13();
    virtual void _slot14();
    virtual void _slot15();
    virtual void _slot16();
    virtual void _slot17();
    virtual void _slot18();
    virtual void _slot19();
    virtual void _slot20();
    virtual void _slot21();
    virtual void _slot22();
    virtual void _slot23();
    virtual void _slot24();
    virtual void _slot25();
    virtual void _slot26();
    virtual void _slot27();
    virtual void _slot28();
    virtual void _slot29();
    virtual void _slot30();
    virtual void _slot31();
    virtual void _slot32();
    virtual void _slot33();
    virtual void _slot34();
    virtual void _slot35();
    virtual void _slot36();
    virtual void _slot37();
    virtual void _slot38();
    virtual void _slot39();
    virtual void _slot40();
    virtual void _slot41();
    virtual void _slot42();
    virtual void _slot43();
    virtual void _slot44();
    virtual void _slot45();
    virtual void _slot46();
    virtual void _slot47();
    virtual void _slot48();
    virtual void _slot49();
    virtual void _slot50();
    virtual void _slot51();
    virtual void _slot52();
    virtual void _slot53();
    virtual void _slot54();
    virtual void _slot55();
    virtual void _slot56();
    virtual void _slot57();
    virtual void _slot58();
    virtual void _slot59();
    virtual void _slot60();
    virtual void _slot61();
    virtual void _slot62();
    virtual void _slot63();
    virtual void _slot64();
    virtual void _slot65();
    virtual void _slot66();
    virtual void _slot67();
    virtual void _slot68();
    virtual void _slot69();
    virtual void FreeTextureFromEngineResourcePools(CTexture* texture);  // slot 70
};
namespace NUISystem {
class CMovie {
public:
    static CIEngine* PEngine;                            // 0x013B8AB8
    static CTexture* MovieTexture;                       // 0x013B8AC0
protected:
    void FreeTextures();
};
void CMovie::FreeTextures()
{
    if (MovieTexture) {
        PEngine->FreeTextureFromEngineResourcePools(MovieTexture);
        MovieTexture = 0;
    }
}
}