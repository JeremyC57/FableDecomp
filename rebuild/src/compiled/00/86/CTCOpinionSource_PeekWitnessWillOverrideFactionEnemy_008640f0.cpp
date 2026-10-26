// CTCOpinionSource::PeekWitnessWillOverrideFactionEnemy(CThingAICreature*) const -- WitnessesWillOverrideFactionEnemy set and the source applies to that witness
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008640F0, length 34 bytes (catalogued under the propagated label
// "PeekWitnessWillNotAttack" of its twin @ 0x008640C0; this one tests the next flag, +0x1a).
// Leaf: PeekWitnessWillOverrideFactionEnemy
// Callees: CTCOpinionSource::AppliesToWitness(CThingAICreature*) const @ 0x00771540 (bool).
// Retail CTCOpinionSource offsets: DefStack (12-byte retail vector) +0x0c, UseNavLineOfSight +0x18,
// WitnessesWillNotAttack +0x19, WitnessesWillOverrideFactionEnemy +0x1a.
// Shape: `mov al ; test al,al` on the flag byte, the bool call tested with `test al,al`, `mov eax,1` / `xor eax,eax`
// returns: a long result from `if (a && b) return 1; return 0;`.
class CThingAICreature;

struct CTCBase_008640f0 {
    virtual void v00();
    unsigned char _pad04[8];
};

class CTCOpinionSource : public CTCBase_008640f0 { public:
    unsigned char DefStack[0x0c];                   // +0x0c std::vector<long>
    bool          UseNavLineOfSight;                // +0x18
    bool          WitnessesWillNotAttack;           // +0x19
    bool          WitnessesWillOverrideFactionEnemy;   // +0x1a
    bool AppliesToWitness(CThingAICreature* witness) const;   // 0x00771540
    long PeekWitnessWillOverrideFactionEnemy(CThingAICreature* witness) const;
};

long CTCOpinionSource::PeekWitnessWillOverrideFactionEnemy(CThingAICreature* witness) const
{
    if (WitnessesWillOverrideFactionEnemy && AppliesToWitness(witness))
        return 1;
    return 0;
}