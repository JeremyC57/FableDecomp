// CAnimationEntry::GetRotationInAnimation(float& rotation) const -- the negated rotation of the main mesh bank's animation this entry's BankIndex names
// Leaf: ?GetRotationInAnimation@CAnimationEntry@@QBEXAAM@Z
// Naming wave 18: the global stand-in g_pMainGameComponent (0x013B86A0) is the protected static CMainGameComponent::PMainGameComponent, read through the
//   inline static accessor Get() as retail code outside the class does (name-only: the bytes are unchanged).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005DBFD0, length 36 bytes (+ int3 padding), `ret 4`.
// Callees: CMeshDataBank::PeekAnimation(unsigned long) const @ 0x00A242C0 (landed; FINDINGS chunk 474: not
//   "CTCInventoryAbilities::GetAbilityEntry"). Name sets: ?GetRotationInAnimation@CAnimationEntry@@QBEXAAM@Z (the out
//   value is a float reference) and ?PeekAnimation@CMeshDataBank@@QBEPBVC3DAnimationInfo@@K@Z.
// Globals: the main game component @ 0x013B86A0. Ego_r.pdb: CMainGameComponent::PWorld (scoped_ptr<CWorld>) at +0x24,
//   CWorld::PMainMeshBank (CCountedPointer<CMeshDataBank>, one pointer in retail) at +0x3C, C3DAnimationInfo::Rotation at +0x24.
// Shape: BankIndex (+0x00) is the animation index; the result is loaded, negated (`fchs`) and stored through the
//   reference, which is read from the stack after the call.
class C3DAnimationInfo {
public:
    unsigned char _before_Rotation[0x24];       // CMeshDataBankEntry base, Data, Duration, NonLoopingDuration, MovementVector
    float Rotation;                             // +0x24
};

class CMeshDataBank {
public:
    const C3DAnimationInfo* PeekAnimation(unsigned long index) const;   // 0x00A242C0
};

class CWorld {
public:
    unsigned char _before_PMainMeshBank[0x3c];
    CMeshDataBank* PMainMeshBank;               // +0x3C
};

class CMainGameComponent {
protected:
    static CMainGameComponent* PMainGameComponent;      // 0x013B86A0 ?PMainGameComponent@CMainGameComponent@@1PAV1@A (protected static; ESTABLISHED_SPELLINGS)
public:
    static CMainGameComponent* __fastcall Get() { return PMainGameComponent; }   // Ego_r / FableWin ?Get@CMainGameComponent@@SIPAV1@XZ (inline)
private:
public:
    unsigned char _before_PWorld[0x24];
    CWorld* PWorld;                             // +0x24
};


class CAnimationEntry {
public:
    long BankIndex;                             // +0x00
    void GetRotationInAnimation(float& rotation) const;
};

void CAnimationEntry::GetRotationInAnimation(float& rotation) const
{
    CMeshDataBank* bank = CMainGameComponent::Get()->PWorld->PMainMeshBank;
    rotation = -bank->PeekAnimation(BankIndex)->Rotation;
}
