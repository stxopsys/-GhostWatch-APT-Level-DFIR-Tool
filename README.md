# 🛰️ GhostWatch – APT-Level DFIR Tool

**GhostWatch** is a surgical detection tool engineered to uncover stealthy thread hijacking, remote memory injections, and syscall-level tampering — the kind of actions most traditional tools never even notice.

🚨 Warning: GhostWatch is **extremely sensitive** — it actively monitors low-level events like remote thread execution and direct memory manipulation via syscalls.  
It does **not** detect process Doppelgänging — focus is strictly on thread hijack, memory maps, and runtime telemetry.

---

## ⚙️ Features

- 🔍 Live detection of:
  - Suspicious memory mappings (e.g. RWX, erased PE headers)
  - Remote thread injections (e.g. `CreateRemoteThread`)
  - Direct memory writes (`NtWriteVirtualMemory`)
- 🧠 Syscall tracing via ETW (Event Tracing for Windows)
- 💾 Portable, no installation required
- 🧪 Ideal for live forensics, IR triage, or APT hunting
- 🛠️ Hardened – reliable under load, even in noisy environments

---

## 🧪 Usage

Run as Administrator to capture full telemetry:


GhostWatch.exe

For triage, it supports plug-and-run execution from external drives or live forensic OS environments. ```

📤 Output Example

[!] Suspicious mapping: PID 2084 [brave.exe]
[*] ALERT: Section creation detected (PID 2436)
[+] Event: CreateRemoteThread in PID 3421
[+] NtWriteVirtualMemory → PID 1324

Operator-grade output. Clean, readable, exportable. 

---

### 🔬 Authorship

- **Developed by:** `<starls/>`  
- **Released under:** Larking Labs project (2025)  
- **Division:** Advanced BlueOps Initiative

Released under the Larking Labs project (2025)

Part of the Advanced BlueOps Initiative


