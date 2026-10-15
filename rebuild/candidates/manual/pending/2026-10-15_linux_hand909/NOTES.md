# Chunk 909 (skipped-row ledger: the five functions of catalogue row 008d6620)

- 008d6620 NCreatureMode::CModeOutOfActionBase::FrameUpdate (vtable 0x0126743C slot 3, shared by CModeDazed 0x01267CC4 and CModeStunned
  0x01267C74): qualified `CModeBase::FrameUpdate()` (0x0073AC50), `if (m_frames > 0) --m_frames;`, then `if (m_canRecover && m_frames == 0) Recover();`
  as a tail jump through slot 15 (+0x3c), which is __purecall in the base. Slot 14 is pure too; slot 13 has a base body at 0x0073DA20.
- 008d6650/008d6670 (slot 13 of Dazed/Stunned): `mov ecx,[ecx+4] ; jmp 0x00662020` = `return ParentThing->GetOutOfActionRecoveryFrames();`
  (0x00662020 reads def +0xfc through the creature's +0xe0; its label "Getter_FieldE0_FC" is junk).
- 008d6660/008d6680 (slot 14): `call ; ret` = a float result through a named local (`float seconds = ...; return seconds;`), GOTCHAS 24k.
  0x006620A0 (def +0x7c/+0x80) is the dazed time, 0x00662040 (def +0x70/+0x74) the stunned time; the manifest labels both GetDazedTimeSeconds.
- Model: CModeBase {vptr, ParentThing +0x04 (CThingCreatureBase*), 0x60 bytes of state}; the out-of-action base is `#pragma pack(1)` with
  m_frames +0x68, m_canRecover +0x6c, the attacker intelligent pointer at +0x6d (landed 0x00740160 model).
- The chunk 148 oracle still carried the 112-byte multi-row for 008d6620 and was removed before grading.
