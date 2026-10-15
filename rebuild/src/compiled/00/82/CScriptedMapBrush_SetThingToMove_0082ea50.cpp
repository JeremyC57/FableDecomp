// CScriptedMapBrush::SetThingToMove(unsigned __int64 thingID, const C3DVector& position, const C3DAngles& facingAngles) -- record where a thing should be moved to: ThingsToMove[id] gets the position, then the facing angles
// Retail: Fable.exe (TLC, VC7.1) @ 0x0082EA50, 75 bytes (`?SetThingToMove@CScriptedMapBrush@@QAEX_KABVC3DVector@@ABVC3DAngles@@@Z`, Ego_r.pdb).
// Callees: std::map<unsigned __int64, CThingPosAndOrientation>::operator[] @ 0x0082FB90 (key by const
// reference: `lea eax,[esp+8] ; push eax` is the address of the by-value id argument), called twice.
//
// Shape: `ThingsToMove[id].Position = position; ThingsToMove[id].FacingAngles = facingAngles;` -- two
// lookups (the map member's address `lea esi,[ecx+20h]` is kept in esi), each 12-byte POD copied word by
// word through edx/ecx (implicit assignment of three floats), the second through `add eax,0Ch`.
// Retail layout: ThingsToMove sits at +0x20 (the engine header's PDB layout puts it at +0x28: the PDB
// vector Cells before it is 16 bytes, the retail one 12), so the members are modelled locally with the
// header's names. CThingPosAndOrientation (PDB, 24 bytes): C3DVector Position +0, C3DAngles FacingAngles +0xC.

struct C3DVector {
    float X, Y, Z;
};

struct C3DAngles {
    float X, Y, Z;
};

struct CThingPosAndOrientation {
    C3DVector Position;                      // +0x00
    C3DAngles FacingAngles;                  // +0x0C
};

struct CThingsToMoveMap {                    // std::map<unsigned __int64, CThingPosAndOrientation> (retail 12 bytes)
    void* _Myhead;
    void* _Reserved;
    unsigned long _Mysize;
    CThingPosAndOrientation& operator[](const unsigned __int64& key);   // 0x0082FB90
};

struct CScriptedMapBrush {
    void* __vftable;                         // +0x00
    unsigned char _pad_0x04[0x1c];           // +0x04 .. Cells, ThingsToDelete
    CThingsToMoveMap ThingsToMove;           // +0x20
    void SetThingToMove(unsigned __int64 thingID, const C3DVector& position, const C3DAngles& facingAngles);
};

void CScriptedMapBrush::SetThingToMove(unsigned __int64 thingID, const C3DVector& position, const C3DAngles& facingAngles)
{
    ThingsToMove[thingID].Position = position;
    ThingsToMove[thingID].FacingAngles = facingAngles;
}