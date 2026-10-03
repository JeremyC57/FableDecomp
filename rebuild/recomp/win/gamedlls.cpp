// Helper DLLs shipped in the game folder are 32-bit code and cannot be loaded
// into the 64-bit host; the few entry points the game calls are provided here.
#include "host.hpp"

namespace host {
namespace {

// Eula.dll!EBUEula(regKey, a, b, flags): shows the EULA on first run, returns 1 once
// accepted. Owning the Steam copy implies acceptance (Steam shows it at install).
IMPORT("eula.dll", EBUEula) {
    log("EBUEula: EULA treated as accepted");
    retStd(c, 1, 4);
}

}  // namespace
}  // namespace host
