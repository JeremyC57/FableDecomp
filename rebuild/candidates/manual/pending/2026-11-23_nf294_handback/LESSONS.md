# NF294 lessons (2026-11-23)

- **The old flags.sh / srcof.sh crossed catalog blocks.** They ran `grep -A14` after `Address = '<addr>'`, so a row with no CompilerFlags
  picked up the next row's flags. 0x00997800 graded DIFFER(207v122) at HEAD with 0x00CAF500's `/EHsc`. Read only the row's own block (awk
  up to the closing brace). Treat old DIFFER-at-HEAD controls from these helpers with suspicion.
- **Defining a broad retail branch only to reach one nested declaration can change `.drectve`.** FABLE_RETAIL_CAFILE pulls engine/CArray.h
  and adds `/DEFAULTLIB:"libcp"`. .text stays equal, so only objsec's `.drectve` check sees it. A guarded add-only opt-in outside the
  branch is the clean fix.
- **"class/ns" entries are mostly ports.** In entries 1-60, 5 were in-place, 2 were stale table lines and 49 were ports. The in-place shapes:
  - an existing header opt-in the row already includes (CBankFile ENTRYQUERY);
  - a real-name local class that takes the PDB member, with access (`private:` / `protected:` on the same line);
  - a nested stand-in struct whose only symbol is the PDB one (a rename: CBRIAR_ROSE_IMPOSTER);
  - pointer -> reference on a ctor called from a derived ctor's init list.
- **Before editing a binder, check whether it is a witness in a sibling agent's ENTRIES, not only a target.** 0x0094FEB0's line 110 holds
  both my IsStopping and NF292's Terminate.
- **An xsites caller missing from the table is not automatically a third spelling.** Read its source to see which listed spelling it binds
  (0x00717C50 binds the same global spelling as 0x00717AA0). Then a `replace` of the respelled witness's line is safe.
