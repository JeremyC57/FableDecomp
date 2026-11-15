// Behaviour test for RunCutsceneMacro (0x00CBFB7D): the same control flow as the source over a small model of the cutscene
// def (SetupCond / Macro / SkipCond line lists), the actor map, the flag and substitution maps and a recording script
// interface. Covered: the def lookup, the follower fade, the line parser (actor prefix only when the first space follows the
// dot, quoted parameters, the wrap of a 13th parameter), the `$` substitution (replaced / empty value kept / "NULL" when
// missing), the verbs .SummonerAttack, GiveHeroExpression, GiveHeroMorality, SetFlag, WaitFlag, TeleportToHSP (teleport, then
// one cutscene frame: retail 0x00CCA417 leaves through CameraRotateThing's frame tail), the skip path (SkipCond takes
// over, screen faded in at the end), the setup pass (SetupCond first, then the macro; an empty SetupCond runs nothing) and
// the clean-up of the actor map. Eleven mutants, each with one decision of the function removed, must all change the record.
#include <cstdio>
#include <cstdlib>
#include <map>
#include <string>
#include <vector>

typedef std::vector<std::string> Lines;

static int g_fail = 0;
static void check(bool ok, const char* what) { if (!ok) { std::printf("FAIL: %s\n", what); ++g_fail; } }

static int g_mutant = 0;            // 0 = the function as retail has it; 1..11 remove one decision each

struct CutsceneDef { Lines SetupCond, Macro, SkipCond; };
struct Actor { bool Valid; std::string Thing; };
typedef std::map<std::string, Actor> ActorMap;

// The recording stand-in for CGameScriptInterface (pDSTGame) and the globals the function reads.
struct Game {
    std::map<std::string, CutsceneDef> Defs;
    Lines Followers;
    Lines Record;
    long Frames;
    long SkipAtFrame;               // IsCutsceneSkipped() from this frame on (-1 = never)
    bool HandLampLit;
    Game() : Frames(0), SkipAtFrame(-1), HandLampLit(false) {}
    void Call(const std::string& text) { Record.push_back(text); }
    void NewScriptFrame() { ++Frames; }
    bool IsCutsceneSkipped() const { return SkipAtFrame != -1 && Frames >= SkipAtFrame; }
};

static bool EqualsNoCase(const std::string& a, const std::string& b)
{
    if (a.size() != b.size())
        return false;
    for (size_t i = 0; i < a.size(); ++i) {
        char x = a[i], y = b[i];
        if (x >= 'A' && x <= 'Z') x = (char)(x - 'A' + 'a');
        if (y >= 'A' && y <= 'Z') y = (char)(y - 'A' + 'a');
        if (x != y)
            return false;
    }
    return true;
}
static bool StartsWith(const std::string& text, const char* prefix) { return text.compare(0, std::string(prefix).size(), prefix) == 0; }
static long FindCharPos(const std::string& text, char c) { size_t p = text.find(c); return p == std::string::npos ? -1 : (long)p; }
static bool IsTrue(const std::string& text) { return EqualsNoCase(text, "true"); }       // 0x00CBEDBA
static bool IsFalse(const std::string& text) { return EqualsNoCase(text, "false"); }     // 0x00CBEE0C
static std::string Num(long v) { char b[32]; std::sprintf(b, "%ld", v); return b; }

static void CutsceneFrame(Game& game) { game.NewScriptFrame(); }

static void RunCutsceneMacro(Game& game, const std::string& name, ActorMap& actors, std::map<std::string, bool>* flags,
                             std::map<std::string, std::string>* substitutions, bool setup, bool skippable)
{
    Lines fadedFollowers;
    std::map<std::string, bool> localFlags;
    bool flagWasSet = false;
    std::map<std::string, CutsceneDef>::iterator def = game.Defs.find(name);
    if (def != game.Defs.end()) {
        game.Call(std::string("SetCutsceneMode ") + (skippable ? "1" : "0"));
        std::string params[12];
        bool skipping = false;
        bool fadeInAtEnd = true;
        bool relightHandLamp = false;
        Lines commands;
        if (setup == true) {
            commands = def->second.SetupCond;
        } else {
            commands = def->second.Macro;
            if (EqualsNoCase(commands[0], "FadeOut 0.5,0"))
                game.Call("FadeScreenOutUntilNextCallToFadeScreenIn");
            game.Call("Preload");
        }
        if (actors.find("HeroFollower0") == actors.end() || g_mutant == 10) {
            for (size_t i = 0; i < game.Followers.size(); ++i) {
                fadedFollowers.push_back(game.Followers[i]);
                game.Call("EntityFadeOut " + game.Followers[i]);
            }
        }
        if (game.HandLampLit) {
            game.Call("SetHeroHandLampAsLit 0");
            relightHandLamp = true;
        }

        for (long line = 0; line < (long)commands.size(); ++line) {
            if (game.IsCutsceneSkipped() && !skipping && g_mutant != 5) {
                game.NewScriptFrame();
                line = 0;
                commands.resize(0);
                if (def->second.SkipCond.size() > 0)
                    commands = def->second.SkipCond;
                skipping = true;
            }
            if (commands.size() > 0) {
                std::string command;
                std::string actorName;
                long dot = FindCharPos(commands[line], '.');
                long space = FindCharPos(commands[line], ' ');
                if (dot != -1 && (space > dot || g_mutant == 6)) {
                    command = commands[line].substr(dot);
                    for (long c = 0; c < dot; ++c) {
                        if (commands[line][c] != ' ')
                            actorName += commands[line][c];
                    }
                } else {
                    if (space == -1)
                        space = (long)commands[line].size();
                    command = commands[line].substr(0, space);
                }
                for (int k = 0; k < 10; ++k)
                    params[k] = "";
                if (space != -1) {
                    long c = space + 1;
                    long param = 0;
                    bool quoted = false;
                    for (; c < (long)commands[line].size(); ++c) {
                        if (commands[line][c] == '\'') {
                            quoted = !quoted;
                        } else if (commands[line][c] == ',' && (!quoted || g_mutant == 3)) {
                            if (++param > 11)
                                param = (g_mutant == 7) ? 11 : 0;
                        } else if (commands[line][c] != ' ' || quoted == true) {
                            params[param] += commands[line][c];
                        }
                    }
                }
                if (substitutions) {
                    for (int k = 0; k < 11; ++k) {
                        if (params[k].size() != 0) {
                            if (substitutions && StartsWith(params[k], "$")) {
                                std::map<std::string, std::string>::iterator it;
                                it = substitutions->find(params[k]);
                                if (it != substitutions->end()) {
                                    if (it->second.size() != 0 || g_mutant == 2)
                                        params[k] = it->second;
                                } else if (g_mutant != 1) {
                                    params[k] = "NULL";
                                }
                            }
                        }
                    }
                }
                Actor* actor = 0;
                ActorMap::iterator found;
                found = actors.find(actorName);
                if (found != actors.end() && found->second.Valid)
                    actor = &found->second;

                if (StartsWith(command, ".SummonerAttack")) {
                    if (actor && params[0].size()) {
                        std::string target = EqualsNoCase("HERO", params[0]) ? std::string("Hero") : "Named:" + params[0];
                        game.Call("SummonerAttack " + actor->Thing + "->" + target);
                    }
                }
                if (StartsWith(command, "GiveHeroExpression")) {
                    if (params[0].size()) {
                        std::string expression(params[0]);
                        std::map<std::string, bool>::iterator it;
                        it = localFlags.find(expression);
                        if (it == localFlags.end() && g_mutant != 9) {
                            long level = -1;
                            bool withMessage = false;
                            if (IsTrue(params[1]))
                                withMessage = true;
                            if (params[2].size())
                                level = std::atol(params[2].c_str());
                            game.Call("GiveHeroExpression [" + expression + "] " + Num(level) + (withMessage ? " 1" : " 0"));
                        }
                    }
                } else if (StartsWith(command, "GiveHeroMorality")) {
                    if (params[0].size())
                        game.Call("GiveHeroMorality " + Num((long)(std::atof(params[0].c_str()) * 10.0)));
                } else if (StartsWith(command, "TeleportToHSP")) {
                    if (params[0].size()) {
                        game.Call("EntityTeleportToThing Hero->" + std::string(EqualsNoCase(params[0], "HERO") ? "Hero" : params[0]));
                        if (g_mutant != 11)
                            CutsceneFrame(game);            // 0x00CCA417 -> 0x00CCA733 -> 0x00CC907D
                    }
                } else if (StartsWith(command, "SetFlag")) {
                    if (params[0].size() && params[1].size() && flags) {
                        if (!IsTrue(params[2]) || !flagWasSet || g_mutant == 4) {
                            if (IsFalse(params[1])) {
                                flagWasSet = true;
                                (*flags)[params[0]] = false;
                            } else {
                                flagWasSet = true;
                                (*flags)[params[0]] = true;
                            }
                        }
                        CutsceneFrame(game);
                    }
                } else if (StartsWith(command, "WaitFlag")) {
                    if (params[0].size() && params[1].size() && flags) {
                        bool value = false;
                        if (IsTrue(params[1]))
                            value = true;
                        while ((*flags)[params[0]] != value) {
                            if (game.IsCutsceneSkipped())
                                break;
                            CutsceneFrame(game);
                            if (game.Frames > 1000)             // the model's guard against a mutant that never ends
                                break;
                        }
                    }
                }

                if (setup == true && (commands.size() == 0 || line == (long)commands.size() - 1)) {
                    commands.resize(0);
                    commands = def->second.Macro;
                    game.Call("Preload");
                    line = -1;
                    setup = false;
                }
            }
        }
        game.Call("SetAsInCutscene 0");
        if ((skipping || g_mutant == 8) && fadeInAtEnd)
            game.Call("FadeScreenIn");
        if (actors.size() != 0) {
            for (ActorMap::iterator it = actors.begin(); it != actors.end(); ++it) {
                if (it->second.Valid == true)
                    game.Call("ClearCommands " + it->second.Thing);
            }
            actors.clear();
        }
        for (size_t i = 0; i < fadedFollowers.size(); ++i)
            game.Call("EntityFadeIn " + fadedFollowers[i]);
        if (relightHandLamp)
            game.Call("SetHeroHandLampAsLit 1");
        game.Call("EnableSounds 1");
    }
}

struct Outcome {
    Lines Record;
    std::map<std::string, bool> Flags;
    long Frames;
    bool ActorsCleared;
    bool operator==(const Outcome& o) const { return Record == o.Record && Flags == o.Flags && Frames == o.Frames && ActorsCleared == o.ActorsCleared; }
};

static CutsceneDef MainDef()
{
    CutsceneDef def;
    def.Macro.push_back("Wizard.SummonerAttack hero");                                  // actor verb, case-blind HERO
    def.Macro.push_back("Nobody.SummonerAttack HERO");                                  // unknown actor: nothing
    def.Macro.push_back("Ghost.SummonerAttack HERO");                                   // invalid actor: nothing
    def.Macro.push_back("GiveHeroExpression $Expr, TRUE ,3");                           // substituted, spaces dropped
    def.Macro.push_back("GiveHeroExpression $Missing");                                 // no such argument: "NULL"
    def.Macro.push_back("GiveHeroExpression $Empty");                                   // empty value: the parameter stays
    def.Macro.push_back("GiveHeroExpression 'Big Laugh, loud',true");                   // quoted comma and space
    def.Macro.push_back("GiveHeroExpression A,b,c,d,e,f,g,h,i,j,k,l,Z");                // the 13th parameter wraps onto the first
    def.Macro.push_back("GiveHeroMorality 2.5");                                        // a dot after the first space: no actor
    def.Macro.push_back("SetFlag Door,false,true");                                     // first "once" SetFlag applies
    def.Macro.push_back("SetFlag Door,true,true");                                      // a later "once" SetFlag does not
    def.Macro.push_back("SetFlag Gate,true");
    def.Macro.push_back("WaitFlag Gate,true");                                          // already set: no frame
    def.Macro.push_back("WaitFlag Door,true");                                          // waits until the cutscene is skipped
    def.Macro.push_back("GiveHeroMorality 9");                                          // never reached: the skip lines take over
    def.SkipCond.push_back("SetFlag Skipped,true");
    def.SkipCond.push_back("GiveHeroMorality -1.5");
    return def;
}

static Outcome RunMain(bool withFollowerActor)
{
    Game game;
    game.Defs["CS_TEST"] = MainDef();
    game.Followers.push_back("Dog");
    game.HandLampLit = true;
    game.SkipAtFrame = 6;
    ActorMap actors;
    Actor wizard = { true, "WizardThing" }, ghost = { false, "GhostThing" };
    actors["Wizard"] = wizard;
    actors["Ghost"] = ghost;
    if (withFollowerActor)
        actors["HeroFollower0"] = wizard;
    std::map<std::string, bool> flags;
    std::map<std::string, std::string> substitutions;
    substitutions["$Expr"] = "Flirt";
    substitutions["$Empty"] = "";
    RunCutsceneMacro(game, "CS_TEST", actors, &flags, &substitutions, false, true);
    Outcome o;
    o.Record = game.Record; o.Flags = flags; o.Frames = game.Frames; o.ActorsCleared = actors.empty();
    return o;
}

// A macro that is not skipped; the flag it waits for is already set.
static Outcome RunPlain()
{
    Game game;
    CutsceneDef def;
    def.Macro.push_back("FadeOut 0.5,0");
    def.Macro.push_back("WaitFlag Door,TRUE");
    def.Macro.push_back("TeleportToHSP");                                               // no marker name: nothing, no frame
    def.Macro.push_back("TeleportToHSP HSP_Guild");                                     // teleport, then one frame
    def.Macro.push_back("GiveHeroMorality 9");
    game.Defs["CS_PLAIN"] = def;
    ActorMap actors;
    std::map<std::string, bool> flags;
    flags["Door"] = true;
    RunCutsceneMacro(game, "CS_PLAIN", actors, &flags, 0, false, false);
    Outcome o;
    o.Record = game.Record; o.Flags = flags; o.Frames = game.Frames; o.ActorsCleared = actors.empty();
    return o;
}

static bool Has(const Lines& record, const char* text)
{
    for (size_t i = 0; i < record.size(); ++i)
        if (record[i] == text)
            return true;
    return false;
}

int main()
{
    g_mutant = 0;
    Outcome base = RunMain(false);
    {
        static const char* const expected[] = {
            "SetCutsceneMode 1", "Preload", "EntityFadeOut Dog", "SetHeroHandLampAsLit 0",
            "SummonerAttack WizardThing->Hero",
            "GiveHeroExpression [Flirt] 3 1",
            "GiveHeroExpression [NULL] -1 0",
            "GiveHeroExpression [$Empty] -1 0",
            "GiveHeroExpression [Big Laugh, loud] -1 1",
            "GiveHeroExpression [AZ] 0 0",                 // level = the third parameter "c" read as a number
            "GiveHeroMorality 25",
            "GiveHeroMorality -15",
            "SetAsInCutscene 0", "FadeScreenIn", "ClearCommands WizardThing", "EntityFadeIn Dog", "SetHeroHandLampAsLit 1",
            "EnableSounds 1" };
        Lines want;
        for (size_t n = 0; n < sizeof(expected) / sizeof(expected[0]); ++n)
            want.push_back(expected[n]);
        check(base.Record == want, "record of the main macro");
        if (base.Record != want)
            for (size_t i = 0; i < base.Record.size(); ++i) std::printf("  %s\n", base.Record[i].c_str());
        check(base.Flags.size() == 3 && base.Flags["Door"] == false && base.Flags["Gate"] == true && base.Flags["Skipped"] == true,
              "flags: Door stays false (once), Gate set, Skipped set by the skip lines");
        // three SetFlag frames, three WaitFlag frames up to the skip frame, the skip's own frame, the skip line's SetFlag frame
        check(base.Frames == 8, "script frames");
        check(base.ActorsCleared, "actor map cleared at the end");
        check(!Has(base.Record, "GiveHeroMorality 90"), "line after the skipped wait is not run");
    }
    {   // an actor called HeroFollower0 keeps the followers visible
        Outcome o = RunMain(true);
        check(!Has(o.Record, "EntityFadeOut Dog") && !Has(o.Record, "EntityFadeIn Dog"), "followers not faded when they act");
        check(Has(o.Record, "SummonerAttack WizardThing->Hero"), "macro still runs");
    }
    {   // no skip: no fade in at the end, the last line runs, the opening fade is asked for
        Outcome o = RunPlain();
        check(o.Record.size() == 7 && o.Record[0] == "SetCutsceneMode 0" && o.Record[1] == "FadeScreenOutUntilNextCallToFadeScreenIn" &&
              o.Record[2] == "Preload" && o.Record[3] == "EntityTeleportToThing Hero->HSP_Guild" && o.Record[4] == "GiveHeroMorality 90" &&
              o.Record[5] == "SetAsInCutscene 0" && o.Record[6] == "EnableSounds 1", "plain macro: opening fade, no skip, no fade in");
        check(o.Frames == 1, "no frame when the flag already matches; one frame after TeleportToHSP");
    }
    {   // no substitution map: `$` parameters are left alone; no flag map: SetFlag does nothing, not even a frame
        Game game;
        CutsceneDef def;
        def.Macro.push_back("GiveHeroExpression $Expr");
        def.Macro.push_back("SetFlag Door,true");
        game.Defs["CS_NOMAPS"] = def;
        ActorMap actors;
        RunCutsceneMacro(game, "CS_NOMAPS", actors, 0, 0, false, false);
        check(Has(game.Record, "GiveHeroExpression [$Expr] -1 0"), "no substitution without the map");
        check(game.Frames == 0, "SetFlag without a flag map");
    }
    {   // setup pass: SetupCond first (no opening-fade test, no preload), then the macro
        Game game;
        CutsceneDef def;
        def.SetupCond.push_back("SetFlag InSetup,true");
        def.SetupCond.push_back("GiveHeroMorality 1");
        def.Macro.push_back("GiveHeroMorality 2");
        game.Defs["CS_SETUP"] = def;
        ActorMap actors;
        std::map<std::string, bool> flags;
        RunCutsceneMacro(game, "CS_SETUP", actors, &flags, 0, true, false);
        check(game.Record.size() == 6 && game.Record[1] == "GiveHeroMorality 10" && game.Record[2] == "Preload" &&
              game.Record[3] == "GiveHeroMorality 20", "setup lines, then preload and the macro");
        check(flags["InSetup"] == true && game.Frames == 1, "setup SetFlag");
    }
    {   // setup pass with an empty SetupCond: the line loop never starts, so the macro is not run either
        Game game;
        CutsceneDef def;
        def.Macro.push_back("GiveHeroMorality 2");
        game.Defs["CS_EMPTYSETUP"] = def;
        ActorMap actors;
        RunCutsceneMacro(game, "CS_EMPTYSETUP", actors, 0, 0, true, false);
        check(game.Record.size() == 3 && !Has(game.Record, "GiveHeroMorality 20"), "empty setup runs nothing");
    }
    {   // unknown def: nothing at all, the actor map is kept
        Game game;
        ActorMap actors;
        Actor wizard = { true, "WizardThing" };
        actors["Wizard"] = wizard;
        RunCutsceneMacro(game, "CS_UNKNOWN", actors, 0, 0, false, false);
        check(game.Record.empty() && actors.size() == 1, "unknown def");
    }
    // Every mutant (one decision removed) must change what the main macro does.
    for (int m = 1; m <= 11; ++m) {
        Outcome reference = RunMain(m == 10), plain = RunPlain();
        g_mutant = m;
        Outcome o = RunMain(m == 10), p = RunPlain();
        g_mutant = 0;
        if (o == reference && p == plain) {
            std::printf("FAIL: mutant %d not detected\n", m);
            ++g_fail;
        }
    }
    if (g_fail == 0) std::printf("00cbfb7d_TEST PASS\n");
    return g_fail != 0;
}