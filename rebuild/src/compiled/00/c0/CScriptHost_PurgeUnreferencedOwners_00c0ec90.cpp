// CScriptHost::PurgeUnreferencedOwners() -- while the host's purge flag at +0x48 is set, repeatedly find the first owner in the array at +0x2c (count +0x34) whose reference count is zero and destroy it (its scripts, then the owner), until none is left
// Leaf: PurgeUnreferencedOwners
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C0EC90, 78 bytes (`ret`). Descriptive names (no PDB covers this library): the
//   manifest's "UISystem_ProcessInputEventLoop" is propagated. The only caller is the `__stdcall` callback thunk
//   0x00C088C0 (`mov ecx,[esp+4] ; call ; ret 4`). The +0x48 word is the one the landed TryActivate 0x00C0E600 tests.
// Callees: CScriptOwnerThing::IsUnreferenced @ 0x00C1EA20 (`cmp word [ecx+1Eh],0 ; sete al`), CScriptHost::DestroyOwner
//   @ 0x00C0EC10 (also deletes the owner, whose destructor drops it from the array).
// Shape: the inner search is `for (i = 0; !found && i < count; ++i)`: after `inc esi` the found test comes first and
//   jumps straight to the destroy call; the exhausted exit tests found again; both destroy arms jump back to the reset.
//   The outer loop is `for (;;) { ... if (!found) break; DestroyOwner(found); }` with found declared inside: a
//   `do { ... if (found) DestroyOwner(found); } while (found);` merges the two arms, re-tests found and swaps ebx/edi.
struct CScriptOwnerThing {
    bool IsUnreferenced() const;                // 0x00C1EA20
};

struct CScriptHost {
    void* m_vtable;                             // +0x00 (0x012B25A8)
    unsigned char _pad04[0x2c - 0x04];
    CScriptOwnerThing** m_owners;               // +0x2c
    unsigned long m_ownersCapacity;             // +0x30
    unsigned long m_ownerCount;                 // +0x34
    unsigned char _pad38[0x48 - 0x38];
    long m_purge;                               // +0x48
    void DestroyOwner(CScriptOwnerThing* owner);   // 0x00C0EC10
    void PurgeUnreferencedOwners();
};

void CScriptHost::PurgeUnreferencedOwners()
{
    if (m_purge) {
        for (;;) {
            CScriptOwnerThing* found = 0;
            for (unsigned long i = 0; !found && i < m_ownerCount; ++i) {
                CScriptOwnerThing* owner = m_owners[i];
                if (owner->IsUnreferenced())
                    found = owner;
            }
            if (!found)
                break;
            DestroyOwner(found);
        }
    }
}