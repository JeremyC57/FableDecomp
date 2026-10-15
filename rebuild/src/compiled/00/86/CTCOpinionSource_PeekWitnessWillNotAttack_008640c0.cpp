// CTCOpinionSource::PeekWitnessWillNotAttack(CThingAICreature*) const -- WitnessesWillNotAttack set and the source applies to that witness
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008640C0, length 34 bytes.
// Callees: CTCOpinionSource::AppliesToWitness(CThingAICreature*) const @ 0x00771540 (bool).
// Retail CTCOpinionSource offsets: DefStack (12-byte retail vector) +0x0c, UseNavLineOfSight +0x18,
// WitnessesWillNotAttack +0x19, WitnessesWillOverrideFactionEnemy +0x1a.
// Shape: `mov al ; test al,al` on the flag byte, the bool call tested with `test al,al`, `mov eax,1` / `xor eax,eax`
// returns: a long result from `if (a && b) return 1; return 0;`.
struct CThingAICreature;

struct CTCBase_008640c0 {
    virtual void v00();
    unsigned char _pad04[8];
};

struct CTCOpinionSource : CTCBase_008640c0 {
    unsigned char DefStack[0x0c];                   // +0x0c std::vector<long>
    bool          UseNavLineOfSight;                // +0x18
    bool          WitnessesWillNotAttack;           // +0x19
    bool          WitnessesWillOverrideFactionEnemy;   // +0x1a
    bool AppliesToWitness(CThingAICreature* witness) const;   // 0x00771540
    long PeekWitnessWillNotAttack(CThingAICreature* witness) const;
};

long CTCOpinionSource::PeekWitnessWillNotAttack(CThingAICreature* witness) const
{
    if (WitnessesWillNotAttack && AppliesToWitness(witness))
        return 1;
    return 0;
}