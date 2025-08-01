# 🛰️ GhostWatch – APT-Level DFIR Tool

**GhostWatch** is a surgical detection tool designed to uncover high-level stealth techniques such as section ghosting, remote thread injections, and virtual memory tampering — often missed by traditional scanners.

Made to operate like an operator.

---

## ⚙️ Features

- 🔍 Live detection of:
  - Suspicious memory mappings (e.g. RWX, erased headers)
  - Section ghosting & Doppelganging indicators
  - Remote thread injection (CreateRemoteThread)
  - Syscall-based memory tampering (NtWriteVirtualMemory)
- 🧠 ETW-based syscall tracing
- 💾 No install – portable, no footprint
- 🦾 Operates under pressure — handles large environments in real-time

---

## 🧪 Usage

Run as Administrator to monitor all processes:

GhostWatch.exe

For triage, it can be executed from USB or live forensics environments with no setup.

---

## 🧷 Output Example

[!] Suspicious mapping: PID 2084 [brave.exe]
[*] ALERT: Section creation detected (PID 2436)
[+] Event: CreateRemoteThread in PID 3421
[+] NtWriteVirtualMemory → PID 1324


Clean. Readable. Operator-ready.

---

## 🔬 Authors

Developed by `<starls/>`

Released under **Larking Labs (2025)**  
Part of the **Advanced BlueOps Arsenal**

---

> GhostWatch doesn't scan — it hunts.  
> No GUI. No noise. Just truth.


