# 0x012133C0 -- funclet row, for the coordinator
Not an authored function: EH unwind funclet (+ the parent's __ehhandler$ thunk, 18 bytes in all) of CPhysicalPlayer::Play 0x00C271F0
(landed). Payload is the usual include-the-parent stub with funclet_offset 0; vland strict: RELOCATION_MATCH PASS.
Only open point: this oracle row spans the thunk too (other Unwind@ rows stop before it). If the catalog wants the thunk as its own
row, cut the oracle to 8 bytes (8d4da0e918cc9fff) -- the same stub matches at offset 0 either way.
